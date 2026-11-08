# 프로젝트 지침 (소유자 jogm)

사용자가 직접 밝힌 방향과 원칙. 코드·계획 작성 시 이 문서가 다른 기술 문서보다 우선한다. 새 지침이 나오면 날짜와 함께 추가하고, 바뀌면 줄을 긋지 말고 "변경" 항목으로 적는다. 같은 내용의 사본이 KHOA S-100 Viewer 저장소(`S-100SimpleEditor/Document/TechDocs/DIRECTIVES.md`)에도 있다. 둘 중 하나를 고치면 다른 쪽도 맞춘다.

## 방향

1. **OpenS100(이 저장소)은 뷰어가 아니라 라이브러리다.** GEOS, GDAL, libcurl처럼 특정 분야(S-100)의 핵심 C++ 라이브러리가 되어야 하고, vcpkg에 등록하는 것이 목표다. (2026-11-08)
2. **S-100은 S-100 관련 표준 전체를 뜻한다.** Part 1~17과 S-98, S-1xx 제품 사양, S-4xx, 심지어 구버전은 S-57/52/63까지 범위에 포함한다. (2026-11-08)
3. **UI보다 코어 기능과 성능이 중요하다.** 지금과 같은 뷰어 형태는 적합하지 않다. 다만 간단한 UI는 개발할 수 있으면 한다. (2026-11-08)
4. **KHOA S-100 Viewer(`gorogara/S-100SimpleEditor`)는 OpenS100을 사용해서 개발한 뷰어가 되어야 한다.** OpenS100은 KHOA S-100 Viewer에서 오픈소스로 파생됐다. 라이브러리 개선은 OpenS100에서 하고, KHOA 뷰어는 OpenS100과 호환되는 소비자 앱으로 바꾼다. KHOA 뷰어에만 있는 코어 기능은 OpenS100으로 올려보낸다. (2026-11-08)
5. **opens100-web과는 병렬 개발이 불가피하다.** opens100-web은 OpenS100을 참조해 만들었지만 지금은 기능·기술적으로 앞서 있다. 앞선 부분은 OpenS100으로 다시 옮긴다(역이식). 두 저장소의 개념 모델과 모듈 경계를 맞춘다. (2026-11-08)
6. **C++ 표준은 더 최신으로, 단 VS2026에서 안정적으로 지원하는 수준까지.** 결정: 기본 C++20(`/std:c++20`). C++23은 MSVC 지원이 부분적이므로 라이브러리 공개 API에는 쓰지 않는다. 모듈(`import`)은 vcpkg·CMake 생태계가 안정될 때까지 보류. (2026-11-08)
7. 장기적으로 KHOA S-100 Viewer는 OpenS100을 사용하는 뷰어이기 때문에 UI관련된거나 Application을 위한 별도 코드 라이브러리만 남아야 한다. 
8. OpenS100과 KHOA S-100 Viewer는 같은 개발 환경을 사용해야 한다. 되도록 안정된 최신 버전 활용한다. 

## 작업 방식

7. **계획은 항상 문서로 남긴다.** `Documents/TechDocs/`에 마크다운으로 쓰고, 다른 세션이 참조할 수 있게 한다. 완료된 항목은 체크 표시하고 커밋 해시를 적는다. (2026-11-08)
8. **커밋은 적당한 단위로 진행한다.** 논리 단위(한 작업 항목 또는 관련 묶음)마다 커밋한다. **OpenS100 코드 수정은 `dev-jogm` 브랜치에서 한다.** (2026-11-08)
9. **사용자가 말한 지침은 이 문서에 남긴다.** (2026-11-08)
10. **장기 작업(라이브러리화)에 들어가기 전에 선행 정리(Phase 0)를 먼저 한다.** 테스트 없이 구조를 바꾸지 않는다. (2026-11-08)
11. 기존 저장소 규약을 따른다: `CONVENTIONS.md`(내부 문자열 `std::string` UTF-8, `W` 접미 wstring getter), `OpenS100.Common.props`(`/utf-8`), `BUILD.md`.

## 참조 자원

12. **한국 전체 S-101 데이터:** `D:\s100-reference\tds\KOR_TDS\S100_ROOT` (교환세트 구조: `CATALOG.XML`, `CATALOG.SIGN`, `S-101\DATASET_FILES\` 808개 `.000`, `SUPPORT_FILES\`). 개발·테스트 시 참조해도 된다. 저장소에는 넣지 않는다. (2026-11-08)
13. KHOA 테스트 데이터셋과 IHO 레지스트리 FC/PC: `D:\s100-reference\tds\KOR_TDS\KOR_S-1xx\`, `D:\s100-reference\tds\FC_PC\`. (2026-11-07)
14. 웹 구현: `C:\Users\jogm\Desktop\Git\bluemap-github\opens100-web` (github.com/bluemap-github/opens100-web). 역이식 검토 문서가 그 저장소 `docs/plans/opens100-cpp-backport-feasibility.md`에 있다(OpenS100 `55cacf84` 기준).
15. KHOA S-100 Viewer: `C:\Users\jogm\Desktop\Git\S-100SimpleEditor` (비공개, 브랜치 `jogm`). 코드 평가와 뷰어 전환 계획은 그 저장소 `Document/TechDocs/`에 있다.

## 추가 지침 (2026-11-08, 두 번째)

16. **KHOA S-100 Viewer는 vcpkg로 OpenS100을 참조해서 만든다.** 서브모듈·소스 복사가 아니라 vcpkg 포트(처음엔 자체 레지스트리, 나중엔 공식)로 받아 링크한다.
17. **빌드 속도를 최신화한다.** OpenS100과 KHOA S-100 Viewer 모두 빌드가 매우 오래 걸린다. 빌드 시스템 현대화(CMake + Ninja, PCH/유니티 빌드 정리, vcpkg 바이너리 캐시, 불필요한 구성·플랫폼 제거)를 계획에 포함한다.
18. **MFC는 최대한 제거한다.** OpenS100은 vcpkg 위주의 순수 C++ 라이브러리가 되어야 하며, **모든 플랫폼(Windows, Linux, macOS, 가능하면 WASM)에서 동작**해야 한다. Windows 전용 API(MFC, ATL, Direct2D, GDI)는 렌더 백엔드와 앱 계층에만 남긴다.

## 변경 이력

- 2026-11-08 최초 작성. 처음에는 KHOA 뷰어 저장소에서 "그 저장소를 라이브러리화"하는 것으로 적었으나, 사용자가 라이브러리화 대상은 OpenS100이고 KHOA 뷰어는 소비자라고 정정함(항목 4).
- 2026-11-08 항목 16~18 추가 (vcpkg 참조 확정, 빌드 속도, MFC 제거·전 플랫폼).
