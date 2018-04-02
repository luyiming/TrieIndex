#ifndef TRIE_INDEX_H
#define TRIE_INDEX_H

#include <vector>
#include <string>
#include <unordered_set>
#include <fstream>

using namespace std;

const string VALID_CHARS = "abcdefghijklmnopqrstuvwxyz0123456789";
const size_t MAX_NODES = 36;

class TrieNode {
public:
    bool isWord = false;
    TrieNode* next[MAX_NODES];
    string term;

    struct Posting {
        Posting(int docID) :docID(docID), withinDocFreq(1) {}
        int docID;         // the document ID
        int withinDocFreq;  // the within-document frequency, a count of how often t occurs in a document
    };
    int docFreq = 0; // the document frequency
    vector<Posting> postings;

    TrieNode() :isWord(false), docFreq(0) {
        for (int i = 0; i < MAX_NODES; i++) {
            next[i] = NULL;
        }
    }

    ~TrieNode() {
        for (int i = 0; i < MAX_NODES; i++) {
            if (next[i]) {
                delete next[i];
            }
        }
    }
};

class TrieIndex {
public:
    void add(int docID, vector<string> terms);
    const TrieNode * find(string term);
    void setStopList(unordered_set<string> &stopList);
    void saveToFile(string lexiconFile = "lexicon", string invlistFile = "invlists");

private:
    void pushFileName(TrieNode* n, int docID);
    TrieNode* addWord(string s);

    TrieNode root;
    unordered_set<string> stopList;
};


#endif //TRIE_INDEX_H