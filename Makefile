test: main.o UI.o eterm.o flights.o
	g++ -std=gnu++11 -g -w -L/usr/local/mysql-connector-c++/lib64/ -lmysqlcppconn main.o UI.o flights.o eterm.o -o test

UI.o: UI.h UI.cpp eterm.h
	g++ -std=gnu++11 -g -w -c UI.cpp

flights.o: flights.h flights.cpp
	g++ -std=gnu++11 -g -w -c flights.cpp

eterm.o: eterm.h eterm.cpp
	g++ -std=gnu++11 -g -w -c -I/usr/local/mysql-connector-c++/include/jdbc eterm.cpp

main.o: main.cpp UI.h eterm.h
	g++ -std=gnu++11 -g -w -c main.cpp

run:
	./test

clean:
	rm -f test main.o UI.o eterm.o flights.o
