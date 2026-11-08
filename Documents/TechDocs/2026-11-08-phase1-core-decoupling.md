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
- [ ] 2-3b. `PortrayalCatalogue.h`에서 D2D 타입 제거: `S100PCManager`(SVG D2D 지오메트리·패턴 비트맵 보유)를 PC 밖 렌더 측 캐시로 이동(`GetS100PCManager()` 참조 24곳: SENC_* 7파일, LayerManager, GISLibrary.cpp, S102_Color). `S100_ColorProfile::GetD2Color` → `GetColor()`(RGBA 구조체).
- [x] 2-4. `Layer::Open`/`S100Layer::Open`에서 `D2D1Resources*` 인자 제거(데이터셋이 더 이상 필요로 하지 않음). 교환세트 커버리지 객체만 `LayerManager::GetRenderResources()`로 렌더러를 받는다. 호출자 없는 `S100Layer::OpenPC` 삭제, `(S100Layer*)layer->Open(...) == false`의 무의미한 캐스트 정리. `LayerManager` 자체는 렌더·로딩·경보가 섞인 sdk 객체라 D2 멤버를 유지(분할은 Phase 4).
- 완료 기준: `PortrayalCatalogue`, `GeoMetryLibrary`, `GISLibrary`의 **헤더**에 `d2d1.h`·`ID2D1*`가 없다(`.cpp`는 2-5 후).

### 3단계. MFC 제거

- [ ] 3-1. `CFile` → `std::ifstream`/`std::filesystem`(117곳). 바이너리 읽기 헬퍼 하나로 통일.
- [ ] 3-2. 공개 API의 `CString` → `std::string`(UTF-8)/`std::wstring`(CONVENTIONS.md). `SpatialObject::Open(CString)` 등. 내부 `CString` 137곳.
- [ ] 3-3. `CMap`/`POSITION` 순회(8곳) → `std::unordered_map`.
- [ ] 3-4. `GISLibrary`, `S100Engine`, `PortrayalCatalogue`, `GeoMetryLibrary`에서 `stdafx.h`의 `afx*` include 제거 → `UseOfMfc=false`. 렌더 전용 코드(`HDC`, `CDC`)는 `S100Engine`(렌더 모듈)로 격리.
- 완료 기준: 코어 프로젝트가 `UseOfMfc=false`로 빌드. 테스트 호스트에서 `AfxWinInit` 제거.

### 4단계. 적합성 복구

- [ ] 4-1. `Common.props`의 `ConformanceMode=false` 임시 조치 제거. GISLibrary 약 180건(C2664 94: 문자열 리터럴→`char*`(HDF5 래퍼 시그니처 `const char*`로), C2665 58: 임시→비상수 참조(`const&`), C2440 11: `CString`→`std::wstring`(3단계와 함께)) 정리.
- [ ] 4-2. `/W4` 프로젝트별 적용, 경고-에러는 코어부터.

### 5단계. 전 플랫폼 게이트

- [ ] 5-1. 코어 프로젝트에 `windows.h`·`afx*`·`d2d1.h` include를 검출하는 스크립트(`tools/check-core-includes.py`)와 테스트.
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
