# Phase 0 작업 로그 (OpenS100): 라이브러리화 선행 정리 (2026-11-08 시작)

- 상위 문서: [로드맵](2026-11-08-roadmap-library-first.md) 5절 Phase 0. 근거: [코드 평가](2026-11-08-code-assessment.md) 4절.
- 규칙: 항목을 끝내면 `[x]`로 바꾸고 커밋 해시를 적는다. 다른 세션은 미완료 항목부터 본다. **작업 브랜치 `dev-jogm`.**
- 환경(2026-11-08 확인): VS 18(2026) Community에 MSVC 14.44(v143)와 14.51(v145) 설치. vcpkg는 `C:\vcpkg`, HEAD `6cf5a3612342c3cf0d7dee5cf6687d0882ec6663`(2026-11-07). 매니페스트 모드, 트리플릿 `x64-windows`, `vcpkg_installed/`에 설치됨.
- 빌드 확인: `"C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" OpenS100.sln -p:Configuration=Debug -p:Platform=x64 -m` (앱이 실행 중이면 `-p:OutDir=`로 분리).

## 작업 항목

### A. 견고성 버그 (KHOA 뷰어와 공유, 그쪽도 같은 패치)

- [x] A1. `GISLibrary/S101Cell.cpp` `UpdateRemoveAll`(:223)/`RemoveAll`(:242): `delete updateInformation` 후 `nullptr` 대입. (953739aa)
- [x] A2. `S101Cell.cpp:418, 572` 8211 레코드 루프: `subFieldCount < 1`이면 `m_recordLength`만큼 전진, 0이면 중단. `curRecordAddress + m_recordLength > endOfBuf + 1`이면 중단. 종결자 불일치는 로그. (953739aa)
- [x] A3. pugi `xml_parse_result` 미확인 5곳(`S100PCLineStylesReader.cpp:61`, `ExchangeCatalogue.cpp:27`, `IC_InteroperabilityCatalogue.cpp:9`, `LayerManager.cpp:1715, 1781`) 실패 시 false/빈 결과 반환. (953739aa)

### B. 툴체인 (지침 6)

- [x] B1. 전 vcxproj `PlatformToolset` v143 → v145, `LanguageStandard` stdcpp17 → stdcpp20 (9개 프로젝트, 54 구성). 기준 빌드(v143)는 VS2026에 v143용 MFC가 없어 MFC 프로젝트가 MSB8041로 실패했으므로 v145 전환은 필수였음. (6b095ed1)
- [~] B2. (6b095ed1, 임시 조치) `/std:c++20`이 `/permissive-`를 함축하므로 전 프로젝트가 엄격 모드가 됐고 GISLibrary에서 적합성 오류 약 180건(C2664 94: 문자열 리터럴→`char*`, C2665 58: 비상수 참조에 임시 바인딩, C2440 11: `CString`→`std::wstring` 암시 변환 등). **임시 조치:** `Common.props`에 `ConformanceMode=false`를 두고, 이미 정리한 FeatureCatalog·PortrayalCatalogue는 vcxproj에서 `ConformanceMode=true`로 고정. GISLibrary·S100Engine·OpenS100 앱의 적합성 정리는 **Phase 1 항목**. `/W4`는 프로젝트별 Level3 설정이 우선하므로 props로 못 올림(프로젝트별 수정 필요).
- [x] B3. `BUILD.md`를 VS2026/v145/C++20 기준으로 갱신(명령줄 빌드·테스트 절 추가). CMake 3.29 제약은 미확인(그대로 둠). (6b095ed1)

### B'. 빌드 속도 기준선 (지침 17)

- [x] B4. 클린 빌드와 증분 빌드(한 .cpp 수정) 시간을 x64 Debug에서 측정해 기록(2026-11-08, 아래 표).
- [x] B5. ~~Win32·ARM64 구성 제거~~ **철회**(지침 19). 구성은 유지하고 CI 매트릭스로 확인. 테스트 프로젝트는 x64만 있으므로 sln에서 x86/ARM64 솔루션 구성에 대한 `Build.0` 매핑만 제거(해당 구성에서 빌드 생략).
- [ ] B6. 로컬 vcpkg 바이너리 캐시 사용 확인(`VCPKG_BINARY_SOURCES`, BUILD.md 5절).

### C. 의존성

- [x] C1. `vcpkg.json`: name `opens100`, description, `builtin-baseline` = `6cf5a3612342c3cf0d7dee5cf6687d0882ec6663`, `gtest` 추가. (93aec60c)
- [ ] C2. (Phase 1로 보류) `extlibs/Lua`의 lua51 DLL/LIB를 vcpkg `lua` 포트로 대체. Part 13은 Lua 5.1 기준이므로 포트 버전 확인 필요.

### D. 테스트·CI

- [x] D1. `Tests/OpenS100Tests/` gtest 콘솔 프로젝트. GISLibrary·FeatureCatalog·PortrayalCatalogue 링크. 첫 테스트: (a) `FeatureCatalogue` 로딩(`ProgramData/FC/S-101_FC.xml`), (b) `S101Cell` 8211 열기(`D:\s100-reference\tds\KOR_TDS\S100_ROOT\S-101\DATASET_FILES\` 중 하나. 파일이 없으면 SKIP), (c) 손상 8211(잘린 버퍼)에서 A2가 중단되는지, (d) `ExchangeCatalogue` 파싱(`KOR_TDS\S100_ROOT\CATALOG.XML`, 없으면 SKIP). (93aec60c)
- [x] D2. 솔루션에 테스트 프로젝트 추가(`04. TEST` 폴더). (93aec60c)
- [x] D3. (결정 20에 따라 `workflow_dispatch` 전용으로 변경) `.github/workflows/build.yml`: `workflow_dispatch` + push(dev-jogm, main). windows-latest, vcpkg 바이너리 캐시(GitHub Actions cache), MSBuild x64 Release, 테스트 실행. 첫 실행은 hdf5·libxslt 소스 빌드로 오래 걸림(수십 분). 캐시가 쌓이면 push 트리거로. (93aec60c)

### E. 정리

- [ ] E1. `origin/makeUpdateFile` 브랜치 내용 확인(업데이트 생성). 병합 가능하면 `dev-jogm`에 병합. 충돌 크면 업스트림 U2로.
- [ ] E2. `S100Geometry/` 폴더(솔루션 미포함) 처리: 삭제 또는 포함.
- [x] E3. `README.md`에 "라이브러리 지향" 재정의 한 단락과 TechDocs 링크. (6b095ed1)

### F. 검증

- [x] F1. `OpenS100.sln` x64 Debug 빌드 통과 (v145, C++20). 2026-11-08.
- [x] F2. 테스트 전부 통과. 9/9 (2026-11-08). `Debug_x64\OpenS100Tests.exe`
- [ ] F3. 앱으로 KOR_TDS S-101 셀 하나 열어 렌더 확인.

## C++20 전환에서 고친 것 (기록)

| 파일 | 문제 | 조치 |
|---|---|---|
| `FeatureCatalog/SpatialPrimitiveType.*` | `StringToSpatialPrimitiveType(std::string&)`에 임시 전달 | `const std::string&` |
| `PortrayalCatalogue/AlertPrioritySelection.*` | 멤버 이름이 키워드 `default` | `isDefault` |
| `PortrayalCatalogue/S100_Element.h` | 포인터 멤버를 `= false`로 초기화 | `= nullptr` |
| `PortrayalCatalogue/Enum_RuleType.*`, `S100_SymbolFill.*` | 비상수 참조 매개변수에 임시 전달 | `const&` (SymbolFill은 `fullPath` 지역 변수) |
| `PortrayalCatalogue/S100_DisplayList.cpp`, `S100_LineStyle.cpp`, `S100_LineSymbol.cpp`, `OpenS100/DATASET_SUPPORTdlg.cpp`, `FC_PCSelectdlg.cpp` | MSVC 확장 `for each` | 범위 기반 for |
| `PortrayalCatalogue/ViewingGroup.cpp` | `GetContents(pugi::xml_node&)`에 임시 전달 | 지역 변수로 바인딩 |
| `GISLibrary/CI_Contact.cpp` | 변수명 `or`(대체 토큰) | `onlineResource` |
| `LuaScriptingReference/point.h`, `spatial_association.h`, `multipoint.h`, `surface.h` | `X() = default;` 때문에 C++20에서 집합체 자격 상실 → `x = {a, b}` 컴파일 오류 | 선언 제거 |
| `GISLibrary/SPoint.*`, `GeoMetryLibrary/GeoPoint.*` | 비상수 `operator==` → C++20 재작성 후보 모호성 | `const` 멤버, `const&` 매개변수 |

## 테스트에서 드러난 결합 문제 (Phase 1 입력)

- `S101Cell::Open()`은 파싱 뒤 `MakeFullSpatialData()` 등에서 D2D 리소스를 필요로 해서 `D2D1Resources* = nullptr`로 열면 접근 위반이 난다. 즉 **데이터셋을 여는 데 렌더러가 필요하다.** 테스트는 `Read8211()`(파서)만 호출한다. 코어에서 D2D를 분리하는 것이 Phase 1의 첫 과제.
- `LuaScriptingReference.dll`이 `extlibs\Luaind\lua51.dll`을 요구하지만 어떤 프로젝트도 OutDir로 복사하지 않는다(앱은 아마 VS 디버그 환경 PATH로 동작). 테스트 프로젝트에 post-build 복사를 넣었고, 근본적으로는 vcpkg `lua` 포트로 대체(C2).

## 빌드 시간 기록

| 날짜 | 조건 | 클린 빌드 | 증분(1 TU) | 비고 |
|---|---|---|---|---|
| 2026-11-08 | 개발 PC, v145, C++20, x64 Debug, `MSBuild -m -t:Rebuild`, vcpkg 캐시 적중 | **117s** (벽시계) | **4s** (`GISLibrary/S10XGML.cpp` touch) | 프로젝트별 포함 시간: OpenS100 115.6s, OpenS100Tests 98.6s, GISLibrary 83.2s, PortrayalCatalogue 28.9s, LuaScriptingReference 15.4s, S100Engine 12.2s, FeatureCatalog 11.9s, GeoMetryLibrary 10.1s. 임계 경로는 GISLibrary(61K줄, 단일 프로젝트라 파일 병렬화만 가능). 커밋 93aec60c 기준 |

## 커밋 기록

| 커밋 | 항목 | 내용 |
|---|---|---|
| 71c5e1e5 | 문서 | TechDocs 생성 |
| 953739aa | A1~A3 | 8211 루프 가드, 이중 delete, pugi 파싱 결과 확인 |
| 6b095ed1 | B1~B3, E3 | VS2026 v145 + C++20, 적합성 수정, Common.props ConformanceMode 임시 완화, BUILD.md/README |
| 93aec60c | C1, D1~D3 | gtest 프로젝트·솔루션 폴더, vcpkg.json 정리(gtest 추가), GitHub Actions |

## 다른 세션을 위한 메모

- 이 저장소는 `Common.props`의 `/utf-8` 덕분에 인코딩 문제가 적지만, 일부 파일은 여전히 CP949일 수 있다. 편집 전 `file` 명령으로 확인하고 필요하면 바이트 단위 치환.
- 테스트 데이터는 저장소 밖(`D:\s100-reference\tds`)에 있다. 테스트는 환경 변수 `S100_TDS_ROOT`(기본값 `D:\s100-reference\tds`)로 경로를 받고, 없으면 `GTEST_SKIP()`.
