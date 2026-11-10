# 커버리지 역이식: S-102 표시, 강화 안전등심선(ESC), 수위 보정(WLA)

2026-11-11 작성. 사용자 요청: "zip으로 된 PC 설치, 102 그리기, opens100-web의 기능(102·ESC·WLA)을 가져온다. 문서부터 하고 시작한다." 지침 5(웹의 앞선 부분 역이식), 7(계획은 문서로), 22·25(SOLID·현대 C++), 23(화면 검증), 24(렌더 백엔드는 Skia로 가되 D2D는 격리만)를 따른다. [역이식 계획](2026-11-08-backport-plan-from-opens100-web.md)의 P1 "S-102 유효 셀 링", "커버리지 재샘플링 1회"와 S-98 부속서 D 항목을 이 문서가 구체화한다.

## 1. 지금 상태 (2026-11-11)

- **설치·선택은 된다.** `.h5`는 `Hdf5DatasetInspector`로 데이터셋으로 설치되고(8e7eebbe), System Database 차트는 경계 상자가 화면과 겹치면 S-102를 연다. 차트의 그리기 경로는 격자를 격자 렌더러로 보낸다.
- **그려지지 않는다.** 기존 `S102Renderer`는 직접 연 레이어에서도 아무것도 그리지 않는다(렌더 테스트 2개 `DISABLED_` 상태). 격자는 제대로 읽힌다(KOR `102KR00T4G3B3000026`: 501×501, 수심 노드 138,200개, -1.2~221.5 m). 이 렌더러는 PC를 쓰지 않고 색을 하드코딩하며(`S102_Color`, `ENCCommon` 등심선), 비트맵을 셀마다 `FillRectangle`로 채운다.
- **PC zip 설치**: `Importer::importPortrayalCatalogue`가 `.zip`을 받아 임시 폴더에 풀고(경로 탈출 이름 거부) `portrayal_catalogue.xml`이 있는 폴더를 설치한다. FC/PC 탭이 `.zip`을 받는다. 교환세트 카탈로그의 `.zip` 항목은 PC로 본다(fd552a76). 테스트 3개 통과(가짜 zip 2개, IHO가 배포한 S-102 PC 3.0.0 zip 1개). 다른 세션이 만든 `SystemDatabase/ZipArchive`를 쓴다.
- Lua 명령 파서(`ProcessS101.cpp`)는 `CoverageColor`·`CoverageFill`을 알아보지만 그리는 쪽이 없다.

## 2. opens100-web의 구현 (참조)

웹 저장소 `bluemap-github/opens100-web` 기준. 줄 수는 TS.

| 영역 | 파일 | 핵심 |
|---|---|---|
| 격자 모델 | `packages/core/src/coverage.ts`(670), `utm.ts`(72) | `RegularGrid`(행 우선, 0행=남쪽, 노드=셀 중심, UTM 선택), `CoverageDataset/Feature/Component/TimeGroup`, `CoverageSource.readValues`(지연 읽기)와 `revision`, `gridPosition/gridCoordinates/gridLine`(UTM은 32셀 조각), `gridCellAt`, `LatticeMapping`, `isNoData`(NaN 또는 채움값과 정확히 같음), `parseS100DateTime`, `timeIndexAt`, `displayTimeIndex`(S-111 9.4) |
| HDF5 리더 | `packages/hdf5/src/read-hdf5.ts`(655) | Part 10c 일반 리더. 루트 속성 → `metadata`, `/Group_F/featureCode`와 구성요소 설명(채움값·하한·상한), `dataCodingFormat` 2·3, 인스턴스 `<Code>.NN`, 시간 그룹 `Group_NNN`(`validTime/timePoint/forecastTime`, 1년 이하는 null), `[행, 열]` 축 순서, `dataOffsetCode/Vector`로 원점 이동, compound 구성요소 해독, LRU 48 |
| S-102 색 | `apps/viewer-ol/public/pc/S102_Portrayal/Rules/BathymetryCoverage.lua`, `core/src/display-list.ts`(L332-410), `core/src/color-profile.ts`(188) | **PC의 Lua가 정한다**: `CoverageColor:<토큰>`·`LookupEntry:<이름>,<하한>,<상한>,<닫힘>` 쌍 뒤에 `CoverageFill:depth`. 두 단계(DEPIT/DEPVS/DEPDW) 또는 `FourShades`면 다섯 단계. 문맥 변수 `SafetyContour`(30), `ShallowContour`(2), `DeepContour`(30), `FourShades`(false)는 S-101과 이름이 같아 함께 설정된다. 첫 번째로 맞는 항목, 맞는 것이 없으면 투명 |
| 래스터 | `packages/render/src/coverage-renderer.ts`(735) | 셀 RGBA 배열 → 지리 격자는 Mercator 행으로 다시 표본(높이 ≤ 16384), UTM은 `LatticeMapping`으로 휘어 넣기. 리비전·팔레트·시간 인덱스당 1회. 셀이 화면에서 3 px 이상이면 보이는 부분만 화면 해상도로 다시 휘어 넣는 `sharpWindow`. 보간 없이(nearest) 한 번에 그린다 |
| 유효 셀 링 | `core/src/valid-grid-rings.ts`(98) | 유효 셀 외곽선(구멍은 별도 링, 짝홀 채움). ESC가 켜지면 S-102 아래의 S-101 `DepthArea`·`DredgedArea`·`DepthContour`를 가린다 |
| ESC | `core/src/enhanced-safety-contour.ts`(179), `sdk/src/depth-jobs.ts` | S-98 D-1.4/D-2.4. 셀 분류(0=격자 밖·자료 없음, 1=`d < 안전등심선`, 2=안전) 후 분류가 다른 셀 사이 변을 모아 격자선 따라 이어 붙인 선분. S-102 경계·WLA 경계도 같은 함수. 선 모양: DEPSC 0.64 mm 실선, S-102 경계 DEPWL 0.96 mm 파선, WLA 경계 DEPWL 1.92 mm 일점쇄선. DEPWL은 S-102 색 프로필에 없어 xyL에서 계산(`xyLToRgb`) |
| WLA | `core/src/water-level-adjustment.ts`(553) | S-98 D-1.5(불확도), D-2.2(데이텀 호환), D-2.5(옵션 1: 한 시각), D-2.6(옵션 2: 기간). 옵션 3(항로 일정)은 웹도 없음. 시간 보간 없음: 앞뒤 기록 중 얕은 쪽. 기록 중 하나라도 값이 없으면 그 셀은 보정 안 함. S-104 셀 → S-102 셀은 겹치는 S-104 셀 중 가장 얕은 값(그림 D-2-9, 경계 여유 1e-6 셀). CRS가 다르면 `LatticeMapping`. 깊이 = 원수심 − 불확도 + 수위(양수가 아래). `verticalCS` 6498이면 부호 반대 |
| UI | `apps/viewer-ol/src/depth-settings.ts`(313), `context-settings.ts`(254) | ESC 켜기(기본 켬), 불확도, WLA 끔/한 시각/기간, 시각 슬라이더(10분 단위, S-104 기록 시각 눈금), 데이텀 불일치 무시(시험용). 표시: `WLA hh:mm dd Mmm yyyy`, "Incompatible datums", "No water level: S-104 does not cover this time", "OVERLAP". 픽 리포트: 원수심, 수위, 보정 수심 |

## 3. 설계

### 3.1 모듈

- **새 라이브러리 `Coverage/`**(MFC·Direct2D 없음, 지침 18·22): 격자 모델, UTM, `LatticeMapping`, 커버리지 원본 인터페이스, 룩업·색 프로필, ESC·경계, 유효 셀 링, WLA. 순수 함수와 작은 클래스. CMake 타깃 `opens100::coverage`, VS 솔루션 `01. LIB`.
- **HDF5 커버리지 리더**: `Coverage/Hdf5Coverage`(의존: hdf5). Part 10c 일반 리더로 웹 리더와 같은 규칙. 기존 `S102H5`·`S100H5`·`H5_*` 클래스는 리더가 대체한 뒤 표시 경로에서 빠진다(삭제는 마지막 단계에서 따로).
- **포트레이얼**: S-102 PC의 Lua를 기존 Lua 엔진으로 실행하되, 커버리지 데이터셋용 호스트(피처 = 커버리지 인스턴스, 공간 = Coverage)를 둔다. 결과 명령(`CoverageColor`/`LookupEntry`/`CoverageFill`)을 `CoverageLookup`으로 모은다. 웹 `coverageToDataset`과 같은 모양. S-104·S-111 PC도 같은 길을 쓴다.
- **렌더**: `GISLibrary/CoverageRasterRenderer`가 `CoverageRaster`(셀 색 → Mercator 래스터, 리비전당 1회)를 D2D 비트맵 한 장으로 그린다. D2D 코드는 이 파일에만 둔다(지침 24: 나중에 Skia 렌더러가 같은 래스터를 그린다). 선(ESC·경계)은 화면 좌표 선분 목록으로 넘긴다.
- **System Database 차트**: S-102 레이어가 새 커버리지 레이어로 열리고, WLA가 켜지면 같은 화면의 S-104를 수위 원본으로 쓴다.

### 3.2 검증 원칙

웹 테스트의 입력과 기대값을 그대로 C++ 테스트로 옮겨 같은 결과를 확인한다(SDB-5 방식). 실제 자료 테스트는 KOR TDS(`D:\s100-reference\tds\KOR_TDS\KOR_S-102`, `KOR_S-104`)와 웹 저장소의 시험 자료(`apps/viewer-ol/public/data`, S-102 5개·S-104 약 100개)를 쓰고, 없으면 건너뛴다. 화면 변경은 오프스크린 PNG와 실제 뷰어 캡처로 확인한다(지침 23).

## 4. 작업 항목

- [x] **CV-0** PC zip 설치(2026-11-11). `Importer::importPortrayalCatalogue`가 `.zip`을 임시 폴더에 풀고(경로 탈출 이름 거부) `portrayal_catalogue.xml`에 가장 가까운 폴더를 설치한다. FC/PC 탭이 `.zip`을 받는다. 데이터 보호 세션의 `ZipArchive`(4f2c5b56)를 쓴다. 테스트: 가짜 zip 2개, IHO S-102 PC 3.0.0 zip.
- [x] **CV-1** `Coverage/` 모듈 뼈대와 격자 모델: `RegularGrid`, UTM(Krüger n³, 웹과 같은 식), `gridPosition/gridCoordinates/gridLine/gridBlockRing/gridBounds/gridCellBounds/gridCellSize/gridCellAt`, `isNoData`(값은 float32, 채움값은 `(double)(float)` 비교), `sampleGrid`, `parseS100DateTime`, `timeIndexAt`, `displayTimeIndex`, `LatticeMapping`. 테스트: 웹 `coverage.test.ts`, `utm.test.ts`의 값. (2026-11-11)
  - 네임스페이스 `s100::coverage`. 파일: `Geo.h`, `Utm`, `RegularGrid`(격자 기하·표본·`LatticeMapping`), `CoverageTime`(`std::chrono::sys_time<milliseconds>`, 1년도 그대로), `CoverageModel`(구성요소·피처·데이터셋·`CoverageSource` 인터페이스, 값은 `shared_ptr<const vector<float>>`), `CoverageLookup`(Part 9a 구간 8종, 첫 일치·비율)도 여기 둔다(CV-3에서 쓸 것).
  - `gridCellAt`의 반올림은 JS `Math.round`와 같이 `floor(x + 0.5)`(음수 반값에서 `std::round`와 다르다).
  - VS 솔루션 `01. LIB`에 `Coverage`(6개 구성, `/W4 /WX`), CMake `opens100::coverage`(옵션 `OPENS100_BUILD_COVERAGE`). 의존성 없음(HDF5는 CV-2).
  - 검증: `CoverageGridTest.cpp` 14개(PROJ 기준값 5 mm 이내, 왕복 5e-8°, UTM 격자선 조각, 격자 대응 0.001셀 이내 포함) 통과. CMake `coverage` 타깃 빌드 확인.
- [x] **CV-2** HDF5 커버리지 리더: 루트 속성, `Group_F` 설명, 인스턴스·시간 그룹, 데이터 오프셋, compound 해독, 채움값, UTM 경계(셀 경계의 합집합), LRU. 테스트: KOR S-102(원점·간격·행렬 수, 오프셋 코드 5), S-104(구성요소, 시간, 범위), 웹 `read-hdf5.test.ts`의 값. (2026-11-11)
  - `Coverage/Hdf5Coverage`: HDF5 C API로 웹 리더와 같은 규칙. 루트 속성은 모두 `metadata`(배열은 ", "로 이음), 열거형 속성·구성요소는 밑 정수로 읽는다. `Group_F/<Code>` 표의 문자열 열(고정·가변 길이), 피처 코드 목록이 없으면 루트의 그룹들. `dataCodingFormat` 2만 읽고 3(비정규 격자, S-111 메시)은 `skipped()`로 알린다(웹은 읽음, 이번 범위 밖). 값은 compound 구성요소마다 float32로, 3차원이면 그 깊이 층만. 최근 48개 격자 캐시. 파일은 열어 둔 채 값만 필요할 때 읽고 HDF5 호출은 뮤텍스로 묶는다.
  - 테스트 경로 `testpaths::webDataRoot()`: `OPENS100_WEB_DATA`, 없으면 이 저장소 옆 `bluemap-github/opens100-web/apps/viewer-ol/public/data`.
  - 검증: `CoverageHdf5Test.cpp` 6개 통과(HDF5가 아닌 파일, KOR S-102: 501×501·수심 노드 138,200·-1.2~221.51 m·시간 없음, 웹 S-102 N4F2K3: 오프셋 5로 원점 126.0015/37.0015·499×500, S-104 F4N30: 구성요소·범위 0.52~2.76·추세 0~3, F2K20: 채움값이 든 범위 버림, KOR S-104: 첫 시각 2026-03-29T15:00Z·verticalCS 6499). CMake `coverage` 타깃이 `hdf5::hdf5-shared`에 의존.
- [x] **CV-3** S-102 표시: 커버리지 Lua 호스트, `CoverageLookup`(첫 일치, 닫힘 4종, 반구간), 색 프로필(sRGB, 같은 이름 토큰 대체), `CoverageRaster`(지리·UTM, sharpWindow), D2D 그리기, 문맥 변수(S-101 SafetyContour를 S-102에도). 기존 `S102Renderer` 경로를 이 경로로 바꾼다. 테스트: 웹 `s102.test.ts`(조회 항목, FourShades 5항목), `coverage-raster.test.ts`, 렌더 테스트 2개를 켠다. 뷰어에서 KOR S-102를 S-101과 겹쳐 캡처. (2026-11-11)
  - 1부(daa64c88): `Coverage/CoverageInstructions`(Part 9a `CoverageColor`/`LookupEntry`/`CoverageFill`), `lookupCellColor`, `CoverageRaster`(지리 격자는 열 그대로·행을 Mercator로 다시 표본, UTM은 `LatticeMapping`으로 휘어 넣기). `sharpWindow`(크게 확대했을 때 화면 해상도 재표본)는 옮기지 않았다(후속).
  - 2부: `GISLibrary/CoverageDataset`(`S100SpatialObject`, 형식 `Coverage`). 커버리지 인스턴스마다 GF 피처 하나(코드만, 속성·공간 없음)를 내놓아 기존 Lua 엔진(`ProcessS101::InitPortrayal`, 일반 `hd_init` 경로)이 PC의 `main.lua`를 그대로 실행한다. 문맥 변수는 뷰어의 `SafetyContour`·`ShallowContour`·`DeepContour`(PC에 있는 것만). 결과의 `CoverageFill`마다 값을 읽어 래스터를 만들고(팔레트·시간 그룹·리비전이 키) D2D 비트맵 한 장으로 그린다. 색은 PC의 `colorProfile.xml`을 직접 읽어 현재 팔레트(`ENCCommon::m_eColorTable`)의 sRGB. `S100Layer`는 S-102 `.h5`를 이 객체로 열고(기존 `S102H5` 경로 대신), `DrawGrid`와 `BuildPortrayalCatalogue`(설정 변경 시 다시 그림)가 이 객체를 안다.
  - 결함 수정: `S100_ColorProfile::ReadFile`이 루트를 `colorProfile`로만 비교해 `cp:colorProfile`(S-100 5.x 스키마, S-102 PC 3.0.0)을 아무 말 없이 읽지 않았다. 지역 이름으로 비교한다.
  - 검증: 렌더 테스트 2개를 켰다(System Database 차트, 레이어로 직접 연 S-102). KOR S-102가 흰색 + 3색(DEPIT·DEPVS·DEPDW, 안전등심선 30 m)으로 그려짐을 PNG로 확인. 전체 155개 통과. 실제 뷰어 화면 확인은 남음.
- [ ] **CV-4** ESC와 경계: `collectEdges`, `safetyContourEdges`, `boundaryEdgesOfClasses`, `depthLines`, `validGridRings`, 선 모양과 DEPWL 색(`xyLToRgb`), ESC가 켜지면 S-102 아래 S-101 수심 피처 가리기. 테스트: 웹 `enhanced-safety-contour.test.ts`(그림 D-1-5: 길이 8, 선분 6 …), `valid-grid-rings.test.ts`, `depth-compute.test.ts`.
  - 1부(알고리즘, 2026-11-11): `Coverage/DepthLines` — `safetyContourEdges`(분류 1=얕음·2=안전·0=자료 없음, 1|2 사이 변만), `boundaryEdges(OfClasses)`, `depthLines`(ESC면 안전등심선과 S-102 경계, WLA가 적용된 셀은 경계에서 빼고 WLA 경계로), `validGridRings`(유효 셀 외곽, 구멍은 따로, 모서리 접촉은 우회전으로 분리), `xyLToRgb`·`depwlColor`, S-98 선 모양 3종. 웹과 같은 순서로 훑어 선분 수까지 같다. 테스트 7개(그림 D-1-5: 길이 8·선분 6, 경계 18/13/8, 3×2 격자 선분 1개, 링 블록·구멍·대각, UTM 격자 경계 조각 4+4+1+1, DEPWL) 통과.
  - 2부(선 그리기, 2026-11-11): `CoverageDataset`이 깊이 `CoverageFill` 위에 안전등심선(DEPSC, 0.64 mm, S-102 색 프로필)과 S-102 경계(DEPWL, 0.96 mm 파선 1.92/1.92, xyL에서 계산)를 그린다. 96 dpi로 mm→화소, D2D 파선은 선 굵기 배수로. 안전등심선은 뷰어의 `SAFETY_CONTOUR`. 선은 안전등심선 값·값 리비전이 바뀔 때만 다시 계산. 선택자 `CoverageDataset::EnhancedSafetyContour`(기본 켬, UI는 CV-6). 오프스크린 PNG로 확인.
  - 남은 것: ESC가 켜지면 S-102 아래 S-101 `DepthArea`·`DredgedArea`·`DepthContour` 가리기(`validGridRings` 사용).
- [x] **CV-5** WLA: 기록 선택, 데이텀 판정(D-2.2), 수위 격자, 겹치는 셀 중 최소(같은 CRS, 다른 CRS), `adjustDepths`, 보정 원본(리비전 증가), 불확도. 테스트: 웹 `water-level-adjustment.test.ts`(표 10, 그림 D-2-9, UTM 8N, 경계 여유, 데이텀, 표시 문자열), KOR S-102/S-104 실제 자료(데이텀 불일치 → 무시 시 적용 > 0, 한 셀 = 원수심 + 수위). (2026-11-11, 알고리즘)
  - `Coverage/WaterLevelAdjustment`: `selectWaterLevelRecords`(옵션 1·2), `verticalDatumOf/Name`(S-100 목록 32종, EPSG), `waterLevelSign`(6498은 −1), `coveragesOverlap`, `formatWlaDateTime`·`wlaIndication`, S-104 수위 격자(선택한 기록 중 가장 얕은 값, 하나라도 없으면 NaN), 겹치는 셀 중 최소(같은 CRS는 열·행 범위, 다른 CRS는 `LatticeMapping`으로 모서리 상자, 경계 여유 1e-6 셀), `adjustDepths`, `WaterLevelAdjustedCoverage`(깊이 구성요소·시간 0만 바꿔 돌려주고, 바뀔 때마다 `revision` 증가, 원본은 `base()`). 웹의 셀 범위 캐시와 비동기 실행(`updateWith`)은 옮기지 않았다(CV-6에서 스레드와 함께).
  - 검증: `CoverageWlaTest.cpp` 10개 통과(그림 D-2-8·D-2-10 기록 선택, 표 10: 10.32/9.86/9.58, 6498: 9.68, 그림 D-2-9, 데이텀 불일치·시간 밖, UTM 8N 대 지리 격자 16셀, 채움값, 경계 여유, 표시 문자열, KOR S-102/S-104: 데이텀 11 대 15로 거부, 무시하면 131,832셀 보정, 보정 수심의 안전등심선 있음).
- [ ] **CV-6** UI: 수심 설정(ESC, 불확도, WLA 방식·시각·기간, 데이텀 무시), 표시 문구, 픽 리포트(원수심·수위·보정 수심), 계산 스레드 풀(`std::jthread`).
  - 1부(2026-11-11): `CoverageDataset::DepthSettings`(ESC·불확도·WLA 방식과 시각·데이텀 무시). S-102는 `WaterLevelAdjustedCoverage`를 거쳐 그리고, 설정이나 열린 S-104 목록이 바뀔 때만 다시 보정한다(열린 커버리지 데이터셋 목록에서 S-104를 찾음). S-104도 `CoverageDataset`으로 열지만 그리지 않는다(웹 기본처럼 숨김, 수위 원본으로만). WLA가 켜지면 WLA 경계(DEPWL 1.92 mm 일점쇄선)를 그리고, S-102 경계는 WLA가 적용된 셀에서 뺀다(D-2.4). 뷰 왼쪽 아래 표시: `WLA hh:mm dd Mmm yyyy`(적용된 셀이 있을 때), "Water level needs S-104", "Incompatible datums: S-102 …, S-104 …"(무시하면 "(ignored for testing)"), "No water level: S-104 does not cover this time", "Depths less vertical uncertainty".
  - 설정 창에 "S-102 / S-104" 탭(`CConfigurationDepth`, `IDD_DIALOG_CONFIG_DEPTH` 372, 컨트롤 1020~1031): ESC, 불확도, WLA 끔/한 시각/기간(UTC 날짜·시각 선택), "From S-104 records"(첫 기록, 기간은 최대 6시간), S-104 기록 범위, 데이텀 불일치 무시. 적용하면 지도를 다시 그린다.
  - 검증: 렌더 테스트 `RenderSmoke.AdjustsS102ByTheS104OnScreen`(KOR S-102 + S-104를 레이어로, 첫 기록 시각·데이텀 무시: 적용 셀 > 0, 데이텀 불일치 보고, 화소 21,432개 달라짐, WLA 경계 그려짐 PNG 확인). 전체 173개 통과. 설정 탭은 빌드만 확인(화면 확인 남음).
  - 남은 것: 픽 리포트(원수심·수위·보정 수심), 계산 스레드(지금은 그리기 스레드에서 동기, KOR 501×501은 수 ms), WLA 기록 시각 눈금 슬라이더. System Database 차트에서 S-104를 쓰려면 S-104 FC가 설치돼 있어야 한다(`ProgramData/FC/S-104_FC.xml`).
- [ ] **CV-7** 화면 검증과 성능 기록: KOR S-102·S-104를 S-101과 함께 축척을 바꿔 캡처, 첫 그리기·다음 프레임 시간(웹 기준: S-102 프레임 1 s → 51 ms).
- [ ] **CV-8** 정리: 표시 경로에서 빠진 `S102H5`·`S102Renderer`·`S102_Color`·`H5_*` 제거, 문서 갱신.

## 5. 위험과 결정할 것

- **프로젝트 파일 동시 수정**: 새 모듈은 `CMakeLists.txt`, `OpenS100.sln`, 테스트 `.vcxproj`를 고친다. 이 중 둘은 데이터 보호 세션이 아직 커밋하지 않은 변경을 갖고 있다. 그 세션의 커밋 뒤에 추가하거나, 먼저 알리고 줄을 덧붙이기만 한다.
- **Lua 버전**: 엔진은 Lua 5.1(S-101용)이다. S-102 PC 규칙은 `tostring(contextParameters.SafetyContour)`처럼 문맥 값을 문자열로 다룬다. 불리언은 진짜 Lua 불리언으로 넘겨야 한다("false" 문자열은 참).
- **채움값 비교**: 웹은 float32 값을 double로 넓혀 double 채움값과 정확히 비교한다. C++도 같은 규칙을 지킨다.
- **S-102 2.x**: 웹은 판 구분이 없고 시험 자료는 모두 3.0이다. 2.x는 같은 `values`(depth, uncertainty) 구성이면 읽힌다. 2.x 자료로 확인하지 못했다.
- **WLA 부호**: `verticalCS` 6499 = +1, 6498 = −1(S-98 D-1.5 각주 5와 반대로 둔 웹의 판단). 그대로 따르고 테스트로 고정한다.
- **경계선은 S-102마다**: 이웃 데이터셋 사이의 경계가 각각 그려진다. 생산자 선택은 웹에도 없다(`OVERLAP` 표시만).
- **렌더 백엔드**: 이번에는 D2D 비트맵으로 그린다. 래스터 생성은 백엔드와 무관한 코드로 두어 Skia 전환 때 그대로 쓴다.

## 변경 이력

- 2026-11-11 작성. 웹 구현 조사(영역 7개)와 C++ 현재 상태 확인(S-102 렌더러가 아무것도 그리지 않음, PC zip 설치 구현).
