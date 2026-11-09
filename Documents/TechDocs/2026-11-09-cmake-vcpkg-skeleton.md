# CMake · vcpkg 뼈대: 끝에서 끝까지 먼저 뚫기 (2026-11-09)

- 상태: 진행 중. 로드맵 [Phase 4·5](2026-11-08-roadmap-library-first.md)의 선행 조각이다.
- 지침: 16(KHOA 뷰어는 vcpkg로 OpenS100을 참조), 18(전 플랫폼), 17(빌드 속도).

## 왜 지금 하나

로드맵은 CMake 전환(Phase 4)과 vcpkg 포트(Phase 5)를 업스트림·역이식 뒤에 둔다. 그런데 "뷰어가 vcpkg로
OpenS100을 쓴다"는 최종 형태는 구성 → 빌드 → 설치 → `find_package` → 포트 → 외부 소비자라는 긴 사슬이고,
이 사슬은 모듈이 하나뿐이어도 똑같이 필요하다. 그래서 **이미 플랫폼 중립인 모듈만으로 사슬 전체를 먼저
세우고**, 나머지 모듈은 Windows 의존이 빠지는 대로 같은 틀에 올린다. Phase 2·3의 내용 작업과 병행한다.

`.sln`/`.vcxproj`는 그대로 전체 빌드다. CMake는 같은 소스 파일을 빌드하며(디렉터리의 `*.cpp` 전부, vcxproj
목록과 동일함을 확인), 두 빌드가 어긋나지 않게 한다.

## 지금 있는 것

| 파일 | 역할 |
|---|---|
| `CMakeLists.txt` | 모듈별 정적 라이브러리. `opens100_add_module(<타깃> <디렉터리> DEPENDS …)` |
| `cmake/opens100Config.cmake.in` | `find_package(opens100 CONFIG)`용 설정. 의존성(pugixml, GeographicLib, utf8cpp)을 다시 찾는다 |
| `ports/opens100/` | 오버레이 포트. 체크아웃 안에 있으면 그 체크아웃을 빌드하고, 다른 곳(레지스트리)에 복사되면 GitHub의 고정 커밋을 받는다 |
| `examples/consumer/` | 별도 프로젝트. vcpkg 매니페스트 + `overlay-ports`로 opens100을 받아 FC를 읽는다 |

타깃 (네임스페이스 `opens100::`):

| 타깃 | 소스 | 의존 |
|---|---|---|
| `latlon` | `LatLonUtility/` | GeographicLib |
| `geometry` | `GeoMetryLibrary/` | latlon |
| `featurecatalog` | `FeatureCatalog/` | latlon, pugixml, utf8cpp |

헤더는 `<prefix>/include/opens100/<모듈 디렉터리>/<헤더>`로 설치된다. 소스 트리와 같은 상대 배치라서
모듈 사이의 `"../LatLonUtility/x.h"` include가 설치 후에도 그대로 동작하고, 소비자는
`#include <FeatureCatalog/FeatureCatalogue.h>`로 쓴다. 최종 헤더 경로(`s100/…`)는 Phase 4의 모듈 재편 때 정한다.

## 검증 (2026-11-09, x64-windows)

```
:: 1) 라이브러리만
cmake -S . -B build/cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ^
      -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake ^
      -DVCPKG_INSTALLED_DIR=%CD%\vcpkg_installed\x64-windows -DOPENS100_WARNINGS_AS_ERRORS=ON
cmake --build build/cmake && cmake --install build/cmake --prefix build/install

:: 2) 포트를 거쳐 외부 소비자까지
cmake -S examples/consumer -B build/consumer -G Ninja -DCMAKE_BUILD_TYPE=Release ^
      -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build/consumer
build\consumer\opens100_consumer.exe ProgramData\FC\S-101_FC.xml
```

결과: 86개 번역 단위가 `/W4 /WX /permissive-`로 통과, 포트 빌드 19초(디버그+릴리스), 소비자 출력
`product: S-101 / feature types: 190`. VS 개발자 명령 프롬프트(vcvars64)와 VS에 포함된 CMake 4.x·Ninja를 쓴다.
`-DVCPKG_INSTALLED_DIR`은 MSBuild가 이미 설치해 둔 패키지를 재사용하려는 것이다(없으면 매니페스트 전체를 새로 설치).

주의:
- 체크아웃을 직접 빌드하는 포트는 vcpkg가 소스 변경을 알지 못한다. 라이브러리를 고친 뒤에는
  `vcpkg remove opens100`(매니페스트 모드에서는 `vcpkg_installed` 삭제)이나 `port-version` 증가로 다시 빌드시킨다.
- 포트는 정적 라이브러리만 만든다(`vcpkg_check_linkage(ONLY_STATIC_LIBRARY)`). DLL 내보내기 매크로는 공개 API가 정해진 뒤.

## 이 과정에서 고친 것

- `LatLonUtility`도 `/W4` + 경고=에러(헤더의 무조건 `#define _CRT_SECURE_NO_WARNINGS`, 미사용 지역 변수,
  `char`에 `towlower`를 쓰던 `To_Lowercase`).
- `FeatureCatalogue::SetVersionNumber`가 원문을 저장하지 않아 `GetVersionNumber()`가 항상 빈 문자열이던 버그.
  소비자 예제 출력으로 드러났다.

## 다음

- [x] KHOA 뷰어가 `overlay-ports`로 이 포트를 받아 쓴다 (2026-11-09, 뷰어 커밋 633437fa). 첫 대상은 FC가 아니라 **측지 함수(`opens100::latlon`)**다. 뷰어에는 `FeatureCatalog`·`GeoMetryLibrary`·`LatLonUtility`의 포크가 있고 클래스 이름이 같아서, 포크를 그대로 둔 채 같은 모듈을 링크할 수 없다. 그래서 (1) 뷰어 포크에만 있던 함수를 OpenS100으로 올리고(업스트림 U0, 85f9c919), (2) 포트에 기능 선택을 넣어 뷰어가 기본 패키지(`latlon`)만 받게 하고(4899a312), (3) 뷰어의 `LatLonUtility.h`를 OpenS100 헤더를 넘겨주는 얇은 헤더로 바꾸고 중복 구현 17개를 지웠다. 뷰어 Debug x64 빌드와 실행(셀 로드)을 확인했다. 같은 순서(올리기 → 기능으로 분리 → 포크 제거)로 `GeoMetryLibrary`, `FeatureCatalog`를 진행한다.
- [x] `GeoMetryLibrary` 수렴: **하지 않기로 했다** (2026-11-09 조사). 뷰어 포크의 `Scaler`(CRect, D2D 행렬, 함수 포인터)와 `Geometry`(HDC/CDC 그리기 가상 함수, POINT 도우미)는 MFC·Direct2D에 묶여 있고, 뷰어 엔진을 통째로 교체할 때 사라질 코드다. 뷰어는 포트 기능 `geometry`를 켜지 않는다.
- [x] `FeatureCatalog` 수렴 (2026-11-09, 6058305f, 뷰어 브랜치 `jogm-opens100-fc` 커밋 502bd0e9). 뷰어 포크에만 있던 것을 올렸다: 경로 생성자, `isProductId`, 연관 바인딩 조회 `FindFeatureBinding`/`FindInformationBinding`(대상의 상위 타입까지 탐색)와 그 역방향 `AdmittedFeatureTypes`/`AdmittedInformationTypes`. 뷰어는 `opens100[feature-catalogue]`를 받고 포크(131개 파일)를 지웠다. 뷰어 쪽 포팅 규칙과 검증 범위는 뷰어 저장소 `Document/TechDocs/2026-11-08-khoa-viewer-plan.md` 3절. 얻은 교훈:
  - 오버레이 포트 뒤의 소스를 고치면 `port-version`을 올려야 한다. `vcpkg_installed`만 지우면 바이너리 캐시에서 예전 빌드가 복원된다(이번에 1 → 3). 소비자 쪽 MSBuild 통합은 `.msbuildstamp-<triplet>.stamp`도 지워야 다시 설치한다.
  - API의 `W` 쌍(`GetNameW()` 등)이 있어서 wide 문자열 소비자의 포팅이 기계적으로 됐다(346개 오류 중 110여 곳). 반대로 `GetAttributeCodeAsWstring()`처럼 이름과 달리 `std::string`을 반환하는 접근자가 남아 있다. 공개 API 정리(Phase 4) 때 없앤다.
  - `LuaScriptingReference`는 바인딩마다 대상 타입 수만큼 같은 Lua 바인딩을 중복 생성한다(`CreateFeatureBinding(ls, fb, i)`가 `i`를 쓰지 않음). 뷰어 쪽 포팅에서는 바인딩당 한 번만 만든다. 이쪽도 고칠 것.
- [ ] `portrayalcatalogue` 타깃: Phase 1의 2-3c(`S100PCManager` 분리)가 선행.
- [ ] `iso8211`·`gml` 타깃: `GISLibrary`에서 8211/GML 모델과 리더를 렌더·Win32 코드와 떼어 내야 한다(Phase 4의 `S101Cell` 분리).
- [ ] CMake로 테스트 빌드(`OPENS100_BUILD_TESTS`), GitHub Actions에 CMake 잡 추가(수동 실행 유지).
- [ ] `x64-linux` 구성 시도. 지금 세 모듈에서 Windows 전용으로 남은 것은 `ENCCommon.cpp`의 로캘 이름과 `LatLonUtility/Logger.h`.
- [ ] 자체 Git 레지스트리(`S-100ExpertTeam/vcpkg-registry`)에 포트 게시: 첫 태그(0.1.0)와 함께.
