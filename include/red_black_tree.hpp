#include <iostream>
#include <memory>
#include <fstream>
#include <cstdint>

namespace Tree {

enum class Color {
    red,
    black,
};

template <typename Key_t>
struct Node {

    std::shared_ptr<Node<Key_t>> parent_ = nullptr;
    std::shared_ptr<Node<Key_t>> left_   = nullptr;
    std::shared_ptr<Node<Key_t>> right_  = nullptr;

    Color color_;
    Key_t key_;

    Node(const Key_t &key, Color color = Color::red) : key_{key}, color_{color} {}
};

/** @brief Red_black_tree - a class of spaces and methods of red-black tree.
 * Invariant - search, insert, delete time is O(logN)
 */
template <class Key_t>
class Red_black_tree final {

private:

    std::shared_ptr<Node<Key_t>> root_ = nullptr;

    /** @brief method that sustains the balace invariant 
     *  @param node - new node, that was inserted 
     */
    void fix_insert(std::shared_ptr<Node<Key_t>> node) {

        while (node != root_ && node->parent_->color_ == Color::red) {

            if (!node->parent_ || !node->parent_->parent_) 
                continue;

            if (node->parent_ == node->parent_->parent_->left_) {

                auto uncle = node->parent_->parent_->right_;

                if (uncle && uncle->color_ == Color::red) {
                    fix_insert_without_rotate(node, uncle);
                }
                else {
                    if (node == node->parent_->right_) {
                        node = node->parent_;
                        left_rotate(node);
                    }
                    node->parent_->color_ = Color::black;
                    node->parent_->parent_->color_ = Color::red;
                    right_rotate(node->parent_->parent_);
                }
            } 
            else {
                auto uncle = node->parent_->parent_->left_;

                if (uncle && uncle->color_ == Color::red) {
                    fix_insert_without_rotate(node, uncle);
                } 
                else {
                    if (node == node->parent_->left_) {
                        node = node->parent_;
                        right_rotate(node);
                    }
                    node->parent_->color_ = Color::black;
                    node->parent_->parent_->color_ = Color::red;
                    left_rotate(node->parent_->parent_);
                }
            }
            
        }
        root_->color_ = Color::black;
    }

    void fix_insert_without_rotate(std::shared_ptr<Node<Key_t>> node, 
                                    std::shared_ptr<Node<Key_t>> uncle) {
        node->parent_->color_ = Color::black;
        uncle->color_ = Color::black;
        node->parent_->parent_->color_ = Color::red;
        node = node->parent_->parent_;
    }

    void left_rotate(const std::shared_ptr<Node<Key_t>> node) {

        auto y = node->right_;
        node->right_ = y->left_;

        if (y->left_) 
            y->left_->parent_ = node;
        y->parent_ = node->parent_;

        if (!node->parent_) 
            root_ = y;
        else if (node == node->parent_->left_) 
            node->parent_->left_ = y;
        else 
            node->parent_->right_ = y;

        y->left_ = node;
        node->parent_ = y;
    }

    void right_rotate(const std::shared_ptr<Node<Key_t>> node) {

        auto y = node->left_;
        node->left_ = y->right_;
        if (y->right_) 
            y->right_->parent_ = node;
        y->parent_ = node->parent_;
        if (!node->parent_) 
            root_ = y;
        else if (node == node->parent_->right_) 
            node->parent_->right_ = y;
        else 
            node->parent_->left_ = y;
        y->right_ = node;
        node->parent_ = y;
    }

public:
    Red_black_tree(Key_t key) {
        root_ = std::make_shared<Node<Key_t>>(key, Color::black);
    }
    Red_black_tree() = default;

    std::shared_ptr<Node<Key_t>> get_root() const {
        return root_;
    }

    void insert_elem(const Key_t& key) {

        auto new_node = std::make_shared<Node<Key_t>>(key);
        auto current = root_;
        std::shared_ptr<Node<Key_t>> parent = nullptr;

        while (current) {
            parent = current;
            if (key < current->key_) 
                current = current->left_;
            else if (key > current->key_)
                current = current->right_;
            else 
                return;
        }

        new_node->parent_ = parent;
        if (!parent) 
            root_ = new_node;
        else if (key < parent->key_) 
            parent->left_ = new_node;
        else 
            parent->right_ = new_node;

        new_node->color_ = Color::red;
        fix_insert(new_node);
    }

    uint64_t range_queries(const Key_t& key1, const Key_t& key2) const { 

        uint64_t counter = 0;

        search(root_, counter, key1, key2);

        return counter;
    }

private:

    void search(const std::shared_ptr<Node<Key_t>> node, uint64_t& counter, 
                                const Key_t& key1, const Key_t& key2) const {

        if (node->key_ >= key1 && node->key_ <= key2) {
            counter++;
            if (node->left_)
                search(node->left_, counter, key1, key2);
            if (node->right_)
            search(node->right_, counter, key1, key2);
        }
        else if (node->key_ < key1 && node->right_) {
            search(node->right_, counter, key1, key2);
        }
        else if (node->key_ > key2 && node->left_) {
            search(node->left_, counter, key1, key2);
        }
        return;
    }
};

template <class Key_t>
class print_tree final {
public:

    /*   create a tree grpah in graphviz   */
    void print_graph(const Red_black_tree<int64_t>& rb_tree) {

        std::ofstream file_graph;
        std::filesystem::path file_name = std::filesystem::path("graphviz") / "file_graph.dot";
        file_graph.open(file_name);

        file_graph << "digraph RB_tree{\n"
                      "label = < Red-black tree >;\n"
                      "bgcolor = \"DarkGrey\";\n"
                      "node [shape = record, tfillcolor = \"pink\", penwidth = 5, color = \"Cornsilk\", fontcolor = \"white\" ];\n"
                      "edge [style = filled ];\n";

        create_graph(file_graph, rb_tree.get_root());
        file_graph << "}";
        file_graph.close();
        std::stringstream graph_cmd;
        graph_cmd << "dot -Tpng " << file_name << " -o " << "graphviz/rb_tree.png -Gdpi=100\n";

        std::system(graph_cmd.str().c_str());
        std::system("open graphviz/rb_tree.png");
    }

    static void create_graph(std::ofstream& file_name, std::shared_ptr<Node<Key_t>> root) {

        create_graph_node(*root, file_name);
    }

    static void create_graph_node(Node<Key_t>& node, std::ofstream& file_name) {

        if (node.left_) {
            file_name << node.key_ << " [shape = Mrecord, style = filled, fillcolor = " 
                << (static_cast<int>(node.color_) == 0 ? "DarkRed" : "black") << ", label = \"" <<  node.key_ << "\" ];\n"
                << node.left_->key_ << " [shape = Mrecord, style = filled, fillcolor = " 
                << (static_cast<int>(node.left_->color_) == 0 ? "DarkRed" : "black") << ", label = \"" << node.left_->key_ << "\" ];\n"
                << node.key_ << " -> " << node.left_->key_ << ";\n";
            create_graph_node(*node.left_, file_name);
        }
        if (node.right_) {
            file_name << node.key_ << " [shape = Mrecord, style = filled, fillcolor = " 
                << (static_cast<int>(node.color_) == 0 ? "DarkRed" : "black") << ", label = \"" <<  node.key_ << "\" ];\n"
                << node.right_->key_ << " [shape = Mrecord, style = filled, fillcolor = " 
                << (static_cast<int>(node.right_->color_) == 0 ? "DarkRed" : "black") << ", label = \"" << node.right_->key_ << "\" ];\n"
                << node.key_ << " -> " << node.right_->key_ << ";\n";
            create_graph_node(*node.right_, file_name);
        }
    }
};
}

namespace Range_queries {

enum class Mode {
    cin,
    file
};

template<typename Key_t>
class range_quries final {

public:

Tree::Red_black_tree<Key_t> rb_tree;

void add_element(Mode mode) {

    if (mode != Mode::cin)
        throw std::invalid_argument("file mode without file.txt");

    Key_t key;
    
    if (!(std::cin >> key).good())
            throw std::invalid_argument("worng value");
    else if (rb_tree.get_root() == nullptr) {
        Tree::Red_black_tree<Key_t> new_tree{key};
        rb_tree = new_tree;
    }

    rb_tree.insert_elem(key);
}
void add_element(Mode mode, std::ifstream& file) {

    if (mode != Mode::file)
        throw std::invalid_argument("cin mode with file.txt");

    Key_t key;

    if (!(file >> key).good())
            throw std::invalid_argument("worng value");
    else if (rb_tree.get_root() == nullptr) {
        Tree::Red_black_tree<Key_t> new_tree{key};
        rb_tree = new_tree;
    }

    rb_tree.insert_elem(key);
}

int64_t find_range_elements(Mode mode) {

    if (mode != Mode::file)
        throw std::invalid_argument("file mode without file.txt");

    int64_t a, b;

    if (!(std::cin >> a >> b).good()) 
        throw std::invalid_argument("wrong value");
    if (a > b)  {
        //std::cout << "0 ";
        return 0;
    }
    else if (rb_tree.get_root() == nullptr) {
        return 0;
    }

    int64_t counter = rb_tree.range_queries(a, b);
    
    return counter;
}
int64_t find_range_elements(Mode mode, std::ifstream& file) {

    if (mode != Mode::file)
        throw std::invalid_argument("cin mode with file.txt");

    int64_t a, b;

    if (!(file >> a >> b).good()) 
        throw std::invalid_argument("wrong value");
    if (a > b)  {
        //std::cout << "0 ";
        return 0;
    }
    else if (rb_tree.get_root() == nullptr) {
        return 0;
    }

    int64_t counter = rb_tree.range_queries(a, b);
    
    return counter;
}
};
}
