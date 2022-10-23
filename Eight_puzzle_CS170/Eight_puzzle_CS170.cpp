// Eight_puzzle_CS170.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

using namespace std;





int goalstate[9] = { 1,2,3,
                     4,5,6,
                     7,8,0 };

struct nodes
{
    int STATE[9];          //puzzle
    char* str;              //solution tree  ex. udlrr = up down left right right
    int cost;               //calculated cost to solve
    bool valid;              //weather node is valid  
    int totalcost;          //totalcost to get to node ie. how many moves
    nodes* next;             //next node
};

bool GOAL_TEST(int* puzzle);        //checks if node is solution
nodes* newnode();            //makes new node

int maxdepth = 32;           //sets how much depth will be searched if it takes more then 31 moves then it doesnt not have a solution
nodes* top;


int main()
{
    const int size = 9;
    
    //puzzle to solve
    int puzzle[size] = {1,2,3,
                        4,5,6,
                        7,8,0};
    top = newnode();
    for (int i = 0; i < size; i++) {
        top->STATE[i] = puzzle[i];
    }

    
    cout << GOAL_TEST(puzzle);
    if (GOAL_TEST(top->STATE)) {
        cout << "correct";
    }
    
}


//checks if the node is the solution
bool GOAL_TEST(int* puzzle) {
    int* solution = goalstate;

    //checks node puzzle against solution
    for (int i = 0; i < 9; i++) {
        if ((*(solution++)) != (*(puzzle++))){
            return 0;               //not solution
        }

    }

    return 1;                       //solution found
}

nodes* newnode()
{
    nodes* newnode = new nodes;
    if (newnode == NULL) { 
        return NULL; 
    }
    newnode->valid = 1;
    newnode->str = new char[maxdepth + 1];
    if (newnode->str == NULL) {
        return NULL;
    }
    newnode->str[0] = 0;
    newnode->cost = 0;
    newnode->totalcost = 0;
    newnode->next = NULL;
    return newnode;
}

