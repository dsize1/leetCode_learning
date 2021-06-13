#include <vector>
#include <string>
#include <regex>
#include <queue>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <iostream>

using namespace std;

/**
 * 广搜遍历出转换的最短路径
 * 深搜获取多条最短路径的结果
 * BFS时需要记录层数，保存同层的相同的转换单词
 * 生成无序集合的单词字典，以便用O(1)时间复杂度判断转换单词是否存在于字典中，
 * 之前已经转换的单词需要从字典中去除，因为之后再转换的路径一定不是最短路径
 **/


void dfs(
	vector<vector<string>> &res,
	const string &endWord,
	unordered_map<string, set<string>> &from,
	vector<string> &path
) {
	if (from[endWord].empty()) {
		res.push_back({path.rbegin(), path.rend()});
		return;
	}
	for (const string &word : from[endWord]) {
		path.push_back(word);
		dfs(res, word, from, path);
		path.pop_back();
	}
}

vector<vector<string>> findLadders(string beginWord, string endWord, vector<string>& wordList) {
	vector<vector<string>> res;
	unordered_set<string> dict(wordList.begin(), wordList.end());
	if (dict.find(endWord) == dict.end()) {
		return res;
	}
	dict.erase(beginWord);

	// 记录转换单词的层数
	unordered_map<string, int> steps = {{beginWord, 0}};
	int step = 0;
	// 记录单词是由那些单词转换而来的,一对多
	unordered_map<string, set<string>> from = {{beginWord, {}}};
	bool found = false;
	// 队列实现bfs
	queue<string> q({beginWord});
	int wordLength = beginWord.size();

	// bfs 构建扩展单词的树
	while(!q.empty()) {
		++step;
		int size = q.size();
		for (int i = 0; i < size; ++i) {
			const string curr = move(q.front());
			string next = curr;
			q.pop();

			for (int j = 0; j < wordLength; ++j) {
				const char origin = next[j];
				
				for (char c = 'a'; c <= 'z' ;++c) {
					next[j] = c;

					// 可能存在多条最短路径所以需要多次存储同层的相同单词
					if (steps[next] == step) {
						from[next].insert(curr);
					}
					if (dict.find(next) == dict.end()) {
						continue;
					}

					// 如果下次转换是已经转换过了的单词，那么该次一定不是在最短的转换路径上，故需要在第一次转换时在字典中去除该单词
					dict.erase(next);
					// 记录转换单词的层数
					steps[next] = step;
					// 记录当前单词所转换成的单词
					from[next].insert(curr);
					// 下层的bfs单词
					q.push(next);
					if (next == endWord) {
						found = true;
					}
				}
				next[j] = origin;
			}
		}
		if (found) {
			break;
		}
	}

	// dfs 最短路径
	if (found) {
		vector<string> path = {endWord};
		dfs(res, endWord, from, path);
	}

	return res;
}

vector<string> str2WordList(string &s) {
	vector<string> wordList;

	string pattern ("\"(\\w+?)\"");
	regex r(pattern);
	sregex_iterator it(s.begin(), s.end(), r), end_it;
	while(it != end_it) {
		auto itStr = it->str();
		auto itStrSize = itStr.size();
		wordList.push_back(
			itStr.substr(1, itStrSize - 2)
		);
		++it;
	}

	return wordList;
}

void logFunc(vector<vector<string>> &resultList) {
	for (auto result : resultList) {
		cout << "-----------" << endl;
		cout << "result: [" << endl;
		for (auto word : result) {
			cout << "\t" << word << endl;
		}
		cout << "]" << endl;
		cout << "-----------" << endl;
	}
}

int main() {
	string beginWord("hit");
	string endWord("cog");
	string wordStr("[\"hot\",\"dot\",\"dog\",\"lot\",\"log\",\"cog\"]");
	auto wordList = str2WordList(wordStr);
	auto resultList = findLadders(beginWord, endWord, wordList);
	logFunc(resultList);
  	return 0;
}
