#ifndef BSTREE_H
#define BSTREE_H

// Creates a binary search tree that is tightly coupled with the accounts file.
// Can search insert and display the tree.

#include "Account.h"

class BSTree {
 public:
	BSTree();

	~BSTree();

	bool Insert(Account*); // TODO: check if account id already exists

	// retrieve object, first parameter is the ID of the account
	// second paratmer holds pointer ot found object, NULL if not found
	bool Retrieve(const int&, Account*&);

	//displays the contents of a tree to cout
    void Display() const;

	void Empty();
	bool isEmpty();

 private:
	struct Node {
	    Account* pAcct;
		Node* right;
		Node* left;
	};

	Node* root_;


	Node* InsertHelper(Node* node, Account* insert_account);

	void DisplayHelper(Node*) const;

	Node* RetrieveHelper(const int id, Node* node);

	void EmptyHelper(Node*);
};

#endif  // BSTREE_H