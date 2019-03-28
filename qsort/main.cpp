#include <stdio.h>
#include <random>
#include <string.h>
typedef unsigned long long u64_t;   //this works on most platform,avoid using the PRId64
typedef long long i64_t;

typedef unsigned int u32_t;
typedef int i32_t;

typedef unsigned short u16_t;
typedef short i16_t;

typedef unsigned char u8_t;
typedef signed char i8_t;

struct my_random_t
{
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<u64_t> dis64;
    std::uniform_int_distribution<u32_t> dis32;

    std::uniform_int_distribution<unsigned char> dis8;

    my_random_t(u64_t seed)
    {
        //std::mt19937 gen_tmp(rd());  //random device is broken on mingw
        std::mt19937 gen_tmp(seed);
        gen=gen_tmp;
        gen.discard(700000);  //magic
    }
    u64_t gen64()
    {
        return dis64(gen);
    }
    u32_t gen32()
    {
        return dis32(gen);
    }

    unsigned char gen8()
    {
        return dis8(gen);
    }
    u32_t random_between(u32_t min,u32_t max)
    {
        return gen32()%(max-min)+min;
    }
}my_random(123456);

inline void swap(int &a,int &b)
{
    int tmp=a;
    a=b;
    b=tmp;
}

int partition(int A[],int p,int r)
{
    int x = A[r];
    int i = p - 1;
    for (int j = p; j <= r - 1; j++)
    {
        if(A[j]<=x)
        {
            i++;
            swap(A[i],A[j]);
        }
    }
    swap(A[i+1],A[r]);
    return i+1;
}
void quicksort_normal(int A[],int p,int r)
{
    if(p<r)
    {
        int q=partition(A,p,r);
        quicksort_normal(A,p,q-1);
        quicksort_normal(A,q+1,r);
    }
}
int random_partition(int A[],int p,int r)
{
    int i=my_random.random_between(p,r+1);
    swap(A[r],A[i]);
    return partition(A,p,r);
}
void quicksort_random(int A[],int p,int r)
{
    if(p<r)
    {
        int q=random_partition(A,p,r);
        quicksort_random(A,p,q-1);
        quicksort_random(A,q+1,r);
    }
}
int prt(int A[],int len)
{
    for(int i=0;i<len;i++)
    {
        printf("<%d>",A[i]);
    }
    printf("\n");
}
u64_t get_current_time()
{
    timespec tmp_time;
    clock_gettime(CLOCK_MONOTONIC, &tmp_time);
    return ((u64_t)tmp_time.tv_sec)*1000llu+((u64_t)tmp_time.tv_nsec)/(1000*1000llu);
}
int A[20][1000*1000];
int B[20][1000*1000];
int random_fill(int A[],int len)
{
    for(int i=0;i<len;i++)
    {
        A[i] = my_random.gen32();
    }
}
int bench_normal(int A[],int p,int r)
{
    u64_t begin=get_current_time();
    quicksort_normal(A,p,r);
    u64_t end=get_current_time();
    printf("quicksort_normal: %d elements, %lld ms\n",r-p+1,end-begin);
}
int bench_random(int A[],int p,int r)
{
    u64_t begin=get_current_time();
    quicksort_random(A,p,r);
    u64_t end=get_current_time();
    printf("quicksort_random: %d elements, %lld ms\n",r-p+1,end-begin);
}
int random_bench()
{
    for(int i=1;i<=10;i++)
    {
        int len=i*10000;
        random_fill(A[i],len);
    }
    memcpy(B,A,sizeof(A));
    for(int i=1;i<=10;i++)
    {
        bench_normal(A[i],0,i*10000-1);
    }
    for(int i=1;i<=10;i++)
    {
        bench_random(B[i],0,i*10000-1);
    }
}
int order_bench()
{
    for(int i=1;i<=10;i++)
    {
        int len=i*10000;
        for(int j=0;j<len;j++)
        {
            A[i][j]=j;
        }
    }
    memcpy(B,A,sizeof(A));
    for(int i=1;i<=10;i++)
    {
        bench_normal(A[i],0,i*10000-1);
    }
    for(int i=1;i<=10;i++)
    {
        bench_random(B[i],0,i*10000-1);
    }
}
int main()
{
    random_bench();
    printf("=======\n");
    order_bench();
    //int len=sizeof(A)/sizeof(A[0]);
    //prt(A,len);
    //quicksort_random(A,0,9);
    //prt(A,len);
    //printf("%lld",get_current_time());
    return 0;
}