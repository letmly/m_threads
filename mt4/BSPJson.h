#ifndef VSTU_MULTITHREADS_BSPJSON_H
#define VSTU_MULTITHREADS_BSPJSON_H

#include <string>
#include "BSP.h"

std::string segmentToJson(const Segment& seg);
std::string vectorToJson(const v_seg& segments);
std::string bspNodeToJson(const BSPNode* node);
void saveTreeToJson(const BSPNode* root, const std::string& filename);
void exportToJSON(const std::vector<Segment>& segments, const std::string& filename);
void printBSP(BSPNode* root, int depth = 0);
std::string get_time();

#endif //VSTU_MULTITHREADS_BSPJSON_H
