# System Database: 로컬 S-100 데이터 통합 관리 (2026-11-10)

지침 26의 설계와 작업 계획. 데이터셋·FC·PC·지원 파일을 한 로컬 DB가 관리하고, 화면에 올릴 데이터셋은 그 DB의 메타데이터로 S-98 부속서 E-1 알고리즘이 고른다.

## 1. 지금 구조와 문제

Dataset Manager(앱 `OpenS100/DATASET_SUPPORTdlg`, `DataSetManagerSupport`)의 현재 동작:

- DB는 `..\TEMP\DatasetManager.db`(실행 위치 기준 상대 경로). 테이블 `FeatureCatalogue`, `PortrayalCatalogue`, `DatasetFiles`, `SupportFiles`.
- 다이얼로그를 닫으면 `DatasetFiles`·`SupportFiles`를 전부 지우고 다시 넣는다. 동시에 목록으로 `..\TEMP\IniCatalog.XML`을 새로 쓰고, 파일을 `..\TEMP\`로 복사하고, 그 카탈로그를 레이어 하나로 연다. 이 세션에서만 `S100ExchangeCatalogue::DrawFiles` → `S100Utilities::SelectDataCoverages`가 로딩·언로딩을 한다.
- 재시작하면 `COpenS100View::OnCreate`가 `DatasetFiles`의 **원본 경로**를 하나씩 `AddLayer` 한다. 808셀이면 808셀을 모두 파싱해 올리고, 선택 알고리즘 없이 MBR만 보고 전부 그린다.

문제:

1. **메타데이터가 DB에 없다.** `DatasetFiles`는 `fileName`, `filePath`뿐이다. 커버리지·축척·제품·판 정보가 XML(`IniCatalog.XML`)에만 있어서, 선택 알고리즘을 돌리려면 XML을 거쳐야 한다. 그 XML은 앱이 만든 임시 파일이라 표준 교환세트도 아니다.
2. **두 저장소가 어긋난다.** DB에는 원본 경로, `TEMP`에는 복사본, XML에는 복사본 기준 상대 경로가 있다. 원본을 옮기거나 지우면 재시작 후 동작과 세션 중 동작이 달라진다.
3. **`SupportFiles` 테이블이 생성되지 않는다.** `CREATE TABLE` 문에 `filePath` 열이 두 번 있어 `duplicate column name`으로 실패한다. 오류는 `TRACE`로만 나가므로, 지원 파일 등록은 매번 조용히 실패한다.
4. 4인자 `insertData`(resourcePurpose)는 없는 열을 쓰고 자리표시자가 2개뿐이다(호출하는 곳은 없다).
5. FC/PC는 경로만 보고 시작 시 전부 파싱한다. 데이터셋이 참조하는 제품·버전과의 연결이 DB에 없다.
6. 전부 앱(MFC) 코드다. 지침 1·18에 따르면 이 기능은 라이브러리 코어에 있어야 한다.

## 2. 목표

- **System Database(SDB)**: 로컬에 설치된 S-100 자원(데이터셋과 그 업데이트, FC, PC, 지원 파일, 가져온 교환세트)을 관리하는 하나의 DB.
- **시작할 때 데이터셋 파일을 열지 않는다.** SDB의 메타데이터(커버리지 폴리곤, 축척, 경계 상자)만 메모리에 올리고, 뷰가 바뀔 때 E-1 선택 결과에 따라 필요한 데이터셋만 열고 닫는다.
- FC/PC는 SDB에 (제품, 버전)으로 등록하고, 데이터셋이 요구하는 제품·버전으로 찾는다(`CatalogManager` → 장기적으로 `CatalogueRegistry`).
- 교환세트(`CATALOG.XML`) 가져오기, 단일 파일 가져오기, 삭제·교체가 모두 SDB 트랜잭션 하나로 끝난다. 임시 XML은 쓰지 않는다.
- 라이브러리 코드(순수 C++20, `sqlite3`, `std::filesystem`)로 둔다. 앱 다이얼로그는 SDB API 위의 얇은 UI다.

## 3. 설계

### 3.1 책임 분리 (지침 22)

| 구성 요소 | 책임 | 위치 |
|---|---|---|
| `s100::sdb::SystemDatabase` | SQLite 연결, 스키마 버전·마이그레이션, 트랜잭션, 레코드 CRUD. 파일 내용은 모른다 | `s100-sdk` |
| `s100::sdb::Importer` | 교환세트·단일 파일을 읽어 메타데이터 추출(데이터셋의 DataCoverage 피처, 제품·판·업데이트 번호, FC/PC 제품·버전), 관리 저장소로 복사, SDB에 기록 | `s100-sdk` (파서는 `s100-core`/`s100-iso8211` 사용) |
| `s100::sdb::ResourceStore` | 관리 저장소의 디렉터리 배치와 파일 복사·삭제. 경로는 SDB 루트 기준 상대 경로로 기록 | `s100-sdk` |
| `CatalogueRegistry` | (제품, 버전) → FC/PC 해석. SDB에서 목록을 받고 실제 파싱은 처음 필요할 때 | `s100-core` (기존 `CatalogManager` 대체) |
| `DatasetSelector` | S-98 E-1.3 데이터 커버리지 선택. 입력은 SDB 커버리지 인덱스와 뷰, 출력은 그릴 데이터셋 ID와 순서 | `s100-sdk` (`S100Utilities::SelectDataCoverages` 대체, opens100-web `dataset-loading.ts`와 맞춤) |
| `DatasetCache` | 열린 데이터셋의 LRU 캐시. 선택에서 빠졌다가 다시 들어온 셀을 다시 파싱하지 않는다 | `s100-sdk` |

### 3.2 스키마 (v1)

경로는 모두 SDB 루트 기준 상대 경로(UTF-8). 시간은 ISO 8601 문자열.

스키마 버전은 `PRAGMA user_version`에 둔다(별도 테이블 없음). 연결마다 `PRAGMA foreign_keys = ON`, 파일 DB는 WAL. 아래는 v1이고, 기준은 코드(`SystemDatabase/Schema.cpp`)다. 배포된 단계는 고치지 않고 다음 단계를 덧붙인다.

```sql
CREATE TABLE exchange_set (
  id INTEGER PRIMARY KEY,
  source_path TEXT NOT NULL,
  identifier TEXT NOT NULL DEFAULT '',
  issue_date TEXT NOT NULL DEFAULT '',
  imported_at TEXT NOT NULL
);

CREATE TABLE feature_catalogue (
  id INTEGER PRIMARY KEY,
  product TEXT NOT NULL,
  version TEXT NOT NULL,
  path TEXT NOT NULL UNIQUE,
  sha256 TEXT NOT NULL,
  issue_date TEXT NOT NULL DEFAULT '',
  imported_at TEXT NOT NULL,
  UNIQUE (product, version)
);

CREATE TABLE portrayal_catalogue (
  id INTEGER PRIMARY KEY,
  product TEXT NOT NULL,
  version TEXT NOT NULL,
  path TEXT NOT NULL UNIQUE,
  sha256 TEXT NOT NULL,
  issue_date TEXT NOT NULL DEFAULT '',
  imported_at TEXT NOT NULL,
  UNIQUE (product, version)
);

CREATE TABLE dataset (
  id INTEGER PRIMARY KEY,
  name TEXT NOT NULL,
  product TEXT NOT NULL,
  product_edition TEXT NOT NULL DEFAULT '',
  format TEXT NOT NULL CHECK (format IN ('iso8211', 'gml', 'hdf5')),
  edition INTEGER,
  update_number INTEGER NOT NULL DEFAULT 0,
  issue_date TEXT NOT NULL DEFAULT '',
  path TEXT NOT NULL UNIQUE,
  min_x REAL, min_y REAL, max_x REAL, max_y REAL,
  status TEXT NOT NULL DEFAULT 'active' CHECK (status IN ('active', 'cancelled')),
  exchange_set_id INTEGER REFERENCES exchange_set(id) ON DELETE SET NULL,
  imported_at TEXT NOT NULL,
  UNIQUE (product, name)
);
CREATE INDEX dataset_exchange_set ON dataset(exchange_set_id);

CREATE TABLE dataset_update (
  dataset_id INTEGER NOT NULL REFERENCES dataset(id) ON DELETE CASCADE,
  update_number INTEGER NOT NULL,
  path TEXT NOT NULL UNIQUE,
  issue_date TEXT NOT NULL DEFAULT '',
  applied INTEGER NOT NULL DEFAULT 0 CHECK (applied IN (0, 1)),
  PRIMARY KEY (dataset_id, update_number)
);

CREATE TABLE data_coverage (
  id INTEGER PRIMARY KEY,
  dataset_id INTEGER NOT NULL REFERENCES dataset(id) ON DELETE CASCADE,
  optimum_display_scale INTEGER,
  minimum_display_scale INTEGER,
  maximum_display_scale INTEGER,
  min_x REAL NOT NULL, min_y REAL NOT NULL, max_x REAL NOT NULL, max_y REAL NOT NULL,
  polygon BLOB NOT NULL
);
CREATE INDEX data_coverage_dataset ON data_coverage(dataset_id);

CREATE TABLE support_file (
  id INTEGER PRIMARY KEY,
  name TEXT NOT NULL,
  path TEXT NOT NULL UNIQUE,
  purpose TEXT NOT NULL DEFAULT '',
  edition INTEGER,
  format TEXT NOT NULL DEFAULT '',
  issue_date TEXT NOT NULL DEFAULT '',
  sha256 TEXT NOT NULL,
  imported_at TEXT NOT NULL
);

CREATE TABLE dataset_support_file (
  dataset_id INTEGER NOT NULL REFERENCES dataset(id) ON DELETE CASCADE,
  support_file_id INTEGER NOT NULL REFERENCES support_file(id) ON DELETE CASCADE,
  PRIMARY KEY (dataset_id, support_file_id)
);
CREATE INDEX dataset_support_file_reverse ON dataset_support_file(support_file_id);
```

- 커버리지는 **데이터셋 안의 DataCoverage 피처**에서 읽는다(S-98 E-1, opens100-web `readDataCoverages`와 같음). 교환세트의 `S100XC:dataCoverage`는 데이터셋에 커버리지가 없을 때만 쓴다. 가져올 때 한 번 파싱하므로 시작 시에는 파싱이 없다.
- 같은 (제품, 이름)이 다시 들어오면 판·업데이트 번호를 비교해 교체·업데이트 추가·거부를 정한다. 판 0 업데이트(취소)는 `status = 'cancelled'`.
- 공간 질의는 처음에는 메모리 인덱스(시작 시 `data_coverage` 전체를 읽어 경계 상자 트리)로 한다. 808셀 규모에서는 충분하다. 필요해지면 SQLite R*Tree 모듈을 쓴다.

### 3.3 관리 저장소

```
<SDB root>/
  sdb.sqlite
  FC/<product>/<version>/<file>.xml
  PC/<product>/<version>/...            (PC folder as delivered)
  DATASET/<product>/<name>/<name>.000, .001, ...
  SUPPORT/<product>/<file>
```

SDB 루트 기본값은 사용자 데이터 폴더(Windows `%LOCALAPPDATA%\OpenS100\SDB`, Linux `$XDG_DATA_HOME/opens100/sdb`)이고, API로 바꿀 수 있다. 라이브러리는 루트 경로를 인자로 받을 뿐 기본값을 정하지 않는다. 기본값은 앱이 정한다.

### 3.4 런타임 흐름

1. 앱 시작: `SystemDatabase::open(root)` → `CatalogueRegistry`에 FC/PC 목록 등록(파싱은 지연) → `DatasetSelector`에 커버리지 인덱스 적재. 데이터셋 파일은 열지 않는다.
2. 뷰 변경: `DatasetSelector::select(view, scale)` → 데이터셋 ID 목록(그리기 순서 포함) → `DatasetCache`가 없는 것만 열고(해당 제품의 FC/PC를 레지스트리에서 받음) 나머지는 캐시에 남긴다 → 렌더.
3. 가져오기·삭제: `Importer`가 트랜잭션으로 처리하고, 끝나면 선택기 인덱스와 캐시를 무효화한다.

`S100ExchangeCatalogue` 레이어(교환세트를 설치하지 않고 바로 보기)는 유지할지 결정이 필요하다(6절).

## 4. 작업 항목

- [x] **SDB-1** 라이브러리 `SystemDatabase`: 스키마 v1, 마이그레이션 틀, 트랜잭션, 레코드 CRUD. gtest는 `:memory:` DB로. (2026-11-10, 73234b71)
  - 새 디렉터리 `SystemDatabase/`(MFC 없음, sqlite3만 의존). VS 솔루션의 `01. LIB`에 6개 구성으로 추가, CMake 타깃 `opens100::sdb`(옵션 `OPENS100_BUILD_SYSTEMDATABASE`), 포트 기능 `system-database`(기본 기능 아님).
  - 구성: `Sqlite`(연결·문장·트랜잭션 RAII, 실패는 `DatabaseError`), `Schema`(`user_version` 기반 마이그레이션), 저장소 5개(`ExchangeSet`, `Catalogue`(FC·PC 겸용), `Dataset`(업데이트·지원 파일 연결 포함), `Coverage`, `SupportFile`), 파사드 `SystemDatabase`.
  - 조회가 없을 수 있는 경우는 `std::optional`, DB 오류·제약 위반은 예외(지침 25c).
  - 검증: `SystemDatabase.*` 16개 통과(전체 46 통과, 1 SKIP은 기존 환경 의존 테스트). 한글 경로 DB 파일, 새 스키마 거부, 외래 키 연쇄 삭제, 트랜잭션 롤백 포함. CMake Release `/W4 /WX` 통과, 설치된 패키지를 외부 프로젝트에서 `find_package(opens100)` → `opens100::sdb`로 링크해 실행 확인.
- [x] **SDB-2** `ResourceStore`: 루트 배치, 복사(해시 계산), 삭제. 경로 UTF-8·`std::filesystem::path`. (2026-11-10, 85f44c84)
  - `ResourceStore`(배치·복사·이동·삭제), `Sha256`(FIPS 180-4 자체 구현, 의존성 추가 없음), `Utf8Path.h`(경로 ↔ UTF-8, Windows ANSI 코드 페이지를 거치지 않음).
  - 제품·버전·이름은 한 경로 조각으로만 쓰인다(`/ \ : * ? " < > |`·제어 문자·끝의 점과 공백을 `_`로). `absolute()`는 절대 경로와 `..`을 거부한다.
  - 파일 복사는 `<대상>.part`에 쓰고 이름을 바꿔 넣는다. 삭제·이동은 비게 된 상위 폴더를 루트 전까지 지운다.
  - 검증: `Sha256.*` 2개(표준 벡터, 나눠 넣기), `ResourceStore.*` 6개(한글 루트 포함) 통과.
- [x] **SDB-3** `Importer`: 단일 FC, 단일 PC 폴더, 단일 데이터셋(8211부터), 지원 파일, 교환세트(`CATALOG.XML`). 데이터셋 DataCoverage 추출. 판·업데이트 비교 규칙. (2026-11-10, 9098bc41)
  - 리더는 인터페이스(`Inspection.h`: `DatasetInspector`, `CatalogueInspector`, `ExchangeSetReader`)이고 구현은 파서가 있는 GISLibrary(`SdbReaders.h`)에 둔다. SDB 모듈은 여전히 sqlite3에만 의존한다. 형식 추가는 리더 추가로 끝난다.
  - `S101DatasetInspector`: FC 없이 8211 레코드에서 DSID와 DataCoverage 피처(`optimum/minimum/maximumDisplayScale`, 면의 RIAS 링)를 읽는다. `DSED`는 "판.업데이트"(`2.4` = 2판 4차 업데이트 포함), `PROF` 2는 업데이트 파일. `fileReference`·`pictorialRepresentation` 값을 지원 파일 참조로 모은다.
  - `XmlCatalogueInspector`: FC/PC의 제품·버전을 XML 루트에서만 읽는다(전체 파싱 없음). `ExchangeCatalogueReader`: `CATALOG.XML`(경로는 `ExchangeCatalogue::OpenFile`로 한글 경로 가능).
  - 판·업데이트 규칙은 `decideDataset` 순수 함수: 새 데이터셋, 새 판(교체), 같은 판 재발행(교체), 같은 파일(변경 없음), 다음 업데이트(기록, 적용은 SDB-8), 판 0 업데이트(취소, 선택에서 빠짐). 거부: 이전 판, 빠진 업데이트, 기반 없는 업데이트, 취소된 데이터셋의 업데이트.
  - 한 번의 호출이 트랜잭션 하나. 파일은 저장소 안 `STAGING/`에 복사했다가 커밋 후 제자리로 옮긴다(교체되는 판의 폴더는 그 전에 지움). 실패하면 설치된 파일은 그대로다. 커밋 후 옮기기 실패는 `installProblems`로 보고한다.
  - 교환세트: 모든 데이터셋을 트랜잭션 전에 읽고(느린 부분), 기반 → 업데이트 순으로 설치, 읽지 못한 데이터셋은 그것만 거부. 지원 파일을 먼저 설치하고 데이터셋에 연결한다. 커버리지는 데이터셋의 DataCoverage, 없을 때만 카탈로그의 것.
  - 검증: 가짜 리더로 규칙·파일 처리 12개, 실제 리더 5개 통과. KOR_TDS 808셀 전체 가져오기 19초(Release x64, `S100_SDB_FULL_IMPORT=1`일 때만 도는 테스트), 거부 0, 모든 셀에 커버리지(821개), 지원 파일 22개·연결 850건, 커버리지 WKB 합계 2.9 MB.
- [x] **SDB-4** `CatalogueRegistry`를 SDB에서 채우기. `initFCList`/`initPCList` 제거. 데이터셋 제품·버전으로 FC/PC 해석(ready/fallback/unresolved, opens100-web `catalogue-registry.ts`와 맞춤). (2026-11-10, f20ca04e)
  - 판정은 SDB 모듈의 순수 함수 `resolveCatalogue`(`CatalogueResolution.h`). 웹 테스트의 경우를 같은 기대값으로 옮겼다. `2.0`과 `2.0.0`은 같은 버전(KOR 셀은 `2.0`, FC/PC는 `2.0.0`).
  - 레지스트리는 별도 클래스를 새로 만들지 않고 기존 `CatalogManager`에 붙였다(뷰어 전체가 이미 이 객체를 돌려 쓴다): `registerFeatureCatalogue`/`registerPortrayalCatalogue`/`registerInstalled(SDB, store)`는 아무것도 읽지 않고, `resolve(product, version)`가 처음 필요할 때 읽는다. 제품별 `getFC/getPC`도 등록된 최신판을 먼저 읽는다. 읽기 실패는 unresolved로 보고하고 다시 시도하지 않는다. PC를 읽을 때마다 `onPortrayalLoaded`로 렌더 캐시 준비를 앱에 맡긴다.
  - `initFCList`/`initPCList` 제거와 앱의 등록 호출은 앱을 SDB로 옮기는 SDB-7에서 한다(`DataSetManagerSupport` 자체가 없어진다).
  - 뷰어 프로젝트가 SystemDatabase를 링크한다.
- [x] **SDB-5** `DatasetSelector`: opens100-web `selectDataCoverages`(경계값 수정 포함)를 역이식하고, 같은 입력으로 두 구현의 결과가 같은지 테스트. `S100Utilities::SelectDataCoverages`의 Clipper 경로를 대체한다. (2026-11-10, 658b0c45)
  - `SystemDatabase/DatasetSelection.h`: `scaleBandOf`(1:10,000,000은 1번 띠), `scaleBandsOf`, 셀 격자 `DeviceRegion`, `selectDataCoverages`(지상 경계 생략, 고정(pinned) 커버리지 포함), DB에서 활성 커버리지를 한 번 읽는 `DatasetSelector`. 웹 테스트의 사례와 기대값을 그대로 옮겨 같은 결과를 확인했다.
  - 웹의 `coverageDisplayMasks`(두 선택 사이 축척이 다른 해도가 겹쳐 보이지 않게 하는 마스크)는 렌더 쪽 일이라 옮기지 않았다. 지금은 선택된 셀을 작은 축척부터 그려 큰 축척이 위에 오게만 한다.
  - `GISLibrary::selectionView(Scaler&)`: 축척, 화면 크기, 경위도 → Mercator → 장치 좌표, 회전이 없을 때 지상 경계.
  - `S100ExchangeCatalogue`(설치 없이 교환세트 보기)도 같은 선택기를 쓴다(6절 결정). Clipper 경로(`S100Utilities::SelectDataCoverages`, 그것만 쓰던 `SCommonFuction` 함수 2개)와 쓰이지 않던 `DrawEx`를 지웠다.
  - 오프스크린 확인(`RenderSmoke.ExchangeSetLoadsOnlyTheSelectedCells`): KOR 교환세트를 바로 열어 부산 1:52,843에서 808셀 중 2개, 남해안 1:1,321,085에서 4개만 열고 그린다. 화면은 정상(사람이 PNG 확인).
- [ ] **SDB-6** `DatasetCache`(LRU, 상한은 셀 수 또는 메모리).
- [ ] **SDB-7** 앱: Dataset Manager 다이얼로그를 SDB 위의 UI로 다시 만든다. `IniCatalog.XML`·`TEMP` 복사·`DatasetManager.db` 제거. `OnCreate`의 일괄 `AddLayer` 제거. 기존 `DatasetManager.db`에서 FC/PC/데이터셋 경로를 읽어 한 번 가져오는 마이그레이션.
- [ ] **SDB-8** 업데이트 파일 적용(`.001`~), 취소, 교환세트 서명(`CATALOG.SIGN`) 검증 연결.
- [ ] **SDB-9** 화면 검증(지침 23): KOR_TDS 808셀 교환세트를 가져와 축척을 바꿔 가며 선택·그리기를 캡처하고, 시작 시간·메모리를 기록한다.

## 5. 성능 목표

- KOR_TDS 808셀 기준: 가져오기 후 앱 시작에서 데이터셋 파싱 0회, 시작 시간은 FC/PC 목록 적재 수준.
- 뷰 변경 시 선택 계산 10 ms 이하(메모리 인덱스).

## 6. 결정 사항과 열린 질문

결정 (2026-11-10, 사용자):
- **설치 없이 `CATALOG.XML`을 바로 여는 기능(`S100ExchangeCatalogue` 레이어)은 남긴다.** 단, SDB와 같은 `DatasetSelector`를 쓰게 바꾼다(SDB-5).
- **앱의 SDB 루트 기본 위치는 사용자 데이터 폴더**(3.3)다.
- **가져올 때 파일을 관리 저장소로 복사한다**(3.3). 원본이 옮겨지거나 지워져도 동작이 같고, 업데이트·교체를 SDB가 일관되게 관리한다. SDB의 경로는 모두 SDB 루트 기준 상대 경로다.

## 변경 이력

- 2026-11-10 초안. 지침 26에서 시작.
- 2026-11-10 결정 반영: 교환세트 바로 열기 유지(같은 선택기), SDB 기본 위치는 사용자 데이터 폴더.
- 2026-11-10 결정 반영: 가져올 때 관리 저장소로 복사. 스키마 버전은 `PRAGMA user_version`.
