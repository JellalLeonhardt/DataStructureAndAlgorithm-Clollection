#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <map>
#include <set>
#include <vector>
#include <queue>
#include <Windows.h>

#define UNICODE

#define inf 1000000000

using namespace std;

int n, m;

struct seg
{
	int l, r, f, v;//l r是当前段落为l-r的区间，f是普通标记，v是懒标记 这里的叶子节点只有存在和不存在两种状态，故而v只有0和1两个值
} t[305]; //静态建树
//叶子节点为一个点

void build(int k, int l, int r) //建树 k:当前节点下标    线段左为l，线段右为r
{
	t[k].l = l;
	t[k].r = r; //线段左端，线段右端
	if (l == r)
		return;					   //线段长度为零，结束
	int mid = (l + r) >> 1;		   //取线段中点
	build(k << 1, l, mid);		   //k<<1:下标为k节点的左儿子下标，线段左为l，线段右为mid                      k<<1==k*2
	build(k << 1 | 1, mid + 1, r); //k<<1|1:下标为k节点的右儿子下标，线段左为mid+1，线段右为r                  k<<1|1==k*2+1
}

int mn(int k)
{
	if(t[k].v)
		return t[k].l;
	if (t[k].l == t[k].r)
		return -1;
	if (t[k << 1].f) //左边段落有点存在
		return mn(k << 1);
	else
		return mn(k << 1 | 1);
}

int mx(int k)
{
	if (t[k].v)
		return t[k].r;
	if (t[k].l == t[k].r)
		return -1;
	if (t[k << 1 | 1].f) //右边段落有点存在
		return mx(k << 1 | 1);
	else
		return mx(k << 1);
}

//基本操作我都写了点和段两个版本，带_seg后缀的便是对段操作

void insert(int k, int val) //k是搜索开始的位置 这个val
{
	int l = t[k].l, r = t[k].r;
	if (l == r)
	{
		t[k].v = 1; //v就是flag 为0表示有此点
		t[k].f = 1;
		return;
	}
	int mid = (l + r) >> 1; //看是在当前这个节点的左边还是右边
	if (val <= mid)
		insert(k << 1, val);
	else
		insert(k << 1 | 1, val);
	t[k].f += 1; //t[k].f = t[k << 1].f + t[k << 1 | 1].f;
	if(t[k].f == t[k].r - t[k].l + 1) //该段全被标记了
		t[k].v = 1;
}

void insert_seg(int k, int start, int end) //k是搜索开始的位置 插入这个段
{
	int l = t[k].l, r = t[k].r;
	if(l == r){
		t[k].v = 1;
		t[k].f = 1;
		return;
	}
	int mid = (l + r) >> 1;
	if(l == start && r == end){
		t[k].v = 1;
		t[k].f = r - l + 1;	//这个地方没有再去更新子节点的普通标记，是因为无论查询还是统计的时候不会再用到
		return;
	}
	//下面几种情况都会导致子节点的普通标记更新，故单独提出来
	if(start <= mid && mid + 1 <= end){
		insert_seg(k << 1, start, mid);
		insert_seg(k << 1 | 1, mid + 1, end);
	}
	else if(mid + 1 <= start){
		insert_seg(k << 1 | 1, start, end);
	}
	else if(end <= mid){
		insert_seg(k << 1, start, end);
	}
	else{
		printf("something miss\n");
	}
	t[k].f += end - start + 1; //t[k].f = t[k << 1].f + t[k << 1 | 1].f;
	if(t[k].f == t[k].r - t[k].l + 1) //该段全被标记了
		t[k].v = 1;
}

int find(int k, int val)
{
	if(t[k].v){
		return 1;
	}
	int l = t[k].l, r = t[k].r;
	if (l == r)
	{
		return 0;
	}
	int mid = (l + r) >> 1;
	if (val <= mid)
		return find(k << 1, val);
	else
		return find(k << 1 | 1, val);
}

int find_seg(int k, int start, int end)
{
	if(t[k].v){
		return 1;
	}
	int l = t[k].l, r = t[k].r;
	if(l == r){	//当前节点没被标记且已经不能再分了，自然返回0
		return 0;
	}
	int mid = (l + r) >> 1;
	if(l == start && r == end){
		return 0;
	}
	else if(start <= mid && mid + 1 <= end){
		return find_seg(k << 1, start, mid) && find_seg(k << 1 | 1, mid + 1, end);
	}
	else if(mid + 1 <= start){
		return find_seg(k << 1 | 1, start, end);
	}
	else if(end <= mid){
		return find_seg(k << 1, start, end);
	}
	else{
		printf("something miss\n");
	}
	return 0;
}

void del(int k, int val)
{
	int l = t[k].l, r = t[k].r;
	if (l == r)
	{
		t[k].v = 0;
		t[k].f = 0;
		return;
	}
	int mid = (l + r) >> 1;
	//之前我们insert_seg的时候，对于一个区域段，是没有去更新子节点的f v标记的，这对查询来说不影响，但是删除的时候就要去更新了
	//理由很简单，查询如果大段的懒标记为1自然认为子段的v标记也为1，就没必要看下去了
	//而删除的时候，比如一个大段有A B两个子段，我们要删除A段，所以我们会去掉大段和A段的v标记，但这样B段也因为没有v标记就无意中被干掉了
	//也没必要全部更新，更新下一层即可
	//这一步操作就是很多网上例子给出的down函数
	if(t[k].v){
		t[k << 1].f = mid - l + 1;
		t[k << 1 | 1].f = r - mid;
		t[k << 1].v = 1;
		t[k << 1 | 1].v = 1;
	}
	if (val <= mid) {	//涉及该节点的所有段落的懒标记全部去除
		del(k << 1, val);
		t[k << 1].v = 0;
	}
	else {
		del(k << 1 | 1, val);
		t[k << 1 | 1].v = 0;
	}
	t[k].f -= 1; //t[k].f = t[k << 1].f + t[k << 1 | 1].f;
}

void del_seg(int k, int start, int end)
{
	int l = t[k].l, r = t[k].r;
	if(l == r){
		t[k].v = 0;
		t[k].f = 0;
		return;
	}
	int mid = (l + r) >> 1;
	if(l == start && r == end){
		t[k].v = 0;
		t[k].f = 0;	//这个地方没有再去更新子节点的普通标记，是因为无论查询还是统计的时候不会再用到
		return;
	}
	//down函数
	if(t[k].v){
		t[k << 1].f = mid - l + 1;
		t[k << 1 | 1].f = r - mid;
		t[k << 1].v = 1;
		t[k << 1 | 1].v = 1;
	}
	//下面几种情况都会导致子节点的普通标记更新，故单独提出来
	if(start <= mid && mid + 1 <= end){
		del_seg(k << 1, start, mid);
		del_seg(k << 1 | 1, mid + 1, end);
		t[k << 1].v = 0;
		t[k << 1 | 1].v = 0;
	}
	else if(mid + 1 <= start){
		del_seg(k << 1 | 1, start, end);
		t[k << 1 | 1].v = 0;
	}
	else if(end <= mid){
		del_seg(k << 1, start, end);
		t[k << 1].v = 0;
	}
	else{
		printf("something miss\n");
	}
	t[k].f -= end - start + 1; //t[k].f = t[k << 1].f + t[k << 1 | 1].f;
}

int findpr(int k, int val)
{
	if (val < 0)
		return -1;
	if (!t[k].v)
		return -1;
	int l = t[k].l, r = t[k].r;
	if (l == r)
		return l;
	int mid = (l + r) >> 1;
	if (val <= mid)
		return findpr(k << 1, val);
	else
	{
		int t = findpr(k << 1 | 1, val);
		if (t == -1)
			return mx(k << 1);
		else
			return t;
	}
}

int findsu(int k, int val)
{
	if (!t[k].v)
		return -1;
	int l = t[k].l, r = t[k].r;
	if (l == r)
		return l;
	int mid = (l + r) >> 1;
	if (val > mid)
		return findsu(k << 1 | 1, val);
	else
	{
		int t = findsu(k << 1, val);
		if (t == -1)
			return mn(k << 1 | 1);
		else
			return t;
	}
}

/*
1 x 若x不存在,插入x
2 x 若x存在,删除x
3 输出当前最小值,若不存在输出-1
4 输出当前最大值,若不存在输出-1
5 x 输出x的前驱,若不存在输出-1
6 x 输出x的后继,若不存在输出-1
7 x 若x存在,输出1,否则输出-1
*/

int main()
{
	wcin>> n;
	build(1, 0, n); //当前节点所在下标为1，为0就没法乘2了，实际上就是棵静态完全二叉树
	int opt, x, y;
	while(1)
	{
		cout<< "input code:";
		wcin>> opt;
		switch (opt)
		{
		case 1:
			cout<< "input target:";
			wcin>> x;
			if (find(1, x) == -1)
				insert(1, x);
			break;
		case 2:
			cout<< "input target:";
			wcin>> x;
			if (find(1, x) == 1)
				del(1, x);
			break;
		case 3:
			cout<< mn(1) <<endl;
			break;
		case 4:
			cout<< mx(1) <<endl;
			break;
		case 5:
			cout<< "input target:";
			wcin>> x;
			cout<< findpr(1, x - 1) <<endl;
			break;
		case 6:
			cout<< "input target:";
			wcin>> x;
			cout<< findsu(1, x + 1) <<endl;
			break;
		case 7:
			cout<< "input target:";
			wcin>> x;
			cout<< find(1, x) <<endl;
			break;
		case 8:
			cout<< "input target segment:";
			wcin>> x >> y;
			cout<< find_seg(1, x, y) <<endl;
			break;
		case 9:
			cout<< "input target segment:";
			wcin>> x >> y;
			insert_seg(1, x, y);
			break;
		case 10:
			cout<< "input target segment:";
			wcin>> x >> y;
			del_seg(1, x, y);
			break;
		default:
			cout<< "invalid input" <<endl;
		}
	}
	return 0;
}