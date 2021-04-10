#include "BSTree.h"
#include <iostream>

BSTree::BSTree() {
	root_ = nullptr;
}

BSTree::~BSTree() {
	Empty();
}

bool BSTree::Insert(Account* insert_account) {
	if (isEmpty()) {
		root_ = new Node;
		root_->pAcct = insert_account;
		root_->left = nullptr;
		root_->right = nullptr;
		return true;
	}

	Node* insert_pointer = root_;
	// When the loop reaches the end the trailing pointer stores the node that 
	// will point to the new node
	Node* trailing_pointer = NULL;

	// Pointer is now right before where it needs to be
	while (insert_pointer != nullptr) {
		trailing_pointer = insert_pointer;

		if (*trailing_pointer->pAcct == *insert_account) {
			//std::cout << "a duplicate was found" << std::endl;
			return false;
		}

		if (*insert_account < * insert_pointer->pAcct) {
			insert_pointer = insert_pointer->left;
		} else {
			insert_pointer = insert_pointer->right;
		}
	}

	Node* insert_node = new Node;
	insert_node->pAcct = insert_account;
	insert_node->left = nullptr;
	insert_node->right = nullptr;

	if (trailing_pointer == nullptr) {
		trailing_pointer = insert_node;
	}

	if (*insert_account < *trailing_pointer->pAcct) {
		trailing_pointer->left = insert_node;
	} else {
		trailing_pointer->right = insert_node;
	}

	return false;
}

// Returns false if account is not found. Returns true if account is found and
// the account is returned in the "output" account object.
bool BSTree::Retrieve(const int& id, Account*& output) {
	if (RetrieveHelper(id, root_)->pAcct == nullptr) {
		return false;
	}
	output = (RetrieveHelper(id, root_))->pAcct;

	return true;
}

// Prints the tree. In order for clarity.
void BSTree::Display() const {
	std::cout << "Processing Done. Final Balances" << std::endl;
	DisplayHelper(root_);
}

void BSTree::Empty() {
	EmptyHelper(root_);
	root_ = nullptr;
}

bool BSTree::isEmpty() {
	return root_ == nullptr;
}

void BSTree::DisplayHelper(Node* node) const {
	if (node == nullptr) {
		return;
	}

	DisplayHelper(node->left);
	node->pAcct->DisplayFunds();
	DisplayHelper(node->right);
}

// Recursive. Finds the node that holds the account with the same id.
BSTree::Node* BSTree::RetrieveHelper(const int id, Node* node) {
	if (node == nullptr || node->pAcct->get_id() == id) {
		return node;
	}

	if (node->pAcct->get_id() > id) {
		return RetrieveHelper(id, node->left);
	} else {
		return RetrieveHelper(id, node->right);
	}
}

// Recursive. Goes and deletes dynamically allocated memory
void BSTree::EmptyHelper(Node* node) {
	if (node == nullptr) {
		return;
	}
	//std::cout << "tried to delete: " << node->pAcct->getName() << std::endl;
	EmptyHelper(node->left);
	node->left = nullptr;

	EmptyHelper(node->right);
	node->right = nullptr;
	
	delete node->pAcct;
	node->pAcct = nullptr;

	delete node;
	node = nullptr;
}

BSTree::Node* BSTree::InsertHelper(Node* node, Account* insert_account) {
	if (node == nullptr) {
		Node* output = new Node();
		output->left = nullptr;
		output->right = nullptr;
		output->pAcct = insert_account;
		return output;
	}

	// If the id already exists then the node needs to not be made
	if (*insert_account == *node->pAcct) {
		std::cerr << "ERROR: Account " << insert_account->get_id() << " is already open. Transaction refused." << std::endl;
		return node;
	}

	if (*insert_account > *node->pAcct) {
		node->right = InsertHelper(node->right, insert_account);
	} else {
		node->left = InsertHelper(node->right, insert_account);
	}
	return node;
}
