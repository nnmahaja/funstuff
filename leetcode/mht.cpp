#include <iostream>
#include <vector>

using namespace std;

#define INF -1

class Solution {
public:
    vector<int> findMinHeightTrees(int n, vector<pair<int, int>>& edges) {
      vector<int> mht_roots;
      
      vector<vector<int>> tbl;
      for (int i = 0; i < n; i++) {
	for (int j = 0; j < n; j++) {
	  if (i == j) {
	    tbl[i][i] = 0;
	  } else {
	    tbl[i][j] = INF;
	  }
	}
      }
      
      // bfs from 0 and build tbl
      
      
      
      // find the max non-inf value for each vertex
      // the vertices with the min maxval are the mht roots
      
      
      return mht_roots;
    }
};


int main() {
}
