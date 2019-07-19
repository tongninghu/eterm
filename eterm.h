#ifndef ETERM_H
#define	ETERM_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <ctime>
#include "flights.h"
#include "PNR.h"

using namespace std;

class MyHashFunction {
public:
    size_t operator()(const flights& f) const {
        return (hash<string>()(f.flight)) ^ (hash<string>()(f.appointed_cabin))\
          ^ (hash<int>()(f.d_day)) ^ (hash<int>()(f.d_month));
    }
};

class eterm {
    private:
        unordered_map<string, PNR> clients;
        unordered_set<flights, MyHashFunction> current_threads;
    public:
        eterm();
        vector<flights>  AV(struct search_criteria &s);
        vector<string> SD(flights a, string mac_a, time_t ts);
        vector<string> NM(string name, string mac_a);
};

#endif
