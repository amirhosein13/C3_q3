#include <bits/stdc++.h>
using namespace std;
// WRONG: Dijkstra over road crossings, paying for every inhabited cell around each visited crossing (counts residents many times)
int main(){
    int h,w;cin>>h>>w;vector<string> a(h);for(auto&s:a)cin>>s;
    auto cost=[&](int y,int x){int k=0;for(int r=y-1;r<=y;r++)for(int c=x-1;c<=x;c++)if(r>=0&&r<h&&c>=0&&c<w&&a[r][c]=='#')k++;return k;};
    vector<vector<int>> d(h+1,vector<int>(w+1,1e9));
    priority_queue<tuple<int,int,int>,vector<tuple<int,int,int>>,greater<>> pq;
    d[0][0]=cost(0,0);pq.push({d[0][0],0,0});
    int dy[]={1,-1,0,0},dx[]={0,0,1,-1};
    while(pq.size()){auto[dd,y,x]=pq.top();pq.pop();if(dd>d[y][x])continue;
        for(int k=0;k<4;k++){int ny=y+dy[k],nx=x+dx[k];if(ny<0||nx<0||ny>h||nx>w)continue;
            int nd=dd+cost(ny,nx);if(nd<d[ny][nx])d[ny][nx]=nd,pq.push({nd,ny,nx});}}
    cout<<d[h][w]<<"\n";
}
