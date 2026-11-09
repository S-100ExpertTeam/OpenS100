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

- [ ] KHOA 뷰어(MSBuild + vcpkg 매니페스트)에서 `overlay-ports`로 이 포트를 받아 FC 로딩 한 곳을 OpenS100 것으로 교체.
      로드맵 Phase 5의 완료 기준("뷰어가 `find_package(opens100)`로 코어 기능 하나 사용")의 축소판이다.
- [ ] `portrayalcatalogue` 타깃: Phase 1의 2-3c(`S100PCManager` 분리)가 선행.
- [ ] `iso8211`·`gml` 타깃: `GISLibrary`에서 8211/GML 모델과 리더를 렌더·Win32 코드와 떼어 내야 한다(Phase 4의 `S101Cell` 분리).
- [ ] CMake로 테스트 빌드(`OPENS100_BUILD_TESTS`), GitHub Actions에 CMake 잡 추가(수동 실행 유지).
- [ ] `x64-linux` 구성 시도. 지금 세 모듈에서 Windows 전용으로 남은 것은 `ENCCommon.cpp`의 로캘 이름과 `LatLonUtility/Logger.h`.
- [ ] 자체 Git 레지스트리(`S-100ExpertTeam/vcpkg-registry`)에 포트 게시: 첫 태그(0.1.0)와 함께.
