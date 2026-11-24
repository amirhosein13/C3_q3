// CMS-protocol checker: checker <input> <correct_output> <contestant_output>
// stdout: score (1 or 0), stderr: message. The answer is a unique integer, so this is a
// whitespace-insensitive comparison of the single token (extra tokens => wrong).
#include <bits/stdc++.h>
using namespace std;
void res(double s,const string&m){printf("%g\n",s);fprintf(stderr,"%s\n",m.c_str());exit(0);}
int main(int argc,char**argv){
    if(argc<4){fprintf(stderr,"usage: checker input correct_output contestant_output\n");return 1;}
    ifstream fc(argv[2]),fo(argv[3]);string j,p,extra;
    if(!(fc>>j)){fprintf(stderr,"cannot read correct output\n");return 1;}
    if(!(fo>>p))res(0,"Output isn't correct");
    if(fo>>extra)res(0,"Output isn't correct");
    if(p.size()>20||p.find_first_not_of("-0123456789")!=string::npos)res(0,"Output isn't correct");
    if(p!=j)res(0,"Output isn't correct");
    res(1,"Output is correct");
}
