#include <iostream>
#include <fstream>
#include <cstdint>

#include "red_black_tree.hpp"

static void run_tests();

int main() {

    run_tests();
}

void run_tests() {

    const int TEST_NUM = 9;

    std::ifstream test_file;
    test_file.open("./tests/tests.txt");
    if (!test_file.is_open())
        return;

    int64_t a = 0, b = 0;
    int64_t key, counter_ref;

    char mode;
    bool all_tests_pass = true;
    int  test_counter = 0;
 
    for (int i = 0; i != TEST_NUM; ++i) {

        test_file >> mode;
        if (mode != 'k')
            break;
        test_file >> key;
        Tree::Red_black_tree<int64_t> rb_tree{key};

        while(test_file >> mode) {

            if (mode == 'k') {
                test_file >> key;
                rb_tree.insert_elem(key);
            }
            else if (mode == 'q') {
                test_file >> a >> b;
                    
                int64_t counter;
                if (a > b) 
                    counter = 0;
                else 
                   counter = rb_tree.search(a, b);

                test_file >> counter_ref;
                if (counter != counter_ref) {
                    std::cout << "test [" << test_counter << "] failed \n";
                    all_tests_pass = false;
                }
                ++test_counter;
                break;
            }
            else 
                break;
        }
    }

    if (all_tests_pass)
       // std::cout << "-- All tests passed -- \n";
       std::cout << "-1";
}