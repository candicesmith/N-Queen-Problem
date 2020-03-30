#include <iostream>
#include <queue>
#include <list>
#include <chrono>
#include <time.h>
#include <random>
#include <cmath>
using namespace std;

vector<__int8> startState(int n) { // creates empty board
    vector<__int8> graph;

    for (int j = 0; j < n; j++) {
        graph.push_back(-1);
    }
    return graph;
}

vector<__int8> generateBoard(const int n) { // generates a board with random configuration of queens
    vector<__int8> board;

    random_device device;
    mt19937 generator(device());
    uniform_int_distribution<int> distribution(0, n - 1);

    for (int j = 0; j < n; j++) {
        board.push_back(distribution(generator));
    }
    return board;
}

void printBoard(const int n, vector<__int8>& board) { //prints board
    for (int k = 0; k < n; k++) {
        cout << "----";
    }
    cout << "-" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (j == board[i])
                cout << "| Q ";
            else
                cout << "|   ";
        }
        cout << "|" << endl;
        for (int k = 0; k < n; k++) {
            cout << "----";
        }
        cout << "-" << endl;
    }
    cout << endl;
}

bool validMove(const int depth, vector<__int8>& board, int j) { // checks if a move is valid for BFS
    for (int i = 0; i < depth; i++) {
        if (board[i] == j)
            return false;
        if (board[i] + depth - i == j || board[i] - depth + i == j)
            return false;
    }
    return true;
}

int fitnessScore(const int n, vector<__int8>& board) { // returns number of attacking queens
    int fitness = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (board[i] == board[j]) // checks for attacking queens in columns
                fitness++;
            if (board[i] - (j - i) == board[j]) // checks for attacking queens in right diagonal
                fitness++;
            if (board[i] + (j - i) == board[j]) // checks for attacking queens in left diagonal
                fitness++;
        }
    }
    return fitness;
}

vector<vector<__int8>> generateNeighbours(const int n, vector<__int8>& board) { // generates a neighbourhood of boards
    vector<__int8> neighbour;
    vector<vector<__int8>> neighbourhood;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            neighbour = board;
            if (neighbour[i] != j) {
                neighbour[i] = j;
                neighbourhood.push_back(neighbour);
            }
        }
    }
    return neighbourhood;
}

void bfsWithPruning(const int n, list<vector<__int8>> q) {
    chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();

    if (n == 2 || n == 3) { // no solutions exist for 2 and 3 
        end = std::chrono::system_clock::now();
        cout << "No solutions exist for " << n << " queens." << endl;
        system("pause");
        return;
    }
    if (n == 1) { // simply returns a queen
        end = std::chrono::system_clock::now();
        chrono::duration<double> totalTime = end - start;
        cout << "Q" << endl;
        cout << n << " Queens" << endl;
        cout << "Goals found: 1" << endl;
        cout << "Time: " << totalTime.count() << " seconds" << endl;
        system("pause");
        return;
    }

    vector<__int8> currentBoard = startState(n); //creates empty board

    int depth = 0;

    for (int j = 0; j < n; j++) { //starting from empty board 
        currentBoard[depth] = j; //add queen to board
        q.push_back(currentBoard); //push new board to queue
        currentBoard[depth] = -1; //resets board back to parent state
    }
    depth++;
    int parents = n;
    int count = 0;
    int goals = 0;

    while (!q.empty()) {
        __int64 k = 0; //how many times it will pop a node on the current layer

        while (k < parents) { // iterates for number of nodes in the layer
            currentBoard = q.front(); // grabs next in queue

            for (int j = 0; j < n; j++) { //pruning
                currentBoard[depth] = j; //generates next move
                if (validMove(depth, currentBoard, j)) {
                    q.push_back(currentBoard); // if move is valid pushes to queue
                    currentBoard[depth] = -1; // resets to parent state
                    count++; // counts how many nodes were created
                }
            }
            q.pop_front(); // pops parent node once finished
            k++;
        }
        depth++; // tracks dept of tree
        parents = count; // sets count for number of nodes in the next layer
        count = 0;


        if (depth == n) {
            end = std::chrono::system_clock::now();
            while (!q.empty()) { // at last layer prints all solutions and pops from queue
                if (n <= 6) {
                    printBoard(n, q.front());
                }
                goals++;
                q.pop_front();
            }
        }
    }

    chrono::duration<double> totalTime = end - start;
    cout << n << " Queens" << endl;
    cout << "Goals found: " << goals << endl;
    cout << "Time: " << totalTime.count() << " seconds" << endl;
    system("pause");
}

void bfs(const int n, list<vector<__int8>>& q) {

    chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();

    if (n == 2 || n == 3) { // no solutions exist for 2 and 3 
        end = std::chrono::system_clock::now();
        cout << "No solutions exist for " << n << " queens." << endl;
        system("pause");
        return;
    }
    if (n == 1) { // simply returns a queen
        end = std::chrono::system_clock::now();
        chrono::duration<double> totalTime = end - start;
        cout << "Q" << endl;
        cout << n << " Queens" << endl;
        cout << "Goals found: 1" << endl;
        cout << "Time: " << totalTime.count() << " seconds" << endl;
        system("pause");
        return;
    }

    vector<__int8> currentBoard = startState(n);
    vector<vector<__int8>> goalStates;
    int depth = 0;

    for (int j = 0; j < n; j++) { //starting from empty board 
        currentBoard[depth] = j; //add queen to board
        q.push_back(currentBoard); //push new board to queue

        currentBoard[depth] = -1; //resets board back to parent state
    }
    depth++; // counts tree depth
    int k = 0;
    int parents = n;
    int goals = 0;

    while (!q.empty()) { //while the queue is not empty
        int k = 0;

        while (k < parents) { // iterates for number of nodes in the layer
            currentBoard = q.front(); // grabs next in queue
            for (int j = 0; j < n; j++) {
                currentBoard[depth] = j; //generates next move
                q.push_back(currentBoard); // pushes to queue
                currentBoard[depth] = -1; // resets to parent state
            }
            q.pop_front(); // pops parent node once finished
            k++; // iterates to next node
        }
        parents *= n; // calculates number of nodes in next layer
        depth++; // calculates depth of tree/layer
        if (depth == n) { //if last layer is reached
            while (!q.empty()) {
                int valid = 0;
                vector<__int8> validPlacement;
                validPlacement.push_back(q.front().at(0));
                for (int i = 1; i < n; i++) {
                    if (validMove(i, validPlacement, q.front().at(i))) { // if goal state insert into goals vector
                        valid++;
                    }
                    else
                        break;
                    validPlacement.push_back(q.front().at(i));
                }
                if (valid == n - 1) {
                    goals++; // counts goals found
                    end = std::chrono::system_clock::now();
                    if (n <= 6) {
                        printBoard(n, q.front()); // prints goals
                    }
                }
                q.pop_front();
            }
        }
    }
    chrono::duration<double> totalTime = end - start;

    cout << endl << n << " Queens" << endl;
    cout << "Goals found: " << goals << endl;
    cout << "Time: " << totalTime.count() << " seconds" << endl;
    system("pause");
}

void hillClimbing(const int n, list<vector<__int8>>& q) {
    chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();

    if (n == 2 || n == 3) { // no solutions exist for 2 and 3 
        end = std::chrono::system_clock::now();
        cout << "No solutions exist for " << n << " queens." << endl;
        system("pause");
        return;
    }
    if (n == 1) { // simply returns a queen
        end = std::chrono::system_clock::now();
        chrono::duration<double> totalTime = end - start;
        cout << "Q" << endl;
        cout << n << " Queens" << endl;
        cout << "Goals found: 1" << endl;
        cout << "Time: " << totalTime.count() << " seconds" << endl;
        system("pause");
        return;
    }

    vector<__int8> board;
    vector<vector<__int8>> neighbours;
    board = generateBoard(n); // generates initial board
    if (n < 7) {
        cout << "Initial board: " << endl;
        printBoard(n, board);
    }
    int changes = 0;
    int newBoard = 0;
    neighbours = generateNeighbours(n, board); // generates neighbourhood
    while (true) {
        int fitness = fitnessScore(n, board); // finds fitness score of board
        vector<__int8> nextNeighbour;
        int min = INT_MAX;

        for (int i = 0; i < neighbours.size(); i++) { // iterates through neighbourhood
            if (fitnessScore(n, neighbours[i]) < min) { // checks if fitness of neighbour is less than current lowest
                nextNeighbour = neighbours[i];
                min = fitnessScore(n, nextNeighbour); // if it is, set neighbour to new lowest
            }
        }

        if (fitnessScore(n, nextNeighbour) != fitness) { // if the best neighbour is less than current node
            fitness = fitnessScore(n, nextNeighbour); // make the move
            board = nextNeighbour;
            if (fitness == 0) { // if global max is found print the board and end algorithm
                if (n < 7) {
                    cout << "Final board: " << endl;
                    printBoard(n, board);
                }
                break;
            }
            neighbours.clear(); // clears neighbourhood
            neighbours = generateNeighbours(n, board); // generate new neighbourhood  
        }
        else { // if local max is found, randomly generate new board and neighbourhood
            newBoard++;
            board = generateBoard(n);
            neighbours.clear();
            neighbours = generateNeighbours(n, board);
        }
    }

    end = std::chrono::system_clock::now();
    chrono::duration<double> totalTime = end - start;

    cout << endl << n << " Queens" << endl;
    cout << "Time: " << totalTime.count() << " seconds" << endl;
    system("pause");
}

void simulatedAnnealing(const int n, list<vector<__int8>>& q)
{
    chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();

    random_device device;
    mt19937 generator(device());
    uniform_real_distribution<double> distribution(0.0, 100.0);
    uniform_int_distribution<int> queen(0, n - 1);

    if (n == 2 || n == 3) { // no solutions exist for 2 and 3 
        end = std::chrono::system_clock::now();
        cout << "No solutions exist for " << n << " queens." << endl;
        return;
    }
    if (n == 1) { // simply returns a queen
        end = std::chrono::system_clock::now();
        chrono::duration<double> totalTime = end - start;
        cout << "Q" << endl;
        cout << n << " Queens" << endl;
        cout << "Goals found: 1" << endl;
        cout << "Time: " << totalTime.count() << " seconds" << endl;
        system("pause");
        return;
    }

    vector<__int8> board;
    vector<__int8> nextBoard;

    double p = 0.0;
    double a = 0.01; // alpha
    double e = exp(1);
    double k = 1000000000;
    double t = 100.0; // temperature
    int iter = 0;

    while (t > 1) {
        board = generateBoard(n); //initial configuration
        if (iter == 0) { // prints initial board
            if (n < 7) {
                cout << "Initial board:" << endl;
                printBoard(n, board);
            }
            iter++;
        }
        int prevFitness = fitnessScore(n, board); //evaluation function: returns number of attacking queens
        while (k > 0) { // iterates k times
            int newPos = queen(generator); // generates random queen position
            int currentPos = newPos;

            while (currentPos == newPos) { // finds a random queen on the board
                newPos = queen(generator);
            }

            nextBoard = board;
            nextBoard[queen(generator)] = newPos; // moves queen to new position

            int currentFitness = fitnessScore(n, nextBoard); // finds fitness of new board

            if (currentFitness == 0) { // if global max found print board and end algorithm
                end = std::chrono::system_clock::now();
                chrono::duration<double> totalTime = end - start;
                if (n < 7) {
                    cout << "Final board: " << endl;
                    printBoard(n, nextBoard);
                }
                cout << endl << n << " Queens" << endl;
                cout << "Time: " << totalTime.count() << " seconds" << endl;
                return;
            }

            double coinFlip = distribution(generator); // generates random number to compare to probability function

            p = pow(e, (-(currentFitness - prevFitness)) / t); // probability function

            if (currentFitness < prevFitness) { //compares fitness of new neighbour to current fitness score
                board = nextBoard;
                prevFitness = currentFitness; // if closer to goal set makes the move
            }
            else if (coinFlip < p) { // else make move with probability 
                board = nextBoard;
                prevFitness = currentFitness;
            }
            k--; // decreases k value
        }
        t *= 1 - a; // decreases temperature
    }
    cout << "No solution found." << endl; // if reached then no solution was found in time
    system("pause");
}

int main() {
    int n, search = 0;
    list<vector<__int8>> q;

    while (true) {
        cout << "\nChoose search:" << endl;
        cout << "\t1. Breadth First Search" << endl;
        cout << "\t2. Breadth First Search with Pruning" << endl;
        cout << "\t3. Hill Climbing Search" << endl;
        cout << "\t4. Simulated Annealing Search" << endl;
        cout << "\t5. Exit" << endl << endl;
        cin >> search;
        cout << endl;

        if (search == 5)
            break;
        if (search == 1) {
            cout << "Enter number of queens: " << endl;
            cin >> n;
            bfs(n, q);
        }
        else if (search == 2) {
            cout << "Enter number of queens: " << endl;
            cin >> n;
            bfsWithPruning(n, q);
        }
        else if (search == 3) {
            cout << "Enter number of queens: " << endl;
            cin >> n;
            hillClimbing(n, q);
        }
        else if (search == 4) {
            cout << "Enter number of queens: " << endl;
            cin >> n;
            simulatedAnnealing(n, q);
        }
        else {
            cerr << "Invalid input" << endl;
        }
    }
    system("pause");
    return 0;
}
