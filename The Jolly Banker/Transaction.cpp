#include "Transaction.h"
#include <iostream>

// Does not start with "  " or an endline. Caller needs to do so.
std::ostream& operator<<(std::ostream& out_stream, const Transaction& transaction) {
	std::cout << Transaction_Names[(int)transaction.transaction_type_] << " " << transaction.id_ << (int)transaction.fund_type_ << " " << transaction.amount_;
	
	if (transaction.transaction_type_ == Transaction_Type::kTransfer) {
		std::cout << " " << transaction.id_transfer_ << (int)transaction.fund_transfer_type_;
	}

	if (!transaction.valid_transaction_) {
		std::cout << " (Failed)";
	}
	
	return out_stream;
}

Transaction::Transaction() {
	id_ = -1;
	id_transfer_ = -1;
	amount_ = -1;
	fund_type_ = Fund_Type::kNone;
	fund_transfer_type_ = Fund_Type::kNone;
	transaction_type_ = Transaction_Type::kHistory;

	valid_transaction_ = false;
}


void Transaction::set_id(int id) {
	id_ = id;
}

void Transaction::set_id_transfer(int id_transfer) {
	id_transfer_ = id_transfer;
}

void Transaction::set_amount(int amount) {
	amount_ = amount;
}

// Assumes only valid transaction types are put in
void Transaction::set_transaction_type(Transaction_Type transaction_type) {
	transaction_type_ = transaction_type;
}

void Transaction::set_fund_type(Fund_Type fund_type) {
	fund_type_ = fund_type;
}

void Transaction::set_fund_transfer_type(Fund_Type fund_transfer) {
	fund_transfer_type_ = fund_transfer;
}

void Transaction::set_last_name(std::string last_name) {
	last_name_ = last_name;
}

void Transaction::set_first_name(std::string first_name) {
	first_name_ = first_name;
}

void Transaction::set_validity(bool valid) {
	valid_transaction_ = valid;
}

int Transaction::get_id() {
	return id_;
}

int Transaction::get_id_transfer() {
	return id_transfer_;
}

int Transaction::get_amount() {
	return amount_;
}

Transaction_Type Transaction::get_transaction_type() {
	return transaction_type_;
}

Fund_Type Transaction::get_fund_type() {
	return fund_type_;
}

Fund_Type Transaction::get_fund_transfer_type() {
	return fund_transfer_type_;
}

std::string Transaction::get_last_name() {
	return last_name_;
}

std::string Transaction::get_first_name() {
	return first_name_;
}

bool Transaction::get_validity() {
	return valid_transaction_;
}
