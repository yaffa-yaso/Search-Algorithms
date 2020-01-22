//
// Created by aharon on 14/01/2020.
//
#include "General.h"

template<class P, class S>
string FileCacheManager<P, S>::hashing(P problem) {
  hash<P> myHash;
  // Using operator() to get hash value
  size_t key_hash = std::hash<P>()(problem);
  return to_string(myHash(key_hash));
}

template<class P, class S>
bool FileCacheManager<P, S>::inCache(P problem) {
  if (this->mapPointers.find(problem)!=mapPointers.end()) {
    return true;
  } else {
    return false;
  }
}

template<class P, class S>
void FileCacheManager<P, S>::intoCache(P problem, S solution) {
  string key = hashing(problem);
  bool found = inCache(key);
  bool exist = inCache(key);
  // if it doesn't exist both in file and cash
  if (!found && !exist) {
    ////
    lru(key);
    wFile(key, solution);
  } else if (found) {
    cacheList.erase(mapPointers[key]);
    wFile(key, solution);

  } else if (exist) {
    S oldObject = getSolution(key);
  }
  cacheList.push_front(make_pair(key, solution));
  mapPointers[key] = cacheList.begin();

}

template<class P, class S>
S FileCacheManager<P, S>::getSolution(P problem) {
  string key = hashing(problem);
  bool found = inCache(key);
  string check = key;
  bool exist = inCache(check);
  // if it doesn't exist both in file and cash
  if (!found & !exist) {
    throw "an error";
    // if it exist only in cache
  } else if (found) {
    S obj = (mapPointers[key])->second;
    cacheList.erase(mapPointers[key]);
    cacheList.push_front(make_pair(key, obj));
    mapPointers[key] = cacheList.begin();
    return mapPointers[key]->second;
  } else {
    S object = rFile((check));
    wFile(check, object);
    //if the cash is full
    if (cacheList.size()==this->capacity) {
      auto least = cacheList.back();
      cacheList.pop_back();
      mapPointers.erase(least.first);
    }
    cacheList.push_front(make_pair(key, object));
    mapPointers[key] = cacheList.begin();
    return object;
  }

}

template<class P, class S>
void FileCacheManager<P, S>::lru(string key) {
  // if cash is full
  if (cacheList.size()==this->capacity) {
    auto least = cacheList.back();
    cacheList.pop_back();
    mapPointers.erase(least.first);
    //if   exist
  } else {
    mapPointers.erase(key);
  }
}

template<class P, class S>
void FileCacheManager<P, S>::wFile(string key, S solution) {
  ofstream output_file{key, ios::binary};
  if (!output_file.is_open()) {
    cout << "error while opening file";
  } else {
    output_file.write((char *) &solution, sizeof(S));
    output_file.close();
  }
}

template<class P, class S>
S FileCacheManager<P, S>::rFile(string key) {
  ifstream in_file(key, ios::binary);
  S object;
  if (!in_file.is_open()) {
    throw "error while opening file";
  } else {
    in_file.read((char *) &object, sizeof(S));
    in_file.close();
    return object;
  }
  return object;
}

template<class P, class S>
string FileCacheManager<P, S>::Convertstr(size_t sz) {
  char buf[24];
  sprintf(buf, "%u", sz);
  string newStr(buf);
  return newStr;
}



