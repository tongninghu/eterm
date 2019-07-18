#include <iostream>
#include "flights.h"

using namespace std;

flights& flights::operator=(const flights& a) {
    flight = a.flight;
    dep = a.dep;
    des = a.des;
    d_day = a.d_day;
    d_month = a.d_month;
    airline = a.airline;
    return *this;
}

void flights::print() {
  cout << flight << " " << dep << " " << des \
   << " " << d_day << " " << d_month << " " \
   << airline << endl;
}
