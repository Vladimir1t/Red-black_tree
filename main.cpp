#include <iostream>
#include <cstdint>
#include <fstream>
#include <string>
#include <set>

#include "red_black_tree.hpp"

static void print_graph(const Tree::Red_black_tree<int64_t>& rb_tree);
static int64_t range_queries_set(const std::set<int64_t>& set, int64_t a, int64_t b);

int main() {

    #ifdef SET_MODE_ENABLED
        std::set<int64_t> set; 
    #endif
    int64_t key = 0;
    char    mode;
    
    while ((std::cin >> mode).good()) {
        if (mode == 'k') {
            if ((std::cin >> key).good()) {
                #ifdef SET_MODE_ENABLED
                    set.insert(key); 
                #endif
                break;
            }
        }
        else if (mode == 'q')
            std::cout << "0 ";
    }
    Tree::Red_black_tree<int64_t> rb_tree{key};

    int64_t a = 0, b = 0;
    
    while((std::cin >> mode).good()) {
        if (mode == 'k') {
            if (!(std::cin >> key).good())
                break;
            #ifdef SET_MODE_ENABLED
                set.insert(key); 
            #endif
            rb_tree.insert_elem(key);
        }
        else if (mode == 'q') {
            if (!(std::cin >> a >> b).good())
                continue;
            if (a > b)  {
                std::cout << "0 ";
                #ifdef SET_MODE_ENABLED
                    std::cout << "set = " << counter_set << ' ';
                #endif 
                continue;
            }
            int64_t counter = rb_tree.range_queries(a, b);
            std::cout << counter << ' ';
            #ifdef SET_MODE_ENABLED
                int64_t counter_set = range_queries_set(set, a, b);
                std::cout << "set = " << counter_set << ' ';
            #endif 
        }
        else 
            break;
    }
    std::cout << std::endl;

    #ifndef NDEBUG
       print_graph(rb_tree);
    #endif

    return 0;
}

/*   create a tree grpah in graphviz   */
void print_graph(const Tree::Red_black_tree<int64_t>& rb_tree) {

    std::ofstream file_graph;
    std::string file_name = "graphviz/file_graph.dot";
    file_graph.open(file_name);

    file_graph << "digraph RB_tree{\n"
                  "label = < Red-black tree >;\n"
                  "bgcolor = \"DarkGrey\";\n"
                  "node [shape = record, tfillcolor = \"pink\", penwidth = 5, color = \"Cornsilk\", fontcolor = \"white\" ];\n"
                  "edge [style = filled ];\n";
           
    rb_tree.create_graph_node(*rb_tree.root, file_graph);
    file_graph << "}";
    file_graph.close();
    std::string graph_cmd = "dot -Tpng " + file_name + " -o " +
                          "graphviz/rb_tree.png -Gdpi=100\n";

    std::system(graph_cmd.c_str());
    std::system("open graphviz/rb_tree.png");
}

int64_t range_queries_set(const std::set<int64_t>& set, int64_t a, int64_t b) {

    auto l1 = set.lower_bound(a);
    auto l2 = set.upper_bound(b);
    if (std::distance(set.begin(), l1) > std::distance(set.begin(), l2))
        std::swap(l1, l2);

    return std::distance(l1, l2);
}