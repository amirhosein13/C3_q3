#!/usr/bin/env python3
"""Checks that every GroupMin regex in score_type.txt selects exactly the tests of its subtask.
Usage: python3 verify_score_type.py   (run from cms_upload/, needs ../tests/mapping from `tps gen`)"""
import json, re, zipfile, os, sys
here = os.path.dirname(os.path.abspath(__file__))
params = json.load(open(os.path.join(here, "score_type.txt")))
names = sorted({n.rsplit(".", 1)[0] for n in zipfile.ZipFile(os.path.join(here, "tests.zip")).namelist()})
for n in names:
    z = zipfile.ZipFile(os.path.join(here, "tests.zip")).namelist()
    assert n + ".in" in z and n + ".out" in z, n
subtasks = json.load(open(os.path.join(here, "..", "subtasks.json")))["subtasks"]
order = sorted(subtasks, key=lambda s: subtasks[s]["index"])
want = {s: set() for s in order}
for line in open(os.path.join(here, "..", "tests", "mapping")):
    s, t = line.split(); want[s].add(t)
ok = len(params) == len(order) and sum(p[0] for p in params) == 100
for (score, rx), s in zip(params, order):
    got = {n for n in names if re.compile(rx).match(n)}   # CMS uses pattern.match
    good = got == want[s] and score == subtasks[s]["score"]
    ok &= good
    print(f"{'OK ' if good else 'BAD'} {s:8s} score={score:3d} regex={rx:22s} matched={len(got):3d} expected={len(want[s]):3d}")
print("tests in zip:", len(names), "| total score:", sum(p[0] for p in params))
sys.exit(0 if ok else 1)
