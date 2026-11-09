# 렌더 검증 기록 (2026-11-09)

Phase 1에서 그리기 코드를 여러 번 옮겼다(GDI 그리기 → `GdiGeometryRenderer`, D2D 뷰 변환 → `D2DView.h`,
`Scaler` API 변경, `/W4` 정리 등). 지침 23번대로 실제 화면으로 확인해야 하는 변경이다.

## 화면 캡처는 이번에 불가능했다

`agent-desktop`으로 뷰어를 띄우고 접근성 트리까지는 읽었지만 스크린숏은 실패했다
(`BitBlt failed for display capture`, PowerShell `CopyFromScreen`도 `The handle is invalid`).
원인은 사용자 세션이 원격 연결 해제 상태(`qwinsta` 기준 `Disc`)였기 때문이다. 연결이 끊긴 RDP 세션은
데스크톱을 그리지 않으므로 어떤 캡처 API도 동작하지 않는다. 세션이 연결되어 있을 때 다시 시도한다.

## 대신 만든 것: 오프스크린 렌더 테스트

`Tests/OpenS100Tests/RenderSmokeTest.cpp`

- 뷰어와 같은 경로(`CGISLibraryApp::Draw(HDC)`)로 그리되 대상이 32비트 메모리 비트맵이다.
  창도, 대화형 데스크톱도 필요 없다(연결이 끊긴 세션에서도 동작함을 확인).
- 결과를 PNG로 저장한다. 출력 위치는 환경 변수 `S100_RENDER_OUT`(없으면 테스트 임시 폴더).
- `RenderSmoke.DrawsKoreanS101CellOffscreen`: KOR_TDS의 가장 작은 셀을 그리고 "빈 화면이 아님"만 단언한다(약 1.3초).
- `RenderSmoke.DrawsCellsFromEnvironment`: `S100_RENDER_CELLS`(`;` 구분 경로)에 적은 셀을 그린다. 기본은 건너뜀.

```
cd Debug_x64
set S100_RENDER_OUT=C:\temp\render
set S100_RENDER_CELLS=D:\s100-reference\tds\KOR_TDS\S100_ROOT\S-101\DATASET_FILES\101KR005G3E12\101KR005G3E12.000
OpenS100Tests.exe --gtest_filter=RenderSmoke.*
```

픽셀 단위 골든 이미지 비교는 디스플레이 리스트 IR(Phase 3)과 함께 넣는다. 지금은 사람이나 에이전트가
PNG를 직접 보는 용도다.

## 이번 결과 (커밋 5466bc38 기준, Debug x64)

| 셀 | 이미지 | 확인한 것 |
|---|---|---|
| 101KR003G2M00 (먼바다) | `assets/2026-11-09-render-check/101KR003G2M00.png` | 수심 구역 채움, 영역 패턴, 수심 숫자, 지명 텍스트, 정보 심볼 |
| 101KR005G3E12 (통영 부근) | `assets/2026-11-09-render-check/101KR005G3E12.png` | 육지·수심 구역 색, 등심선, 등화 문자열(`Fl R 4s11m8M`), 부표·교량·항만 이름, 등광 부채꼴 |
| 101KR004F4H20 (광양·여수) | `assets/2026-11-09-render-check/101KR004F4H20.png` | 위와 같음 + 항로 표지 밀집 구역, 교통 분리·주의 구역 경계 |

이상 징후는 보이지 않았다. 셀 전체를 한 화면에 맞춰 그려서 텍스트와 심볼이 겹쳐 보이는 것은
축척 필터를 적용하지 않은 전체 보기라서 그렇다.

## 알게 된 것: 그리기가 느리다

상세 셀 2개(4급·5급)를 여는 데서 그리는 데까지 Debug 빌드로 336초가 걸렸다. Release에서 다시 재고,
열기(8211 파싱) / 포트레이얼(Lua) / 그리기(D2D) 중 어디가 병목인지 나눠 측정해야 한다. 성능은 이 라이브러리의
핵심 목표이므로 Phase 2에 들어가기 전에 기준선을 만든다.
