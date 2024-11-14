#include <iostream>
#include <cstdint>
#include <fstream>
#include <string>

#include "red_black_tree.hpp"

int main() {

    int64_t key = 0;
    
    char mode;
    
    if ((std::cin >> mode).good() && mode == 'k') {
        if (!(std::cin >> key).good())
            return 0;
    }
    else 
        return 0;
    Tree::Red_black_tree<int64_t> rb_tree{key};

    int64_t a = 0, b = 0;

    while((std::cin >> mode).good()) {
        if (mode == 'k') {
            if (!(std::cin >> key).good())
                break;
            rb_tree.insert_elem(key);
        }
        else if (mode == 'q') {
            if (!(std::cin >> a >> b).good())
                continue;
            if (a > b)  {
                std::cout << "0\n";
                continue;
            }
            int64_t counter = rb_tree.search(a, b);
            std::cout << counter << std::endl;
        }
        else 
            break;
    }
    #ifndef NDEBUG
            /*   create a tree in graphviz   */
        std::ofstream file_graph;
        std::string file_name = "graphviz/file_graph.dot";
        file_graph.open(file_name);
    
        file_graph << "digraph RB_tree{\n"
                      "label = < Red-black tree >;\n"
                      "bgcolor = \"DarkGrey\";\n"
                      "node [shape = record, tfillcolor = \"pink\", penwidth = 5, color = \"Cornsilk\", fontcolor = \"white\" ];\n"
                      "edge [style = filled ];\n";
               
        rb_tree.create_graph_node(*rb_tree.root_, file_graph);
        file_graph << "}";
        file_graph.close();
        std::string graph_cmd = "dot -Tpng " + file_name + " -o " +
                              "graphviz/rb_tree.png -Gdpi=100\n";
    
        std::system(graph_cmd.c_str());
        std::system("open graphviz/rb_tree.png");
    #endif

    return 0;
}