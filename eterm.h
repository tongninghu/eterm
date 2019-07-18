#ifndef ETERM_H
#define	ETERM_H

#include <string>
#include <vector>
#include "flights.h"

using namespace std;

class eterm {
    private:
        int count = 0;
    public:
      eterm();
      vector<flights>  AV(struct search_criteria &s);

};

#endif
