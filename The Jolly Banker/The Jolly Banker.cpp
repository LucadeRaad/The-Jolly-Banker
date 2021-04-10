// The Jolly Banker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "BSTree.h"
#include "Bank.h"

int main() {
    Bank jolly_banker("BankTransIn.txt");
    jolly_banker.ExecuteTransactions();

    //jolly_banker.get_tree().Display();
    jolly_banker.Display();

    return 0;
    //BSTree tree;
    //Account test1(1500, "joe", "example");
    //Account test2(1600, "sally", "test");
    //Account test3(1400, "bob", "exp");
    //Account test4(1300, "grug", "sample");
    //Account test5(1200, "mona", "lisa");
    //Account test6(1100, "fabio", "lanzoni");
    //Account test7(1000, "liz", "ard");
    //Account test8(1000, "liz2", "ard2");
    //tree.insert(&test1);
    //tree.insert(&test2);
    //tree.insert(&test3);
    //tree.insert(&test4);
    //tree.insert(&test5);
    //tree.insert(&test6);
    //tree.insert(&test7);
    //tree.insert(&test8);
    //tree.Display();
}
