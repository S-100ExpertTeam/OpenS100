# R8: 뷰를 모든 플랫폼에서 (2026-11-24)

- 상태: R8a~R8c 완료. 남은 것은 5절.
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

- Release·Win32·ARM64 구성의 빌드(Release x64는 사용자의 뷰어가 실행 중이라 DLL이 잠겨 빌드하지 못했다), CI, macOS.
- 뷰어 화면에서의 확인(지침 23). 뷰어 쪽 코드는 바뀌지 않았다(`OpenS100View`는 `SkiaViewFrame`의 `Begin`/`End(HDC)`를 전처럼 쓴다).
- Linux 그림을 Windows 그림과 수치로 대조하지 않았다. Linux에서 격자(S-102/104/111)·교환 카탈로그·System Database 차트는 컴파일·링크만 했고 그려 보지 않았다.
- `opens100_view`를 Linux에서 돌릴 때 `PortrayalAPI.lua:806: Object given is of type "nil" not of type "SpatialType"`가 두 번 찍힌다. Windows에서도 같은지 보지 않았다.
- Windows CMake 빌드에서의 `view`·`view-skia`.

## 5. 남은 것

- 4.1의 흔들림의 원인 — 이후의 모든 그림 검증이 이것에 걸린다.
- 포트(`ports/opens100`)의 기능 `view`·`view-skia`, `opens100Config.cmake.in`, 소비자 예제에서 뷰 그리기, CI `port` 잡에 추가.
- `GISLIBRARY_MODEL_ONLY`와 `stdafx.h`의 `<windows.h>`·ATL: 뷰 소스가 이제 그것 없이 빌드되므로 솔루션 쪽에서도 걷을 수 있다.
- `DeviceTypes.h`의 `HDC`·`RECT`·`POINT`: `HDC`는 `SkiaViewFrame`과 `CGISLibraryApp::Draw(HDC&)`만 쓴다. `RECT`는 `SetScreen`·`SetViewMBR`의 인자다.
- 한 표면에 한 프레임(R7 3.1), GPU 표면(R7g).
