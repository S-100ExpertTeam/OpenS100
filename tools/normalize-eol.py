"""Rewrite modified working-tree files with the line endings their committed version uses.

The repository stores files as committed (.gitattributes: -text) and has a mix of LF and CRLF
blobs, while many working-tree files are CRLF from an old autocrlf checkout. Editing such a file
and committing it as-is flips every line. Run this in the repo root before `git add`.
"""
import subprocess, pathlib, sys

names = subprocess.run(["git", "diff", "--name-only"], capture_output=True, text=True, check=True).stdout.split("\n")
names = [n for n in names if n]
if not names:
    print("no modified files"); sys.exit(0)
eol = {}
out = subprocess.run(["git", "ls-files", "--eol", "--"] + names, capture_output=True, text=True, check=True).stdout
for line in out.splitlines():
    info, path = line.split("\t", 1)
    eol[path.strip()] = info.split()[0]          # i/lf, i/crlf, i/mixed, i/-text, i/none
fixed = 0
for n in names:
    kind = eol.get(n, "")
    p = pathlib.Path(n)
    if not p.exists():
        continue
    b = p.read_bytes()
    if b"\0" in b[:4096]:
        continue                                   # UTF-16 / binary
    if kind == "i/lf":
        nb = b.replace(b"\r\n", b"\n")
    elif kind == "i/crlf":
        nb = b.replace(b"\r\n", b"\n").replace(b"\n", b"\r\n")
    else:
        continue                                   # mixed/none: leave as written
    if nb != b:
        p.write_bytes(nb); fixed += 1
print(f"{len(names)} modified, {fixed} normalised to the committed line endings")
