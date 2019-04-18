#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <assert.h>
#include <deque>
using namespace std;
typedef unsigned long long u64_t;   //this works on most platform,avoid using the PRId64
typedef long long i64_t;
typedef unsigned int u32_t;
typedef int i32_t;

const int max_len=1000;
const u32_t max_uint32=0xffffffff;

const int up=1;
const int left=2;
const int di=3;
struct lcs_t
{
    int C[max_len+5][max_len+5];
    int B[max_len+5][max_len+5];
    vector<char> result;
    void cal_lcs(char X[],int lx,char Y[],int ly)
    {
        int i,j,k;
        for(i=0;i<=lx;i++)
            C[i][0]=0;
        for(i=0;i<=ly;i++)
            C[0][i]=0;
        for(i=1;i<=lx;i++)
            for(j=1;j<=ly;j++)
            {
                if(X[i-1]==Y[j-1])
                {
                    C[i][j]=C[i-1][j-1]+1;
                    B[i][j]=di;
                }
                else if(C[i-1][j]>= C[i][j-1])
                {
                    C[i][j]=C[i-1][j];
                    B[i][j] = up;
                }
                else
                {
                    C[i][j]=C[i][j-1];
                    B[i][j]=left;
                }
            }
    }
    void print_lcs(char X[],int i,int j)
    {
        if(i==0||j==0) return ;
        switch(B[i][j])
        {
            case di:
                print_lcs(X, i - 1, j - 1);
                result.push_back(X[i - 1]);
                printf("<%c>", X[i - 1]);
                break;
            case up:
                print_lcs(X, i - 1, j);
                break;
            case left:
                print_lcs(X, i, j - 1);
                break;
            default:
                assert(0 == 1);
        }
    }
    vector<char> run(char X[],int lx,char Y[],int ly)
    {
        result.clear();
        cal_lcs(X,lx,Y,ly);
        printf("lcs_len=%d\n",C[lx][ly]);
        print_lcs(X,lx,ly);
        printf("\n");
        return result;
    }

}lcs;
struct lcs_rec_t
{
    vector<char> result;
    int C[max_len+5][max_len+5];
    void init(int lx,int ly)
    {
        int i,j,k;
        for(i=0;i<=lx;i++)
            for(j=0;j<=ly;j++)
            {
                C[i][j]=-1;
            }
    }
    int rec(char X[],int i,char Y[],int j)
    {
        if(C[i][j]!=-1) return C[i][j];
        if(i==0||j==0) return C[i][j]=0;
        if(X[i-1]==Y[j-1])
        {
            C[i][j] = rec(X, i - 1, Y, j - 1)+1;
            //printf("<%c>",X[i-1]);
        }
        else if(rec(X,i-1,Y,j) >= rec(X,i,Y,j-1))
            C[i][j]=rec(X,i-1,Y,j);
        else
            C[i][j]=rec(X,i,Y,j-1);
        return C[i][j];
    }
    void print_lcs(char X[],char Y[],int i,int j)
    {
        if(i==0||j==0) return ;
        if(X[i-1]==Y[j-1])
        {
            print_lcs(X,Y,i-1,j-1);
            result.push_back(X[i-1]);
            printf("<%c>",X[i-1]);
        }
        else if(C[i][j]==C[i-1][j])
            print_lcs(X,Y,i-1,j);
        else if(C[i][j]==C[i][j-1])
            print_lcs(X,Y,i,j-1);
        else
            assert(0==1);
    }
    vector<char> run(char X[],int lx,char Y[],int ly)
    {
        result.clear();
        init(lx,ly);
        rec(X,lx,Y,ly);
        printf("lcs_rec_len=%d\n",C[lx][ly]);
        print_lcs(X,Y,lx,ly);
        printf("\n");
        return result;
    }
}lcs_rec;
void prt(vector<char> &a)
{
    for (auto it=a.begin();it!=a.end();it++)
    {
        printf("<%c>",*it);
    }
    printf("\n");
}
vector<char> random_vec(int len,int l)
{
    vector<char> res;
    for(int i=0;i<len;i++)
    {
        res.push_back('a'+rand()%l);
    }
    return res;
}

int main()
{
    int cnt=0;

    {
        printf("===case %d===\n",cnt++);
        char X[]={'a','a','b','c','b','d','a','b'};
        char Y[]={'b','d','c','a','b','c'};
        int lx=sizeof(X);
        int ly=sizeof(Y);
        auto ret1=lcs.run(X,lx,Y,ly);
        auto ret2=lcs_rec.run(X,lx,Y,ly);
        //printf("%d %d\n",ret1.size(),ret2.size());
        //prt(ret1);
        //prt(ret2);
        assert(ret1==ret2);
    }

    int max_len=20;
    int max_letter_num=10;
    for(int idx=0;idx<1000;idx++)
    {
        printf("===case %d===\n",cnt++);
        int letter=1+rand()%(max_letter_num-1);
        vector<char> x=random_vec(rand()%max_len,letter);
        vector<char> y=random_vec(rand()%max_len,letter);
        int lx=x.size();
        int ly=y.size();
        char *X=&x[0];
        char *Y=&y[0];
        prt(x);
        prt(y);
        auto ret1=lcs.run(X,lx,Y,ly);
        auto ret2=lcs_rec.run(X,lx,Y,ly);
        assert(ret1==ret2);
        assert(ret1.size()==ret2.size());
    }
	return 0;
}
