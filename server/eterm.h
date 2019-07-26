#ifndef ETERM_H
#define	ETERM_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <ctime>
#include <queue>
#include "flights.h"
#include "PNR.h"

#define MAX 1024
#define PORT 8080
#define SA struct sockaddr
#define TRUE   1
#define FALSE  0

using namespace std;

class MyHashFunction {
    public:
        size_t operator()(const flights& f) const {
            return (hash<string>()(f.flight)) ^ (hash<string>()(f.appointed_cabin))\
              ^ (hash<int>()(f.d_day)) ^ (hash<int>()(f.d_month));
        }
};

struct PtrComp {
    bool operator()(const PNR* l, const PNR* r) const {
        return l->getEndtime() < r->getEndtime();
    }
};

class eterm {
    private:
        unordered_set<string> cities {"PEK", "SHA", "CTU", "SZX"};
        unordered_map<string, double> discount {{"F", 1.5}, {"C", 1.3},
            {"Y", 1.0}, {"B", 0.9}, {"K", 0.8}, {"M", 0.7}};
        unordered_map<string, int> months {{"JAN",1}, {"FEB", 2}, {"MAR", 3},
            {"APR", 4}, {"MAY", 5}, {"JUN", 6}, {"JUL", 7}, {"AUG", 8},
            {"SEP", 9}, {"OCT", 10}, {"NOV", 11}, {"DEC", 12}};
        unordered_set<string> airlines {"CA", "MU", "CZ"};
        vector<string> currency {"CNY", "USD"};

        set<PNR*, PtrComp> ticketingList;
        unordered_map<string, PNR*> clients;
        unordered_map<string, PNR*> numberToPNR;
        unordered_set<flights, MyHashFunction> current_threads;
        unordered_map<flights, set<time_t>, MyHashFunction> lockTickets;

    public:
        eterm();
        void clientHandler();
        string requestHanlder(const string &id, string request);

        bool avTrigger(const string &id, vector<string> & temp, string &reply); // AV PEKSHA/31JUL
        bool sdTrigger(const string &id, vector<string> & temp, string &reply); // SD 1/F/1
        bool nmTrigger(const string &id, vector<string> & temp, string &data, string &reply); // NM HU/TONGNING
        bool tkTrigger(const string &id, vector<string> & temp, string &data, string &reply); // TK TL/1830/25JUL/P1
        bool patTrigger(const string &id, vector<string> & temp, string &reply); // PAT A
        bool sfcTrigger(const string &id, vector<string> & temp, string &reply); // SFC 1
        bool etdzTrigger(const string& id, vector<string> & temp, string& reply); //ETDZ 4

        string AV(const string &id, struct search_criteria &s);
        string SD(const string &id, flights &a);
        string NM(const string &id, string &name);
        string TK(const string &id, string &ticketing, time_t endtime);
        string PAT(const string &id);
        string SFC(const string &id, int num);
        void sealPNR(const string& id, string& reply);  // @
        void RT(const string& id, vector<string> & temp, string& reply);  // RT VP52M8
        string ETDZ(const string & id, flights &a, time_t ts);

        vector<string> priceCalculator(int distance, string cabin, string currency);
        void PNR_maintain();  // delete the expired PNR from the queue
        string random_string(size_t length);
};

#endif
