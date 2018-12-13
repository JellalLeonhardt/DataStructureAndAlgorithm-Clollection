#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

using namespace std;

#define MAXCONTENT 101

void merge(int array[], int start, int mid, int end) //将两个有序数组（start 到 mid - 1 和 mid 到 end）合并为一个有序数组
{
    int asist[MAXCONTENT];
    int i = start, j = mid, k = 0;
    while (i < mid && j <= end) {
        if (array[i] < array[j]) {
            asist[k++] = array[i++];
        } else {
            asist[k++] = array[j++];
        }
    }
    if (i == mid) {
        while (j <= end) {
            asist[k++] = array[j++];
        }
    } else {
        while (i < mid) {
            asist[k++] = array[i++];
        }
    }
    for (i = 0; i < k; i++) {
        array[start + i] = asist[i];
    }
}

void MergeSort(int array[], int _start, int _end)
{
    int i, j, dest, end;
    for (i = _start; i < _end; i += 2) { //使最小局部（两个一组）有序
        if (array[i] > array[i + 1]) {
            dest = array[i];
            array[i] = array[i + 1];
            array[i + 1] = dest;
        }
    }
    for (i = 2; i < _end; i *= 2) { //从局部有序到全局有序，核心就是利用辅组数组，将两个有序数组合并为一个有序数组
        j = 1; //从1开始
        while ((j + i) <= _end) {
            if (j + 2 * i - 1 <= _end) { //如果后一个数组还不是末尾，或者后一个数组的最后一个元素刚好是数组。
                end = j + 2 * i - 1;
            } else {
                end = _end;
            }
            merge(array, j, j + i, end);
            j += 2 * i;
        }
    }
}

int main()
{
    int array[MAXCONTENT], result[MAXCONTENT];
    int i, j, k;
    srand((unsigned int)time(NULL));
    for (i = 0; i < 20; i++) {
        for (j = 1; j <= 10; j++) { //0乘以2还是0，干脆不用
            array[j] = rand() % 100;
        }
        memcpy(result, array, sizeof(array));
        MergeSort(array, 1, 10); // 1 - 10排序
        sort(result + 1, result + 11);
        for (j = 1; j <= MAXCONTENT - 1; j++) {
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
        if (j == MAXCONTENT) {
            printf("test success\n");
        }
    }
    printf("\n");
    system("pause");
    return 0;
}