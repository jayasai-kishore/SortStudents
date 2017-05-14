
all:
	$(CXX) -std=c++11 -Wall -Wextra StudentSort.cpp -o StudentSort

test:
	echo "#################################################################################";
	echo "Original test case";
	./StudentSort test_input.txt;
	cat sorted_output.txt;
	echo "#################################################################################";
