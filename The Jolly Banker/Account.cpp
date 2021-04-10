#include "Account.h"
#include <iostream>

Account::Account(int id, std::string first_name, std::string last_name) {
    id_ = id;
    first_name_ = first_name;
    last_name_ = last_name;

    funds_.resize(kNumberOfFunds);
}

int Account::get_id() const {
    return id_;
}

std::string Account::get_name() const {
    return first_name_ + " " + last_name_;
}

int Account::get_fund_amount(Fund_Type fund_type) {
    return funds_[(int)fund_type].get_amount();
}

// Deposits money into a specific fund. All deposits are recorded.
void Account::Deposit(Transaction transaction) {
    //std::cout << "within the deposit function" << std::endl;
    funds_[(int)transaction.get_fund_type()].Deposit(transaction);
}

// Withdraws from a fund. Fails if there is not enough money to withdraw from.
// Certain funds are linked and withdrawing from a fund without enough money
// will withdraw from the linked fund if enough is in the linked fund. All
// withdraws are recorded even if they fail.
void Account::Withdraw(Transaction transaction) {
    bool did_split = false;

    switch (transaction.get_fund_type()) {
    case Fund_Type::kMoneyMarket:
        did_split = SplitWithdrawTransactionIfNeeded(transaction, Fund_Type::kMoneyMarket, Fund_Type::kPrimeMoneyMarket);
        break;
    case Fund_Type::kPrimeMoneyMarket:
        did_split = SplitWithdrawTransactionIfNeeded(transaction, Fund_Type::kPrimeMoneyMarket, Fund_Type::kMoneyMarket);
        break;
    case Fund_Type::kShortTermBond:
        did_split = SplitWithdrawTransactionIfNeeded(transaction, Fund_Type::kShortTermBond, Fund_Type::kLongTermBond);
        break;
    case Fund_Type::kLongTermBond:
        did_split = SplitWithdrawTransactionIfNeeded(transaction, Fund_Type::kLongTermBond, Fund_Type::kShortTermBond);
        break;
    default:
        break;
    }
    
    if (!did_split) {
        funds_[(int)transaction.get_fund_type()].Withdraw(transaction, get_name());
    }
}

// Transfers from one fund to another. The funds can be from different accounts
// and history will be stored on each fund's history. The transfer will only
// succeed if
void Account::Transfer(Transaction transaction) {
    if (transaction.get_id() == transaction.get_id_transfer() && transaction.get_fund_type() == transaction.get_fund_transfer_type()) {
        std::cerr << "ERROR: cannot transfer from " << transaction.get_id() << (int)transaction.get_fund_type() << " to " << transaction.get_id_transfer() << (int)transaction.get_fund_transfer_type() << ". Transferal refused" << std::endl;
        transaction.set_validity(false);
    }

    bool successful_withdraw = funds_[(int)transaction.get_fund_type()].Transfer(transaction);

    if (successful_withdraw) {
        funds_[(int)transaction.get_fund_transfer_type()].Deposit(transaction);
    }
}

// Certain funds are linked together and if a withdraw is called on one linked
// fund then it can take from the other linked fund. This function returns
// true if the withdraw transaction is split and automatically withdraws.
// Returns false and does nothing if otherwise. If successful records two
// transactions.
bool Account::SplitWithdrawTransactionIfNeeded(Transaction transaction, Fund_Type main_fund, Fund_Type linked_fund) {
    if (transaction.get_amount() > funds_[(int)main_fund].get_amount()) {
        if (transaction.get_amount() < funds_[(int)main_fund].get_amount() + funds_[(int)linked_fund].get_amount()) {
            Transaction main_transaction;
            Transaction linked_transaction;
            
            main_transaction.set_id(transaction.get_id());
            linked_transaction.set_id(transaction.get_id());

            main_transaction.set_fund_type(main_fund);
            linked_transaction.set_fund_type(linked_fund);

            main_transaction.set_validity(true);
            linked_transaction.set_validity(true);

            main_transaction.set_transaction_type(transaction.get_transaction_type());
            linked_transaction.set_transaction_type(transaction.get_transaction_type());

            main_transaction.set_amount(funds_[(int)main_fund].get_amount());
            linked_transaction.set_amount(transaction.get_amount() - funds_[(int)main_fund].get_amount());

            // These withdraws should always work so no need to pass the name of the account
            funds_[(int)main_transaction.get_fund_type()].Withdraw(main_transaction, "");
            funds_[(int)linked_transaction.get_fund_type()].Withdraw(linked_transaction, "");

            return true;
        }
    }
    
    return false;
}

void Account::DisplayFunds() {
    std::cout << first_name_ << " " << last_name_ << " Account ID: " << id_ << std::endl;
    std::cout << "    Money Market: $" << funds_[(int)Fund_Type::kMoneyMarket].get_amount() << std::endl;
    std::cout << "    Prime Money Market: $" << funds_[(int)Fund_Type::kPrimeMoneyMarket].get_amount() << std::endl;
    std::cout << "    Long-Term Bond: $" << funds_[(int)Fund_Type::kLongTermBond].get_amount() << std::endl;
    std::cout << "    Short-Term Bond: $" << funds_[(int)Fund_Type::kShortTermBond].get_amount() << std::endl;
    std::cout << "    500 Index Fund: $" << funds_[(int)Fund_Type::kFiveHundredIndexFund].get_amount() << std::endl;
    std::cout << "    Capital Value Fund: $" << funds_[(int)Fund_Type::kCapitalValueFund].get_amount() << std::endl;
    std::cout << "    Growth Equity Fund: $" << funds_[(int)Fund_Type::kGrowthEquityFund].get_amount() << std::endl;
    std::cout << "    Growth Index Fund: $" << funds_[(int)Fund_Type::kGrowthIndexFund].get_amount() << std::endl;
    std::cout << "    Value Fund: $" << funds_[(int)Fund_Type::kValueFund].get_amount() << std::endl;
    std::cout << "    Value Stock Index: $" << funds_[(int)Fund_Type::kValueStockIndex].get_amount() << std::endl;
}

void Account::DisplayHistoryOneFund(Fund_Type fund_type) {
    funds_[(int)fund_type].DisplayHistory();
}

void Account::DisplayHistoryAllFunds() {
    for (size_t i = 0; i < funds_.size(); i++) {
        if (funds_[i].HasHistory()) {
            std::cout << Fund_Names[i] << ": $" << funds_[i].get_amount() << std::endl;
            funds_[i].DisplayHistory();
        }
    }
}

// Only checks account IDs
bool Account::operator>(const Account& rhs) const {
    return id_ > rhs.id_;
}

// Only checks account IDs
bool Account::operator<(const Account& rhs) const {
    return id_ < rhs.id_;
}

// Only checks account IDs
bool Account::operator==(const Account& rhs) const {
    return id_ == rhs.id_;
}

// Only checks account IDs
bool Account::operator!=(const Account& rhs) const {
    return id_ != rhs.id_;
}
