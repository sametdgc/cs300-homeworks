// Huseyin Samed Dagci 30996


#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <string>
#include <sstream>
#include "Stack.cpp"

using namespace std;



struct Cell {               // cell struct to store coordinates and wall info
    int row;
    int col;
    bool visited;
    bool topWall;
    bool bottomWall;
    bool leftWall;
    bool rightWall;

    // Default constructor
    Cell() : row(), col(), visited(false), topWall(true), bottomWall(true), leftWall(true), rightWall(true) {}

    // Constructor with parameters
    Cell(int r, int c) : row(r), col(c), visited(false), topWall(true), bottomWall(true), leftWall(true), rightWall(true) {}
};


// check if the coordinates are within the maze boundaries
bool isValidCoordinate(int x, int y, int M, int N) {
    
    return x >= 0 && x < N && y >= 0 && y < M;
}

// function to generate mazes
vector<vector<Cell>> MazeGenerator(int M, int N) {
    vector<vector<Cell>> maze(M, vector<Cell>(N));      // create a 2D vector to represent the maze

    Stack<Cell*> cells;         // create a stack to store cell pointers
    Cell* starting_cell = &maze[M - 1][0];          // start from bottom left
    starting_cell->visited = true;      // mark the starting cell as visited
    starting_cell->col = 0;             // starting cell coordinates 
    starting_cell->row = 0;
    cells.push(starting_cell);      // push the starting cell onto the stack

    // define directions: up, down, left, right
    int directionX[4] = { 0, 0, -1, 1 };        
    int directionY[4] = { 1, -1, 0, 0 };

    // main loop for maze generation
    while (!cells.isEmpty()) {
        Cell* currentCell = cells.top(); // get the current cell from the top of the stack in each iteration
        int x = currentCell->col; // get the x-coordinate of the current cell
        int y = currentCell->row; // get the y-coordinate of the current cell

        // check if there are unvisited neighboring cells
        vector<int> validDirections; // store valid direction options
        for (int i = 0; i < 4; ++i) {
            int nx = x + directionX[i]; // calculate the x-coordinate of the neighboring cell
            int ny = y + directionY[i]; // calculate the y-coordinate of the neighboring cell

            // Check if the neighboring cell is valid and unvisited
            if (isValidCoordinate(nx, ny, M, N) && !maze[M - 1 - ny][nx].visited) {
                validDirections.push_back(i); // Store the index of valid directions
            }
        }

        if (!validDirections.empty()) {
            // If there are valid directions, choose a random one from the valid options
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> dis(0, validDirections.size() - 1);
            int randomIndex = dis(gen); // generate a random index
            int dir = validDirections[randomIndex]; // get the randomly selected direction
            int neighborX = x + directionX[dir]; // calculate the x-coordinate of the new cell
            int neighborY = y + directionY[dir]; // calculate the y-coordinate of the new cell

            // knock down the wall between the current cell and the chosen cell
            if (dir == 0) {
                currentCell->topWall = false;
                maze[M - 1 - neighborY][neighborX].bottomWall = false;
            }
            else if (dir == 1) {
                currentCell->bottomWall = false;
                maze[M - 1 - neighborY][neighborX].topWall = false;
            }
            else if (dir == 2) {
                currentCell->leftWall = false;
                maze[M - 1 - neighborY][neighborX].rightWall = false;

            }
            else if (dir == 3) {
                currentCell->rightWall = false;
                maze[M - 1 - neighborY][neighborX].leftWall = false;
            }

            Cell* newCell = &maze[M - 1 - neighborY][neighborX]; // get the pointer to the new cell
            newCell->col = neighborX;               // define new cell coordinates
            newCell->row = neighborY;
            newCell->visited = true; // mark the new cell as visited
            cells.push(newCell); // push the new cell onto the stack

        }
        else {
            // if there are no unvisited neighboring cells, backtrack
            cells.pop(); // poping the current cell from the stack
        }
    }

    return maze; // Return the generated maze
}


bool IntToBool(const int& value) {

    return value != 0;
}

// function to read maze files
vector<vector<Cell>> readMazeFromFile(const string &filename, int M, int N) {

    ifstream file(filename);                            // read the specified file
    vector<vector<Cell>> maze(M, vector<Cell>(N));      // create a 2d matrix to store maze info

    if (file.is_open()) {
        
        int M, N;
        string line;

        getline(file, line);
        stringstream ss(line);
        ss >> M >> N;

        while (getline(file, line)) {
            // parse the cell values
            int x = line.at(2) - '0';
            int y = line.at(6) - '0';
            bool l = IntToBool(line.at(10) - '0');
            bool r = IntToBool(line.at(14) - '0');
            bool u = IntToBool(line.at(18) - '0');
            bool d = IntToBool(line.at(22) - '0');
            // assign cells
            maze[M - 1 -y][x].col = x;
            maze[M - 1 - y][x].row = y;
            maze[M - 1 - y][x].leftWall = l;
            maze[M - 1 - y][x].rightWall = r;
            maze[M - 1 - y][x].topWall = u;
            maze[M - 1 - y][x].bottomWall = d;
            maze[M - 1 - y][x].visited = false;
        }
        file.close();
    }   
    else {
        cerr << "file not found" << endl;
    }


    return maze;


}

// find a path in the maze given entry and exit points
Stack<Cell*> pathFinder(vector<vector<Cell>>& maze, int start_x, int start_y, int finish_x, int finish_y, int maze_id) {
    
    // directions up, down, left, right
    int directionX[4] = { 0, 0, -1, 1 };
    int directionY[4] = { 1, -1, 0, 0 };

    int M = maze.size();
    int N = maze[0].size();

    Stack<Cell*> solution;                              // create a stack to hold path information   
    maze[M - 1 - start_y][start_x].visited = true;      //  mark the starting cell as visited

    solution.push(&maze[M - 1 - start_y][start_x]);         


    int current_x = start_x;
    int current_y = start_y;

    while (!solution.isEmpty()) {
        
        current_x = solution.top()->col;
        current_y = solution.top()->row;

        if ((current_x == finish_x) && (current_y == finish_y)) {           // if the coordinates on top of the stack matches ending coordinates return
            return solution;
        }

        vector<int> valid_neighbors;                            // check for valid neighbors
        for (int i = 0; i < 4; ++i) {
            int nx = current_x + directionX[i];
            int ny = current_y + directionY[i];
            if (isValidCoordinate(nx, ny, M, N) && !maze[M - 1 - ny][nx].visited) {
                // Check if there is no wall between the current cell and the neighboring cell in the chosen direction
                switch (i) {
                case 0: // up
                    if (!solution.top()->topWall) {
                        valid_neighbors.push_back(i);
                    }
                    break;
                case 1: // down
                    if (!solution.top()->bottomWall) {
                        valid_neighbors.push_back(i);
                    }
                    break;
                case 2: // left
                    if (!solution.top()->leftWall) {
                        valid_neighbors.push_back(i);
                    }
                    break;
                case 3: // right
                    if (!solution.top()->rightWall) {
                        valid_neighbors.push_back(i);
                    }
                    break;
                }
            }
        }
        if (!valid_neighbors.empty()) { 
            // if there are valid neighbors go to one of them randomly
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> dis(0, valid_neighbors.size() - 1);

            int randomIndex = dis(gen);
            int dir = valid_neighbors[randomIndex];
            int neighborX = current_x + directionX[dir];
            int neighborY = current_y + directionY[dir];

            maze[M - 1 - neighborY][neighborX].visited = true;          // mark the cell as visited
            solution.push(&maze[M - 1 - neighborY][neighborX]);         // push it to the stack
        }
        else {
            // backtrack only if there are no valid neighbors
            if (!solution.isEmpty()) {
                solution.pop();
            }
        }
    }

}



int main() {

    int M;  // number of rows
    int N;  // number of columns
    int K;  // number of mazes


    //Get the input from the user
    cout << "Enter the number of mazes: ";
    cin >> K;

    cout << "Enter the number of rows and columns (M and N): ";
    cin >> M >> N;

    vector <vector<vector<Cell>>> all_mazes;                                    //store all generated mazes


    for (int i = 0; i < K; i++) {

        vector<vector<Cell>> current_maze = MazeGenerator(M, N);                // generate each maze
        all_mazes.push_back(current_maze);                                      
        ofstream file("maze_" + to_string(i + 1) + ".txt");                     //write them into txt files

        file << M << " " << N << endl;
       
        for (int row = 0; row < M; row++) {
            for (int col = 0; col < N; col++) {
                const Cell& cell = current_maze[M - 1 - row][col];
                file << "x=" << cell.col << " y=" << cell.row
                    << " l=" << (cell.leftWall ? 1 : 0)
                    << " r=" << (cell.rightWall ? 1 : 0)
                    << " u=" << (cell.topWall ? 1 : 0)
                    << " d=" << (cell.bottomWall ? 1 : 0) << endl;
            }
        }
        file.close();
    }

    
    
    int maze_id, start_x, start_y, finish_x, finish_y;                          //    pathfinding inputs


 

    cout << "All maze are generated." << endl << endl;

    cout << "Enter a maze ID between 1 to " + to_string(K) + " inclusive to find a path : ";
    cin >> maze_id;

    cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): ";
    cin >> start_x >> start_y;

    cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): ";
    cin >> finish_x >> finish_y;


    //vector<vector<Cell>> u = readMazeFromFile("maze_" + to_string(maze_id) + ".txt", M, N);
    vector<vector<Cell>> m = all_mazes[maze_id - 1];                                // get the desired maze from maze_id

    
    for (int i = 0; i < m.size(); ++i) {                                            // mark all the cell unvisited
        for (int j = 0; j < m[0].size(); ++j) {
            m[i][j].visited = false;
        }
    }
    
    Stack<Cell*> solution = pathFinder(m, start_x, start_y, finish_x, finish_y, maze_id);
    
    // to print from beginning to end
    Stack<Cell*> reversed;  

    while (!solution.isEmpty()) {
        reversed.push(solution.top());
        solution.pop();
    }

    // write the found path into a txt file, name them according to the maze_id and entry ending points

    ofstream outFile("maze_" + to_string(maze_id) + "_path_" + to_string(start_x) + "_" + to_string(start_y) + "_" + to_string(finish_x) + "_" + to_string(finish_y) + ".txt");
    while (!reversed.isEmpty()) {
        Cell* pathCell = reversed.top();
        outFile << pathCell->col << " " << pathCell->row << endl;
        reversed.pop();
    }
    outFile.close();
            
    return 0;
}
