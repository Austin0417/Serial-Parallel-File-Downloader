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
  pid_t childPid;
  std::string filePrefix = "log";

  while (!file.eof()) {
    file >> url;
    url_list.push_back(url);
  }

  for (int i = 0; i < url_list.size() - 1; i++) {
    // parent process calls fork() n times
    childPid = fork();

    // error check for fork()
    if (childPid < 0) {
      perror("fork");
      exit(-1);
    }

    // child process
    if (childPid == 0) {
      std::string filename = filePrefix + std::to_string(i + 1) + ".txt";
      if (execlp("/usr/bin/wget", "wget", url_list[i].c_str(), "-o", filename.c_str(), nullptr) < 0) {
        perror("execlp");
        exit(-1);
      }
    }
  }

  // parent process calls wait() n times
  for (int j = 0; j < url_list.size() - 1; j++) {
    if (wait(nullptr) < 0) {
      perror("wait");
      exit(-1);
    }
  }
  return 0;
}
