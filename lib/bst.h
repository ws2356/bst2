#include <iostream>

// TODO: wansong, 封装
template <typename T>
struct Node {
  Node(T val_, bool is_red_, Node<T> *left_, Node<T> *right_):
    val(val_), is_red(is_red_), left(left_), right(right_) {}

  bool red() { return is_red; }

  T val;
  bool is_red;
  Node *left;
  Node *right;
};

template <typename T>
class Bst {
public:
  Bst(): root(NULL) {}

  // TODO: wansong, 使用迭代器
  Node<T> * search (T val) {
    return search_imp(val, root);
  }

  void insert (T val) {
    root = insert_imp(val, root);
    root ->is_red = false;
  }

  void remove (T val) {
    std::cout << "remove: todo" << std::endl;
  }

private:
  Node<T> *root;

private:
  Node<T> * search_imp (T val, Node<T> *node) {
    if (!node) {
      return NULL;
    }
    if (node ->val == val) {
      return node;
    } else if (node ->val < val) {
      return search_imp(val, node ->right);
    } else {
      return search_imp(val, node ->left);
    }
  }

  Node<T> * insert_imp (T val, Node<T> *node) {
    if (!node) {
      return new Node<T>(val, true, NULL, NULL);
    }

    if (node ->val == val) {
      return node;
    } else if (node ->val < val) {
      node ->right = insert_imp(val, node ->right);
    } else {
      node ->left = insert_imp(val, node ->left);
    }

    auto ret = node;
    if (node ->right && node ->right ->red() && (!node ->left || !node ->left ->red())) {
      ret = rotate_left(node);
    }

    if (node ->left && node ->left ->red() && node ->left ->left && node ->left ->left ->red()) {
      ret = rotate_right(node);
    }

    if (node ->right && node ->right ->red() && node ->left && node ->left ->red()) {
      ret = flip_color(node);
    }

    return ret;
  }

  static Node<T> *rotate_left(Node<T> *node) {
    assert(node ->right ->red());
    assert(!node ->left || !node ->left ->red());
    auto right = node ->right;
    node ->right = right ->left;
    right ->left = node;
    right ->is_red = node ->is_red;
    node ->is_red = true;
    return right;
  }

  static Node<T> *rotate_right(Node<T> *node) {
    assert(node ->left ->red());
    assert(node -> left ->left ->red());
    auto left = node ->left;
    node ->left = left ->right;
    left ->right = node;
    left ->is_red = node ->is_red;
    node ->is_red = true;
    return left;
  }

  static Node<T> *flip_color(Node<T> *node) {
    assert(node ->left ->red());
    assert(node ->right ->red());
    assert(!node ->red());
    node ->left ->is_red = false;
    node ->right ->is_red = false;
    node ->is_red = true;
    return node;
  }
};