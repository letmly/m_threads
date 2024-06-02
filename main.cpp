#include "string"

void mt1_test();
void mt3_test();
void mt3_test_hard();
void mt4_test(int segments_count = 10, bool write_tree = false, std::string from = "", std::string spec_dir = "");


int main() {
//    mt1_test();
//    mt3_test_hard();
    mt4_test(8000, false, "D:\\.dev\\m_threads\\mt4\\segmets_from_imgs\\horseeye.json");

}