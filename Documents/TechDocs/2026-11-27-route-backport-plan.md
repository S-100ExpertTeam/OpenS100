# 항로 기능 역이식 계획: opens100-web → OpenS100 (2026-11-27)

- 상태: 계획만 작성. 미착수.
- 지침: 1(라이브러리), 5(opens100-web 역이식, 개념 모델·모듈 경계 맞춤), 18(전 플랫폼), 22(SOLID), 25(현대적 C++).
- 기준: opens100-web `b134c7b`(bluemap-github 체크아웃), OpenS100 `3c9a9248`(`dev-jogm`).
- 웹 쪽 계획 문서: `docs/plans/route-monitoring-plan.md`(1~3단계 완료), `docs/plans/route-edit-core-plan.md`(미착수).

## 1. 지금 상태

### 1.1 OpenS100: 항로 기능이 없다

| 있는 것 | 위치 | 항로에 쓰이는 곳 |
|---|---|---|
| S-421 FC 1.0.0, S-421 PC(XSLT: `RouteWaypoint`, `RouteWaypointLeg`, `RouteActionPoint`, 선 스타일 `RTEACT*`/`RTEALT*`) | `ProgramData/FC/S-421_FC.xml`, `ProgramData/PC/S421_Portrayal` | S-421 GML을 데이터셋으로 열어 **보는 것**만 된다 |
| 항정선·측지선 정·역 문제(미터, GeographicLib) | `LatLonUtility/Geodesy.h` (`s100::geodesy`) | 구간 코스·거리 |
| 경보 카탈로그 파서, 인스트럭션의 `AlertReference` 보존 | `PortrayalCatalogue/AlertCatalog.*`, `GISLibrary/SENC_AlertReference.h`, `DisplayListJson` | 항로 검사의 재료. **탐지(영역과 만나는지)는 없다** |
| 그리기 추상 | `GISLibrary/IRenderer.h`, `ViewFrame.h` | 항로 그리기 |
| GML 읽기 | `GISLibrary/S10XGML` | S-421 읽기 |

없는 것: 항로 모델, 변침점 편집, 스케줄, 선회 호, XTD 회랑, 항로 검사, 항로 감시(XTE·경보), 자선·오토파일럿, S-421/RTZ 읽기·쓰기(모델로), 항로 그리기·편집 UI. 뷰어의 `beModifyWaypoint`/`modifyWaypointIndex`(`OpenS100/OpenS100View.h:68`)는 쓰이지 않는 흔적이다.

### 1.2 opens100-web: 계획 → 검사 → 감시가 돈다

| 묶음 | 파일 (`packages/core/src/`) | 줄 | 내용 |
|---|---|---|---|
| 모델·편집 | `route/route.ts`, `s421-model.ts`, `rtz-model.ts`, `action-point.ts` | 734 | S-421(IEC 63173-1) 속성명 그대로의 Route/Waypoint/ActionPoint, `validateRoute`, append/insert/delete/reverse, `routeLegs`, `recalculateSchedule` |
| 기하 | `route/turn.ts`, `corridor.ts`, `vector.ts`, `planar.ts` | 450 | 선회 호, XTL 회랑(오프셋 선 + 교차 트림 + 커넥터 + 바깥쪽 호), 구간별 닫힌 링, `crossTrackDistance` |
| 검사 | `route/route-check.ts`, `look-ahead.ts`의 `detectAlerts`, `alert-catalogue.ts` | 89 + 365 | 구간 회랑 링을 `routePlan` 모드로 경보 탐지, `routeWaypointLegIssue` 요약 |
| 감시 | `route/route-tracker.ts`, `route-monitor.ts` | 271 | WOP 기반 목표 WP 전환, XTE·DTW·BTW·TTG·ETA, `offTrack`/`waypointApproach`/`endOfRoute` |
| 시뮬레이션 | `own-ship.ts`, `autopilot.ts`, `own-ship-fix.ts`, `route/track-pilot.ts` | 728 | Nomoto 1차 자선, 캐스케이드 오토파일럿, Track 모드 |
| 그리기·편집 | `packages/openlayers/src/route-layer.ts`, `route-editor.ts` | 748 | OpenLayers에 묶여 있다. core로 내리는 계획(`route-scene`/`route-style`/`route-hit-test`/`route-edit`)은 미착수 |
| 앱 | `apps/viewer-ol/src/route.ts` | 432 | Route 카드, localStorage |
| 테스트 | `packages/core/test/route*.test.ts` 6개 | 593 | 이식 정확성의 기준 |

웹에도 **아직 없는 것**: S-421/RTZ 읽기·쓰기(원천 `web-ecdis/frontend/src/domain/route/{S421Converter,S421V1Parser,S421V1Writer,RtzConverter}.ts`와 테스트가 로컬에 있다), 기록·재생, 센서 공급원, 액션 포인트 편집.

다른 원천: KHOA 뷰어 `S-100SimpleEditor/RoutePlanner`(C++/MFC, RTZ 리더가 pugixml판까지 있다). 모델은 웹 것을 따르고, RTZ 읽기를 옮길 때만 대조한다.

## 2. 결론

1. 웹 `core/src/route`를 **새 모듈 `opens100::route`(폴더 `Route/`, 네임스페이스 `s100::route`)** 로 1:1 이식한다. 파일 이름과 함수 경계를 웹과 맞춘다(지침 5). `latlon`에만 의존하고 MFC·Windows·렌더러를 모른다.
2. 경보 탐지(`detectAlerts`)가 먼저 있어야 항로 검사가 된다. 이것은 항로와 별개로 전방 감시에도 쓰이므로 **`s100::alert`로 따로 이식**하고, 항로 모듈은 인터페이스(`IAlertDetector`)로만 본다(의존성 역전).
3. 그리기·편집은 웹의 OL 코드를 옮기지 않는다. 웹이 계획만 세운 **scene/style/hit-test/edit-session을 C++에서 먼저 만든다.** 어댑터는 `IRenderer`(그리기)와 뷰어의 마우스 핸들러(입력) 둘이다. 웹이 나중에 같은 모양으로 내리면 두 저장소가 맞는다.
4. S-421/RTZ는 web-ecdis의 변환기와 테스트를 옮긴다. 웹보다 OpenS100이 먼저 갖게 된다.
5. 단위는 웹과 같게 둔다: 모델 내부 NM·kn, XTE와 S-421은 m. `s100::geodesy`(미터)와의 환산은 모듈 경계 한 곳에서 한다. 테스트 수치를 그대로 옮기기 위해서다.

## 3. 설계

### 3.1 모듈과 의존

```
latlon (Geodesy, +Planar)  ←  route (모델·기하·tracker·monitor·ownship·scene·edit)
portrayal (디스플레이 리스트) ←  alert (DetectAlerts, look-ahead 영역)   — IAlertDetector 구현
dataset (pugixml)           ←  route-io (S-421 2.0/1.0, RTZ 읽기·쓰기)
view                         ←  RouteLayer (RouteScene → IRenderer)
OpenS100 (MFC)               ←  항로 패널, 마우스 → RouteEditSession
```

### 3.2 TypeScript → C++ 대응

| 웹 | C++ |
|---|---|
| `Waypoint` 판별 합집합(start/intermediate/end) | `struct Waypoint { WaypointType type; … std::optional<LegParams> leg; std::optional<double> radius; }`. 종류와 필드의 일치는 `ValidateRoute`가 지킨다 |
| `Route = Waypoint[]`, 불변 편집 함수 | `using Route = std::vector<Waypoint>`; `[[nodiscard]] Route AppendWaypoint(const Route&, Position, const RouteDefaults&)` 등 값 반환 |
| `Date` | `std::chrono::sys_time<std::chrono::milliseconds>` |
| 선택 필드 | `std::optional` |
| `throw new Error` (잘못된 색인) | `std::optional`/`Result` 반환. 예외는 쓰지 않는다(지침 25c) |
| `RouteIssue.path: (string\|number)[]` | `std::vector<std::variant<std::string, std::size_t>>` |
| `crypto.randomUUID()` | 호스트가 주는 id 생성기, 기본은 `<random>` UUID v4 |
| `mercatorAbout`, `inRing`, `segmentsMeet` | `LatLonUtility/Planar.h` (등장위도는 GeographicLib) |
| `WeakMap` 회랑 캐시 | `RouteMonitor` 객체가 회랑을 멤버로 든다 |
| `onChange` 콜백 | `std::function` |

### 3.3 그리기와 편집

- `RouteScene BuildRouteScene(route, actionPoints, check)`: 그리는 순서대로의 항목(`waypoint`, `leg`, `turn`, `limit`, `action-point`, `action-radius`, `leg-label`, `highlight`)과 지리 좌표. 웹 `route-layer.ts`의 `redraw()` 본문이 원천이다.
- `RouteStyle`: 색 토큰(`PLRTE`, `APLRT`, `CHGRD`, `CHBLK`, `DNGHL`, `CHYLW`)과 굵기·점선·반지름. 색은 S-101 PC의 색 프로파일에서 찾고 없으면 대체 색.
- `RouteLayer`(`view`): 장면을 `Scaler`로 화면 좌표로 옮겨 `IRenderer::StrokePath`/`FillPath`/`FillText`로 그린다. 차트 위, 자선 아래.
- `HitTestRoute(scene, pixel, project, tolerance)`와 `RouteEditSession`(append/edit 모드, click/doubleClick/dragStart·Move·End/contextMenu/key): 화면 픽셀과 지리 좌표만 다룬다. 뷰어는 `OnLButtonDown` 등에서 이것을 부르고, `dragStart`가 `true`일 때만 지도 이동을 막는다.
- 파일로 연 S-421 GML은 지금처럼 PC(XSLT)로도 그려진다. "항로로 가져오기"를 하면 모델이 되어 `RouteLayer`가 그린다. 편집 중인 항로를 PC로 그리는 것(모델 → S-421 피처 → XSLT)은 이번 범위 밖이다(6절 결정 2).

## 4. 단계

각 단계는 따로 커밋하고, 끝날 때 Windows 솔루션 테스트와 CMake(Linux) 빌드가 통과해야 한다.

| 단계 | 내용 | 원천 | 산출물 | 검증 | 크기 |
|---|---|---|---|---|---|
| **RT-0** | 평면 기하. `Planar.h`: `MercatorAbout`, `InRing`, `SegmentsMeet`, `SegmentIntersection`, 벡터, `PlanarLength` | `planar.ts`, `route/vector.ts` | `LatLonUtility/Planar.*` | `PlanarTest.cpp`: 날짜변경선 왕복, 위도별 길이 | S |
| **RT-1** | 모델·검증·편집·스케줄. CMake 타깃 `opens100::route`, vcxproj | `route.ts`, `s421-model.ts`, `rtz-model.ts`, `action-point.ts` | `Route/Route.*`, `S421Model.h`, `RtzModel.h`, `ActionPoint.*` | `route.test.ts`(174줄) 이식 | M |
| **RT-2** | 선회 호, 회랑, XTE | `turn.ts`, `corridor.ts` | `Route/Turn.*`, `Corridor.*` | `route-turn.test.ts`, `route-corridor.test.ts` 이식 | M |
| **RT-3** | 경보 탐지. 디스플레이 리스트의 `AlertReference`가 붙은 기하를 영역(닫힌 링)과 대조, 경보 카탈로그의 `routePlan`/`routeMonitor` 우선순위 | `look-ahead.ts`, `area-clip.ts`, `alert-catalogue.ts` | `GISLibrary/AlertDetection.*`(`s100::alert`) | `look-ahead.test.ts` 이식 + KOR_TDS 셀에서 안전등심선 교차 | M~L |
| **RT-4** | 항로 검사 | `route-check.ts` | `Route/RouteCheck.*`, `IAlertDetector` | `route-check.test.ts` 이식 | S |
| **RT-5** | 그리기. `RouteScene`, `RouteStyle`, `RouteLayer`. `examples/view`에 `--route` | `route-layer.ts` | `Route/RouteScene.*`, `RouteStyle.*`, `GISLibrary/RouteLayer.*` | 장면 단위 테스트 + 차트 위 항로 PNG(Windows·Linux) | M |
| **RT-6** | S-421 2.0 읽기·쓰기, S-421 1.0, RTZ 1.0/1.2 | web-ecdis `S421Converter.ts`, `S421V1*.ts`, `RtzConverter.ts`; KHOA `RTZReaderByPugiXml` 대조 | `Route/io/S421Reader.*`, `S421Writer.*`, `RtzReader.*`, `RtzWriter.*` | web-ecdis 테스트 4개 이식, 읽기 → 쓰기 → 읽기 왕복 | M~L |
| **RT-7** | 편집. `HitTestRoute`, `RouteEditSession`, 뷰어 연결(툴바 모드, 우클릭 메뉴, WP 표 패널, 열기·저장). 죽은 `beModifyWaypoint` 제거 | `route-editor.ts`, `apps/viewer-ol/src/route.ts` | `Route/RouteHitTest.*`, `RouteEdit.*`, `OpenS100/Route*` | 세션 단위 테스트 + 뷰어에서 손으로(이어 그리기, 끌어 옮기기, 구간에 삽입, 삭제, 뒤집기) | L |
| **RT-8** | 감시. tracker, monitor. 입력은 `OwnShipFix` 하나 | `route-tracker.ts`, `route-monitor.ts`, `own-ship-fix.ts` | `Route/RouteTracker.*`, `RouteMonitor.*`, `OwnShipFix.h` | `route-tracker.test.ts`, `route-monitor.test.ts` 이식 | M |
| **RT-9** | 시뮬레이션. 자선 모델, 오토파일럿, Track 모드, 전방 감시 영역 | `own-ship.ts`, `autopilot.ts`, `track-pilot.ts`, `look-ahead.ts`(영역) | `Route/OwnShip.*`, `Autopilot.*`, `TrackPilot.*` | 시뮬레이션 초로 완주, XTE가 XTD 안 | M |

순서: RT-0 → 1 → 2는 고정. RT-3은 0 뒤 언제든(1·2와 병행 가능). RT-4는 2·3 뒤. RT-5는 2 뒤. RT-6은 1 뒤 독립. RT-7은 5 뒤. RT-8은 2 뒤, RT-9는 8 뒤.

첫 묶음 권장: **RT-0 → RT-1 → RT-2 → RT-5** (항로가 차트 위에 그려지는 데까지), 다음 RT-6(파일), RT-3·4(검사), RT-7(편집), RT-8·9.

## 5. 주의할 점

- **측지 수치.** 웹 `geodesy.ts`는 자체 WGS 84 구현, OpenS100은 GeographicLib이다. 항정선 결과는 mm 수준으로 같아야 하지만, 이식한 테스트의 기대값은 허용 오차를 두고 옮긴다. 어긋나면 그 차이를 먼저 밝힌다.
- **S-421 판.** 들어 있는 FC는 1.0.0(2019)이고 web-ecdis 변환기는 2.0(XSD 2.0.0-20260624)과 1.0을 둘 다 다룬다. PC가 2.0 데이터를 그리는지는 RT-6에서 확인한다. S-421 표본 데이터가 `SampleData`와 `D:\s100-reference`에 없다 — web-ecdis 테스트의 픽스처를 쓴다.
- **경보 탐지의 기하.** 웹은 디스플레이 리스트 항목이 지리 좌표 기하를 들고 있다. OpenS100의 SENC 인스트럭션이 탐지에 쓸 좌표를 어떤 형태로 들고 있는지(투영 전 좌표인지)를 RT-3 첫 작업으로 확인한다. 이 단계의 크기가 여기에 달렸다.
- **줄바꿈.** 기존 소스는 파일마다 줄바꿈이 다르다. 새 파일은 저장소 기본을 따르고 기존 파일은 `git diff --stat`으로 확인한다.
- 웹에서 일부러 바꾼 두 가지(구간은 대권이 아니라 항정선, 구간 시간은 분 반올림 없이 초)는 그대로 가져온다.

## 6. 정해야 할 것

1. **모듈 위치.** 새 폴더 `Route/`와 타깃 `opens100::route`(권장) / `GISLibrary` 안. 권장 이유: `latlon`에만 의존하므로 `view`·`portrayal` 없이도 쓸 수 있고, vcpkg 기능 `route`로 따로 낼 수 있다.
2. **편집 중인 항로의 그리기.** 직접 그리기(`RouteScene` → `IRenderer`, 권장, 웹과 같음) / S-421 PC로 그리기. PC로 그리면 "그림은 카탈로그가 정한다"는 원칙에는 맞지만 회랑·선택 표시·안내선·검사 하이라이트는 PC에 없어 어차피 직접 그려야 한다.
3. **뷰어 UI의 범위(RT-7).** 지침 3(UI보다 코어)에 따라 OpenS100 뷰어에는 최소(그리기 모드, 우클릭 메뉴, 열기·저장)만 두고 WP 표 패널은 KHOA 뷰어로 미룰지.
4. **시뮬레이션(RT-9)을 라이브러리에 둘지.** 자선 모델·오토파일럿은 S-100 표준 밖이다. `route`와 다른 타깃(`opens100::simulation`)으로 떼거나 예제에만 둘 수 있다.

## 7. 범위 밖

- AIS, CPA/TCPA, 센서(NMEA·STOMP) 공급원, 기록·재생.
- 대권 구간(모델에 `routeWaypointLegGeometryType`만 보존), UKC(S-129) 연계, 불확실성 회랑.
- KHOA 뷰어 `RoutePlanner`의 교체. 이 계획이 끝난 뒤 KHOA 저장소의 계획으로 쓴다.
- 웹 쪽 `route-edit-core-plan.md`의 실행. 단, RT-5·7에서 정한 scene·session의 모양은 그 계획과 맞춘다.

## 8. 진행 기록

| 날짜 | 한 일 | 검증 |
|---|---|---|
| 2026-11-27 | 조사와 계획 작성 | — |
