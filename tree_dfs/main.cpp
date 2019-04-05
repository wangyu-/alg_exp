#include <stdio.h>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <assert.h>
using namespace std;
typedef unsigned long long u64_t;   //this works on most platform,avoid using the PRId64
typedef long long i64_t;
typedef unsigned int u32_t;
typedef int i32_t;

const u32_t max_uint32=0xffffffff;
u32_t highest_value=100000;

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
        return gen64()%(max-min)+min;
    }
}my_random(123456);

struct node_t
{
	u32_t value;
	node_t * left=0;
	node_t * right=0;
};
u32_t lowest= max_uint32;
struct res_t
{
	u32_t left_most;
	u32_t right_most;
};
u32_t min(u32_t a,u32_t b)
{
	return a<b?a:b;
}
res_t dfs(node_t * node)
{
	res_t left_res,right_res,my_res;
	my_res.left_most=my_res.right_most=node->value;
	if(node->left)
	{
		left_res=dfs(node->left);
		my_res.left_most=left_res.left_most;
        lowest=min(lowest,node->value-left_res.right_most);
	}
	if(node->right)
	{
		right_res=dfs(node->right);
		my_res.right_most=right_res.right_most;
        lowest=min(lowest,right_res.left_most-node->value);
	}
	return my_res;
}
u32_t get_lowest(node_t *node)
{
	lowest=max_uint32;
	dfs(node);
	return lowest;
}
void insert_into(node_t * &node,u32_t value)
{
	if(node==0)
	{
		node=new node_t;
		node->value=value;
	}
	else if(value<node->value)
	{
		insert_into(node->left,value);
	}
	else
	{
		insert_into(node->right,value);
	}
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
int run(int n,u32_t *arr)
{
	printf("=======case %d, size:%d =========\n",cnt++,n);
    assert(n>=2);
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
	printf("test_data:");
	for(int i=0;i<n;i++)
	{
	    printf("<%u>",test_data[i]);
		insert_into(node,test_data[i]);
	}
	printf("\n");

	dfs_prt_output.clear();
	dfs_prt(node);

	dfs_prt_output2.clear();
	dfs_prt2(node);
	printf("midOrder:");
	for(int i=0;i<dfs_prt_output.size();i++)
	{
		printf("<%u>", dfs_prt_output[i]);
	}
	printf("\n");
	printf("preOrder:");
	for(int i=0;i<dfs_prt_output2.size();i++)
	{
		printf("<%u>", dfs_prt_output2[i]);
	}
	printf("\n");
	u32_t low1=get_lowest(node);
	printf("output:<%u>\n",low1);
	sort(test_data.begin(),test_data.end());
	assert(test_data==dfs_prt_output);
	u32_t low2=max_uint32;
	printf("sorted_data:");
	for(int i=0;i<test_data.size();i++)
    {
	    printf("<%u>",test_data[i]);
	    if(i!=0)
        {
	        low2=min(low2,test_data[i]-test_data[i-1]);
        }
    }
	printf("\n");
	printf("verify:<%u>",low2);
	printf("\n");
	assert(low1==low2);
}
int main()
{
    u32_t arr[5]={4,2,1,3,6};
    run(5,arr);
    u32_t arr2[2]={0,max_uint32};
    run(2,arr2);
    u32_t arr3[9]={100,50,25,75,1,200,150,250,125};
    run(9,arr3);
    for(int i=0;i<500;i++)
    {
        int n=my_random.random_between(2,101);
        run(n,0);
    }
	return 0;
}
