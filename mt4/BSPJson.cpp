#include "BSPJson.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <iostream>

using namespace std;


string segmentToJson(const Segment& seg) {
    ostringstream oss;
    oss << "{\"x1\":" << seg.x1 << ",\"y1\":" << seg.y1 << ",\"x2\":" << seg.x2 << ",\"y2\":" << seg.y2 << "}";
    return oss.str();
}

string vectorToJson(const v_seg& segments) {
    ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < segments.size(); ++i) {
        if (i != 0) oss << ",";
        oss << segmentToJson(segments[i]);
    }
    oss << "]";
    return oss.str();
}

string bspNodeToJson(const BSPNode* node) {
    if (node == nullptr) {
        return "null";
    }

    ostringstream oss;
    oss << "{";
    oss << "\"segments\":" << vectorToJson(node->segments) << ",";
    oss << "\"front\":" << bspNodeToJson(node->front) << ",";
    oss << "\"back\":" << bspNodeToJson(node->back);
    oss << "}";

    return oss.str();
}

std::string get_time() {
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);
    ostringstream oss;
    oss << put_time(localTime, "%d_%H-%M-%S");
    return oss.str();
}

void saveTreeToJson(const BSPNode* root, const string& filename) {
    ofstream file(filename + get_time() + ".json");
    file << bspNodeToJson(root);
    file.close();
}

void exportToJSON(const std::vector<Segment>& segments, const std::string& filename) {
    std::ofstream file(filename + get_time() + ".json");
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

void printBSP(BSPNode* root, int depth) {
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