#include <bits/stdc++.h>
using namespace std;
struct E{int to,c;};
vector<E> e;vector<vector<int>> g;vector<int> lv,it;
void ae(int u,int v,int c){g[u].push_back(e.size());e.push_back({v,c});g[v].push_back(e.size());e.push_back({u,0});}
bool bfs(int s,int t){fill(lv.begin(),lv.end(),-1);lv[s]=0;queue<int> q;q.push(s);while(q.size()){int u=q.front();q.pop();for(int id:g[u])if(e[id].c&&lv[e[id].to]<0)lv[e[id].to]=lv[u]+1,q.push(e[id].to);}return lv[t]>=0;}
int dfs(int u,int t,int f){if(u==t)return f;for(int&i=it[u];i<(int)g[u].size();i++){E&x=e[g[u][i]];if(x.c&&lv[x.to]==lv[u]+1){int d=dfs(x.to,t,min(f,x.c));if(d){x.c-=d;e[g[u][i]^1].c+=d;return d;}}}return 0;}
int main(){
    int h,w;cin>>h>>w;vector<string> a(h);for(auto&s:a)cin>>s;
    int n=h*w,S=n,T=n+1;g.assign(n+2,{});lv.resize(n+2);it.resize(n+2);
    for(int r=0;r<h;r++)for(int c=0;c<w;c++)if(a[r][c]=='#'){
        int id=r*w+c;
        if(c==0||r==h-1)ae(S,id,1);
        if(r==0||c==w-1)ae(id,T,1);
        for(int dr=-2;dr<=2;dr++)for(int dc=-2;dc<=2;dc++){int y=r+dr,x=c+dc;
            if((dr||dc)&&y>=0&&y<h&&x>=0&&x<w&&a[y][x]=='#')ae(id,y*w+x,1);}
    }
    int f=0;while(bfs(S,T)){fill(it.begin(),it.end(),0);while(int d=dfs(S,T,1e9))f+=d;}
    cout<<f<<"\n";
}
