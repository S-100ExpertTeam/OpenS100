#!/usr/bin/env python3
"""Progress gauge for Phase 1 (core decoupling).

Counts, per core project, the headers that still name Direct2D/DirectWrite/WIC types,
MFC types, or Win32 handles. The goal state is zero for every core project; render-side
files are listed separately and are allowed to use Direct2D.

Usage:
    python tools/check-core-includes.py            # report
    python tools/check-core-includes.py --fail     # exit 1 if any core header is tainted
                                                   # (switch on in CI once a project is clean)

See Documents/TechDocs/2026-11-08-phase1-core-decoupling.md, step 5-1.
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

CORE_PROJECTS = ["FeatureCatalog", "PortrayalCatalogue", "GeoMetryLibrary", "S100Engine", "GISLibrary", "LuaScriptingReference"]

# Files that are render-side by design and may use Direct2D (they must still not leak into model headers).
RENDER_SIDE = {
    "GISLibrary/D2D1Resources.h",
    "GISLibrary/D2DGeometryCache.h",
    "GISLibrary/S102Renderer.h",
    "GISLibrary/PortrayalRenderCache.h",
}

PATTERNS = {
    "d2d": re.compile(r"\b(ID2D1\w*|IDWrite\w*|IWIC\w*|D2D1_\w*|D2D1Resources)\b|d2d1(_1)?\.h"),
    "mfc": re.compile(r"\b(CString|CStringA|CStringW|CFile|CMap|CArray|CList|CPoint|CRect|CDC|CWnd|POSITION)\b|\bafx\w*\.h"),
    "win32": re.compile(r"\b(HDC|HWND|HBITMAP|HANDLE|DWORD|BYTE|POINT|RECT|COLORREF)\b|windows\.h"),
}


def scan(project: str):
    counts = {k: [] for k in PATTERNS}
    headers = sorted((ROOT / project).glob("*.h"))
    for h in headers:
        rel = f"{project}/{h.name}"
        try:
            text = h.read_text(encoding="utf-8", errors="replace")
        except OSError:
            continue
        for key, pat in PATTERNS.items():
            if pat.search(text):
                counts[key].append(rel)
    return len(headers), counts


def main(argv):
    fail = "--fail" in argv
    tainted_core = 0
    print(f"{'project':22s} {'headers':>7s} {'d2d':>5s} {'mfc':>5s} {'win32':>6s}")
    for project in CORE_PROJECTS:
        total, counts = scan(project)
        core_d2d = [f for f in counts["d2d"] if f not in RENDER_SIDE]
        tainted_core += len(core_d2d) + len(counts["mfc"]) + len(counts["win32"])
        print(f"{project:22s} {total:7d} {len(core_d2d):5d} {len(counts['mfc']):5d} {len(counts['win32']):6d}")
    print()
    if "--verbose" in argv:
        for project in CORE_PROJECTS:
            _, counts = scan(project)
            for key in PATTERNS:
                files = [f for f in counts[key] if not (key == "d2d" and f in RENDER_SIDE)]
                if files:
                    print(f"[{project}] {key}: " + ", ".join(Path(f).name for f in files))
    print(f"tainted core headers: {tainted_core} (render-side files allowed: {len(RENDER_SIDE)})")
    if fail and tainted_core:
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
