#include <vector>
#include <utility>
#include <iostream>
#include <queue>
#include <regex>

using namespace std;

static vector<pair<int, int>> direction{
    make_pair(-1, 0),
    make_pair(0, 1),
    make_pair(1, 0),
    make_pair(0, -1)
};

void dfs(
    vector<vector<int>> &grid,
    vector<vector<int>> &visited,
    queue<pair<int, int>> &points,
    int x = 0,
    int y = 0
) {
    points.push(make_pair(x, y));
    visited[x][y] = 1;
    int n = grid.size();
    for (auto dir : direction) {
        int dx = x + dir.first, dy = y + dir.second;
        if (dx < 0 || dx == n || dy < 0 || dy == n || visited[dx][dy] == 1 || visited[dx][dy] == -1)
            continue;
        if (grid[dx][dy] == 0) {
            visited[dx][dy] = -1;
        } else {
            dfs(grid, visited, points, dx, dy);
        }
    }
}

bool bfs (
    vector<vector<int>> &grid,
    vector<vector<int>> &visited,
    queue<pair<int, int>> &points,
    int x,
    int y
) {
	int result = false;
    int n = grid.size();
    for (auto dir : direction) {
        int dx = x + dir.first, dy = y + dir.second;
        if (dx < 0 || dx == n || dy < 0 || dy == n || visited[dx][dy] == 1)
            continue;
		if (grid[dx][dy] == 1) {
            result = true;
			break;
		} else {
            visited[dx][dy] = 1;
            points.push(
                make_pair(dx, dy)
            );
		}
    }
    return result;
}

int shortestBridge(vector<vector<int>> &grid) {
    int n = grid.size();
    vector<vector<int>> visited(
        n,
        vector<int>(n, 0)
    );
    queue<pair<int, int>> points;
    int shortest = -1;
    bool flag = false;

    for (int i = 0; i < n; ++i) {
      	if (flag) break;
		for (int j = 0; j < n; ++j) {
			if (grid[i][j]) {
				dfs(grid, visited, points, i, j);
				flag = true;
				break;
			}
		}
    }
    int levelSize = points.size();
    bool isFound = false;
    // 必须使用bfs，dfs会超时。
    // dfs出一个岛屿后，逐层扩张这个岛屿，记录扩张层数，知道连接另一个岛屿；
    while (levelSize > 0) {
        while (levelSize > 0) {
            auto point = points.front();
            points.pop();
            isFound = bfs(grid, visited, points, point.first, point.second);
            --levelSize;
            if (isFound)
                return shortest + 1;
        }
        ++shortest;
        levelSize = points.size();
    }
    return shortest;
}

vector<int> str2Vec(string &str) {
	vector<int> row;
	string pattern ("\\d+");
	regex r(pattern);
	sregex_iterator it(str.begin(), str.end(), r);
	sregex_iterator end_it;
	while (it != end_it) {
		auto numStr = it->str();
		row.push_back(stoi(numStr));
		++it;
	}
	return row;
}

vector<vector<int>> str2Grid(string &str) {
	vector<vector<int>> grid;
	string rowPattern ("\\[(([^\\[])+?)\\]");
	regex r(rowPattern);
	sregex_iterator row_it(str.begin(), str.end(), r);
	sregex_iterator end_it;
	while (row_it != end_it) {
		auto rowStr = row_it->str();
		auto row = str2Vec(rowStr);
		grid.push_back(row);
		++row_it;
	}
	return grid;
}

int main() {
	string gridString("[[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0],[0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0],[0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,0],[0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0],[0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0],[0,0,0,0,0,0,0,0,0,1,0,1,1,0,0,1,0,0,0,0],[0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0,1,0,0,0],[0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0,0,0],[0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,1,1,0],[0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0],[0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0],[0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0],[0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0]]");
	auto grid = str2Grid(gridString);
	int result = shortestBridge(grid);
	cout << "result: " << result << endl;
	return 0;
}