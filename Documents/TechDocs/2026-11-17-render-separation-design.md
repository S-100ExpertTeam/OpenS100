# 그리기를 인스트럭션 모델 밖으로 빼는 설계

- 작성: 2026-11-17. 상태: **진행 중** — R1 완료(9절). 8절의 결정 중 2번(색을 그릴 때 푼다)은 사용자가 "R1부터"로 승인. 나머지는 해당 단계 전에 정한다.
- 상위 문서: [로드맵](2026-11-08-roadmap-library-first.md)(Phase 3), [지침](DIRECTIVES.md) 18·22·24, [Phase 1 작업 로그](2026-11-08-phase1-core-decoupling.md) 5-3 (5), [역이식 계획](2026-11-08-backport-plan-from-opens100-web.md)("디스플레이 리스트 IR 공통화", "셀 간 통합 정렬").
- 요청: "Windows 종속성을 추가로 걷어내야 한다" → 남은 것은 걷어내기가 아니라 구조 변경이므로 설계부터 한다(사용자, 2026-11-17).

## 1. 왜 필요한가

5-3 (5)까지 하고 나니 GISLibrary 347개 소스 중 323개가 Linux에서 컴파일된다. 남은 24개와 S100Engine 36개는 Direct2D·GDI를 실제로 호출한다. 더 줄이지 못하는 이유는 하나다.

**포트레이얼 결과(드로잉 인스트럭션)가 스스로를 Direct2D로 그린다.** `SENC_Instruction::DrawInstruction(D2D1Resources*, …)`가 가상 함수라서, 인스트럭션 모델을 링크하려면 Direct2D 그리기 코드가 따라온다. 그래서

- 포트레이얼 실행(Lua 호스트, `ProcessS101`, `PCOutputSchemaManager`)을 vcpkg 패키지에 넣을 수 없다. 패키지는 지금 "데이터셋을 읽는다"까지이고 "무엇을 그려야 하는지 안다"가 없다.
- 지침 24의 Skia 백엔드를 나란히 붙일 자리가 없다.
- 지침 22(한 클래스가 파싱·모델·렌더를 겸하지 않는다)에 어긋난다.

## 2. 지금 구조 (2026-11-17 기준, 코드에서 확인)

```
Lua 규칙 / XSLT ──> S100_Instruction (PortrayalCatalogue 모듈, 순수 데이터)
                        │  SENC_Instruction::S1002SENC / FromS100Instruction
                        ▼
                SENC_Instruction 계층 + SENC_DisplayList   ← 모델이면서 렌더러
                        │
   LayerManager::DrawS100Layer ─ 셀마다, 우선순위마다, 타입마다 GetDrawingInstruction
                        │
        ┌───────────────┴────────────────────────────┐
        │ 점·텍스트: LayerManager::AddSymbolDrawing이 직접 그림   │
        │ 선·면·AugmentedRay/Path: instruction->DrawInstruction │
        │ 선 스타일: SENC_LineStyleBase::DrawInstruction (가상)   │
        └────────────────────────────────────────────┘
                        ▼
        D2D1Resources (렌더 타깃·브러시·텍스트 포맷) + S100Engine (SVG·선 스타일·면 패턴·색상 프로파일의 D2D 자원)
```

인스트럭션 모델에 섞여 있는 렌더 쪽 상태와 동작:

| 무엇 | 어디 | 성격 |
|---|---|---|
| `DrawInstruction` (가상) | `SENC_Instruction`, `SENC_LineStyleBase`와 파생 6개 | 그리기 동작 |
| `ChangePallete` (가상) | 인스트럭션·선 스타일·면 채움·`SENC_Color` | 팔레트가 바뀌면 모델 안의 RGB를 다시 채운다 |
| `geometryVector` (`ID2D1PathGeometry*`) | `SENC_LineStyleBase` | D2D 지오메트리 캐시 |
| `pGeometry` | `SENC_AugmentedRay` | D2D 지오메트리 캐시 |
| `textMatrix` (`DWRITE_TEXT_METRICS*`) | `SENC_TextInstruction` | 텍스트 크기 캐시 |
| `pColor`, `RGBColor`, `pen_color` | `SENC_Element`, `SENC_Color`, `SENC_LineStyle` | 토큰을 푼 색 |
| `pSvg` (`SVGReader*`) | `SENC_Symbol` | D2D 심볼 자원 포인터 |
| `s100Render` | `SENC_LineStyle`, `SENC_LineStyleReference` | D2D 도우미 |
| `GetDrawPoints` | 점·텍스트 인스트럭션 | 화면 좌표 계산(백엔드와 무관한 로직) |

그리기 코드의 크기: 인스트럭션 쪽 약 2,700줄(`SENC_AreaInstruction` 352, `AugmentedPath` 299, `AugmentedRay` 385, `LineInstruction` 220, `LineStyle` 292, `LineStyleReference` 102, `CompositeLineStyle` 85, `TextInstruction` 330, `PointInstruction` 671 — 절반가량은 주석 처리된 옛 코드), `LayerManager.cpp` 2,130줄 중 그리기 800줄가량, S100Engine 3,548줄.

참조 구현: opens100-web은 이미 이 구조다. `core/src/display-list.ts`(446줄, 순수 데이터 IR), `render/src/canvas-renderer.ts`(1,449줄, IR을 Canvas 2D로 그린다), `comparePainting`(표시면 → 우선순위 → 뷰잉 그룹 → 셀 우선순위).

## 3. 목표 구조

```
opens100::portrayal  (새 타깃, 전 플랫폼)
   Lua 호스트 · ProcessS101 · PCOutputSchemaManager
   드로잉 인스트럭션 모델 (지금의 SENC_*, 순수 데이터)
   DisplayList  ── 셀 간 병합·정렬, JSON 직렬화(웹 IR과 같은 필드)
          │  읽기만
          ▼
s100::render::DisplayListPainter   (백엔드와 무관, 전 플랫폼)
   인스트럭션을 순회하며 "무엇을 어디에"를 계산:
   투영, 선 스타일의 대시·심볼 배치, 면 패턴 배치, 텍스트 위치, 색 토큰 → 색
          │  그리기 원시 연산만 호출
          ▼
s100::render::IRenderer   (지침 24의 추상 인터페이스)
   ┌──────────────┬──────────────┐
   D2DRenderer     SkiaRenderer    (그 밖: 테스트용 기록 렌더러)
   (전환기 기준)     (최종, 전 플랫폼)
```

### 3.1 인스트럭션 모델은 데이터만 갖는다

- `DrawInstruction`, `ChangePallete`를 모델에서 뺀다. 모델에는 Part 9가 정의한 것만 남는다: 타입, 뷰잉 그룹, 표시면, 우선순위, 축척 범위, 피처·공간 참조, 심볼·선 스타일·면 채움·텍스트의 **기술**(참조 이름, 토큰, 수치).
- 색은 **토큰과 투명도만** 저장한다. RGB는 그릴 때 현재 팔레트로 푼다. 그러면 `ChangePallete` 사슬 전체와 `RGBColor`·`pen_color`·`pColor`가 없어지고, 팔레트 전환은 "다시 그리기"만으로 끝난다(지금은 모든 인스트럭션을 돌며 색을 다시 채운다).
- 렌더 캐시(`geometryVector`, `textMatrix`)는 렌더러가 만들고 읽는다. 모델 객체는 내용을 모르는 **빈 칸 하나**(`GISLibrary::RenderCacheSlot`, `RenderCacheSlot.h`)만 갖고, 객체가 없어질 때 칸도 같이 없앤다. Direct2D 쪽 내용물은 `D2DInstructionCache`가 정의한다.
  - 초안은 "렌더러가 인스트럭션 포인터를 키로 갖는 표 + 모델 수명이 끝날 때 비우는 알림"이었다. R2에서 칸으로 바꿨다. 이유: (1) 표는 알림을 빠뜨리면 해제된 주소가 재사용될 때 남의 캐시를 읽는다 — 칸은 수명이 구조로 맞는다. (2) 알림은 모델이 렌더러를 부르는 방향의 의존을 만든다. (3) `DrawInstruction` 시그니처에 캐시를 넘기는 인자를 더하지 않아도 된다(R3에서 어차피 없어질 시그니처).
  - 칸의 한계: 한 인스트럭션을 두 렌더러가 동시에 그리면 칸을 서로 덮어쓴다(틀린 그림은 아니고 캐시가 매번 다시 만들어진다). R5에서 D2D와 Skia를 나란히 돌릴 때 문제가 되면 칸을 렌더러별로 나눈다.
  - `pGeometry`(광선)는 그리는 동안만 쓰던 것이라 지역 변수가 됐고, `pSvg`는 쓰기만 하고 읽는 곳이 없어 지웠고, `s100Render`는 상태를 쓰지 않는 함수 둘을 `static`으로 바꿔 객체를 없앴다.
- 타입 분기는 방문자(visitor)로 한다. 인스트럭션 종류는 표준이 정한 닫힌 집합이므로 방문자가 맞다(새 백엔드는 추가, 새 인스트럭션 종류는 표준 개정 때만).

```cpp
namespace s100::portrayal {
    class InstructionVisitor {            // 모델 쪽 헤더. 백엔드를 모른다.
    public:
        virtual ~InstructionVisitor() = default;
        virtual void Visit(const PointInstruction&) = 0;
        virtual void Visit(const LineInstruction&) = 0;
        virtual void Visit(const AreaInstruction&) = 0;
        virtual void Visit(const TextInstruction&) = 0;
        virtual void Visit(const AugmentedRay&) = 0;
        virtual void Visit(const AugmentedPath&) = 0;
        virtual void Visit(const NullInstruction&) {}
    };
    // 각 인스트럭션: void Accept(InstructionVisitor& v) const override { v.Visit(*this); }
    // 선 스타일(LineStyle/Reference/Composite)과 면 채움(Color/Symbol/Hatch/Pixmap/Reference)도 같은 방식.
}
```

### 3.2 `IRenderer`는 캔버스 수준으로 둔다 (권고)

두 가지 선택지가 있다.

| | (A) 캔버스 수준 `IRenderer` + 공용 페인터 | (B) 백엔드마다 인스트럭션 방문자를 구현 |
|---|---|---|
| 백엔드가 구현하는 것 | 경로 채우기·긋기, 비트맵/심볼, 텍스트, 클립, 변환 등 원시 연산 20개 안팎 | 인스트럭션별 그리기 전부 |
| S-100 로직(대시·심볼 배치, 패턴, 텍스트 위치) | 한 번만 쓴다(공용 페인터) | 백엔드마다 다시 쓴다 |
| D2D와 Skia가 같은 그림을 낼 가능성 | 높다(로직이 같고 원시 연산만 다르다) | 낮다(로직이 둘) |
| 당장의 작업량 | 많다(지금 D2D 코드에서 로직을 분리해야 한다) | 적다(지금 코드를 그대로 방문자로 옮긴다) |
| 웹 구현과의 대응 | `canvas-renderer.ts`가 Canvas 2D 위에서 같은 구조 | 대응 없음 |

**권고: 최종 목표는 (A), 가는 길에 (B)를 한 번 거친다.** 먼저 지금 D2D 코드를 고치지 않고 `D2DInstructionPainter`(방문자)로 옮겨 모델을 깨끗하게 만든다(4절 R3). 그 뒤 인스트럭션 종류 하나씩 공용 페인터 + `IRenderer`로 바꾼다(R5). 지침 24의 "D2D 코드는 격리만 하고 최소한으로 손댄다"와도 맞는다.

`IRenderer` 초안(이름·시그니처는 R5에서 확정):

```cpp
namespace s100::render {
    struct Paint { ColorF color; float width; std::span<const float> dashes; LineCap cap; LineJoin join; };
    class IRenderer {
    public:
        virtual ~IRenderer() = default;
        virtual void Save() = 0;  virtual void Restore() = 0;
        virtual void SetTransform(const Matrix3x2&) = 0;
        virtual void ClipPath(const Path&) = 0;
        virtual void FillPath(const Path&, ColorF) = 0;
        virtual void FillPathWithPattern(const Path&, const Pattern&) = 0;
        virtual void StrokePath(const Path&, const Paint&) = 0;
        virtual void DrawSymbol(SymbolHandle, PointF at, float rotationDeg, float scale, const ColorOverride*) = 0;
        virtual TextMetrics MeasureText(std::u8string_view, const Font&) = 0;
        virtual void DrawText(std::u8string_view, const Font&, PointF at, ColorF) = 0;
        virtual void DrawImage(ImageHandle, const RectF& dest, float opacity) = 0;   // 커버리지 래스터
    };
}
```

`Path`·`Matrix3x2`·`PointF`·`ColorF`는 자체 값 타입이다(`DrawTypes.h`가 그 시작). 심볼(SVG)·패턴·이미지의 실체는 백엔드가 만들고 핸들로만 오간다. 카탈로그 쪽은 이미 `Portrayal::ResourceStore` 뒤에 있다.

### 3.3 그리기 순서는 한곳에서 정한다

지금은 `DrawS100Layer`가 셀마다 따로, 우선순위 루프 안에서 타입별로 꺼내 그린다. 목표는 역이식 계획대로 "보이는 셀의 인스트럭션을 모아 `comparePainting`으로 한 번 정렬 → 한 번 순회"다. 순서 규칙이 모델 쪽 함수 하나가 되므로 백엔드와 무관하게 테스트할 수 있다. 이 변경은 셀 경계의 겹침 순서를 바꾸므로 **그림이 달라진다**. 별도 단계(R4b)로 떼어 결과를 따로 본다.

## 4. 단계

각 단계는 그 자체로 빌드·테스트가 통과하고 커밋 가능한 단위다. "그림 동일"은 KOR TDS 808개 셀을 `RenderSmoke.DrawsCellsFromEnvironment`로 그려 PNG를 바이트 단위로 비교하는 것을 뜻한다(오늘 LuaJIT 비교에 쓴 방법, 1회 약 3분).

| 단계 | 내용 | 그림 | 끝나면 |
|---|---|---|---|
| **R1** ✔ | 색을 토큰으로만 저장. 그릴 때 팔레트로 푼다. `ChangePallete` 사슬과 모델 안의 RGB 삭제 | 동일해야 함 | 모델에서 색상 프로파일(S100Engine) 의존이 사라진다 |
| **R2** ✔ | 렌더 캐시를 모델 밖으로(`geometryVector`, `pGeometry`, `textMatrix`, `pSvg`, `s100Render`). `D2DInstructionCache` 신설 | 동일해야 함 | 모델에 D2D 포인터가 없다 |
| **R3** ✔ | 방문자 도입. `DrawInstruction` 본문과 `LayerManager::AddSymbolDrawing`의 점·텍스트 그리기를 `D2DInstructionPainter`로 **그대로 옮긴다** | 동일해야 함 | `SENC_*` 전체가 Direct2D 없이 컴파일·링크된다 |
| **R3p** | 새 타깃 `opens100::portrayal`: 인스트럭션 모델 + `PCOutputSchemaManager` + Lua 호스트 + `ProcessS101`. 포트 기능 `portrayal`. 소비자 예제가 셀 하나를 포트레이얼해 인스트럭션 수를 낸다 | 해당 없음 | **패키지가 "무엇을 그릴지"를 낸다.** Windows·Linux에서 같은 수 |
| **R4a** | 디스플레이 리스트 JSON 직렬화(웹 IR과 같은 필드). 골든 해시 테스트 | 해당 없음 | 플랫폼 간·웹과의 결과 비교가 그림 없이 된다 |
| **R4b** | 셀 간 병합 + `comparePainting` 정렬 후 한 번에 그리기 | **달라짐**(의도) | 셀 경계 겹침 순서가 표준대로 |
| **R5** | `IRenderer` + `DisplayListPainter`. D2D로 `IRenderer` 구현. 인스트럭션 종류 하나씩 공용 페인터로 전환 | 종류별로 동일 또는 허용 오차 | S-100 그리기 로직이 백엔드와 분리된다 |
| **R6** | Skia로 `IRenderer` 구현(포트 기능 `render-skia`). D2D 결과와 허용 오차로 비교. 통과하면 D2D 제거(지침 24) | 허용 오차 | 전 플랫폼 렌더링 |

R1~R3은 "옮기기"라서 그림이 바뀌면 안 되고, 바뀌면 버그다. R3p가 vcpkg 관점의 첫 성과다. R4b·R5·R6은 그림이 바뀔 수 있어 사람이 봐야 한다(지침 23).

작업량 감각(추정, 측정 아님): R1 작음, R2 작음~중간, R3 중간(약 2,700줄 이동), R3p 중간(링크 닫힘 확인과 포트), R4a 중간, R4b 중간, R5 큼, R6 큼.

## 5. 검증

- **그림**: 808개 셀 PNG 바이트 비교(R1~R3은 전부 동일해야 한다). 기준 그림은 단계 시작 직전 커밋으로 다시 뽑는다.
- **인스트럭션**: R3p부터 셀별 인스트럭션 수(타입별)를 Windows와 Linux에서 대조한다. R4a부터는 JSON 해시로.
- **단위 테스트**: `comparePainting`, 색 토큰 해석, 대시 계산처럼 백엔드와 무관해진 로직은 `OpenS100Tests`에 넣는다. `IRenderer`에는 호출을 기록만 하는 테스트용 구현을 둔다.
- **성능**: `RenderSmoke`가 이미 내는 시간(포트레이얼, 첫 그리기, 다음 프레임)을 단계 전후로 적는다. R1은 그릴 때 색을 풀므로 프레임 시간이 늘 수 있다 — 늘면 팔레트별 해석 결과를 렌더러 캐시에 둔다.

## 6. 위험과 모르는 것

- **XSLT 경로**: S-101 외 제품은 `ProcessS101`이 libxslt로 포트레이얼한다. 결과가 같은 `SENC_*`로 들어오므로 설계는 같지만, `opens100::portrayal`이 libxml2·libxslt 의존을 갖게 된다(포트 기능을 `portrayal`과 `portrayal-xslt`로 나눌지 R3p에서 정한다).
- **`pTheFC` 등 전역 상태**: Lua 호스트가 전역으로 피처 카탈로그·데이터셋을 본다(닫힘 검사에서 `pTheFC` 미해결 심벌 12건). R3p에서 세션 객체로 묶어야 할 수 있다. 범위를 아직 읽지 않았다.
- **텍스트**: 지금 텍스트 배치는 DirectWrite의 측정값에 의존한다. Skia로 가면 글꼴·측정이 달라 "같은 그림"이 가장 어려운 부분이다. R6의 허용 오차를 텍스트에 대해 따로 정해야 한다.
- **S100Engine**: SVG·선 스타일·면 패턴의 D2D 자원이다. R5에서 `IRenderer`의 심볼·패턴 핸들 뒤로 들어가고, R6에서 Skia 구현이 대체한다. 그 전에는 손대지 않는다.
- **커버리지(S-102 등)**: `CoverageDataset`은 인스트럭션 경로가 아니라 자체 D2D 그리기다. 래스터 생성은 이미 백엔드와 무관하므로(`Coverage` 모듈) R5에서 `DrawImage`로 붙인다. 이 문서의 R1~R4에는 포함하지 않는다.
- **교환 카탈로그 뷰·SHP 배경·편집 렌더**(`ExchangeCatalogue`, `SHPFile`, `S100EditRender`): 인스트럭션이 아닌 직접 그리기다. R5 이후에 `IRenderer`로 옮기거나 앱 계층으로 내린다.
- **Skia의 무게**: vcpkg `skia` 포트는 빌드가 길고 크다. 선택 기능으로만 두고 기본 기능에는 넣지 않는다.

## 7. 이 설계가 하지 않는 것

- 인스트럭션 클래스의 이름 바꾸기(`SENC_*` → `s100::portrayal::*`), raw 포인터를 스마트 포인터로 바꾸기. 필요하지만 이동과 섞으면 그림 동일 검증의 의미가 흐려지므로 R3 뒤에 따로 한다.
- `LayerManager`의 레이어 관리·피킹·경보 분리(로드맵의 `s100-sdk`).
- 타일·점진 렌더링, 공간 색인.

## 8. 결정이 필요한 것 (사용자)

1. **`IRenderer`의 수준**: (A) 캔버스 수준 + 공용 페인터를 최종 목표로 하고 (B)를 거쳐 간다는 3.2의 권고대로 갈지.
2. **색을 그릴 때 푼다(R1)**: 모델에서 RGB를 없애는 데 동의하는지. 대안은 RGB를 모델에 두고 "팔레트 적용"만 모델 밖 함수로 빼는 것(변경이 작지만 모델이 팔레트 상태를 계속 갖는다).
3. **R3p를 vcpkg 등록보다 먼저 할지**: 지금 등록하면 패키지는 "읽기"까지다. R3p까지 하면 "포트레이얼 결과"까지 낸다.
4. **셀 간 통합 정렬(R4b)**: 그림이 달라지는 변경이다. 이번 흐름에 넣을지, 따로 뗄지.
5. **Skia(R6)의 시점**: R5 직후에 갈지, vcpkg 등록과 KHOA 뷰어 전환 뒤로 미룰지.

## 9. 진행 기록

| 날짜 | 내용 | 커밋 | 검증 |
|---|---|---|---|
| 2026-11-17 | 설계 초안 작성 | (문서만) | 코드 변경 없음 |
| 2026-11-17 | **R1**: 인스트럭션은 색 토큰만 갖고 렌더러가 그릴 때 푼다(`GISLibrary::PortrayalRender::Color`). `SENC_Color::RGBColor`, `SENC_LineStyle::pen_color`, `SENC_Element::pColor`와 `ChangePallete` 사슬(모델 클래스 14개 + `PCOutputSchemaManager`) 삭제. 텍스트는 "전경색이 지정됐는가"만 `hasForeground`로 남긴다 | fdaa6dee | KOR TDS 808개 셀 그림이 변경 전과 바이트 단위로 동일. 새 테스트 `RenderSmoke.PaletteSwitchRedrawsAnOpenChartInTheNewColours`(열린 해도의 팔레트를 Night로 바꾼 그림 = Night로 연 그림)가 변경 전·후 코드 모두 통과하고 두 코드의 Night 그림이 같다. 테스트 207/210(3개는 외부 데이터 필요로 건너뜀). 시간(150개 셀, 조용한 상태): 첫 그리기 7.7 s → 7.3~7.4 s, 다음 4프레임 17.2 s → 16.3~16.6 s — 늘지 않았다(오차 범위). CMake `dataset` 빌드 통과. 뷰어 화면·Linux 포트 빌드·CI 미확인. 팔레트 테스트의 기준 셀(`101KR003G2M00`)은 색이 적은 심해 셀이라 검증력이 약하다 |
| 2026-11-17 | **R2**: 모델에서 Direct2D 캐시를 뺐다. 텍스트 크기와 호 지오메트리는 모델 객체의 빈 칸(`RenderCacheSlot`)에 `D2DInstructionCache`가 넣는다(3.1의 결정 참고). `SENC_AugmentedRay::pGeometry`는 지역 변수로, `SENC_Symbol::pSvg`(읽는 곳 없음)는 삭제, `S100Render`의 비트맵 선 그리기 함수 둘은 `static`으로 바꿔 선 스타일마다 만들던 `S100Render` 객체를 없앴다. 모델 헤더에 남은 Direct2D 이름은 `DrawInstruction` 시그니처의 전방 선언뿐이다(R3에서 없어진다) | 2c6b6386 | KOR TDS 808개 셀 그림이 기준과 바이트 단위로 동일. 테스트 207/210(3개는 외부 데이터 필요로 건너뜀). CMake `dataset` 빌드 통과(`D2DInstructionCache`는 표시 쪽 제외 목록에 추가 — 제외 49개). Linux GCC 구문 검사: Direct2D 없이 컴파일되는 GISLibrary 소스 323 → 325개(`SENC_LineStyleBase`, `SENC_TextInstruction` 추가; 남은 22개는 실제로 Direct2D·GDI를 부른다). 시간(150개 셀, R1·R2 번갈아 2회): 포트레이얼 48.4/30.9 s → 43.7/26.4 s, 첫 그리기 12.5/8.2 s → 11.2/7.3 s, 다음 4프레임 29.4/19.6 s → 26.9/17.2 s — 회차 간 편차가 커서 "느려지지 않았다"까지만 말할 수 있다. 뷰어 화면·Linux 포트 빌드·CI 미확인 |
| 2026-11-17 | **R3**: 그리기를 모델에서 뺐다. 모델에는 방문자 인터페이스(`SENC_InstructionVisitor`, `SENC_LineStyleVisitor`)와 `Accept`만 남고, `DrawInstruction` 본문 6개와 `LayerManager::AddSymbolDrawing`의 점·텍스트 루프 본문 2개는 `D2DInstructionPainter`(1,179줄)로 **그대로** 옮겼다 — 각 함수는 옛 본문이 쓰던 멤버를 먼저 별칭으로 잡고 옛 본문이 이어진다. `AddSymbolDrawing`에는 인스트럭션 묶음 사이의 일(변환, 브러시 초기화, 수심 억제 구간)만 남았다. 선 스타일은 그리는 곡선이 인자로 필요해, 페인터가 곡선을 들고 방문한다(`DrawLineStyle`) | f62bea55 | KOR TDS 808개 셀 그림이 기준과 바이트 단위로 동일(첫 실행에서 808개). 테스트 207/210(3개는 외부 데이터 필요로 건너뜀). CMake `dataset` 빌드 통과(제외 50개). Linux GCC 구문 검사: 325 → 332개, **`SENC_*` 소스 전부가 Direct2D 없이 컴파일된다**(남은 16개는 렌더·표시 코드). 시간(150개 셀, 변경 전·후 번갈아 2회): 첫 그리기 8.1/7.1 s → 7.1/7.0 s, 다음 4프레임 18.0/16.7 s → 16.8/16.8 s — 차이 없음. **링크는 확인하지 않았다**: `SENC_*`는 아직 `opens100::dataset`에 넣지 않았고(표시 쪽 제외 목록에 그대로), 닫힌 집합 확인은 R3p의 일이다. 뷰어 화면·Linux 포트 빌드·CI 미확인 |

### 9.1 검증 환경에서 본 것: 그림이 실행마다 ±1 흔들린 구간

R2 검증 중 808개 셀 첫 실행에서 3개가 기준과 달랐다. 원인을 가리려고 같은 120개 묶음을 반복해 그렸더니 **R1 빌드(변경 전 코드)도** 실행마다 0~42개가 달랐고, 다른 셀은 늘 연속한 구간이었다. 차이는 대부분 안티에일리어싱 픽셀의 채널값 ±1이다. 그 뒤의 808개 전체 실행은 첫 실행에서 808개 모두 기준과 같았다.

- 코드 변경과 무관한, 이 PC의 래스터라이징이 일정 시간 달라지는 현상이다. 원인은 확인하지 못했다. 추정: `D2D1Resources`가 렌더 타깃을 `D2D1_RENDER_TARGET_TYPE_DEFAULT`로 만들어 하드웨어/소프트웨어 선택을 시스템에 맡기는데, 이 PC에는 원격 디스플레이 어댑터가 있어 세션 상태에 따라 선택이 바뀔 수 있다.
- 그날 앞선 비교(LuaJIT, R1)에서는 반복 실행이 모두 재현됐으므로 그 결과는 유효하다.
- 이후 비교는 "다르게 나온 셀을 다시 그려 기준 바이트가 재현되는지"까지 본다(재현되지 않는 차이만 실제 차이). 테스트에서 렌더 타깃을 소프트웨어로 고정하면 근본적으로 없어질 수 있다 — 하지 않았다(결정 필요: D2D 코드를 건드린다).
