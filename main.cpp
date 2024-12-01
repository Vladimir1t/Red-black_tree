#include <iostream>
#include <sstream>
#include <cstdint>
#include <fstream>
#include <filesystem>
#include <string>
#include <set>

#include "red_black_tree.hpp"

static void print_graph(const Tree::Red_black_tree<int64_t>& rb_tree);
static int64_t range_queries_set(const std::set<int64_t>& set, int64_t a, int64_t b);

int main() {

    try {
        #ifdef SET_MODE_ENABLED
            std::set<int64_t> set; 
        #endif
        int64_t key = 0;
        char    mode;

        Range_queries::range_quries<int64_t> range_quer;

        int64_t a = 0, b = 0;

        while((std::cin >> mode).good()) {
            switch(mode) {
                case 'k':
                    range_quer.add_element(Range_queries::Mode::cin);
                    break;

                case 'q': 
                    std::cout << range_quer.find_range_elements(Range_queries::Mode::cin) << ' '; 
                    break;

                default:  
                    throw std::invalid_argument("wrong mode value");
                    break;
            }
        }
        std::cout << std::endl;

        #ifndef NDEBUG
           print_graph(range_quer.rb_tree);
        #endif
    }
    catch (const std::exception&) {
        std::cout << "Error\n";
        return -1;
    }
    
    return 0;
}

/*   create a tree grpah in graphviz   */
void print_graph(const Tree::Red_black_tree<int64_t>& rb_tree) {

    std::ofstream file_graph;
    std::filesystem::path file_name = "graphviz/file_graph.dot";
    file_graph.open(file_name);

    file_graph << "digraph RB_tree{\n"
                  "label = < Red-black tree >;\n"
                  "bgcolor = \"DarkGrey\";\n"
                  "node [shape = record, tfillcolor = \"pink\", penwidth = 5, color = \"Cornsilk\", fontcolor = \"white\" ];\n"
                  "edge [style = filled ];\n";
           
    rb_tree.create_graph(file_graph);
    file_graph << "}";
    file_graph.close();
    std::stringstream graph_cmd;
    graph_cmd << "dot -Tpng " << file_name << " -o " << "graphviz/rb_tree.png -Gdpi=100\n";
    
    std::system(graph_cmd.str().c_str());
    std::system("open graphviz/rb_tree.png");
}

int64_t range_queries_set(const std::set<int64_t>& set, int64_t a, int64_t b) {

    auto l1 = set.lower_bound(a);
    auto l2 = set.upper_bound(b);
    if (std::distance(set.begin(), l1) > std::distance(set.begin(), l2))
        std::swap(l1, l2);

    return std::distance(l1, l2);
}