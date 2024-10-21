#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_N 100000

typedef struct {
    int sum;
    int min;
    int max;
} Node;

Node tree[4 * MAX_N];
int arr[MAX_N];

void build(int node, int start, int end) {
    if (start == end) {
        tree[node].sum = arr[start];
        tree[node].min = arr[start];
        tree[node].max = arr[start];
        return;
    }
    
    int mid = (start + end) / 2;
    build(2 * node, start, mid);
    build(2 * node + 1, mid + 1, end);
    
    tree[node].sum = tree[2 * node].sum + tree[2 * node + 1].sum;
    tree[node].min = (tree[2 * node].min < tree[2 * node + 1].min) ? tree[2 * node].min : tree[2 * node + 1].min;
    tree[node].max = (tree[2 * node].max > tree[2 * node + 1].max) ? tree[2 * node].max : tree[2 * node + 1].max;
}

int query_sum(int node, int start, int end, int l, int r) {
    if (r < start || end < l) return 0;
    if (l <= start && end <= r) return tree[node].sum;
    
    int mid = (start + end) / 2;
    int left_sum = query_sum(2 * node, start, mid, l, r);
    int right_sum = query_sum(2 * node + 1, mid + 1, end, l, r);
    return left_sum + right_sum;
}

int query_min(int node, int start, int end, int l, int r) {
    if (r < start || end < l) return INT_MAX;
    if (l <= start && end <= r) return tree[node].min;
    
    int mid = (start + end) / 2;
    int left_min = query_min(2 * node, start, mid, l, r);
    int right_min = query_min(2 * node + 1, mid + 1, end, l, r);
    return (left_min < right_min) ? left_min : right_min;
}

int query_max(int node, int start, int end, int l, int r) {
    if (r < start || end < l) return INT_MIN;
    if (l <= start && end <= r) return tree[node].max;
    
    int mid = (start + end) / 2;
    int left_max = query_max(2 * node, start, mid, l, r);
    int right_max = query_max(2 * node + 1, mid + 1, end, l, r);
    return (left_max > right_max) ? left_max : right_max;
}

int main() {
    int n = 6;
    int i;
    int input[] = {1, 3, 5, 7, 9, 11};
    for (i = 0; i < n; i++) {
        arr[i] = input[i];
    }
    
    build(1, 0, n - 1);
    
    printf("Array: ");
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n\n");
    
    printf("Sample Queries:\n");
    printf("1. Sum of elements in range [2, 5]: %d\n", query_sum(1, 0, n - 1, 2, 5));
    printf("2. Minimum element in range [2, 4]: %d\n", query_min(1, 0, n - 1, 2, 4));
    printf("3. Maximum element in range [1, 5]: %d\n", query_max(1, 0, n - 1, 1, 5));
    printf("4. Sum of all elements: %d\n", query_sum(1, 0, n - 1, 0, n - 1));
    printf("5. Minimum element in the entire array: %d\n", query_min(1, 0, n - 1, 0, n - 1));
    printf("6. Maximum element in the entire array: %d\n", query_max(1, 0, n - 1, 0, n - 1));
    
    return 0;
}
