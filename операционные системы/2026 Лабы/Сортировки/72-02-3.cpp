#include <iostream>
#include <sys/time.h>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <algorithm>

using namespace std;

//Лабораторная работа №3 Процессы и неименованые каналы
//ЗАМЕТКА: [0] - чтение [1] - запись 

int max_depth;
timeval global_t0;

void log_event(const char* msg, int l, int r, int depth, pid_t pid) {
    timeval tv;
    gettimeofday(&tv, nullptr);

    double ms = (tv.tv_sec - global_t0.tv_sec) * 1000.0 +
                (tv.tv_usec - global_t0.tv_usec) / 1000.0;

    cout << "[PID " << pid << "] "
         << "depth=" << depth << " [" << l << "," << r << "] "
         << msg << " t=+" << ms << " ms" << endl;
}

void merge_arrays(int* arr, int l, int m, int r) {
    int left_size = m - l + 1;
    int right_size = r - m;
    
    int* left = new int[left_size];
    int* right = new int[right_size];
    
    for (int i = 0; i < left_size; i++)
        left[i] = arr[l + i];
    for (int i = 0; i < right_size; i++)
        right[i] = arr[m + 1 + i];
    
    int i = 0, j = 0, k = l;
    
    while (i < left_size && j < right_size) {
        if (left[i] <= right[j])
            arr[k++] = left[i++];
        else
            arr[k++] = right[j++];
    }
    
    while (i < left_size)
        arr[k++] = left[i++];
    
    while (j < right_size)
        arr[k++] = right[j++];
    
    delete[] left;
    delete[] right;
}

void quicksort(int arr[], int l, int r) {
    if (l >= r) return;

    int i = l, j = r;
    int pivot = arr[(l + r) / 2];

    while (i <= j) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;
        if (i <= j) {
            swap(arr[i], arr[j]);
            i++; j--;
        }
    }

    if (l < j) quicksort(arr, l, j);
    if (i < r) quicksort(arr, i, r);
}

bool checksort(int arr[], int n) {
    for (int i = 1; i < n; i++)
        if (arr[i] < arr[i - 1]) return false;
    return true;
}


void write_all(int fd, void* buf, size_t bytes) {
    size_t total = 0;
    while (total < bytes) {
        ssize_t res = write(fd, (char*)buf + total, bytes - total);
        if (res <= 0) {
            perror("write");
            exit(1);
        }
        total += res;
    }
}

void write_array(int fd, int arr[], int size) {
    write_all(fd, &size, sizeof(size));
    write_all(fd, arr, size * sizeof(int));
}

void read_all(int fd, void* buf, size_t bytes) {
    size_t total = 0;
    while (total < bytes) {
        ssize_t res = read(fd, (char*)buf + total, bytes - total);
        if (res <= 0) {
            perror("read");
            exit(1);
        }
        total += res;
    }
}

void read_array(int fd, int*& arr, int& size) {
    read_all(fd, &size, sizeof(size));
    arr = new int[size];
    read_all(fd, arr, size * sizeof(int));
}

void cool_sort(int* arr, int l, int r, int depth) {
    if (l >= r) return;

    int size = r - l + 1;
    
    if (depth >= max_depth || size < 100000) {
        log_event("LOCAL SORT", l, r, depth, getpid());
        quicksort(arr, l, r);
        return;
    }

    int mid = (l + r) / 2;

    int left_size = mid - l + 1;
    int right_size = r - mid;

    int p2c_left[2], c2p_left[2];
    int p2c_right[2], c2p_right[2];

    pipe(p2c_left);
    pipe(c2p_left);
    pipe(p2c_right);
    pipe(c2p_right);

    pid_t left_pid = fork();

    if (left_pid == 0) {
        close(p2c_left[1]); // закрыли запись из родителя в потомка 
        close(c2p_left[0]); // закрыли чтение из потомка в родителя
        close(p2c_right[0]); // закрыли правую часть
        close(p2c_right[1]); // закрыли правую часть
        close(c2p_right[0]); // закрыли правую часть
        close(c2p_right[1]); // закрыли правую часть

        int* buf;
        int buf_size;
        read_array(p2c_left[0], buf, buf_size);

        cool_sort(buf, 0, buf_size - 1, depth + 1);

        write_array(c2p_left[1], buf, buf_size);
        
        delete[] buf;
        exit(0);
    }

    pid_t right_pid = fork();

    if (right_pid == 0) {
        close(p2c_right[1]); // закрыли запись из родителя в потомка 
        close(c2p_right[0]); // закрыли чтение из потомка в родителя
        close(p2c_left[0]); // закрыли левую часть
        close(p2c_left[1]); // закрыли левую часть
        close(c2p_left[0]); // закрыли левую часть
        close(c2p_left[1]); // закрыли левую часть

        int* buf;
        int buf_size;
        read_array(p2c_right[0], buf, buf_size);

        cool_sort(buf, 0, buf_size - 1, depth + 1);

        write_array(c2p_right[1], buf, buf_size);
        
        delete[] buf;
        exit(0);
    }

    close(p2c_left[0]); // закрыли чтение из родителя в потомка
    close(c2p_left[1]); // закрыли запись и потомка в родителя
    close(p2c_right[0]);// закрыли чтение из родителя в потомка
    close(c2p_right[1]);// закрыли запись и потомка в родителя

    write_array(p2c_left[1], arr + l, left_size);
    write_array(p2c_right[1], arr + mid + 1, right_size);

    close(p2c_left[1]);
    close(p2c_right[1]);

    int* left_res;
    int left_res_size;
    int* right_res;
    int right_res_size;

    read_array(c2p_left[0], left_res, left_res_size);
    read_array(c2p_right[0], right_res, right_res_size);

    close(c2p_left[0]);
    close(c2p_right[0]);

    waitpid(left_pid, nullptr, 0);
    waitpid(right_pid, nullptr, 0);

    for (int i = 0; i < left_res_size; i++) {
        arr[l + i] = left_res[i];
    }
    for (int i = 0; i < right_res_size; i++) {
        arr[mid + 1 + i] = right_res[i];
    }

    merge_arrays(arr, l, mid, r);

    delete[] left_res;
    delete[] right_res;

    log_event("MERGED", l, r, depth, getpid());
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <max_depth> <I>" << endl;
        return 1;
    }
    
    max_depth = atoi(argv[1]);
    int I = atoi(argv[2]);
    int n = 1 << I;

    gettimeofday(&global_t0, nullptr);

    int* arr = new int[n];

    srand(time(nullptr));
    for (int i = 0; i < n; i++)
        arr[i] = rand() % 10000;

    cout << "Starting sort with n=" << n << ", max_depth=" << max_depth << endl;
    
    cool_sort(arr, 0, n - 1, 0);

    cout << "Sorted: " << (checksort(arr, n) ? "OK" : "FAIL") << endl;

    delete[] arr;
    return 0;
}