#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <random>
#include <sstream>
#include <queue>
using namespace std;

class Student
{
public:
	
	int score;
	string firstName;
	string lastName;
	
public:
	int index = 0;

	Student()
	:score(0)
	{
	}
	Student(int score, string fName, string lName, int index)
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
	ofstream out;
	out.open("test_input.txt");
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

bool writeListToFile(const list<Student> & lst, int fileNum)
{
	stringstream s;
	s<<fileNum;
	string str("Intermediate_" + s.str() + ".txt");
	cout<<str<<endl;
	ofstream out;
	out.open(str);
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
	const long long NUM_TEST_RECORDS = 10000000;
	//generateTestInputFile(NUM_TEST_RECORDS);
	
/*	if(argc <= 1)
	{
		cout<<"Pass input file name"<<endl;
		return -1;
	}
	
	cout<<argv[1]<<endl;


	
	string inputFilePath(argv[1]);*/
	string inputFilePath("test_input.txt");
	ifstream in;
	in.open(inputFilePath.c_str());
	if(!in.is_open())
	{
		cout<<"File open failed: "<<endl;
		return -1;
	}

	ofstream outFinal;
	outFinal.open("sorted_output.txt");
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
	const int RECORD_LIMIT = 1000000;
	bool bErrorOccurred = false;
	int fileNum = 0;
	
//	system("rm -rf IntermediateFiles");
//	if(system("mkdir IntermediateFiles") != 0)
//	{
//		cout<<"Failed to create intermediate directories"<<endl;
//		return -1;
//	}
	
	while(1)
	{
		//Read RECORD_LIMIT number of records or till the end of file whichever is earlier
		while(count <= RECORD_LIMIT) 
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
					cout<<"End of file:"<<endl;
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
		
		if(bErrorOccurred || in.eof()) //Stop reading if error occurred or end of file reached
			break;
			
		lst.sort(Comparator());
		cout<<"Writing to file: "<<endl;
		writeListToFile(lst, fileNum);
		lst.clear();
		count = 1;
		bErrorOccurred = false;
		fileNum++;
	}

	cout<<"fileNum: "<<fileNum<<endl;
	if(!bErrorOccurred)
	{
		//Process intermediate files to generate final output file
		vector<ifstream> interFileList(fileNum);
		for(int i = 0; i < fileNum ; i++)
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
			for(int i = 0; i < fileNum; i++)
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
			//Select a record from vStudents and write to the final output file
			int chosenIndex = 0;
			Student ob;
			list<Student> outList;
			const int OUTPUT_BUFFER_SIZE = 10000;
			while(interFileList.size() > 0)
			{
				//ob = chooseRecord(vStudents, chosenIndex);
				ob = pq.top();
				//cout<<ob.score<<"  "<<ob.index<<" "<<ob.lastName<<" "<<ob.firstName<<endl;
				pq.pop();
				//cout<<"Chosen index: "<<ob.index<<endl;
				//cout<<"Writing to final one: "<<endl;
				outList.push_back(ob);
				if(outList.size() == OUTPUT_BUFFER_SIZE)
				{
					writeOutput(outFinal, outList);
					outList.clear();
				}

				if(interFileList[ob.index] >> vStudents[ob.index])
				{
					vStudents[ob.index].index = ob.index;
					//cout<<"Pushing node: "<<ob.index<<endl;

					pq.push(vStudents[ob.index]);
					//cout<<"Newly read record: score: "<<ob.score<<endl;
					continue;
				}
				else
				{
					if(interFileList[ob.index].eof())
					{
						cout<<"Removing file from the list: "<<endl;
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
	//cout<<"Outside:"<<endl;
	outFinal.close();

	
	in.close();
	cout<<"Last: "<<endl;

	return 0;
}
