# Phase 0 작업 로그 (OpenS100): 라이브러리화 선행 정리 (2026-11-08 시작)

- 상위 문서: [로드맵](2026-11-08-roadmap-library-first.md) 5절 Phase 0. 근거: [코드 평가](2026-11-08-code-assessment.md) 4절.
- 규칙: 항목을 끝내면 `[x]`로 바꾸고 커밋 해시를 적는다. 다른 세션은 미완료 항목부터 본다. **작업 브랜치 `dev-jogm`.**
- 환경(2026-11-08 확인): VS 18(2026) Community에 MSVC 14.44(v143)와 14.51(v145) 설치. vcpkg는 `C:\vcpkg`, HEAD `6cf5a3612342c3cf0d7dee5cf6687d0882ec6663`(2026-11-07). 매니페스트 모드, 트리플릿 `x64-windows`, `vcpkg_installed/`에 설치됨.
- 빌드 확인: `"C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" OpenS100.sln -p:Configuration=Debug -p:Platform=x64 -m` (앱이 실행 중이면 `-p:OutDir=`로 분리).

## 작업 항목

### A. 견고성 버그 (KHOA 뷰어와 공유, 그쪽도 같은 패치)

- [ ] A1. `GISLibrary/S101Cell.cpp` `UpdateRemoveAll`(:223)/`RemoveAll`(:242): `delete updateInformation` 후 `nullptr` 대입.
- [ ] A2. `S101Cell.cpp:418, 572` 8211 레코드 루프: `subFieldCount < 1`이면 `m_recordLength`만큼 전진, 0이면 중단. `curRecordAddress + m_recordLength > endOfBuf + 1`이면 중단. 종결자 불일치는 로그.
- [ ] A3. pugi `xml_parse_result` 미확인 5곳(`S100PCLineStylesReader.cpp:61`, `ExchangeCatalogue.cpp:27`, `IC_InteroperabilityCatalogue.cpp:9`, `LayerManager.cpp:1715, 1781`) 실패 시 false/빈 결과 반환.

### B. 툴체인 (지침 6)

- [ ] B1. 전 vcxproj `PlatformToolset` v143 → v145, `LanguageStandard` stdcpp17 → stdcpp20 (9개 프로젝트, 54 구성).
- [ ] B2. `OpenS100.Common.props`에 `/W4 /permissive-`(ConformanceMode) 추가. 경고-에러는 코어 프로젝트가 정리된 뒤(Phase 1).
- [ ] B3. `BUILD.md`를 VS2026/v145/C++20 기준으로 갱신. CMake 3.29 제약이 여전한지 확인.

### C. 의존성

- [ ] C1. `vcpkg.json`: name `opens100`, description, `builtin-baseline` = `6cf5a3612342c3cf0d7dee5cf6687d0882ec6663`, `gtest` 추가.
- [ ] C2. (Phase 1로 보류) `extlibs/Lua`의 lua51 DLL/LIB를 vcpkg `lua` 포트로 대체. Part 13은 Lua 5.1 기준이므로 포트 버전 확인 필요.

### D. 테스트·CI

- [ ] D1. `Tests/OpenS100Tests/` gtest 콘솔 프로젝트. GISLibrary·FeatureCatalog·PortrayalCatalogue 링크. 첫 테스트: (a) `FeatureCatalogue` 로딩(`ProgramData/FC/S-101_FC.xml`), (b) `S101Cell` 8211 열기(`D:\s100-reference\tds\KOR_TDS\S100_ROOT\S-101\DATASET_FILES\` 중 하나. 파일이 없으면 SKIP), (c) 손상 8211(잘린 버퍼)에서 A2가 중단되는지, (d) `ExchangeCatalogue` 파싱(`KOR_TDS\S100_ROOT\CATALOG.XML`, 없으면 SKIP).
- [ ] D2. 솔루션에 테스트 프로젝트 추가(`04. TEST` 폴더).
- [ ] D3. `.github/workflows/build.yml`: `workflow_dispatch` + push(dev-jogm, main). windows-latest, vcpkg 바이너리 캐시(GitHub Actions cache), MSBuild x64 Release, 테스트 실행. 첫 실행은 hdf5·libxslt 소스 빌드로 오래 걸림(수십 분). 캐시가 쌓이면 push 트리거로.

### E. 정리

- [ ] E1. `origin/makeUpdateFile` 브랜치 내용 확인(업데이트 생성). 병합 가능하면 `dev-jogm`에 병합. 충돌 크면 업스트림 U2로.
- [ ] E2. `S100Geometry/` 폴더(솔루션 미포함) 처리: 삭제 또는 포함.
- [ ] E3. `README.md`에 "라이브러리 지향" 재정의 한 단락과 TechDocs 링크.

### F. 검증

- [ ] F1. `OpenS100.sln` x64 Debug 빌드 통과 (v145, C++20).
- [ ] F2. 테스트 전부 통과.
- [ ] F3. 앱으로 KOR_TDS S-101 셀 하나 열어 렌더 확인.

## 커밋 기록

| 커밋 | 항목 | 내용 |
|---|---|---|
| | 문서 | TechDocs 생성 |

## 다른 세션을 위한 메모

- 이 저장소는 `Common.props`의 `/utf-8` 덕분에 인코딩 문제가 적지만, 일부 파일은 여전히 CP949일 수 있다. 편집 전 `file` 명령으로 확인하고 필요하면 바이트 단위 치환.
- 테스트 데이터는 저장소 밖(`D:\s100-reference\tds`)에 있다. 테스트는 환경 변수 `S100_TDS_ROOT`(기본값 `D:\s100-reference\tds`)로 경로를 받고, 없으면 `GTEST_SKIP()`.
