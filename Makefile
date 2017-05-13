
all:
	$(CXX) -std=c++11 -Wall -Wextra StudentSort.cpp -o StudentSort

test:
	 ./StudentSort test_input.txt
	 cat sorted_output.txt
	 
	 ./StudentSort test_input_1M_records.txt
	 cat sorted_output.txt | more
