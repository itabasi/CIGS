#include <iostream>
using namespace std;
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <TGraph.h>

const int nfile;
extern vector<string> split(string&input, char delimiter);


void CV_AIST_check(){ // Current timde dependence

  ifstream* ifp[nfile];
  ifp[0] = 
  
}


vector<string> split(string&input, char delimiter){

  istringstream stream(input);
  string field;
  vector<string> result;
  while(getline(stream,field,delimiter))
    result.push_back(field);

  return result;
};




