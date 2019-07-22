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
    distance = a.distance;
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

string flights::toString() const{
    string reply;
    reply += flight + " " + dep + " " + des + " " + to_string(d_day) + " "\
          + to_string(d_month) + " " + airline + " F" + F + " C" + C + " Y" + Y\
          +" B" + B + " K" + K + " M" + M;
    return reply;
}

string flights::reply() {
    string reply;
    reply += flight + " " + dep + " " + des + " " + to_string(d_day) + " "\
          + to_string(d_month) + " " + appointed_cabin + " " + to_string(appointed_num);
    return reply;
}
