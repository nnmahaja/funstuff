#include <iostream>
#include <algorithm>
#include <iterator>
using namespace std;

vector<vector<int>> threeSum(vector<int>& nums) {
  std::sort(nums.begin(), nums.end());
  int sz = nums.size();
  vector<vector<int>> ret;
  for (int i = 0; i < (sz - 2); i++) {
      int curr = nums[i];
      int beg = i + 1; int end = sz - 1;
      while (beg < end) {
	  int b = nums[beg];
	  int e = nums[end];
	  if (0 == (curr + b + e)) {
	      vector<int> triple;
	      triple.push_back(curr);
	      triple.push_back(b);
	      triple.push_back(e);
	      ret.push_back(triple);
	      beg++;
	      end--;
	  } else if (0 < (curr + b + e)) {
	    end--;
	  } else {
	    beg++;
	  }
      }
  }
  return ret;
}

int main() {
//   vector<int> nums = {-1, 0, 1, 2, -1, -4};
  vector<int> nums = {3, 0, -2, -1, 1, 2};
  auto res = threeSum(nums);
  for (auto v : res) {
    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
  }
}

