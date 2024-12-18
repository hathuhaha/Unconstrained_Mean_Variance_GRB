#include "Data.h"

void read_data(const string& filename, int& N, int& T, vector<double>& Mean, vector<vector<double>>& Covariance) {
	ifstream input(filename);
	input >> N >> T;
	vector<vector<double>> stock_price(N, vector<double>(T + 1));
	vector<vector<double>> stock_return(N, vector<double>(T));
	for (int i = 0; i < N; i++) {
		for (int j = 0; j <= T; j++)
			input >> stock_price[i][j];
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < T; j++) {
			stock_return[i][j] = (stock_price[i][j + 1] - stock_price[i][j]) / stock_price[i][j];
		}
	}
	Mean.resize(N);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < T; j++)
			Mean[i] += stock_return[i][j];
		Mean[i] /= T;
	}
	Covariance.resize(N);
	for (int i = 0; i < N; i++) {
		Covariance[i].resize(N);
		for (int j = 0; j < N; j++) {
			for (int k = 0; k < T; k++) {
				Covariance[i][j] += (stock_return[i][k] - Mean[i]) * (stock_return[j][k] - Mean[i]);
			}
			Covariance[i][j] /= (T - 1);
		}
	}
	input.close();
}

