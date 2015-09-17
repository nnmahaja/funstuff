#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <deque>
#include <iterator>

using namespace std;

class Solution {
public:
      struct pair_comp {
	bool operator()(const pair<int,int>& p1, const pair<int,int>& p2) {
	  return (p1.first <= p2.first)  && (p1.second < p2.second);
	}
      };
    int numIslands(vector<vector<char>>& grid) {
      size_t nislands = 0;
      // collect all the island locations
      set< pair<int, int> > all_locs;
      for (int i = 0; i < grid.size(); i++) {
	int cols = grid[i].size();
	for (int j = 0; j < cols; j++) {
	  if (grid[i][j]) all_locs.insert(make_pair(i, j));
	}
      }
      while (!all_locs.empty()) {
	pair<int, int> root = *(all_locs.begin());
	// do a bfs to collect locs in an island
	set<pair<int,int>> inodes; inodes.insert(root);
	deque<pair<int,int>> q; q.push_back(root);
	set<pair<int,int>> marked;
	while(!q.empty()) {
	  pair<int,int> f = q.front();
	  q.pop_front();
	  marked.insert(f);
	  
	  int cols = (grid[f.first].size()-1);
	  // add neighbors
	  if (f.first != (grid.size()-1)) {
	    if (grid[f.first+1][f.second]) {
	      auto e = make_pair(f.first+1, f.second);
	      if (marked.find(e) == marked.end()) q.push_back(e);
	      inodes.insert(e);
	    }
	  }
	  if (f.first != 0) {
	    if (grid[f.first-1][f.second]) {
	      auto e = make_pair(f.first-1, f.second);
	      if (marked.find(e) == marked.end()) q.push_back(e);
	      inodes.insert(e);
	    }
	  }
	  if (f.second != 0) {
	    if (grid[f.first][f.second-1]) {
	      auto e = make_pair(f.first, f.second-1);
	      if (marked.find(e) == marked.end()) q.push_back(e);
	      inodes.insert(e);
	    }
	  }
	  if (f.second != cols) {
	    if (grid[f.first][f.second+1]) {
	      auto e = make_pair(f.first, f.second+1);
	      if (marked.find(e) == marked.end()) q.push_back(e);
	      inodes.insert(e);
	    }
	  }
	}
	
	nislands++;
	
	// delete island locs from all_locs
	set<pair<int,int>> diff;
	set_difference(all_locs.begin(), all_locs.end(), 
		      inodes.begin(), inodes.end(), 
		      std::inserter(diff, diff.begin()), pair_comp());
	
	all_locs = diff;
	
    //     cout << "all_locs: ";
    //     for (auto p : all_locs) {
    //       cout << "<" << p.first << ", " << p.second << "> ";
    //     }
    //     cout << endl;
    //     cout << "inodes: ";
    //     for (auto p : inodes) {
    //       cout << "<" << p.first << ", " << p.second << "> ";
    //     }
    //     cout << endl;
    //     cout << inodes.size() << ", " << all_locs.size() << endl;
	
	// start next island
	inodes.clear();
      }
      
      return nislands;
    }
};

typedef vector< vector<char> > char_mat;

int main() {
  
  vector<string> vec = {"10011101100000000000","10011001000101010010","00011110101100001010","00011001000111001001",
    "00000001110000000000","10000101011000000101","00010001010101010101","00010100110101101110","00001001100001000101",
    "00100100000100100010","10010000000100101010","01000101011011101100","11010000100000010001","01001110001111101000",
    "00111000110001010000","10010100001000101011","10100000010001010000","01100011101010111100","01000011001010010011","00000011110100011000"};
  
//   char_mat grid = { {1,1,1,1,0},
// 		  {1,1,0,1,0},
// 		  {1,1,0,0,0},
// 		  {0,0,0,0,0} };
		  
  char_mat grid = { {1,1,0,0,0},
		    {1,0,0,0,0},
		    {0,0,1,0,0},
		    {0,0,0,1,1} };
  
  Solution s;
  cout << s.numIslands(grid) << endl;
  return 0;
}
