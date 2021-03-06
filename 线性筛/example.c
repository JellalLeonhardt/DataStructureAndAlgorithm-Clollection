#define MAXN 10000
int vis[MAXN]; //标记
int prime[MAXN];
void Prime(int n)
{
    int cnt = 0;
    for (int i = 2; i <= n; i++) {
        if (!vis[i]) //如果访问到它时，它没被标记，说明是质数，否则是合数
            prime[cnt++] = i;
        for (int j = 0; j < cnt && i * prime[j] <= n; j++) {
            vis[i * prime[j]] = i; //把目前已知的质数和当前数字相乘，得到的一定是和数
            if (i % prime[j] == 0) //这里的标记用的是最小质因子来标记，如果说i本身是和数的话，那么对于i的倍数来说，最小质因子一定是当前质数，则另外个乘积一定在i的后面，所以无需看下去了
                break; //举个例子，比如i为6，则prime表为2 3 5，首先我们会把vis[12]标记出来，那么为什么vis[18]不需要标记了呢，因为6能被2整除，那么6的所有倍数的最小质因子都是2
        } //则除了第一个倍数12以外，其他倍数一定可以分解为 2 * i'，而这个i'一定是大于i的，故而后面会出现，现在不用标记，否则就重复标记了
    }
}