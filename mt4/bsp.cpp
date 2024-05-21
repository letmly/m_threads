#include <iostream>
#include <utility>
#include <vector>
#include <random>
#include <sstream>
#include "BSP.h"
#include "BSPJson.h"

#include <omp.h>
#include <omp_llvm.h>

using namespace std;

void getLineEquation(Segment& seg, double& A, double& B, double& C) {
    A = seg.y1 - seg.y2;
    B = seg.x2 - seg.x1;
    C = seg.x1 * seg.y2 - seg.x2 * seg.y1;
}

double evaluateLineEquation(double& A, double& B, double& C, double& x, double& y) {
    return A * x + B * y + C;
}

bool findIntersection(double& A, double& B, double& C, Segment& seg, double& x, double& y) {
    double A1, B1, C1;
    getLineEquation(seg, A1, B1, C1);

    double det = A * B1 - A1 * B;
    if (std::abs(det) < std::numeric_limits<double>::epsilon()) {
        return false;  // Линии параллельны или совпадают
    }
    x = (C1 * B - C * B1) / det;
    y = (A1 * C - C1 * A) / det;

    if ((x >= std::min(seg.x1, seg.x2) && x <= std::max(seg.x1, seg.x2)) &&
        (y >= std::min(seg.y1, seg.y2) && y <= std::max(seg.y1, seg.y2))) {
        return true;
    }

    return false;
}

void divideSet(v_seg& Set, Segment& div_seg, v_seg& frontSet, v_seg& backSet, v_seg& intersectSet) {
    double A, B, C;
    getLineEquation(div_seg, A, B, C);

    for (Segment& seg : Set) {
        if (seg == div_seg){
            intersectSet.push_back(seg);
            continue;
        }

        double D1 = evaluateLineEquation(A, B, C, seg.x1, seg.y1);
        double D2 = evaluateLineEquation(A, B, C, seg.x2, seg.y2);

        if (D1 > 0 && D2 > 0) {
            frontSet.push_back(seg);
        } else if (D1 < 0 && D2 < 0) {
            backSet.push_back(seg);
        } else if (D1 == 0 && D2 == 0){
            intersectSet.push_back(seg);
        } else {
            // Сегмент пересекает линию
            double x, y;
            if (findIntersection(A, B, C, seg, x, y)) {
                if (D1 > 0) {
                    frontSet.push_back({seg.x1, seg.y1, x, y});
                    backSet.push_back({x, y, seg.x2, seg.y2});
                } else {
                    backSet.push_back({seg.x1, seg.y1, x, y});
                    frontSet.push_back({x, y, seg.x2, seg.y2});
                }
            } else {
                intersectSet.push_back(seg);
            }
        }
    }
    Set.clear();
    Set.shrink_to_fit();
}

BSPNode* buildBSP(v_seg& segments, int depth = 0) {
    if (segments.empty()) return nullptr;
    if (segments.size() == 1) {
        auto node = new BSPNode(segments);
        return node;
    }
    Segment div_seg = segments[segments.size() / 2];
    v_seg frontSet, backSet, intersectSet;

    divideSet(segments, div_seg, frontSet, backSet, intersectSet);
    auto node = new BSPNode(intersectSet);

    node->back = buildBSP(backSet, depth + 1);
    node->front = buildBSP(frontSet, depth + 1);

    return node;
}

v_seg generateSegments(int& count) {
    v_seg segments;
    mt19937 rng(random_device{}());
    uniform_int_distribution<> dist(0.0, 1000.0);

    while (segments.size() < count) {
        Segment seg = {double (dist(rng)), double(dist(rng)), double(dist(rng)), double(dist(rng))};
        segments.push_back(seg);
    }
    return segments;
}


void mt4_test(int s_count = 10, bool write_tree = false, std::string from = "", std::string spec_dir = "out") {
    v_seg segments;

    if (!from.empty()) {
        // TODO читать из файла
    }

    v_seg segments_copy;

    cout << "worst predict of segments amount = " << s_count + 4 * s_count * log2(s_count) << "\n";
    cout << "good predict of segments amount = " << s_count + 2 * s_count * log2(s_count) << "\n";


    for (int i : {1, 2, 3, 1652334, 9978595, 536262334, 88878595}) {
        BSPNode* root = nullptr;

        segments = generateSegments(s_count);
        std::minstd_rand0 g1 (i);
        std::shuffle(std::begin(segments), std::end(segments), g1);

        segments_copy = segments;

        double st = omp_get_wtime();
        root = buildBSP(segments_copy);
        double ft = omp_get_wtime();
        cout << "\n\n-----------------TEST-----------------\n";
        cout << "\tRACE BSP built of " << s_count << "segs in: " << ft - st << " seconds.\n";
        cout << "\tRACE BSP seg count: " << root->countSegments() << "\n\n";

        segments_copy = segments;

        st = omp_get_wtime();
        omp_set_max_active_levels(10);
        Segment div_seg = segments[segments.size() / 2];
        v_seg frontSet, backSet, intersectSet;

        divideSet(segments, div_seg, frontSet, backSet, intersectSet);
        auto mt_root = new BSPNode(intersectSet);

        v_seg frontSet1, backSet1, intersectSet1;
        div_seg = frontSet[frontSet.size() / 2];
        divideSet(frontSet, div_seg, frontSet1, backSet1, intersectSet1);
        auto sub_node1 = new BSPNode(intersectSet1);

        v_seg frontSet2, backSet2, intersectSet2;
        div_seg = backSet[backSet.size() / 2];
        divideSet(backSet, div_seg, frontSet2, backSet2, intersectSet2);
        auto sub_node2 = new BSPNode(intersectSet2);


        #pragma omp parallel num_threads(4)
        {
            #pragma omp single
            {
                sub_node1->front = buildBSP(frontSet1);
            }
            #pragma omp single
            {
                sub_node1->back = buildBSP(backSet1);
            }
            #pragma omp single
            {
                sub_node2->front = buildBSP(frontSet2);
            }
            #pragma omp single
            {
                sub_node2->back = buildBSP(backSet2);
            }
        }

        mt_root->front = sub_node1;
        mt_root->back = sub_node2;

        cout << "\tMultiProc BSP built of " << s_count << "segs in: " << omp_get_wtime() - st << " seconds.\n";
        cout << "\tMultiProc BSP seg count: " << mt_root->countSegments() << "\n";
    }
}

void mt4_test_hard(string spec_dir = "out") {

    vector<int> segments_counts = { 100, 1000, 10000};
    double st;
    v_seg segments_copy;

    BSPNode* root = nullptr;
    BSPNode* mt_root = nullptr;
    for (int& count : segments_counts) {
        v_seg segments = generateSegments(count);
        segments_copy = segments;

        double st = omp_get_wtime();
        root = buildBSP(segments_copy);

        BSPNode* mt_root = nullptr;
        st = omp_get_wtime();
        mt_root = buildBSP(segments_copy);
        cout << "\tMultiProc BSP built of " << count << "segs in: " << omp_get_wtime() - st << " seconds.\n";

        cout << "saving trees...\n";

    }
}
