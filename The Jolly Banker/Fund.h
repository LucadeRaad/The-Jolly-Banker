#ifndef FUND_H
#define FUND_H

// Holds one specific fund and its history.

#include <vector>
#include <string>
#include <list>

#include "Transaction.h"

class Fund {
 public:
	Fund();

	void Deposit(Transaction transaction);
	bool Withdraw(Transaction transaction, std::string name);
	bool Transfer(Transaction transaction);

	int get_amount();

	void DisplayHistory();

	bool HasHistory();

 private:
	int amount_;

	std::list<Transaction> history_;
};

#endif  // FUND_H
