#include <bits/stdc++.h>
using namespace std;
int main(){int h,w;cin>>h>>w;int k=0;for(int i=0;i<h;i++){string s;cin>>s;k+=count(s.begin(),s.end(),'#');}cout<<k<<"\n";}
