# Express Eviction (TPS task, target: CMS via Admin web panel only)

* Persian upload guide: [`CMS_UPLOAD_GUIDE.md`](CMS_UPLOAD_GUIDE.md)
* Everything to upload by hand: [`cms_upload/`](cms_upload/) (values in `cms_upload/task_settings.txt`)
* Statements: `statement/index.md` (en), `statement/index-fa.md` (fa); PDFs built by `statement/build.sh`

## Limits (chosen)
Time 1 s (model ≤ 26 ms on every test), memory 256 MiB, `1 ≤ H, W ≤ 50` (from the original statement).
The answer is a unique integer → white-diff / whitespace-insensitive comparison; `checker/checker.cpp`
(CMS protocol: `checker input correct_output contestant_output`, score on stdout, message on stderr) is optional.

## Intended solution — min vertex cut (max flow)
The road runs on the `(H+1)×(W+1)` lattice of corner points. A kept resident blocks the 4 corners of their cell.
The road is impossible iff kept inhabited cells form a chain from the **left/bottom** border to the **top/right**
border where consecutive cells satisfy `max(|dr|,|dc|) ≤ 2` (the blocked 2×2 corner blocks are then 8-connected);
a cell touches left/bottom if `c = 0` or `r = H−1`, top/right if `r = 0` or `c = W−1`.
So the answer is the minimum number of cells to delete to break every such chain = min **vertex** cut:
split each `#` into `in→out` (cap 1), `S→in` for left/bottom cells, `out→T` for top/right cells,
`out→in'` (∞) for every pair at Chebyshev distance ≤ 2. Dinic. `V ≤ 5002`, `E ≈ 25·HW`, flow ≤ `H+W−1`,
so `O(F·E) ≈ 6·10^6` — instant.

## Subtasks
| # | name | score | constraint | idea rewarded |
|---|---|---|---|---|
| 0 | samples | 0 | sample | – |
| 1 | h1 | 6 | `H = 1` | every cell is touched → count `#` |
| 2 | small | 14 | ≤ 12 `#` | brute force over eviction sets + BFS |
| 3 | one | 18 | answer ≤ 1 | BFS, then try deleting each single cell |
| 4 | h2 | 17 | `H ≤ 2` | barrier = bipartite graph rows 0/1, König + greedy matching |
| 5 | full | 45 | – | barrier modelling + max-flow min vertex cut |

Includes: small ⊇ samples, one ⊇ samples, h2 ⊇ h1, full ⊇ all. 77 tests.
(The grid is tiny, so there is no meaningful polynomial "partial complexity" subtask; the partial subtasks
reward different ideas instead.)

## Solutions (`tps invoke` — every verdict matches `solutions.json`)
| solution | samples | h1 | small | one | h2 | full | CMS score |
|---|---|---|---|---|---|---|---|
| model.cpp | AC | AC | AC | AC | AC | AC | 100 |
| brute.cpp (exponential subsets) | AC | TLE | AC | TLE | TLE | TLE | 14 |
| tle_iddfs.cpp (iterative deepening) | AC | TLE | AC | AC | TLE | TLE | 32 |
| p_h1.cpp (count `#`) | WA | AC | WA | WA | WA | WA | 6 |
| p_one.cpp (0/1 check) | AC | WA | WA | AC | WA | WA | 18 |
| p_h2.cpp (H≤2 matching) | WA | AC | WA | WA | AC | WA | 23 |
| wa_cheb1.cpp (barrier only if cells touch) | AC | AC | WA | WA | WA | WA | 6 |
| wa_border.cpp (border off-by-one) | WA | AC | WA | WA | WA | WA | 6 |
| wa_nosplit.cpp (edge cut, no node split) | AC | AC | WA | WA | WA | WA | 6 |
| wa_dijkstra.cpp (pays per visited corner) | WA | WA | WA | WA | WA | WA | 0 |

Answers are ≤ 2500, so `int` overflow is not a realistic failure mode for this task.
Note: a Dijkstra that only pays for cells newly touched w.r.t. the previous corner agreed with the model on
20 000+ random tests, so it was *not* registered as a wrong solution.

## Verification done
* `tps gen`: all tests generated, validated per subtask (`validator.cpp --group {subtask}`, the `one` group
  recomputes the answer with a flow).
* Stress: model vs brute on 7 000 random grids (≤ 14×14, ≤ 14 `#`); `p_h2` vs brute on 3 000 `H ≤ 2` grids.
* TL: model max 26 ms; brute / iddfs run > 5–10 s on their TLE tests.
* `cms_upload/verify_score_type.py`: each GroupMin regex selects exactly its subtask's tests.
* Checker: accepts the model output on all 77 tests, rejects off-by-one answers on all 77.
