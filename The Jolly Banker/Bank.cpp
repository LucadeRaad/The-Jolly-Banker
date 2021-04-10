#include "Bank.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

Bank::Bank(std::string file_name) {
    buildQueue(file_name);
}

// Goes through the queue and executes transactions. Does not check to see if
// transactions are legitimate. Does actions centerting around the tree.
void Bank::ExecuteTransactions() {
    //std::cout << "queue size is: " << transaction_queue_.size() << std::endl; // DEBUG
    while (!transaction_queue_.empty()) {
        //transaction_queue_.front().execute(tree_);
        Transaction transaction = transaction_queue_.front();

        if (transaction.get_validity()) {
            switch (transaction.get_transaction_type()) {
            case Transaction_Type::kOpen: {
                // debug
                //std::cout << "opening account " << transaction.get_id() << " for: " << transaction.get_first_name() << " " << transaction.get_last_name() << std::endl;
                Account* insert_account = new Account(transaction.get_id(), transaction.get_first_name(), transaction.get_last_name());
                tree_.Insert(insert_account);
                break;
            }
            case Transaction_Type::kDeposit: {
                //debug
                //std::cout << "depositing in account " << transaction.get_id() << " in fund: " << (int)transaction.get_fund_type() << " for: " << transaction.get_amount() << std::endl;
                Account* deposit;
                bool valid_transaction = tree_.Retrieve(transaction.get_id(), deposit);

                if (!valid_transaction) {
                    std::cerr << "ERROR: Account " << transaction.get_id() << " not found. Deposit refused." << std::endl;
                    break;
                }

                deposit->Deposit(transaction);
                break;
            }
            case Transaction_Type::kWithdraw: {
                // debug
                //std::cout << "withdrawing from account " << transaction.get_id() << " in fund: " << (int)transaction.get_fund_type() << " for: " << transaction.get_amount() << std::endl;
                Account* withdraw;
                bool valid_transaction = tree_.Retrieve(transaction.get_id(), withdraw);

                if (!valid_transaction) {
                    std::cout << "ERROR: Account " << transaction.get_id() << " not found. Withdrawal refused." << std::endl;
                    break;
                }

                withdraw->Withdraw(transaction);
                break;
            }
            case Transaction_Type::kTransfer: {
                Account* transfer;
                bool valid_transaction = tree_.Retrieve(transaction.get_id(), transfer);
                if (!valid_transaction) {
                    std::cout << "ERROR: Account " << transaction.get_id() << " not found. Transfer refused." << std::endl;
                    break;
                }

                transfer->Transfer(transaction);

                break;
            }
            case Transaction_Type::kHistory: {
                Account* history;
                bool valid_transaction = tree_.Retrieve(transaction.get_id(), history);

                if (!valid_transaction) {
                    std::cout << "ERROR: Account " << transaction.get_id() << " not found. History refused." << std::endl;
                    break;
                }

                std::cout << "Transaction History for " << history->get_name();
                // If a transaction's type is none, display all funds
                if (transaction.get_fund_type() != Fund_Type::kNone) {
                    // debug
                    //std::cout << "history from account " << transaction.get_id() << " in fund: " << (int)transaction.get_fund_type() << std::endl;
                    std::cout << Fund_Names[(int)transaction.get_fund_type()] << ": $" << history->get_fund_amount(transaction.get_fund_type()) << std::endl;
                    history->DisplayHistoryOneFund(transaction.get_fund_type());
                }
                else {
                    std::cout << " by fund." << std::endl;
                    history->DisplayHistoryAllFunds();
                }
                break;

            } default:
                std::cerr << "Unexpected Transaction type" << std::endl;
            }
        }
        transaction_queue_.pop();
    }
}

void Bank::Display() {
    std::cout << std::endl;
    tree_.Display();
}

// Creates the queue. It takes the raw input from the txt file and creates
// transactions. Erronious transactions are added to the que but are flagged
// as such.
bool Bank::buildQueue(std::string file_name) {
    std::ifstream in_file;

    in_file.open(file_name);
    if (in_file.is_open()) {
        while (!in_file.eof()) {
            char transaction_enum = 'X';
            in_file >> transaction_enum;

            transaction_enum = toupper(transaction_enum);
            //std::cout << "transaction enum: " << transaction_enum << std::endl;
            
            switch (transaction_enum) {
                // Opening an account requires two strings (first and last) followed by a 4 digit number (id)
			    case 'O': {
                    Transaction input_transaction;
		    		input_transaction.set_transaction_type(Transaction_Type::kOpen);

	    			std::string first_name;
    				std::string last_name;
                    in_file >> last_name;
                    in_file >> first_name;

                    input_transaction.set_first_name(first_name);
                    input_transaction.set_last_name(last_name);

				    std::string account_id;
				    in_file >> account_id;

                    int int_account_id = SafeStringToInt(account_id);

                    if (int_account_id < 0) {
                        input_transaction.set_validity(false);
                        transaction_queue_.push(input_transaction);

                        break;
                    }

				    if (int_account_id > 9999) {
					    std::cerr << "ERROR: Account " << int_account_id << " invalid. Transaction refused." << std::endl;
                        input_transaction.set_validity(false);
                        transaction_queue_.push(input_transaction);
					    break;
				    }

                    input_transaction.set_validity(true);

				    input_transaction.set_id(int_account_id);
                    transaction_queue_.push(input_transaction);

			    	break;
		    	} 
                // Depositing in an account requires a 5 digit number(first 4 are the id and last 1 is the fund) and the amount
                case 'D': {
                    Transaction input_transaction;
    				input_transaction.set_transaction_type(Transaction_Type::kDeposit);

                    std::string id_and_fund;
                    in_file >> id_and_fund;

                    int int_id_and_fund = SafeStringToInt(id_and_fund);

                    if (int_id_and_fund < 0) {
                        input_transaction.set_validity(false);
                        transaction_queue_.push(input_transaction);

                        break;
                    }

                    if (stoi(id_and_fund) < 10000 || stoi(id_and_fund) > 99999) {
                        std::cerr << "ERROR: input " << id_and_fund << " for depositing is not right. Deposit refused." << std::endl;

                        input_transaction.set_validity(false);
                        transaction_queue_.push(input_transaction);
                        break;
                    }

                    int account_id = stoi(id_and_fund.substr(0, 4));

                    input_transaction.set_id(account_id);

                    int fund = stoi(id_and_fund.substr(4, 4));
                    input_transaction.set_fund_type((Fund_Type)fund);

                    std::string amount;
                    in_file >> amount;

                    int int_amount = SafeStringToInt(amount);

                    if (int_amount < 0) {
                        input_transaction.set_validity(false);
                        transaction_queue_.push(input_transaction);

                        break;
                    }

                    input_transaction.set_amount(int_amount);

                    input_transaction.set_validity(true);

                    transaction_queue_.push(input_transaction);
                    break;
                }
                // Withdrawal in an account requires a 5 digit number(first 4 are the id and last 1 is the fund) and the amount
                case 'W': {
                    Transaction input_transaction;
			    	input_transaction.set_transaction_type(Transaction_Type::kWithdraw);

                    std::string id_and_fund;
                    in_file >> id_and_fund;

                    int int_id_and_fund = SafeStringToInt(id_and_fund);

                    if (int_id_and_fund < 0) {
                        input_transaction.set_validity(false);
                        transaction_queue_.push(input_transaction);

                        break;
                    }

                    if (stoi(id_and_fund) < 10000 || stoi(id_and_fund) > 99999) {
                        std::cerr << "ERROR: input " << id_and_fund << " for withdrawal is not right. Withdrawal refused." << std::endl;

                        input_transaction.set_validity(false);
                        transaction_queue_.push(input_transaction);
                        break;
                    }

                    int account_id = stoi(id_and_fund.substr(0, 4));

                    input_transaction.set_id(account_id);

                    int fund = stoi(id_and_fund.substr(4, 4));
                    input_transaction.set_fund_type((Fund_Type)fund);

                    std::string amount;
                    in_file >> amount;

                    int int_amount = SafeStringToInt(amount);

                    if (int_amount < 0) {
                        input_transaction.set_validity(false);
                        transaction_queue_.push(input_transaction);

                        break;
                    }

                    input_transaction.set_amount(int_amount);

                    input_transaction.set_validity(true);

                    transaction_queue_.push(input_transaction);
                    break;
		    	}
                // Transfering requires a 5 digit number of id and fund
                // followed by an amount followed by another 5 digit number
                case 'T': {
                    Transaction input_transaction;
			    	input_transaction.set_transaction_type(Transaction_Type::kTransfer);

                    std::string id_and_fund;
                    in_file >> id_and_fund;

                    int int_id_and_fund = SafeStringToInt(id_and_fund);

                    if (int_id_and_fund < 0) {
                        input_transaction.set_validity(false);
                        transaction_queue_.push(input_transaction);

                        break;
                    }

                    if (int_id_and_fund > 99999 || int_id_and_fund < 10000) {
                        std::cerr << "ERROR: Account "
                                  << int_id_and_fund
                                  << " invalid. Transfer refused."
                                  << std::endl;
                        input_transaction.set_validity(false);
                        transaction_queue_.push(input_transaction);

                        break;
                    }

                    input_transaction.set_id(stoi(id_and_fund.substr(0, 4)));
                    input_transaction.set_fund_type((Fund_Type)stoi(id_and_fund.substr(4, 4)));

                    std::string amount;
                    in_file >> amount;

                    int int_amount = SafeStringToInt(amount);

                    if (int_amount < 0) {
                        input_transaction.set_validity(false);
                        transaction_queue_.push(input_transaction);

                        break;
                    }

                    input_transaction.set_amount(int_amount);

                    // Set up the transfer account id and funds

                    std::string transfer_id_and_fund;
                    in_file >> transfer_id_and_fund;

                    int int_transfer_id_and_fund = 
                        SafeStringToInt(transfer_id_and_fund);

                    if (int_transfer_id_and_fund < 0) {
                        input_transaction.set_validity(false);
                        transaction_queue_.push(input_transaction);

                        break;
                    }

                    if (int_transfer_id_and_fund > 99999 || int_transfer_id_and_fund < 10000) {
                        std::cerr << "ERROR: Account "
                                  << int_transfer_id_and_fund
                                  << " invalid. Transfer refused." << std::endl;
                        input_transaction.set_validity(false);
                        transaction_queue_.push(input_transaction);

                        break;
                    }

                    input_transaction.set_id_transfer(stoi(transfer_id_and_fund.substr(0, 4)));
                    input_transaction.set_fund_transfer_type((Fund_Type)stoi(transfer_id_and_fund.substr(4, 4)));

                    input_transaction.set_validity(true);

                    transaction_queue_.push(input_transaction);
		    		break;
	    		}
                // History requires either a 5 or 4 digit number. If a 4 digit
                // number is supplied the entire account's history is told.
                // Otherwise only the fund specified is displayed.
                case 'H': {
                    Transaction input_transaction;
			    	input_transaction.set_transaction_type(Transaction_Type::kHistory);

                    std::string id_and_fund;
                    in_file >> id_and_fund;

                    int int_id_and_fund = SafeStringToInt(id_and_fund);

                    if (int_id_and_fund > 99999 || int_id_and_fund < 1000) {
                        std::cerr << "ERROR: Account " << int_id_and_fund << " invalid. History refused." << std::endl;
                        input_transaction.set_validity(false);
                        transaction_queue_.push(input_transaction);

                        break;
                    }

                    //std::cout << "the id and fund are: " << int_id_and_fund << std::endl; //debug
                    if (int_id_and_fund > 9999) {
                        // This means a fund is added to the end of the history acct
                        input_transaction.set_id(stoi(id_and_fund.substr(0, 4)));
                        input_transaction.set_fund_type((Fund_Type)stoi(id_and_fund.substr(4, 4)));
                    } else {
                        input_transaction.set_id(int_id_and_fund);
                    }

                    input_transaction.set_validity(true);
                    transaction_queue_.push(input_transaction);
		    		break;

	    		} default: {
				    //std::cerr << "Input is not a correct transaction!" << std::endl;
			    }
			}
        }
        in_file.close();
        return true;
    } else {
        std::cout << "File: " << file_name << " not found." << std::endl;
        return false;
    }
}

// Checks that a string is actually ints or not.
int Bank::SafeStringToInt(std::string input) {
    int output = 0;
    std::istringstream iss(input);
    // iostream::>> does two things- returns a boolean if conversion was successful and extracts int to "val"
    if (!(iss >> output)) {
        std::cerr << "ERROR: int expected: " << input << std::endl;

        return -1;
    }
    return output;
}
