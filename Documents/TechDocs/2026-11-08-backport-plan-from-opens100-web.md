# opens100-web → OpenS100 역이식 계획 (2026-11-08)

- 대상 저장소: **OpenS100**(이 저장소, `dev-jogm`). 처음 KHOA 뷰어 저장소에 작성됐던 것을 옮겼다. 표의 "OpenS100 (C++)" 열 중 일부는 KHOA 뷰어 코드를 보고 적은 것이므로, OpenS100 자체 코드 기준으로 재확인이 필요한 항목에는 (확인 필요)를 붙였다.

- 상태: 초안. 상위 문서: [로드맵](2026-11-08-roadmap-library-first.md). 이 문서는 로드맵 Phase 3의 세부다.
- 대상 저장소: `C:\Users\jogm\Desktop\Git\bluemap-github\opens100-web` (github.com/bluemap-github/opens100-web, v0.3.12, 191커밋 기준)
- **먼저 읽을 것:** opens100-web 안에 이미 `docs/plans/opens100-cpp-backport-feasibility.md`(2026-09-26, OpenS100 `55cacf84` 기준)가 있다. 각 웹 기법을 Direct2D 등가물로 대응시키고 효과순으로 매긴 문서다. 측정치는 `docs/plans/visualisation-optimisation-handoff.md`. 이 문서는 그것을 대체하지 않고, 라이브러리화 로드맵 관점에서 범위와 순서를 다시 정한 것이다.

## 1. 현재 두 구현의 위치

| 영역 | opens100-web | OpenS100 (C++) | 역이식 방향 |
|---|---|---|---|
| 모듈 경계 | core/gml/hdf5/iso8211/portrayal/render/sdk/openlayers/cesium 분리 | GISLibrary 모놀리스 + 소형 라이브러리 | 웹 → C++ (구조) |
| 제품 판별 | DSID PRSP, GML `productIdentifier`·네임스페이스, HDF5 `productSpecification`, CATALOG.XML, 불일치 보고 | FC 이름 문자열 완전 일치 | 웹 → C++ |
| 카탈로그 관리 | `CatalogueRegistry`: product+version 키, ready/fallback/unresolved, 늦게 도착해도 재포트레이얼 | 레이어 생성 시 FC/PC 고정 | 웹 → C++ |
| 데이터셋 검증 | `validateDataset`: 미해결 참조, 연관 대상, 알 수 없는 역할·타입·속성, 필수 속성 누락, 기하, 중복 ID, 버전 불일치 | 없음 | 웹 → C++ |
| 디스플레이 리스트 | 단일 `DisplayList` IR(`S100Presentation/5.2`), 셀 간 병합 후 `comparePainting` 정렬 | 셀별 개별 그리기 | 웹 → C++ |
| Lua | Lua 5.1(WASM), 셀마다 새 상태, 샌드박스, 워커 풀, 취소·타임아웃 | LuaJIT, UI 스레드, 전역 호스트 상태 | 웹 → C++ (격리·백그라운드) |
| 심볼 렌더 | (심볼, 크기)별 1회 래스터, 패턴은 격자 셀 하나로 | SVG → D2D 지오메트리, `SENC_LineStyle`이 매 프레임 지오메트리 재생성(웹 역이식 문서가 OpenS100 `55cacf84`에서 확인) | 웹 → C++ |
| S-98 | WLA, 강화 안전등심선, E-1 셀 선택, 과축척 X1.3, 업데이트 하이라이트, 픽 리포트, 경보 카탈로그, 항로 감시 | `IC_InteroperabilityCatalogue` 파서 있음. 그리기 모델은 KHOA 뷰어(확인 필요) | 웹 → C++ |
| 교환세트 | Part 17 읽기·쓰기, zip | 읽기만 | 웹 → C++ (쓰기) |
| 셀 캐시 | `.bmc` 셀 팩(업데이트 적용 완료 바이너리) | SENC 메모리 구조 | 평가 후 결정 |
| ISO 8211 | 읽기 + 업데이트 적용. **쓰기 없음** | 읽기. 쓰기·업데이트 생성은 KHOA 뷰어에 있음(업스트림 대상) | **C++ → 웹** (업스트림 후) |
| 편집 | S-128 스프레드시트 편집만 | OpenS100에 `S101Creator`(확인 필요). 전체 편집은 KHOA 뷰어 | **C++ → 웹** |
| `HostSpatialRelate` | 미구현(throw) | 구현 | **C++ → 웹** |
| S-100 에디션 | 5.2만 | 4.0과 5.x 혼재 | C++가 넓음 |
| XSLT | 브라우저 `XSLTProcessor`(Chrome 제거 예정) | libxslt(디스크 왕복) | C++가 안정적. 디스크 왕복 제거는 C++ 과제 |
| 테스트 | vitest 117파일, 약 750케이스 | 0 | 웹 → C++ (방법론) |

## 2. 역이식 항목 (우선순위)

P1은 로드맵 Phase 2, P2는 Phase 3 이후. 각 항목은 "웹 참조 → C++ 목표 위치 → 완료 기준"으로 적는다.

### P1. 구조와 정확성

- [ ] **카탈로그 레지스트리.** 참조 `packages/sdk/src/catalogue-registry.ts`, `products.ts`. 목표 `s100-core`의 `CatalogueRegistry`: (product, version) → FC/PC, 상태 ready/fallback/unresolved. `LayerManager::AddLayer`의 제품 판별을 이것으로 대체(OpenS100 쪽 현재 방식 확인 필요). 완료 기준: 버전이 다른 FC로도 fallback 로딩되고, 카탈로그 없는 데이터셋이 "unresolved"로 보존됨.
- [ ] **제품 판별.** 참조 `sdk/src/dataset-product.ts`, `gml-loader.ts#detectProductCode`. 목표 `s100::detectProduct()`: 8211은 DSID의 PRSP/PRED, GML은 `productIdentifier` → 루트 네임스페이스 → xmlns → 파일명, HDF5는 루트 `productSpecification`. 네임스페이스와 에디션 불일치를 Issue로 보고. 완료 기준: TDS 전 파일에 대해 판별 결과 표가 테스트로 고정됨.
- [ ] **데이터셋 검증.** 참조 `core/src/dataset-issues.ts`. 목표 `s100::validate(Dataset, FC)` → `std::vector<Issue>`. 완료 기준: 웹과 같은 이슈 코드 집합, 같은 TDS에서 같은 이슈 수.
- [ ] **디스플레이 리스트 IR 공통화.** 참조 `core/src/display-list.ts`, `parse-display-list.ts`, `portrayal/src/parse-lua-instructions.ts`. 목표: C++ `SENC_DisplayList`/드로잉 인스트럭션 구조를 웹 IR과 필드 단위로 맞추고 JSON 직렬화 추가. 완료 기준: 같은 셀·같은 PC에 대해 두 구현의 직렬화 결과가 정렬 후 일치(교차 검증 테스트).
- [ ] **셀 간 통합 정렬.** 참조 `comparePainting`(display plane → drawing priority → viewing group → cell precedence). 목표: `LayerManager::DrawS100Layer`의 셀별 그리기를 "모든 셀 인스트럭션 병합 → 정렬 → 1회 그리기"로. 완료 기준: 인접 셀 경계에서 면·선 겹침 순서가 S-52/S-101 PC 의도대로.
- [ ] **교환세트 쓰기.** 참조 `core/src/exchange-set-writer.ts`. 목표 `s100-core`에 `writeExchangeSet`. 편집기가 저장한 8211 셀을 교환세트로 묶을 수 있게.

### P1. 성능 (웹 측 측정치: 렌더 335→71ms, S-102 1s→51ms)

- [ ] **심볼 비트맵 캐시 + `ID2D1BitmapBrush` 패턴.** 웹 `render/src/symbol-cache.ts#symbolBitmap`, `symbolFillPattern`. KHOA 뷰어의 `S100_SVG_D2D1_DLL`과 OpenS100의 SVG 처리 중 어느 쪽을 기반으로 할지 결정 후. (심볼, 크기, 색상 프로파일)별 비트맵 캐시 추가. 면 패턴은 격자 셀 하나짜리 비트맵 + 스큐 행렬.
- [ ] **매 프레임 지오메트리 재생성 제거.** 웹 문서가 지적한 `SENC_LineStyle.cpp`의 `GetNewD2Geometry`. 지오메트리를 피처 로드 시 1회 생성하고 뷰 변환만 행렬로.
- [ ] **대시 배열·텍스트 상태 인터닝.** 웹 `canvas-renderer.ts`. D2D에서는 `ID2D1StrokeStyle`을 대시 패턴 내용으로 키잉해 캐시, `IDWriteTextFormat`도 동일.
- [ ] **S-102 유효 셀 링.** 웹 `core/src/valid-grid-rings.ts`. 28k 사각형 → 1.6k 링. C++ S102 커버리지 그리기에 동일 알고리즘.
- [ ] **깊이·커버리지 재샘플링 1회.** 웹 `coverage-renderer.ts`. 리비전별 1회.

### P2. 실행 모델

- [ ] **Lua 격리.** 셀마다 `lua_State` 새로 생성, io/os/dofile 제거, 호스트 함수 상태를 `host_data.cpp` static에서 세션 객체로. 그 다음 백그라운드 스레드 풀 + 취소. 웹 `portrayal/src/lua-runtime.ts`, `worker-pool.ts`.
- [ ] **렌더 스레드.** D2D 리소스 소유권 정리 후(`D2D1Resources`, `BindDC` 경로). 웹의 "마지막 프레임을 유사변환으로 보여주고 새 프레임 대기" 패턴은 D2D에서도 가능.
- [ ] **XSLT 디스크 왕복 제거.** libxslt를 메모리 버퍼로. 웹과 무관하지만 같은 단계에서.
- [ ] **S-98 부속서 D.** WLA(`core/water-level-adjustment.ts`), 강화 안전등심선, E-1 셀 선택(`dataset-loading.ts`), 과축척, 업데이트 하이라이트(`update-report.ts`), 픽 리포트(14장). 현재 C++ IC 그리기를 이 모델 위로 옮김.
- [ ] **셀 팩(`.bmc`).** 웹 `docs/plans/cell-pack-format.md`. C++ SENC 직렬화로 쓸 가치가 있는지 평가 후 결정. 포맷을 공유하면 웹과 C++가 같은 캐시를 읽을 수 있다.

### 이식하지 않을 것

- 워커 경계용 타입드 어레이 패킹(`core/transfer.ts`), GC 예산(`CACHED_BYTES_PER_INSTRUCTION`). 브라우저 제약 때문에 존재한다. (웹 역이식 문서도 같은 결론.)
- OpenLayers/Cesium 어댑터, Canvas/WebGL 렌더러 자체. 백엔드가 다르다.
- 브라우저 `XSLTProcessor` 의존 구조. C++는 libxslt가 더 낫다.

## 3. 반대 방향 (C++ → 웹) 후보

웹 저장소에서 다룰 항목이지만 여기에 적어 두어 양쪽이 같은 목록을 본다.

- ISO 8211 쓰기와 업데이트 파일 생성(KHOA 뷰어 `S101Cell::Save`, `CreateUpdateFile`; OpenS100으로 업스트림 후).
- `HostSpatialRelate` 구현(`S100Geometry` 기반).
- 피처·기하 편집 모델(스냅, 실행 취소, 관계·자동 관계).
- S-100 4.0 데이터 로딩.
- 복합 선 스타일의 모든 컴포넌트 그리기(웹은 최상위만).

## 4. 교차 검증 체계 (두 저장소 공통)

- 공통 TDS: `D:\s100-reference\tds`와 `ProgramData/KRS_TDS`(저장소에서 분리 예정). 두 저장소가 같은 파일 목록을 참조하는 `conformance/manifest.json`을 둔다.
- 기대 결과 스냅샷: 파일별 (제품 판별 결과, 피처 수, 검증 이슈 목록, 디스플레이 리스트 JSON 해시). 웹 vitest와 C++ gtest가 같은 스냅샷을 읽는다.
- 스냅샷 저장소는 처음엔 각 저장소의 `test/fixtures`에 복사본으로 두고, 안정되면 `opens100-conformance` 저장소로 분리.

## 5. 진행 기록

| 날짜 | 항목 | 커밋 | 비고 |
|---|---|---|---|
| 2026-11-08 | 문서 작성, OpenS100 저장소로 이동 | | opens100-web 인벤토리 조사 기반 |

## 변경 이력

- 2026-11-08 초안 작성.
