#include "GRB_Model.h"
#include "gurobi_c++.h"
#include <chrono>
#include <iostream>
#include <thread>
#include <iomanip>


void SetupModel(ofstream& output, const int& N, vector<double>& Mean, vector<vector<double>>& Covariance, string SetupType, const double& Constraint) {
	GRBEnv env;
	GRBModel model = GRBModel(env);
	GRBQuadExpr Risk = 0;
	GRBLinExpr Profit = 0;
	auto st = chrono::steady_clock::now();
	vector <GRBVar> w;
	vector<GRBVar> z;
	w.resize(N);
	z.resize(N);
	for (int i = 0; i < N; i++) {
		w[i] = model.addVar(0, 1, 0, GRB_CONTINUOUS);
		z[i] = model.addVar(0, 1, 0, GRB_BINARY);
	}
	GRBLinExpr SumW;
	GRBLinExpr SumZ;
	for (int i = 0; i < N; i++) {
		SumW += w[i];
		SumZ += z[i];
	}
	model.addConstr(SumW == 1.0);

	for (int i = 0; i < N; i++) {
		model.addConstr(w[i] <= 1.0 * z[i]);
	}
	for (int i = 0; i < N; i++) {
		Profit += w[i] * Mean[i];
		for (int j = 0; j < N; j++) {
			Risk += w[i] * Covariance[i][j] * w[j];
		}
	}
	if (SetupType != "Balance") {
	if (SetupType == "Profit") {
		model.addQConstr(Risk <= Constraint);
		model.setObjective(Profit, GRB_MAXIMIZE);
	}
	else {
		model.addConstr(Profit == Constraint);
		model.setObjective(Risk, GRB_MINIMIZE);
	}
	model.set(GRB_DoubleParam_TimeLimit, 3600);
	model.set(GRB_DoubleParam_ImproveStartTime, 3000);
	try {
		model.optimize();
		auto status = model.get(GRB_IntAttr_Status);
		auto ed = std::chrono::steady_clock::now();
		auto duration = chrono::duration_cast<chrono::milliseconds>(ed - st);
		if(status != GRB_UNBOUNDED && status != GRB_INFEASIBLE)
			output << fixed << setprecision(8) << Constraint << ' ' << Profit.getValue() << ' ' << Risk.getValue() << ' ' << duration.count() << '\n';
	}
	catch (...) {
		cout << "Can't solve\n";
		output << '\n';
	}
	}
	else {
		const double& lambda = Constraint;
		GRBQuadExpr Object = lambda * Risk - (1 - lambda) * Profit;
		model.setObjective(Object, GRB_MINIMIZE);
		model.set(GRB_DoubleParam_TimeLimit, 3600);
		model.set(GRB_DoubleParam_ImproveStartTime, 3000);
		model.optimize();
		auto ed = std::chrono::steady_clock::now();
		auto status = model.get(GRB_IntAttr_Status);
		auto duration = chrono::duration_cast<chrono::milliseconds>(ed - st);
		if (status != GRB_UNBOUNDED && status != GRB_INFEASIBLE)
			output << fixed << setprecision(8) << lambda << ' ' <<  Object.getValue() << ' ' << duration.count() << '\n';
	}
}
