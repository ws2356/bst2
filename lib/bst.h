#include <iostream>

#define IS_2(n) (n && !n ->red() && (!n ->left || !n ->left ->red()) && (!n ->right || !n ->right ->red()))
#define IS_3(n) (n && !n ->red() && n ->left && n ->left ->red() && (!n ->right || !n ->right ->red()))
#define IS_4(n) (n && !n ->red() && n ->left && n ->left ->red() && n ->right && n ->right ->red())
#define IS_RED(n) (n && n ->red())
#define IS_LEAF(n) (!n ->right || (n ->left ->red() && n ->right ->red() && !n ->left ->left))

// FIXME: 封装
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

  // FIXME: 使用迭代器
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

    if (IS_2(root)) {
      auto left = root ->left;
      auto right = root ->right;
      if (root ->val > val) {
        if (IS_2(left)) {
          root = borrow_from_right(root);
          left = root ->left;
          right = root ->right;
        }
        auto res_rm = remove_imp(root ->left, val);
        root ->left = res_rm.first
        return res_rm.second
      } else if (root ->val < val) {
        if (IS_2(right)) {
          root = borrow_from_left(root);
          left = root ->left;
          right = root ->right;
        }
        auto res_rm = remove_imp(right, val);
        root ->right = res_rm.first
        return ret_rm.second
      } else {
        if (IS_2(left)) {
          root = borrow_from_right(root);
          auto res_rm = remove_imp(root ->left, val);
          root ->left = res_rm.first;
          return res_rm.second;
        } else {
          root = borrow_from_left(root);
          auto res_rm = remove_imp(root ->right, val);
          root ->right = res_rm.first;
          return res_rm.second;
        }
      }
    }

    auto res_rm = remove_imp(root, val);
    root = res_rm.first;
    root ->is_red = false;
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
    if (IS_RED(node ->right) && !IS_RED(node ->left)) {
      ret = rotate_left(node);
    }
    if (IS_RED(node ->left) && IS_RED(node ->left ->left)) {
      ret = rotate_right(node);
    }
    if (IS_RED(node ->left) && IS_RED(node ->right)) {
      ret = flip_color(node);
    }
    return ret;
  }

private:
  static Node<T> *rotate_left(Node<T> *node) {
    if(!node ->right ->red()) {
      return node;
    }
    auto right = node ->right;
    node ->right = right ->left;
    right ->left = node;
    right ->is_red = node ->is_red;
    node ->is_red = true;
    return right;
  }

  static Node<T> *rotate_right(Node<T> *node) {
    if(!node ->left ->red()) {
      return node;
    }
    // assert(node -> left ->left ->red());
    auto left = node ->left;
    node ->left = left ->right;
    left ->right = node;
    left ->is_red = node ->is_red;
    node ->is_red = true;
    return left;
  }

  static Node<T> *flip_color(Node<T> *node) {
    if (!node ->left ->red()) {
      return node;
    }
    if(!node ->right ->red()) {
      return node;
    }
    if(!node ->red()) {
      return node;
    }
    node ->left ->is_red = false;
    node ->right ->is_red = false;
    node ->is_red = true;
    return node;
  }

  static Node<T> *borrow_from_left(Node<T> *node) {
    assert(node);
    if (IS_3(node)) {
      auto borrowed_parent = node ->left;
      auto borrowed = borrowed_parent ->right;
      auto borrowed_remain = borrowed ->left;
      auto borrower = node ->right;
      auto borrower_parent = node;
      borrowed_remain ->is_red = false;
      borrowed ->is_red = borrower_parent ->is_red;
      borrower_parent ->is_red = true;
      borrowed_parent ->right = borrowed_remain;
      borrowed ->left = borrowed_parent;
      borrower_parent ->left = borrowed ->right;
      borrowed ->right = borrower;
      borrower_parent ->right = borrower ->left;
      borrower ->left = borrower_parent;
      return borrowed;
    } else {
      auto parent = node;
      auto borrowed = node ->left;
      auto borrowed_remain = borrowed ->left;
      auto borrower = node ->right;
      borrowed ->is_red = parent ->is_red;
      borrowed_remain ->is_red = false;
      parent ->is_red = true;
      parent ->left = borrowed ->right;
      borrowed ->right = borrower;
      parent ->right = borrower ->left;
      borrower ->left = parent;
      return borrowed;
    }
  }

  static Node<T> *borrow_from_right(Node<T> *node) {
    if (IS_3(node)) {
      assert(false, "no need");
    } else {
      auto parent = node;
      auto borrowed = node ->right ->left;
      auto borrowed_remain = node ->right;
      auto borrower = node ->left;
      borrower >is_red = true;
      borrowed ->is_red = parent ->is_red;
      parent ->is_red = false;
      parent ->right = borrowed ->left;
      borrowed ->left = parent;
      borrowed_remain ->left = borrowed ->right;
      borrowed ->right = borrowed_remain;
      return borrowed;
    }
  }

  static Node<T> *swap_with_next(Node<T> *node) {
    auto ret_parent = node ->right;
    while (ret_parent ->left && ret_parent ->left ->left) {
      ret_parent = ret_parent ->left;
    }
    if (!ret_parent ->left) {
      auto ret = ret_parent;
      ret ->left = node ->left;
      ret ->right = node;
      ret ->is_red = node ->is_red;
      node ->left = NULL;
      node ->right = NULL;
      return ret;
    }

    auto ret = ret_parent ->left;
    ret ->left = node ->left;
    ret ->is_red = node ->is_red;
    ret ->right = node ->right;
    ret_parent ->left = node;
    node ->left = NULL;
    node ->right = NULL;
    return ret;
  }

  static Node<T> *form_4node (Node<T> *node, Node<T> *node2) {
    if(IS_3(node)) {
      if (node == node2) {
        ret = node ->left;
        ret ->is_red = node ->is_red;
        node ->is_red = false;
        node ->left = ret ->right;
        ret ->right = node;
        node ->left ->is_red = true;
        node ->right ->is_red = true;
        return ret;
      } else {
        node2 ->is_red = false;
        node2 ->left ->is_red = true;
        node2 ->right ->is_red = true;
        return node;
      }
    } else if (IS_4(node)) {
      assert(node != node2);
      node2 ->is_red = false;
      node2 ->left ->is_red = true;
      node2 ->right ->is_red = true;
      return rotate_left(node);
    } else {
      assert(false);
    }
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
    if (IS_RED(node ->left) && IS_RED(node ->left ->left)) {
      node = rotate_right(node);
    }
    if (IS_RED(node ->left) && IS_RED(node ->right)) {
      node = flip_color(node);
    }

    return std::make_pair(node, rm);
  }

  std::pair<Node<T> *, Node<T> *> remove_imp(Node<T> *node, T val) {
    auto left = node ->left;
    auto right = node ->right;
    auto res = node;
    auto rm = NULL;
    if (IS_LEAF(node)) {
      if (node ->val == val) {
        if (right) {
          res = right;
          res ->left = left;
          res ->is_red = false;
          rm = node;
          rm ->left = NULL;
          rm ->right = NULL;
        } else {
          res = left;
          res ->is_red = false;
          rm = node;
          rm ->left = NULL;
        }
      } else if (right && right ->val == val) {
        node ->right = NULL;
        rm = right;
      } else if (left ->val == val) {
        rm = left;
        node ->left = NULL;
      }
    } else {
      if (node ->val > val) {
        if (left ->val == val) {
          if (IS_2(left ->left) && IS_2(left ->right)) {
            form_4node(node, left);
            auto res_rm = remove_imp(node ->left, val);
            node ->left = res_rm.first;
            rm = res_rm.second;
          } else if (IS_2(left ->right)) {
              left = node ->left = borrow_from_left(left);
              auto res_rm = remove_imp(left ->right, val);
              left ->right = res_rm.first;
              rm = res_rm.second;
          } else {
            left = node ->left = swap_with_next(left);
            auto res_rm = remove_min_imp(left ->right);
            left ->right = res_rm.first;
            rm = res_rm.second;
          }
        } else if (IS_2(left ->left) && IS_2(left ->right)) {
          form_4node(node, left);
          auto res_rm = remove_imp(left, val);
          node ->left = res_rm.first;
          rm = res_rm.second;
        } else {
          if (left ->val > val) {
            if (IS_2(left ->left)) {
              left = node ->left = borrow_from_right(left);
            }
            auto res_rm = remove_imp(left ->left, val);
            left ->left = res_rm.first;
            rm = res_rm.second;
          } else {
            if (IS_2(left ->right)) {
              left = node ->left = borrow_from_left(left);
            }
            auto res_rm = remove_imp(left ->right, val);
            left ->right = res_rm.first;
            rm = res_rm.second;
          }
        }
      } else if (node ->val < val) {
        if (IS_4(node)) {
          if (right ->val == val) {
            if (IS_2(right ->left) && IS_2(right ->right)) {
              form_4node(node, right);
              auto res_rm = remove_imp(right, val);
              node ->right = res_rm.first;
              rm = res_rm.second;
            } else if (IS_2(right ->right)) {
              right = node ->right = borrow_from_left(right);
              auto res_rm = remove_imp(right ->right, val);
              right ->right = res_rm.first;
              rm = res_rm.second;
            } else {
              right = node ->right = swap_with_next(right);
              auto res_rm = remove_min_imp(right ->right);
              right ->right = res_rm.first;
              rm = res_rm.second;
            }
          } else if (IS_2(right ->left) && IS_2(right ->right)) {
            form_4node(node, right);
            auto res_rm = remove_imp(right, val);
            node ->right = res_rm.first;
            rm = res_rm.second;
          } else if (right ->val < val){
            if (IS_2(right ->right)) {
              right = node ->right = borrow_from_left(right);
            }
            auto res_rm = remove_imp(right ->right, val);
            right ->right = res.first;
            rm = res_rm.second;
          } else {
            if (IS_2(right ->left)) {
              right = node ->right = borrow_from_right(right);
            }
            auto res_rm = remove_imp(right ->left, val);
            right ->left = res_rm.first;
            rm = res_rm.second;
          }
        } else {
          if (IS_2(right) && IS_2(left ->right)) {
            res = node = form_4node(node, node);
            auto res_rm = remove_imp(node ->right, val);
            node ->right = res_rm.first;
            rm = res_rm.second;
          } else {
            if (IS_2(right)) {
              res = node = borrow_from_left(node);
            }
            auto res_rm = remove_imp(node ->right, val);
            node ->right = res_rm.first;
            rm = res_rm.second;
          }
        }
      } else {
        if (IS_3(node)) {
          if (!IS_2(right)) {
            res = node = swap_with_next(node);
            auto res_rm = remove_min_imp(node ->right);
            node ->right = res_rm.first;
            rm = res_rm.second;
          } else {
            if (IS_2(left ->right) && IS_2(right)) {
              res = node = form_4node(node, node);
            } else if (IS_2(right)) {
              res = node = borrow_from_left(node);
            }
            auto res_rm = remove_imp(node ->right, val);
            node ->right = res_rm.first;
            rm = res_rm.second;
          }
        } else {
          if (IS_2(right ->left) && IS_2(right ->right)) {
            form_4node(node, right);
            res = node = swap_with_next(node);
            auto res_rm = remove_min_imp(node ->right);
            node ->right = res_rm.first;
            rm = res_rm.second;
          } else {
            if (IS_2(right ->left)) {
              right = node ->right = borrow_from_right(right);
            }
            res = node = swap_with_next(node);
            right = node ->right;
            auto res_rm = remove_min_imp(right ->left);
            right ->left = res_rm.first;
            rm = res_rm.second;
          }
        }
      }
    }
    // 旋转变色
    if (!IS_RED(res ->left) && IS_RED(res ->right)) {
      res = rotate_left(res)
    }
    if (IS_RED(res ->left) && IS_RED(res ->left ->left)) {
      res = rotate_right(res)
    }
    if (IS_RED(res ->left) && IS_RED(res ->right)) {
      res = flip_color(res)
    }
    return std::make_pair(res, rm);
  }
};