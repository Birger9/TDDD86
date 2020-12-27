/**
* A 2-dimensional grid is used to simulate the lifecycle of bacteria and a simple GUI menu with three choices is used
* to navigate the Game of Life.
*/

#include <iostream>
#include <fstream>
#include "grid.h"
#include "lifeutil.h"
#include <string>

/**
 * Sets the values the grid will contain, as bacteria, X, and empty grid places -.
 *
 * @param Grid variable where the current state of the grid will be stored, the file containing information, Integer of number of rows and columns.
 * @return Nothing.
 */
void setGrid(Grid<std::string> &grid, std::ifstream &file, const int &nRows, const int &nColumns) {
    for (int i = 0; i < nRows; i++) {
        if (!file.eof()) { // Checks so the file is not empty
            std::string line;
            std::getline(file, line);

            for (int j = 0; j < nColumns; j++) {
                grid.set(i, j, std::string(1, line[j]));
            }
        }
    }
}

/**
 * Prints the current state of the grid
 *
 * @param Grid variable where the information about the grid is stored.
 * @return Nothing.
 */
void printGridState(const Grid<std::string> &grid) {
    for(int i = 0; i < grid.numRows(); i++) {
        for (int j = 0; j < grid.numCols(); j++) {
            std::cout << grid.get(i, j);
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}

/**
 * Returns the number of nearby neighbors to a specific x and y-value.
 *
 * @param The grid, the row and column that will be checked.
 * @return An integer with the number of nearby neighbors.
 */
int neighborCount(const Grid<std::string> &grid, const int row, const int column) {
    int count = 0;
    for (int i = -1; i <= 1; i++) { // -1, 0, 1
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue; // Ignore self

            if (grid.inBounds(row + i, column + j)) {
                if (grid.get(row + i, column + j) == "X") {
                    count++;
                }
            }
        }
    }
    return count;
}

/**
 * Advances the grid simulation and updates it
 *
 * @param Grid variable where the current state of the grid is stored.
 * @return Nothing.
 */
void advanceSimulation(Grid<std::string> &grid) {
    Grid<std::string> newGrid;
    newGrid.deepCopy(grid);

    for (int i = 0; i < grid.numRows(); i++) {
        for (int j = 0; j < grid.numCols(); j++) {
            int nNeighbors = neighborCount(grid, i, j);
            if (nNeighbors <= 1 || nNeighbors >= 4) {
                if (newGrid.get(i, j) == "X") {
                    newGrid.set(i, j, "-"); // Kill if neighbor count is 0, 1, or greater than 3
                }
            }
            else if (nNeighbors == 3) {
                newGrid.set(i, j, "X"); // Create a new bacteria
            }
        }
    }
    grid = newGrid;
}

/**
 * For 1 tick the grid simulation is advanced and then the new state is printed
 *
 * @param Grid variable where where the current state of the grid is stored.
 * @return Nothing.
 */
void tick(Grid<std::string> &grid) {
    advanceSimulation(grid);
    printGridState(grid);
}

/**
 * A GUI menu with three options
 *
 * @param Grid variable where where the current state of the grid is stored.
 * @return Returns false if the user wants to exit the game otherwise true.
 */
bool iterateMenu(Grid<std::string> &grid) {
    std::cout << "a)nimate, t)ick, q)uit? ";
    std::string input;
    std::cin >> input;
    std::cout << std::endl;

    if (input == "a") {
        while (true) {
            tick(grid);
            pause(100);
        }
    }
    else if (input == "t") {
        tick(grid);
    }
    else if (input == "q") {
        std::cout << "Have a nice life!" << std::endl;
        return false;
    }
    return true;
}

/**
 * The main function of the game
 *
 * @param Nothing.
 * @return Nothing.
 */
int main() {

    Grid<std::string> grid;
    int nRows = 0;
    int nColumns = 0;

    std::cout << "Welcome to the TDDD86 Game of Life, a simulation of the lifecycle of a bacteria colony.\n"
              << "Cells (X) live and die by the following rules:\n"
              << "- A cell with 1 or fewer neighbours dies.\n"
              << "- Locations with 2 neighbours remain stable.\n"
              << "- Locations with 3 neighbours will create life.\n"
              << "- A cell with 4 or more neighbours dies. \n";

    // Get file name from user
    std::cout << "Grid input file name? ";
    std::string filename;
    std::cin >> filename;

    std::ifstream file;
    file.open(filename);

    // Get row count
    std::string outputText;
    std::getline(file, outputText);
    nRows = std::stoi(outputText);

    // Get column count
    std::getline(file, outputText);
    nColumns = std::stoi(outputText);

    // Resize grid
    grid.resize(nRows, nColumns);

    // Set grid values
    setGrid(grid, file, nRows, nColumns);
    file.close();

    // Print initial state
    printGridState(grid);

    bool run = true;
    while (run) {
        run = iterateMenu(grid);
    }

    return 0;
}


