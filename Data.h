#ifndef DAHA_H
#define DATA_H
#include "gurobi_c++.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
void read_data(const string& filename, int& N, int& T, vector<double>& Mean, vector<vector<double>>& Covariance);
#endif
