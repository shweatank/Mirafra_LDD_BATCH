#include <stdio.h>

int main() {
    int a[]={1,0,1,0,1,0};
    int n = sizeof(a)/sizeof(a[0]);
    int i, j = 0;

    // Move all 0s to the beginning
    for (i = 0; i < n; i++) {
        if (a[i] == 0) {
            int temp = a[i];
            a[i] = a[j];
            a[j] = temp;
            j++;
        }
    }

    // Print result
    for (i = 0; i < n; i++)
        printf("%d ", a[i]);

    return 0;
}

