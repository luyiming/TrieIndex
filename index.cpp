#include <iostream>
#include "TrieIndex.h"

using namespace std;

struct CommandArgs {
    bool printTerm = false;
    char *docFile = NULL;
    char *stopFile = NULL;
} cmdArgs;


int main(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        if (argv[i] == "-p") {
            cmdArgs.printTerm = true;
        }
        else if (argv[i] == "-s") {
            cmdArgs.stopFile = argv[i + 1];
            i++;
        }
        else {
            cmdArgs.docFile = argv[i];
        }
    }

    TrieIndex index;
    index.add(0, { "a", "b" });
    const TrieNode *p = index.find("a");
    cout << "docFreq: " << p->docFreq << endl;
    return 0;
}