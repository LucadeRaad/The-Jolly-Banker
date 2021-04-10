#include "Fund.h"
#include <iostream>

Fund::Fund() {
	amount_ = 0;
}

// Deposits into an account
void Fund::Deposit(Transaction transaction) {
	amount_ += transaction.get_amount();
	history_.push_back(transaction);
}

// Withdraws from an account.
bool Fund::Withdraw(Transaction transaction, std::string name) {
	if (amount_ < transaction.get_amount()) {
		if (transaction.get_transaction_type() == Transaction_Type::kWithdraw) {
			std::cout << "ERROR: Not enough funds to withdraw " << transaction.get_amount() << " from " << name << " " << Fund_Names[(int)transaction.get_fund_type()] << std::endl;
		}
		transaction.set_validity(false);
		history_.push_back(transaction);
		return false;
	}
	amount_ -= transaction.get_amount();

	history_.push_back(transaction);
	return true;
}

// Transfer fails if unable to withdraw or transfering to itself. Otherwise
// succeeds.
bool Fund::Transfer(Transaction transaction) {
	if (transaction.get_validity()) {
		return Withdraw(transaction, "");
	} else {
		history_.push_back(transaction);
		return false;
	}
}

int Fund::get_amount() {
	return amount_;
}

void Fund::DisplayHistory() {
	std::list<Transaction>::iterator it;
	for (it = history_.begin(); it != history_.end(); ++it) {
		std::cout << "  " << *it << std::endl;
	}
}

bool Fund::HasHistory() {
	return history_.size() > 0;
}
