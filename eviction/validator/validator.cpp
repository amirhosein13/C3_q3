#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;
// min-cut (barrier) answer, used to check the "answer <= 1" subtask
int H,W;vector<string> a;
int solve(){
    int n=H*W,S=2*n,T=S+1;
    vector<vector<pair<int,int>>> g(2*n+2);vector<int> cap;
    auto ae=[&](int u,int v,int c){g[u].push_back({v,(int)cap.size()});cap.push_back(c);g[v].push_back({u,(int)cap.size()});cap.push_back(0);};
    for(int r=0;r<H;r++)for(int c=0;c<W;c++)if(a[r][c]=='#'){int id=r*W+c;ae(2*id,2*id+1,1);
        if(c==0||r==H-1)ae(S,2*id,1e9);if(r==0||c==W-1)ae(2*id+1,T,1e9);
        for(int dr=-2;dr<=2;dr++)for(int dc=-2;dc<=2;dc++){int y=r+dr,x=c+dc;if((dr||dc)&&y>=0&&y<H&&x>=0&&x<W&&a[y][x]=='#')ae(2*id+1,2*(y*W+x),1e9);}}
    int f=0;
    while(true){vector<pair<int,int>> pr(2*n+2,{-1,-1});vector<char> vis(2*n+2,0);queue<int> q;q.push(S);vis[S]=1;
        while(q.size()){int u=q.front();q.pop();for(auto [v,id]:g[u])if(cap[id]&&!vis[v]){vis[v]=1;pr[v]={u,id};q.push(v);}}
        if(!vis[T])break;for(int v=T;v!=S;v=pr[v].first){cap[pr[v].second]--;cap[pr[v].second^1]++;}f++;}
    return f;
}
int main(int argc,char**argv){
    registerValidation(argc,argv);
    string grp=validator.group();
    H=inf.readInt(1,50,"H");inf.readSpace();W=inf.readInt(1,50,"W");inf.readEoln();
    a.resize(H);for(int i=0;i<H;i++){a[i]=inf.readToken(format("[.#]{%d}",W),"row");inf.readEoln();}
    inf.readEof();
    int cnt=0;for(auto&s:a)cnt+=count(s.begin(),s.end(),'#');
    if(grp=="h1")ensuref(H==1,"subtask h1: H must be 1");
    if(grp=="small")ensuref(cnt<=12,"subtask small: at most 12 '#' (got %d)",cnt);
    if(grp=="one")ensuref(solve()<=1,"subtask one: answer must be <= 1");
    if(grp=="h2")ensuref(H<=2,"subtask h2: H must be <= 2");
}
