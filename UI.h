#ifndef UI_H
#define	UI_H

#include <unordered_set>
#include <unordered_map>

#include "flights.h"
#include "eterm.h"

using namespace std;

class UI {
    private:
        vector<flights> reply;
        unordered_set<string> cities {"PEK", "SHA", "CTU", "SZX"};
        unordered_map<string, int> months {{"JAN",1}, {"FEB", 2}, {"MAR", 3},
            {"APR", 4}, {"MAY", 5}, {"JUN", 6}, {"JUL", 7}, {"AUG", 8}, {"SEP", 9},
            {"OCT", 10}, {"NOV", 11}, {"DEC", 12}};
        unordered_set<string> airlines {"CA", "MU", "CZ"};
        eterm * e;

    public:
        UI(eterm &e);
        void print(vector<string>& input);
        void print(vector<flights>& input);
        void getCommand();
        bool avTrigger(string arg);
};

#endif
