#ifndef VSTU_MULTITHREADS_BSP_H
#define VSTU_MULTITHREADS_BSP_H

#include "vector"

struct Segment {
    double x1, y1, x2, y2;

    bool operator==(const Segment& other) const {
        return (x1 == other.x1 && y1 == other.y1 && x2 == other.x2 && y2 == other.y2
        ||      x1 == other.x2 && y1 == other.y2 && x2 == other.x1 && y2 == other.y1);
    }
};

typedef std::vector<Segment> v_seg;

struct BSPNode {
    v_seg segments;
    BSPNode* front;
    BSPNode* back;

    explicit BSPNode(v_seg Set) : segments(std::move(Set)), front(nullptr), back(nullptr) {}
    ~BSPNode() {
        delete front;
        delete back;
    }

    bool operator==(const BSPNode& other) const {
        if (segments.size() != other.segments.size()) return false;

        for (const auto& seg : segments) {
            if (std::find(other.segments.begin(), other.segments.end(), seg) == other.segments.end()) {
                return false;
            }
        }

        bool frontEqual = (front == nullptr && other.front == nullptr) || (front != nullptr && other.front != nullptr && *front == *other.front);
        bool backEqual = (back == nullptr && other.back == nullptr) || (back != nullptr && other.back != nullptr && *back == *other.back);

        return frontEqual && backEqual;
    }

    int countSegments() const {
        int count = segments.size();
        if (front) {
            count += front->countSegments();
        }
        if (back) {
            count += back->countSegments();
        }
        return count;
    }

};

#endif //VSTU_MULTITHREADS_BSP_H
