#include <iostream>
#include <sys/time.h>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>

using namespace std;

mutex log_mtx;
mutex thread_mtx;
atomic<int> active_threads{0};
int max_threads;  
timeval global_t0;

void log_event(const char* msg, int l, int r, int level, bool is_thread = false) {
    timeval tv;
    gettimeofday(&tv, nullptr);

    double ms = (tv.tv_sec - global_t0.tv_sec) * 1000.0 +
                (tv.tv_usec - global_t0.tv_usec) / 1000.0;

    lock_guard<mutex> lock(log_mtx);
    cout << "[TID " << this_thread::get_id() << "] ";
    cout << "level=" << level << " [" << l << "," << r << "] "
         << msg << " t=+" << ms << " ms" << endl;
}

void quicksort(int in[], int a, int b) {
    if (a >= b) return;
    
    int i = a, j = b;
    int pivot = in[(a + b) / 2];
    
    while (i <= j) {
        while (in[i] < pivot) i++;
        while (in[j] > pivot) j--;
        if (i <= j) {
            swap(in[i], in[j]);
            i++;
            j--;
        }
    }
    
    if (a < j) quicksort(in, a, j);
    if (i < b) quicksort(in, i, b);
}

bool checksort(int arr[], int n) {
    for (int i = 1; i < n; i++)
        if (arr[i] < arr[i - 1]) return false;
    return true;
}


void cool_sort(int* arr, int l, int r, int level) {
        int i = l, j = r;
        int pivot = arr[(l + r) / 2];

        while (i <= j) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;
        if (i <= j) {
            swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }
        
        thread right_thread([arr, i, r, level]() {
            log_event("RIGHT THREAD START", i, r, level + 1, true);
            quicksort(arr,i,r);
            log_event("RIGHT THREAD FINISH", i, r, level + 1, true);
        });
        log_event("LEFT THREAD START", l, j, level + 1, true);
        if (l < j) quicksort(arr,l,j);
        log_event("LEFT THREAD FINISH", l, j, level + 1, true);
        right_thread.join();
}

int main(int argc, char* argv[]) {
    int I = 1000000;            
    
    gettimeofday(&global_t0, nullptr);

    cout << "Array size: " << I << endl;
    cout << "Max threads: " << max_threads << endl << endl;
    
    int* arr = new int[I];
    
    srand(time(nullptr));
    for (int i = 0; i < I; i++)
        arr[i] = rand() % 10000;
    
    log_event("START SORTING", 0, I-1, 0);
    
    timeval t1, t2;
    gettimeofday(&t1, nullptr);
    
    cool_sort(arr, 0, I-1, 0);
    
    gettimeofday(&t2, nullptr);
    
    double time_ms = (t2.tv_sec - t1.tv_sec) * 1000.0 +
                     (t2.tv_usec - t1.tv_usec) / 1000.0;
    
    cout << "Sorted: " << (checksort(arr, I) ? "OK" : "FAIL") << endl;
    cout << "Time: " << time_ms << " ms" << endl;
    
    delete[] arr;
    return 0;
}

