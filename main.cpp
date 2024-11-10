#include <iostream>
#include <cstdint>
#include <memory>
#include <fstream>
#include <string>

#include "red_black_tree.hpp"

// что сделать еще
// поиск элемента 
// удаление элемента
// сделать 5 (copy ctor, move ctor, op=, op=, dtor)

int main() {
    uint64_t key = 100; // сделать добавление в режиме реального времени
    int a = 0, b = 0;

    Tree::Red_black_tree<uint64_t> rb_tree{key};

    std::cout << "root was created\n";

    char mode;

    while(std::cin >> mode) {
        if (mode == 'k') {
            std::cout << "insert\n"; 
            std::cin >> key;
            rb_tree.insert_elem(key);
        }
        else if (mode == 'q') {
            std::cin >> a >> b;
            if (a > b)  {
                std::cout << "0\n";
                break;
            }
            rb_tree.search(a, b);
        }
        else 
            break;
    }
    //#ifndef NDEBUG
        /*   create a tree in graphviz   */
    std::ofstream file_graph;
    std::string file_name = "graphviz/file_graph.dot";
    file_graph.open(file_name);

    file_graph << "digraph RB_tree{\n"
                  "label = < Red-black tree >;\n"
                  "bgcolor = \"DarkGrey\";\n"
                  "node [shape = record, tfillcolor = \"pink\", penwidth = 5, color = \"Cornsilk\", fontcolor = \"white\" ];\n"
                  "edge [style = filled ];\n";
           
    rb_tree.print(*rb_tree.root_, file_graph);
    file_graph << "}";
    file_graph.close();
    std::string graph_cmd = "dot -Tpng " + file_name + " -o " +
                          "graphviz/rb_tree.png -Gdpi=100\n";

    std::system(graph_cmd.c_str());
    //#endif

    return 0;
}