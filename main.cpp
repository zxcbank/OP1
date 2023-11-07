#include "functions.h"

int main(int argc, char **argv) {
  Params current;
  current  = Parsing(argc, argv);
  PrintFile(current);
  return 0;
}