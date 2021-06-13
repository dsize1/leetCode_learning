#include <vector>
#include <string>
#include <iostream>

using namespace std;

bool compare_queen(string &row, int i);

bool can_place(int x, int y, vector<string> &placement);

string create_row(int y, int n);

void backtracking(
	int n,
	vector<vector<string>> &result,
	vector<string> &placement,
	int x = 0
);

vector<vector<string>> solve_n_queens(int n);

void logResult(ostream &os, vector<vector<string>> &result);

int main() {
	vector<vector<string>> result = solve_n_queens(4);
	logResult(cout, result);
	return 0;
}

bool compare_queen(string &row, int i) {
	return row.compare(i, 1, "Q") == 0;
}

bool can_place(int x, int y, vector<string> &placement, int n) {
	if (x == 0) {
		return true;
	}
	int m = x + 1;
	for (auto row : placement) {
		if (compare_queen(row, y)) {
			return false;
		}
	}
	for (int i = x - 1, j = y - 1; 0 <= i && 0 <= j; --i, --j) {
		if (compare_queen(placement[i], j)) {
			return false;
		}
	}
	for (int i = x - 1, j = y + 1; 0 <= i && j < n; --i, ++j) {
		if (compare_queen(placement[i], j)) {
			return false;
		}
	}
	return true;
}

string create_row(int y, int n) {
	string row(n, '.');
	row.replace(y, 1, "Q");
	return row;
}

void backtracking(
	int n,
	vector<vector<string>> &result,
	vector<string> &placement,
	int x
) {
	for (int y = 0; y < n; ++y) {
		if (can_place(x, y, placement, n)) {
			placement.push_back(
				create_row(y, n)
			);
			if (x + 1 == n) {
				result.push_back(placement);
				placement.pop_back();
				break;
			}
			backtracking(n, result, placement, x + 1);
			placement.pop_back();
		}
	}
}

vector<vector<string>> solve_n_queens(int n) {
	vector<vector<string>> result;
	vector<string> placement;
	backtracking(n, result, placement, 0);
	return result;
}

void logResult(ostream &os, vector<vector<string>> &result) {
	for (auto item : result) {
		os << "result:==========" << endl;
		for (auto row : item) {
			os << row << endl;
		}
		os << "==========" << endl;
		os << endl;
	}
}
