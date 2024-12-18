#ifndef GRB_MODEL_H
#define GRB_MODEL_H
#include <string>
#include <vector>
#include <fstream>
#include "gurobi_c++.h"
using namespace std;
void SetupModel(ofstream& output, const int& N, vector<double>& Mean, vector<vector<double>>& Covariance, string SetupType, const double& Constraint);
#endif

