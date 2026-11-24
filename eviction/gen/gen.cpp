#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;
// usage: gen <mode> H W [params...] <seed-tag>
//  rand  H W p        : each cell '#' with probability p percent
//  cnt   H W k        : exactly k '#' at random positions
//  carve H W p e      : random self-avoiding road from corner to corner, its neighbourhood cleared,
//                       background density p, then e (0/1) '#' put next to the road  -> answer <= e
//  bar   H W k g p    : k barriers (left/bottom -> top/right) whose cells are spaced g apart, noise p
//  maze  H W gap p    : alternating horizontal walls with openings (forces a long winding road), noise p
//  full  H W          : every cell '#'
//  diag  H W k        : k diagonal Chebyshev-2-spaced barriers (gap 2 kills "touching only" logic)
int H,W;vector<string> g;
void noise(int p){for(auto&s:g)for(auto&ch:s)if(ch=='.'&&rnd.next(100)<p)ch='#';}
void putc_(int r,int c){if(r>=0&&r<H&&c>=0&&c<W)g[r][c]='#';}
int main(int argc,char**argv){
    registerGen(argc,argv,1);
    string m=argv[1];H=atoi(argv[2]);W=atoi(argv[3]);
    g.assign(H,string(W,'.'));
    if(m=="rand"){int p=atoi(argv[4]);for(auto&s:g)for(auto&ch:s)ch=rnd.next(100)<p?'#':'.';}
    else if(m=="full"){for(auto&s:g)for(auto&ch:s)ch='#';}
    else if(m=="cnt"){int k=atoi(argv[4]);vector<int> id(H*W);iota(id.begin(),id.end(),0);shuffle(id.begin(),id.end());for(int i=0;i<k;i++)g[id[i]/W][id[i]%W]='#';}
    else if(m=="carve"){
        int p=atoi(argv[4]),e=atoi(argv[5]);
        for(auto&s:g)for(auto&ch:s)ch=rnd.next(100)<p?'#':'.';
        // random walk on lattice vertices biased towards (H,W), loop-erased
        vector<pair<int,int>> path={{0,0}};map<pair<int,int>,int> pos;pos[{0,0}]=0;
        int y=0,x=0;
        while(!(y==H&&x==W)){
            int dy[]={1,0,-1,0},dx[]={0,1,0,-1};int k;
            int r=rnd.next(10);k=r<4?0:r<8?1:r<9?2:3;
            int ny=y+dy[k],nx=x+dx[k];if(ny<0||nx<0||ny>H||nx>W)continue;
            y=ny;x=nx;
            if(pos.count({y,x})){int q=pos[{y,x}];while((int)path.size()>q+1){pos.erase(path.back());path.pop_back();}}
            else{pos[{y,x}]=path.size();path.push_back({y,x});}
        }
        vector<pair<int,int>> adj;
        for(auto [vy,vx]:path)for(int r=vy-1;r<=vy;r++)for(int c=vx-1;c<=vx;c++)if(r>=0&&r<H&&c>=0&&c<W){g[r][c]='.';adj.push_back({r,c});}
        if(e){auto [r,c]=adj[rnd.next((int)adj.size())];g[r][c]='#';}
    }
    else if(m=="bar"||m=="diag"){
        int k=atoi(argv[4]),gp=m=="bar"?atoi(argv[5]):2,p=m=="bar"?atoi(argv[6]):0;
        // barrier i: from left column / bottom row going up-right with step gp
        for(int i=0;i<k;i++){
            int off=(i+1)*(H+W)/(k+1);
            int r,c;if(off<H){r=H-1-off;c=0;}else{r=H-1;c=off-H;}
            // walk from (r,c) in direction up-right until leaving the top/right border
            r=min(r+ (c==0?0:0),H-1);
            int sr=c==0?r:H-1,sc=c==0?0:c;
            // start on left/bottom border
            if(c==0){sr=r;sc=0;}else{sr=H-1;sc=c;}
            int cr=sr,cc=sc;
            while(cr>=0&&cc<W){putc_(cr,cc);int t=rnd.next(3);if(t==0)cr-=gp;else if(t==1)cc+=gp;else cr-=gp,cc+=gp;}
            // ensure it touches top or right
            if(cr<0)putc_(0,min(cc,W-1));else putc_(max(cr,0),W-1);
        }
        noise(p);
    }
    else if(m=="maze"){
        int gap=atoi(argv[4]),p=atoi(argv[5]);bool L=true;
        for(int r=2;r<H-2;r+=gap,L=!L){
            if(L)for(int c=0;c<=W-4;c++)g[r][c]='#';else for(int c=3;c<W;c++)g[r][c]='#';
        }
        noise(p);
    }
    else quitf(_fail,"bad mode");
    printf("%d %d\n",H,W);for(auto&s:g)printf("%s\n",s.c_str());
}
