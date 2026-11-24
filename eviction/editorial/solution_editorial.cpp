#include <bits/stdc++.h>
using namespace std;

struct Dinic {
    struct Edge { int to, cap; };
    vector<Edge> e; vector<vector<int>> g; vector<int> lvl, it;
    Dinic(int n) : g(n), lvl(n), it(n) {}
    void add(int u, int v, int c) {
        g[u].push_back(e.size()); e.push_back({v, c});
        g[v].push_back(e.size()); e.push_back({u, 0});
    }
    bool bfs(int s, int t) {
        fill(lvl.begin(), lvl.end(), -1); lvl[s] = 0;
        queue<int> q; q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int id : g[u]) if (e[id].cap && lvl[e[id].to] < 0)
                lvl[e[id].to] = lvl[u] + 1, q.push(e[id].to);
        }
        return lvl[t] >= 0;
    }
    int dfs(int u, int t, int f) {
        if (u == t) return f;
        for (int &i = it[u]; i < (int)g[u].size(); i++) {
            Edge &x = e[g[u][i]];
            if (x.cap && lvl[x.to] == lvl[u] + 1)
                if (int d = dfs(x.to, t, min(f, x.cap))) {
                    x.cap -= d; e[g[u][i] ^ 1].cap += d; return d;
                }
        }
        return 0;
    }
    int maxflow(int s, int t) {
        int f = 0;
        while (bfs(s, t)) { fill(it.begin(), it.end(), 0); while (int d = dfs(s, t, INT_MAX)) f += d; }
        return f;
    }
};

int main() {
    int H, W; cin >> H >> W;
    vector<string> a(H); for (auto &s : a) cin >> s;
    const int INF = 1e9, S = 2 * H * W, T = S + 1;
    Dinic D(2 * H * W + 2);
    auto in  = [&](int r, int c) { return 2 * (r * W + c); };
    auto out = [&](int r, int c) { return 2 * (r * W + c) + 1; };
    for (int r = 0; r < H; r++) for (int c = 0; c < W; c++) if (a[r][c] == '#') {
        D.add(in(r, c), out(r, c), 1);                       // evicting this resident costs 1
        if (c == 0 || r == H - 1) D.add(S, in(r, c), INF);   // touches left/bottom border
        if (r == 0 || c == W - 1) D.add(out(r, c), T, INF);  // touches top/right border
        for (int dr = -2; dr <= 2; dr++) for (int dc = -2; dc <= 2; dc++) {
            int y = r + dr, x = c + dc;
            if ((dr || dc) && 0 <= y && y < H && 0 <= x && x < W && a[y][x] == '#')
                D.add(out(r, c), in(y, x), INF);             // the two houses close the gap between them
        }
    }
    cout << D.maxflow(S, T) << "\n";
}
