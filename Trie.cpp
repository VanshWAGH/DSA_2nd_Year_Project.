#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <fstream>

using namespace std;

class TrieNode {
public:
    char data;
    TrieNode* children[26];
    bool isTerminal;

    TrieNode(char ch) {
        data = ch;
        for (int i = 0; i < 26; i++) {
            children[i] = nullptr;
        }
        isTerminal = false;
    }
};

class Trie {
public:
    TrieNode* root;

    Trie() {
        root = new TrieNode('\0');
    }

    void insertUtil(TrieNode* root, const std::string& word) {
        if (word.length() == 0) {
            root->isTerminal = true;
            return;
        }

        int index = word[0] - 'a';
        TrieNode* child;

        if (root->children[index] != nullptr) {
            child = root->children[index];
        } else {
            child = new TrieNode(word[0]);
            root->children[index] = child;
        }

        insertUtil(child, word.substr(1));
    }

    void insertWord(const std::string& word) {
        insertUtil(root, word);
    }

    bool searchUtil(TrieNode* root, const std::string& word) {
        if (word.length() == 0) {
            return root->isTerminal;
        }

        int index = word[0] - 'a';
        TrieNode* child;

        if (root->children[index] != nullptr) {
            child = root->children[index];
        } else {
            return false;
        }

        return searchUtil(child, word.substr(1));
    }

    bool searchWord(const std::string& word) {
        return searchUtil(root, word);
    }

    void removeUtil(TrieNode* root, const std::string& word) {
        if (word.length() == 0) {
            root->isTerminal = false;
            return;
        }

        int index = word[0] - 'a';
        TrieNode* child;

        if (root->children[index] != nullptr) {
            child = root->children[index];
        } else {
            return;
        }

        removeUtil(child, word.substr(1));

        if (!child->isTerminal) {
            for (int i = 0; i < 26; i++) {
                if (child->children[i] != nullptr) {
                    return;
                }
            }
            delete child;
            root->children[index] = nullptr;
        }
    }

    void removeWord(const std::string& word) {
        removeUtil(root, word);
    }

    void printSuggestions(TrieNode *curr, vector<string>&temp, string prefix){
        if(curr->isTerminal){
            temp.push_back(prefix);
            //cout<<prefix<<endl;
        }

        for(char ch = 'a'; ch <= 'z'; ch++){
            TrieNode* next = curr->children[ch - 'a'];

            if(next != NULL){
                prefix.push_back(ch);
                printSuggestions(next, temp, prefix);
                prefix.pop_back();
            }
        }
    }

    vector<vector<string>> getSuggestions(string str) {
        TrieNode* prev = root;
        vector<vector<string>> output;
        string prefix = "";

        for (int i = 0; i < str.length(); i++) {
            char lastch = str[i];
            prefix.push_back(lastch);

            // check for lastch
            TrieNode* curr = prev->children[lastch - 'a'];

            // if not found
            if (curr == NULL) {
                break;
            }

            vector<string> temp;
            printSuggestions(curr, temp, prefix);
            /*for(int i = 0; i < temp.size(); i++){
                cout<<temp[i]<<" ";
            }*/

            output.push_back(temp);
            prev = curr;  // Update the value of prev
        }
        return output;
    }
};

std::string removeSpaces(const std::string& word) {
    std::string result;
    for (char character : word) {
        if (character != ' ') {
            result += character;
        }
    }
    return result;
}

vector<vector<string>> Directory(vector<string>&contactList, string &queryStr)
{
    Trie* t = new Trie();

    //querystr -> which we have to search in the list.
    //i shall take names form the file and make it to a vector of contact list and then pass it down to this fucntion

    //insert all contacts in the trie

    for(int i = 0; i < contactList.size(); i++){
        string str = contactList[i];
        t->insertWord(str);
    }

    //return ans
    return t->getSuggestions(queryStr);

}

int main() {
    //Trie* t = new Trie();
    //int n; // number of inputs
    //std::cout << "Number of Inputs: ";
    //std::cin >> n;
    //std::cin.ignore(); // Consume the newline character left by previous input

    /*for (int i = 0; i < n; i++) {
        std::string input;
        std::cout << "Enter input " << i + 1 << ": ";

        // Use getline to read a line that may contain spaces
        std::getline(std::cin, input);

        input = removeSpaces(input);

        // Convert the word to lowercase using std::transform
        std::transform(input.begin(), input.end(), input.begin(), ::tolower);

        // Insert the word into the Trie after removing spaces
        t->insertWord(input);
    }*/

    //std::cout << "Present or not: " << t->searchWord("vanshwagh") << std::endl;

    // Don't forget to delete the Trie object to avoid memory leaks

    Trie* trie = new Trie();

    // Read lines from file and build Trie
    ifstream file("doc.odt");

    if (!file.is_open()) {
        cout << "Error while opening the file" << endl;
        return 1;  // Exit the program if file opening fails
    }

    string line;
    while (getline(file, line)) {
        // Insert each line into the Trie
        trie->insertWord(removeSpaces(line));
    }

    file.close();  // Close the file after reading

    // Now you can use trie->getSuggestions() or other Trie methods as needed

    // Clean up memory
    


   vector<vector<string>> suggestions = trie->getSuggestions("vansh wagh");

    // Print the suggestions
    for (const auto& suggestion : suggestions) {
        for (const auto& word : suggestion) {
            cout << word << " ";
        }
        break;
        cout << endl;
    }
    cout<<endl;
    delete trie;



    return 0;
}
