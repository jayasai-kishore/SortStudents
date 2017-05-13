
all:
	$(CXX) -std=c++11 -Wall -Wextra StudentSort.cpp -o StudentSort

test:
	 ./StudentSort test_input.txt
	 cat sorted_output.txt

