//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Nyzheir Warner
// Version     : 1.0
// Copyright   : Copyright � 2023 SNHU COCE
// Description : Project 2
//============================================================================

#include <iostream>
#include <time.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <set>

using namespace std;

struct Course {
	string courseID; 
	string title;
	vector<string> prerequisite;
};

struct Node {
    Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) : Node() {
        course = aCourse;
    }
};

class BinarySearchTree {

private:
    Node* root;
    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    void printNode(Course course);

public:
    BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    Course Search(string courseID);
};

void BinarySearchTree::addNode(Node* node, Course course) {
    if (node->course.courseID.compare(course.courseID) > 0)
    {
        if (node->left == nullptr) {
            node->left = new Node(course);
        }
        else {
            addNode(node->left, course);
        }
    }
    else {
        if (node->right == nullptr)
        {
            node->right = new Node(course);
        }
        else
        {
            addNode(node->right, course);
        }
    }
}

void BinarySearchTree::inOrder(Node* node) {
    if (node != nullptr)
    {
        inOrder(node->left);
        printNode(node->course);
        inOrder(node->right);
        
    }
}

void BinarySearchTree::printNode(Course course) {
    cout << course.courseID << ", " << course.title << endl;
}

BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

void BinarySearchTree::InOrder() {
    inOrder(root);
}

void BinarySearchTree::Insert(Course course) {
    if (root == nullptr)  //if root is null set root equal new node with course
    {
        root = new Node(course);
    }
    else {
        addNode(root, course);  // add course to tree 
    }
}

Course BinarySearchTree::Search(string courseID) {
    Node* curr = root;
    while (curr != nullptr)
    {
        if (curr->course.courseID.compare(courseID) == 0) //if match found return course
        {
            return curr->course;
        }
        if (courseID.compare(curr->course.courseID) < 0)  //if passed id smaller than curr id traverse left else go right 
        {
            curr = curr->left;
        }
        else
        {
            curr = curr->right;
        }
    }
    Course course;
    return course;
}

string toUpper(string str) {
    for (auto& c : str) c = (char)toupper(c); //funciton to convert string to uppercase 
    return str;
}

void loadCourses(const string& csvPath, BinarySearchTree* tree) {
    cout << "Loading Binary Search Tree . . . " << endl;

    ifstream myfile(csvPath);
    if (!myfile.is_open()) 
    {
        throw runtime_error("Could not open file");
    }

    string line;
    set<string> validIDs;
    vector<Course> courses; // temp to store course validated courses before adding to tree

    while (getline(myfile, line)) 
    {
        istringstream ss(line);
        string field;
        vector<string> fields;
       
        while (getline(ss, field, ',')) 
        {
            field.erase(0, field.find_first_not_of(" \t"));
            field.erase(field.find_last_not_of(" \t") + 1);  // Trim spaces from each field
            fields.push_back(field);
        }

        if (fields.size() >= 2) 
        {
            Course course;
            course.courseID = fields[0];
            course.title = fields[1];

            validIDs.insert(course.courseID); // Add course ID to validIDs

           
            if (fields.size() > 2) // Handle additional fields if present
            {
                vector<string> validPrerequisites;
                for (size_t i = 2; i < fields.size(); ++i) 
                {
                    if (validIDs.find(fields[i]) != validIDs.end()) 
                    {
                        validPrerequisites.push_back(fields[i]);
                    }
                }
                course.prerequisite = validPrerequisites;
            }
            courses.push_back(course); // Store course temporarily
        }
    }
    myfile.close();
    for (const auto& course : courses)
    {
        tree->Insert(course);
    }
}


int main(int argc, char* argv[])
{
    // process command line arguments
    string csvPath, courseID;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        courseID = "CSCI400";
        break;
    case 3:
        csvPath = argv[1];
        courseID = argv[2];
        break;
    default:
        csvPath = "CS_300_ABCU_Advising_Program_Input.csv";
        courseID = "CSCI400";
    }

    // Define a timer variable
    clock_t ticks;
    bool dataLoaded = false; // bool to check if data is loaded


    // Define a binary search tree to hold all courses
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;

    int choice = 0;
    cout << "Welcome to the course planner." << endl;
    while (choice != 9) {
        cout << "\n  1. Load Data Structure." << endl;
        cout << "  2. Print Course List." << endl;
        cout << "  3. Print Course." << endl;
        cout << "  9. Exit\n" << endl;       
        cout << "Enter choice: ";
        cin >> choice;
        cout << endl;


        switch (choice) {

        case 1:
            if (!dataLoaded)
            {
                // Initialize a timer variable before loading 
                ticks = clock();

                // Complete the method call to load the courses
                loadCourses(csvPath, bst);
                dataLoaded = true;

                // Calculate elapsed time and display result
                ticks = clock() - ticks; // current clock ticks minus starting clock ticks
                //cout << "time: " << ticks << " clock ticks" << endl;
                cout << "Loading Time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds \n" << endl;              
            }
            else
            {
                cout << "Data already Loaded." << endl;                
            }
            break;

        case 2:
            if (!dataLoaded)
            {
                cout << "Load data first." << endl;
            }
            else
            {
                cout << "Here is a sample schedule: " << endl;
                bst->InOrder();
                cout << endl;
            }
            break;

        case 3: 
            if (!dataLoaded)
            {
                cout << "Load data first." << endl;
            }
            else
            {
                cout << "What course do you want to know about?";
                cin >> courseID;
                courseID = toUpper(courseID);
                course = bst->Search(courseID);

                if (!course.courseID.empty()) {
                    cout << course.courseID << ", " << course.title << endl;

                    cout << "Prerequisites: ";  // Print prerequisites
                    if (!course.prerequisite.empty()) {
                        for (size_t i = 0; i < course.prerequisite.size(); ++i) {
                            cout << course.prerequisite[i];
                            if (i < course.prerequisite.size() - 1) {
                                cout << ", ";
                            }
                        }
                    }
                    else {
                        cout << "None";
                    }
                    cout << endl;
                }
                else {
                    cout << "Course " << courseID << " not found." << endl;
                }
            }
            break;

        case 9:
            cout << "Good bye." << endl;
            return 0;

        default:
            cout << choice << " is not a valid option." << endl;
            break;
        }  
    }
    return 0;
}

 