#include <iostream>
#include <vector>
#include <set>

using namespace std;

class Solution {
public:
  static const size_t max = 9;
  static const size_t rootmax = 3;
    
  bool validVal(vector<vector<char>>& board, int row, int col, const char& cval) {
    // check row, column and area
    for(int i = 0; i < max; i++) {
      if (board[row][i] == cval) {
	return false;
      }
      if (board[i][col] == cval) {
	return false;
      }
    }
    size_t ar = (row/rootmax) * rootmax;
    size_t ac = (col/rootmax) * rootmax;
    for (size_t i = ar; i < (ar + rootmax); i++) {
      for (size_t j = ac; j < (ac + rootmax); j++) {
	if (board[i][j] == cval) {
	  return false;
	}
      }
    }
    
    return true;
  }
  
  bool fillCell(vector<vector<char>>& board, int row, int col) {
    if (row == max) {
      return true;
    }
    if (col == max) {
      return fillCell(board, row + 1, 0);
    }
    if (board[row][col] != '.') {
	return fillCell(board, row, col+1);
    }
    const size_t area = (row / rootmax) * rootmax + (col / rootmax);
    // chose a row value
    for (int val = 1; val <= max; val++) {
      const char cval = '0' + val;
      
      if (validVal(board, row, col, cval)) {
	// fill the board and mark the value as not available
	board[row][col] = cval;
	if (fillCell(board, row, col + 1)) {
	  return true;
	}
	board[row][col] = '.';
      }
      
    }
    return false;
  }
  
  void solveSudoku(vector<vector<char>>& board) {
    fillCell(board, 0, 0);
  }
};

int main() {
   
}
