#include <iostream>
#include <vector>
#include <climits>
#include <omp.h>
#include <ctime>
#include <random>
#include <sstream>
#include <fstream>
#include <iomanip>

const int INF = INT_MAX;

using namespace std;

void initializeGraph(std::vector<std::vector<int>>& graph, int N) {
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> dist(1, N);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (i == j) {
                graph[i][j] = 0;
            } else {
                graph[i][j] = INF;
            }
        }
    }

    // Для каждой вершины добавляю 3 новых ребра
    for (int i = 0; i < N; ++i) {
        for (int k = 0; k < 3; ++k){
            int v = dist(mt);
            while (graph[i][v] == 0 || graph[i][v] != INF){
                v = dist(mt);
            }
            int weight = dist(mt) * dist(mt);
            graph[i][v] = weight;
        }
    }
}

void mt_floydWarshall(std::vector<std::vector<int>>& dist, int N) {
#pragma omp parallel for
    for (int k = 0; k < N; ++k) {
#pragma omp parallel for
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    int newDist = dist[i][k] + dist[k][j];
                    if (newDist < dist[i][j]) {
                        dist[i][j] = newDist;
                    }
                }
            }
        }
    }
}

void floydWarshall(std::vector<std::vector<int>>& dist, int N) {
    //Пробегаемся по всем вершинам и ищем более короткий путь
    //через вершину k
    for(int k = 0; k < N; k++) {
        for(int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++) {
                //Новое значение ребра равно минимальному между старым
                //и суммой ребер i <-> k + k <-> j (если через k пройти быстрее)
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    int newDist = dist[i][k] + dist[k][j];
                    if (newDist < dist[i][j]) {
                        dist[i][j] = newDist;
                    }
                }
            }
        }
    }
}

void printGraph(const std::vector<std::vector<int>>& graph, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (graph[i][j] == INF) {
                std::cout << "INF ";
            } else {
                std::cout << graph[i][j] << " ";
            }
        }
        std::cout << std::endl;
    }
}


void mt3_test() {
    int N = 7; // Количество вершин
    std::vector<std::vector<int>> graph(N, std::vector<int>(N, INF));

    initializeGraph(graph, N);
    std::vector<std::vector<int>> graph_seq(graph);

    std::cout << "Graph init:" << std::endl;
    printGraph(graph, N);

    mt_floydWarshall(graph, N);
    floydWarshall(graph_seq, N);

    std::cout << "After seq:" << std::endl;
    printGraph(graph_seq, N);
    std::cout << "After mt:" << std::endl;
    printGraph(graph, N);
}


void mt3_test_hard() {
    // Получаем текущее время
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);

    // Создаем строковый поток для форматирования строки
    ostringstream oss;

    // Форматируем дату и время в строку
    oss << put_time(localTime, "%Y-%m-%d_%H-%M-%S");

    // Формируем имя файла
    string outputFileName = "../tests/floid_" + oss.str() + ".txt";

    ofstream out(outputFileName.c_str());
    streambuf* coutbuf = cout.rdbuf();
    cout.rdbuf(out.rdbuf());

    cout << "N,seq_time,mt_time\n";
    vector<int> N{ 5, 10, 20, 40, 80, 160, 320, 640, 800, 900, 1000,
                   1100, 1150, 1200, 1300, 1500, 2000, 2500, 3000}; // Количество вершин

    for(auto n : N){
        cout << n;
        std::vector<std::vector<int>> graph(n, std::vector<int>(n, INF));

        initializeGraph(graph, n);
        std::vector<std::vector<int>> graph_seq(graph);

        double st = omp_get_wtime();
        floydWarshall(graph_seq, n);
        double ft = omp_get_wtime();
        cout << "," << ft - st;

        st = omp_get_wtime();
        mt_floydWarshall(graph, n);
        ft = omp_get_wtime();
        cout << "," << ft - st << endl;
    }
}
