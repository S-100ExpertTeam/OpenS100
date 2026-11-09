"""Parse an MSBuild log into unique compiler warnings (JSON) and print a summary."""
import collections, json, pathlib, re, sys

log_path, out_path = sys.argv[1], sys.argv[2]
only = set(sys.argv[3].split(",")) if len(sys.argv) > 3 else None
log = pathlib.Path(log_path).read_text(encoding="utf-8", errors="replace")
pat = re.compile(r"^(?P<file>[A-Za-z]:[^(\n]+)\((?P<line>\d+)(?:,(?P<col>\d+))?\): warning (?P<code>C\d+): (?P<msg>.*?)\s*\[(?P<proj>[^\]]+)\]\s*$", re.M)
seen, rows = set(), []
for m in pat.finditer(log):
    key = (m["file"].lower(), m["line"], m["col"], m["code"], m["msg"])
    if key in seen:
        continue
    seen.add(key)
    if only and m["code"] not in only:
        continue
    rows.append({"file": m["file"], "line": int(m["line"]), "col": int(m["col"] or 0), "code": m["code"],
                 "msg": m["msg"], "proj": pathlib.Path(m["proj"]).stem})
pathlib.Path(out_path).write_text(json.dumps(rows, indent=1), encoding="utf-8")
print("unique warnings:", len(rows))
print("per project:", dict(collections.Counter(r["proj"] for r in rows)))
print("per code:", collections.Counter(r["code"] for r in rows).most_common(15))
print("per file (top 15):", collections.Counter(pathlib.Path(r["file"]).name for r in rows).most_common(15))
