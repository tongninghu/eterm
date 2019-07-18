#ifndef FLIGHTS_H
#define	FLIGHTS_H

#include <vector>
#include <string>

using namespace std;

struct search_criteria {
    string dep;
    string des;
    int day;
    int month;
    int year;
    int hour = -1;
    int min = -1;
    string airline;
};

class flights {
public:
    string flight;
    string dep;
    string des;
    int d_day = -1;
    int d_month = -1;
    string airline;

    flights& operator=(const flights& a);
    void print();
};

#endif
