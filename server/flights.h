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
    int distance;
    int d_day = -1;
    int d_month = -1;
    string airline;
    string F = "not available"; // indicate the condition of available tickets in this cabin
    string C = "not available";
    string Y = "not available";
    string B = "not available";
    string K = "not available";
    string M = "not available";

    string appointed_cabin;
    int appointed_num;

    flights();
    flights& operator=(const flights& a);
    string toString() const;
    string reply();
    friend bool operator==(const flights& l, const flights& r);
};

#endif
