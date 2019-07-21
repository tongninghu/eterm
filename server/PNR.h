#ifndef PNR_H
#define	PNR_H

#include <ctime>
#include "flights.h"

using namespace std;


class PNR {
  private:
      string id;
      time_t ts;  // when is the ticket been locked by eterm
      time_t endtime;  // the deadline for ticketing
      string name;
      flights f;
      string phone;
      string ticketing;
      vector<flights> search;

  public:
      void insert_id(const string & id);
      void insert_search(const flights a);
      void insert_flight(const flights& a);
      void insert_ts(time_t timeStamp);
      void insert_name(const string & name);
      void insert_endtime(time_t endtime);
      void insert_ticketing(const string & ticketing);

      vector<flights>& getSearch();
      flights getFlight();
      time_t getEndtime() const;
      string getId() const;
      string getName();
      string getTicketing();

      string reply();


    //  friend bool operator<(const PNR& l, const PNR& r);
};

#endif
