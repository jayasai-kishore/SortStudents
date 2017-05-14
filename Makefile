
all:
	$(CXX) -std=c++11 -Wall -Wextra StudentSort.cpp -o StudentSort

test:
	echo "#################################################################################"
	echo "Original test case"
	 ./StudentSort test_input.txt
	 cat sorted_output.txt
	 echo "#################################################################################"
	 
	 echo "Test by not specifying input file name in command line"
	 ./StudentSort
 	 echo "#################################################################################"
	 
	 echo "Test by specifying non existing input file"
	 ./StudentSort non_existing.txt
 	 echo "#################################################################################"
