// Boggle contains the basic logic for the game, such as
// finding words on the board.

#include <sstream>
#include "Boggle.h"
#include "random.h"
#include "shuffle.h"
#include "strlib.h"

static const int NUM_CUBES = 16;   // the number of cubes in the game
static const int CUBE_SIDES = 6;   // the number of sides on each cube
static string CUBES[NUM_CUBES] = {        // the letters on all 6 sides of every cube
   "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
   "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
   "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
   "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

// TODO: implement the members you declared in Boggle.h
Boggle::~Boggle() {
    for(int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            delete boardGrid->get(j, i);
        }
    }
    delete boardGrid;
    delete lexicon;
}

/*
 * Set board values and shuffle grid.
 */
void Boggle::setGrid() {
    Grid<char> temp(BOARD_SIZE, BOARD_SIZE);
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            int cubeIndex = y*BOARD_SIZE + x;
            int randCubeIndex = rand() % CUBE_SIDES;
            char cubeChar = CUBES[cubeIndex][randCubeIndex];
            temp.set(x, y, cubeChar);
        }
    }
    shuffle(temp);

    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            Cube *cube = new Cube(temp.get(x, y), x, y);
            boardGrid->set(x, y, cube);
        }
    }
}

/*
 * Manually set board values. The input string represents the board symbols. Return true if string is accepted.
 */
bool Boggle::setGrid(std::string values) {
    if (values.length() != BOARD_SIZE*BOARD_SIZE) return false; // Check length

    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            int cubeIndex = y*BOARD_SIZE + x;
            if (ALPHABET.find(toupper(values[cubeIndex])) == std::string::npos) return false; // Check A-Z
            boardGrid->set(x, y, new Cube(toupper(values[cubeIndex]), x, y));
        }
    }
    return true;
}


/*
 * Return the string you get by flattening the board.
 */
std::string Boggle::getBoardStr() {
    std::string boardStr = "";
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            boardStr.append(std::string(1, boardGrid->get(x, y)->getLetter()));
            //std::cout << boardGrid->get(x, y)->getLetter() << "    ";
        }
        //std::cout << std::endl;
    }
    return boardStr;
}

/*
 * Checks to see of an inputted word from user is valid.
 */
bool Boggle::wordIsValid(std::string word, bool human) {
    if(!lexicon->contains(word)) {
        if(human) std::cout << "The word doesnt exist in the lexicon" << std::endl;
        return false;
    }

    else if(guessedWords.count(word) != 0) {
        if(human) std::cout << "The word has already been guessed by the player" << std::endl;
        return false;
    }

    else if(word.length() < MIN_WORD_LENGTH) {
        if(human) std::cout << "The word must be 4 characters or longer" << std::endl;
        return false;
    }

    else{return true;}
}

/*
 * Adds a word to the players guessed set.
 */
void Boggle::addGuessedWord(std::string word) {guessedWords.insert(word);}

/*
 * Adds a word to the players valid set.
 */
void Boggle::addValidWord(std::string word) {acceptedWords.insert(word);}

bool Boggle::checkIfTaken(std::string word) {return acceptedWords.find(word) != acceptedWords.end();}

/*
 * Adds a word to the computer valid set.
 */
void Boggle::addComputerValidWord(std::string word){computerAcceptedWords.insert(word);}

/*
 * Find all words (computer).
 */
void Boggle::findAllWords(Grid<Cube*> board) {
    std::string word = "";
    for(int y = 0; y < BOARD_SIZE; y++) {
        for(int x = 0; x < BOARD_SIZE; x++) {
            Cube* cube = board.get(x, y);

            //cube->setVisited(true);
            findAllWordsHelper(word, board, *cube); // Explore all words that could start with the cubes letter
            cube->setVisited(false);
        }
    }
}

/*
 * Find all words helper (recursively search for words).
 */
void Boggle::findAllWordsHelper(std::string currentWord, Grid<Cube*> board, Cube cube) {
    if (!lexicon->containsPrefix(currentWord)) {
        return;
    }

    if(lexicon->contains(currentWord) && wordIsValid(currentWord, false) && !checkIfTaken(currentWord)) { // We found a match
        addComputerValidWord(currentWord);
    }

    for (auto &neighbor : neighbors(board, cube)) {
        neighbor->setVisited(true);
        findAllWordsHelper(currentWord + std::string(1, neighbor->getLetter()), board, *neighbor);
        neighbor->setVisited(false);
    }
}

/*
 * Verify if a word can be found on the board.
 */
bool Boggle::verifyWord(std::string finalWord, Grid<Cube*> board) {
    std::string currentWord;
    currentWord.append(std::string(1, finalWord[0]));

    foundWord = false;

    for(int y = 0; y < BOARD_SIZE; y++) {
        for(int x = 0; x < BOARD_SIZE; x++) {
            Cube* cube = board.get(x, y);
            if (cube->getLetter() == finalWord[0]) {

                cube->setVisited(true);

                verifyWordHelper(currentWord, board, *cube, finalWord);
                cube->setVisited(false);
                if(foundWord) return true;
            }
        }
    }
    return foundWord; // Inputted word is found on the board
}

/*
 * Verify word helper.
 */
void Boggle::verifyWordHelper(std::string currentWord, Grid<Cube*> board, Cube cube, std::string finalWord) {
    if(currentWord == finalWord) { // We found a match
        foundWord = true;
        return;
    }
    if (currentWord.length() >= finalWord.length()) {
        return;
    }

    for (auto &neighbor : neighbors(board, cube)) {
        if (finalWord.find(currentWord + neighbor->getLetter()) == std::string::npos) continue;
        neighbor->setVisited(true);
        verifyWordHelper(currentWord + std::string(1, neighbor->getLetter()), board, *neighbor, finalWord);
        neighbor->setVisited(false);
    }
    return;
}

/*
 * Returns a vector containing the nearby neighbors of a cube.
 */
vector<Cube*> Boggle::neighbors(Grid<Cube*> board, Cube cube) {
    vector<Cube*> neighbors;

    for(int y = cube.getY() - 1; y <= cube.getY() + 1; y++) {
        for(int x = cube.getX() - 1; x <= cube.getX() + 1; x++) {
            if(y == cube.getY() && x == cube.getX()) continue;
            if(y < 0 || y >= BOARD_SIZE || x < 0 || x >= BOARD_SIZE) continue;
            if (board.get(x, y)->getVisited()) continue;
            neighbors.push_back(board.get(x, y));
        }
    }
    return neighbors;
}

/*
 * Reset the board.
 */
void Boggle::resetBoard(Grid<Cube*> board){
    for(int y = 0; y < BOARD_SIZE; y++) {
        for(int x = 0; x < BOARD_SIZE; x++) {
            Cube* cube = board.get(x, y);         
            cube->setVisited(false);
        }
    }
}

/*
 * Calculate and return the score.
 */
int Boggle::calcScore(std::set<string> words) {
    int totalScore = 0;
    for (auto &elem : words) {
        totalScore += (elem.size() - 3);
    }
    return totalScore;
}

/*
 * Show player score.
 */
void Boggle::showPlayerScore() {
    std::cout << "Your words (" << acceptedWords.size() << "): {";
    for (auto elem : acceptedWords)
    {
        std::cout << " '" << elem << "' ";
    }
    std::cout << "}" << std::endl;

    std::cout << "Your score: " << calcScore(acceptedWords) << std::endl;
}

/*
 * Show computer score.
 */
void Boggle::showComputerScore() {
    std::cout << "My words (" << computerAcceptedWords.size() << "): {";
    for (auto elem : computerAcceptedWords)
    {
        std::cout << " '" << elem << "' ";
    }
    std::cout << "}" << std::endl;

    std::cout << "My score: " << calcScore(computerAcceptedWords) << std::endl;
}

/*
 * Reset all the word lists.
 */
void Boggle::resetAllWordLists() {
   guessedWords.clear();
   acceptedWords.clear();
   computerAcceptedWords.clear();
}

Grid<Cube*> Boggle::getBoard() const {return *boardGrid;}


