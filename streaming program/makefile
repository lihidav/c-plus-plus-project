all: splflix

splflix: bin/Main.o bin/Session.o bin/Action.o bin/Watchable.o bin/User.o 
	@echo 'Linking'
	g++ -o bin/splflix bin/Main.o bin/Session.o bin/Watchable.o bin/Action.o bin/User.o
	
bin/Main.o: src/Main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Main.o src/Main.cpp
	
bin/Session.o: src/Session.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Session.o src/Session.cpp

bin/Watchable.o: src/Watchable.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Watchable.o src/Watchable.cpp

bin/Action.o: src/Action.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp

bin/User.o: src/User.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/User.o src/User.cpp

clean:
	@echo 'Cleaning'
	rm -f bin/*

