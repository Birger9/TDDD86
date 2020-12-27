// This is the .h file you will edit and turn in.
// We have provided a minimal skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own, as well as on the members.
// TODO: remove this comment header and replace it with your own

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "grid.h"
#include "Cube.h"
#include "lexicon.h"
#include <set>
// TODO: include any other header files you need

using namespace std;

class Boggle {
public:
    const string DICTIONARY_FILE = "EnglishWords.dat";
    const int MIN_WORD_LENGTH = 4;
    const uint16_t BOARD_SIZE = 4;
    const std::string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const Lexicon *lexicon = new Lexicon(DICTIONARY_FILE); // Lexicon that stores all valid words
    std::set<std::string> guessedWords;
    std::set<std::string> acceptedWords; // The players accepted words is stored here
    std::set<std::string> computerAcceptedWords; // The computers accepted words is stored here

    // TODO: decide the public member functions and declare them
    ~Boggle();
    void setGrid();
    bool setGrid(std::string values);
    void shuffleCubes();
    std::string getBoardStr();
    bool wordIsValid(std::string word, bool human);
    void addGuessedWord(std::string word);
    void addValidWord(std::string word);
    bool verifyWord(std::string word, Grid<Cube*> board);
    void verifyWordHelper(std::string currentWord, Grid<Cube*> board, Cube cube, std::string finalWord);
    void resetBoard(Grid<Cube*> board);
    void showPlayerScore();
    void findAllWords(Grid<Cube*> board);
    void findAllWordsHelper(std::string currentWord, Grid<Cube*> board, Cube cube);
    void addComputerValidWord(std::string word);
    void showComputerScore();
    bool checkIfTaken(std::string word);
    void resetAllWordLists();
    vector<Cube*> neighbors(Grid<Cube*> board, Cube cube);
    Grid<Cube*> getBoard() const;

private:
    // TODO: decide the private member variables/functions and declare them
    int calcScore(std::set<std::string> words);
    Grid<Cube*> *boardGrid = new Grid<Cube*>(BOARD_SIZE,BOARD_SIZE);
    bool foundWord;

};

#endif
