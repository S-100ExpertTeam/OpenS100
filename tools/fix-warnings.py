"""Mechanical fixes for MSVC /W4 warnings listed in a warnings JSON (see collect_warnings.py).

C4458/C4456/C4457  shadowing declaration: rename `x` -> `_x` in its scope
C4100              unreferenced parameter: comment the name out
C4101              unreferenced local: drop the catch variable name / delete the declaration line
C4189              initialised but unused local: keep a call for its side effects, else delete the line

Anything that does not fit a safe single-line form is listed under "manual" and left untouched.
usage: fix_warnings_v2.py <warnings.json> [file:line to skip ...]
"""
import collections, json, pathlib, re, sys

rows = json.load(open(sys.argv[1], encoding="utf-8"))
skip = set(a.lower() for a in sys.argv[2:])
HANDLED = {"C4458", "C4456", "C4457", "C4100", "C4101", "C4189"}
SIDE_EFFECT = re.compile(r"(?<![\w])(_mkdir|mkdir|Open\w*|Read\w*|Write\w*|Add\w*|Insert\w*|Create\w*|Copy\w*|Set\w*|Load\w*|Save\w*|Delete\w*|Remove\w*|Init\w*|Release|push_back|emplace\w*|erase|insert|fopen\w*|fclose|H5\w+)\s*\(")

def mask_code(s):
    out = list(s)
    i, n = 0, len(s)
    while i < n:
        c = s[i]
        if s.startswith("//", i):
            j = s.find("\n", i)
            j = n if j < 0 else j
            for k in range(i, j): out[k] = " "
            i = j
        elif s.startswith("/*", i):
            j = s.find("*/", i + 2)
            j = n if j < 0 else j + 2
            for k in range(i, j):
                if s[k] != "\n": out[k] = " "
            i = j
        elif c == '"' or c == "'":
            j = i + 1
            while j < n and s[j] != c and s[j] != "\n":
                j += 2 if s[j] == "\\" else 1
            for k in range(i + 1, min(j, n)):
                if s[k] != "\n": out[k] = " "
            i = j + 1
        else:
            i += 1
    return "".join(out)

def prev_nonspace(m, i):
    i -= 1
    while i >= 0 and m[i] in " \t\r\n":
        i -= 1
    return i

def next_nonspace(m, i):
    while i < len(m) and m[i] in " \t\r\n":
        i += 1
    return i

def match_forward(m, i, open_ch, close_ch):
    depth = 0
    while i < len(m):
        if m[i] == open_ch: depth += 1
        elif m[i] == close_ch:
            depth -= 1
            if depth == 0:
                return i
        i += 1
    return -1

def occurrences(m, name, a, b):
    for mt in re.finditer(r"(?<![\w])" + re.escape(name) + r"(?![\w])", m[a:b]):
        yield a + mt.start()

def qualified(m, pos):
    p = prev_nonspace(m, pos)
    if p < 0:
        return False
    return m[p] == "." or (p >= 1 and m[p - 1:p + 1] in ("->", "::"))

report = collections.Counter()
manual = []
by_file = collections.defaultdict(list)
for r in rows:
    if r["code"] in HANDLED:
        by_file[r["file"]].append(r)

for file, items in by_file.items():
    p = pathlib.Path(file)
    b = p.read_bytes()
    bom = b.startswith(b"\xef\xbb\xbf")
    s = b.decode("utf-8-sig")
    m = mask_code(s)
    line_start = [0]
    for mt in re.finditer("\n", s):
        line_start.append(mt.end())
    edits = {}          # offset -> (old_len, new_text)

    def line_span(line):
        ls = line_start[line - 1]
        le = line_start[line] if line < len(line_start) else len(s)
        return ls, le

    def locate(line, col, name):
        ls, le = line_span(line)
        cands = list(occurrences(m, name, ls, le))
        if not cands:
            return -1
        want = ls + col - 1
        return min(cands, key=lambda o: abs(o - want))

    def add(off, old_len, text, r):
        for o, (l, _) in edits.items():
            if off < o + l and o < off + old_len and not (o == off and l == old_len):
                manual.append((r, "overlapping edit"))
                return False
        edits[off] = (old_len, text)
        return True

    # shadowing first (their ranges are what line deletions must not overlap)
    order = {"C4458": 0, "C4456": 0, "C4457": 0, "C4100": 1, "C4101": 2, "C4189": 2}
    for r in sorted(items, key=lambda r: order[r["code"]]):
        if f"{p.name}:{r['line']}".lower() in skip:
            report["skipped"] += 1
            continue
        name = re.search(r"'([^']+)'", r["msg"]).group(1)
        off = locate(r["line"], r["col"], name)
        if off < 0:
            manual.append((r, "identifier not found on line"))
            continue
        ls, le = line_span(r["line"])
        mline = m[ls:le]

        if r["code"] == "C4100":
            add(off, len(name), f"/*{name}*/", r)
            report["C4100"] += 1
            continue

        if r["code"] == "C4101":
            if re.search(r"\bcatch\s*\(", mline):
                # `catch (const T& e)` -> `catch (const T&)`
                start = off
                while start > ls and m[start - 1] in " \t":
                    start -= 1
                add(start, off + len(name) - start, "", r)
                report["C4101 catch"] += 1
            elif re.fullmatch(r"\s*[\w:<>\*&,\s]+?[\s\*&]" + re.escape(name) + r"(\s*\[[^\]]*\])?\s*;\s*", mline) and mline.count(",") == 0:
                add(ls, le - ls, "", r)
                report["C4101 line"] += 1
            else:
                manual.append((r, "not a simple declaration"))
            continue

        if r["code"] == "C4189":
            stripped = mline.strip()
            eq = mline.find("=", off - ls + len(name))
            balanced = mline.count("(") == mline.count(")") and mline.count("{") == mline.count("}")
            if eq < 0 or not stripped.endswith(";") or not balanced or mline.count(";") != 1 or mline[eq:eq + 2] == "==":
                manual.append((r, "not a single-line `T x = expr;`"))
                continue
            first = ls + len(mline) - len(mline.lstrip())
            init = mline[eq + 1:].strip().rstrip(";").strip()
            # declaration must start the statement: nothing but the type before the name
            if not re.fullmatch(r"[\w:<>\*&,\s]+", m[first:off]):
                manual.append((r, "declaration does not start the line"))
                continue
            depth0_comma, dd = False, 0
            for ch in init:
                if ch in "(<[{": dd += 1
                elif ch in ")>]}": dd -= 1
                elif ch == "," and dd == 0: depth0_comma = True
            if depth0_comma:
                manual.append((r, "several declarators on one line"))
                continue
            # keep the initialiser as a statement only when the call plausibly has side effects
            has_call = SIDE_EFFECT.search(init) is not None and not init.startswith("new")
            if has_call:
                expr_start = next_nonspace(m, ls + eq + 1)
                add(first, expr_start - first, "", r)          # keep the call, drop `T x = `
                report["C4189 keep call"] += 1
            else:
                add(ls, le - ls, "", r)
                report["C4189 line"] += 1
            continue

        # ---- shadowing: C4458 / C4456 / C4457
        if r["code"] == "C4456" and any(
                x["code"] in ("C4101", "C4189") and f"'{name}'" in x["msg"] and 0 < r["line"] - x["line"] < 400
                for x in items):
            # the outer declaration is unused and is being removed; no shadowing remains
            report["C4456 outer removed"] += 1
            continue
        new = "_" + name
        i, depth, open_paren = off - 1, 0, -1
        while i >= 0:
            ch = m[i]
            if ch == ")": depth += 1
            elif ch == "(":
                if depth == 0:
                    open_paren = i
                    break
                depth -= 1
            elif ch in ";{}" and depth == 0:
                break
            i -= 1

        targets = [off]
        if open_paren >= 0:
            close_paren = match_forward(m, open_paren, "(", ")")
            # rest of the parenthesised header (for-loop condition/increment)
            for o in occurrences(m, name, off + len(name), close_paren):
                if not qualified(m, o):
                    targets.append(o)
            j = close_paren + 1
            k, d = j, 0
            while k < len(m):
                if m[k] == "(": d += 1
                elif m[k] == ")": d -= 1
                elif d == 0 and m[k] in "{;":
                    break
                k += 1
            if k < len(m) and m[k] == "{":
                for o in occurrences(m, name, j, k):
                    pr = prev_nonspace(m, o)
                    nx = next_nonspace(m, o + len(name))
                    is_member_init = m[pr] in ":," and nx < len(m) and m[nx] in "({"
                    if not is_member_init and not qualified(m, o):
                        targets.append(o)
                body_close = match_forward(m, k, "{", "}")
                for o in occurrences(m, name, k, body_close):
                    if not qualified(m, o):
                        targets.append(o)
            elif k < len(m):
                for o in occurrences(m, name, j, k):
                    if not qualified(m, o):
                        targets.append(o)
            report[r["code"] + " header"] += 1
        else:
            k, d = off, 0
            while k < len(m):
                if m[k] == "{": d += 1
                elif m[k] == "}":
                    d -= 1
                    if d < 0:
                        break
                k += 1
            for o in occurrences(m, name, off + len(name), k):
                if not qualified(m, o):
                    targets.append(o)
            report[r["code"] + " local"] += 1

        lo, hi = min(targets), max(targets) + len(name)
        if re.search(r"(?<![\w])" + re.escape(new) + r"(?![\w])", m[lo:hi]):
            manual.append((r, f"{new} already used in scope"))
            continue
        for o in targets:
            edits[o] = (len(name), new)

    for o in sorted(edits, reverse=True):
        old_len, text = edits[o]
        s = s[:o] + text + s[o + old_len:]
    p.write_bytes(("\ufeff" if bom else "").encode() + s.encode("utf-8"))
    report["files"] += 1

print(dict(report))
print("manual:", len(manual))
for r, why in manual:
    print(f"  {r['code']} {pathlib.Path(r['file']).name}:{r['line']} {why} | {r['msg'][:60]}")
