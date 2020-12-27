/*
 * Evil hangman. A game of hangman where the game "cheats" by changing the correct word bases on the user's input.
 */
#include <fstream>
#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include<vector>

using std::cout;
using std::cin;
using std::endl;
using std::string;

const string ALPHABET  = "abcdefghijklmnopqrstuvwxyz";

/*
 * Set dictionary. Each element is a pair consisting of a word and its length.
 */
void setDictionary(std::map<string, int> &dict) {
    string filename = "dictionary.txt"; // di.txt
    std::ifstream file;
    file.open(filename);
    string word;

    while (file >> word) {
        dict[word] = word.size();
    }
    file.close();
}

/*
 * Return true if the dictionary contains a word of length wordLen.
 */
bool wordLenAccepted(const uint16_t &wordLen, const std::map<string, int> &dict) {
    for (auto const& x : dict) {
        int len = x.second;

        if (len == wordLen) {
            return true;
        }
    }
    return false;
}

/*
 * Set the word.
 */
void concealWord(const uint16_t &len, string &word) {
    word = "";
    for(uint i = 0; i < len; i++) {
        word += "-";
    }
}

/*
 * Return a set of words where the length of each word matches wordLen.
 */
void findWordsOfLen(const uint16_t &wordLen, const std::map<string, int> &dict, std::set<string> &words) {
    for (auto const& x : dict) {
        string word = x.first;
        int len = x.second;

        if (len == wordLen) words.insert(word);
    }
}


/*
 *  Return whether guess is valid.
 */
bool guessIsValid(const string &str, const std::vector<char> &guesses) {
    if (str.size() != 1) return false;

    for (const char &guess : guesses) {
        if (str[0] == guess) {
            return false; // Letter has already been guessed.
        }
    }
    return true;
}

/*
 *  Return a set of words that match the current guess pattern. E.g. if the current guess is e--e, an example of a remaining word would be "else"
 */
void findRemainingWords(const string &currentGuess, const std::set<string> &validWords, std::set<string> &remaining) {
    for (auto const& wrd : validWords) {
        for (uint i = 0; i < currentGuess.length(); i++) {
            if (currentGuess[i] == '-') {
                continue;
            }
            else if (currentGuess[i] != wrd[i]) {
                break;
            }
        }
        remaining.insert(wrd);
    }
}

/*
 * A list of indices of where a letter can be found in a word. For example, letterPositions("else", 'e') would return {0, 3}.
 */
void findLetterPositions(const string &word, const char &guessedLetter, std::vector<int> &letterPos) {
    for (uint i = 0; i < word.length(); i++) {
        if (word[i] == guessedLetter) {
            letterPos.push_back(i);    // ----       // else, {0, 3}
        }
    }
}

/*
 * The families of a word, where each family contains words that match a unique pattern. (e.g. "e--", "--e", "---" etc.)
 */
void findWordFamilies(const std::set<string> &remainingWords, const char &guessedLetter, std::vector<std::set<string>> &families) {
    for (auto const& word : remainingWords) {
        // Check if there is an existing family.
        bool hasFamily = false;
        for (auto &family : families) {
            // There is a family for these letter positions
            const string familyWord = *family.begin();
            std::vector<int> wordLtrPositions;
            std::vector<int> familyLtrPositions;
            findLetterPositions(word, guessedLetter, wordLtrPositions);
            findLetterPositions(familyWord, guessedLetter, familyLtrPositions);
            if (wordLtrPositions == familyLtrPositions) {
                family.insert(word);
                hasFamily = true;
            }
        }
        // New family.
        if (!hasFamily) {
            std::set<std::string> family;
            family.insert(word);
            families.push_back(family);
        }
    }
}

/*
 * Main function.
 */
int main() {
    cout << "Welcome to Hangman." << endl;

    std::map<string, int> dict;
    setDictionary(dict);

    // Get word len from user
    uint16_t wordLen;
    bool accepted = false;
    while(!accepted) {
        cout << "Please type the length of the word: ";
        cin >> wordLen;
        accepted = wordLenAccepted(wordLen, dict);
    }
    std::vector<char> guessedLetters;
    guessedLetters.reserve(ALPHABET.length());

    string word;
    concealWord(wordLen, word); // The word that shall be guessed is saved here.
    string currentGuess = word;
    cout << "Word: " << word << endl;

    // Number of guesses?
    uint16_t currentNumGuesses = 0;
    uint16_t maxNumGuesses;
    cout << "Please type the number of guesses you want to have: ";
    cin >> maxNumGuesses;

    // See words?
    string choice;
    bool seeWords;
    cout << "Do you want to see remaining words in the wordlist?: ";
    cin >> choice;
    seeWords = (choice == "yes");


    // Valid words.
    std::set<string> validWords;
    findWordsOfLen(wordLen, dict, validWords);
    while (true) {
        cout << "--- NEW ROUND ---" << endl;
        cout << "=================" << endl << endl;

        word = *validWords.begin();
        if (seeWords) cout << "The word is " << word << ". " << endl; // Print the current correct word.
        cout << "The current guess is " << currentGuess << ". " << endl;
        cout << "You have " << maxNumGuesses - currentNumGuesses << " guess(es) left." << endl;

        bool acceptedGuess = false;
        string guessedLetter;
        while(!acceptedGuess) {
            cout << "Guess a letter in the alphabet: ";
            cin >> guessedLetter;
            std::size_t found = ALPHABET.find(std::tolower(guessedLetter[0]));

            if(guessedLetter.length() == 1 && found != string::npos) {
                acceptedGuess = guessIsValid(guessedLetter, guessedLetters);
            }
        }
        guessedLetters.push_back(guessedLetter[0]);
        std::set<string> remaining;
        findRemainingWords(currentGuess, validWords, remaining);
        std::vector<std::set<string>> families;
        findWordFamilies(remaining, guessedLetter[0], families);

        // Print families and set biggest family.
        validWords = families.at(0);
        for (auto const &family : families) {
            if (family.size() > validWords.size()) {
                validWords = family; // Biggest family.
            }
            if (seeWords) {
                cout << "Family size: " << family.size() << endl;
            }
        }

        // Is guess correct?
        bool guessCorrect = false;
        const string wrd = *validWords.begin(); // Get sample word from biggest family.
        std::vector<int> letterPos;
        findLetterPositions(wrd, guessedLetter[0], letterPos);
        if (letterPos.size() > 0) { // If the new family still contains the letter, then the guess is correct.
            for (const int &a : letterPos) {
                currentGuess[a] = guessedLetter[0];
                guessCorrect = true;
            }
        }
        if (!guessCorrect) {
            currentNumGuesses++;
        }
        if (currentGuess == wrd) {
            cout << "You win!" << endl;
            break;
        }
        else {
            if (currentNumGuesses >= maxNumGuesses) {
                cout << "You lose!" << endl;
                break;
            }
        }
    }
    return 0;
}



