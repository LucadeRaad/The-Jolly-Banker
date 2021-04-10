#ifndef TRANSACTION_H
#define TRANSACTION_H

// Holds all the neccessary info for transactions and what types of funds there
// are. Does no logic.

#include <istream>

enum class Transaction_Type { kOpen, kDeposit, kWithdraw, kTransfer, kHistory };

static char Transaction_Names[] { 'O', 'D', 'W', 'T', 'H' };

enum class Fund_Type { kMoneyMarket,
					   kPrimeMoneyMarket,
					   kLongTermBond,
					   kShortTermBond,
					   kFiveHundredIndexFund,
					   kCapitalValueFund,
					   kGrowthEquityFund,
					   kGrowthIndexFund,
					   kValueFund,
					   kValueStockIndex,
					   kNone };

static std::string Fund_Names[] { "Money Market",
								  "Prime Money Market",
								  "Long Term Bond",
								  "Short Term Bond",
								  "500 Index Fund",
								  "Capital Value Fund",
								  "Growth Equity Fund",
								  "Growth Index Fund",
								  "Value Fund",
								  "Value Stock Index",
								  "None" };


class Transaction {
 public:
	Transaction();

	friend std::ostream& operator<<(std::ostream& out_stream,
		const Transaction& transaction);

	// Setters
	void set_id(int);
	void set_id_transfer(int);
	void set_amount(int);
	void set_transaction_type(Transaction_Type);
	void set_fund_type(Fund_Type);
	void set_fund_transfer_type(Fund_Type);
	void set_last_name(std::string);
	void set_first_name(std::string);
	void set_validity(bool);

	// getters. If not used might be deleted
	int get_id();
	int get_id_transfer();
	int get_amount();
	Transaction_Type get_transaction_type();
	Fund_Type get_fund_type();
	Fund_Type get_fund_transfer_type();
	std::string get_last_name();
	std::string get_first_name();
	bool get_validity();


 private:
	int id_;
	int id_transfer_;

	Fund_Type fund_type_;
	Fund_Type fund_transfer_type_;

	int amount_;

	Transaction_Type transaction_type_;
	
	std::string first_name_;
	std::string last_name_;

	bool valid_transaction_;
};

#endif  // TRANSATION_H
