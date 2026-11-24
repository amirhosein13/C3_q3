#!/usr/bin/env python3
"""Generates the inline SVG figures of the editorial (fig_*.svg)."""
import os
D = os.path.dirname(os.path.abspath(__file__))
HOUSE = "#4b5563"; EMPTY = "#d7dde4"; ROAD = "#2563eb"; BAD = "#dc2626"; SRC = "#16a34a"; SNK = "#ea580c"


def grid(g, C=44, M=24, evict=(), corners=(), road=None, barrier=False, borders=False, extra=""):
    H, W = len(g), len(g[0])
    o = [f'<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 {W*C+2*M} {H*C+2*M}" width="{W*C+2*M}" height="{H*C+2*M}">']
    o.append(f'<rect x="{M}" y="{M}" width="{W*C}" height="{H*C}" fill="#f3f5f7"/>')
    for r in range(H):
        for c in range(W):
            x, y = M+c*C+5, M+r*C+5
            o.append(f'<rect x="{x}" y="{y}" width="{C-10}" height="{C-10}" rx="5" fill="{HOUSE if g[r][c]=="#" else EMPTY}"/>')
            if (r, c) in evict:
                s = C-10
                o.append(f'<path d="M{x+7} {y+7} L{x+s-7} {y+s-7} M{x+s-7} {y+7} L{x+7} {y+s-7}" stroke="{BAD}" stroke-width="4" stroke-linecap="round"/>')
    for r in range(H+1):
        o.append(f'<line x1="{M}" y1="{M+r*C}" x2="{M+W*C}" y2="{M+r*C}" stroke="#9aa4b1" stroke-dasharray="3 3"/>')
    for c in range(W+1):
        o.append(f'<line x1="{M+c*C}" y1="{M}" x2="{M+c*C}" y2="{M+H*C}" stroke="#9aa4b1" stroke-dasharray="3 3"/>')
    if borders:
        o.append(f'<path d="M{M} {M} V{M+H*C} H{M+W*C}" stroke="{SRC}" stroke-width="6" fill="none" stroke-linecap="round"/>')
        o.append(f'<path d="M{M} {M} H{M+W*C} V{M+H*C}" stroke="{SNK}" stroke-width="6" fill="none" stroke-linecap="round" stroke-dasharray="1 0"/>')
    if barrier:
        cells = [(r, c) for r in range(H) for c in range(W) if g[r][c] == "#" and (r, c) not in evict]
        for i, (r, c) in enumerate(cells):
            for (y, x) in cells[i+1:]:
                if max(abs(r-y), abs(c-x)) <= 2:
                    o.append(f'<line x1="{M+c*C+C/2}" y1="{M+r*C+C/2}" x2="{M+x*C+C/2}" y2="{M+y*C+C/2}" stroke="#f59e0b" stroke-width="3"/>')
    for (y, x) in corners:
        o.append(f'<circle cx="{M+x*C}" cy="{M+y*C}" r="5" fill="{BAD}"/>')
    if road:
        d = "M" + " L".join(f"{M+x*C} {M+y*C}" for y, x in road)
        o.append(f'<path d="{d}" fill="none" stroke="{ROAD}" stroke-width="6" stroke-linejoin="round" stroke-linecap="round" opacity=".9"/>')
        o.append(f'<circle cx="{M}" cy="{M}" r="6" fill="{SRC}"/><circle cx="{M+W*C}" cy="{M+H*C}" r="6" fill="{SRC}"/>')
    o.append(extra)
    o.append('</svg>')
    return "\n".join(o)


def blocked(g):
    s = set()
    for r, row in enumerate(g):
        for c, ch in enumerate(row):
            if ch == "#":
                s |= {(r, c), (r+1, c), (r, c+1), (r+1, c+1)}
    return sorted(s)


sample = [".##...", ".#....", "##....", "....#."]
figs = {}
# 1: the sample with the optimal road
figs["sample"] = grid(sample, evict={(2, 0)}, road=[(0, 0), (4, 0), (4, 3), (2, 3), (2, 6), (4, 6)])
# 2: one kept house blocks its 4 corners
figs["corners"] = grid(["...", ".#.", "..."], C=50, corners=blocked(["...", ".#.", "..."]))
# 3: gap of one cell (distance 2) is closed, gap of two cells (distance 3) is open
g2 = ["#.#", "...", "..."]; g3 = ["#..#", "....", "...."]
figs["gap2"] = grid(g2, C=46, corners=blocked(g2))
figs["gap3"] = grid(g3, C=46, corners=blocked(g3), road=None,
                    extra=f'<path d="M{24+2*46} {24+3*46} V24" stroke="{ROAD}" stroke-width="6" stroke-linecap="round"/>')
gd = ["#..", "...", "..#"]
figs["diag2"] = grid(gd, C=46, corners=blocked(gd))
# 4: barrier graph of the sample, borders coloured
figs["barrier"] = grid(sample, C=48, barrier=True, borders=True)
figs["barrier_cut"] = grid(sample, C=48, barrier=True, borders=True, evict={(2, 0)})
# 5: H = 2 bipartite picture
h2 = [".#..#.#.", "#..#..#."]
figs["h2"] = grid(h2, C=40, barrier=True, borders=True)
for k, v in figs.items():
    open(os.path.join(D, f"fig_{k}.svg"), "w").write(v)
print("figures:", ", ".join(figs))
