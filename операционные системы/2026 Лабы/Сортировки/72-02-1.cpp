#include <iostream>

#include <sys/time.h>
#include <cstdlib>

#include <unistd.h>

#include <sys/wait.h>
#include <signal.h>
#include <cstring>

#include <sys/mman.h> 

#define USE_FORK 1  // 0 - без fork, 1 - с fork
#define MERGE_FORK_LEVEL 3 // до какого уровня создавать процессы для слияния
#define MAX_DEPTH 3 // до какого уровня создавать процессы для сортировки

using namespace std;

volatile sig_atomic_t child_ready1 = 0;
volatile sig_atomic_t child_ready2 = 0;

void sig_handler1я(int) { child_ready1 = 1; }
void sig_handler2(int) { child_ready2 = 1; }

timeval global_t0;

void log_event(const char* msg) {
    timeval tv;
    gettimeofday(&tv, nullptr);

    double ms =
        (tv.tv_sec - global_t0.tv_sec) * 1000.0 +
        (tv.tv_usec - global_t0.tv_usec) / 1000.0;

    cout << "[PID " << getpid() << "] "
         << msg << ", t = +" << ms << " ms" << endl;
}

void log_fork(const char* msg, int l, int r, int level) {
    timeval tv;
    gettimeofday(&tv, nullptr);

    double ms =
        (tv.tv_sec - global_t0.tv_sec) * 1000.0 +
        (tv.tv_usec - global_t0.tv_usec) / 1000.0;

    char buf[256];
    int n = snprintf(
        buf, sizeof(buf),
        "[PID %d] level=%d [%d,%d] %s, t = +%.3f ms\n",
        getpid(), level, l, r, msg, ms
    );

    write(STDOUT_FILENO, buf, n); 
}

void quicksort(int in[], int a, int b) {
    int i, j, mode;
    if (a >= b) return;
    for (i = a, j = b, mode = 1; i < j; mode > 0 ? j-- : i++)
        if (in[i] > in[j]) {
            int c = in[i];
            in[i] = in[j];
            in[j] = c;
            mode = -mode;
        }
    quicksort(in, a, i - 1);
    quicksort(in, i + 1, b);
}

bool checksort(int arr[], int n) {
    for (int i = 1; i < n; i++)
        if (arr[i] < arr[i - 1]) return false;
    return true;
}

void marge(int arr1[], int size1, int arr2[], int size2, int result[]) {
    int i = 0, j = 0, k = 0;
    while (i < size1 && j < size2)
        result[k++] = (arr1[i] < arr2[j]) ? arr1[i++] : arr2[j++];
    while (i < size1) result[k++] = arr1[i++];
    while (j < size2) result[k++] = arr2[j++];
}

void parallel_sort(int* arr, int l, int r, int level, int maxLevel) { 
    if (l >= r) return; 

    if (level >= maxLevel) {
        log_fork("LOCAL SORT", l, r, level);
        quicksort(arr, l, r);
        return;
    }

    int mid = (l + r) / 2;

    log_fork("FORK LEFT", l, mid, level);
    pid_t left_pid = fork();
    if (left_pid == 0) {
        log_fork("LEFT CHILD START", l, mid, level + 1);
        parallel_sort(arr, l, mid, level + 1, maxLevel);
        _exit(0);
    }

    log_fork("FORK RIGHT", mid + 1, r, level);
    pid_t right_pid = fork();
    if (right_pid == 0) {
        log_fork("RIGHT CHILD START", mid + 1, r, level + 1);
        parallel_sort(arr, mid + 1, r, level + 1, maxLevel);
        _exit(0);
    }

    waitpid(left_pid, nullptr, 0);
waitpid(right_pid, nullptr, 0);

if (level < MERGE_FORK_LEVEL) {
    pid_t merge_pid = fork();
    if (merge_pid == 0) {
        log_fork("MERGE CHILD", l, r, level);

        int n1 = mid - l + 1;
        int n2 = r - mid;

        int* left = new int[n1];
        int* right = new int[n2];

        memcpy(left, arr + l, n1 * sizeof(int));
        memcpy(right, arr + mid + 1, n2 * sizeof(int));

        marge(left, n1, right, n2, arr + l);

        delete[] left;
        delete[] right;
        _exit(0);
    }
    waitpid(merge_pid, nullptr, 0);
    } else {
        log_fork("MERGE LOCAL", l, r, level);

        int n1 = mid - l + 1;
        int n2 = r - mid;

        int* left = new int[n1];
        int* right = new int[n2];

        memcpy(left, arr + l, n1 * sizeof(int));
        memcpy(right, arr + mid + 1, n2 * sizeof(int));

        marge(left, n1, right, n2, arr + l);

        delete[] left;
        delete[] right;
    }
}

int main(int argc,char* argv[]) {
    cout.setf(std::ios::unitbuf);
    gettimeofday(&global_t0, nullptr);
    log_event("START");
    if (argc < 1) {
        cout << "Usage: " << argv[0] <<  "N" << endl;
        return 0;     
    }
    int N = atoi(argv[1]);
    const int I = 1 << N;

    int* a = (int*)mmap(
        nullptr,
        I * sizeof(int),
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS,
        -1,
        0
    );

    if (a == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    srand(time(nullptr));
    for (int i = 0; i < I; i++) a[i] = rand();

#if USE_FORK
    parallel_sort(a, 0, I - 1, 0, MAX_DEPTH);
#else
    quicksort(a, 0, N - 1);
#endif

    cout << "check: " << (checksort(a, I) ? "OK" : "FAIL") << endl;

    munmap(a, I * sizeof(int));   
    log_event("FINISH");
    return 0;
}