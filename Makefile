
all:
	$(CXX) -std=c++11 -Wall -Wextra StudentSort.cpp -o StudentSort

test:
	##################### Original test case ########################
	./StudentSort test_input.txt;
	#cat test_input-graded.txt;
	tail -f -n +0 test_input-graded.txt &;
	##################### Not passing input file ##########################################
	./StudentSort || true;
	##################### Passing non existing file #######################################
	./StudentSort non_existing.txt || true;
	##################### Passing empty input file ########################################
	./StudentSort test_input_0_records.txt || true;
	##################### Passing input file with 1 record ################################
	./StudentSort test_input_1_record.txt || true;
	#cat test_input_1_record-graded.txt;
	tail -f -n +0 test_input_1_record-graded.txt &;
	##################### Passing input file with invalid content #########################
	./StudentSort test_input_invalid_content.txt || true;
	##################### Passing input file with negative marks ##########################
	./StudentSort test_input_negative_marks.txt || true;
	#cat test_input_negative_marks-graded.txt;
	tail -f -n +0 test_input_negative_marks-graded.txt &;
	##################### Passing input with long names #################################
	./StudentSort test_input_long_names.txt || true;
	#cat test_input_long_names-graded.txt;
	tail -f -n +0 test_input_long_names-graded.txt &;
	##################### Passing input with no comma #################################
	./StudentSort test_input_nocomma.txt || true;
	##################### Passing input with duplicate record #################################
	./StudentSort test_input_duplicate_record.txt || true;
	#cat test_input_duplicate_record-graded.txt;
	tail -f -n +0 test_input_duplicate_record-graded.txt &;
	##################### Passing input with same score and last name #################################
	./StudentSort test_input_same_score_lastname.txt || true;
	#cat test_input_same_score_lastname-graded.txt;
	tail -f -n +0 test_input_same_score_lastname-graded.txt &;
	##################### Passing input file with invalid extension ###################################
	./StudentSort test_input.txtt || true
	##################### Passing input with 1000 records #################################
	./StudentSort test_input_1k_records.txt || true;
	#cat test_input_1k_records-graded.txt;
	tail -f -n +0 test_input_1k_records-graded.txt &;
	#######################################################################################
