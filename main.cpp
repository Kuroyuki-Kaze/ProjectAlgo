#define _CRT_RAND_S

#include "modules/secrets.hpp"
#include "modules/tcolors.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>
#include <cstdlib>
#include <fstream>
#include <limits>
#include <cstdio>
#include <ctime>
#include <map>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/ioctl.h>
#endif

using namespace std;

struct Student {
    string name;
    string id;
    char g;
    map<int, vector<float>> term_scores;
    map<int, string> term_grades;
    float gpa;

    Student(string name, string id, char g) {
        this->name = name;
        this->id = id;
        this->g = g;
        this->gpa = 0;
    }

    Student() {
        this->name = string();
        this->id = string();
        this->g = ' ';
        this->gpa = 0;
    }

    bool operator==(const Student& other) const {
        return this->id == other.id;
    }

    string determine_grade(vector<float> scores) {
        float avg = 0;
        for (float score : scores) {
            avg += score;
        }
        avg /= scores.size();

        if (avg >= 90) {
            return "A+";
        }
        else if (avg >= 80) {
            return "A";
        }
        else if (avg >= 75) {
            return "B+";
        }
        else if (avg >= 70) {
            return "B";
        }
        else if (avg >= 65) {
            return "C+";
        }
        else if (avg >= 60) {
            return "C";
        }
        else if (avg >= 55) {
            return "D+";
        }
        else if (avg >= 50) {
            return "D";
        }
        else {
            return "F";
        }
    }

    void update_gpa() {
        float gpa = 0;
        int total_credits = term_grades.size();
        for (auto term : term_grades) {
            if (term.second == "A+") {
                gpa += 4.0;
            }
            else if (term.second == "A") {
                gpa += 3.5;
            }
            else if (term.second == "B+") {
                gpa += 3.0;
            }
            else if (term.second == "B") {
                gpa += 2.5;
            }
            else if (term.second == "C+") {
                gpa += 2.0;
            }
            else if (term.second == "C") {
                gpa += 1.5;
            }
            else if (term.second == "D+") {
                gpa += 1.0;
            }
            else if (term.second == "D") {
                gpa += 0.5;
            }
            else {
                gpa += 0;
            }
        }

        this->gpa = gpa / total_credits;
    }

    void update_grade(int term) {
        term_grades[term] = determine_grade(term_scores[term]);
        update_gpa();
    }

    void add_term_scores(int term, vector<float> scores) {
        term_scores[term] = scores;
        term_grades[term] = determine_grade(scores);
        this->update_grade(term);
    }

    void print_data() {
        cout << "Name: " << name << endl;
        cout << "ID: " << id << endl;
        cout << "Gender: " << g << endl;
        
        for (auto term: term_scores) {
            cout << "Term " << term.first << ":" << endl;
            int count = 1;
            for (auto score: term.second) {
                cout << "Score " << count << ": " << score << endl;
                count++;
            }
            cout << "Grade: " << term_grades[term.first] << endl;
            cout << endl;
        }
        cout << endl;
    }

    string getID() {
        return id;
    }

    float getGPA() {
        return gpa;
    }

    vector<float> *getScores(int term) {
        if (term_scores.find(term) != term_scores.end()) {
            return &term_scores[term];
        }
        else {
            return nullptr;
        }
    }

    void edit_term_scores(int term, vector<float> scores) {
        term_scores[term] = scores;
        term_grades[term] = determine_grade(scores);
        update_gpa();
    }
};

template <class T>
struct Node {
    T data;
    Node<T>* next;
    Node<T>* prev;

    Node(T data) {
        this->data = data;
        this->next = nullptr;
        this->prev = nullptr;
    }

    Node() {
        this->data = T();
        this->next = nullptr;
        this->prev = nullptr;
    }
};

template <class T>
struct List {
    Node<T>* head;
    Node<T>* tail;
    int size;

    List() {
        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    void insert(T data) {
        Node<T>* new_node = new Node<T>;
        new_node->data = data;
        new_node->next = nullptr;
        new_node->prev = nullptr;

        if (head == nullptr) {
            head = new_node;
            tail = new_node;
        }
        else {
            tail->next = new_node;
            new_node->prev = tail;
            tail = new_node;
        }
        size++;
    }

    Node<T>* get(unsigned index) {
        if (index >= size) {
            return nullptr;
        }
        else {
            Node<T>* curr = head;
            for (int i = 0; i < index; i++) {
                curr = curr->next;
            }
            return curr;
        }
    }

    void remove(unsigned index) {
        if (index >= size) {
            return;
        }
        else {
            Node<T>* curr = head;
            for (unsigned i = 0; i < index; i++) {
                curr = curr->next;
            }
            if (curr == head) {
                head = curr->next;
                head->prev = nullptr;
            }
            else if (curr == tail) {
                tail = curr->prev;
                tail->next = nullptr;
            }
            else {
                curr->prev->next = curr->next;
                curr->next->prev = curr->prev;
            }
            delete curr;
            size--;
        }
    }

    void remove(Node<T> *node) {
        if (node == head) {
            head = head->next;
            head->prev = nullptr;
        }
        else if (node == tail) {
            tail = tail->prev;
            tail->next = nullptr;
        }
        else {
            node->prev->next = node->next;
            node->next->prev = node->prev;
        }
        delete node;
        size--;
    }

    void remove(string id);

    Node<T> *search(string id) {
        Node<T> *curr = head;
        while (curr != nullptr) {
            if (curr->data == id) {
                return curr;
            }
            curr = curr->next;
        }
        return nullptr;
    }

    void clear() {
        Node<T> *curr = head;
        while (curr != nullptr) {
            Node<T> *next = curr->next;
            delete curr;
            curr = next;
        }

        head = nullptr;
        tail = nullptr;
        size = 0;
    }
};

template <>
Node<Student> *List<Student>::search(string id) {
    Node<Student> *curr = head;
    while (curr != nullptr) {
        if (curr->data.getID() == id) {
            return curr;
        }
        curr = curr->next;
    }
    return nullptr;
}

template <>
void List<Student>::remove(string id) {
    Node<Student> *before = nullptr;
    Node<Student> *curr = head;

    while (curr != nullptr) {
        if (curr->data.getID() == id) {
            if (before == nullptr && curr->next == nullptr) {
                head = nullptr;
                tail = nullptr;
            }
            else if (before == nullptr) {
                head = curr->next;
                head->prev = nullptr;
            }
            else {
                before->next = curr->next;
                curr->next->prev = before;
            }
            delete curr;
            size--;
            return;
        }
        before = curr;
        curr = curr->next;
    }
}

template <class T>
struct HashTable {
    List<T> *table;
    int size;

    HashTable(int size) {
        this->size = size;
        table = new List<T>[size];
    }

    unsigned hash(string key) {
        unsigned hash = 0;
        for (unsigned i = 0; i < key.length(); i++) {
            hash = (hash << 5) + hash + key[i];
        }
        return hash % size;
    }
    
    void insert(T data);
    void remove(string key);
    void remove(Node<T> *node);
    Node<T>* search(string key);
    void print();

    void writeToFile(string filename);
    void readFromFile(string filename);
    void clear();

    Node<T> *find(unsigned index) {
        return table[index].head;
    }
};

template <>
void HashTable<Student>::insert(Student data) {
    unsigned index = hash(data.getID());
    
    if (table[index].search(data.getID()) != nullptr) {
        cout << "Student already exists." << endl;
        return;
    }

    Node<Student>* node = new Node<Student>;
    node->data = data;
    node->next = nullptr;
    node->prev = nullptr;

    table[index].insert(data);
}

template <>
Node<Student>* HashTable<Student>::search(string key) {
    unsigned index = hash(key);
    Node<Student> *node = this->find(index);

    while (node != nullptr) {
        if (node->data.getID() == key) {
            return node;
        }

        node = node->next;
    }

    return nullptr;
}

template <>
void HashTable<Student>::remove(string key) {
    unsigned index = hash(key);
    Node<Student>* node = table[index].search(key);

    if (node != nullptr) {
        table[index].remove(key);
    }
}

template <>
void HashTable<Student>::print() {
    for (int i = 0; i < size; i++) {
        if (table[i].head == nullptr) {
            continue;
        }

        Node<Student>* curr = table[i].head;
        while (curr != nullptr) {
            curr->data.print_data();
            curr = curr->next;
        }

        cout << "Index: " << i << endl;
    }
}

template <>
void HashTable<Student>::writeToFile(string filename) {
    ofstream file(filename);

    if (!file.is_open()) {
        cout << Tcolors::RED << "Error: Could not open file." << Tcolors::RESET << endl;
        return;
    }

    file << "id,name,gender,term,score\n";

    for (int i = 0; i < size; i++) {
        if (table[i].head == nullptr) {
            continue;
        }

        Node<Student> *curr = table[i].head;
        while (curr != nullptr) {
            if (curr->data.term_scores.size() == 0) {
                file << curr->data.getID() << "," << curr->data.name << "," << curr->data.g << "," << "-1,-1\n";
            }

            for (pair<int, vector<float>> p : curr->data.term_scores) {
                for (float score : p.second) {
                    file << curr->data.getID() << "," << curr->data.name << "," << curr->data.g << "," << p.first << "," << score << "\n";
                }
            }

            curr = curr->next;
        }

    }

    file.close();

    cout << Tcolors::GREEN << "Successfully wrote to file." << Tcolors::RESET << endl;
}

template <>
void HashTable<Student>::readFromFile(string filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cout << Tcolors::RED << "Error: Could not open file." << Tcolors::RESET << endl;
        cout << Tcolors::YELLOW << "Creating new file..." << Tcolors::RESET << endl;

        ofstream new_file(filename);
        new_file << "id,name,gender,term,score\n";
        new_file.close();

        return;
    }

    string line;
    getline(file, line);

    int lastTerm = numeric_limits<int>::min();
    string lastID;

    while (getline(file, line)) {
        stringstream ss(line);
        vector<string> tokens;
        string token;

        

        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        string id = tokens[0];
        string name = tokens[1];
        char g = *(tokens[2]).c_str();
        int term = stoi(tokens[3]);
        float score = stof(tokens[4]);

        if (term == -1 && score == -1) {
            Student stdnt(name, id, g);
            this->insert(stdnt);
        }
        else {
            if (lastID == id) {
                if (lastTerm == term) {
                    vector<float> *scores = this->search(id)->data.getScores(term);
                    scores->push_back(score);
                    this->search(id)->data.update_grade(term);
                }
                else {
                    vector<float> scores = {score};
                    this->search(id)->data.add_term_scores(term, scores);
                }
            } else {
                Student student(name, id, g);
                vector<float> scores = {score};
                student.add_term_scores(term, scores);
                this->insert(student);
            }
        }

        lastTerm = term;
        lastID = id;
    }

    file.close();

    cout << Tcolors::GREEN << "Successfully read from file." << Tcolors::RESET << endl;
}

template <>
void HashTable<Student>::clear() {
    for (int i = 0; i < size; i++) {
        table[i].clear();
    }
}

template <>
void HashTable<Student>::remove(Node<Student> *node) {
    unsigned index = hash(node->data.getID());
    table[index].remove(node);
}

template <class T>
void betterCin(string, T&, string, bool);

void disableEcho();
void enableEcho();
unsigned c_rand();

void print_student_menu();
void print_teacher_menu();
void print_main_menu();


int main() {
    srand(time(NULL));
    unsigned choice;
    HashTable<Student> students(1000);

    cout << Tcolors::YELLOW << "INFO: Starting up, reading from file..." << Tcolors::RESET << endl;
    students.readFromFile("students.csv");

    cout << Tcolors::BOLDGREEN << "Welcome to the student management system!" << Tcolors::RESET << endl;

    while (choice != 6) {
        print_main_menu();
        cout << endl;
        betterCin("Please enter your choice: ", choice, "Invalid choice. Please try again.", false);

        if (choice == 1) {
            string username, password;

            betterCin("Please enter your username: ", username, "Invalid username. Please try again.", false);
            betterCin("Please enter your password: ", password, "Invalid password. Please try again.", true);
            cout << endl;

            bool found = false;
            bool isTeacher = false;

            string line;
            ifstream file("users.txt");

            if (!file.is_open()) {
                cout << Tcolors::RED << "Error: Could not open file." << Tcolors::RESET << endl;
                cout << endl;
                continue;
            }

            string user;
            string hash;
            string salt;
            string digest;

            while (getline(file, line)) {
                user = line.substr(0, line.find(":"));
                hash = line.substr(line.find(":") + 1);

                if (user == username) {
                    salt = hash.substr(0, hash.find("."));
                    digest = hash.substr(hash.find(".") + 1);

                    if (digest == SHA256(password + salt + "isTeacher")) {
                        found = true;
                        isTeacher = true;
                        break;
                    }
                    else if (digest == SHA256(password + salt)) {
                        found = true;
                        isTeacher = false;
                        break;
                    }
                }
            }

            file.close();

            if (!found) {
                cout << Tcolors::RED << "Error: Invalid username or password." << Tcolors::RESET << endl;
                cout << endl;
                continue;
            }
            else {
                if (isTeacher) {
                    cout << Tcolors::YELLOW << "Info: You have entered login information of a teacher." << Tcolors::RESET << endl;
                    cout << endl;
                    continue;
                }
                else {
                    cout << Tcolors::YELLOW << "Info: You have entered login information of a student." << Tcolors::RESET << endl;
                    cout << Tcolors::BLINKGREEN << "Login successful!" << Tcolors::RESET << endl;
                    cout << endl;

                    //Student features
                    // view informations : score, ....


                    int choice2 = 0;

                    while (choice2 != 4) {
                        print_student_menu();
                        cout << endl;

                        betterCin("> Please enter your choice: ", choice2, "Invalid choice. Please try again.", false);

                        if (choice2 == 1) {
                            // view basic informations (name, id, ...)
                            cout << Tcolors::YELLOW << "Info: You have selected to view your basic informations." << Tcolors::RESET << endl;
                            cout << endl;

                            string id;
                            betterCin("> Please enter your ID: ", id, "Invalid ID. Please try again.", false);
                            cout << endl;

                            Node<Student>* node = students.search(id);

                            if (node == nullptr) {
                                cout << Tcolors::RED << "Error: Invalid ID." << Tcolors::RESET << endl;
                                cout << endl;
                                continue;
                            }

                            cout << Tcolors::YELLOW << "Info: Your basic informations are:" << Tcolors::RESET << endl;
                            cout << endl;

                            node->data.print_data();
                        }
                        else if (choice2 == 2) {
                            cout << Tcolors::YELLOW << "Info: You have selected to view your score for a specific term." << Tcolors::RESET << endl;
                            cout << endl;

                            string id;
                            betterCin("> Please enter your ID: ", id, "Invalid ID. Please try again.", false);
                            cout << endl;

                            Node<Student>* node = students.search(id);

                            if (node == nullptr) {
                                cout << Tcolors::RED << "Error: Invalid ID." << Tcolors::RESET << endl;
                                cout << endl;
                                continue;
                            }

                            int term;
                            betterCin("> Please enter the term you want to view: ", term, "Invalid term. Please try again.", false);
                            cout << endl;

                            cout << Tcolors::YELLOW << "Info: Your score for term " << term << " is: " << Tcolors::RESET << endl;
                            cout << endl;

                            vector<float> *scores = node->data.getScores(term);

                            if (scores == nullptr) {
                                cout << Tcolors::RED << "Error: You have not taken any exam in this term." << Tcolors::RESET << endl;
                                cout << endl;
                                continue;
                            } else {
                                cout << "Scores:\n";
                                for (unsigned i = 0; i < scores->size(); i++) {
                                    cout << "Score " << i + 1 << ": " << scores->at(i) << endl;
                                }
                                cout << endl;
                            }
                        }
                        else if (choice2 == 3) {
                            continue;
                        }
                        else if (choice2 == 4) {
                            // logout
                            cout << Tcolors::YELLOW << "Info: You have selected to logout." << Tcolors::RESET << endl;
                            cout << endl;
                            break;
                        }
                        else {
                            cout << Tcolors::RED << "Error: Invalid choice. Please try again." << Tcolors::RESET << endl;
                            cout << endl;
                            continue;
                        }
                    }

                    continue;
                }
            }
        }
        else if (choice == 2) {
            // login as teacher
            string username, password;

            betterCin("Please enter your username: ", username, "Invalid username. Please try again.", false);
            betterCin("Please enter your password: ", password, "Invalid password. Please try again.", true);
            cout << endl;

            bool found = false;
            bool isTeacher = false;

            string line;
            ifstream file("users.txt");

            if (!file.is_open()) {
                cout << Tcolors::RED << "Error: Could not open file." << Tcolors::RESET << endl;
                cout << endl;
                continue;
            }

            string user;
            string hash;
            string salt;
            string digest;

            while (getline(file, line)) {
                user = line.substr(0, line.find(":"));
                hash = line.substr(line.find(":") + 1);

                if (user == username) {
                    salt = hash.substr(0, hash.find("."));
                    digest = hash.substr(hash.find(".") + 1);

                    if (digest == SHA256(password + salt + "isTeacher")) {
                        found = true;
                        isTeacher = true;
                        break;
                    }
                    else if (digest == SHA256(password + salt)) {
                        found = true;
                        isTeacher = false;
                        break;
                    }
                }
            }

            file.close();

            if (!found) {
                cout << Tcolors::RED << "Error: Invalid username or password." << Tcolors::RESET << endl;
                cout << endl;
                continue;
            }
            else {
                if (isTeacher) {
                    cout << Tcolors::YELLOW << "Info: You have entered login information of a teacher." << Tcolors::RESET << endl;
                    cout << Tcolors::BLINKGREEN << "Login successful!" << Tcolors::RESET << endl;
                    cout << endl;
                    // Teacher features
                    // createa, read , update, delete

                    int choiceT = 0;

                    while (choiceT != 12) {
                        print_teacher_menu();
                        cout << endl;

                        betterCin("# Please enter your choice: ", choiceT, "Invalid choice. Please try again.", false);

                        if (choiceT == 1) {
                            // Add a student
                            cout << Tcolors::YELLOW << "Info: You have selected to add a student." << Tcolors::RESET << endl;
                            cout << endl;

                            string id;
                            betterCin("# Please enter the ID of the student: ", id, "Invalid ID. Please try again.", false);
                            cout << endl;

                            string name;
                            betterCin("# Please enter the name of the student: ", name, "Invalid name. Please try again.", false);
                            cout << endl;

                            char gender;
                            betterCin("# Please enter the gender of the student: ", gender, "Invalid gender. Please try again.", false);
                            cout << endl;

                            if (gender != 'M' && gender != 'F') {
                                cout << "Invalid gender. Please try again." << endl;
                                cout << endl;
                                continue;
                            }

                            Student stdnt(name, id, gender);

                            students.insert(stdnt);
                        }
                        else if (choiceT == 2) {
                            // View a student's information

                            cout << Tcolors::YELLOW << "Info: You have selected to view a student's information." << Tcolors::RESET << endl;
                            cout << endl;

                            string id;
                            betterCin("# Please enter the ID of the student: ", id, "Invalid ID. Please try again.", false);
                            cout << endl;

                            Node<Student>* node = students.search(id);

                            if (node == nullptr) {
                                cout << Tcolors::RED << "Error: Invalid ID." << Tcolors::RESET << endl;
                                cout << endl;
                                continue;
                            }

                            cout << Tcolors::YELLOW << "Info: The information of the student is: " << Tcolors::RESET << endl;
                            cout << endl;
                            
                            node->data.print_data();
                        }
                        else if (choiceT == 3) {
                            // View a student's score for a specific term
                            cout << Tcolors::YELLOW << "Info: You have selected to view a student's score for a specific term." << Tcolors::RESET << endl;
                            cout << endl;

                            string id;
                            betterCin("# Please enter the ID of the student: ", id, "Invalid ID. Please try again.", false);
                            cout << endl;

                            Node<Student>* node = students.search(id);

                            int term;
                            betterCin("# Please enter the term you want to view: ", term, "Invalid term. Please try again.", false);
                            cout << endl;

                            vector<float> *scores = node->data.getScores(term);

                            if (scores == nullptr) {
                                cout << Tcolors::RED << "Error: The student have not taken any exam in this term." << Tcolors::RESET << endl;
                                cout << endl;
                                continue;
                            } else {
                                cout << "Scores:\n";
                                for (unsigned i = 0; i < scores->size(); i++) {
                                    cout << "Score " << i + 1 << ": " << scores->at(i) << endl;
                                }
                                cout << endl;
                            }
                        }
                        else if (choiceT == 4) {
                            // Add a student's score for a term
                            cout << Tcolors::YELLOW << "Info: You have selected to add a student's score for a term." << Tcolors::RESET << endl;
                            cout << endl;

                            bool fail = 0;

                            string id;
                            betterCin("# Please enter the ID of the student: ", id, "Invalid ID. Please try again.", false);
                            cout << endl;

                            Node<Student>* node = students.search(id);
                            int term = node->data.term_scores.size() + 1;

                            int how_many;
                            betterCin("# Please enter the number of scores you want to add: ", how_many, "Invalid number. Please try again.", false);
                            cout << endl;

                            vector<float> scores;
                            for (int i = 0; i < how_many; i++) {
                                float score;
                                betterCin("# Please enter the score: ", score, "Invalid score. Please try again.", false);
                                cout << endl;

                                if (score < 0 || score > 100) {
                                    cout << Tcolors::RED << "Error: Invalid score. Please try again." << Tcolors::RESET << endl;
                                    cout << endl;
                                    fail = 1;
                                    break;
                                }

                                scores.push_back(score);
                            }

                            if (fail) {
                                continue;
                            }

                            node->data.add_term_scores(term, scores);
                        }
                        else if (choiceT == 5) {
                            // Edit a student's score for a term
                            cout << Tcolors::YELLOW << "Info: You have selected to edit a student's score for a term." << Tcolors::RESET << endl;
                            cout << endl;

                            string id;
                            betterCin("# Please enter the ID of the student: ", id, "Invalid ID. Please try again.", false);
                            cout << endl;

                            Node<Student>* node = students.search(id);

                            int term;
                            betterCin("# Please enter the term you want to edit: ", term, "Invalid term. Please try again.", false);
                            cout << endl;

                            vector<float> *scores = node->data.getScores(term);

                            if (scores == nullptr) {
                                cout << Tcolors::RED << "Error: The student have not taken any exam in this term." << Tcolors::RESET << endl;
                                cout << endl;
                                continue;
                            }
                            else {
                                cout << "Scores: ";
                                for (unsigned i = 0; i < scores->size(); i++) {
                                    cout << "Score " << i + 1 << ": " << scores->at(i) << endl;
                                }
                                cout << endl;
                            }

                            int how_many;
                            betterCin("# Please enter how many scores you want to add: ", how_many, "Invalid number. Please try again.", false);
                            cout << endl;

                            vector<float> scores_new;
                            for (int i = 0; i < how_many; i++) {
                                float score;
                                betterCin("# Please enter the score: ", score, "Invalid score. Please try again.", false);
                                cout << endl;

                                if (score < 0 || score > 100) {
                                    cout << Tcolors::RED << "Error: Invalid score. Please try again." << Tcolors::RESET << endl;
                                    cout << endl;
                                    continue;
                                }

                                scores_new.push_back(score);
                            }

                            node->data.edit_term_scores(term, scores_new);
                        }
                        else if (choiceT == 6) {
                            // Edit a student's information
                            cout << Tcolors::YELLOW << "Info: You have selected to edit a student's information." << Tcolors::RESET << endl;
                            cout << endl;

                            string id;
                            betterCin("# Please enter the ID of the student: ", id, "Invalid ID. Please try again.", false);
                            cout << endl;

                            Node<Student>* node = students.search(id);

                            if (node == nullptr) {
                                cout << Tcolors::RED << "Error: Invalid ID." << Tcolors::RESET << endl;
                                cout << endl;
                                continue;
                            }

                            cout << "Info: The information of the student is: " << endl;
                            cout << "Name: " << node->data.name << endl;
                            cout << "ID: " << node->data.id << endl;
                            cout << "Gender: " << node->data.g << endl;
                            cout << endl;

                            string newName;
                            string newID;
                            char newGender;

                            betterCin("# Please enter the new name: ", newName, "Invalid name. Please try again.", false);
                            betterCin("# Please enter the new ID: ", newID, "Invalid ID. Please try again.", false);
                            betterCin("# Please enter the new Gender: ", newGender, "Invalid Gender. Please try again.", false);

                            if (newGender != 'M' && newGender != 'F') {
                                cout << "Error: Invalid Gender." << endl;
                                cout << endl;
                                continue;
                            }

                            node->data.name = newName;
                            node->data.id = newID;
                            node->data.g = newGender;
                        }
                        else if (choiceT == 7) {
                            // Remove a student from the system
                            cout << Tcolors::YELLOW << "Info: You have selected to remove a student from the system." << Tcolors::RESET << endl;
                            cout << endl;

                            string id;
                            betterCin("# Please enter the ID of the student: ", id, "Invalid ID. Please try again.", false);
                            cout << endl;

                            Node<Student>* node = students.search(id);

                            if (node == nullptr) {
                                cout << Tcolors::RED << "Error: Invalid ID." << Tcolors::RESET << endl;
                                cout << endl;
                                continue;
                            }

                            students.remove(id);
                        }
                        else if (choiceT == 8) {
                            // Show teacher menu again
                            continue;
                        }
                        else if (choiceT == 9) {
                            // Force reload the student list
                            cout << Tcolors::YELLOW << "Info: You have selected to force reload the student list." << Tcolors::RESET << endl;
                            cout << endl;

                            students.clear();
                            students.readFromFile("students.csv");
                        }
                        else if (choiceT == 10) {
                            // Export the current list to a file
                            cout << Tcolors::YELLOW << "Info: You have selected to export the current list to a file." << Tcolors::RESET << endl;
                            cout << endl;
                            
                            students.writeToFile("students.csv");
                        }
                        else if (choiceT == 11) {
                            // Print all students' information
                            cout << Tcolors::YELLOW << "Info: You have selected to print all students' information." << Tcolors::RESET << endl;
                            cout << endl;

                            students.print();
                        }
                        else if (choiceT == 12) {
                            // Exit the teacher menu
                            cout << Tcolors::YELLOW << "Info: You have selected to exit the teacher menu." << Tcolors::RESET << endl;
                            cout << endl;
                            
                            cout << Tcolors::BLINKRED << "Warning: You may have unsaved changes to the list, do you want to save the list? (Y/N): " << Tcolors::RESET << endl;
                            cout << endl;

                            char choiceSave;
                            cin >> choiceSave;

                            if (choiceSave == 'Y' || choiceSave == 'y') {
                                students.writeToFile("students.csv");
                            }
                            else if (choiceSave == 'N' || choiceSave == 'n') {
                                cout << Tcolors::YELLOW << "Info: You have selected to not save the list." << Tcolors::RESET << endl;
                                cout << endl;
                            }
                            else {
                                cout << Tcolors::RED << "Error: Invalid choice." << Tcolors::RESET << endl;
                                cout << endl;
                            }

                            break;
                        }
                        // TODO: another choice for viewing all students' information sorted by GPA
                        else {
                            cout << Tcolors::RED << "Error: Invalid choice. Please try again." << Tcolors::RESET << endl;
                            cout << endl;
                            continue;
                        }
                    }

                    continue;
                }
                else {
                    cout << Tcolors::YELLOW << "Info: You have entered login information of a student." << Tcolors::RESET << endl;
                    cout << endl;

                    continue;
                }
            }
        }
        else if (choice == 3) {
            string username, password, confPassword;

            betterCin("Please enter your username: ", username, "Invalid username. Please try again.", false);
            betterCin("Please enter your password: ", password, "Invalid password. Please try again.", true);
            cout << endl;

            betterCin("Please enter your password again: ", confPassword, "Invalid password. Please try again.", true);
            cout << endl;

            if (password != confPassword) {
                cout << Tcolors::RED << "Error: Passwords do not match." << Tcolors::RESET << endl;
                cout << endl;
                continue;
            }

            bool found = false;

            string line;
            ifstream file("users.txt");

            if (!file.is_open()) {
                cout << Tcolors::RED << "Error: Could not open file." << Tcolors::RESET << endl;
                cout << endl;
                continue;
            }

            string user;
            string hash;
            string salt;
            string digest;

            while (getline(file, line)) {
                user = line.substr(0, line.find(":"));
                hash = line.substr(line.find(":") + 1);

                if (user == username) {
                    found = true;
                    break;
                }
            }

            file.close();

            if (found) {
                cout << Tcolors::RED << "Error: Username already exists." << Tcolors::RESET << endl;
                continue;
            }
            else {
                vector<char> v_salt(0);
                for (int i = 0; i < 16; i++) {
                    v_salt.push_back(c_rand());
                }
                string salt = string(v_salt.begin(), v_salt.end());

                digest = SHA256(password + salt);

                ofstream ofile("users.txt", ios::app);

                if (!ofile.is_open()) {
                    cout << Tcolors::RED << "Error: Could not open file." << Tcolors::RESET << endl;
                    cout << endl;
                    continue;
                }

                ofile << username << ":" << salt << "." << digest << endl;

                ofile.close();

                cout << Tcolors::BLINKGREEN << "Registration successful!" << Tcolors::RESET << endl;
                cout << endl;
                continue;
            }
        }
        else if (choice == 4) {
            // register as teacher
            string username, password, confPassword;

            betterCin("Please enter your username: ", username, "Invalid username. Please try again.", false);
            betterCin("Please enter your password: ", password, "Invalid password. Please try again.", true);
            cout << endl;

            betterCin("Please enter your password again: ", confPassword, "Invalid password. Please try again.", true);
            cout << endl;

            if (password != confPassword) {
                cout << Tcolors::RED << "Error: Passwords do not match." << Tcolors::RESET << endl;
                cout << endl;
                continue;
            }

            bool found = false;

            string line;
            ifstream file("users.txt");

            if (!file.is_open()) {
                cout << Tcolors::RED << "Error: Could not open file." << Tcolors::RESET << endl;
                cout << endl;
                continue;
            }

            string user;
            string hash;
            string salt;
            string digest;

            while (getline(file, line)) {
                user = line.substr(0, line.find(":"));
                hash = line.substr(line.find(":") + 1);

                if (user == username) {
                    found = true;
                    break;
                }
            }

            file.close();

            if (found) {
                cout << Tcolors::RED << "Error: Username already exists." << Tcolors::RESET << endl;
                continue;
            }
            else {
                vector<char> v_salt(0);
                for (int i = 0; i < 16; i++) {
                    v_salt.push_back(c_rand());
                }
                string salt = string(v_salt.begin(), v_salt.end());

                digest = SHA256(password + salt + "isTeacher");

                ofstream ofile("users.txt", ios::app);

                if (!ofile.is_open()) {
                    cout << Tcolors::RED << "Error: Could not open file." << Tcolors::RESET << endl;
                    cout << endl;
                    continue;
                }

                ofile << username << ":" << salt << "." << digest << endl;

                ofile.close();

                cout << Tcolors::BLINKGREEN << "Registration successful!" << Tcolors::RESET << endl;
                cout << endl;
                continue;
            }
        }
        else if (choice == 5) {
            cout << "1. Login as a student" << endl;
            cout << "2. Login as a teacher" << endl;
            cout << "3. Register as a student" << endl;
            cout << "4. Register as a teacher" << endl;
            cout << "5. Show help again" << endl;
            cout << "6. Exit" << endl;
            cout << endl;
        }
        else if (choice == 6) {
            cout << Tcolors::BLINKGREEN << "Goodbye!" << Tcolors::RESET << endl;
            cout << endl;
            continue;
        }
        else {
            cout << Tcolors::RED << "Error: Invalid choice. Please try again." << Tcolors::RESET << endl;
            cout << endl;
            continue;
        }
    }
}

template <class T>
void betterCin(string __Prompt, T& __Return_Buffer, string __Error_Message, bool __Hidden) {
    cout << __Prompt;

    if (__Hidden) {
        disableEcho();
    }

    cin >> __Return_Buffer;

    if (__Hidden) {
        enableEcho();
    }

    if (cin.fail()) {
        cout << Tcolors::RED << __Error_Message << Tcolors::RESET << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// disableEcho() disables writing to the terminal
#ifdef _WIN32
void disableEcho() {
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));
}
#else
void disableEcho() {
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    tty.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}
#endif

// enableEcho() enables writing to the terminal
#ifdef _WIN32
void enableEcho() {
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode | ENABLE_ECHO_INPUT);
}
#else
void enableEcho() {
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    tty.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}
#endif

// crand() returns a random unsigned integer
#ifdef _WIN32
unsigned c_rand() {
    unsigned r, v;
    rand_s(&r);
    v = (unsigned)((double)r / (double)UINT_MAX * 127);

    if (!(v >= 48 && v <= 57) && !(v >= 65 && v <= 90) && !(v >= 97 && v <= 122)) {
        v = c_rand();
    }

    return v;
}
#else
unsigned c_rand() {
    unsigned r = rand() % 127;

    if (!(r >= 48 && r <= 57) && !(r >= 65 && r <= 90) && !(r >= 97 && r <= 122)) {
        r = c_rand();
    }

    return r;
}
#endif

void print_student_menu() {
    cout << "1. View basic information" << endl;
    cout << "2. View score for a specific term" << endl;
    cout << "3. Show the student menu again" << endl;
    cout << "4. Exit" << endl;
    cout << endl;
}

void print_teacher_menu() {
    cout << "1. Add a student into the system" << endl;
    cout << "2. View a student information" << endl;
    cout << "3. View a student's score for a specific term" << endl;
    cout << "4. Add a student's score for a term" << endl;
    cout << "5. Edit a student's score for a term" << endl;
    cout << "6. Edit a student's information" << endl;
    cout << "7. Remove a student from the system" << endl;
    cout << "8. Show the teacher menu again" << endl;
    cout << "9. Import student information from a file" << endl;
    cout << "10. Export student information to a file" << endl;
    cout << "11. Show all students' information" << endl;
    cout << "12. Exit" << endl;
    cout << endl;
}

void print_main_menu() {
    cout << "Please select an option:" << endl;
    cout << "1. Login as a student" << endl;
    cout << "2. Login as a teacher" << endl;
    cout << "3. Register as a student" << endl;
    cout << "4. Register as a teacher" << endl;
    cout << "5. Show help again" << endl;
    cout << "6. Exit" << endl;
    cout << endl;
}