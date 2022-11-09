#include <fstream>
#include <vector>
#include <string>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>


int main() {
  std::vector<std::string> url_list;
  std::ifstream file("urls.txt");
  std::string url;

  while (!file.eof()) {
    file >> url;
    url_list.push_back(url);
  }

  int url_count = 0;
  std::string filePrefix = "log";

  while (url_count < url_list.size() - 1) {
  pid_t childPid = fork();

// error check
  if (childPid < 0) {
    perror("fork");
    exit(-1);
  }

// child process
  if (childPid == 0) {
    std::string filename = filePrefix + std::to_string(url_count + 1) + ".txt";
    if(execlp("/usr/bin/wget", "wget", url_list[url_count].c_str(), "-o", filename.c_str(), nullptr) < 0) {
      perror("execlp");
      exit(-1);
    }

// parent process
  } else {
    if (wait(nullptr) < 0) {
      perror("wait");
      exit(-1);
    }
  }
  url_count++;
}
return 0;
}
