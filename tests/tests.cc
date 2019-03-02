#include <iostream>
#include <string>
#include <bst.h>

using std::string;

int main (int argc, char **argv)  {
  auto tree = new Bst<int>();
  tree ->insert(1);

  auto node = tree ->search(1);
  if (!node || node ->val != 1) {
    return 1;
  }

  auto rm = tree ->remove(1);
  if (!rm || rm ->val != 1) {
    return 1;
  }
  delete rm;
  
  const int N = 4;
  for (auto i = 1; i <= N; i++) {
    println("inserting: ");
    tree ->insert(i);
  }
  tree ->print();

  for (auto i = 1; i <= N; i++) {
    println(string("removing: ") + std::to_string(i));
    auto rm = tree ->remove(i);
    if (rm && rm ->val == i) {
      tree ->print();
    } else {
      if (rm) {
        println(string("remove failed, really is: ") + std::to_string(i));
      } else {
        println("remove failed, NULL");
      }
      return 1;
    }
    auto rm_val = rm ->val;
    delete rm;
    if (rm_val != i) {
      return 1;
    }
  }

  delete tree;
  return 0;
}