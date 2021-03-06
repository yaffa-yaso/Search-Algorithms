//
// Created by aharon on 24/01/2020.
//

#ifndef EX4__MYCLAIENTHANDLER_H_
#define EX4__MYCLAIENTHANDLER_H_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <chrono>
#include <thread>
#include <mutex>
#include <cstddef>
#include <stdlib.h>
#include <string>
#include <functional>
#include "MyClientHandler.h"
#include "Solver.h"
#include "ClientHandler.h"
#include "Matrix.h"
#include <algorithm>

typedef vector<vector<int>> matrix;

class MyClientHandler : public ClientHandler {

 private:
  Solver<Matrix, vector<string>> *solver;
  CacheManager<string, vector<string>> *cache;

 public:
  MyClientHandler() {}
  MyClientHandler *clone() const {
    return new MyClientHandler(*this);
  }
  MyClientHandler *create() const {
    return new MyClientHandler();
  }

  MyClientHandler(Solver<Matrix, vector<string>> *sol, CacheManager<string, vector<string>> *cacheManager)
      : solver(sol),
        cache(cacheManager) {};
  void handleClient(int socket) {

    matrix problem;
    vector<string> solution;
    string message = " ";
    string strProblem = " ";
    string tempToken = "";
    bool check = true;
    string tempStr;
    int colCounter = 0;
    unsigned int rowCounter = 0;
    while (check) {
      char buffer[1024] = {0};
      read(socket, buffer, 1024);
      string copy(buffer);
      if (copy.find("end")!=std::string::npos) {
        check = false;
      }
      tempStr = tempStr + copy;
    }
    char *temp12 = const_cast<char *>(tempStr.c_str());
    char *tempBuffer = strtok(temp12, "\n");
    while (tempBuffer!=NULL) {
      char delimiter = ',';
      int m = 0;
      int j = 0;
      if (!strcmp(tempBuffer, "end")) {
        tempBuffer = NULL;
        continue;
      } else {
        string str = tempBuffer;
        str = str + "\n";
        int i = 0;
        for (i; str[m - 1]!='\n'; i++) {
          vector<int> v;
          // find next value
          while ((str[j]!=delimiter) && (str[j]!='\n')) {
            j++;
          }
          // separate value from the rest of the string
          string token = "";
          while (m < j) {
            token += str[m];
            m++;
          }
          m++;
          if (colCounter==0 || i > colCounter) {
            while (v.size() < rowCounter) {
              v.push_back(-1);
            }
            v.push_back(atoi(token.c_str()));
            problem.push_back(v);
          } else {
            problem[i].push_back(atoi(token.c_str()));
          }
          j++;
          tempToken += token + " ";
          if (i > colCounter) {
            colCounter = i;
          }
        }
        rowCounter++;
//          cout << strProblem << endl;
        strProblem = strProblem + tempToken;
        tempToken = "";
      }
      tempBuffer = strtok(NULL, "\n");
    }
    for (unsigned int q = rowCounter - 2; q < problem[0].size(); q++) {
      for (unsigned int p = 2; p < problem.size(); p++) {
        problem[p].push_back(-1);
      }
    }

    for (int q = 0; q < problem[0].size(); q++) {
    }
    //searching for solution in the cache
    if (this->cache->inCache(strProblem)) {
      solution = this->cache->getSolution(strProblem);
      //if solution wasn't found create one
    } else {
      solution = this->solver->solve(Matrix(problem));
      this->cache->intoCache(strProblem, solution);
    }

    for (unsigned int i = 0; i < solution.size(); i++) {
      message = message + solution[i] + " ";
    }

    write(socket, message.c_str(), message.length());
  }
  ~MyClientHandler() {}
};
#endif //EX4__MYCLAIENTHANDLER_H_
