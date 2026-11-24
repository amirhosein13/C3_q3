#include <bits/stdc++.h>
using namespace std;
int h,w;vector<string> a;
bool ok(){
    auto fr=[&](int y,int x){for(int r=y-1;r<=y;r++)for(int c=x-1;c<=x;c++)if(r>=0&&r<h&&c>=0&&c<w&&a[r][c]=='#')return false;return true;};
    if(!fr(0,0))return false;
    vector<vector<char>> v(h+1,vector<char>(w+1,0));queue<pair<int,int>> q;q.push({0,0});v[0][0]=1;
    int dy[]={1,-1,0,0},dx[]={0,0,1,-1};
    while(q.size()){auto[y,x]=q.front();q.pop();if(y==h&&x==w)return true;
        for(int k=0;k<4;k++){int ny=y+dy[k],nx=x+dx[k];if(ny<0||nx<0||ny>h||nx>w||v[ny][nx]||!fr(ny,nx))continue;v[ny][nx]=1;q.push({ny,nx});}}
    return false;
}
vector<pair<int,int>> p;int best;
void go(int i,int k){
    if(k>=best)return;
    if(i==(int)p.size()){if(ok())best=k;return;}
    go(i+1,k);a[p[i].first][p[i].second]='.';go(i+1,k+1);a[p[i].first][p[i].second]='#';
}
int main(){
    cin>>h>>w;a.resize(h);for(auto&s:a)cin>>s;
    for(int r=0;r<h;r++)for(int c=0;c<w;c++)if(a[r][c]=='#')p.push_back({r,c});
    int n=p.size();best=n;go(0,0);
    cout<<best<<"\n";
}
