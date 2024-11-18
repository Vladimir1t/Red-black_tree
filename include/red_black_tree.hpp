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

    Node(Key_t key) : key_(key), color_(Color::red) {}
    Node() = default;

    Node(const Node&& rhs) { 
        key_ = rhs.key_;
        color_ = rhs.color_;
    }

    Node& operator=(const Node& rhs) {  
        if (this == &rhs) {
            return *this;
        }
        key_    = rhs.key_;
        color_  = rhs.color_;
        parent_ = rhs.parent_;
        left_   = rhs.left_;
        right_  = rhs.right_;

        return *this;
    }
};

/** @brief Red_black_tree - a class of spaces and methods of red-black tree.
 * Invariant - search, insert, delete time is O(logN)
 */
template <class Key_t>
class Red_black_tree {

public:

    std::shared_ptr<Node<Key_t>> root = nullptr;

private:

    /** @brief method that sustains the balace invariant 
     *  @param node - new node, that was inserted 
     */
    void fix_insert(std::shared_ptr<Node<Key_t>>& node) {

        while (node != root && node->parent_->color_ == Color::red) {

            if (node->parent_ && node->parent_->parent_) {

                if (node->parent_ == node->parent_->parent_->left_) {
                    auto uncle = node->parent_->parent_->right_;

                    if (uncle && uncle->color_ == Color::red) {
                        node->parent_->color_ = Color::black;
                        uncle->color_ = Color::black;
                        node->parent_->parent_->color_ = Color::red;
                        node = node->parent_->parent_;
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
                        node->parent_->color_ = Color::black;
                        uncle->color_ = Color::black;
                        node->parent_->parent_->color_ = Color::red;
                        node = node->parent_->parent_;
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
        }
        root->color_ = Color::black;
    }

    void left_rotate(const std::shared_ptr<Node<Key_t>> node) {

        auto y = node->right_;
        node->right_ = y->left_;

        if (y->left_) 
            y->left_->parent_ = node;
        y->parent_ = node->parent_;

        if (!node->parent_) 
            root = y;
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
            root = y;
        else if (node == node->parent_->right_) 
            node->parent_->right_ = y;
        else 
            node->parent_->left_ = y;
        y->right_ = node;
        node->parent_ = y;
    }

public:
    Red_black_tree(Key_t key) {
        root = std::make_shared<Node<Key_t>>(key);
        root->color_ = Color::black;
    }

    void insert_elem(Key_t key) {

        auto new_node = std::make_shared<Node<Key_t>>(key);
        auto current = root;
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
            root = new_node;
        else if (key < parent->key_) 
            parent->left_ = new_node;
        else 
            parent->right_ = new_node;

        new_node->color_ = Color::red;
        fix_insert(new_node);
    }

    uint64_t search(Key_t key1, Key_t key2) const { 

        uint64_t counter = 0;

        search_(root, counter, key1, key2);

        return counter;
    }

private:

    void search_(const std::shared_ptr<Node<Key_t>>& node, uint64_t& counter, Key_t key1, Key_t key2) const {

        if (node->key_ >= key1 && node->key_ <= key2) {
            counter++;
            if (node->left_)
                search_(node->left_, counter, key1, key2);
            if (node->right_)
            search_(node->right_, counter, key1, key2);
        }
        else if (node->key_ < key1 && node->right_) {
            search_(node->right_, counter, key1, key2);
        }
        else if (node->key_ > key2 && node->left_) {
            search_(node->left_, counter, key1, key2);
        }
        return;
    }

    std::shared_ptr<Node<Key_t>> lower_bound(Key_t key) const;
    std::shared_ptr<Node<Key_t>> upper_bound(Key_t key) const; 

    void delete_node() {

    }

public:

    static void create_graph_node(Node<Key_t>& node, std::ofstream& file_name) {

        if (node.left_) {
            file_name << node.key_ << " [shape = Mrecord, style = filled, fillcolor = " << (static_cast<int>(node.color_) == 0 ? "DarkRed" : "black") << ", label = \"" <<  node.key_ << "\" ];\n"
                      << node.left_->key_ << " [shape = Mrecord, style = filled, fillcolor = " << (static_cast<int>(node.left_->color_) == 0 ? "DarkRed" : "black") << ", label = \"" << node.left_->key_ << "\" ];\n"
                      << node.key_ << " -> " << node.left_->key_ << ";\n";
            create_graph_node(*node.left_, file_name);
        }
        if (node.right_) {
            file_name << node.key_ << " [shape = Mrecord, style = filled, fillcolor = " << (static_cast<int>(node.color_) == 0 ? "DarkRed" : "black") << ", label = \"" <<  node.key_ << "\" ];\n"
                      << node.right_->key_ << " [shape = Mrecord, style = filled, fillcolor = " << (static_cast<int>(node.right_->color_) == 0 ? "DarkRed" : "black") << ", label = \"" << node.right_->key_ << "\" ];\n"
                      << node.key_ << " -> " << node.right_->key_ << ";\n";
            create_graph_node(*node.right_, file_name);
        }
    }
};
}
