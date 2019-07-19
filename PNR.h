#ifndef PNR_H
#define	PNR_H

#include <ctime>
#include "flights.h"

using namespace std;


class PNR {
  private:
      time_t ts;  // when is the ticket been locked by eterm
      time_t endtime;  // the deadline for ticketing
      string name;
      flights f;
      string phone;
      string ticketing;
      string mac_a;

  public:
      void insert_flight(const flights& a);
      void insert_ts(time_t timeStamp);
      void insert_name(const string & name);
      void insert_mac(const string & mac_a);
      void insert_endtime(time_t endtime);
      void insert_ticketing(const string & ticketing);

      vector<string> reply();
      time_t getEndtime() const;
      string getMacAddress() const;
    //  friend bool operator<(const PNR& l, const PNR& r);
};

#endif
