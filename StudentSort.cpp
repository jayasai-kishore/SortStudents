#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <random>
#include <sstream>
#include <queue>
#include <cstdio>
using namespace std;

//========================================================================================================
//                 Problem statement
//========================================================================================================
//Records need to be in the descending order of scores
//If scores are same, records need to be sorted in the descending alphabetical order of last names
//If last names are also same, records need to be sorted in the descending alphabetical order of first names
//If first names are also same, then the records need to be identified as exact duplicates, hence they will occur
//in sequence.

//=========================================================================================================
//	Algorithm is based on External sort to handle any amount of data
//=========================================================================================================
//1.Read the records from input file up to a limit set for intermediate file creation or until the end of
//	input file has been reached.
//2. Sort the records(based on above criteria) and write to an intermediate file
//3. Execute step 1 until end of reading input file
//4. Create as many file streams as the number of intermediate files
//	 (Follow K-way merge sort algorithm to pick desired record from the files)
//5. Read one record from each file stream
//6. Use a priority queue to keep the record to be picked (based on sort criteria)
//   (Priority queue automatically keeps the required record in the top by following max heap algorithm)
//7. Get the record from the top of the priority queue and identify to which file stream it belongs to.
//8. Add this record to the output buffer
//9. Check whether the size of the output buffer reached a certain limit and 
//   if so, write the contents of the buffer to the final output file.
//10. Read one record from a file stream which was identified in step 7
//11. Go to step 7 and repeat until all the records are read and sent to the final output file


//Student class holds information of score, last name, first name
class Student
{
public:
	
	int score;
	string firstName;
	string lastName;
	
public:
	size_t index = 0; //This is for storing the index of intermediate file from where next record to be fetched

	Student()
	:score(0)
	{
	}
	Student(int score, string fName, string lName, size_t index)
	{
		this->score = score;
		this->firstName = fName;
		this->lastName = lName;
		this->index = index;
	}
	friend istream& operator>>(istream& in, Student &s);
	friend ostream& operator<<(ostream &out, const Student &s);
};

//This outputs the given record to the given output stream
//and returns the output stream
ostream& operator<<(ostream &out, const Student &s)
{
	out<<s.firstName<<" "<<s.lastName<<" "<<s.score<<endl;
	return out;
}

//This extracts a record from the given input stream
//and returns the input stream
istream& operator>>(istream& in, Student &s)
{
	in>>s.firstName>>s.lastName>>s.score;
	return in;
}

//This is only for generating test data with random names and scores
//Not used in the core logic of sorting data in the input file
class RandomNameGenerator
{
public:
	static string getRandomName()
	{
		string str;
		for(int i = 0; i < 10; i++)
			str += ('A' + (rand() % 26));
		str += ",";
		return str;
	}
};

//This is only for generating test input data file
//Not used in the core logic of sorting the data
void generateTestInputFile(long long numRecords)
{
	ofstream out("test_input_10M_records.txt");
	Student s;
	for(long long i = 0; i < numRecords; i++)
	{
		s.firstName = RandomNameGenerator::getRandomName();
		s.lastName = RandomNameGenerator::getRandomName();
		s.score = rand() % 101; //Score can be any where between 0 and 100 inclusive
		out<<s;
	}
	out.close();
}

//This helps in finding the order of records during sort
struct Comparator
{
	bool operator()(const Student &s1, const Student &s2)
	{
		if(s1.score == s2.score) //If scores are same, compare last names
		{
			if(s1.lastName == s2.lastName) //If last names are also same, compare first names
			{
				if(s1.firstName.compare(s2.firstName) > 0)
					return true;
				else
					return false;
			}
			else
			{
				if(s1.lastName.compare(s2.lastName) > 0) //If last names are different, determine the order
				{
					return true;
				}
				else
					return false;
			}
		}
		else
		{
			if(s1.score > s2.score) //If scores are different, determine the order
				return true;
			else
				return false;
		}
	}
};

//Priority Queue comparator is different from above comparator
//Below comparator helps in the functionality of finding the highest rated record and 
//keeping it at the top of max heap
//This helps in improved performance during K-way merge sort when one highest rated record has to be
//identified from a set of records
struct ComparatorForMaxPriorityQueue
{
	bool operator()(const Student &s1, const Student &s2)
	{
		if(s1.score == s2.score) //If scores are same, compare last names
		{
			if(s1.lastName == s2.lastName) //If last names are also same, compare first names
			{
				if(s1.firstName.compare(s2.firstName) < 0)
					return true;
				else
					return false;
			}
			else
			{
				//If last names are different, determine the order as per max heap requirement
				if(s1.lastName.compare(s2.lastName) < 0)
				{
					return true;
				}
				else
					return false;
			}
		}
		else
		{
			if(s1.score < s2.score) //If scores are different, determine the order as per max heap requirement
				return true;
			else
				return false;
		}
	}
};

//This writes the data in the given list to an intermediate file whose suffix
//will be the given fileNum.
//Out param intermediateFileList will have the name of newly created intermediate file
//Returns true if creation of intermediate file is successful
//Returns false otherwise
bool writeListToFile(const list<Student> &lst, const size_t fileNum, list<string> &intermediateFileList)
{
	stringstream s;
	s<<fileNum;
	string str("Intermediate_" + s.str() + ".txt");
	//cout<<str<<endl;
	ofstream out(str);
	if(!out.is_open())
	{
		cout<<"File open failed: "<<str<<endl;
		return false;
	}
		
	for(auto it = lst.begin(); it != lst.end(); it++)
	{
		out<<it->firstName<<" "<<it->lastName<<" "<<it->score<<endl;
	}
	out.close();
	intermediateFileList.push_back(str);
	return true;
}

//This writes the data present in the given list to the given output file
void writeOutput(ofstream &outFinal, const list<Student> &lst)
{
	for(auto it = lst.begin(); it != lst.end(); it++)
	{
		outFinal<<*it;
	}
	outFinal.flush();
}

int main(int argc, char *argv[])
{
	//Below code is commented because it is required only when generating test data
	//const long long NUM_TEST_RECORDS = 9999990;
	//generateTestInputFile(NUM_TEST_RECORDS);
	
	//As per the program requirement, input file must be passed as command line argument
	//If not passed, it is considered as an error
	if(argc <= 1)
	{
		cout<<"Pass input file name"<<endl;
		return -1;
	}
	
	string inputFilePath(argv[1]);
	//string inputFilePath("test_input_1k_records.txt");
	size_t pos = inputFilePath.find(".txt");
	string inputFilePathNoExtn;
	
	//Check whether given input file ends with valid text file extension ".txt"
	if((pos != string::npos) && (inputFilePath.length() == (pos + 4))) //4 is the length of .txt
	{
		inputFilePathNoExtn = inputFilePath.substr(0, pos);
	}
	else
	{
		cout<<"Input file invalid extension "<<endl;
		return -1;
	}
	ifstream in(inputFilePath.c_str());
	if(!in.is_open())
	{
		cout<<"File open failed: "<<endl;
		return -1;
	}

	//Create output file name string as given in the program requirement
	//Append "-graded.txt" to the input file name without extension
	string outFilePath(inputFilePathNoExtn + "-graded.txt");
	ofstream outFinal(outFilePath);
	if(!outFinal.is_open())
	{
		cout<<"Output file open failed: "<<endl;
		in.close();
		return -1;
	}
	cout<<"Processing input file: please wait..."<<endl;
	list<Student> lst;
	Student ob;
	int count = 1;
	
	//Since the input file can be of huge size such as 2GB, it is required to produce the intermediate
	//sort results in intermediate files which can be merged later.
	//Here, we are defining a limit after which intermediate file will be generated with sorted records
	//As per several executions, handling 1 million records does not take up much memory and
	//sort can be done quickly.
	//This constant value can't be low because K-way merge becomes slower if too many intermediate files
	//are generated
	//So, as per the observation of memory usage, fixing 1 million as the limit
	const int INTERMEDIATE_FILE_RECORD_LIMIT = 1000000;
	
	//Below variable indicates whether any error has occurred during any stage of processing
	bool bErrorOccurred = false;
	
	//Below variable tells how many intermediate files got generated
	size_t fileNum = 0;
	
	//Below list holds the names of intermediate files
	list<string> lstIntermediateFileList;
	
	try
	{
		while(1)
		{
			//Read INTERMEDIATE_FILE_RECORD_LIMIT number of records or till the end of file whichever is earlier
			//Sort records and write to intermediate file
			while(count <= INTERMEDIATE_FILE_RECORD_LIMIT) 
			{
				//Read a record from the input stream
				if(!(in >> ob)) //If read failed
				{
					if(!in.eof()) //If end of file has not reached, this must be an error
					{
						bErrorOccurred = true;
						cout<<"Invalid content format in the input file "<<endl;
						break;
					}
					else
					{
						//cout<<"End of file:"<<endl;
						break;	//End of file
					}
				}
				//Record has been read. Now, check the format of the names in the record
				//First name and Last name must start with an alphabet and
				//There must be comma character at the end in both the names
				//If these conditions are not satisfied, flag it as an error
				else if((ob.firstName.empty() || !isalpha(ob.firstName[0]) || ob.firstName[ob.firstName.length() - 1] != ',')
					 || (ob.lastName.empty() || !isalpha(ob.lastName[0]) || ob.lastName[ob.lastName.length() - 1] != ','))
				{
					bErrorOccurred = true;
					cout<<"Invalid content format in the input file"<<endl;
					break;
				}
				
				//Record has valid content, so, store it in the list
				lst.push_back(ob);
				count++;
			}
			
			if(bErrorOccurred) //Stop reading if error occurred
				break;
			
			//Sort the records in the list using Comparator
			//Comparator determines the order of records based on score, last name, first name
			lst.sort(Comparator());
			//cout<<"Writing to file: "<<endl;
			
			//Write sorted records to an intermediate file for further processing
			if(!writeListToFile(lst, fileNum, lstIntermediateFileList))
			{
				//Final output should not be generated if intermediate files are not generated.
				//Because partial sorted output won't be of any use
				cout<<"Intermediate file creation failed"<<endl;
				in.close();
				outFinal.close();
				return -1;
			}
			
			//Clear the list to start accumulating records for the next intermediate file
			lst.clear();
			count = 1; //Reset record counter
			bErrorOccurred = false;
			fileNum++; //Keep incrementing fileNum to count the number of intermediate files

			if(in.eof()) //End of input file has been reached.
				break;
		}

		//cout<<"fileNum: "<<fileNum<<endl;
		
		//K-Way merge sort happnes here using intermediate files
		if(!bErrorOccurred)
		{
			//Process intermediate files to generate final output file
			vector<ifstream> interFileList(fileNum);
			
			//Open all the intermediate files
			for(size_t i = 0; i < fileNum ; i++)
			{
				std::string fileName = string("Intermediate_") + std::to_string(i) + ".txt";
				interFileList[i].open(fileName);
				if(!interFileList[i].is_open())
				{
					cout<<"Error opening intermediate file: i "<<i<<endl;
					bErrorOccurred = true;
					break;
				}
			}
			
			if(!bErrorOccurred)
			{
				vector<Student> vStudents(fileNum);
				//Read one record from each of the intermediate files
				//Choose the record with highest marks, then descending order of lastname, firstname
				for(size_t i = 0; i < fileNum; i++)
				{
					if(!(interFileList[i]>>vStudents[i]))
					{
						cout<<"File read failed: index i: "<<i<<endl;
						bErrorOccurred = true;
						break;
					}
					vStudents[i].index = i;
				}
				
				//Push the records to a Priority Queue because it handles the Max Heap logic
				//efficiently and keeps the desired record at the top for easy retrieval
				//It is efficient than manually looking for best record each time
				priority_queue<Student, vector<Student>, ComparatorForMaxPriorityQueue> pq;
				for(Student s: vStudents)
					pq.push(s);
				
				Student ob;
				list<Student> outList;
				
				//Below constant is used to define the number of records to be written in output file.
				//Since the disk writes can be slow, it is better to write group of records
				//at a time and flush the buffer instead of writing one by one.
				//As per some executions, below limit seems to work well
				const int OUTPUT_BUFFER_SIZE = 10000;
				
				//Process intermediate files until there are none left
				while(interFileList.size() > 0)
				{
					//Pick the best record from priority queue
					ob = pq.top();
					pq.pop();
					outList.push_back(ob);
					
					//Write group of records at once, when the limit is reached
					if(outList.size() == OUTPUT_BUFFER_SIZE)
					{
						writeOutput(outFinal, outList);
						outList.clear(); //Clear to start accumulating next group of records
					}

					//Read the record from a file whose index is obtained by the best record
					//that was chosen above.
					//Because, we need to read from a file only when the record from that file has been
					//selected, written to the final output file. That's the way merging works.
					if(interFileList[ob.index] >> vStudents[ob.index])
					{
						//Store the index to help determining where to read next record if this record is
						//picked as best
						vStudents[ob.index].index = ob.index;

						//Keep pushing new records to the priority queue which internally maintains best record
						pq.push(vStudents[ob.index]);
						continue;
					}
					else
					{
						//An intermediate file has been read till the end of file
						if(interFileList[ob.index].eof())
						{
							//Delete from the list of intermediate file names
							interFileList.erase(interFileList.begin() + ob.index);
							
							//Delete record from the vector to invalidate this index
							vStudents.erase(vStudents.begin() + ob.index);
						}
					}
				}
				
				//outList will have the last group of records to be written to the final output file
				if(!outList.empty())
				{
					writeOutput(outFinal, outList);
					outList.clear();
				}
			}
		}
	}
	//Catch and print exception information if any unknown exception araises
	catch(const std::exception &e)
	{
		cout<<"Exception caught: "<<e.what()<<endl;
	}
	
	//Delete all intermediate files
	for(auto it = lstIntermediateFileList.begin(); it != lstIntermediateFileList.end(); it++)
	{
		if(remove(it->c_str()) != 0)
		{
			cout<<"Error deleting intermediate file: "<<*it<<endl;
		}
	}
	outFinal.close();
	in.close();
	
	//If any error has occurred, we don't want to generate a partial output file
	//So, remove it
	if(bErrorOccurred)
	{
		if(remove(outFilePath.c_str()) != 0)
		{
			cout<<"Error deleting output file: "<<outFilePath<<endl;
		}
	}
	else
	{
		//In success cases, below log helps us to check the output
		cout<<"Finished: Created "<<outFilePath<<endl;
	}

	return 0;
}
