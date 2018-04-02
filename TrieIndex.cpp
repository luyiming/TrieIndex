#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <sstream>
#include <fstream>
#include "TrieIndex.h"

using namespace std;

void TrieIndex::add(int docID, vector<string> terms) {
    //transform(s.begin(), s.end(), s.begin(), tolower); // TODO: remove this
    for (int i = 0; i < terms.size(); i++) {
        if (this->stopList.find(terms[i]) == this->stopList.end()) {
            pushFileName(addWord(terms[i]), docID);
        }
    }
}

const TrieNode * TrieIndex::find(string term) {
    size_t idx;
    transform(term.begin(), term.end(), term.begin(), tolower);
    TrieNode* p = &root;
    for (string::iterator i = term.begin(); i != term.end(); i++) {
        idx = VALID_CHARS.find(*i);
        if (idx < MAX_NODES) {
            if (!p->next[idx]) {
                return NULL;
            }
            p = p->next[idx];
        }
    }
    if (p->isWord) {
        return p;
    }
    return NULL;
}

void TrieIndex::setStopList(unordered_set<string>& stopList)
{
    this->stopList = stopList;
}

void TrieIndex::saveToFile(string lexiconFile, string invlistFile)
{
    ofstream lexicon;
    ofstream invlists;
    lexicon.open(lexiconFile);
    invlists.open(invlistFile);

    queue<TrieNode*> queue;
    queue.push(&root);
    int offset = 0;
    while (!queue.empty()) {
        TrieNode *p = queue.front();
        queue.pop();
        if (p->isWord) {
            stringstream ss;
            ss << p->docFreq;
            for (int i = 0; i < p->postings.size(); i++) {
                ss << " " << p->postings[i].docID << " " << p->postings[i].withinDocFreq;
            }
            ss << endl;
            lexicon << p->term << " " << offset << "\n";
            offset += ss.str().size();
            invlists << ss.str();
        }

        for (int i = 0; i < MAX_NODES; i++) {
            if (p->next[i] != NULL) {
                queue.push(p->next[i]);
            }
        }
    }

    lexicon.close();
    invlists.close();
}

void TrieIndex::pushFileName(TrieNode* n, int docID) {
    int i;
    for (i = 0; i < n->postings.size(); i++) {
        if (n->postings[i].docID == docID) {
            n->postings[i].withinDocFreq++;
            break;
        }
    }
    if (n->postings.size() == 0 || i == n->postings.size()) {
        n->postings.push_back(TrieNode::Posting(docID));
        n->docFreq = n->postings.size();
    }
}

TrieNode* TrieIndex::addWord(string s) {
    size_t idx;
    TrieNode* p = &root, *q;
    for (string::iterator i = s.begin(); i != s.end(); i++) {
        idx = VALID_CHARS.find(*i);
        if (idx < MAX_NODES) {
            q = p->next[idx];
            if (q == NULL) {
                p->next[idx] = new TrieNode();
                p = p->next[idx];
            }
            else {
                p = q;
            }
        }
        else {
            cout << "invalid word: " << s << endl;
            exit(1);
        }
    }
    p->isWord = true;
    p->term = s;
    return p;
}

