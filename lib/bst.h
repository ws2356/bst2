#include <iostream>

#define IS_2(n) (n && !n ->red() && (!n ->left || !n ->left ->red()) && (!n ->right || !n ->right ->red()))
#define IS_3(n) (n && !n ->red() && n ->left && n ->left ->red() && (!n ->right || !n ->right ->red()))
#define IS_4(n) (n && !n ->red() && n ->left && n ->left ->red() && n ->right && n ->right ->red())
#define IS_RED(n) (n && n ->red())

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

  Node<T>* remove (T val) {
    if (!root) {
      return NULL;
    }
    auto left = root ->left;
    auto right = root ->right;
    if (!left && !right) {
      if (root ->val = val) {
        auto ret = root;
        root = NULL;
        return ret;
      } else {
        return NULL;
      }
    }
    // root及左右都是2节点，变成4节点
    if (!IS_3(root) && !IS_3(left) && !IS_3(right)) {
      left ->is_red = true;
      right ->is_red = true;
    }

    auto res_rm = remove_imp(root, val);
    root = res_rm.first;
    if (root) {
      root ->is_red = false;
    }
    return res_rm.second;
  }

  Node<T>* remove_min () {
    if (!root) {
      return NULL;
    }
    auto left = root ->left;
    auto right = root ->right;
    if (!left && !right) {
      auto ret = root;
      root = NULL;
      return ret;
    }

    // root及左右都是2节点，变成4节点
    if (!IS_3(root) && !IS_3(left) && !IS_3(right)) {
      left ->is_red = true;
      right ->is_red = true;
    }
    auto res_rm = remove_min_imp(root);
    root = res_rm.first;
    if (root) {
      root ->is_red = false;
    }
    return res_rm.second;
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

private:
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

private:
  std::pair<Node<T> *, Node<T> *> remove_min_imp(Node<T> *node) {
    auto left = node ->left;
    auto right = node ->right;
    auto left_child = IS_2(node) ? left : left ->left;
    auto left_sibling = IS_2(node) ? right : left ->right;
    if (!left_child) {
      // 叶节点（3或4）
      auto was_4 = IS_4(node);
      node ->left = NULL;
      auto ret = was_4 ? rotate_left(node) : node;
      return std::make_pair(ret, left);
    } else if (!IS_2(left_child)) {
      ; // noop
    } else if (!IS_2(left_sibling)) {
      // 从右边借
      assert(IS_3(left_sibling));
      auto from_sibling = left_sibling ->left;
      auto shift_left = IS_2(node) ? node : left;
      from_sibling ->is_red = shift_left ->is_red;
      left_sibling ->left = from_sibling ->right;
      from_sibling ->right = left_sibling;
      shift_left ->right = from_sibling ->left;
      from_sibling ->left = shift_left;
      left_child ->is_red = true;
      if (IS_2(node)) {
        node = from_sibling;
      } else {
        node ->left = from_sibling;
      }
    } else {
      // 合并4节点
      left ->is_red = false;
      left ->left ->is_red = true;
      left ->right ->is_red = true;
      if (IS_4(node)) {
        node = rotate_left(node);
      }
    }
    // 结构改变后更新相关变量
    left = node ->left;
    right = node ->right;
    left_child = IS_2(node) ? left : left ->left;

    auto ret_rm = remove_min_imp(left_child);
    if (IS_2(node)) {
      node ->left = ret_rm.first;
    } else {
      left ->left = ret_rm.first;
    }

    auto rm = ret_rm.second;
    if (!IS_RED(node ->left) && IS_RED(node ->right)) {
      node = rotate_left(node);
    }
    if (IS_RED(node ->left) && node ->left && IS_RED(node ->left ->left)) {
      node = rotate_right(node);
    }
    if (IS_RED(node ->left) && IS_RED(node ->right)) {
      node = flip_color(node);
    }

    return std::make_pair(node, rm);
  }

  std::pair<Node<T> *, Node<T> *> remove_imp(Node<T> *node, T val) {
  }
};