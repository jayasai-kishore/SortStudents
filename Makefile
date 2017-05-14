
all:
	$(CXX) -std=c++11 -Wall -Wextra StudentSort.cpp -o StudentSort

test:
	##################### Original test case ########################
	./StudentSort test_input.txt;
	cat sorted_output.txt;
	##################### Not passing input file ##########################################
	"./StudentSort" || true;
	##################### Passing non existing file #######################################
	"./StudentSort non_existing.txt" || true;
	#######################################################################################
