
all:
	$(CXX) -std=c++11 -Wall -Wextra StudentSort.cpp -o StudentSort

test:
	##################### Original test case ########################
	./StudentSort test_input.txt;
	cat sorted_output.txt;
	##################### Not passing input file ##########################################
	./StudentSort || true;
	##################### Passing non existing file #######################################
	./StudentSort non_existing.txt || true;
	##################### Passing empty input file ########################################
	./StudentSort test_input_0_records.txt || true;
	##################### Passing input file with 1 record ################################
	./StudentSort test_input_1_record.txt || true;
	##################### Passing input file with invalid content #########################
	./StudentSort test_input_invalid_content.txt || true;
	##################### Passing input file with negative marks ##########################
	./StudentSort test_input_negative_marks.txt || true;
	#######################################################################################
