all:
	g++ -std=c++17 -Wall -Wextra -Werror main.cpp -o lab2

run:
	./lab2

clean:
	rm -f lab2
