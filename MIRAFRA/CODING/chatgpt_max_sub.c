#include <stdio.h>
int maxSubarraySum(int arr[], int n, int c) {
    int maxSum = 0, windowSum = 0;

    // Compute the sum of the first window
    for (int i = 0; i < c; i++) {
        windowSum += arr[i];
    }
    maxSum = windowSum;

    // Slide the window across the array
    for (int i = c; i < n; i++) {
        windowSum += arr[i] - arr[i - c];
        if (windowSum > maxSum) {
            maxSum = windowSum;
        }
    }
    return maxSum;
}

int main() {
    int arr[] = {9,6,5,2,1};
    int n = sizeof(arr) / sizeof(arr[0]);
    int c = 4;
    printf("Max subarray sum of length %d: %d\n", c, maxSubarraySum(arr, n, c));
    return 0;
}

