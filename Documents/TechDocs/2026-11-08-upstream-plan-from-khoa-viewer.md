# KHOA S-100 Viewer → OpenS100 업스트림 계획 (2026-11-08)

- 상태: 초안. 상위 문서: [로드맵](2026-11-08-roadmap-library-first.md) Phase 2. 지침 4.
- 원본 저장소: `C:\Users\jogm\Desktop\Git\S-100SimpleEditor` (비공개, 브랜치 `jogm`). 그쪽 관점의 계획은 그 저장소 `Document/TechDocs/2026-11-08-khoa-viewer-plan.md`.
- 원칙: **코드를 복사하지 않고 OpenS100의 경계와 규약(UTF-8 `std::string`, 스마트 포인터, 코어에 MFC·D2D 금지)에 맞춰 다시 쓴다.** KHOA 구현은 참조 구현이자 테스트 오라클이다. 가져온 기능마다 gtest와 TDS 기반 테스트를 붙인다.

## 1. 현황 비교

KHOA 쪽 코드는 KHOA 저장소 경로, OpenS100 쪽은 이 저장소 경로.

| 기능 | KHOA | OpenS100 | 판정 |
|---|---|---|---|
| S-102 HDF5 | `libS100HDF5/S102.cpp` + `GISLibrary/S102Cell.cpp`(컬러맵, 안전수심, no-go) | `GISLibrary/S102H5.cpp` 231줄, D2D 비트맵 | 부분 |
| S-104 / S-111 / S-413 HDF5 | `libS100HDF5/S104.cpp, S111.cpp, S413.cpp` + `*Cell.cpp`(시계열, S-111 애니메이션, 화살표 thinning) | `GISLibrary/S100H5.cpp` 67줄 스텁 | **없음** |
| BAG | `GISLibrary/BAG.cpp`, `BAGLayer.cpp` | 없음 | 없음 (우선순위 낮음) |
| 교환세트 | `S100ExchangeSet/`(3.4K): `ExchangeSetXML`, `ENC_ExchangeSet`, CATD | `GISLibrary/ExchangeCatalogue.cpp`, `S100ExchangeCatalogue.cpp`, 데이터셋 매니저 UI | 대체로 동등. 차이 diff 필요 |
| S-98 IC | `DLL_InteroperabilityCatalog/`(3.8K) + `LayerManager::DrawS100Datasets_IC_ON/OFF` + 리본 UI | `GISLibrary/IC_*.cpp` 모델 530줄, 그리기 없음 | 모델 부분, **그리기 없음** |
| 8211 쓰기 | `DRReaderWriter`, `DRDirectoryInfoWriter`, `S101Cell::Save` | `S101Cell::SaveAsENC`(R_DDR 기반) | 동등 |
| 업데이트 파일 **생성** | `S101Cell::CreateUpdateFile`(:3811), 병합(:5342) | 원격 브랜치 `origin/makeUpdateFile`에만 | 브랜치 병합 필요 |
| 업데이트 **적용** | `S101Cell::Update*` | `S101Cell.cpp:2426-3243` | 동등 |
| 피처·기하 편집 | `NewFeatureManager`, `ENCEditStructure`, `S100EditingResource`, 스냅·실행 취소·관계 | `S101Creator`(1.2K), `S100EditRender` | 부분 |
| CS 절차 (C++) | `GISLibrary/S101_CS_*.cpp` 11개 + `S101_ConditionalSymbol` | 없음(Lua PC의 CS만) | 없음. **S-101 PC 2.0.0은 Lua CS를 포함하므로 C++ CS는 레거시일 수 있음. 필요성 확인** |
| XSLT 포트레이얼 | `S101Portrayal/ProcessS101XSLT.cpp` | `ProcessS101::ProcessS100_XSLT` | 동등 |
| SVG → D2D | `S100_SVG_D2D1_DLL/`(6.5K): 지오메트리 캐시, WIC 패턴 비트맵 | `S100Engine/SVGReader.cpp`, `S100SymbolManager.cpp` | 동등~부분. 캐시 전략 비교 |
| 픽 리포트 | `GISLibrary/PickReportResult`, `libS100HDF5/PickedFeature`(라이브러리) | `OpenS100View.cpp:1191-1375`(뷰) | KHOA가 위치상 옳음 |
| 커버리지 레이어·컬러맵 | `ColorMap*`, `S101_CoverageLayer` | 없음 | 없음 |
| S-124 매니저 | `S124Manager` | `S10XGML` 범용 | 범용으로 충분한지 확인 |
| S-57 / S-52 / S-63 | `ENC_S101_Factory`, `ENC_Feature/Vector`(감지만, 로딩 거부), `PLLibrary`(S-52식 심볼) | 없음 | 지침 2에 따라 **범위 안**. 단 후순위(P3 이후). KHOA 코드는 감지 수준이라 별도 설계 필요 |
| AIS/NMEA/선박 감시, 항로, SECOM, OpenAPI, 조석 | 다수 | 없음 | **뷰어에 남긴다.** 단 항로 감시의 코어 모델(S-98 route monitoring)은 opens100-web 역이식 항목 |

## 2. 업스트림 항목 (우선순위)

### P1. 코어 데이터 (Phase 2 전반)

- [x] **U0. 측지 함수 (LatLonUtility 수렴 1차).** (85f9c919) 뷰어 포크에만 있던 대권·항정선 거리/방위/도착점, 도분초 변환, 다각형 중심을 `s100::geodesy`(`LatLonUtility/Geodesy.h`: `Position`, `Course`, `LengthUnit`, `RhumbInverse/Direct`, `GeodesicInverse/Direct`, `DmsToDegrees`, `PolygonCentroid`)로 올리고, 뷰어가 쓰는 옛 이름(`GetDistanceRhumbLineMercator`, `CalcGreatCirclePosByGeographicLib`, `DMStoDegree` 등)은 얇은 래퍼로 유지. 테스트 9건. 이것이 뷰어가 자기 `LatLonUtility` 포크를 버리고 `opens100::latlon`을 링크하기 위한 선행 조건이다. 남은 차이: 좌표 문자열 함수(`CheckFormWGS`, `GetDegree`, `LatLonToWGS` 등 wide 문자열), `UNIT`이 `enum class`인 점, `Pos`의 가상 소멸자. 코어에 넣지 않을 것: `Convert`(PROJ 기반 EPSG 변환), 타임스탬프·파일 쓰기 유틸, `TileCalculator`, `TimeUtil` — 뷰어에 남긴다.
- [ ] **U1. HDF5 모듈.** `libS100HDF5`를 참조해 `s100-hdf5` 경계로 재작성: 공통 `S100H5File`(메타데이터, 피처 정보 그룹, 축·격자 파라미터), S-102(수심 격자, 불확도), S-104(수위 시계열), S-111(유속·유향 시계열, 정점/격자), S-413(기상). 렌더링은 분리(격자 → 커버리지 디스플레이 인스트럭션). 완료 기준: `D:\s100-reference\tds\KOR_TDS\KOR_S-1xx\`의 S-102/104/111 파일 로딩 테스트 통과, 시각 확인.
- [ ] **U2. 업데이트 생성.** `origin/makeUpdateFile` 브랜치를 `dev-jogm`에 병합하고 KHOA `CreateUpdateFile`과 결과 비교. 완료 기준: 베이스+업데이트 → 병합 → 재분해 라운드트립 테스트.
- [ ] **U3. 교환세트 차이 보완.** KHOA `S100ExchangeSet`과 OpenS100 `ExchangeCatalogue` diff. CATD·서명·지원 파일 처리 중 빠진 것 보완. 완료 기준: `KOR_TDS\S100_ROOT\CATALOG.XML`(808셀) 파싱 테스트.
- [ ] **U4. 픽 리포트 라이브러리화.** 뷰의 구현을 `s100-sdk`로 옮기고 KHOA `PickReportResult` 구조 참고. HDF5 격자 픽 포함.

### P2. S-98과 렌더 (Phase 2 후반)

- [ ] **U5. S-98 IC 그리기 모델.** KHOA `DrawS100Datasets_IC_ON/OFF`의 로직을 "IC 적용 → 디스플레이 리스트 필터·우선순위 조정"으로 재정의해 렌더러와 분리. opens100-web의 S-98 구현(`dataset-loading.ts`, `update-report.ts`)과 함께 설계.
- [ ] **U6. 커버리지·컬러맵.** S-102 컬러맵, 안전수심 기준 채색, no-go 영역. U1 이후.
- [ ] **U7. SVG 캐시 전략 통합.** 두 구현 비교 후 지오메트리 1회 생성 + 비트맵 캐시(역이식 P1과 합쳐서).

### P3. 편집 (Phase 2 이후, 사용자 결정 후)

- [ ] **U8. 편집 모델.** KHOA `NewFeatureManager`, 스냅, 실행 취소, 관계·자동 관계를 `S101Creator` 위로. 편집 기능을 공개 라이브러리에 넣을지는 사용자 결정(KHOA 요구사항에 묶인 부분 분리).

### 하지 않을 것

- `S101_CS_*` C++ CS 절차: S-101 PC 2.0.0 Lua CS로 대체됐는지 확인 후, 대체됐으면 가져오지 않는다.
- BAG: 후순위. S-57/52/63은 지침 2에 따라 범위에 포함하되 Phase 2에서는 다루지 않는다(별도 계획).
- 뷰어 특화(AIS, 항로 UI, SECOM, OpenAPI, 조석, 설치): KHOA에 남긴다.

## 3. 절차 (항목마다)

1. KHOA 구현의 진입점·의존성·입출력을 짧게 적는다(이 문서 또는 항목별 노트).
2. OpenS100 경계에 맞는 헤더(공개 API)를 먼저 설계한다. MFC·D2D·`CString` 금지.
3. gtest로 TDS 기반 테스트를 먼저 쓴다(KHOA 결과를 오라클로).
4. 구현. 스마트 포인터, UTF-8.
5. `OpenS100` 앱에서 시각 확인.
6. 이 문서 체크, 커밋.

## 4. 진행 기록

| 날짜 | 항목 | 커밋 | 비고 |
|---|---|---|---|
| 2026-11-08 | 문서 작성 | | 두 저장소 비교 조사 기반 |
| 2026-11-09 | U0: 측지 함수 업스트림 + `s100::geodesy` | 85f9c919 | 테스트 26/26, CMake 타깃 `opens100::latlon`에도 포함 |
| 2026-11-09 | FC 수렴: 연관 바인딩 조회, 경로 생성자, `isProductId` | 6058305f | 테스트 30/30. 뷰어가 `FeatureCatalog` 포크를 지우고 `opens100[feature-catalogue]` 사용(뷰어 브랜치 `jogm-opens100-fc`, 502bd0e9) |
