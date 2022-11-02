// Eight_puzzle_CS170.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;        //needed for timer




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
void print(int* puzzle);     //prints puzzle
int Houristic(int* puzzle, int hour);   //calulates houristic value
int EMPTY(nodes* nodes);                //checks if there is no valid nodes left
int EXPAND(int* nowstate, int* expblock, int direct);   //expands the node needs to take in all directions
int CHECKDOPE(int STATE[]);                             //checks if the new node is a dupe
void ADDTOFRONT(nodes* addnode, nodes* stack, int direct,int heur); //adds the new node to the top
nodes* QUEUE();                                         //finds node with best houristic value

char direction[] = {'u','d','l','r'};       //tells which direction to expand
int invaldexpand[3][4] = { {0,6,0,2}, {1,7,3,5}, {2,8,6,8} }; //checks all 3 arrays for if they are invalid for direction[value]
//        0, 1, 2,      only go up if 0 is not at invalidexpand[1-3][1]
//        3, 4, 5,
//        6, 7, 8
int moves[4] = {-3,+3,-1,+1};                 //because puzzle is just a array moving up is -3 down +3 left -1 right 1

int maxdepth = 32;           //sets how much depth will be searched if it takes more then 31 moves then it doesnt not have a solution
nodes* top;
nodes* awnser;






int main()
{
    int hist = 2;
    const int size = 9;
    int updog = 1;
    int visited = 0;
    int queuecount = 0;
    //puzzle to solve
    int puzzle[size] = {1,3,6,
                        4,5,7,
                       8,0,2};
    top = newnode();
    int awnser[size];
    for (int i = 0; i < size; i++) {                                //sets top of stack to puzzle input
        top->STATE[i] = puzzle[i];
        awnser[i] = puzzle[i];
    }
    top->cost = Houristic(puzzle,hist);                             //sets top of houristic value

    

   auto start = high_resolution_clock::now();                       //starts timer

   // print(top->STATE);

    nodes* addnode = newnode();                                     //general node used to add to stack

    cout << "Uniform Cost: 1   Misplaced Tile:  2   Manhattan Distance: 3\n\n Choose Heuristic: ";
    cin >> hist;
    cout << "please wait\n";

    //loop
    while (updog == updog) {                                        //whats updog?
        
        nodes* node = QUEUE();                                      //gets best node from stack need to be on top ot avoid issues

        if (EMPTY(top)) {
            cout << "Failer no solution";                           //all nodes are not valid nothing left to do no solution
            return 0;
        }
        visited++;
        if (GOAL_TEST(node->STATE)) {
            cout << "Solution found!!!\n\n Puzzle:\n";
            print(awnser);
            int value;
            for (int i = 0; i < (node->str).size(); i++) {          //reexpands the starter puzzle to get solution output
                switch (node->str[i])                               //based on what str char is get the value to the direction
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


                EXPAND(puzzle, awnser, value);                      //expands the puzzle to next step
                cout << "Move "  << node->str[i] << endl;           //tells what movewas
                print(puzzle);                                      //prints new puzzle
                for (int i = 0; i < 9; i++) {                       //copys puzzle to awnser
                    awnser[i] = puzzle[i];
                }
            }
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<seconds>(stop - start);        //timer stuff

            cout << "Time taken by function: "                          //outputs results
                << duration.count() << " seconds" << endl;
            cout << visited << " nodes expanded\n";
            cout << node->totalcost << " Depth\n";
            cout << queuecount << " max queue\n";
            cout << "Solution " << node->str <<  "   u = up, d = down, l = left, r = right" << endl;
            return 0;
        }

       // print(node->STATE);


        for (int i = 0; i < 4; i++) {                                   //expand 4 times
            if (EXPAND(addnode->STATE, node->STATE, i) !=1) {
                continue;
            }
            if (CHECKDOPE(addnode->STATE) == 0) {                       //if node is not a dupe add to stack
                queuecount++;
                ADDTOFRONT(addnode, node, i, hist);
                addnode = newnode();
            }
        }
    
        //cout << "value " << Houristic(top->STATE,2) << endl;
        //cout << "str" << node->str << endl;



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
    int xcost = 0;
    int ycost = 0;
    int cost = 0;
    int* solution = goalstate;
    switch (hour) {                     //case 1 = uniform case 2 = misplaced, case 3 = manhattan
    case 1: 
        cost = 0;                       //returns cost of 0 means only depth matters
        break;
    case 2: 
        
        
        for (int i = 0; i < 9; i++) {
            if (((*(solution)) != (*(puzzle)))&&(*(puzzle)!=0)) {       //if the value of the puzzle isnt the solution means cost ++
                cost++;
            }
                solution++;
                puzzle++;
        }
        break;
    
    case 3: 
        
        for (int i = 0; i < 9; i++) {                               //calcs cost of each piece to move to right location
         


           // xcost += abs((i / 3) - (*(puzzle) / 3));
           // ycost += abs((i % 3) - (*(puzzle) % 3));
            puzzle++;
        }
        cost = xcost + ycost;
        break;
    default: 
        cout << "Houristic error";
        break;

    }
    return cost;
}

int EMPTY(nodes* nodes)                             
{
    if (nodes == NULL) {                            //if top of node is NULL its empty
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
    if (moveblock == 10) {          //didnt find it massive error
        return -1;
    }
    for (i = 0; i <= 2; i++) {
        if (moveblock == invaldexpand[i][direct]) {
            return -1;                              //cant expand 
        }
    }

    for (i = 0; i < 9; i++) {                       //copys node 
        newstate[i] = expblock[i];
    }
    newstate[moveblock] = newstate[moveblock + moves[direct]];  //moves new node piece to 0
    newstate[moveblock + moves[direct]] = 0;                    //replaces piece orignal location with 0
    return 1;  
}

int CHECKDOPE(int STATE[])
{
    nodes* start = top;                                         //starst at top of stack
    while (start != NULL) {                                     //untill end
        for (int i = 0; i < 9; i++) {                           //
            if (start->STATE[i] != STATE[i]) {                  //if state has any difference to a node end
                break;
            }if (i == 9) {                                      //if we checked all 9 spots then node is a dupe dont expand
                return 1;
            }
        }
        start = start->next;                                    //next node
    }
    return 0;
}

void ADDTOFRONT(nodes* addnode, nodes* stack, int direct,int heur)  //adds to front of stack
{
   

    addnode->next = top;                                //sets before top to next

    top = addnode;                                      //new top = addnode

    addnode->str = stack->str;                          //copys parent str
    addnode->str += direction[direct];                  //adds new movement


    addnode->totalcost = stack->totalcost + 1;          //adds one cost for new depth



    addnode->cost = addnode->totalcost + Houristic(addnode->STATE, heur);   //adds cost of node
}

nodes* QUEUE()
{
    int currentlowest = 5000;                   //if cost more than 5k its not happening
    nodes* return_node = NULL;                  //node to return as best
    nodes* traversenode = top;

    while (traversenode != NULL) {              //goes though stack
        if ((traversenode->valid == 1) && (traversenode->cost < currentlowest)) {       
            return_node = traversenode;                 //if lowest so far and valid new return
            currentlowest = traversenode->cost;
        }
        traversenode = traversenode->next;              //next on stack
    }
    
    if (return_node != NULL) {                          
        return_node->valid = 0;                         //been expanded
    }

    return return_node;                         //returns the node for test
}


