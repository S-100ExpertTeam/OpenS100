# TechDocs

OpenS100 개발자용 기술 문서. 새 세션은 **DIRECTIVES → 로드맵 → Phase 0 작업 로그** 순으로 읽는다.

| 문서 | 내용 |
|---|---|
| [DIRECTIVES.md](DIRECTIVES.md) | 소유자가 직접 밝힌 방향·작업 방식·참조 자원. 다른 문서보다 우선 |
| [2026-11-08-roadmap-library-first.md](2026-11-08-roadmap-library-first.md) | OpenS100을 S-100 핵심 C++ 라이브러리로 만들고 vcpkg에 등록하는 로드맵. 목표 아키텍처, 단계, KHOA 뷰어·opens100-web과의 관계 |
| [2026-11-08-backport-plan-from-opens100-web.md](2026-11-08-backport-plan-from-opens100-web.md) | opens100-web에서 앞선 기능·기술을 OpenS100으로 역이식하는 항목과 우선순위 |
| [2026-11-08-upstream-plan-from-khoa-viewer.md](2026-11-08-upstream-plan-from-khoa-viewer.md) | KHOA S-100 Viewer에만 있는 코어 기능을 OpenS100으로 올려보내는 계획 |
| [2026-11-08-code-assessment.md](2026-11-08-code-assessment.md) | OpenS100 코드베이스 평가 |
| [2026-11-08-phase0-worklog.md](2026-11-08-phase0-worklog.md) | 선행 정리 작업 목록. 체크박스와 커밋 해시 |
| [2026-11-08-phase1-core-decoupling.md](2026-11-08-phase1-core-decoupling.md) | Phase 1: 코어에서 MFC·Direct2D 분리. 결합 인벤토리와 5단계 계획, 진행 기록 |
| [2026-11-09-render-verification.md](2026-11-09-render-verification.md) | 렌더 검증 기록: 오프스크린 렌더 테스트, 결과 이미지, 성능 기준선 |
| [2026-11-09-cmake-vcpkg-skeleton.md](2026-11-09-cmake-vcpkg-skeleton.md) | CMake 빌드·설치·오버레이 포트·외부 소비자 예제. Phase 4·5의 선행 뼈대와 다음 할 일 |

기존 문서: 상위 `../Functions/`(기능 설명), `../../BUILD.md`, `../../CONVENTIONS.md`, `../../DOXYGEN.md`.
