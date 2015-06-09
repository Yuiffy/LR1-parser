#include<iostream>
#include<cstdio>
#include<map>
#include<cstring>
using namespace std;
static const int ACC=-10000;
static const int MAX=570;
static const int KO=-20000;

void Initag(int action[][MAX],int gto[][MAX]) {
    int i,j;
    for(i=0; i<MAX; i++)
        for(j=0; j<MAX; j++) {
            action[i][j]=KO;
            gto[i][j]=KO;
        }
}

void MakeShift(char c[],int action[][MAX],int& row,int& col,int column[MAX]) {
    int i=6;
    while(c[i]<'0'||c[i]>'9') {
        if(c[i]=='\0') {
            printf("MakeShiftError\n");
            return;
        }
        i++;
    }
    int t=0;
    while(c[i]>='0' && c[i]<='9') {
        t=t*10+c[i]-'0';
        i++;
    }
    printf("action[%d][%d]=%d (col=%d)",row,column[col],t,col);
    action[row][column[col]]=t;
    puts(";");
}

static char c2[100][MAX];

void MakeReduce(char c[],int action[][MAX],int &row,int &col,int &rcnt,int A[], string B[], int beta[],map<string,int>&stoi, map<string,int> &rtable,int column[]) {
    string red=c;
    if(rtable.find(red)!=rtable.end()) {
        action[row][column[col]]=-rtable[red]-1;///之前这里column[col]写成了col，怒错
        printf("action[%d][%d]=%d , Already have :r%d\t",row,column[col],-rtable[red]-1,rtable[red]);
        cout<<red<<endl;
        return;
    }
    int ccnt,p;
    int left;
    string right;
    int cr=0;
    p=0;
    ccnt=0;
    int i,len;
    len=strlen(c);
    for(i=7; i<len; i++) {
        if(c[i]!=' ') {
            c2[ccnt][p++]=c[i];
        } else {
            if(p==0)continue;
            c2[ccnt][p]='\0';
//            printf("!%s,%d,%d!",c2[ccnt],i,len);
            ccnt++;
            p=0;
        }
    }
    if(p!=0) {
        c2[ccnt][p]='\0';
        if(!(c2[ccnt][0]==-50 && c2[ccnt][1]==-75 && p==2)) {
            //printf("!%d,%d,%d!",c2[ccnt][0],c2[ccnt][1],p);
            ccnt++;
        }
    }
    rtable[red]=rcnt;
    action[row][column[col]]=-rcnt-1;
    A[rcnt]=stoi[c2[0]];
    B[rcnt]="";
    for(i=2; i<ccnt; i++) {
        B[rcnt]+=c2[i];
        if(i!=ccnt-1)B[rcnt]+=" ";
    }
    beta[rcnt]=ccnt-2;
    rcnt++;
//    printf("reduce: ");
//    for(i=0; i<ccnt;i++){
//        printf("%s ",c2[i]);
//    }
//    puts("");
    printf("action[%d][%d]=%d (col=%d)\t",row,column[col],action[row][column[col]],col);
    cout<<"reduce: "<<A[rcnt-1]<<" -> "<<B[rcnt-1];
    printf(" (beta[%d]=%d r%d)(",rcnt-1,beta[rcnt-1],rcnt-1);
    cout<<red<<")\n";
    return;
}

void MakeGto(char c[],int gto[][MAX],int& row,int &col, int column[MAX]) {
    int i=0;
    while(c[i]<'0'||c[i]>'9') {
        if(c[i]=='\0') {
            printf("MakeGtoError(%s)\n",c);
            return;
        }
        i++;
    }
    int t=0;
    while(c[i]>='0' && c[i]<='9') {
        t=t*10+c[i]-'0';
        i++;
    }
    printf("gto[%d][%d]=%d (col=%d)",row,column[col],t,col);
    gto[row][column[col]]=t;
    puts(";");
}

void GetTable(const string& fname,int action[][MAX],int gto[][MAX], int A[],string B[], int beta[], int& jing,    string itos[MAX]) {
    if(freopen(fname.data(),"r",stdin) == NULL) {
        printf("Can\'t find the source code.\n");
        return;
    }
    printf("Log is in readTableLog.txt\n");
    freopen("readTableLog.txt","w",stdout);
    int cnt,cnt2;
    string s;
    int x;
    map<string,int>stoi;
    Initag(action,gto);
    scanf("%d%d",&cnt,&cnt2);
    for(int i=0; i<cnt; i++) {
        cin>>s>>x;
        stoi[s]=x;
        itos[x]=s;
        cout<<"itos["<<x<<"]="<<s<<endl;
    }
    jing=stoi["$"];
    int column[MAX];
    char c;
    char cs[11111],cn=0,c1[11111];
    int col=0, row=0;
    getchar();
    gets(cs);
    int p=0;
    int len=strlen(cs);
    for(int i=0; i<len; i++) {
        if(cs[i]!='\t') {
            c1[p++]=cs[i];
        } else {
            c1[p]='\0';
            string t=c1;
            if (stoi.find(t)==stoi.end()) {
                itos[cnt2]=t;
                stoi[t]=cnt2++;
            }
            column[col++]=stoi[t];
            printf("column[%d]=%d\t",col-1,column[col-1]);
            cout<<t<<endl;
            p=0;
        }
    }
    ///之前漏了下面这个，导致最后一列表头没读进去
    if(p!=0) {
        c1[p]='\0';
        string t=c1;
        if (stoi.find(t)==stoi.end()) {
            itos[cnt2]=t;
            stoi[t]=cnt2++;
        }
        column[col++]=stoi[t];
        printf("column[%d]=%d\t",col-1,column[col-1]);
        cout<<t<<endl;
        p=0;
    }
    col=-1;
    row=0;
    int rcnt=0;
    map<string,int> rtable;
    while(scanf("%d",&row)!=EOF) {
        gets(cs);
        len=strlen(cs);
        p=0;
        col=-1;
        for(int i=0; i<len; i++) {
            if(cs[i]=='\t') {
                c1[p]='\0';
                if((p==1 && c1[0]!=' ') || p>1) {
                    if(c1[0]=='s') {
                        MakeShift(c1,action,row,col,column);
                    } else if(c1[0]=='r') {
                        MakeReduce(c1,action,row,col,rcnt,A,B,beta,stoi,rtable,column);
                    } else if(c1[0]=='a') {
                        action[row][column[col]]=ACC;
                    } else {
                        MakeGto(c1,gto,row,col,column);
                    }
                }
                p=0;
                col++;
            } else {
                c1[p++]=cs[i];
            }
        }
        ///之前也少了下面那个，导致最后一列根本不读！
        if(p>0) {
            c1[p]='\0';
            if((p==1 && c1[0]!=' ') || p>1) {
                if(c1[0]=='s') {
                    MakeShift(c1,action,row,col,column);
                } else if(c1[0]=='r') {
                    MakeReduce(c1,action,row,col,rcnt,A,B,beta,stoi,rtable,column);
                } else if(c1[0]=='a') {
                    action[row][column[col]]=ACC;
                } else {
                    MakeGto(c1,gto,row,col,column);
                }
            }
            p=0;
            col++;
        }
    }
    freopen( "CON", "w", stdout );
    printf("readTable Over\n");
    return;
}

//int main() {
//    int A[MAX];
//    string B[MAX];
//    int beta[MAX];
//    int jing;
//    int action[MAX][MAX];
//    int gto[MAX][MAX];
//    GetTable("biao.txt",action,gto,A,B,beta,jing);
//    return 0;
//}
