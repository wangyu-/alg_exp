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
        if(B[i][j]==di)
        {
            print_lcs(X,i-1,j-1);
            printf("<%c>",X[i-1]);
        }
        else if(B[i][j]==up)
            print_lcs(X,i-1,j);
        else if(B[i][j]==left)
            print_lcs(X,i,j-1);
        else
            assert(0==1);
    }
    void run(char X[],int lx,char Y[],int ly)
    {
        cal_lcs(X,lx,Y,ly);
        printf("lcs_len=%d\n",C[lx][ly]);
        print_lcs(X,lx,ly);
        printf("\n");

    }

}lcs;
struct lcs_rec_t
{
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
    void print_lcs(char X[],int i,int j)
    {
        if(i==0||j==0) return ;
        if(C[i][j]==C[i-1][j-1]+1)
        {
            print_lcs(X,i-1,j-1);
            printf("<%c>",X[i-1]);
        }
        else if(C[i][j]==C[i-1][j])
            print_lcs(X,i-1,j);
        else if(C[i][j]==C[i][j-1])
            print_lcs(X,i,j-1);
        else
            assert(0==1);
    }
    void run(char X[],int lx,char Y[],int ly)
    {
        init(lx,ly);
        rec(X,lx,Y,ly);
        printf("lcs_rec_len=%d\n",C[lx][ly]);
        print_lcs(X,lx,ly);
        printf("\n");

    }
}lcs_rec;
int main()
{
    {
        char X[]={'a','a','b','c','b','d','a','b'};
        char Y[]={'b','d','c','a','b','c'};
        int lx=sizeof(X);
        int ly=sizeof(Y);
        lcs.run(X,lx,Y,ly);
        lcs_rec.run(X,lx,Y,ly);
    }
	return 0;
}
