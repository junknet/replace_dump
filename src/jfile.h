#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

#include <chrono>
using namespace std;
using namespace chrono;

class JFile {
public:
  JFile(const char *file_path);
  uint8_t *map();
  void unmap();
  // ssize_t write(uint8_t *data, size_t size);
  // ssize_t write(const char *data);
  // ssize_t write(string &str);
  vector<string> readlines();
  size_t size();
  char *filePath();

public:
  ~JFile();

private:
  int fd = 0;
  size_t file_size;
  char *file_path;
  uint8_t *map_addr;
};

inline JFile::JFile(const char *filePath) {

  file_path = (char *)malloc(strlen(filePath) + 1);
  strcpy(file_path, filePath);
  struct stat st;
  stat(file_path, &st);
  file_size = st.st_size;
}

inline uint8_t *JFile::map() {
  if (!fd) {
    if ((fd = open(file_path, O_RDWR, 00777)) < 0) {
      perror("open\n");
    }
  }
  map_addr = (uint8_t *)mmap(NULL, file_size, PROT_READ | PROT_WRITE,
                             MAP_SHARED, fd, 0);
  if (map_addr == MAP_FAILED) {
    printf("mmap failed! err: %s  file_path: %s", strerror(errno), file_path);
  }
  return map_addr;
}

inline void JFile::unmap() { munmap(map_addr, file_size); }

inline vector<string> JFile::readlines() {
  vector<string> lines;
  ifstream file(file_path);
  if (file.is_open()) {
    string line;
    while (getline(file, line)) {
      lines.push_back(line);
    }
  }
  return lines;
}

inline size_t JFile::size() { return file_size; }

inline char *JFile::filePath() { return file_path; }

inline JFile::~JFile() { free(file_path); }

class JTimer {

public:
  JTimer() { start = high_resolution_clock::now(); }

  void speed() {
    end = high_resolution_clock::now();
    std::chrono::duration<float> duration;
    duration = end - start;
    float ms = duration.count() * 1000.0f;
    std::cout << "JTimer speed " << ms << " ms"
              << "\n";
  };

private:
  time_point<std::chrono::system_clock, duration<long, ratio<1, 1000000000>>>
      start, end;
};