server: server.o eterm.o flights.o PNR.o
	clang++ -std=c++11 -g -w -L/usr/local/mysql-connector-c++/lib64/ -lmysqlcppconn server.o flights.o PNR.o eterm.o -o server

flights.o: flights.h flights.cpp
	clang++ -std=c++11 -g -w -c flights.cpp

PNR.o: PNR.h PNR.cpp flights.h flights.cpp
	clang++ -std=c++11 -g -w -c PNR.cpp

eterm.o: eterm.h eterm.cpp PNR.h PNR.cpp
	clang++ -std=c++11 -g -w -c -I/usr/local/mysql-connector-c++/include/jdbc eterm.cpp

server.o: server.cpp eterm.h eterm.cpp
	clang++ -std=c++11 -g -w -c server.cpp

run:
	./server

clean:
	rm -f server server.o eterm.o flights.o PNR.o
