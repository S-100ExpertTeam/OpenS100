# R8: 뷰를 모든 플랫폼에서 (2026-11-24)

- 상태: R8a~R8f 완료(포트 기능 `view`·`view-skia` 4.4, `stdafx.h`에서 COM·ATL 제거 4.5, `<windows.h>` 제거 4.6, Windows에서 포트 확인과 기하 없는 피처의 Lua 오류 4.7, 죽은 코드 정리 4.8), 4.1의 흔들림은 원인을 찾아 고쳤다(4.3). 남은 것은 5절.
- 지침: 1(라이브러리), 18(전 플랫폼), 24(Skia 단일). 앞 단계: [R7](2026-11-24-r7-viewer-on-skia-design.md).

## 1. 출발점

R6에서 Linux는 이미 **셀 하나**를 PNG로 그렸다(소비자 예제 `--png`: 데이터셋 → 포트레이얼 → `DisplayListPainter` → Skia). 그러나 "뷰" — 여러 레이어를 열고, 순서를 정하고, 배경 지도·격자(S-102/104/111)·교환 카탈로그·System Database 차트를 함께 그리는 쪽 — 은 CMake 타깃 밖에 있었다(`cmake/GISLibraryDatasetSources.cmake`의 "(no target)": `LayerManager`, `CoverageDataset`, `SHPFile`, `S100ExchangeCatalogue`, `SystemDatabaseChart` 등 24개 소스). Visual Studio 솔루션만 빌드했다.

R7에서 Direct2D·GDI+가 빠진 뒤 이 소스들을 Windows에 묶는 것을 세어 보니 거의 하나였다: **그리기 함수가 `HDC&`를 받는다.** `SpatialObject::Draw(HDC&, …)`, `Layer::Draw`, `LayerManager::Draw*` 열 개, `S100Layer::DrawGrid`, `CoverageDataset::DrawCoverage`. 함수 안에서 `HDC`는 쓰이지 않고 `SkiaViewFrame::Shared().End(hDC)`까지 전달될 뿐이었다. `SkiaViewFrame`도 DIB와 `AlphaBlend`를 빼면 플랫폼 중립이다.

## 2. 설계

- **`ViewFrame`**(`GISLibrary/ViewFrame.h`, 추상): 뷰의 한 번 그리기가 그려지는 곳. `Begin(scaler)` / `Renderer(catalogue)` / `Paint(priority, drawingSet, catalogue, scaler, texts)` / `End()`. 레이어는 프레임 뒤에 창이 있는지 메모리 그림이 있는지 모른다. 뷰의 그리기 함수는 모두 `HDC&` 대신 `ViewFrame&`를 받는다.
- **`SkiaViewFrame`**이 `ViewFrame`을 구현한다. 기록(SkPicture) → 띠로 나눠 여러 스레드가 래스터화하는 구조는 그대로. Windows에서는 픽셀이 DIB이고 `End(HDC)`·`Scene(frame, HDC)`가 `AlphaBlend`로 얹는다. 그 밖에서는 픽셀이 제 버퍼다. 창 없이 쓰는 길: `Draw(CGISLibraryApp&)`(한 장면) → `WritePng(path, 바탕색)`. 서체는 `SetFontFiles`로 파일을 주면 그것을 쓰고(어느 시스템에서나 같은 글자), 없으면 지금처럼 시스템 서체를 이름으로 찾는다(Windows만 답한다).
- `CGISLibraryApp::Draw(ViewFrame&)`가 중립 진입점이다. `Draw(HDC&)`는 Windows에만 선언되고 `SkiaViewFrame.cpp`가 정의한다(프로세스의 프레임에 장면을 열고 위 함수를 부른다) — 뷰어와 테스트의 호출부는 그대로다. 장면을 여는 곳이 `LayerManager::Draw`에서 여기로 올라왔다(안쪽 `LayerManager`의 장면은 어차피 바깥 것에 묻혔다).
- CMake 타깃: **`opens100::view`**(뷰 소스 24개, `portrayal`·`sdb`·`coverage`에 의존, 백엔드를 모른다)와 **`opens100::view-skia`**(`SkiaViewFrame`, `view`·`render-skia`에 의존). 옵션 `OPENS100_BUILD_VIEW`(기본 ON), `OPENS100_BUILD_EXAMPLES`(기본 OFF).
- 예제 `examples/view`(`opens100_view`): FC·PC·데이터셋 여러 개(선택: 배경 셰이프파일, 서체 파일)를 받아 뷰를 PNG로 쓴다. 트리 안에서 빌드한다(포트를 거치지 않는다).

"한 표면에 한 프레임"(R7 3.1)은 하지 않았다: 레이어마다 `Begin`/`End`를 부르는 구조는 그대로이고, 장면이 그것들을 한 번의 래스터화로 묶는다.

## 3. Linux에서 고친 것

`view` 소스 24개 중 다섯이 GCC에서 걸렸다.

| 파일 | 걸린 것 | 고친 것 |
|---|---|---|
| `CoverageDataset.cpp` | `UINT32` | `std::uint32_t` |
| `S100Utilities.cpp`, `S100ExchangeCatalogue.cpp` | `min`/`max` 매크로, `path` → `wstring` 암묵 변환 | `(std::min)`/`(std::max)`, `.wstring()` |
| `LayerManager.cpp` | `<mmsystem.h>`(쓰이지 않음) | 삭제 |
| `GISLibrary.cpp` | `<direct.h>`, `_mkdir("..\\TEMP")` | `std::filesystem::create_directory` |

실행해서 나온 것: 규칙 파일 경로를 `L"Rules\\"`로 잇는 곳 둘(`LayerManager.cpp`, `CoverageDataset.cpp`) — Linux에서 `Rules\main.lua`를 못 열어 레이어가 열리지 않았다. `L"Rules/"`로 바꿨다.

## 4. 진행 기록

| 날짜 | 한 일 | 검증 |
|---|---|---|
| 2026-11-24 | **R8a** `ViewFrame`, 뷰의 그리기 함수가 `HDC&` 대신 `ViewFrame&`. **R8b** `SkiaViewFrame`이 `ViewFrame`을 구현하고 모든 플랫폼에서 빌드된다(`Draw`, `WritePng`, `SetFontFiles`). **R8c** CMake 타깃 `view`·`view-skia`, 예제 `opens100_view`, 3절의 수정 | Windows Debug x64: 솔루션 빌드, 전체 테스트 230개 중 226개 통과·4개 건너뜀·실패 0. 그림 테스트 62장 비교는 4.1. Linux(Docker `opens100-lin`, Ubuntu 24.04, GCC 13, vcpkg `x64-linux`, `render-skia`): 빌드 621단계 통과, `opens100_view` 링크(정의 안 된 심벌 없음). `opens100_view`로 배경 지도 + `101KR00659C71` + `101KR00659C72`를 1600×1000 PNG로 그렸다(여는 데 832 ms, 그리는 데 664 ms; 눈으로 확인: 육지·수심 면·수심 숫자·심벌·글자). |

### 4.1 그림 비교와, 원래 있던 흔들림

`RenderSmoke.*`의 그림 62장을 수정 전(HEAD `fc95d229`, Debug)과 수정 후(Debug)로 그려 바이트 단위로 비교했다: **58장 동일, 4장 다름**(`s102-covers-data-esc-off/on`, `sdb-chart-s101-s124`, `sdb-chart-124KR.KCG.XI.S100.TEST2`).

다른 4장이 수정 탓인지 보려고 **수정 전 HEAD의 같은 실행 파일을 한 번 더** 돌렸다: HEAD끼리도 62장 중 **9장이 다르다**(`adjacent-*` 4장, `s102-covers-data-*` 2장, `sdb-chart-*` 3장). 다를 때의 모양은 한쪽 그림에 수심 숫자·점 심벌·무늬 채움이 통째로 빠지는 것이다(차이 5~13%). 즉 **HEAD가 실행마다 다른 그림을 낸다** — R8과 무관하게 있던 것이고, 원인은 찾지 않았다. 의심: `9d3332ad`(성능)나 `a5d7b03f`(워커 스레드로 열기) 이후의 것. 이 테스트들은 색 수와 흰 픽셀 수만 확인해서 통과한다.

따라서 수정 후의 4장은 "HEAD 자체의 흔들림 범위 안"이라고만 말할 수 있고, 바이트 동일은 확인하지 못했다.

### 4.2 미확인

(4.1의 "바이트 동일은 확인하지 못했다"는 4.3에서 풀렸다.)

- Release·Win32·ARM64 구성의 빌드(Release x64는 사용자의 뷰어가 실행 중이라 DLL이 잠겨 빌드하지 못했다), CI, macOS.
- 뷰어 화면에서의 확인(지침 23). 뷰어 쪽 코드는 바뀌지 않았다(`OpenS100View`는 `SkiaViewFrame`의 `Begin`/`End(HDC)`를 전처럼 쓴다).
- Linux 그림을 Windows 그림과 수치로 대조하지 않았다. Linux에서 격자(S-102/104/111)·교환 카탈로그·System Database 차트는 컴파일·링크만 했고 그려 보지 않았다.
- `opens100_view`를 Linux에서 돌릴 때 `PortrayalAPI.lua:806: Object given is of type "nil" not of type "SpatialType"`가 두 번 찍힌다. Windows에서도 같은지 보지 않았다.
- Windows CMake 빌드에서의 `view`·`view-skia`.

### 4.3 흔들림의 원인과 수정 (2026-11-25)

**원인.** `SkiaViewFrame`은 카탈로그에서 읽은 것(심벌 그림, 선 스타일, 면 채움, 심벌 이미지)을 `PortrayalCatalogue*` **주소**로 기억했다(`State::catalogues`). 프로세스의 프레임(`Shared()`)은 카탈로그보다 오래 산다. 카탈로그가 지워지고 **같은 주소에 다른 카탈로그가 만들어지면**(힙이 주소를 다시 내줄 때) 프레임은 앞 카탈로그의 것을 그대로 썼다. S-124 카탈로그가 있던 자리에 S-101 카탈로그가 들어서면 S-101의 수심 숫자·점 심벌·무늬는 S-124의 심벌 목록에 없으므로 빠진다. 주소가 다시 쓰이는지는 힙 사정이라 실행마다 달랐다. 테스트에서는 `CGISLibraryApp`을 테스트마다 새로 만들어서 드러났고, 뷰어에서는 카탈로그를 내리고 다시 올릴 때 같은 일이 생길 수 있었다. `9d3332ad`·`a5d7b03f`의 스레드와는 무관하다(주소로 기억하는 것은 R7부터).

**수정.** `PortrayalCatalogue::Lifetime()`이 카탈로그와 함께 끝나는 표(`weak_ptr`)를 준다(`Delete`로 비울 때도 끝난다). `CatalogueResources`가 그것을 함께 갖고, 프레임은 렌더러를 만들 때 표가 끝난 카탈로그의 것을 먼저 버린다.

**검증.** 새 테스트 `RenderSmoke.ACatalogueMadeWhereAnotherWasIsDrawnWithItsOwnSymbols`: 한 자리에 S-124 카탈로그, 이어서 S-101 카탈로그를 만들어 부표 심벌을 그린다. 수정 전 실패(795픽셀이 다름), 수정 후 통과. Windows Debug x64 전체 테스트를 세 번: 매번 231개 중 227개 통과·4개 건너뜀, **그림 62장이 세 번 모두 바이트 단위로 같다.** 수정 전 Debug 그림과는 5장이 다르다(`s102-covers-data-*` 2장, `sdb-chart-s101-s124`, `sdb-chart-124KR…TEST1/2`) — 수정 전 쪽이 심벌이 빠진 그림이다(`sdb-chart-s101-s124`는 눈으로 확인: 수심 숫자·무늬·심벌이 있다).

R8a~R8c의 그림이 수정 전과 같은지는 여전히 직접 대조하지 못했다(대조할 수정 전 그림이 흔들렸으므로). 다만 R8의 58장은 수정 전과 같았고 나머지는 위 원인으로 설명된다.

### 4.4 R8d: 포트 기능 `view`·`view-skia` (2026-11-26)

- `ports/opens100`: 기능 `view`(→ `portrayal`·`system-database`·`coverage`)와 `view-skia`(→ `view`·`render-skia`). `portfile.cmake`가 `OPENS100_BUILD_VIEW`를 기능에 맞춰 켜고 끈다. `port-version` 1.
- **R8c가 포트를 깨뜨려 놓았었다**: `OPENS100_BUILD_VIEW`의 기본값이 ON인데 포트는 그 옵션을 몰랐으므로, `portrayal`·`system-database`·`coverage`를 다 고르지 않은 설치(기본 기능 포함)는 구성 단계에서 `FATAL_ERROR`로 멈췄다. 기능이 생기면서 풀렸다.
- `opens100Config.cmake.in`은 바꿀 것이 없었다: 뷰가 찾는 의존성은 `portrayal`·`sdb`·`coverage`의 것과 같다.
- 소비자 예제: 패키지에 `opens100::view-skia`가 있으면 `opens100_consumer_view`를 함께 빌드한다. 소스는 `examples/view/main.cpp` 그대로다(포함 경로를 `<GISLibrary/...>`로 바꿔 트리 안 빌드와 설치본 양쪽에서 쓴다).
- 소비자 예제의 `--png` 쪽이 `9d3332ad` 이후 컴파일되지 않고 있었다(`SkiaResources`에 멤버가 끼어들어 중괄호 초기화가 어긋남). 멤버 이름으로 채우게 고쳤다.

검증(Linux, Docker `opens100-lin`, vcpkg 클래식 모드, 오버레이 포트): `opens100`(기본 기능) 설치 통과, `opens100[core,view-skia]` 설치 통과(라이브러리 12개), 소비자 예제 구성·빌드 통과, `opens100_consumer`(카탈로그 읽기, `--png`로 셀 한 장) 실행, `opens100_consumer_view`로 배경 지도 + 셀 2개를 그린 PNG가 트리 안 빌드의 `opens100_view`가 그린 것과 **바이트 단위로 같다**. 트리 안 빌드(621단계)도 다시 통과.

### 4.5 R8e: `stdafx.h`에서 COM·ATL을 걷었다 (2026-11-26)

- `GISLibrary/stdafx.h`: `<tchar.h>`, `<comdef.h>`, `<atlbase.h>`, `<atlconv.h>`, `<atlsafe.h>`, `SafeRelease`, `ole32.lib`·`winmm.lib` 링크 지시를 지웠다. `GISLibrary`의 어떤 소스도 쓰지 않았다(주석 속 흔적뿐).
- `GISLIBRARY_MODEL_ONLY`가 없어졌다: 솔루션과 CMake가 같은 머리말로 같은 코드를 빌드한다.
- `<windows.h>`는 남았다. 통째로 빼 보니 `POINT`·`RECT`의 정의가 없어 일곱 소스가 걸렸다(`DeviceTypes.h`는 Windows에서 Win32의 형을 선언만 한다). 그 형을 라이브러리의 것으로 바꾸는 일은 뷰어의 호출부까지 닿으므로 따로 한다(5절).
- ATL 머리말이 꺼 주던 경고 C4505가 드러나 `host_data.cpp`의 쓰이지 않는 정적 함수 둘(`get_spatial_association(F_SPAS*)`, `get_simple_attribute_values(R_InformationRecord*, …)`)을 지웠다. CMake의 `portrayal`·`view`에서 `/wd4505`·`-Wno-unused-function`도 뺐다(Linux 빌드에 그 경고 0건).

검증: Windows Debug x64·Release x64 솔루션 빌드, 전체 테스트 각각 231개 중 227개 통과·4개 건너뜀, 그림 62장이 걷기 전과 같다(다른 픽셀 0%). Linux 트리 안 빌드 621단계 통과. (Release x64는 R8a~R8d와 4.3의 수정에 대해서도 이때 처음 돌렸다: 통과, 그림은 Debug와 같다.)

### 4.6 R8f: `POINT`·`RECT`가 라이브러리의 형이 되었다, `stdafx.h`에 `<windows.h>`가 없다 (2026-11-26)

- `GeoMetryLibrary/ScreenTypes.h`: `ScreenRect`(`Scaler.h`에서 옮김)와 `LongPoint`(긴 정수 둘). `GISLibrary`의 `POINT` → `LongPoint`, `RECT` → `ScreenRect`(20개 파일). 멤버 이름과 크기는 Win32의 것과 같다.
- `DeviceTypes.h`는 `ScreenTypes.h`를 포함하고 `HDC`의 이름만 선언한다.
- **공개 API가 바뀌었다**: `CGISLibraryApp::SetScreen(const ScreenRect&)`, `SetViewMBR(ScreenRect)`, `LayerManager::SetViewMBR(ScreenRect)`. `SetScreen(RECT*)`는 없앴다. Win32의 `RECT`나 MFC의 `CRect`는 저절로 바뀌지 않으므로 호출부가 `ScreenRect{ r.left, r.top, r.right, r.bottom }`으로 넘긴다: 뷰어 5곳(`OpenS100View.cpp`), 테스트 22곳, 예제 1곳.
- `GISLibrary/stdafx.h`에서 `<windows.h>`를 뺐다. Windows를 부르는 소스는 제가 포함한다(`DebugOutput.cpp`가 하나 더해졌다; `PathUtil.cpp`·`SkiaViewFrame.cpp`·`SystemDatabaseSession.cpp`는 이미 그랬다). `examples/view/main.cpp`도 `<windows.h>` 없이 빌드된다.

검증: Windows Debug x64·Release x64 솔루션 빌드(뷰어 포함), 전체 테스트 각각 231개 중 227개 통과·4개 건너뜀, 그림 62장이 바꾸기 전과 같다(다른 픽셀 0%). Linux 트리 안 빌드 621단계 통과, `opens100_view`의 그림이 R8c 때의 것과 바이트 단위로 같다. 뷰어는 빌드만 했고 화면은 보지 않았다. 포트 설치본으로의 소비자 예제 빌드는 이 변경 뒤에 다시 돌리지 않았다.

### 4.7 포트를 Windows에서 확인했다, 기하 없는 피처의 Lua 오류를 고쳤다 (2026-11-27)

**포트(R8f 뒤)**: 두 플랫폼에서 의존성을 바이너리 캐시에서 꺼내고 `opens100`(기본 기능)과 `opens100[core,view-skia]`를 클래식 모드로 설치한 뒤, 설치본으로 `examples/consumer`를 빌드해 돌렸다.

- Linux(GCC 13, `x64-linux`): 설치·빌드·실행 통과, `opens100_consumer_view`의 그림이 트리 안 빌드의 것과 바이트 단위로 같다.
- Windows(MSVC 19.51, `x64-windows`, `C:cpkg`): 설치 통과(라이브러리 12개), 소비자 예제 빌드·실행 통과, 셀 두 개의 그림을 썼다. Windows에서 포트를 확인한 것은 처음이다.
- 소비자는 패키지를 빌드한 것과 같은 MSVC 도구 집합으로 빌드해야 한다. 패키지는 v145(VS 18)로 빌드되었는데 PATH의 CMake 3.29가 VS 2022(v143)를 골라 STL의 벡터화 함수(`__std_find_first_not_of_trivial_pos_1` 등)가 풀리지 않았다. 포트의 결함이 아니라 정적 라이브러리의 일반 규칙이다(새 도구 집합으로 만든 것을 옛 도구 집합으로 링크할 수 없다).

**Lua 오류**: `PortrayalAPI.lua:806: Object given is of type "nil" not of type "SpatialType"`가 셀마다 한 번 났다(Windows에서는 `OutputDebugString`으로 가서 보이지 않았을 뿐 같은 코드다). 기하가 없는 피처에 `hd_get_feature_spatial_associations`가 공간 형이 빈 연관 하나를 돌려주었고, 규칙 파일이 그것으로 연관을 만들다 멈추어 그 피처는 포트레이얼을 받지 못했다. 이제 빈 목록을 돌려준다: 규칙은 그 피처를 기본형 `None`으로 본다. 테스트 `HostData.AFeatureWithNoGeometryHasNoSpatialAssociation`.

- 함께 고친 것: `hd_get_curve`가 위상 지시자 3(시작이자 끝, 닫힌 곡선)을 끝점으로만 읽었다. 이제 시작점도 채운다(`R_CurveRecord.cpp`는 이미 3을 양쪽으로 읽는다). 이 경우를 가진 시험 데이터는 없었다: 그림은 바뀌지 않았다.

검증: Windows Debug x64·Release x64 솔루션 빌드, 전체 테스트 각각 232개 중 228개 통과·4개 건너뜀, 그림 62장이 바꾸기 전과 같다. Linux 트리 안 빌드 통과, `opens100_view`에서 Lua 오류가 사라졌고 그림은 바이트 단위로 같다. 새 테스트를 고치기 전 코드에 돌려 보지는 않았다.

### 4.8 주석으로 남은 죽은 코드와 Direct2D의 흔적을 지웠다 (2026-11-27)

- **주석 처리된 코드 약 1,900줄**(46개 파일)을 지웠다. 큰 것: `SENC_PointInstruction.cpp`(`D2D1_POINT_2F`로 점을 모으던 옛 함수들), `host_data.cpp`(8211 레코드에서 속성을 찾던 옛 구현), `SENC_TextInstruction.cpp`, `SENC_AugmentedRay.cpp`, `S100_CIE.*`, `S100_Symbol.*`. 줄 전체가 `//`인 것만 지웠으므로 컴파일되는 것은 바뀌지 않는다. 고른 기준: 주석 블록에서 코드로 읽히는 첫 줄부터 마지막 줄까지, 코드로 읽히는 줄이 셋 이상이고 3분의 2 이상일 때(설명 주석은 남긴다).
- **Direct2D를 말하던 주석 13곳**을 지금의 사실로 고쳤다(`LayerOpen.cpp`, `S100SpatialObjectPortrayal.cpp`, `DisplayListPainter.*`, `DrawTypes.h` 등). 라이브러리 모듈과 테스트에 `Direct2D`·`D2D1`·`GDI+`라는 말이 남지 않았다.
- `GISLibrary/Namespace_GISLibrary.h`를 지웠다: 없는 클래스(`D2D1Resources`, S-57·S-52의 것 열여섯)의 선언뿐이었다.
- `LuaScriptingReference/stdafx.h`에서 `<windows.h>`를 뺐다(`lua_session.cpp`가 `OutputDebugStringA` 때문에 제가 포함한다). 미리 컴파일된 헤더에 운영체제 헤더가 있는 라이브러리 모듈이 없다.

검증: Windows Debug x64·Release x64 솔루션 빌드, 전체 테스트 각각 233개 중 229개 통과·4개 건너뜀, 그림 62장이 바꾸기 전과 같다. Linux 트리 안 빌드 통과, `opens100-render`의 그림 셋(전체·중심+축척·Night)이 바꾸기 전과 바이트 단위로 같다.

남긴 것: `namespace libS100Engine`(`PortrayalCatalogue/Color.*`, 이름만 옛 엔진의 것이다), `LibMFCUtil` 프로젝트(뷰어만 쓴다), 저장소에 든 `*.vcxproj.user` 여덟 개.

## 5. 남은 것

- CI `port` 잡은 `view`만 넣었다(`render-skia`·`view-skia`는 Skia 빌드가 무거워 CI에 없다). CI는 돌려 보지 않았다.
- 상대 경로의 배경 레이어는 실행 파일의 디렉터리에서 찾는다(`LayerOpen.cpp`, 뷰어의 옛 동작). 라이브러리의 소비자는 절대 경로를 넘겨야 한다. 바꾸지 않았다.
- `DeviceTypes.h`에 남은 것은 `HDC`의 이름뿐이다: `SkiaViewFrame`의 Windows 끝과 `CGISLibraryApp::Draw(HDC&)`만 쓴다.
- `LongPoint`는 화면의 픽셀과 데이터셋의 정수 좌표(1e-7도)에 함께 쓰인다(`POINT`가 그렇게 쓰였다). 둘을 가르는 것은 하지 않았다.
- 한 표면에 한 프레임(R7 3.1), GPU 표면(R7g).
