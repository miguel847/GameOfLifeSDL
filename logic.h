#include <map>
#include <unordered_map>
#include <utility>
#include <functional>
#include <list>
#include <unordered_set>

#pragma once
struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const;
};

struct GridClass{
	GridClass(std::list<std::pair<int,int>> input);
	std::unordered_map<std::pair<int,int>, bool, hash_pair> grid;
	std::list<std::pair<int,int>> cellToCreateList;
	std::list<std::pair<int,int>> cellToEraseList;
	std::unordered_set<std::pair<int,int>, hash_pair> checkedCells;
	void insertAliveCell(std::pair<int,int> p);
	void eraseCell(std::pair<int,int> p);
	int numberOfAdjacentCells(std::pair<int,int> p);
	std::list<std::pair<int,int>> getAdjacentsCells(std::pair<int,int> p);
	void handleTick();
	void handleErases();
	void handleCreations();
};
