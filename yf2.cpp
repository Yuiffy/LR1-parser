#include<iostream>
#include<cstdio>
#include<map>
#include<vector>
#include<vector>
#include "cf.h"
#include "readTable.h"

using namespace std;
void error(int x) {
    if(x==0)printf("action ERROR\n");
    else if (x==1)printf("goto ERROR\n");
    else printf("Unknown Error\n");
}

void OutputCheck(vector<int>s,vector<int>w,int a,int act,int cas,int jing,string itos[], FILE* yufaFile) {
    fprintf(yufaFile, "%d.\taction[%d][%d] = ",cas, s.back(),a);
    if(act>=0)fprintf(yufaFile, "S%d",act);
    else if(act!=ACC && act!=KO)fprintf(yufaFile, "r%d",-act-1);
    else if(act!=KO)fprintf(yufaFile, "ACC");
    else fprintf(yufaFile, "NONE");

    fprintf(yufaFile, "\nstack: ");
    for(int i=s.size()-1; i>0; i--)
        fprintf(yufaFile, "%d(%s) ",s[i],itos[s[i]].c_str());
    fprintf(yufaFile, "%d(%s)\n",s[0],itos[s[0]].c_str());
    fprintf(yufaFile, "input: ");
    for(int i=w.size()-1; i>0; i--)
        if(w[i]!=jing)fprintf(yufaFile, "[%s %d] ",itos[w[i]].c_str(), w[i]);
        else fprintf(yufaFile, "[#] ");
    if(w[0]!=jing)    fprintf(yufaFile, "[%s %d]",itos[w[0]].c_str(), w[0]);
    else fprintf(yufaFile, "[#]");
    fprintf(yufaFile, "\n");
}

struct Node {
    string s;
    int next;
} node[12345];

int head[12345],nn,tail[12345];

void initNode() {
    for(int i=0;i<12345;i++){
        node[i].s="";
        node[i].next=-1;
    }
    memset(head,-1,sizeof(head));
    memset(tail,-1,sizeof(tail));
    nn=0;
}

void addNode(int x, string s) {
    if(head[x]==-1){
        head[x]=nn;
        tail[x]=nn;
    }else{
        node[tail[x]].next=nn;
    }
    node[nn].s=s;
    node[nn].next=-1;
    tail[x]=nn;
    nn++;
}

void Merge(int x,int y){
    tail[x]=tail[y];
    node[tail[x]].next=head[y];
}

void putNode(int x){
    int j=0;
    for(int i=head[x]; i!=-1 && j<200; i=node[x].next){
        cout<<node[i].s<<endl;
        j++;
    }
}

struct ST{
    string first;
    int second;
    int blk;
    ST(string f,int s, int b){
        first=f;
        second=s;
        blk=b;
    }
};

const int FUZHI=38;     ///<赋值函数>-><变量> = <表达式>
const int BIAODASHI=23; ///<表达式>-><因子> <运算符> <表达式>
const int CHENG=21;     ///<因子>-><因式> * <因子>
const int CHU=22;       ///<因子>-><因式> / <因子>
const int BIANLIANG=3;  ///<变量>->id
const int DIGIT=9;      ///<因式>->digit
const int BIAODA2=8;    ///<表达式>-><因子>
const int YINZI=7;      ///<因子>-><因式>
const int KUOHAO=24;    ///<因式>->( <表达式> )
const int FUCHUZHI=25;  ///<赋初值> -> = <表达式>
const int FUNULL=10;    ///<赋初值> -> ε
const int SHENGMING=19; ///<声明> -> <类型> <变量> <赋初值>
const int IF=45;        ///<条件语句>->if ( <表达式> ) { <函数块> } <否则语句>
const int ELSE=47;      ///<否则语句>->else { <函数块> }
///从右往左，分别是2,4,6,....

ST yuyi(int k,vector<ST>stk,map<string,int> &mp,int A[], int &cnt, int &cnt2,int &blkcnt) {
    int len=stk.size();
    if(k==BIANLIANG) {  ///变量，从map里读值
        string re=stk[len - 2].first;
        int x=mp[re];
        int blk=stk[len-2].blk;
        return ST(re,x,blk);
    } else if(k==CHENG || k==CHU || k==BIAODASHI) {
        int t=cnt++;
        string re="t"+itos(t);
        int x=0 , y=stk[len-6].second, z=stk[len-2].second;
        string op = stk[len - 4].first;
        switch(k) {
        case CHENG:
            x=y*z;
            break;
        case CHU:
            if(z==0)printf("ERROR:divide 0\n");
            else x=y/z;
            break;
        case BIAODASHI:
            if(op=="+")x=y+z;
            else if(op=="-")x=y-z;
            else if(op=="<")x=(y<z);
            else if(op==">")x=(y>z);
            else if(op=="==")x=(y==z);
            else if(op=="!=")x=(y!=z);
            else printf("Error(%s)\n",stk[len-4].first.c_str());
        }
        printf("%d:\t",cnt2++);
        string yz4= re+"="+stk[len-6].first + stk[len-4].first + stk[len-2].first ;
        cout<<yz4;
        printf("  \t(%d = %d %s %d)",x, y,op.c_str(), z);
        cout<<endl;
        int blk1=stk[len-6].blk;
        int blk2=stk[len-2].blk;
        Merge(blk1,blk2);
        addNode(blk1, yz4);
        return ST(re,x,blk1);

    } else if(k==FUZHI) {
        string re=stk[len - 6].first;
        int x=stk[len-2].second;
        mp[re] = x;
        printf("%d:\t",cnt2++);
        string yz4=stk[len - 6].first + stk[len - 4].first + stk[len - 2].first;
        cout<<yz4<<endl;
        int blk=stk[len-2].blk;
        addNode(blk, yz4);
        return ST(re,x,blk);
    } else if(k==YINZI || k==BIAODA2) {
        return stk[len - 2];
    } else if(k==DIGIT) {
        string s=stk[len-2].first;
        int x=stoi(stk[len-2].first);
        return ST(s,x,0);
    } else if(k==KUOHAO) {
        return stk[len-4];
    } else if(k==SHENGMING) {
        string s=stk[len-4].first;
        int x=stk[len-2].second;
        mp[s]=x;
        printf("%d:\t",cnt2++);
        string yz4=s+"="+itos(x);
        cout<<yz4<<endl;
        int blk=stk[len-2].blk;
        addNode(blk, yz4);
        return ST(s,x,blk);
    } else if(k==FUNULL) {
        return ST("",0,0);
    } else if(k==IF) {

    } else return stk[len - 2];
}

/*************************************************
Function:       LR
Description:    语法分析，判断该语言能否被接受
Calls:          error,OutputCheck
Input:      w为逆序的输入数字序列，vector类型，可用pop_back删除最后一个/back取得最后一个的值
                  action[][]为action表，action[x][y]=z表示栈顶为x，输入为a时的操作，大于等于0为移进并跳转到状态Sz
                                                                                                                                                小于0且大于ACC表示按(-z-1)号产生式进行归约
                                                                                                                                                等于ACC（设为-10000)表示可接受
                    gto[][]为goto表，归并弹出2*beta个符号后要把生成式左端A和gto[S'][A]先后压入栈中，S'为栈顶状态
                    A为产生式表左端，B为产生式表右端，beta为产生式 表右端元素数量，jing为#号的编号

Output:         输出语法分析过程，栈/输入缓冲区
Return:         返回0说明ACC
Others:         // 其它说明
*************************************************/
int LR(vector<int> &w,vector<string> &wtk,int action[][MAX],int gto[][MAX], int A[],string B[], int beta[],const int &jing,string itos[MAX], FILE *yufaFile) {
    vector<int>s;
    vector<ST> stk;
    map<string,int> mp;
    int idcnt=0;
    int sencnt=0;
    int blkcnt=0;
    int lastblk=0;
    initNode();
    s.clear();
    stk.clear();
    s.push_back(jing);
    s.push_back(0);
    stk.push_back(ST("#",0,0));
    stk.push_back(ST("0",0,0));
    int cas=1;
    while(true) {
        int a=w.back();
        string b=wtk.back();
        //printf("%d,%d\n",s.back(),a);
        int act=action[s.back()][a];
        OutputCheck(s,w,a,act,cas++,jing,itos, yufaFile);
        if(act>=0) {    ///移进
            s.push_back(a);
            s.push_back(act);
            w.pop_back();
            stk.push_back(ST(b,0,0));
            stk.push_back(ST("act",0,0));
            wtk.pop_back();
        } else if (act > ACC) { ///规约
            int k= -act - 1;
            ST id = yuyi(k,stk,mp,A,idcnt,sencnt,blkcnt);
            lastblk = id.blk;
            for(int i=0; i<2*beta[k]; i++) {
                s.pop_back();
                stk.pop_back();
            }
            int s_=s.back();
            s.push_back(A[k]);
            stk.push_back(id);
            if(gto[s_][A[k]]==KO) {
                fprintf(yufaFile, "gto[%d][%d]=KO , beta[%d]=%d\n",s_,A[k],k,beta[k]);
                error(1);
                break;
            }
            s.push_back(gto[s_][A[k]]);
            stk.push_back(ST("gto",0,0));
            fprintf(yufaFile, "(k=%d)\t%s->%s",k,itos[A[k]].c_str(), B[k].c_str());
            fprintf(yufaFile, "\t (goto[%d][%d]=%d)(beta[%d]=%d)\n",s_,A[k],gto[s_][A[k]],k,beta[k]);
        } else if (act==ACC) break;///完成
        else {///错误
            error(0);
            break;
        }
        fprintf(yufaFile, "\n");
    }
    putNode(lastblk);
    return 0;
}
int action[MAX][MAX]= {{3,4,KO},{KO,KO,ACC},{3,4,KO},{3,4,KO},{-3,-3,-3},{-1,-1,-1},{-2,-2,-2}};
int gto[MAX][MAX]= {{KO,KO,KO,1,2},
    {KO,KO,KO,KO,KO},
    {KO,KO,KO,KO,5},
    {KO,KO,KO,KO,6},
    {KO,KO,KO,KO,KO},
    {KO,KO,KO,KO,KO},
    {KO,KO,KO,KO,KO}
};
int main() {
    int A[MAX]= {3,4,4};
    string B[MAX]= {"BB","aB","b"};
    string itos[MAX];
    ///a=0,b=1,#=2,S=3,B=4
    int beta[MAX]= {2,2,1};
    int jing=2;
    vector<int> w;
    vector<string> wtk;
    vector<pair<int,string> > inp;
    GetTable("biao.txt",action,gto,A,B,beta,jing,itos);
    inp=cf("source2.c");
    inp.push_back(make_pair(jing,"#"));
    for(int i=inp.size()-1; i>=0; i--) {
        w.push_back(inp[i].first);
        wtk.push_back(inp[i].second);
    }
    FILE* yufaFile = fopen("yfLog.txt","w");
    printf("===================\n");
    if(LR(w,wtk,action,gto,A,B,beta,jing,itos,yufaFile)==0) fprintf(yufaFile, "ACC");
    else fprintf(yufaFile, "ERROR");
    return 0;
}
