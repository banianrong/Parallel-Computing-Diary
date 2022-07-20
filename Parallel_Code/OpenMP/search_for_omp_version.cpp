#include<iostream>
#include<omp.h>
#include<unordered_map>
#include<string>
using namespace std;

int main() {
  unordered_map<unsigned, string> map{
    {200505,"2.5"}, {200805, "3.0"}, {201107, "3.1"}, {201307,"4.0"}, {201511, "4.5"}
  };
  printf("OpenMP version:%s.\n", map.at(_OPENMP).c_str());
  return 0;
} 
