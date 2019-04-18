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

const u32_t max_uint32=0xffffffff;
u32_t highest_value=1000;

u64_t get_current_time()
{
    timespec tmp_time;
    clock_gettime(CLOCK_MONOTONIC, &tmp_time);
    return ((u64_t)tmp_time.tv_sec)*1000000llu+((u64_t)tmp_time.tv_nsec)/(1000llu);
}
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
        return rand()%(max-min)+min;
        //return gen64()%(max-min)+min;
    }
}my_random(123456);

struct node_t
{
	u32_t value;
	node_t * left=0;
	node_t * right=0;
};
u32_t lowest= max_uint32;

u32_t min(u32_t a,u32_t b)
{
	return a<b?a:b;
}
int non_even=0;
void insert_into(node_t * &node,u32_t value)
{
    int left;
    if(!non_even)
    {
        left=my_random.random_between(0,2);
    }
    else
    {
        left=my_random.random_between(0,1000)<50;
    }

	if(node==0)
	{
		node=new node_t;
		node->value=value;
	}
	else if(left)
	{
		insert_into(node->left,value);
	}
	else
	{
		insert_into(node->right,value);
	}
	return ;
}
void insert_into_n(node_t * &node,int n)
{
    if(n==0) return ;
    node=new node_t;
    node->value=n;
    insert_into_n(node->right,n-1);
    return ;
}
vector<u32_t> dfs_prt_output;
void dfs_prt(node_t *node)
{
    if(node==0) return ;
    dfs_prt(node->left);
    dfs_prt_output.push_back(node->value);
    //printf("<%d>",node->value);
    dfs_prt(node->right);
}
vector<u32_t> dfs_prt_output2;
void dfs_prt2(node_t *node)
{
	if(node==0) return ;
	dfs_prt_output2.push_back(node->value);
	dfs_prt2(node->left);
	//printf("<%d>",node->value);
	dfs_prt2(node->right);
}
int cnt=0;
u32_t min_deep=max_uint32;
int dfs(node_t *node)
{
    if(node==0) return 0;
    return 1+min(dfs(node->left),dfs(node->right));
}

int dfs2_(node_t *node,u32_t current_deep)
{
    //return 0;
    if(node==0) {
        if(current_deep<min_deep) min_deep=current_deep;
        return current_deep;
    }
    if(current_deep>=min_deep) return current_deep;
    return min(dfs2_(node->left,current_deep+1),dfs2_(node->right,current_deep+1));
}
int dfs2(node_t *node)
{
    if(node==0) return 0;
    min_deep=max_uint32;
    dfs2_(node,0);
}
struct element_t{
    node_t * node;
    u32_t depth;
};
int bfs(node_t *node)
{
    if(node==0) return 0;
    deque<element_t> q;
    element_t e,tmp;
    tmp.node=node;
    tmp.depth=1;
    q.push_back(tmp);
    while(!q.empty())
    {
        element_t &e= *q.begin();
        if(e.node->left==0) return e.depth;
        else
        {
            tmp.node=e.node->left;
            tmp.depth=e.depth+1;
            q.push_back(tmp);
        }
        if(e.node->right==0) return e.depth;
        else
        {
            tmp.node=e.node->right;
            tmp.depth=e.depth+1;
            q.push_back(tmp);
        }
        q.pop_front();
    }
    assert(0==1);
}
int run(int n,u32_t *arr,int mute=0)
{
	printf("=======case %d, size:%d =========\n",cnt++,n);
    //assert(n>=2);
	vector<u32_t> test_data;
	if(arr==0)
    {
        for (int i = 0; i < n; i++)
        {
            if(highest_value!=max_uint32)
                test_data.push_back(my_random.random_between(0, highest_value+1));
            else
                test_data.push_back(my_random.gen32());
        }
    } else{
	    for(int i=0;i<n;i++)
        {
	        test_data.push_back(arr[i]);
        }

	}
	node_t * node=0;
	if(!mute)printf("test_data:");
	if(non_even!=2)
	for(int i=0;i<n;i++)
	{
        if(!mute)printf("<%u>",test_data[i]);
		insert_into(node,test_data[i]);
	}
	else
        insert_into_n(node,n);
    if(!mute)printf("\n");
    printf("gen data done\n");
	if(!mute)
    {
        dfs_prt_output.clear();
        dfs_prt(node);

        dfs_prt_output2.clear();
        dfs_prt2(node);
        printf("midOrder:");
        for (int i = 0; i < dfs_prt_output.size(); i++)
        {
            printf("<%u>", dfs_prt_output[i]);
        }
        printf("\n");
        printf("preOrder:");
        for (int i = 0; i < dfs_prt_output2.size(); i++)
        {
            printf("<%u>", dfs_prt_output2[i]);
        }
        printf("\n");
    }

	u64_t time1=get_current_time();
    int res=dfs(node);
    u64_t time2=get_current_time();
    printf("dfs  result:<%d>, time:%lld\n",res,time2-time1);
    int res2=dfs2(node);
	assert(res==res2);
    u64_t time3=get_current_time();
    printf("dfs2 result:<%d>, time:%lld\n",res,time3-time2);
    int res3=bfs(node);
    assert(res3==res);
    u64_t time4=get_current_time();
    printf("bfs  result:<%d>, time:%lld\n",res,time4-time3);

}

int main()
{
    srand(123123);
    u32_t arr[5]={4,2,1,3,6};
    run(5,arr);
    u32_t arr2[2]={0,max_uint32};
    run(2,arr2);
    u32_t arr3[9]={100,50,25,75,1,200,150,250,125};
    run(9,arr3);
    for(int i=0;i<500;i++)
    {
        int n=my_random.random_between(0,30);
        run(n,0);
    }
    run(1000,0,1);
    run(10000,0,1);
    run(100000,0,1);
    run(1000000,0,1);
    run(1000000,0,1);
    run(5000000,0,1);
    non_even=1;
    run(100000,0,1);
    non_even=2;
    run(100000,0,1);
	return 0;
}
