#include <iostream>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <utility>
#include <list>
#include <unordered_set>

struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const
    {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
 
        if (hash1 != hash2) {
            return hash1 ^ hash2;              
        }
         
        // If hash1 == hash2, their XOR is zero.
          return hash1;
    }
};

// Every cell interacts with its eight neighbours, which are the cells that are horizontally, vertically, or diagonally adjacent. At each step in time, the following transitions occur:
// Any live cell with fewer than two live neighbours dies.
// Any live cell with two or three live neighbours lives.
// Any live cell with more than three live neighbours dies.
// Any dead cell with exactly three live neighbours becomes a live cell.

struct GridClass {
	GridClass(std::list<std::pair<int,int>> input);
	
	// Control Variables
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
void GridClass::insertAliveCell(std::pair<int,int> p){
	grid.insert({p, true});
}

void GridClass::eraseCell(std::pair<int,int> p){
	grid.erase(p);
}
	
int GridClass::numberOfAdjacentCells(std::pair<int,int> p){
	int nAC = 0;
	if (grid.find({p.first+1,p.second}) != grid.end()){
		nAC++;
	}
	if (grid.find({p.first-1,p.second}) != grid.end()){
		nAC++;
	}
	if (grid.find({p.first,p.second+1}) != grid.end()){
		nAC++;
	}
	if (grid.find({p.first,p.second-1}) != grid.end()){
		nAC++;
	}
	if (grid.find({p.first+1,p.second+1}) != grid.end()){
		nAC++;
	}
	if (grid.find({p.first-1,p.second-1}) != grid.end()){
		nAC++;
	}
	if (grid.find({p.first-1,p.second+1}) != grid.end()){
		nAC++;
	}
	if (grid.find({p.first+1,p.second-1}) != grid.end()){
		nAC++;
	}
	return nAC;
}

std::list<std::pair<int,int>> GridClass::getAdjacentsCells(std::pair<int,int> p){
	std::list<std::pair<int,int>> aC = {};
	if (grid.find({p.first+1,p.second}) == grid.end()){
		aC.push_back({p.first+1, p.second});
	}
	if (grid.find({p.first-1,p.second}) == grid.end()){
		aC.push_back({p.first-1, p.second});
	}
	if (grid.find({p.first,p.second+1}) == grid.end()){
		aC.push_back({p.first, p.second+1});
	}
	if (grid.find({p.first,p.second-1}) == grid.end()){
		aC.push_back({p.first, p.second-1});
	}
	if (grid.find({p.first+1,p.second+1}) == grid.end()){
		aC.push_back({p.first+1, p.second+1});
	}
	if (grid.find({p.first-1,p.second-1}) == grid.end()){
		aC.push_back({p.first-1, p.second-1});
	}
	if (grid.find({p.first-1,p.second+1}) == grid.end()){
		aC.push_back({p.first-1, p.second+1});
	}
	if (grid.find({p.first+1,p.second-1}) == grid.end()){
		aC.push_back({p.first+1, p.second-1});
	}
	return aC;
}

void GridClass::handleTick(){
	for(auto cell = grid.begin(); cell != grid.end(); ++cell){
		for (auto cellToCheck : getAdjacentsCells((*cell).first)){
			if (checkedCells.find(cellToCheck) != checkedCells.end()){
				continue;
			}
			if (numberOfAdjacentCells(cellToCheck) == 3){
				cellToCreateList.push_back(cellToCheck);
			}
			checkedCells.insert(cellToCheck);
		}
		int nOAC = numberOfAdjacentCells((*cell).first);
		if (nOAC < 2){cellToEraseList.push_back((*cell).first);}
		else if (nOAC > 3){cellToEraseList.push_back((*cell).first);}
		else {continue;}
	}
	handleErases();
	handleCreations();
	checkedCells.clear();
	return;
}

void GridClass::handleErases(){
	for (auto i = cellToEraseList.begin(); i != cellToEraseList.end(); ++i){
		if (grid.find(*i) != grid.end()){
			std::cout << "Erase Cell at: " << "(" << (*i).first << "," << (*i).second << ") \n";
			eraseCell(*i);
		}
	}
	cellToEraseList.clear();
}
void GridClass::handleCreations(){
	for (auto i = cellToCreateList.begin(); i != cellToCreateList.end(); ++i){
		if (grid.find(*i) == grid.end()){
			std::cout << "Create Cell at: " << "(" << (*i).first << "," << (*i).second << ") \n";
			insertAliveCell(*i);
		}
	}
	cellToCreateList.clear();
}

GridClass::GridClass(std::list<std::pair<int,int>> input){
	for (auto i = input.begin(); i != input.end(); ++i){
		if (grid.find(*i) != grid.end()){
			std::cout << "Repeated Cell \n";
			continue;
		}
		insertAliveCell(*i);
	}
	return;
}