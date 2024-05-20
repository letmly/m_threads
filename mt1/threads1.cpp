#include <vector>
#include <iostream>
#include <random>
#include <omp.h>
#include <thread>
#include <mutex>
#include <numeric>
#include <fstream>
#include <ctime>
#include <sstream>
#include <iomanip>


using namespace std;

void gen_arr(vector<int>&, int& M);
void exp(int& N, int& M, vector<int>& a);

void counting_sort(vector<int>& arr, int& M);
void race_counting_srt(vector<int>& arr, int& M);
void mt_counting_srt(vector<int>& arr, int& M);
void mutex_counting_srt(vector<int>& arr, int& M);

void me_test(void (*method)(vector<int>&, int&) ,vector<int>& a, int& M);

mutex mut;
int threads_count = 4;

void mt1_test() {

    // Получаем текущее время
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);

    // Создаем строковый поток для форматирования строки
    ostringstream oss;

    // Форматируем дату и время в строку
    oss << put_time(localTime, "%Y-%m-%d_%H-%M-%S");

    // Формируем имя файла
    string outputFileName = "../tests/" + oss.str() + ".txt";

    ofstream out(outputFileName.c_str());
    streambuf* coutbuf = cout.rdbuf();
    cout.rdbuf(out.rdbuf());

//    vector<int> N{ 100000, 1000000, 10000000, 100000000, 250000000, 350000000, 650000000, 750000000 };
    vector<int> N{ 100000, 1000000 };
    vector<int> M{ 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000 };

    cout << "N,M,seq_time,race_time,mutex_time,mt_time,race_val,mutex_val,mt_val";
    for (int & i : N) {
        vector<int> g(i);
        for (int & j : M) {
            gen_arr(g, j);
            exp(i, j, g);
        }
    }
}

void exp(int& N, int& M, vector<int>& snip) {
    cout << endl << N << "," << M;
    vector<int> a(snip);
    vector<int> b(snip);
    vector<int> c(snip);
    vector<int> d(snip);

    me_test(counting_sort, a, M);
    me_test(race_counting_srt, b, M);
    me_test(mutex_counting_srt, c, M);
    me_test(mt_counting_srt, d, M);

    (a == b) ? cout << ",True" : cout << ",False";
    (a == c) ? cout << ",True" : cout << ",False";
    (a == d) ? cout << ",True" : cout << ",False";
}

void gen_arr(vector<int>& arr, int& M) {
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> dist(1, M - 1);

    for (int& a : arr) {
        a = dist(mt);
    }
}

void counting_sort(vector<int>& arr, int& M) {
    vector<int> C(M, 0);
    for (int a : arr) {
        C[a]++;
    }
    int pos = 0;
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < C[i]; ++j) {
            arr[pos] = i;
            pos++;
        }
    }
}

void race_for(vector<int>& A, vector<int>& C, int start, int end) {
    for (; start < end; ++start) {
        C[A[start]]++;
    }
}

void mutex_for(vector<int>& A, vector<int>& C, int start, int end) {
    // локал счетчик
    vector<int> Cloc(C.size(), 0);
    for (; start < end; ++start) {
        Cloc[A[start]]++;
    }

    lock_guard<mutex> lock(mut);
    for (int i = 0; i < size(C); ++i) {
        C[i] += Cloc[i];
    }
}

void threaded_placer(vector<int>& A, vector<int>& C, int pos, int start, int end) {
    for (; start < end; ++start) {
        for (int j = 0; j < C[start]; ++j) {
            A[pos] = start;
            pos++;
        }
    }
}

void threaded_summy(vector<int>& C1, vector<int>& C2) {
    for (int i = 0; i < size(C1); ++i) {
        C1[i] += C2[i];
    }
}

void cool_t_placer(vector<int>& A, vector<int>& C, int pos, int start, int end, int cnt) {
    for (; start < end; ++start) {
        for (int j = 0; j < C[start]; ++j) {
            A[pos] = start;
            pos++;
        }
    }
    for (int j = 0; j < cnt; ++j) {
        A[pos] = end;
        pos++;
    }
}

void race_counting_srt(vector<int>& A, int& M) {
    vector<int> C(M, 0);
    int chunkSize = A.size() / threads_count;

    vector<thread> threads;
    for (int i = 0; i < threads_count; ++i) {
        int start = i * chunkSize;
        int end = (i == threads_count - 1) ? A.size() : start + chunkSize;
        threads.emplace_back(race_for, ref(A), ref(C), start, end);
    }
    for (auto& thread : threads) {
        thread.join();
    }

    vector<int> poses = { 0 };
    int slice_s = M / threads_count;
    for (int i = 0; i < threads_count - 1; ++i) {
        int start = i * slice_s;
        int end = start + slice_s;
        int posis = accumulate(next(C.begin(), start), next(C.begin(), end), 0);
        poses.push_back(poses[i] + posis);
    }

    vector<thread> placing_threads;
    for (int i = 0; i < threads_count; ++i) {
        int start = i * slice_s;
        int end = (i == threads_count - 1) ? M : start + slice_s;
        placing_threads.emplace_back(threaded_placer, ref(A), ref(C), poses[i], start, end);
    }

    for (auto& thread : placing_threads) {
        thread.join();
    }
}

void mt_counting_srt(vector<int>& A, int& M) {
    int chunkSize = A.size() / threads_count;

    vector<vector<int>> Clocs(threads_count, vector<int>(M, 0));
    vector<thread> threads;
    for (int i = 0; i < threads_count; ++i) {
        int start = i * chunkSize;
        int end = (i == threads_count - 1) ? A.size() : start + chunkSize;
//      race_for, так как локальные Cloc
        threads.emplace_back(race_for, ref(A), ref(Clocs[i]), start, end);
    }
    for (auto& thread : threads) {
        thread.join();
    }

    // бесполезное говно - локнуть суммирование локальных Cloc быстрее
    int st = 0;
    if ((threads_count & (threads_count - 1)) == 0) {
        st = static_cast<int>(log2(threads_count));
    } else {
        st = static_cast<int>(log2(threads_count)) + 1;
    }
    for (int layer = 0; layer < st; ++layer) {
        vector<thread> summy;
        for (int i = 0; i + pow(2, layer) < threads_count; i+= pow(2, layer + 1)) {
            summy.emplace_back(threaded_summy, ref(Clocs[i]), ref(Clocs[i + pow(2, layer)]));
        }
        for (auto& thread : summy) {
            thread.join();
        }
    }

    vector<int> poses = { 0 };
    int slice_s = M / threads_count;
    for (int i = 0; i < threads_count - 1; ++i) {
        int start = i * slice_s;
        int end = start + slice_s;
        int posis = accumulate(next(Clocs[0].begin(), start), next(Clocs[0].begin(), end), 0);
        poses.push_back(poses[i] + posis);
    }

    vector<thread> placing_threads;
    for (int i = 0; i < threads_count; ++i) {
        int start = i * slice_s;
        int end = (i == threads_count - 1) ? M : start + slice_s;
        placing_threads.emplace_back(threaded_placer, ref(A), ref(Clocs[0]), poses[i], start, end);
    }

    for (auto& thread : placing_threads) {
        thread.join();
    }
}

void mutex_counting_srt(vector<int>& A, int &M) {
    vector<int> C(M, 0);
    int chunkSize = A.size() / threads_count;

    vector<thread> threads;
    for (int i = 0; i < threads_count; ++i) {
        int start = i * chunkSize;
        int end = (i == threads_count - 1) ? A.size() : start + chunkSize;
        threads.emplace_back(mutex_for, ref(A), ref(C), start, end);
    }
    for (auto& thread : threads) {
        thread.join();
    }

    int N = A.size();
    int t_bunch = N / threads_count;
    int appendix = N % threads_count;
    vector<int> ind = { 1 };
    vector<int> cnt = { 0 };
    for (int k = 0; k < threads_count; ++k) {
        int cur_cnt = t_bunch;
        int i = ind[ind.size() - 1];
        if (k == threads_count - 1) {
            cur_cnt += appendix;
        }

        while (cur_cnt > 0) {
            if (C[i] >= cur_cnt) {
                ind.push_back(i);
                cnt.push_back(cur_cnt);
                C[i] -= cur_cnt;
                cur_cnt = 0;
            }
            else {
                cur_cnt -= C[i];
                i++;
            }
        }
    }

    vector<thread> placing_threads;
    for (int i = 0; i < threads_count; ++i) {
        placing_threads.emplace_back(cool_t_placer, ref(A), ref(C), i * t_bunch, ind[i], ind[i + 1], cnt[i + 1]);
    }

    for (auto& thread : placing_threads) {
        thread.join();
    }
}

void me_test(void (*method)(vector<int>&, int&), vector<int>& a, int& M){
    double st;
    st = omp_get_wtime();
    method(a, M);
    double ft;
    ft = omp_get_wtime();
    cout << "," << ft - st;
}
