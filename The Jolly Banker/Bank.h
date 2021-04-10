#ifndef BANK_H
#define BANK_H

// Bank takes file input from a .txt and creates a que. It also checks for
// bugus data. It then reads from the que and decides whether transactions
// can be executed or whether they fail.

#include "Transaction.h"
#include "BSTree.h"

#include <queue>
#include <string>
#include <vector>

class Bank {
 public:
	Bank(std::string file_name);

	void ExecuteTransactions();

	void Display();

 private:
	bool buildQueue(std::string file_name);

	std::queue<Transaction> transaction_queue_;

	int SafeStringToInt(std::string input);

	BSTree tree_;
};

#endif  // BANK_H
