# Phase 1: 코어에서 MFC·Direct2D 분리 (2026-11-08 시작)

- 상위 문서: [로드맵](2026-11-08-roadmap-library-first.md) 5절 Phase 1, 지침 3·18·19. 선행: [Phase 0 작업 로그](2026-11-08-phase0-worklog.md).
- 목표: **코어(모델·파서·카탈로그·포트레이얼 규칙)가 MFC, Direct2D, Win32 없이 컴파일되고 동작한다.** 렌더링은 `DisplayList`/모델을 입력으로 받는 별도 계층이 된다. 이것이 전 플랫폼(지침 18), 빌드 속도(지침 17), opens100-web 역이식의 전제다.
- 규칙: 항목을 끝내면 `[x]`와 커밋 해시. 각 단계는 **빌드와 9개+ 테스트가 통과하는 상태로 커밋**한다. 큰 이동(파일·프로젝트 재배치)은 Phase 4(CMake)로 미루고, 여기서는 의존 방향만 바로잡는다.

## 1. 현재 결합 인벤토리 (커밋 dfbaedfa 기준)

| 프로젝트 | D2D/DWrite 사용 파일 | MFC 타입(헤더) | 비고 |
|---|---|---|---|
| FeatureCatalog | 0 | 0 | 이미 깨끗(`UseOfMfc=false`) |
| PortrayalCatalogue | 3 (`PortrayalCatalogue.h/.cpp`: `CreateSVGD2Geometry`, `CreatePatternImages`, `CreateLineImages`, `ChangePalette(ID2D1Factory1*, IWICImagingFactory*)`; `S100_ColorProfile::GetD2Color`) | 0 | 카탈로그 모델에 렌더 캐시 생성 함수가 붙어 있음 |
| GeoMetryLibrary | 1 (`Geometry.h`의 `virtual CreateD2Geometry(ID2D1Factory1*)`) | `CString` 1, `HDC/CDC` 8 | 기하 기반 클래스가 D2D 훅을 가짐 |
| S100Engine | 13 (`S100Render`, `S100SymbolManager`, `SVGReader`, `SVGGeometry`, `AreaPatternBitmap`, `BackgroundLayer`, `S100PCManager`) | `CString` 13, `CFile` 12 | 사실상 렌더 모듈. 이름만 "Engine" |
| GISLibrary | 55 | `CString` 137(헤더), `CFile` 117, `CMap` 8, `HDC/CDC` 29 | 모델·파서·SENC·렌더가 섞임 |

구체적 결합 지점:
- **모델 생성이 렌더 캐시를 즉시 만든다.** `S101Cell::MakeLineData`(:850)·`MakeAreaData`(:958), `S10XGML` 지오메트리 변환(:1505~1551, 9곳)이 `CreateD2Geometry(D2->Factory())`를 호출. `D2 == nullptr`이면 접근 위반. 이 때문에 테스트가 `Open()` 대신 `Read8211()`만 호출 중.
- `SpatialObject`(모든 데이터셋의 기반)가 `D2D1Resources*`를 멤버로 가지고 생성자 인자로 받는다. `Open(CString)`, `Draw(CDC*/HDC/ID2D1...)` 6종 가상 함수가 한 클래스에 있다.
- `SCurve`/`SSurface`가 `ID2D1PathGeometry* pGeometry`를 멤버로 가진다(모델 객체 안의 렌더 핸들).
- `CatalogManager(D2D1Resources*)`: 카탈로그 관리자가 렌더 리소스를 받아 PC의 SVG 지오메트리·패턴 비트맵을 만든다.
- `LayerManager`, `S100Layer::Open`이 `D2D1Resources*`를 요구.
- 파일 I/O가 `CFile`(117곳), 경로가 `CString`.

## 2. 단계

### 1단계. 렌더러 없이 데이터셋 열기 (렌더 캐시 지연 생성)

- [x] 1-1. `SCurve`/`SSurface`에 `GetD2Geometry(ID2D1Factory1*)` 오버로드 추가: `pGeometry`가 없으면 그 자리에서 생성. `CreateD2Geometry(nullptr)`는 no-op.
- [x] 1-2. 렌더 소비자(`SENC_AreaInstruction.cpp` 3곳, `SENC_LineStyle.cpp` 2곳)가 팩토리를 넘기는 오버로드를 쓰도록 변경.
- [x] 1-3. 모델 생성 코드(`S101Cell` 2곳, `S10XGML` 8곳)의 즉시 생성은 `D2`가 있을 때만 수행(있으면 기존과 동일한 성능 특성 유지, 없으면 지연 생성).
- [x] 1-4. 테스트 `OpensSmallestKoreanCellWithoutRenderer`: `S101Cell(fc, nullptr).Open(cell)`이 true, 피처·곡선 레코드 수 양수. GML(`S10XGML`)은 생성자가 FC를 받지 않아 보류(1-5).
- [x] 1-5(추가). **FC/PC가 Layer에 묶여 있던 문제.** `S101Cell(FeatureCatalogue*, ...)` 생성자가 `fc`를 버리고 `GetFC()`가 항상 `m_pLayer`를 거쳐서, Layer 없이 열면 `ATTRtoAttribute()`에서 널 FC의 속성 맵을 조회하다 접근 위반. `S100SpatialObject`에 `m_fc/m_pc` 멤버와 `SetFC/SetPC`를 두고 Layer가 없거나 Layer의 카탈로그가 비어 있으면 멤버를 쓴다. 데이터셋이 UI 개념(Layer) 없이 자립하는 첫걸음.
- [x] 1-6. `S10XGML`은 `SetFC()`로 FC를 직접 바인딩해 KOR_S-124 GML을 렌더러·Layer 없이 연다(테스트 `OpensKoreanS124WithoutRenderer`). 과정에서 발견·수정한 기존 버그: (a) `S10XGML::Open/OpenMetadata`가 pugi 파싱 결과를 무시해 없는 파일도 true 반환, (b) `GF::SpatialAttributeType::SetGMGeometry`가 `unique_ptr.reset()`으로 소유권을 가져가 `~S10XGML`에서 모든 피처 기하가 이중 해제됨(앱에서는 객체가 소멸되지 않아 숨어 있었음). 비소유 참조 `sharedGeometry`로 변경. (6e24bcd5)
- 완료 기준: `OpenS100Tests`에서 D2D 없이 8211 셀을 끝까지 연다(달성, 2026-11-08). 앱 렌더링 동일(기동 확인).

### 2단계. 렌더 훅을 모델에서 떼어내기

- [x] 2-1. `pGeometry`와 `CreateD2Geometry/GetD2Geometry/GetNewD2Geometry`를 `SCurve`/`SSurface`/`SCompositeCurve`에서 제거하고 `GeoMetryLibrary/Geometry.h`의 `CreateD2Geometry` 가상 함수도 제거. 대신 `Geometry::RenderAttachment`(불투명 슬롯, `unique_ptr`, 복사되지 않음)를 두고, 렌더 측 `GISLibrary/D2DGeometryCache.{h,cpp}`가 `D2DGeometry::Get(curve|surface, factory)`(첫 사용 시 생성·캐시)와 `CreateScaled(...)`(프레임용, 호출자가 Release)를 제공. 소비자 5개 파일(SENC_AreaInstruction, SENC_LineStyle, ExchangeCatalogue, S100ExchangeCatalogue, OpenS100View)과 모델 측 사전 생성(S101Cell, S10XGML, TestGISLibrary) 정리. 모델 헤더에서 `ID2D1*` 타입이 사라짐. 다중 링 표면의 지오메트리 그룹을 `ID2D1PathGeometry*`로 캐스팅하던 타입 펀도 함께 제거.
- [x] 2-2a. `SpatialObject`/`S100SpatialObject`에서 `D2D1Resources*` 멤버·생성자·`GetD2()` 제거. 데이터셋 생성자(`S101Cell`, `S10XGML`, `S100H5`, `S102H5`, `SHPFile`)에서 렌더러 인자 제거. S-102 그리기(`Draw`, `CreateBitmap`)를 `GISLibrary/S102Renderer`로 이동하고 `S100Layer::DrawGrid(d2, hdc, scaler)`가 호출(LayerManager가 D2를 명시적으로 넘김). `S100EditRender`는 `Set(..., renderResources)`로 렌더러를 받음. `S101Creator`의 D2 멤버·생성자 제거. `S100ExchangeCatalogue`(커버리지를 직접 그리는 UI 측 객체)는 자기 `D2` 멤버를 가짐. 남은 것: `S102H5::pWICBitmap`(렌더 캐시)과 `getColor()`의 `D2D1_COLOR_F`, `S102_Color`.
- [x] 2-2b. `SpatialObject`의 Draw 가상 함수 6종 중 5종 제거(`CDC*`, `D2D1Resources*`, `CDCRenderTarget*`, `ID2D1HwndRenderTarget*`, priority/geoType 변형). 호출자 없는 `Layer::Draw` 2종과 `S101Cell::Draw` 3종(빈 함수·스텁) 제거. 남은 것: `Draw(HDC&, Scaler*, double)` 하나(`SHPFile`, `S100ExchangeCatalogue`가 사용, GDI). 이것은 3단계(MFC 제거)에서 렌더 측으로.
- [x] 2-3a. `CatalogManager`에서 `D2D1Resources*` 멤버·생성자·`ChangeColorPallete` 제거. 카탈로그 추가 후 렌더 캐시 생성은 렌더 측 `GISLibrary/PortrayalRenderCache`(`PortrayalRender::Prepare(pc, d2)`, `ChangePalette(pc, name, d2)`)가 담당하고, 호출은 PC를 추가하는 쪽(앱의 `FC_PCSelectdlg`, `OpenS100View`, `S100Layer::OpenPC`)과 팔레트 변경(`LayerManager`)에서 명시적으로. 테스트 `CatalogManager.LoadsCataloguesWithoutRenderer`(FC+PC 로딩, 렌더러 없음).
- [x] 2-3b. `PortrayalCatalogue.h/.cpp`와 `S100_ColorProfile.h/.cpp`에서 D2D 타입 제거. PC의 `CreateSVGD2Geometry/CreatePatternImages/CreateLineImages/ChangePalette(d2...)`를 삭제하고 렌더 측 `PortrayalRender::Prepare/ChangePalette`가 `S100PCManager`의 캐시 함수를 직접 호출. PC의 `ChangePalette(name)`은 모델만(팔레트 이름·컬러 프로파일). 호출자 없는 `S100_ColorProfile::GetD2Color`, `SetS100PCManager` 삭제. `#include "../S100Engine/S100Render.h"`를 PC 헤더에서 제거(PortrayalCatalogue → S100Engine 역방향 의존 해소; 필요한 GISLibrary 파일 10개가 `S100PCManager.h`/`S100Render.h`를 직접 include). `s100PCManager`는 `std::unique_ptr`로, 헤더 기본 초기화 + `Init()` 이중 할당 누수 제거(그 누수가 `Init()` 미호출을 숨기고 있었음 → 생성자에서 `Init()` 호출). 남은 것(2-3c): `S100PCManager` 자체가 모델(SVG·선스타일·면채움·컬러 프로파일 파싱)과 D2D 캐시를 겸함. 모델 부분을 PortrayalCatalogue 쪽으로, 캐시를 렌더 쪽으로 분리.
- [x] 2-4. `Layer::Open`/`S100Layer::Open`에서 `D2D1Resources*` 인자 제거(데이터셋이 더 이상 필요로 하지 않음). 교환세트 커버리지 객체만 `LayerManager::GetRenderResources()`로 렌더러를 받는다. 호출자 없는 `S100Layer::OpenPC` 삭제, `(S100Layer*)layer->Open(...) == false`의 무의미한 캐스트 정리. `LayerManager` 자체는 렌더·로딩·경보가 섞인 sdk 객체라 D2 멤버를 유지(분할은 Phase 4).
- 완료 기준: `PortrayalCatalogue`, `GeoMetryLibrary`, `GISLibrary`의 **헤더**에 `d2d1.h`·`ID2D1*`가 없다(`.cpp`는 2-5 후).

### 3단계. MFC 제거

- [x] 3-1. `CFile` → 표준 C++. (a) 읽기: `GISLibrary/BinaryFile.h`의 `s100::io::ReadAllBytes(path)`(헤더 전용, `std::filesystem`+`std::ifstream`)를 도입하고 `S101Cell::Read8211`/`OpenMetadataBy000`가 사용. 수동 `new BYTE[]`/`delete[]`도 `std::vector`로 대체. 테스트 `S101Cell.MissingFileReturnsFalse`. (b) 쓰기: `GISLibrary/BinaryWriter.h`의 `s100::io::BinaryWriter`(`std::ostream` 위의 `Write(ptr, len)`)와 `BinaryFileWriter`를 도입하고, `WriteRecord/WriteField/WriteDirectory/WriteLeader`의 `CFile*` 매개변수 95개(헤더 53개 포함)를 `BinaryWriter*`로 일괄 교체. 호출 형태 `file->Write(p, n)`은 그대로라 185곳의 본문은 변경 없음. `S101Cell::SaveAsENC`는 `BinaryFileWriter`로 연다. 테스트 `SaveAsEncRoundTrip`(저장 후 재읽기, 레코드 수 동일). (c) `LayerManager::CheckFileType` 두 오버로드에 복사돼 있던 8211 헤더 스니핑을 `SniffIso8211(path)` 하나로 합치고(`s100::io::ReadBytes`), 기존의 `delete`/`delete[]` 불일치·태그 미일치 시 누수·1024바이트 이하 파일 처리 누락을 함께 정리. `SHPFile::Open`은 `s100::io::BinaryFileReader`(ifstream 기반 `Open/Seek/Read`)로. **GISLibrary 코드에서 `CFile` 사용 0.** 게이지 213 → 169 (BinaryWriter.h 전환으로 헤더 53개에서 MFC 타입이 사라짐).
- [~] 3-2. 공개 API의 `CString` 제거. (a) **ISO 8211 필드 계층** (ac910a4b): DSID(11)·CRSH(3)·GDAT(3)·VDAT(3)·`ATTR::m_atvl`·`CodeWithNumericCode::m_code`와 `R_DSGIR`/`F_CodeWithNumericCode`의 코드 조회 API가 `std::wstring`으로(CString 오버로드 삭제, `const std::wstring&`만 유지), `S101Cell`의 DSID getter/setter도 동일. `buf2charArr`는 `s100::text::ToWide`로 읽고(ATL `CA2W`·1 KiB 전역 버퍼 제거), 쓰기는 `s100::iso8211::WriteUtf8/WriteUtf8Unit/Utf8Length`(`ISO8211Fuc.h`)로 통일해 `CT2CA` 관용구 25곳 제거. 부수 효과로 DSID/CRSH/GDAT/VDAT의 `GetFieldLength`가 UTF-16 코드 유닛 수가 아니라 UTF-8 바이트 수를 세게 되어 비ASCII 텍스트에서의 잠재적 길이 불일치가 사라짐. `Leader`/`Directory`는 `CString::Format` → `std::format`. **문자열 타입 선택**: CONVENTIONS.md는 내부 저장을 `std::string`(UTF-8)로 두라고 하지만, 8211 계층의 기존 `std::wstring` 오버로드·FC 조회 키(`std::wstring`)·뷰어 UI와의 마찰을 최소화하기 위해 이 단계에서는 `std::wstring`을 택했다. UTF-8 `std::string` + `W` 접미사 getter로의 통일은 MFC 제거가 끝난 뒤 별도 단계로 다룬다. 게이지: GISLibrary MFC 헤더 33 → 17, 전체 137(게이지가 `//` 주석을 무시하도록 수정). (b) **파일 경로 API** (9b99556e): `SpatialObject`(Open/OpenMetadata/FilePath/FileName)·`Layer`(Open, GetLayerName/Path/Type)·`S100Layer`·`LayerManager`(AddLayer/AddLayerFront/AddBackgroundLayer/DeleteLayer/CheckFileType/pathToProductNumber 등)·`S100LayerFactory::Copy*`·`Inventory*::strFilePath`·`S100ExchangeCatalogue`·`CGISLibraryApp` 래퍼와 데이터셋 클래스 5종이 `std::wstring` 경로를 받고 돌려준다. `GISLibrary/PathUtil.{h,cpp}`(`s100::path::Extension/FileName/Stem/Directory/IsAbsolute/ExecutableDirectory`)가 `LibMFCUtil::GetExtension/GetFileName`과 손으로 짠 CString 슬라이싱을 대체하고(구분자 `\`·`/` 모두 인식), `GISLibrary/TextCompare.h`(`s100::text::CompareNoCase/IEquals`)가 확장자 비교의 `CompareNoCase`를 대체. `Layer::Open`은 상대 경로를 `std::filesystem`으로 실행 파일 디렉터리에 대해 해석하고, 모르는 확장자면 null 역참조 대신 false. `LayerManager` 생성자의 쓰이지 않던 모듈 경로 블록 삭제, `GetLayerName(int)/GetLayerNameByKey`가 이름을 실제로 반환하도록 수정(기존엔 항상 빈 문자열). 교환 카탈로그 커버리지 텍스트는 `std::wstring`/`std::to_wstring`, XML의 파일명은 UTF-8로 디코드(기존 `CString(const char*)`는 ANSI 코드 페이지). 좁은 문자열 XML 로더에는 `std::filesystem::path::string()`(네이티브 인코딩)으로 경로를 넘겨 `CT2CA`와 같은 동작 유지. 게이지: GISLibrary MFC 헤더 17 → 3, 전체 123. (c) 남은 것: GISLibrary 헤더 3개(`D2D1Resources.h`, `GISLibrary.h`(MFC 앱 클래스), `SCommonFuction.h`)와 S100Engine(`SVGReader` 12, `BackgroundSHPFile`, `S100Render`, `SHPPolygon.h`, `CStringCompareNoCaseLessThan.h` 삭제)·`D2D1Resources.cpp`(10)·`S101Cell.cpp` 잔여(`CString strValue`는 처리됨).
- [x] 3-3. `S101Cell`의 레코드 맵 7개를 `s100::RecordMap<R_X>`(`std::unordered_map<std::int64_t, R_X*>`, `GISLibrary/RecordMap.h`)로, `POSITION` 순회 25곳을 범위 기반 for로. 외부 호출자가 없던 `POSITION` 기반 공개 API 14개(`GetXStartPosition`/`GetNextAssoc`) 삭제. 업데이트 병합 루프 14곳이 `new R_X()` 뒤 바로 덮어쓰던 누수도 사라짐. `__int64` → `std::int64_t`(S101Cell). `S100PCManager`의 `CMap<CString,...>` → `std::unordered_map<std::wstring, IWICBitmap*>`, `F_DSID::m_dstc`의 `CArray<int>` → `std::vector<int>`. **코어에 MFC 컨테이너 0.**
- [x] 3-4. (a) **PortrayalCatalogue가 `UseOfMfc=false`로 빌드된다.** `stdafx.h`를 표준 헤더 + pugixml + StringUtil만으로 교체, 정적 라이브러리에 들어 있던 `dllmain.cpp`·`targetver.h` 삭제, 호출자 없는 `S100_ColorProfile::GetRGBRef`(COLORREF) 삭제, `S100_LineStyle.h`가 PCH를 include하던 것 제거(이것이 PC의 `using namespace Gdiplus`를 모든 소비자에게 새어 나가게 하고 있었음 → GISLibrary는 자기 PCH에서 GDI+를 받도록). S100Engine 쪽: 렌더 헤더 11개가 `RenderPrelude.h`(windows/d2d/dwrite/wincodec)를 직접 include해 자체 완결이 되었고, `S100ColorProfile`의 호출자 없는 `CString` 헬퍼 3개 삭제, `AreaFillInfo::patternMap` 키를 `CString`+`CStringCompareNoCaseLessThan`에서 `std::wstring`+`s100::CaseInsensitiveLessW`로. 게이지: PortrayalCatalogue 0/0/0, 전체 163. `tools/check-core-includes.py`는 FeatureCatalog·PortrayalCatalogue를 '깨끗한 프로젝트'로 고정해 회귀 시 실패한다. (b) **GeoMetryLibrary가 `UseOfMfc=false`로 빌드된다** (62ba9eab). `Geometry`의 `DrawGeometry/DrawTextInfo(HDC)` 가상 함수와 `CPoints` 스크래치 버퍼를 제거하고 GDI 그리기는 렌더 쪽 `GISLibrary::GdiRender::Draw`(`GdiGeometryRenderer.{h,cpp}`)로 이동(셰이프 배경 레이어, 레코드 디버그 그리기가 사용). `Scaler`는 숫자만 다루는 API로(`ScreenRect`, `SetScreen/SetMapFromScreen`, `GetScreenRect`, `GetFormattedScale()->std::wstring`), D2D 행렬/사각형/`RECT` 변환은 `GISLibrary/D2DView.h`(`Matrix/InverseMatrix/Rect/ScreenRECT`)로, 회전 MBR 계산은 `Gdiplus::PointF` 대신 표준 수학으로. `GeometricFuc`의 Win32 `min/max` 매크로 → `std::min/max`. LibMFCUtil 프로젝트 참조를 끊기 위해 `TextCodec.h`(`s100::text::ToWide/ToUtf8`, UTF-8↔wchar_t 순수 구현)와 헤더 전용 `StringTokenizer.h`(`s100::StringTokenizer`)를 GeoMetryLibrary에 추가. PCH는 표준 헤더만. 게이지: GeoMetryLibrary 0/0/0, 전체 153, 깨끗한 프로젝트 게이트에 GeoMetryLibrary 추가. 남은 것: S100Engine(렌더 모듈이므로 D2D는 허용, `CString` 5개 헤더만), GISLibrary(`CString` 33, Win32 74) → 3-2에서 처리.
- 완료 기준: 코어 프로젝트가 `UseOfMfc=false`로 빌드. 테스트 호스트에서 `AfxWinInit` 제거.

### 4단계. 적합성 복구

- [ ] 4-1. `Common.props`의 `ConformanceMode=false` 임시 조치 제거. GISLibrary 약 180건(C2664 94: 문자열 리터럴→`char*`(HDF5 래퍼 시그니처 `const char*`로), C2665 58: 임시→비상수 참조(`const&`), C2440 11: `CString`→`std::wstring`(3단계와 함께)) 정리.
- [ ] 4-2. `/W4` 프로젝트별 적용, 경고-에러는 코어부터.

### 5단계. 전 플랫폼 게이트

- [x] 5-1. `tools/check-core-includes.py`: 코어 프로젝트별로 D2D/DWrite/WIC 타입, MFC 타입, Win32 핸들을 이름으로 쓰는 **헤더 수**를 센다(렌더 측으로 선언된 4개 헤더는 D2D 허용). `--fail`로 CI 게이트, `--verbose`로 파일 목록. 기준선(2026-11-08, 2-4 이후):

```
project                headers   d2d   mfc  win32
FeatureCatalog              71     0     0      0
PortrayalCatalogue         128     3     1      2
GeoMetryLibrary             19     1     4      8
S100Engine                  51    11     8      0
GISLibrary                 375    26    76     72
LuaScriptingReference       16     0     0      1

tainted core headers: 213 (render-side files allowed: 4)
```

  목표는 모든 코어 프로젝트에서 0. 2-3b(S100PCManager 분리)와 3단계(MFC 제거)가 끝나면 FeatureCatalog·PortrayalCatalogue·GeoMetryLibrary부터 `--fail`을 켠다.
- [ ] 5-2. CMake 전환(Phase 4)과 함께 `x64-linux` 빌드 시도.

## 3. 진행 기록

| 날짜 | 항목 | 커밋 | 비고 |
|---|---|---|---|
| 2026-11-08 | 문서 작성, 인벤토리 | | |
| 2026-11-08 | 1-1~1-5: 렌더 캐시 지연 생성, 카탈로그 직접 바인딩, 렌더러 없는 Open 테스트 | bcbc2feb | 테스트 10/10 |
| 2026-11-08 | 1-6: GML도 렌더러 없이 열기, 이중 해제·파싱 실패 무시 수정 | 6e24bcd5 | 테스트 12/12. **1단계 완료** |
| 2026-11-08 | 2-1: D2D 지오메트리를 모델 밖 `D2DGeometryCache`로 | e0047a78 | 테스트 12/12, 앱 기동 확인 |
| 2026-11-08 | 2-2a: 데이터셋에서 렌더러 핸들 제거, `S102Renderer` 분리 | d6e09a08 | 테스트 12/12, 앱 기동 확인 |
| 2026-11-08 | 2-2b, 2-3a: Draw 인터페이스 축소, CatalogManager 렌더러 분리(`PortrayalRenderCache`) | f04e886b | 테스트 14/14, 앱 기동 확인 |
| 2026-11-08 | 2-4: `Layer::Open`에서 렌더러 인자 제거 | 13851aff | 테스트 14/14, 앱 기동 확인 |
| 2026-11-08 | 5-1: 결합 측정 스크립트와 기준선 | bea7251e | |
| 2026-11-08 | 3-1(a): 8211 리더의 `CFile`을 `s100::io::ReadAllBytes`로 | 74c5f817 | 테스트 15/15 |
| 2026-11-08 | 3-1(b): 8211 라이터 체인(95개 시그니처)을 `s100::io::BinaryWriter`로 | 62091ea2 | 테스트 16/16, 라운드트립 |
| 2026-11-08 | 3-1(c): `CheckFileType` 중복 제거·`SHPFile` 전환, GISLibrary에서 `CFile` 0 | ef05a1cc | 테스트 16/16, 앱 기동(셰이프 배경 로딩) |
| 2026-11-08 | 3-3: `CMap`/`POSITION`/`CArray` → 표준 컨테이너, 누수 14곳 제거 | 2ef3c6af | 테스트 16/16, 앱 기동 확인 |
| 2026-11-08 | 2-3b: PC 헤더에서 D2D 제거, PC→S100Engine 역방향 include 해소, PCManager 이중 할당 제거 | 356a2a8f | 테스트 16/16, 앱 기동 확인, 게이지 167 |
| 2026-11-08 | 3-4a: PortrayalCatalogue `UseOfMfc=false`, S100Engine 렌더 헤더 자체 완결(`RenderPrelude.h`) | d55a9251 | 테스트 16/16, 앱 기동 확인, 게이지 163 |
| 2026-11-09 | 3-4b: GeoMetryLibrary `UseOfMfc=false`, GDI 그리기·D2D 뷰 변환을 렌더 쪽으로, LibMFCUtil 참조 제거 | 62ba9eab | 테스트 16/16, 앱 기동 확인, 게이지 153. **3-4 완료** |
| 2026-11-09 | 3-2a: ISO 8211 필드 계층 `CString` → `std::wstring`, UTF-8 쓰기 헬퍼, `std::format` | ac910a4b | 테스트 16/16(8211 라운드트립 포함), 앱 기동 확인, 게이지 137 |
| 2026-11-09 | 3-2b: 파일 경로 API `CString` → `std::wstring`, `PathUtil`/`TextCompare` 도입 | 9b99556e | 테스트 16/16, 앱 기동 확인, 게이지 123 |
