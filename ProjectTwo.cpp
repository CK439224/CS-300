//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Chris King
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : CS-300 Project Two
//============================================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <sstream>

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

//Each course will have an Id, Name, and Vector to hold the prereqs
struct Course {
	string courseNum;
	string courseName;
	vector <string> preReqs;

	Course(){}
};


struct Node {
	Course course;
	Node* left;
	Node* right;

	Node() {
		left = nullptr;
		right = nullptr;
	}

	Node(Course aCourse) : Node() {
		this->course = aCourse;
	}
};

//============================================================================
// CourseBST Definition
//============================================================================

class CourseBST {
private:
	Node* root;
	void addNode(Node* node, Course course);
	void printCourseList(Node* node);
	void printCourseInfo(Node* node, string courseNum);

public:
	CourseBST ();
	virtual ~CourseBST();
	void DeleteRec(Node* node);
	void Insert(Course course);
	int NumPrereqCourses(Course course);
	void PrintCourseList();
	void PrintCourseInfo(string courseNum);
};

//Construct a BST and initiates
CourseBST::CourseBST() {
	root = nullptr;
}

//Deconstruct BST
CourseBST::~CourseBST() {
	DeleteRec(root);
}

//Delete BST recursively
void CourseBST::DeleteRec(Node* node) {
	if (node) {
		DeleteRec(node->left);
		DeleteRec(node->right);
		delete node;
	}
}

//Insert node into BST
void CourseBST::Insert(Course course) {
	if (root == nullptr) {
		root = new Node(course);
	}
	else {
		this->addNode(root, course);
	}
}

//Count number of PreReqs
int CourseBST::NumPrereqCourses(Course course) {
	int count = 0;
	for (unsigned int i = 0; i < course.preReqs.size(); i++) {
		if (course.preReqs.at(i).length() > 0) {
			count = count + 1;
		}
	}
	return count;
}

//Pass BST root to private method
void CourseBST::PrintCourseList() {
	this->printCourseList(root);
}

//Pass BST root and course Id to private method
void CourseBST::PrintCourseInfo(string courseNum) {
	this->printCourseInfo(root, courseNum);
}

//Using the Insert method
//If current child is nullptr
//Adds the node to the BST
//If current child is not nullptr,
//it continues to traverse tree until nullptr is found.
void CourseBST::addNode(Node* node, Course course) {

	if (node->course.courseNum.compare(course.courseNum) > 0) {
		if (node->left == nullptr) {
			node->left = new Node(course);
		}
		else {
			this->addNode(node->left, course);
		}
	}

	else {
		if (node->right == nullptr) {
			node->right = new Node(course);
		}
		else {
			this->addNode(node->right, course);
		}
	}
}

//Recursibely prints loaded course list in order
void CourseBST::printCourseList(Node* node) {
	if (node != nullptr) {
		printCourseList(node->left);
		cout << node->course.courseNum << ", " << node->course.courseName << endl;
		printCourseList(node->right);
	}
}

//Prints the a single course information and prerequisits
void CourseBST::printCourseInfo(Node* curr, string courseNum) {


		while (curr != nullptr) {
			if (curr->course.courseNum.compare(courseNum) == 0) {
				cout << endl << endl << curr->course.courseNum << ", " << curr->course.courseName << endl;
				int size = NumPrereqCourses(curr->course);
				cout << "Required Prerequisite(s): " << endl;
				//Loops through the prerequisites if prerequisites are found
				int i = 0;
				for (i = 0; i < size; i++) {
					cout << curr->course.preReqs.at(i);
					if (i != size - 1) {
						cout << ", ";
					}
				}
				//If no prerequisites are found
				if (i == 0) {
					cout << endl;
					cout << "No Prerequisites Required for " << curr->course.courseName << "\n" << endl;
					return;
				}
				break;

			}
			//Moves down through the tree based on comparison
			else if (courseNum.compare(curr->course.courseNum) < 0) {
				curr = curr->left;
			}
			else {
				curr = curr->right;
			}
		}
		//If course is not found
		if (curr == nullptr) {
			cout << "Course " << courseNum << " Not found." << endl;
		}
		cout << "\n";

}



//Load courses from CSV file entered by user
bool loadCourses(string csvPath, CourseBST* courseBST) {
	//Opens course file, gets and seperates each line and inserts into BST
	try {
		ifstream courseFile(csvPath);
		
		if (courseFile.is_open()) {
			while (!courseFile.eof()) {
				vector<string> courseInfo;
				string courseData;
				getline(courseFile, courseData);

				istringstream iss(courseData);
				string field;
				while (getline(iss, field, ',')) {
					courseInfo.push_back(field);
				}
				
				if (courseInfo.size() >= 2) {
					Course course;
					course.courseNum = courseInfo[0];
					course.courseName = courseInfo[1];

					for (unsigned int i = 2; i < courseInfo.size(); i++) {
						course.preReqs.push_back(courseInfo[i]);
					}

					courseBST->Insert(course);
				}
			}
			courseFile.close();
			return true;
		}
	}

	catch (const std::exception& e) {
		std::cerr << e.what() << endl;
	}

	return false;
}

//Main Method

int main(int argc, char* argv[]){
	//Process comand line argument
	string csvPath, courseId;
	switch (argc) {
	case 2:
		csvPath = argv[1];
		break;
	case 3:
		csvPath = argv[1];
		courseId = argv[2];
		break;
	default:
		csvPath = "";
		break;
	}

	//Initialize BST
	CourseBST* courseBST = nullptr;

	//Welcome user to advising assistance software
	cout << endl;
	cout << "*******************************************" << endl;
	cout << "* WELCOME TO ADVISING ASSISTANCE SOFTWARE *" << endl;
	cout << "*******************************************\n" << endl;

	string choice = "0";
	int userChoice = choice[0] - '0';

	//Display menu until 9 is chosen
	while (userChoice != 9) {

		cout << " 1. Load Data Structure" << endl;
		cout << " 2. Print Course List" << endl;
		cout << " 3. Print Course" << endl;
		cout << " 9. Exit" << endl;
		cout << endl;
		cout << "Please enter a choice listed above:" << endl;
		cin >> choice;

		if (choice.length() == 1) {
			userChoice = choice[0] - '0';
		}
		else {
			userChoice = 6;
		}

		bool fileOpened = 1;

		switch (userChoice) {
		case 1:
			if (courseBST == nullptr) {
				courseBST = new CourseBST();
			}
			if (csvPath.length() == 0) {
				cout << "Please enter the file name that contains the Course Data: " << endl;
				cin >> csvPath;
			}
			fileOpened = loadCourses(csvPath, courseBST); 
			if (fileOpened) {
				cout << endl;
				cout << "****************************" << endl;
				cout << "* File opened sucessfully. *" << endl;
				cout << "****************************\n" << endl;
				break;
			}
			else {
				cout << "******************************" << endl;
				cout << "* File could not be located. *" << endl;
				cout << "******************************" << endl;
				csvPath = "";
				break;
			}
		case 2:
			if (courseBST != nullptr && fileOpened) {
				cout << "Here is the course list:\n" << endl;
				courseBST->PrintCourseList();
				cout << endl;
				break;
			}
			else {
				cout << "******************************" << endl;
				cout << "* Please load courses first. *" << endl;
				cout << "******************************" << endl;
				break;
			}
		case 3:
			courseId = "";
			if (courseBST != nullptr && fileOpened) {
				if (courseId.length() == 0) {
					cout << "What course do you want to search for?\n";
					cin >> courseId;
					for (auto& userChoice : courseId) userChoice = toupper(userChoice);
				}
				courseBST->PrintCourseInfo(courseId);
				cout << endl;

				break;
			}
			else {
				cout << "******************************" << endl;
				cout << "* Please load courses first. *" << endl;
				cout << "******************************" << endl;
				break;
			}
		case 9:
			cout << "*****************************************************" << endl;
			cout << "* Thank you for using ADVISING ASSISTANCE SOFTWARE. *" << endl;
			cout << "*****************************************************" << endl;
			break;

		default:
			cout << endl;
			cout << "****************************" << endl;
			cout << "* " << choice << " is not a valid option. *" << endl;
			cout << "****************************\n" << endl;
			break;
		}
	}

	delete courseBST;
	return 0;
}

