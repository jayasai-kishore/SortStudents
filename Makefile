
all:
	$(CXX) -std=c++11 -Wall -Wextra StudentSort.cpp -o StudentSort

test:
	 ./StudentSort test_input.txt
	 cat sorted_output.txt
	 
	 git config --global user.email "jay.software.pro@gmail.com"
	 git config --global user.name "jayasai-kishore"
	 
	 git add sorted_output.txt
	 git commit -m "Output file sent from CI server"
	 git push https://github.com/jayasai-kishore/SortStudents.git master
