#ifndef PNR_H
#define	PNR_H

#include <ctime>
#include "flights.h"

using namespace std;


class PNR {
  private:
    time_t ts;
    string name;
    flights f;
    string phone;

  public:

    void insert_flight(const flights& a);
    void insert_ts(time_t timeStamp);
    void insert_name(const string & name);
    vector<string> reply();
};

#endif
