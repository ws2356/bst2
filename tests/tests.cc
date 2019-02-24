#include <bst.h>

int main (int argc, char **argv)  {
  auto tree = new Bst<int>();
  tree ->insert(1);

  auto node = tree ->search(1);
  if (!node || node ->val != 1) {
    return 1;
  }

  tree ->remove(1);
  delete tree;
  return 0;
}