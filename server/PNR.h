#ifndef PNR_H
#define	PNR_H

#include <ctime>
#include "flights.h"

using namespace std;


class PNR {
  private:
      string name;
      flights f;
      string phone;
      string ticketing;
      string fp = "FP/CASH ";
      string fc = "FC/";
      string fn = "FN/";

      string id;
      time_t ts;  // when is the ticket been locked by eterm
      time_t endtime;  // the deadline for ticketing
      vector<flights> search;
      vector<vector<string>> searchPrice; // 0.CNY 1.FARE 2.TAX 3.YQ 4.TOTAL
  public:
      void insert_id(const string & id);
      void insert_search(const flights a);
      void insert_flight(const flights& a);
      void insert_ts(time_t timeStamp);
      void insert_name(const string & name);
      void insert_endtime(time_t endtime);
      void insert_ticketing(const string & ticketing);
      void insert_searchPrice(vector<string> price);
      void insert_price(int num);

      vector<flights>& getSearch();
      flights getFlight();
      time_t getEndtime() const;
      string getId() const;
      string getName();
      string getTicketing();
      vector<vector<string>>& getSearchPrice();

      string reply();

    //  friend bool operator<(const PNR& l, const PNR& r);
};

#endif
