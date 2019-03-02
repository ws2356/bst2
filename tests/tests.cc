#include <iostream>
#include <string>
#include <bst.h>

using std::string;

int main (int argc, char **argv)  {
  auto tree = new Bst<int>();
  
  const int N = 1000000;
  // 10,000,000 takes 30 seconds on 2013 macbook air
  for (auto i = 1; i <= N; i++) {
    tree ->insert(i);
  }
//  tree ->print(tree ->root);

  for (auto i = 1; i <= N; i++) {
    // println(string("removing: ") + std::to_string(i));
    auto rm = tree ->remove(i);
    if (rm && rm ->val == i) {
//      tree ->print(tree ->root);
      delete rm;
    } else {
      if (rm) {
        println(string("remove failed, really is: ") + std::to_string(i));
      } else {
        println("remove failed, NULL");
      }
      return 1;
    }

    // real slow
//    if (!Bst<int>::verify(tree ->root)) {
//      return 1;
//    }
  }

  delete tree;
  return 0;
}
