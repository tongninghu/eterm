#include <iostream>
#include <string>
#include "flights.h"

using namespace std;

flights::flights() {
    appointed_cabin = "";
}

flights& flights::operator=(const flights& a) {
    flight = a.flight;
    dep = a.dep;
    des = a.des;
    d_day = a.d_day;
    d_month = a.d_month;
    airline = a.airline;
    F = a.F;
    C = a.C;
    Y = a.Y;
    B = a.B;
    K = a.K;
    M = a.M;
    appointed_cabin = a.appointed_cabin;
    appointed_num = a.appointed_num;
    return *this;
}

bool operator==(const flights& l, const flights& r) {
    if (l.flight != r.flight || l.d_day != r.d_day || l.d_month != r.d_month ||\
        l.appointed_cabin != r.appointed_cabin ) return false;
    return true;
}

void flights::print() {
    cout << flight << " " << dep << " " << des << " " << d_day << " " << d_month\
    << " " << airline << " F" << F << " C" << C << " Y" << Y << " B" << B <<\
    " K" << K << " M" << M << endl;
}

string flights::reply() {
    string reply;
    reply += flight;
    reply += " ";
    reply += dep;
    reply += " ";
    reply += des;
    reply += " ";
    reply += to_string(d_day);
    reply += " ";
    reply += to_string(d_month);
    reply += " ";
    reply += appointed_cabin;
    reply += " ";
    reply += to_string(appointed_num);

    return reply;
}
