#include<stdio.h>
#include<string.h>
#include<iostream>
#include<cstring>
#include<map>
#include<vector>

using namespace std;

static const int TOKEN_MAX = 1024;
static const int PROG_MAX = 8192;
static char prog[PROG_MAX];
static char token[TOKEN_MAX];
static int p,len,m=0,row,sum=0;
static double dsum=0.0;
static map<string,int> tab;

void Init() {
    tab.clear();
    tab["auto"]=1;
    tab["double"]=2;
    tab["int"]=3;
    tab["struct"]=4;
    tab["break"]=5;
    tab["else"]=6;
    tab["long"]=7;
    tab["switch"]=8;
    tab["case"]=9;
    tab["enum"]=10;
    tab["registe"]=11;
    tab["typedef"]=12;
    tab["char"]=13;
    tab["extern"]=14;
    tab["return"]=15;
    tab["union"]=16;
    tab["const"]=17;
    tab["float"]=18;
    tab["short"]=19;
    tab["unsigned"]=20;
    tab["continue"]=21;
    tab["for"]=22;
    tab["signed"]=23;
    tab["void"]=24;
    tab["default"]=25;
    tab["goto"]=26;
    tab["sizeof"]=27;
    tab["volatile"]=28;
    tab["do"]=29;
    tab["if"]=30;
    tab["while"]=31;
    tab["static"]=32;
}


int scaner() {
    /*
        共分为三大块，分别是标示符、数字、符号，对应下面的 if   else if  和 else


    */
    int syn = 0;
    char ch;
    memset(token,0,sizeof(token));
    ch=prog[p++];
    while(ch==' '|| ch=='\t' || ch=='\r') {
        ch=prog[p];
        p++;
    }
    if((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')) { //可能是标示符或者变量名
        m=0;
        while((ch>='0'&&ch<='9')||(ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')) {
            token[m++]=ch;
            ch=prog[p++];
        }
        token[m++]='\0';
        p--;
        string s = token;
        syn = tab[s];
        if(syn == 0)syn=40;
    } else if((ch>='0'&&ch<='9') || (ch=='.')) { //数字
        {
            sum=0;
            while((ch>='0'&&ch<='9')) {
                sum=sum*10+ch-'0';
                ch=prog[p++];
            }
        }
        double multi=0.1;
        if(ch=='.') {
            ch=prog[p++];
            dsum=(double)sum;
            while((ch>='0'&&ch<='9')) {
                dsum += multi * (ch-'0');
                multi *= 0.1;
                ch=prog[p++];
            }
            p--;
            syn=83;
        } else {
            p--;
            syn=41;
        }
    } else switch(ch) { //其他字符
        case'+':
            m=0;
            token[m++]=ch;
            ch=prog[p++];
            if(ch=='+') {
                syn=47;
                token[m++]=ch;
            } else if(ch=='=') {
                syn=65;
                token[m++]=ch;
            } else {
                syn=42;
                p--;
            }
            break;
        case'-':
            m=0;
            token[m++]=ch;
            ch=prog[p++];
            if(ch=='-') {
                syn=48;
                token[m++]=ch;
            } else if(ch=='=') {
                syn=66;
                token[m++]=ch;
            } else if(ch=='>') {
                syn=89;
                token[m++]=ch;
            } else {
                syn=43;
                p--;
            }
            break;
        case'*':
            m=0;
            token[m++]=ch;
            ch=prog[p++];
            if(ch=='=') {
                syn=67;
                token[m++]=ch;
            } else {
                syn=44;
                p--;
            }
            break;
        case'/':
            m=0;
            token[m++]=ch;
            ch=prog[p++];
            if(ch=='=') {
                syn=68;
                token[m++]=ch;
            } else {
                syn=45;
                p--;
            }
            break;
        case'%':
            m=0;
            token[m++]=ch;
            ch=prog[p++];
            if(ch=='=') {
                syn=69;
                token[m++]=ch;
            } else {
                syn=46;
                p--;
            }
            break;
        case'<':
            m=0;
            token[m++]=ch;
            ch=prog[p++];
            if(ch=='<') {
                token[m++]=ch;
                ch=prog[p++];
                if(ch=='=') {
                    syn=74;
                    token[m++]=ch;
                } else {
                    syn=62;
                    p--;
                }
            } else if(ch=='=') {
                syn=52;
                token[m++]=ch;
            } else {
                syn=50;
                p--;
            }
            break;
        case'>':
            m=0;
            token[m++]=ch;
            ch=prog[p++];
            if(ch=='>') {
                token[m++]=ch;
                ch=prog[p++];
                if(ch=='=') {
                    syn=73;
                    token[m++]=ch;
                } else {
                    syn=63;
                    p--;
                }
            } else if(ch=='=') {
                syn=51;
                token[m++]=ch;
            } else {
                syn=49;
                p--;
            }
            break;
        case'!':
            m=0;
            token[m++]=ch;
            ch=prog[p++];
            if(ch=='=') {
                syn=53;
                token[m++]=ch;
            }
            break;
        case'=':
            m=0;
            token[m++]=ch;
            ch=prog[p++];
            if(ch=='=') {
                syn=54;
                token[m++]=ch;
            } else {
                syn=64;
                p--;
            }
            break;
        case'&':
            m=0;
            token[m++]=ch;
            ch=prog[p++];
            if(ch=='&') {
                syn=55;
                token[m++]=ch;
            } else if(ch=='=') {
                syn=70;
                token[m++]=ch;
            } else {
                syn=58;
                p--;
            }
            break;
        case'|':
            m=0;
            token[m++]=ch;
            ch=prog[p++];
            if(ch=='|') {
                syn=56;
                token[m++]=ch;
            } else if(ch=='=') {
                syn=71;
                token[m++]=ch;
            } else {
                syn=59;
                p--;
            }
            break;
        case'~':
            syn=60;
            token[0]=ch;
            break;
        case'^':
            m=0;
            token[m++]=ch;
            ch=prog[p++];
            if(ch=='=') {
                syn=72;
                token[m++]=ch;
            } else {
                syn=61;
                p--;
            }
            break;
        case',':
            syn=75;
            token[0]=ch;
            break;
        case';':
            syn=76;
            token[0]=ch;
            break;
        case'(':
            syn=77;
            token[0]=ch;
            break;
        case')':
            syn=78;
            token[0]=ch;
            break;
        case'[':
            syn=79;
            token[0]=ch;
            break;
        case']':
            syn=80;
            token[0]=ch;
            break;
        case'{':
            syn=81;
            token[0]=ch;
            break;
        case'}':
            syn=82;
            token[0]=ch;
            break;
        case'.':
            syn=84;
            token[0]=ch;
            break;
        case'?':
            syn=85;
            token[0]=ch;
            break;
        case':':
            syn=86;
            token[0]=ch;
            break;
        case'\"':
            syn=87;
            m=0;
            token[m++]=ch;
            ch=prog[p++];
            while(ch!='"') {
                token[m++]=ch;
                ch=prog[p++];
                if(ch=='"'&& prog[p-2]=='\\') {
                    token[m++]=ch;
                    ch=prog[p++];
                }
                if(p>=len) {
                    syn=-1;
                    break;
                }
            }
            if(syn!=-1)token[m++]=ch;
            break;
        case'\'':
            syn=87;
            m=0;
            token[m++]=ch;
            ch=prog[p++];
            while(ch!='\'') {
                token[m++]=ch;
                ch=prog[p++];
                if(ch=='\''&& prog[p-2]=='\\') {
                    token[m++]=ch;
                    ch=prog[p++];
                }
                if(p>=len) {
                    syn=-1;
                    break;
                }
            }
            if(syn!=-1)token[m++]=ch;
            break;
        case'\n':
            syn=-2;
            break;
        default:
            syn=-1;
            token[0]=ch;
            break;
        }
    return syn;
}

string itos(int x){
    string re="";
    if(x==0)return "0";
    while(x){
        re+='0'+(x%10);
        x/=10;
    }
    for(int x=0;x<re.length()/2;x++){
        re[x]=re[re.length()-1-x];
    }
    return re;
}

int stoi(string s){
    int x=0;
    int len=s.length();
    int cf=1;
    for(int i=len-1; i>=0; i--){
        x+=(s[i]-'0')*cf;
        cf*=10;
    }
    return x;
}

/*************************************************
Function:       cf
Description:    词法分析
Calls:          Init,scaner
Input:     输入需要词法分析的源文件文件名
Output:         输出vector，存有词法分析得到的各个token
Return:         token串
*************************************************/
vector<pair<int,string> > cf(string filename) {
    vector<pair<int,string> >re;
    Init();
    p=0;
    row=1;
    if(freopen(filename.data(),"r",stdin) == NULL) {
        printf("Can\'t find the source code.\n");
        return re;
    }
    char ch;
    while((ch=getchar())!=EOF) {
        prog[p++]=ch;
        //printf("[%d,%c] ",ch,ch);
    }
    len = p;
    p=0;
    int syn;
    do {
        syn = scaner();
        switch(syn) {
        case 41:
            cout<<"( "<<syn<<" , "<<sum<<" )"<<endl;
            re.push_back(make_pair(syn,itos(sum)));
            break;
        case 83:
            cout<<"( "<<syn<<" , "<<dsum<<" )"<<endl;
            re.push_back(make_pair(syn,itos((int)dsum)));
            break;
        case -1:
            printf("Error:Line %d, Found [%d,%c].\n",row,token[0],token[0]);
            break;
        case -2:
            row++;
            break;
        default:
            cout<<"( "<<syn<<" , "<<token<<" )"<<endl;
            re.push_back(make_pair(syn,token));
            break;
        }
    } while (p<len);
    return re;
}
