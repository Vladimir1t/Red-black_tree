#include <iostream>
#include <memory>
#include <fstream>

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

    Color color;
    Key_t key_;

    Node(Key_t key) : key_(key), color(Color::red) {}
    Node() = default;

    Node(const Node&& rhs) { // move ctor
        key_ = rhs.key_;
        color = rhs.color;
    }

    Node& operator=(const Node& rhs) {  // оператор присваивания
        if (this == &rhs) {
            return *this;
        }
        key_    = rhs.key_;
        color   = rhs.color;
        parent_ = rhs.parent_;
        left_   = rhs.left_;
        right_  = rhs.right_;
        return *this;
    }
    void operator%(const Node& rhs) {
        std::cout << "operator %\n";
    }
};

/** @brief Red_black_tree - a class of spaces and methods of red-black tree.
 * Invariant - search, insert, delete time is O(logN)
 */
template <typename Key_t>
class Red_black_tree {

public:

    std::shared_ptr<Node<Key_t>> root_ = nullptr;

private:

    /** @brief method that sustains the balace invariant 
     *  @param node - new node, that was inserted 
     */
    void fix_insert(std::shared_ptr<Node<Key_t>> node) {

        while (node != root_ && node->parent_->color == Color::red) {

            if (node->parent_ && node->parent_->parent_) {

                if (node->parent_ == node->parent_->parent_->left_) {
                    auto uncle = node->parent_->parent_->right_;

                    if (uncle && uncle->color == Color::red) {
                        node->parent_->color = Color::black;
                        uncle->color = Color::black;
                        node->parent_->parent_->color = Color::red;
                        node = node->parent_->parent_;
                    }
                    else {
                        if (node == node->parent_->right_) {
                            node = node->parent_;
                            left_rotate(node);
                        }
                        node->parent_->color = Color::black;
                        node->parent_->parent_->color = Color::red;
                        right_rotate(node->parent_->parent_);
                    }
                } 
                else {
                    auto uncle = node->parent_->parent_->left_;

                    if (uncle && uncle->color == Color::red) {
                        node->parent_->color = Color::black;
                        uncle->color = Color::black;
                        node->parent_->parent_->color = Color::red;
                        node = node->parent_->parent_;
                    } 
                    else {
                        if (node == node->parent_->left_) {
                            node = node->parent_;
                            right_rotate(node);
                        }
                        node->parent_->color = Color::black;
                        node->parent_->parent_->color = Color::red;
                        left_rotate(node->parent_->parent_);
                    }
                }
            }
        }
        root_->color = Color::black;
    }

    void left_rotate(std::shared_ptr<Node<Key_t>> node) {
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

    void right_rotate(std::shared_ptr<Node<Key_t>> node) {
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
        root_ = std::make_shared<Node<Key_t>>(key);
        root_->color = Color::black;
    }

    void insert_elem(Key_t key) {

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

        new_node->color = Color::red;
        fix_insert(new_node);
    }

    bool search(Key_t key1, Key_t key2) {

        uint64_t counter = 0;

        //std::shared_ptr<Node<Key_t>> node = root_;

        search_(root_, counter, key1, key2);

        // while (node) { // для key1

        //     if (key1 < *node) {
        //         node = node->right_
        //         counter++;
        //     }
        // }
        std::cout << "counter = " << counter << std::endl;

        return true;
    }

    void search_(std::shared_ptr<Node<Key_t>> node, uint64_t& counter, Key_t key1, Key_t key2) {

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

    void delete_node() {

    }

    void print(Node<Key_t>& node, std::ofstream& file_name) {
       // std::cout << " { " << node.key_ << " color: " << static_cast<int>(node.color);

        if (node.left_) {
            file_name << node.key_ << " [shape = Mrecord, style = filled, fillcolor = " << (static_cast<int>(node.color) == 0 ? "DarkRed" : "black") << ", label = \"" <<  node.key_ << "\" ];\n"
                      << node.left_->key_ << " [shape = Mrecord, style = filled, fillcolor = " << (static_cast<int>(node.left_->color) == 0 ? "DarkRed" : "black") << ", label = \"" << node.left_->key_ << "\" ];\n"
                      << node.key_ << " -> " << node.left_->key_ << ";\n";
            print(*node.left_, file_name);
        }
        if (node.right_) {
            file_name << node.key_ << " [shape = Mrecord, style = filled, fillcolor = " << (static_cast<int>(node.color) == 0 ? "DarkRed" : "black") << ", label = \"" <<  node.key_ << "\" ];\n"
                      << node.right_->key_ << " [shape = Mrecord, style = filled, fillcolor = " << (static_cast<int>(node.right_->color) == 0 ? "DarkRed" : "black") << ", label = \"" << node.right_->key_ << "\" ];\n"
                      << node.key_ << " -> " << node.right_->key_ << ";\n";
            print(*node.right_, file_name);
        }
        //std::cout << " }";
    }

};


template <typename Key_t>
class binary_tree {

    Node<Key_t> root_;
    void print(Node<Key_t>& node) {
        std::cout << " { " << node.key_;

        if (node.left_) {
            print(*node.left_);
        }
        if (node.right_) {
            print(*node.right_);
        }
        std::cout << " }";
    }


};
}
