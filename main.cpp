#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
#include <vector>

#define N_min 256
#define N_max 16777216
#define K 10

int *fill_in_forward(size_t size) {
    auto arr = new int[size];
    for (int i = 0; i < size - 1; i++) {
        arr[i] = i + 1;
    }
    arr[size - 1] = 0;
    return arr;
}
int main() {
    int *test_arr;
    test_arr = fill_in_forward(N_min);
    return 0;
}
