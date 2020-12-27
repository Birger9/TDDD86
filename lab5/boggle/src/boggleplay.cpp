// Boggle play contains higher level functionality such
// as handling the turns, printing the board, etc.

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "Boggle.h"
#include "bogglemain.h"
#include "strlib.h"
// TODO: include any other header files you need

/*
 * Plays one game of Boggle using the given boggle game state object.
 */
void playOneGame(Boggle& boggle) {
    // TODO: implement this function (and add any other functions you like to help you)
    bool manual = !yesOrNo("Do you want to generate a random board? (y/n) ");

    if (manual) {
        enterGridManually(boggle);
    } else {
        setBoard(boggle);
    }
    printBoard(boggle);

    startHumanTurn(boggle);
    startComputerTurn(boggle);
}

/*
 * The user inputs the board manually.
 */
void enterGridManually(Boggle& boggle){
    bool valid = false;
    while (!valid) {
        std::string val;
        std::cout << "Enter the values: ";
        getline(cin, val);
        valid = boggle.setGrid(val);
    }
}

/*
 * Prints current board to the screen.
 */
void printBoard(Boggle& boggle) {
    std::string boardStr = boggle.getBoardStr();
    std::cout << std::endl;
    for (uint16_t i = 0; i < boardStr.length(); i++) {
        std::cout << boardStr[i] << "   ";
        if ((i+1) % boggle.BOARD_SIZE == 0) {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}

/*
 * Sets the board.
 */
void setBoard(Boggle& boggle) {boggle.setGrid();}

/*
 *  Start human turn.
 */
void startHumanTurn(Boggle& boggle) {
    bool humanTurn = true;

    while(humanTurn) {
        std::string word;
        std::cout << "Type a word (or press Enter to end your turn): ";
        getline(cin, word);

        if(!word.empty()) {
            bool valid = boggle.wordIsValid(word, true);
            if(valid){
                boggle.addGuessedWord(word);
                word = toUpperCase(word);
                bool validWord = boggle.verifyWord(word, boggle.getBoard());
                if(!validWord) std::cout << "That word is not on the board" << std::endl;
                if(validWord) {
                    std::cout << "You found a new word! " << word << std::endl;
                    boggle.addValidWord(word); // Add valid word
                    boggle.resetBoard(boggle.getBoard()); // Resets all the cubes, in the board, to false
                    boggle.showPlayerScore(); // Print all accepted words
                }
            }
        } else {// End human turn
            humanTurn=false;
            std::cout << "You ended your turn " << word << std::endl;
        }
    }
}

/*
 *  Start computer turn.
 */
void startComputerTurn(Boggle& boggle) {
    boggle.findAllWords(boggle.getBoard());
    boggle.showComputerScore();
    boggle.resetBoard(boggle.getBoard()); // Reset the board grid
    boggle.resetAllWordLists(); // Resets all word-lists for player and computer
}

/*
 * Erases all currently visible text from the output console.
 */
void clearConsole() {
#if defined(_WIN32) || defined(_WIN64)
    std::system("CLS");
#else
    // assume POSIX
    std::system("clear");
#endif
}
