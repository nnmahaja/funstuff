#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <list>
#include <functional>
#include <unordered_set>

using namespace std;

class matcher {

private:
  
  struct book_entry_type {
    int price;
    int quant;
    std::string order_id;
    
    book_entry_type(int p, int q, std::string o_id) : 
      price(p), quant(q), order_id(o_id) {}
  };
  
  // books are maintained in decreasing order of prices
  // multiple orders with the same price are maintained in decreasing
  // order of time
  using book_type = std::list<book_entry_type>;
  book_type sell_book;
  book_type buy_book;
  
  std::unordered_set<std::string> canceled_orders;
  
  void process_order(std::string type, const book_entry_type& order, 
		     book_type& lookup_book, book_type& order_book, 
		     const std::function<bool(int, int)>& compare_func) {
    int price = order.price;
    int quantity = order.quant;
    std::string o_id = order.order_id;
    
    // see if the order can be satisfied
    for (auto itr = lookup_book.begin(); itr != lookup_book.end();) {
      if (quantity <= 0) {
	// input order has been satisfied, return
	return;
      }
      // check if order was canceled, if yes delete it and continue
      if (canceled_orders.find(o_id) != canceled_orders.end()) {
	itr = lookup_book.erase(itr);
	canceled_orders.erase(o_id);
	continue;
      }
      if (compare_func(price, itr->price)) {
	int traded_quant = std::min(quantity, itr->quant);
	// trade!
	itr->quant -= traded_quant;
	quantity -= traded_quant;
	
	// print the trade
	std::cout << "TRADE " 
		  << itr->order_id << " " << itr->price << " "
		  << o_id << " " << price << " "
		  << traded_quant << std::endl;
	
	if (itr->quant <= 0) {
	  // delete this order from the book
	  itr = lookup_book.erase(itr);
	} else {
	  ++itr;
	}
	
      } else {
	++itr;
      }
    }
    
    if ((type.compare("GFD") == 0) && (quantity > 0)) {
      // order could not be satisfied, make entry only for order of type "GFD"
      auto itr = std::find_if(order_book.begin(), order_book.end(), 
			      [price](const book_entry_type& e){return (price > e.price);});
      order_book.insert(itr, book_entry_type(price, quantity, o_id));
    }
  }
  
  void process_modify(std::string modify_to, const book_entry_type& order) {
    int price = order.price;
    int quantity = order.quant;
    std::string o_id = order.order_id;
    
    // find the order
    auto itr1 = std::find_if(buy_book.begin(), buy_book.end(), 
			     [o_id](const book_entry_type& e){return (o_id.compare(e.order_id) == 0);});
    if (itr1 == buy_book.end()) {
      // not present in buy_book, should be there in sell_book
      itr1 = std::find_if(sell_book.begin(), sell_book.end(), 
			  [o_id](const book_entry_type& e){return (o_id.compare(e.order_id) == 0);});
      // erase it
      itr1 = sell_book.erase(itr1);
      
    } else {
      // present in buy_book
      
      // erase the found order
      itr1 = buy_book.erase(itr1);
      
    }
    
    // insert order in appropriate book
    if (modify_to.compare("SELL") == 0) {
      auto itr = std::find_if(sell_book.begin(), sell_book.end(), 
			      [price](const book_entry_type& e){return price > e.price;});
      sell_book.insert(itr, order);
    } else {
      auto itr = std::find_if(buy_book.begin(), buy_book.end(), 
			      [price](const book_entry_type& e){return price > e.price;});
      buy_book.insert(itr, order);
    }
  }
  
  void process_cancel(std::string o_id) {
    canceled_orders.insert(o_id);
  }
  
  void print_book(const book_type& book) const {
    int curr_price = 0, curr_quant = 0;
    for (auto itr = book.begin(); itr != book.end(); ++itr) {
      curr_price = itr->price;
      curr_quant += itr->quant;
      auto nx = std::next(itr, 1);
      if ((nx != book.end()) && 
	  (nx->price == curr_price)) {
	// keep adding to the current price and quantity
      } else {
	// print it
	std::cout << curr_price << " " << curr_quant << std::endl;
	curr_price = 0, curr_quant = 0;
      }
    }
  }
  
  void process_print() const {
    // iterate the books which are already in descending order of price
    // accumulate quantities with same price
    std::cout << "SELL:" << std::endl;
    print_book(sell_book);
    
    std::cout << "BUY:" << std::endl;
    print_book(buy_book);
  }
  
  inline
  void seq_impl(string line) {
    
    // split columns
    vector<string> tokens;
    
    stringstream ss(line);
    string tok;
    while (ss >> tok) {
      tokens.push_back(tok);
    }
//     for (string tok : tokens) {
//       cout << tok << endl;
//     }
//     cout << endl;
    
    
    if (tokens[0].compare("BUY") == 0) {
      
      if (tokens.size() == 5) {
	book_entry_type o(atoi(tokens[2].c_str()), atoi(tokens[3].c_str()), tokens[4]);
	process_order(tokens[1], o, sell_book, buy_book, [](int order_price, int book_price){return (order_price >= book_price);});
      } else {
	std::cerr << "Something wrong with the buy order, bypassing processing...";
      }
      return;
    }
    
    if (tokens[0].compare("SELL") == 0) {
      
      if (tokens.size() == 5) {
	book_entry_type o(atoi(tokens[2].c_str()), atoi(tokens[3].c_str()), tokens[4]);
	process_order(tokens[1], o, buy_book, sell_book, [](int order_price, int book_price){return (book_price >= order_price);});
      } else {
	std::cerr << "Something wrong with the sell order, bypassing processing...";
      }
      
      return;
    }
    
    if (tokens[0].compare("CANCEL") == 0) {
      process_cancel(tokens[1]);
      return;
    }
    
    if (tokens[0].compare("MODIFY") == 0) {
      if (tokens.size() == 5) {
	book_entry_type o(atoi(tokens[3].c_str()), atoi(tokens[4].c_str()), tokens[1]);
	process_modify(tokens[2], o);
      } else {
	std::cerr << "Something wrong with the modify order, bypassing processing...";
      }
      return;
    }
    
    if (tokens[0].compare("PRINT") == 0) {
      
      process_print();
      
      return;
    }
  }
  
public:
  
  void process(string line) {
    seq_impl(line);
  }
  
};

int main() {
  
  string line;
  matcher m;
  while (getline(cin, line)) {
    m.process(line);
  }
  
  return 0;
}
