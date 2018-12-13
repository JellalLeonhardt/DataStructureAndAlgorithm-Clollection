#include <Windows.h>
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;

#define DEBUG 1

void Sift(int array[], int root, int n)
{
    int dest;
    int pos;
    while (root * 2 <= n) { //要保证每一个节点往下都是大顶堆
        if (root * 2 < n) { //不仅有左节点，还有右节点
            pos = array[root * 2] > array[root * 2 + 1] ? root * 2 : root * 2 + 1;
        } else { //只有左节点
            pos = n;
        }
        if (array[root] < array[pos]) {
            dest = array[root];
            array[root] = array[pos];
            array[pos] = dest;
            root = pos; //要保证每一个节点往下都是大顶堆，也可以说是pos所在的大顶堆堆顶改变，所以要重新比较交换
        } else {
            break;
        }
    }
}

void HeapSort(int array[], int n)
{
    int i;
    for (i = n / 2; i >= 1; i--) { //自下向上构建大顶堆 注意，这里直接除以2就ok
        Sift(array, i, n);
    }
#if DEBUG
    printf("\n");
#endif
    for (i = 0; i < n; i++) {
#if DEBUG
        printf("%d ", array[1]);
#endif
        array[0] = array[1];
        array[1] = array[n - i]; //堆顶被拿走了，要最快形成堆，自然是把叶子节点放到堆顶
        array[n - i] = array[0];
        Sift(array, 1, n - i - 1); //因为只有堆顶一个元素变了，自然只需要Sift一次，只是范围要相应变小
    }
#if DEBUG
    printf("\n");
#endif
}

int main()
{
    int array[11], result[11]; //为不破坏堆元素 0位置保存
    int i, j, k;
    srand((unsigned int)time(NULL));
    for (i = 0; i < 20; i++) {
        for (j = 1; j <= 10; j++) {
            array[j] = rand() % 100;
        }
        memcpy(result, array, sizeof(array));
        HeapSort(array, 10); // 1 - 10排序
        sort(result + 1, result + 11);
        for (j = 1; j <= 10; j++) {
            if (array[j] != result[j]) {
                printf("test failed\n");
#if DEBUG
                printf("\n");
                for (k = 0; k < 10; k++) {
                    printf("%d ", array[k]);
                }
                printf("\n");
                for (k = 0; k < 10; k++) {
                    printf("%d ", result[k]);
                }
                printf("\n");
#endif
                break;
            }
        }
    }
    printf("\n");
    system("pause");
    return 0;
}