#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <random>
#include <sstream>
#include <queue>
#include <cstdio>
using namespace std;

//                 Problem statement
//========================================================================================================
//Records need to be in the descending order of scores
//If scores are same, records need to be sorted in the descending alphabetical order of last names
//If last names are also same, records need to be sorted in the descending alphabetical order of first names
//If first names are also same, then the records to be identified as exact duplicates, hence they will occur
//in sequence.

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

class Student
{
public:
	
	int score;
	string firstName;
	string lastName;
	
public:
	size_t index = 0;

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

ostream& operator<<(ostream &out, const Student &s)
{
	out<<s.firstName<<" "<<s.lastName<<" "<<s.score<<endl;
	return out;
}

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
struct Comparator
{
	bool operator()(const Student &s1, const Student &s2)
	{
		if(s1.score == s2.score)
		{
			if(s1.lastName == s2.lastName)
			{
				if(s1.firstName.compare(s2.firstName) > 0)
					return true;
				else
					return false;
			}
			else
			{
				if(s1.lastName.compare(s2.lastName) > 0)
				{
					return true;
				}
				else
					return false;
			}
		}
		else
		{
			if(s1.score > s2.score)
				return true;
			else
				return false;
		}
	}
};

struct ComparatorForMaxPriorityQueue
{
	bool operator()(const Student &s1, const Student &s2)
	{
		if(s1.score == s2.score)
		{
			if(s1.lastName == s2.lastName)
			{
				if(s1.firstName.compare(s2.firstName) < 0)
					return true;
				else
					return false;
			}
			else
			{
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
			if(s1.score < s2.score)
				return true;
			else
				return false;
		}
	}
};

istream& operator>>(istream& in, Student &s)
{
	in>>s.firstName>>s.lastName>>s.score;
	return in;
}

void generateTestInputFile(long long numRecords)
{
	ofstream out("test_input_1k_records");
	Student s;
	for(long long i = 0; i < numRecords; i++)
	{
		s.firstName = RandomNameGenerator::getRandomName();
		s.lastName = RandomNameGenerator::getRandomName();
		s.score = rand() % 101;
		out<<s;
	}
	out.close();
}

bool writeListToFile(const list<Student> & lst, size_t fileNum, list<string> &intermediateFileList)
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

void writeOutput(ofstream &outFinal, list<Student> &lst)
{
	for(auto it = lst.begin(); it != lst.end(); it++)
	{
		outFinal<<*it;
	}
	outFinal.flush();
}

int main(int argc, char *argv[])
{
	//const long long NUM_TEST_RECORDS = 1000;
	//generateTestInputFile(NUM_TEST_RECORDS);
	
	if(argc <= 1)
	{
		cout<<"Pass input file name"<<endl;
		return -1;
	}
	
	cout<<argv[1]<<endl;
	
	string inputFilePath(argv[1]);
	//string inputFilePath("test_input_1k_records.txt");
	size_t pos = inputFilePath.find(".txt");
	string inputFilePathNoExtn;
	if((pos != string::npos) && (inputFilePath.length() == (pos + 4))) //4 is the length of .txt
	{
		inputFilePathNoExtn = inputFilePath.substr(0, pos);
	}
	else
	{
		cout<<"Input file invalid extension "<<endl;
		return -1;
	}
	cout<<"Input path with no extn: "<<inputFilePathNoExtn<<endl;
	ifstream in(inputFilePath.c_str());
	if(!in.is_open())
	{
		cout<<"File open failed: "<<endl;
		return -1;
	}

	ofstream outFinal(inputFilePathNoExtn + "-graded.txt");
	if(!outFinal.is_open())
	{
		cout<<"Output file open failed: "<<endl;
		in.close();
		return -1;
	}
	cout<<"Processing"<<endl;
	list<Student> lst;
	Student ob;
	int count = 1;
	const int INTERMEDIATE_FILE_RECORD_LIMIT = 1000000;
	bool bErrorOccurred = false;
	size_t fileNum = 0;
	list<string> lstIntermediateFileList;
	
	try
	{
		while(1)
		{
			//Read INTERMEDIATE_FILE_RECORD_LIMIT number of records or till the end of file whichever is earlier
			//Sort records and write to intermediate file
			while(count <= INTERMEDIATE_FILE_RECORD_LIMIT) 
			{
				if(!(in >> ob))
				{
					if(!in.eof())
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
				else if((ob.firstName.empty() || !isalpha(ob.firstName[0]) || ob.firstName[ob.firstName.length() - 1] != ',')
					 || (ob.lastName.empty() || !isalpha(ob.lastName[0]) || ob.lastName[ob.lastName.length() - 1] != ','))
				{
					bErrorOccurred = true;
					cout<<"Invalid content format in the input file"<<endl;
					break;
				}
				lst.push_back(ob);
				count++;
			}
			
			if(bErrorOccurred) //Stop reading if error occurred
				break;
				
			lst.sort(Comparator());
			//cout<<"Writing to file: "<<endl;
			if(!writeListToFile(lst, fileNum, lstIntermediateFileList))
			{
				cout<<"Intermediate file creation failed"<<endl;
				in.close();
				outFinal.close();
				return -1;
			}
			lst.clear();
			count = 1;
			bErrorOccurred = false;
			fileNum++;

			if(in.eof())
				break;
		}

		//cout<<"fileNum: "<<fileNum<<endl;
		if(!bErrorOccurred)
		{
			//Process intermediate files to generate final output file
			vector<ifstream> interFileList(fileNum);
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
				//Read one record from all intermediate files
				//Choose the record with highest marks, then descending order of firstname, lastname
				for(size_t i = 0; i < fileNum; i++)
				{
					if(!(interFileList[i]>>vStudents[i]))
					{
						cout<<"File read failed"<<endl;
						bErrorOccurred = true;
						break;
					}
					vStudents[i].index = i;
				}
				
				priority_queue<Student, vector<Student>, ComparatorForMaxPriorityQueue> pq;
				for(Student s: vStudents)
					pq.push(s);
				
				//Process all the intermediate files to generate a final output file
				Student ob;
				list<Student> outList;
				const int OUTPUT_BUFFER_SIZE = 10000;
				while(interFileList.size() > 0)
				{
					ob = pq.top();
					pq.pop();
					outList.push_back(ob);
					if(outList.size() == OUTPUT_BUFFER_SIZE)
					{
						writeOutput(outFinal, outList);
						outList.clear();
					}

					if(interFileList[ob.index] >> vStudents[ob.index])
					{
						vStudents[ob.index].index = ob.index;

						pq.push(vStudents[ob.index]);
						continue;
					}
					else
					{
						if(interFileList[ob.index].eof())
						{
							interFileList.erase(interFileList.begin() + ob.index);
							vStudents.erase(vStudents.begin() + ob.index);
						}
					}
				}
				if(!outList.empty())
				{
					writeOutput(outFinal, outList);
					outList.clear();
				}
			}
		}
	}
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

	return 0;
}
