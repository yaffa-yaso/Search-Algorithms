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
  MyClientHandler(Solver<Matrix, vector<string>> *sol, CacheManager<string, vector<string>> *cacheManager)
      : solver(sol),
        cache(cacheManager) {};
  void handleClient(int socket) {

    char buffer[1024] = {0};
    matrix problem;
    vector<string> solution;
    string message = " ";
    string strProblem = " ";
    int colRow = 3;
    int lineNum = 0;
    bool isN = false;
    int countStr = 0;
    bool check = true;
    string tempStr;
    int maxLine = 0;
    int sizeCounter = 0;
    while (check) {
      read(socket, buffer, 1024);
      string copy(buffer);
      if (copy.find("end")!=std::string::npos) {
        check = false;
      }
      tempStr = tempStr + copy;
    }
    char *temp12 = const_cast<char *>(tempStr.c_str());
    char *tempBuffer = strtok(temp12, "\n");
    while (tempBuffer!=NULL && (colRow > 1)) {
      char delimiter = ',';
      int m = 0;
      int j = 0;
      sizeCounter = 0;
      if (!strcmp(tempBuffer, "end")) {
        colRow--;
        continue;
      } else {
        if (colRow < 3) {
          colRow--;
        }
        string str = tempBuffer;
        str = str + '\n';
        for (int i = 0; str[m - 1]!='\n'; i++) {
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
          if (lineNum==0) {
            v.push_back(atoi(token.c_str()));
            problem.push_back(v);
          } else {
            problem[i].push_back(atoi(token.c_str()));
          }
          j++;
          strProblem += token + " ";
          sizeCounter++;
        }
        cout << strProblem << endl;
        strProblem = "";
      }
      lineNum++;
      tempBuffer = strtok(NULL, "\n");
    }
    cout << "yyyyy" << endl;
    //searching for solution in the cache
    if (this->cache->inCache(strProblem)) {
      solution = this->cache->getSolution(strProblem);
      //if solution wasn't found create one
    } else {///// אולי בלבול יכול להיות שהצריך לשלוח את המטריצה הגדולה (סולבר)
      solution = this->solver->solve(problem);
      this->cache->intoCache(strProblem, solution);//////hash להוסיף
    }

    for (int i = 0; i < solution.size(); i++) {
      message = message + solution[i];/// להוסיך רווח
    }

    write(socket, message.c_str(), message.length());
  }
  ~MyClientHandler() {}
};
#endif //EX4__MYCLAIENTHANDLER_H_