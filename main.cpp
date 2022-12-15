#include <iostream>
#include <fstream>

//size in bytes:
constexpr int cache_size_lvl3 = 8 * 1024 * 1024;
constexpr int cache_size_int = cache_size_lvl3 / 4;
constexpr int offset = cache_size_int;
constexpr int repeats = 10;

int *fill(size_t size, int fragm_count) {
    auto arr = new int[size];
    int fragm_size = cache_size_int / fragm_count;
    int ind;
    for (int i = 0; i < fragm_size; i++) {
        ind = i;
        for (int j = 0; j < fragm_count - 1; j++) {
            arr[ind] = ind + offset;
            ind += offset;
        }
        arr[ind] = i + 1;
    }
    arr[ind] = 0;
    return arr;
}

void bypass(const int *arr, int fragm_count, std::ofstream &fout) {
    union ticks {
        unsigned long long t64;
        struct s32 {
            long th, tl;
        } t32;
    } start{}, end{};
    int fragm_size = cache_size_int / fragm_count;
    long long min = LLONG_MAX;
    for (int j = 0; j < repeats; j++) {
        asm volatile ("rdtsc\n":"=a"(start.t32.th), "=d"(start.t32.tl)::"memory");
        for (int k = 0, i = 0; i < fragm_count * fragm_size; i++) {
            k = arr[k];
            if (k == -1)
                fout << "--\n";
        }
        asm volatile ("rdtsc\n":"=a"(end.t32.th), "=d"(end.t32.tl)::"memory");
        long long ticks = end.t64 - start.t64;
        min = min < ticks ? min : ticks;
    }
    fout << "Count of fragments: " << fragm_count << "   ";
    fout << "Time taken: " << (min) / (fragm_count * fragm_size) << " ticks\n";
    fout.flush();
}

int main() {
    try {
        int *test_arr;
        std::ofstream res("res.txt");
        if (!res.is_open()) {
            std::cerr << "Failed to open file" << std::endl;
            return 1;
        }
        for (int frag_cnt = 1; frag_cnt <= 32; frag_cnt++) {
            int arr_size = offset * frag_cnt;
            test_arr = fill(arr_size, frag_cnt);
            bypass(test_arr, frag_cnt, res);
        }
        res.close();
    }
    catch (std::bad_alloc &e) {
        std::cerr << e.what();
    }
    return 0;
}
