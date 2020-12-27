/*
 * This program takes two words from the user and morphs one word into the other by altering one letter at a time.
 * The resulting word chain is then printed.
 */
#include <fstream>
#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <set>

const std::string ALPHABET  = "abcdefghijklmnopqrstuvwxyz";

/*
 * Set dictionary values. The dictionary is represented as a map containing each word along with their lengths.
 */
void setDictionary(std::set<std::string> &dict) {
    std::string filename = "dictionary.txt";
    std::ifstream file;
    file.open(filename);
    std::string word;

    while (file >> word) {
        dict.insert(word);
    }
    file.close();
}

/*
 * Find word neighbours.
 */
void wordNeighbours(const std::string word, const std::set<std::string> &dict, std::stack<std::string> &neighbours) {
    for (uint i = 0; i < word.size(); i++) {
        for (uint j = 0; j < ALPHABET.size(); j++) {
            std::string neighbour = word;
            neighbour[i] = ALPHABET[j];

            if (dict.count(neighbour) > 0) { // dict contains neighbour
                    neighbours.push(neighbour);
            }
        }
    }
}

/*
 * Print chain.
 */
void printWordChain(const std::string w1, const std::string w2, std::stack<std::string> chain) {
    std::cout << "Chain from " << w2 << " back to " << w1 << ": " << std::endl;
    while (!chain.empty()) {
        std::cout << chain.top() << " ";
        chain.pop();
    }
    std::cout << std::endl << "Have a nice day!" << std::endl;
}

/*
 * Calculate and print the word chain, given two words and the dictionary.
 */
void wordChain(const std::string w1, const std::string w2, const std::set<std::string> &dict) {

    std::queue<std::stack<std::string>> stackQueue;
    std::set<std::string> usedWords;

    std::stack<std::string> wordStack;
    wordStack.push(w1);
    stackQueue.push(wordStack);

    while (!stackQueue.empty()) {
        std::stack<std::string> partialChain = stackQueue.front();
        stackQueue.pop();

        if (partialChain.top() == w2) {
            printWordChain(w1, w2, partialChain);
            return;
        }
        else {
            std::stack<std::string> neighbours;
            wordNeighbours(partialChain.top(), dict, neighbours);
            while (!neighbours.empty()) {
                std::string neighbour = neighbours.top();
                neighbours.pop();

                if (usedWords.count(neighbour) == 0) { // usedWords doesnt contain neighbour
                    //std::cout << neighbour << " <-- current neighbour \n";
                    std::stack<std::string> partialChainCopy{partialChain};
                    partialChainCopy.push(neighbour);
                    stackQueue.push(partialChainCopy);
                    usedWords.insert(neighbour);
                }
            }
        }
    }
}

/*
 * Main function.
 */
int main() {
    std::cout << "Welcome to TDDD86 Word Chain." << std::endl;
    std::cout << "If you give me two English words, I will transform the" << std::endl;
    std::cout << "first into the second by changing one letter at a time." << std::endl;
    std::cout << std::endl;

    std::set<std::string> dict;
    setDictionary(dict);

    std::cout << "Please type two words: ";
    std::string w1, w2;
    std::cin >> w1 >> w2;

    wordChain(w1, w2, dict);

    return 0;
}
