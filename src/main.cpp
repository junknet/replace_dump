#include "jfile.h"
#include <iostream>
using namespace std;

int main(int argc, char **argv) {
tes
  if (argc < 3) {
    printf("./replace_dump raw dump offset\n");
    return -1;
  }
  JTimer timer;
  uint64_t offset;
  sscanf(argv[3], "%ld", &offset);
  JFile file_1(argv[1]);
  JFile file_2(argv[2]);

  if (file_1.size() - offset < file_2.size()) {
    printf("file_1.size() - offset < file_2.size()");
    return -1;
  }
  auto p1 = file_1.map();
  auto p2 = file_2.map();

  memcpy(p1 + offset, p2, file_2.size());
  file_1.unmap();
  file_2.unmap();
  timer.speed();
  return 0;
}
