#ifndef ACCOUNT_H
#define ACCOUNT_H

// Holds multiple funds and has functions on modifying funds.

#include <string>
#include <vector>

#include "Fund.h"

const int kNumberOfFunds = 10;

class Account {
public:
    Account(int id, std::string first_name, std::string last_name);

    int get_id() const;

    std::string get_name() const;

    int get_fund_amount(Fund_Type fund_type);

    void Deposit(Transaction transaction);
    void Withdraw(Transaction transaction);
    void Transfer(Transaction transaction);

    bool SplitWithdrawTransactionIfNeeded(Transaction transaction, Fund_Type main_fund, Fund_Type alt_fund);

    void DisplayFunds();

    void DisplayHistoryOneFund(Fund_Type fund_type);

    void DisplayHistoryAllFunds();

    // For BSTree search. Remove ones that aren't needed. For ID checks
    bool operator>(const Account& rhs) const;

    bool operator<(const Account& rhs) const;

    bool operator==(const Account& rhs) const;
    bool operator!=(const Account& rhs) const;

 private:
    int id_;

    std::string first_name_;
    std::string last_name_;

    std::vector<Fund> funds_;
};

#endif  // ACCOUNT_H
