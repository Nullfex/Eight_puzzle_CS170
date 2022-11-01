// Eight_puzzle_CS170.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;

//#pragma warning(disable : 4996)


int goalstate[9] = { 1,2,3,
                     4,5,6,
                     7,8,0 };

struct nodes                //i named the struct nodes so top will refer to nodes from example
{
    int STATE[9];          //puzzle
    string str;              //solution tree  ex. udlrr = up down left right right
    int cost;               //calculated cost to solve
    bool valid;              //weather node is valid  
    int totalcost;          //totalcost to get to node ie. how many moves pathcost
    nodes* next;             //next node
};

bool GOAL_TEST(int* puzzle);        //checks if node is solution
nodes* newnode();            //makes new node
void print(int* puzzle);
int Houristic(int* puzzle, int hour);
int EMPTY(nodes* nodes);
int EXPAND(int* nowstate, int* expblock, int direct);
int CHECKDOPE(int STATE[]);
void ADDTOFRONT(nodes* addnode, nodes* stack, int direct,int heur);
nodes* QUEUE();

char direction[] = {'u','d','l','r'};       //tells which direction to expand
int invaldexpand[3][4] = { {0,6,0,2}, {1,7,3,5}, {2,8,6,8} }; //checks all 3 arrays for if they are invalid for direction[value]
//        0, 1, 2,      only go up if 0 is not at invalidexpand[1-3][1]
//        3, 4, 5,
//        6, 7, 8
int moves[4] = {-3,3,-1,1};                 //because puzzle is just a array moving up is -3 down +3 left -1 right 1

int maxdepth = 32;           //sets how much depth will be searched if it takes more then 31 moves then it doesnt not have a solution
nodes* top;
nodes* awnser;

auto start = high_resolution_clock::now();

int main()
{
    int hist = 1;
    const int size = 9;
    int updog = 1;
    int visited = 0;
    int queuecount = 0;
    //puzzle to solve
    int puzzle[size] = {1,3,6,
                        5,0,2,
                        4,7,8};
    top = newnode();
    int awnser[size];
    for (int i = 0; i < size; i++) {
        top->STATE[i] = puzzle[i];
        awnser[i] = puzzle[i];
    }
    top->cost = Houristic(puzzle,hist);

    
    //cout << GOAL_TEST(puzzle);
    if (GOAL_TEST(top->STATE)) {
        cout << "correct\n";
    }

    

    print(top->STATE);

    nodes* addnode = newnode();

    //loop
    while (updog == updog) {
        
       nodes* node = QUEUE();

        if (EMPTY(top)) {
            cout << "Failer no solution";
            return 0;
        }
        visited++;
        if (GOAL_TEST(node->STATE)) {
            cout << "Solution found!!!\n\n Puzzle:\n";
            print(awnser);
            int value;
            for (int i = 0; i < (node->str).size(); i++) {
                switch (node->str[i])
                {
                case 'u':value = 0;
                    break;
                case 'd':value = 1;
                    break;
                case 'l':value = 2;
                    break;
                case 'r':value = 3;
                    break;
                }


                EXPAND(puzzle, awnser, value);
                cout << "Move "  << node->str[i] << endl;
                print(puzzle);
                for (int i = 0; i < 9; i++) {
                    awnser[i] = puzzle[i];
                }
            }
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<seconds>(stop - start);

            cout << "Time taken by function: "
                << duration.count() << " seconds" << endl;
            cout << visited << " nodes expanded\n";
            cout << node->totalcost << " Depth\n";
            cout << queuecount << " max queue\n";
            cout << "Solution " << node->str << endl;
            return 0;
        }

        print(node->STATE);


        for (int i = 0; i < 4; i++) {
            if (EXPAND(addnode->STATE, node->STATE, i) !=1) {
                continue;
            }
            if (CHECKDOPE(addnode->STATE) == 0) {
                queuecount++;
                ADDTOFRONT(addnode, node, i, hist);
                addnode = newnode();
            }
        }

         

        







        cout << "value " << Houristic(top->STATE,2) << endl;
        cout << "str" << node->str << endl;



    }

        return 0;

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
    newnode->str = "";
    //if (newnode->str == NULL) {
    //    return NULL;
    //}
    //newnode->str = EMPTY;
    newnode->cost = 0;
    newnode->totalcost = 0;
    newnode->next = NULL;
    return newnode;
}

void print(int* puzzle)
{
    int o = 0;
    for (int i = 0; i < 9; i++) {
        cout << *(puzzle++) << "  ";        //cout array val
        if (o == 2) {
            cout << "\n";                   //drops line every 3 outputs needs to be changed 
            o = 0;
        }
        else {
            o++;
        }
    }
    cout << "\n\n";     //drops some lines

}

int Houristic(int* puzzle, int hour)    //holds the houristic takes in the puzzle to test and hour to choose
{
    int cost = 0;
    int* solution = goalstate;
    switch (hour) {
    case 1: 
        cost = 0;
        break;
    case 2: 
        
        
        for (int i = 0; i < 9; i++) {
            if (((*(solution)) != (*(puzzle)))&&(*(puzzle)!=0)) {
                cost++;
            }
                solution++;
                puzzle++;
        }
        break;
    
    case 3: 
        
        cout << "went here";

        break;
    default: 
        cout << "Houristic error";
        break;

    }
    return cost;
}

int EMPTY(nodes* nodes)
{
    if (nodes == NULL) {
        return 1;
    }
    else {
        return 0;
    }
}

int EXPAND(int* newstate, int* expblock, int direct)
{
    int i;
    int moveblock = 10;
    for (i = 0; i < 9; i++) {       //finds location of 0 and sets it to moveblock
        if (expblock[i] == 0) {
            moveblock = i;
            break;
        }
    }
    if (moveblock == 10) {
        return -1;
    }
    for (i = 0; i < 2; i++) {
        if (moveblock == invaldexpand[i][direct]) {
            return -1;                              //cant expand 
        }
    }

    for (i = 0; i < 9; i++) {
        newstate[i] = expblock[i];
    }
    newstate[moveblock] = newstate[moveblock + moves[direct]];
    newstate[moveblock + moves[direct]] = 0;
    return 1;
}

int CHECKDOPE(int STATE[])
{
    nodes* start = top;
    while (start != NULL) {
        for (int i = 0; i < 9; i++) {
            if (start->STATE[i] != STATE[i]) {
                break;
            }if (i == 9) {
                return 1;
            }
        }
        start = start->next;
    }
    return 0;
}

void ADDTOFRONT(nodes* addnode, nodes* stack, int direct,int heur)
{
   

    addnode->next = top;

    top = addnode;

    addnode->str = stack->str;
    addnode->str += direction[direct];

    //addnode->str[stack->totalcost + 1] = 0;

    addnode->totalcost = stack->totalcost + 1;


   /* addnode->totalcost = (stack->totalcost + 1);
    
    addnode->str[stack->totalcost] = direction[direct];
    addnode->str[stack->totalcost + 1] = 0;*/

    addnode->cost = addnode->totalcost + Houristic(addnode->STATE, heur);
}

nodes* QUEUE()
{
    int currentlowest = 5000;
    nodes* return_node = NULL;
    nodes* traversenode = top;

    while (traversenode != NULL) {
        if ((traversenode->valid == 1) && (traversenode->cost < currentlowest)) {
            return_node = traversenode;
            currentlowest = traversenode->cost;
        }
        traversenode = traversenode->next;
    }
    
    if (return_node != NULL) {
        return_node->valid = 0;                         //been visted
    }

    return return_node;
}


