#include <Windows.h>
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;

#define DEBUG 0

int GetRandomNum(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

int partition(int array[], int start, int end, int position) //快排写的时候最值得注意的地方：要知道什么时候数组中哪个位置是空出来的
{
    int i, par = array[position], low = start, high = end;
#if DEBUG
    printf("partition:%d\n", position);
    for (i = 0; i < 10; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
#endif
    while (array[low] < par && low < high)
        low++;
    array[position] = array[low];
    while (high > low) {
        while (array[high] >= par && high > low)
            high--;
        array[low] = array[high];
        while (array[low] < par && low < high)
            low++;
        array[high] = array[low];
#if DEBUG
        for (i = 0; i < 10; i++) {
            printf("%d ", array[i]);
        }
        printf("\n");
#endif
    }
    array[low] = par;
#if DEBUG
    for (i = 0; i < 10; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
#endif
    return low;
}

void QuickSort(int array[], int start, int end)
{
    int position;
    if (start < end) {
        position = GetRandomNum(start, end);
        position = partition(array, start, end, position);
        QuickSort(array, start, position - 1);
        QuickSort(array, position + 1, end);
    }
}

int main()
{
    int array[10], result[10];
    int i, j, k;
    srand((unsigned int)time(NULL));
    for (i = 0; i < 20; i++) {
        for (j = 0; j < 10; j++) {
            array[j] = rand() % 100;
        }
        memcpy(result, array, sizeof(array));
        QuickSort(array, 0, 9);
        sort(result, result + 10);
        for (j = 0; j < 10; j++) {
            if (array[j] != result[j]) {
                printf("test failed\n");
#if DEBUG
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