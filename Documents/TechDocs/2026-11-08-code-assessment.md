# OpenS100 코드베이스 평가 (2026-11-08)

- 대상: `OpenS100.sln` (`dev-jogm`, 커밋 `6d15077a` 기준). 9개 vcxproj, C++ 약 10만 줄(GISLibrary 61K, OpenS100 앱 11K, PortrayalCatalogue 11K, FeatureCatalog 8.7K, S100Engine 5.2K, GeoMetryLibrary 3.9K, LuaScriptingReference 1.4K). 835 커밋, 2021-12 ~ 2026-11.
- 비교 대상: KHOA S-100 Viewer(`S-100SimpleEditor`, 이하 KHOA). 그쪽 평가는 그 저장소 `Document/TechDocs/2026-11-08-code-assessment.md`.
- 범례: `[ ]`는 처리 여부. 고치면 `[x]`와 커밋 해시.

## 1. 총평

| 영역 | 점수 (10) | 한 줄 요약 |
|---|---|---|
| 기능 범위 | 5 | S-101 8211/GML, 전 제품 FC/PC 27종 로딩, 버전 인식 카탈로그 관리, S-102 HDF5(초기), 교환세트 읽기, 8211 쓰기, S101Creator 편집 API. **S-104/111/413 렌더링·S-98 IC 그리기·업데이트 생성 없음** |
| 아키텍처 | 6 | KHOA보다 명확히 신형: 범용 `S100Layer`/`S100SpatialObject`, `CatalogManager`(제품+버전), 별도 `PortrayalCatalogue` 프로젝트, Part 9 드로잉 인스트럭션 클래스, S-100 Ed5 모델(GM_*, CI_*, 서명·Permit). 그러나 코어에 MFC·D2D 결합이 남아 있음 |
| 코드 품질 | 5 | 스마트 포인터 284곳(KHOA 6곳), UTF-8 규약 채택(W 접미 getter 176개). 그래도 raw `new` 631곳이 다수. `S101Cell.cpp` 5,620줄 |
| 엔지니어링 관행 | 3 | 자동 테스트 0, CI 없음. 문서는 KHOA보다 낫다(BUILD.md, CONVENTIONS.md, DOXYGEN.md, Common.props) |

KHOA 뷰어의 **정리된 포크**다. 완전 재작성은 아니며(공통 파일에서 정규화 기준 30~40% 라인 공유), 설계는 OpenS100 쪽이 새롭다. 라이브러리화의 출발점으로 KHOA보다 적합하다. 다만 "기능은 KHOA, 구조는 OpenS100"인 상태라 KHOA 기능을 가져오면서 구조를 지켜야 한다.

## 2. 잘된 점

- `GISLibrary/CatalogManager`: 제품과 **버전**으로 FC/PC 선택. opens100-web의 `CatalogueRegistry`와 같은 방향(ready/fallback/unresolved 상태 모델은 아직 없음).
- `GISLibrary/S10XGML.cpp`(1,735줄): 제품 무관 S-1xx GML 읽기. `S100Layer.cpp:51`에서 확장자·제품으로 분기.
- `PortrayalCatalogue` 프로젝트 분리, CSP Context/ContextParameter, PC 설정 UI(최근 커밋).
- `S101Creator.cpp`(1,234줄): 피처·정보 레코드 추가/삭제, 속성, 기하 API. `S101Cell::Save`/`SaveAsENC`/`SaveAsGML`(`S101Cell.cpp:3438`)로 8211·GML 쓰기.
- S-100 Ed5 모델 클래스: `GM_*`, `CI_*`, `S100_SE_*`(서명), Permit, `ExchangeCatalogue`.
- `CONVENTIONS.md`와 `OpenS100.Common.props`(`/utf-8`)로 인코딩 문제를 빌드 수준에서 차단. KHOA에는 없다.
- `FeatureCatalog` 프로젝트는 MFC 없이 컴파일된다(`UseOfMfc=false`). 코어 분리의 첫 사례.
- `LayerManager.cpp` 1,911줄(KHOA 9,749줄).

## 3. 문제점

### 3.1 견고성 (KHOA와 공유하는 버그)

- [ ] **이중 delete 가능.** `GISLibrary/S101Cell.cpp:227, 248` `delete updateInformation` 후 nullptr 미대입. KHOA와 동일 패턴. 호출 경로(생성자·소멸자·재오픈) 확인 후 수정.
- [ ] **8211 레코드 루프.** `S101Cell.cpp:418, 572` `subFieldCount < 1`이면 포인터를 전진시키지 않고 `continue`. 손상 파일에서 무한 루프 가능. 레코드 길이·버퍼 경계 검증 없음.
- [ ] pugi `xml_parse_result`를 받아 놓고 확인하지 않는 곳 점검: `S100Engine/S100PCLineStylesReader.cpp:61`, `GISLibrary/ExchangeCatalogue.cpp:27`, `IC_InteroperabilityCatalogue.cpp:9`, `LayerManager.cpp:1715, 1781`.
- [ ] XSLT 경로가 `..\TEMP\input.xml` 디스크 왕복(`LayerManager.cpp:1314`).

### 3.2 아키텍처

- [ ] **코어에 MFC·D2D 결합.** `PortrayalCatalogue.h`에 `ID2D1Factory`/`ID2D1StrokeStyle`. `PortrayalCatalogue`, `GeoMetryLibrary`가 `UseOfMfc=Dynamic`. `S100Engine`에 `CString` 50곳, D2D/GDI 약 250곳(렌더러 역할이라 일부는 정당). `GISLibrary` 레이어·공간객체 API가 `CString`/`HDC`를 받고 생성자가 `D2D1Resources*`를 받음(`S100Layer::Open`). 모델과 렌더링이 분리돼 있지 않다.
- [ ] `S101Cell.cpp` 5,620줄: 8211 I/O, 모델, 업데이트 적용(2426~3243줄), 쓰기, GML 출력이 한 클래스.
- [ ] 픽 리포트가 라이브러리가 아니라 뷰(`OpenS100View.cpp:1191-1375`)에 있음.
- [ ] `IC_*` 모델(약 530줄)은 있으나 사용처가 주석 처리된 한 줄(`OpenS100View.cpp:119`)뿐.
- [ ] `S100H5.cpp` 67줄: 루트 그룹만 읽는 스텁. S-104/111/413 렌더링 없음.
- [ ] `S101Creator`의 모든 `Add*`가 `std::string`/`std::wstring` 두 벌(규약상 의도된 중복이나 템플릿 또는 변환 레이어로 줄일 수 있음).
- [ ] `S100Geometry/` 폴더가 있으나 솔루션에 없음(죽은 폴더?).

### 3.3 엔지니어링 관행

- [ ] **자동 테스트 0.** `OpenS100/TestGISLibrary.cpp`는 앱 내 수동 코드. CI 없음(`.github` 없음).
- [ ] Doxygen 설정은 있으나 문서 주석이 있는 헤더는 2개.
- [ ] `vcpkg.json` 이름이 `my-geospatial-app`, 베이스라인 없음, 버전 고정 없음. `extlibs/Lua`에 lua51 DLL/LIB가 git에 추적됨(vcpkg `lua` 포트로 대체 가능).
- [ ] 툴셋 v143(VS2022), C++17. BUILD.md도 VS2022 기준. 지침 6(C++20, VS2026)과 불일치.
- [ ] 경고 설정 Level3, `/permissive-`·경고-에러 미설정.
- [ ] 커밋 메시지 대부분 한 단어("mac", "local", "host"). `dev-jogm`이 `main`보다 41커밋 앞. 원격 전용 브랜치 `makeUpdateFile`(업데이트 생성 기능)이 병합되지 않음.
- [ ] 추적 중인 대용량: `Documents/OpenS100.eap` 5.4MB, `ProgramData/PC/S413_Portrayal/S413_Portrayal.zip`, Lua DLL/LIB, 폰트. FC XML 합계 수 MB(S-101 2MB).

## 4. 개선 우선순위

Phase 0([작업 로그](2026-11-08-phase0-worklog.md))에서 1~5, Phase 1 이후에 나머지.

1. 3.1의 공유 버그 수정 (KHOA 쪽과 같은 패치).
2. C++20 + VS2026 v145 전환, `Common.props`에 `/W4 /permissive-` 추가.
3. `vcpkg.json` 정리(이름·베이스라인), Lua를 vcpkg 포트로.
4. gtest 프로젝트와 GitHub Actions(수동 트리거 → 바이너리 캐시 후 push 트리거).
5. `makeUpdateFile` 브랜치 검토·병합.
6. 코어에서 MFC·D2D 제거(Phase 1). `PortrayalCatalogue.h`의 D2D 타입을 렌더 모듈로, `S100Layer::Open`의 `D2D1Resources*` 의존 제거.
7. `S101Cell` 분할: 8211 리더/라이터, 모델, 업데이트 적용.
8. KHOA 업스트림(Phase 2)과 opens100-web 역이식(Phase 3)은 각 계획 문서.
