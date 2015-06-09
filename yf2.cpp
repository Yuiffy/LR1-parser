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

const int FUZHI=38;     ///<��ֵ����>-><����> = <���ʽ>
const int BIAODASHI=23; ///<���ʽ>-><����> <�����> <���ʽ>
const int CHENG=21;     ///<����>-><��ʽ> * <����>
const int CHU=22;       ///<����>-><��ʽ> / <����>
const int BIANLIANG=3;  ///<����>->id
const int DIGIT=9;      ///<��ʽ>->digit
const int BIAODA2=8;    ///<���ʽ>-><����>
const int YINZI=7;      ///<����>-><��ʽ>
const int KUOHAO=24;    ///<��ʽ>->( <���ʽ> )
const int FUCHUZHI=25;  ///<����ֵ> -> = <���ʽ>
const int FUNULL=10;    ///<����ֵ> -> ��
const int SHENGMING=19; ///<����> -> <����> <����> <����ֵ>
const int IF=45;        ///<�������>->if ( <���ʽ> ) { <������> } <�������>
const int ELSE=47;      ///<�������>->else { <������> }
///�������󣬷ֱ���2,4,6,....

ST yuyi(int k,vector<ST>stk,map<string,int> &mp,int A[], int &cnt, int &cnt2,int &blkcnt) {
    int len=stk.size();
    if(k==BIANLIANG) {  ///��������map���ֵ
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
Description:    �﷨�������жϸ������ܷ񱻽���
Calls:          error,OutputCheck
Input:      wΪ����������������У�vector���ͣ�����pop_backɾ�����һ��/backȡ�����һ����ֵ
                  action[][]Ϊaction��action[x][y]=z��ʾջ��Ϊx������Ϊaʱ�Ĳ��������ڵ���0Ϊ�ƽ�����ת��״̬Sz
                                                                                                                                                С��0�Ҵ���ACC��ʾ��(-z-1)�Ų���ʽ���й�Լ
                                                                                                                                                ����ACC����Ϊ-10000)��ʾ�ɽ���
                    gto[][]Ϊgoto���鲢����2*beta�����ź�Ҫ������ʽ���A��gto[S'][A]�Ⱥ�ѹ��ջ�У�S'Ϊջ��״̬
                    AΪ����ʽ����ˣ�BΪ����ʽ���Ҷˣ�betaΪ����ʽ ���Ҷ�Ԫ��������jingΪ#�ŵı��

Output:         ����﷨�������̣�ջ/���뻺����
Return:         ����0˵��ACC
Others:         // ����˵��
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
        if(act>=0) {    ///�ƽ�
            s.push_back(a);
            s.push_back(act);
            w.pop_back();
            stk.push_back(ST(b,0,0));
            stk.push_back(ST("act",0,0));
            wtk.pop_back();
        } else if (act > ACC) { ///��Լ
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
        } else if (act==ACC) break;///���
        else {///����
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
