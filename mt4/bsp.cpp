#include <iostream>
#include <utility>
#include <vector>
#include <omp.h>
#include <fstream>
#include <random>
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;

struct Segment {
    double x1, y1, x2, y2;
};

typedef vector<Segment> v_seg;

struct BSPNode {
    v_seg segments;
    BSPNode* front;
    BSPNode* back;

    explicit BSPNode(v_seg Set) : segments(std::move(Set)), front(nullptr), back(nullptr) {}
    ~BSPNode() {
        delete front;
        delete back;
    }
};

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
//    x = (B1 * C - B * C1) / det;
    y = (A1 * C - C1 * A) / det;
//    y = (A * C1 - A1 * C) / det;

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
        if (&seg == &div_seg){
            intersectSet.push_back(seg);
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
                intersectSet.push_back({seg.x1, seg.y1, x, y});
            }
        }
    }
    Set.clear();
    Set.shrink_to_fit();
}

BSPNode* buildBSP(v_seg& segments, int depth = 0) {
    if (segments.empty()) return nullptr;

    Segment div_seg = segments[segments.size() / 2];
    v_seg frontSet, backSet, intersectSet;

    divideSet(segments, div_seg, frontSet, backSet, intersectSet);
    auto node = new BSPNode(intersectSet);

    node->back = buildBSP(backSet, depth + 1);
    node->front = buildBSP(frontSet, depth + 1);

    return node;
}


BSPNode* mt_buildBSP(vector<Segment>& segments) {
    // TODO
    return nullptr;
}

v_seg generateSegments(int& count) {
    v_seg segments;
    mt19937 rng(random_device{}());
    uniform_int_distribution<> dist(0.0, 100.0);

    while (segments.size() < count) {
        Segment seg = {double (dist(rng)), double(dist(rng)), double(dist(rng)), double(dist(rng))};
        segments.push_back(seg);
    }
    return segments;
}

void exportToJSON(const std::vector<Segment>& segments, const std::string& filename) {
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);
    ostringstream oss;
    oss << put_time(localTime, "%d_%H-%M-%S");

    std::ofstream file(filename + oss.str() + ".json");
    file << "[\n";
    for (size_t i = 0; i < segments.size(); ++i) {
        const auto& seg = segments[i];
        file << "  {\"x1\": " << seg.x1 << ", \"y1\": " << seg.y1 << ", \"x2\": " << seg.x2 << ", \"y2\": " << seg.y2 << "}";
        if (i < segments.size() - 1) {
            file << ",";
        }
        file << "\n";
    }
    file << "]";
    file.close();
}

void printBSP(BSPNode* root, int depth = 0) {
    if (root == nullptr) return;

    for (auto seg : root->segments) {
        for (int i = 0; i < depth; ++i) cout << "  ";
        cout << "Segment: (" << seg.x1 << ", " << seg.y1 << ") -> ("
                  << seg.x2 << ", " << seg.y2 << ")\n";
    }

    for (int i = 0; i < depth; ++i) cout << "  ";
    cout << "back\n";
    printBSP(root->back, depth + 1);
    for (int i = 0; i < depth; ++i) cout << "  ";
    cout << "front\n";
    printBSP(root->front, depth + 1);
}

void mt4_test() {
//    v_seg segments = {
//            {80, 2, 40, 35}, {70, 5, 30, 48}
//            {80, 2, 70, 10}, {70, 6, 30, 48}
//            {70, 6, 30, 48}, {80, 2, 70, 10}
//    };
    int countSegments = 5;
    v_seg segments = generateSegments(countSegments);

    exportToJSON(segments, "../mt4/gens/");
    BSPNode* root = nullptr;

    root = buildBSP(segments);

    printBSP(root);

}
