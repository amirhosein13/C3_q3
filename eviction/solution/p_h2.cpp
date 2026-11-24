#include <bits/stdc++.h>
using namespace std;
int main(){
    int h,w;cin>>h>>w;vector<string> a(h);for(auto&s:a)cin>>s;
    if(h==1){cout<<count(a[0].begin(),a[0].end(),'#')<<"\n";return 0;}
    if(h>2){puts("0");return 0;}
    int ans=0;vector<int> top,bot;
    for(int c=0;c<w;c++){
        bool T=a[0][c]=='#',B=a[1][c]=='#';
        if(T){if(c==0)ans++;else top.push_back(c);}
        if(B){if(c==w-1)ans++;else bot.push_back(c);}
    }
    vector<char> used(top.size(),0);
    for(int b:bot)for(int i=0;i<(int)top.size();i++)if(!used[i]&&abs(top[i]-b)<=2){used[i]=1;ans++;break;}
    cout<<ans<<"\n";
}
