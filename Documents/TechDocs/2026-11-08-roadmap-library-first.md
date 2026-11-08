# OpenS100 로드맵: S-101 뷰어에서 S-100 핵심 라이브러리로 (2026-11-08)

- 상태: 초안. 소유자: jogm. 다른 세션은 [DIRECTIVES.md](DIRECTIVES.md)와 이 문서를 먼저 읽고, 변경 시 날짜와 함께 아래 "변경 이력"에 적는다.
- 관련 문서: [코드 평가](2026-11-08-code-assessment.md), [opens100-web 역이식](2026-11-08-backport-plan-from-opens100-web.md), [KHOA 뷰어 업스트림](2026-11-08-upstream-plan-from-khoa-viewer.md), [Phase 0 작업 로그](2026-11-08-phase0-worklog.md)

## 1. 목표

OpenS100을 **S-100 분야의 핵심 C++ 라이브러리**로 만든다. 포지션은 GEOS(기하), GDAL(포맷), libcurl(프로토콜)과 같다. "S-100 관련 표준을 다루려면 가져다 쓰는 기본 라이브러리"이며, 최종적으로 **vcpkg 공식 레지스트리에 `opens100` 포트로 등록**한다.

S-100은 프레임워크 표준 전체를 뜻한다. Part 3(데이터 모델), 4a(메타데이터), 5(FC), 7(공간 스키마), 9/9a(포트레이얼, Lua), 10a/10b/10c(ISO 8211, GML, HDF5), 13(스크립팅), 15(보안), 17(교환세트), S-98(상호운용), S-101~S-131, S-201, S-4xx 제품 사양.

현재 OpenS100은 README대로 "오픈소스 S-101 ENC 뷰어"다. 이 정의를 바꾼다. **라이브러리가 중심이고, `OpenS100` 앱(MFC)은 라이브러리를 검증하는 얇은 소비자 앱**이 된다. 같은 라이브러리의 다른 소비자가 KHOA S-100 Viewer, 그리고 장기적으로 다른 기관의 ECDIS·편집기다.

## 2. 세 저장소의 역할

| 저장소 | 역할 | 방향 |
|---|---|---|
| **OpenS100** (이 저장소, 공개, MPL-2.0) | 코어 라이브러리 + 최소 검증 앱 | 모든 코어 기능이 여기로 모인다 |
| **KHOA S-100 Viewer** (`S-100SimpleEditor`, 비공개) | OpenS100을 사용하는 완성형 뷰어·편집기. KHOA OpenAPI, SECOM, AIS, 항로, 조석 등 기관 특화 기능 | 자체 엔진(GISLibrary 90K줄)을 버리고 OpenS100을 링크. **KHOA 뷰어에만 있는 코어 기능은 OpenS100으로 업스트림** |
| **opens100-web** (공개, TS) | 브라우저 구현. OpenS100을 참조해 시작했으나 구조·테스트·성능에서 앞섬 | **앞선 설계를 OpenS100으로 역이식.** 웹에 없는 것(8211 쓰기, 편집)은 반대 방향 |

업스트림(KHOA → OpenS100)과 역이식(web → OpenS100)이 동시에 OpenS100으로 들어오므로, **OpenS100의 모듈 경계를 먼저 세우고 그 경계에 맞춰 받아들인다.** 경계 없이 KHOA 코드를 그대로 가져오면 OpenS100이 KHOA 뷰어의 복사본이 될 뿐이다.

## 3. 목표 아키텍처

### 3.1 모듈 분할 (opens100-web 패키지와 1:1 대응을 목표로)

| 모듈 | 책임 | OpenS100 현재 | KHOA 뷰어에서 올 것 | opens100-web 대응 |
|---|---|---|---|---|
| `s100-core` | 데이터 모델(피처·정보·공간 객체·속성), FC/PC/IC 파서, 디스플레이 리스트 IR, 제품 판별, 카탈로그 레지스트리, 데이터셋 검증, 교환세트 모델 | `S100Engine`, `FeatureCatalog`, `PortrayalCatalogue`, `GISLibrary/ExchangeCatalogue`, `IC_InteroperabilityCatalogue` | `S100ExchangeSet`(읽기 확장), `DLL_InteroperabilityCatalog` 비교 후 병합 | `packages/core` |
| `s100-iso8211` | Part 10a 읽기·쓰기, 업데이트 적용·생성 | `GISLibrary/S101Cell`(읽기), `DRReader*`, `R_*Record` | 8211 쓰기(`S101Cell::Save`), 업데이트 생성·병합(`CreateUpdateFile`) 확인 후 | `packages/iso8211`(쓰기 없음) |
| `s100-gml` | Part 10b 읽기·쓰기 | `GISLibrary/S10XGML` | `S100DatasetGML`, `GmlifileMakeByPugi` 비교 | `packages/gml` |
| `s100-hdf5` | Part 10c 읽기, 격자·시계열(S-102/104/111/413/507) | (확인 중: S-102 test stage) | **`libS100HDF5` 전체** | `packages/hdf5` |
| `s100-portrayal` | Lua(Part 13 호스트 함수), XSLT 엔진, 드로잉 인스트럭션 파서, CSP, 뷰잉 그룹·스케일 필터, S-98 부속서 D | `LuaScriptingReference`, `GISLibrary/ProcessS101`, `host_*` | `S101_CS_*` 23개, XSLT 경로 비교 | `packages/portrayal` |
| `s100-geometry` | 기하 연산, 투영, MBR, 공간 관계(`HostSpatialRelate`) | `GeoMetryLibrary`, `LatLonUtility` | `S100Geometry` 비교 | core 일부 |
| `s100-render-d2d` | Direct2D 백엔드: SVG 심볼 지오메트리·비트맵 캐시, 패턴 브러시, 선 스타일, 텍스트 | `GISLibrary`(그리기 부분) | `S100_SVG_D2D1_DLL` | `packages/render` |
| `s100-sdk` | 파사드: 세션, 레이어 관리, 셀 선택(S-98 E-1), 피킹, 경보, 항로 감시 | `GISLibrary/LayerManager`(1.9K줄) | KHOA `LayerManager`의 경보·커버리지 중 코어에 속하는 것만 | `packages/sdk` |
| `s100-c` | C ABI 래퍼 | 없음 | `DLL_GisLibraryWrapper` 참고 | 해당 없음 |
| `apps/opens100` | 최소 MFC 검증 앱 | `OpenS100` (11K줄) | 없음 | `apps/viewer` |
| (KHOA 저장소) | 완성형 뷰어 | | `GISViewer`, `RoutePlanner`, `SECOMLib`, `AISStream`, `libOpenAPI`, `TideTables` | `apps/viewer-ol` |

원칙:
- 코어 모듈(`s100-core`~`s100-sdk`)은 **MFC, ATL, `d2d1.h`, `afx*.h`, `windows.h`를 include하지 않는다**(지침 18, 전 플랫폼). 문자열은 CONVENTIONS.md대로 내부 `std::string`(UTF-8). `CString`을 코어에서 제거한다(현재 S100Engine 55곳, GISLibrary 325곳).
- 코어 모듈은 UI를 모른다. 에러는 예외 또는 `std::expected` 스타일로 돌려주고 `AfxMessageBox`, `CFileDialog`, `OutputDebugString`을 호출하지 않는다.
- 소유권은 `unique_ptr`/`shared_ptr`로 명시한다(현재 373곳 사용 중, raw `new` 705곳). 코어에서 raw `new`/`delete`를 없앤다.
- 전역 상태(Lua 호스트 statics, 클래스 static 설정값)를 세션 객체로 옮긴다.
- 의존성 방향: core ← iso8211/gml/hdf5 ← portrayal ← sdk ← render-d2d ← apps. 역방향 include 금지.

### 3.2 빌드 시스템

- **CMake로 전환.** vcpkg 포트는 사실상 CMake를 요구한다. `.vcxproj`/`.sln`은 전환 기간 동안 유지하되, 코어 모듈부터 CMake 타깃으로 옮기고 앱은 마지막에.
- 의존성은 전부 vcpkg 매니페스트: pugixml, hdf5, geographiclib, libxml2, libxslt, lua(5.1 호환), openssl, sqlite3, boost-geometry 또는 clipper2, utfcpp. `extlibs/`와 벤더링 제거. 현재 `vcpkg.json`의 이름 `my-geospatial-app`을 `opens100`으로, `builtin-baseline` 추가.
- 컴파일러: `/std:c++20 /W4 /permissive- /utf-8`, 코어는 경고를 에러로. `OpenS100.Common.props`에 넣어 전 프로젝트에 적용.

### 3.3 빌드 속도 (지침 17)

현재 두 저장소 모두 풀 빌드가 매우 느리다. 원인 후보와 대책:

| 원인 | 대책 | 단계 |
|---|---|---|
| 프로젝트마다 Win32/x64/ARM64 × Debug/Release 구성, 사용하지 않는 플랫폼까지 유지 | x64만 남기고 Win32·ARM64 구성 삭제(ARM64는 CMake 전환 후 크로스 빌드로) | Phase 0~1 |
| PCH가 프로젝트마다 제각각, 일부는 PCH 없이 `<afx*.h>`·`<d2d1.h>`·HDF5 헤더를 매 TU마다 포함 | 코어에서 무거운 헤더 제거(MFC·D2D 제거가 곧 빌드 속도 개선), 모듈별 PCH 정리, 전방 선언 | Phase 1 |
| 거대 TU(`S101Cell.cpp` 5.6K줄, KHOA `LayerManager.cpp` 9.7K줄) | 분할 | Phase 1~4 |
| vcpkg가 hdf5·libxslt·openssl·boost를 소스 빌드 | 바이너리 캐시(로컬 `files` + CI `GitHub Actions cache` 또는 NuGet), boost-geometry 같은 헤더 전용 의존성은 필요 범위만 | Phase 0 |
| MSBuild 증분 빌드가 느리고 `-m` 병렬화가 프로젝트 단위 | CMake + Ninja로 전환(파일 단위 병렬), `ccache`/`sccache` | Phase 4 |
| 템플릿 과다 사용 또는 헤더에 구현 | 측정 후(`/d1reportTime`, `-ftime-trace`) 상위 헤더부터 정리 | Phase 1 |

측정부터 한다: Phase 0에서 클린 빌드·증분 빌드 시간을 기록하고(`MSBuild -clp:PerformanceSummary`, `/Bt+`), 이후 단계마다 같은 조건으로 다시 잰다.

### 3.4 공개 API 형태 (초안)

```cpp
namespace s100 {
  FeatureCatalogue   loadFeatureCatalogue(const std::filesystem::path&);
  PortrayalCatalogue loadPortrayalCatalogue(const std::filesystem::path&);
  class CatalogueRegistry;            // (product, version) -> FC/PC, ready/fallback/unresolved

  struct ProductInfo { std::string productId; std::string edition; Encoding enc; };
  ProductInfo detectProduct(const std::filesystem::path&);   // DSID/PRSP, GML namespace, HDF5 attr
  std::unique_ptr<Dataset> openDataset(const std::filesystem::path&, const CatalogueRegistry&);
  void applyUpdates(Dataset&, std::span<const std::filesystem::path>);
  void writeIso8211(const Dataset&, const std::filesystem::path&);
  void writeGml(const Dataset&, const std::filesystem::path&);
  std::vector<Issue> validate(const Dataset&, const FeatureCatalogue&);

  DisplayList portray(const Dataset&, const PortrayalCatalogue&, const ContextParameters&);
  DisplayList mergeAndSort(std::span<const DisplayList>);     // 셀 간 통합 정렬

  ExchangeSet readExchangeSet(const std::filesystem::path&);
  void writeExchangeSet(const ExchangeSet&, const std::filesystem::path&);
}
```

렌더링은 `DisplayList`를 입력으로 받는 별도 모듈이며, 코어 API는 렌더러를 모른다.

## 4. vcpkg 등록 요건과 경로

1. 공개 Git 저장소(충족)와 **태그된 릴리스**(SemVer). 포트는 태그/커밋을 고정한다.
2. **CMake 빌드**와 `install()` 규칙, `find_package(opens100 CONFIG)`용 config export.
3. 모든 의존성이 vcpkg 포트. 벤더링 금지.
4. OSI 라이선스(MPL-2.0, 충족). 포함된 FC/PC 카탈로그(IHO·KHOA 저작물)는 라이선스가 다르므로 **라이브러리 소스와 분리**해 별도 데이터 패키지로.
5. `x64-windows`, `x64-windows-static`, `x64-linux`, `arm64-osx`. 지침 18에 따라 전 플랫폼이 목표이므로 코어는 Windows 전용 API를 쓰지 않는다. 장기적으로 `wasm32-emscripten`(opens100-web과 코어 공유 가능성).
6. `portfile.cmake`, 포트용 `vcpkg.json`, `usage`. vcpkg CI 통과.

권장 경로: 먼저 **자체 Git 레지스트리**(예: `S-100ExpertTeam/vcpkg-registry`)에 포트를 만들어 KHOA 뷰어가 `vcpkg-configuration.json`으로 받아 쓰게 한다. 이것이 "KHOA 뷰어가 OpenS100을 사용한다"의 기술적 형태다. 코어가 안정되고 Linux 빌드가 되면 microsoft/vcpkg에 PR.

## 5. 단계

| 단계 | 내용 | 완료 기준 |
|---|---|---|
| **Phase 0** 선행 정리 | 코드 평가, 메모리·견고성 버그 수정, C++20, `vcpkg.json` 정리, gtest 프로젝트 + GitHub Actions, 개발자 문서 | [Phase 0 작업 로그](2026-11-08-phase0-worklog.md) 전부 체크. x64 Debug/Release 빌드 통과 |
| **Phase 1** 경계 세우기 | 코어 프로젝트에서 MFC·D2D include 제거(컴파일 가드로 측정), `CString` 제거, UI 호출 제거, 전역 상태를 세션 객체로, 의존성 방향 강제 | 코어가 `afx*.h`, `d2d1.h` 없이 컴파일. 테스트가 코어만 링크 |
| **Phase 2** 업스트림 1차 | KHOA 뷰어에서 `libS100HDF5`(S-102/104/111/413), 교환세트 읽기 확장, 8211 쓰기·업데이트 생성, S-98 IC 그리기 모델. 경계(3.1)에 맞춰 재작성하며 가져온다 | [업스트림 계획](2026-11-08-upstream-plan-from-khoa-viewer.md) P1 완료. TDS 전 제품 로딩 테스트 |
| **Phase 3** 역이식 1차 | 카탈로그 레지스트리, 제품 판별, 데이터셋 검증, 디스플레이 리스트 셀 간 통합 정렬, 심볼 비트맵 캐시, 매 프레임 지오메트리 재생성 제거 | [역이식 계획](2026-11-08-backport-plan-from-opens100-web.md) P1 완료. 렌더 시간 측정치 문서화 |
| **Phase 4** 모듈화·CMake | 3.1 표대로 디렉터리·타깃 재편. CMake로 코어 빌드. `S101Cell`을 8211 I/O와 모델로 분리 | `cmake --build`로 코어·테스트 빌드. OpenS100 앱이 CMake 산출물 링크 |
| **Phase 5** API 안정화·자체 레지스트리 | 공개 헤더 정리, C ABI, Doxygen, SemVer 0.x 태그, 자체 vcpkg 레지스트리 포트. **KHOA 뷰어가 이 포트를 링크하기 시작** | KHOA 뷰어가 `find_package(opens100)`로 코어 기능 하나(8211 로딩) 사용 |
| **Phase 6** 공식 등록 | Linux·macOS 빌드 CI, 카탈로그 분리, microsoft/vcpkg PR | 포트 머지. KHOA 뷰어가 공식 포트로 전환 |

Phase 2와 3은 순서를 바꿔도 되지만, **Phase 0 없이 Phase 4를 시작하지 않는다.**

## 6. 결정 사항과 열린 질문

결정:
- 라이브러리 이름 `opens100`, 네임스페이스 `s100`.
- **C++20을 기본 표준으로 한다**(지침 6). VS2026(MSVC v145)에서 안정. `std::span`, `std::format`, concepts, ranges, designated initializers 사용. C++23·모듈은 보류.
- **우선순위는 코어 기능과 성능, UI는 그 다음**(지침 3). `OpenS100` 앱은 검증 수단으로 유지.
- Lua는 Part 13 기준인 5.1 호환 유지.
- **KHOA 뷰어는 vcpkg 포트로 OpenS100을 참조한다**(지침 16). 서브모듈·소스 복사 없음.
- **전 플랫폼 지원이 목표다**(지침 18): Windows, Linux, macOS, 가능하면 WASM. 코어는 MFC·ATL·Direct2D·GDI·Win32 API를 쓰지 않는다. 파일 I/O는 `std::filesystem`/`std::fstream`, 문자열은 UTF-8 `std::string`, 스레드는 `std::thread`/`std::jthread`. Windows 전용 코드는 `s100-render-d2d`와 앱에만.
- **빌드 속도는 별도 작업 흐름으로 관리한다**(지침 17). 아래 3.4절.

열린 질문 (사용자 결정 필요):
- FC/PC 카탈로그 재배포 라이선스(IHO, KHOA 임시 카탈로그).
- `GISLibrary`라는 이름을 유지할지, 3.1의 모듈 이름으로 바꿀지(Phase 4).

## 변경 이력

- 2026-11-08 초안. 처음엔 KHOA 뷰어 저장소에 작성했으나 사용자가 대상은 OpenS100이라고 정정하여 이 저장소로 이동하고 2절(세 저장소 역할)과 업스트림 단계를 추가.
- 2026-11-08 지침 16~18 반영: vcpkg 참조 확정, 빌드 속도 절(3.3) 추가, 전 플랫폼·MFC 제거를 결정 사항으로.
