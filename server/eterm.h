#ifndef ETERM_H
#define	ETERM_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <ctime>
#include <queue>
#include "flights.h"
#include "PNR.h"

#define MAX 1024
#define PORT 8080
#define SA struct sockaddr

using namespace std;

class MyHashFunction {
    public:
        size_t operator()(const flights& f) const {
            return (hash<string>()(f.flight)) ^ (hash<string>()(f.appointed_cabin))\
              ^ (hash<int>()(f.d_day)) ^ (hash<int>()(f.d_month));
        }
};

struct compare {
   bool operator()(const PNR& l, const PNR& r) {
       return l.getEndtime() > r.getEndtime();
   }
};

class eterm {
    private:
        unordered_set<string> cities {"PEK", "SHA", "CTU", "SZX"};
        unordered_map<string, int> months {{"JAN",1}, {"FEB", 2}, {"MAR", 3},
            {"APR", 4}, {"MAY", 5}, {"JUN", 6}, {"JUL", 7}, {"AUG", 8}, {"SEP", 9},
            {"OCT", 10}, {"NOV", 11}, {"DEC", 12}};
        unordered_set<string> airlines {"CA", "MU", "CZ"};
        priority_queue<PNR, vector<PNR>, compare> ticketingList;
        unordered_map<string, PNR> clients;
        unordered_set<flights, MyHashFunction> current_threads;
    public:
        eterm();
        void socketCreation();
        void clientHandler(const string& id, int sockfd);
        string requestHanlder(const string &id, string request);

        bool avTrigger(const string &id, string data, string &reply); // AV PEKSHA/25JUL
        bool sdTrigger(const string &id, string data, string &reply); // SD 1/F/1
        bool nmTrigger(const string &id, string data, string &reply); // NM HU/TONGNING
        bool tkTrigger(const string &id, string data, string &reply); // TK TL/1200/20JUL/P1

        string AV(const string &id, struct search_criteria &s);
        string SD(const string &id, flights &a, time_t ts);
        string NM(const string &id, string &name);
        string TK(const string &id, string &ticketing, time_t endtime);

        void PNR_maintain();  // delete the expired PNR from the queue
};

#endif
