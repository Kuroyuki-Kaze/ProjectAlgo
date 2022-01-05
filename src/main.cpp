#define _CRT_RAND_S

#include "../modules/secrets.hpp"
#include "../modules/tcolors.hpp"
#include "../modules/logging.hpp"

// what's a header? never heard of it.

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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"

#pragma region PreMain
Logging logger = Logging();
string fmt = "[%(lvl)s]: %(msg)s";

void linfo(std::string msg, std::string fmt) {
    logger.info(msg, Tcolors::YELLOW + fmt + Tcolors::RESET);
}

void lsuccess(std::string msg, std::string __fmt) {
    logger.info(msg, Tcolors::GREEN + __fmt + Tcolors::RESET);
}

void lerror(std::string msg, std::string fmt) {
    logger.error(msg, Tcolors::RED + fmt + Tcolors::RESET);
}

void lblinksuccess(std::string msg, std::string __fmt) {
    logger.info(msg, Tcolors::BLINKGREEN + __fmt + Tcolors::RESET);
}

void lwarn(std::string msg, std::string fmt) {
    logger.warn(msg, Tcolors::BOLDYELLOW + fmt + Tcolors::RESET);
}

std::string cut_off(std::string str, int precision, char delim = '.') {
    int pos = str.find(delim);
    if (pos == -1) {
        return str;
    }
    return str.substr(0, pos + precision + 1);
}

template <typename T>
std::string vec_s (std::vector<T> &vec);

template <>
std::string vec_s (std::vector<float> &vec) {
    std::string s = "";

    for (auto &i : vec) {
        s += cut_off(std::to_string(i), 2) + ", ";
    }

    return s;
}

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
        // tabular printing format
        // +-+-+-+-+-+-+-+-+
        // | Name | ID | Gender |
        // +-+-+-+-+-+-+-+-+
        // | term | scores (comma separated) | grade |
        // +-+-+-+-+-+-+-+-+

        // print header
        cout << Tcolors::RESET << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+" << endl;
        cout << Tcolors::BOLDWHITE << "| " << setw(40) << "Name" << " | " << setw(20) << "ID" << " | " << setw(6) << "Gender" << " | " << setw(6) << "GPA" << " |" << endl;
        cout << Tcolors::RESET << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+" << endl;

        cout << Tcolors::BOLDWHITE << "| " << setw(40) << this->name << " | " << setw(20) << this->id << " | " << setw(6) << this->g << " | " << setw(6) << cut_off(std::to_string(this->getGPA()), 2) << " |" << endl;
        cout << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+" << endl;

        cout << Tcolors::BOLDWHITE << "| " << setw(10) << "Term" << " | " << setw(50) << "Scores (comma separated)" << " | " << setw(15) << "Grade" << " |" << endl;
        cout << Tcolors::RESET << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+" << endl;
        
        // print data
        for (auto term : term_scores) {
            cout << Tcolors::BOLDWHITE << "| " << setw(10) << term.first << " | " << setw(50) << std::left << vec_s(term.second) << std::right << " | " << setw(15) << term_grades[term.first] << " |" << endl;
        }

        cout << Tcolors::RESET << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+" << endl;
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

    int getSize() {
        return size;
    }

    int getEffectiveSize() {
        int count = 0;
        for (int i = 0; i < size; i++) {
            if (table[i].head != nullptr) {
                count++;
            }
        }
        return count;
    }

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
        lerror("Student with ID " + data.getID() + " already exists", fmt);
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
            cout << "\n\n" << endl;
            curr = curr->next;
        }

        logger.debug("Index: " + to_string(i));
    }
}

template <>
void HashTable<Student>::writeToFile(string filename) {
    ofstream file(filename);

    if (!file.is_open()) {
        lerror("Could not open file " + filename, fmt);
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

    lsuccess("Successfully wrote to file " + filename, "[SUCCESS]: %(msg)s");
}

template <>
void HashTable<Student>::readFromFile(string filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        lerror("Could not open file " + filename, fmt);
        lerror("Creating new file...", fmt);

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

    lsuccess("Successfully read from file " + filename, fmt);
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
#pragma endregion PreMain

#pragma GCC diagnostic pop

int main() {
    srand(time(NULL));
    unsigned choice;
    HashTable<Student> students(1000);

    int level = Logging::__WARN;
    logger.configure(level, fmt);

    linfo("Starting program...", fmt);
    linfo("Loading data from file...", fmt);
    Sleep(700);
    students.readFromFile("students.csv");
    linfo("Data loaded successfully!", fmt);
    cout << endl;

    if (level >= Logging::__INFO) {
        system("pause");
    }
    
    system("cls");

    cout << Tcolors::BOLDGREEN << "Welcome to the student management system!" << Tcolors::RESET << endl;
    cout << endl;

    while (choice != 6) {
        print_main_menu();
        cout << endl;
        betterCin(Tcolors::BLINKCYAN + "Please enter your choice: " + Tcolors::RESET, choice, Tcolors::RED + "Invalid choice. Please try again." + Tcolors::RESET, false);
        cout << endl;

        if (choice == 1) {
            string username, password;

            betterCin(Tcolors::CYAN + "Please enter your username: " + Tcolors::RESET, username, Tcolors::RED +  "Invalid username. Please try again." + Tcolors::RESET, false);
            betterCin(Tcolors::CYAN + "Please enter your password: " + Tcolors::RESET, password, Tcolors::RED +  "Invalid password. Please try again." + Tcolors::RESET, true);
            cout << endl;

            bool found = false;
            bool isTeacher = false;

            string line;
            ifstream file("users.txt");

            if (!file.is_open()) {
                lerror("Could not open file users.txt", fmt);
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
                lerror("Invalid username or password", fmt);
                cout << endl;
                continue;
            }
            else {
                if (isTeacher) {
                    linfo("You have entered a teacher's credentials.", fmt);
                    cout << endl;
                    continue;
                }
                else {
                    linfo("You have entered a student's credentials.", fmt);
                    lblinksuccess("Login succesful!", "[SUCCESS]: %(msg)s");
                    cout << endl;

                    //Student features
                    // view informations : score, ....


                    int choiceS = 0;

                    while (choiceS != 4) {
                        print_student_menu();
                        cout << endl;

                        betterCin(Tcolors::BLINKCYAN + "> Please enter your choice: " + Tcolors::RESET, choiceS, Tcolors::RED + "Invalid choice. Please try again." + Tcolors::RESET, false);
                        cout << endl;

                        if (choiceS == 1) {
                            // view basic informations (name, id, ...)
                            linfo("You have selected to view your basic informations.", fmt);
                            cout << endl;

                            string id;
                            betterCin("> Please enter your ID: ", id, Tcolors::RED + "Invalid ID. Please try again." + Tcolors::RESET, false);
                            cout << endl;

                            Node<Student>* node = students.search(id);

                            if (node == nullptr) {
                                lerror("Could not find student with ID " + id, fmt);
                                cout << endl;
                                continue;
                            }

                            linfo("Your basic informations are:", fmt);
                            cout << endl;

                            node->data.print_data();
                        }
                        else if (choiceS == 2) {
                            linfo("You have selected to view your score for a specific term.", fmt);
                            cout << endl;

                            string id;
                            betterCin("> Please enter your ID: ", id, Tcolors::RED + "Invalid ID. Please try again." + Tcolors::RESET, false);
                            cout << endl;

                            Node<Student>* node = students.search(id);

                            if (node == nullptr) {
                                lerror("Could not find student with ID " + id, fmt);
                                cout << endl;
                                continue;
                            }

                            int term;
                            betterCin("> Please enter the term you want to view: ", term, Tcolors::RED + "Invalid term. Please try again." + Tcolors::RESET, false);
                            cout << endl;

                            linfo("Your score for term " + to_string(term) + " is:", fmt);
                            cout << endl;

                            vector<float> *scores = node->data.getScores(term);

                            if (scores == nullptr) {
                                lerror("You have not taken any exams in this term", fmt);
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
                        else if (choiceS == 3) {
                            cout << endl;
                            continue;
                        }
                        else if (choiceS == 4) {
                            // logout
                            linfo("You have selected to logout.", fmt);
                            cout << endl;
                            break;
                        }
                        else {
                            lerror("Invalid choice.", fmt);
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

            betterCin(Tcolors::CYAN + "Please enter your username: " + Tcolors::RESET, username, Tcolors::RED + "Invalid username. Please try again." + Tcolors::RESET, false);
            betterCin(Tcolors::CYAN + "Please enter your password: " + Tcolors::RESET, password, Tcolors::RED + "Invalid password. Please try again." + Tcolors::RESET, true);
            cout << endl;

            bool found = false;
            bool isTeacher = false;

            string line;
            ifstream file("users.txt");

            if (!file.is_open()) {
                lerror("Could not open file users.txt", fmt);
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
                lerror("Invalid username or password", fmt);
                cout << endl;
                continue;
            }
            else {
                if (isTeacher) {
                    linfo("You have entered a teacher's credentials.", fmt);
                    lblinksuccess("Login succesful!", "[SUCCESS]: %(msg)s");
                    cout << endl;
                    // Teacher features
                    // createa, read , update, delete

                    int choiceT = 0;

                    while (choiceT != 12) {
                        print_teacher_menu();
                        cout << endl;

                        // print the size of the students list
                        linfo("The number of students in the system is: " + to_string(students.getEffectiveSize()), fmt);

                        betterCin("# Please enter your choice: ", choiceT, Tcolors::RED + "Invalid choice. Please try again." + Tcolors::RESET, false);
                        cout << endl;
                            
                        if (choiceT == 1) {
                            // Add a student
                            linfo("You have selected to add a student.", fmt);
                            cout << endl;

                            char repeatChoice;
                            while (true) {
                                string id;
                                betterCin("# Please enter the ID of the student: ", id, Tcolors::RED + "Invalid ID. Please try again." + Tcolors::RESET, false);
                                cout << endl;

                                string name;
                                betterCin("# Please enter the name of the student: ", name, Tcolors::RED + "Invalid name. Please try again." + Tcolors::RESET, false);
                                cout << endl;

                                char gender;
                                betterCin("# Please enter the gender of the student: ", gender, Tcolors::RED + "Invalid gender. Please try again." + Tcolors::RESET, false);
                                cout << endl;

                                if (gender != 'M' && gender != 'F') {
                                    lerror("Invalid gender.", fmt);
                                    cout << endl;
                                } else {

                                    cout << "ID: " << id << endl;
                                    cout << "Name: " << name << endl;
                                    cout << "Gender: " << gender << endl;

                                    char confirm;

                                    betterCin("Are you sure that you want to add this student into the system? (y/{n | any other key}): ", confirm, Tcolors::RED + "Invalid choice. Please try again." + Tcolors::RESET, false);

                                    if (confirm == 'y') {
                                        Student stdnt(name, id, gender);

                                        students.insert(stdnt);

                                        lsuccess("Successfully inserted student into the list.", "[SUCCESS]: %(msg)s");
                                        cout << endl;
                                    }
                                    else {
                                        linfo("Student addition has been dropped.", fmt);
                                        cout << endl;
                                    }
                                }

                                betterCin("Do you want to add another student? (y/{n | any other key}): ", repeatChoice, Tcolors::RED + "Invalid choice. Please try again." + Tcolors::RESET, false);
                                cout << endl;

                                if (repeatChoice == 'y') {
                                    continue;
                                }
                                else {
                                    break;
                                }
                            }
                        }
                        else if (choiceT == 3) {
                            // View a student's information
                            linfo("You have selected to view a student's information.", fmt);

                            char repeatChoice;
                            while (true) {
                                string id;
                                betterCin("# Please enter the ID of the student: ", id, Tcolors::RED + "Invalid ID. Please try again." + Tcolors::RESET, false);
                                cout << endl;

                                Node<Student>* node = students.search(id);

                                if (node == nullptr) {
                                    lerror("Could not find student with ID " + id, fmt);
                                    cout << endl;
                                } else {
                                    linfo("Student's information:", fmt);
                                    cout << endl;
                                    
                                    node->data.print_data();
                                }

                                betterCin("Do you want to view another student's information? (y/{n | any other key}): ", repeatChoice, Tcolors::RED + "Invalid choice. Please try again." + Tcolors::RESET, false);
                                cout << endl;

                                if (repeatChoice == 'y') {
                                    continue;
                                }
                                else {
                                    break;
                                }
                            }
                        }
                        else if (choiceT == 4) {
                            // View a student's score for a specific term
                            linfo("You have selected to view a student's score for a specific term.", fmt);
                            cout << endl;

                            char repeatChoice;
                            while (true) {
                                string id;
                                betterCin("# Please enter the ID of the student: ", id, Tcolors::RED + "Invalid ID. Please try again." + Tcolors::RESET, false);
                                cout << endl;

                                Node<Student>* node = students.search(id);

                                if (node == nullptr) {
                                    lerror("Could not find student with ID " + id, fmt);
                                    cout << endl;
                                } else {
                                    int term;
                                    betterCin("# Please enter the term you want to view: ", term, Tcolors::RED + "Invalid term. Please try again." + Tcolors::RESET, false);
                                    cout << endl;

                                    vector<float> *scores = node->data.getScores(term);

                                    if (scores == nullptr) {
                                        lerror("Could not find student with ID " + id, fmt);
                                        cout << endl;
                                    } else {
                                        cout << "Scores:\n";
                                        for (unsigned i = 0; i < scores->size(); i++) {
                                            cout << "Score " << i + 1 << ": " << scores->at(i) << endl;
                                        }
                                        cout << endl;
                                    }
                                }

                                betterCin("Do you want to view another student's score? (y/{n | any other key}): ", repeatChoice, Tcolors::RED + "Invalid choice. Please try again." + Tcolors::RESET, false);
                                cout << endl;

                                if (repeatChoice == 'y') {
                                    continue;
                                }
                                else {
                                    break;
                                }
                            }
                        }
                        else if (choiceT == 2) {
                            // Add a student's score for a term
                            linfo("You have selected to add a student's score for a term.", fmt);
                            cout << endl;

                            char repeatChoice;
                            while (true) {
                                bool fail = 0;

                                string id;
                                betterCin("# Please enter the ID of the student: ", id, Tcolors::RED + "Invalid ID. Please try again." + Tcolors::RESET, false);
                                cout << endl;

                                Node<Student>* node = students.search(id);

                                if (node == nullptr) {
                                    lerror("Could not find student with ID " + id, fmt);
                                    cout << endl;
                                    fail = 1;
                                } else {
                                    int term = node->data.term_scores.size() + 1;

                                    int how_many;
                                    betterCin("# Please enter the number of scores you want to add: ", how_many, Tcolors::RED + "Invalid number. Please try again." + Tcolors::RESET, false);
                                    cout << endl;

                                    vector<float> scores;
                                    for (int i = 0; i < how_many; i++) {
                                        float score;
                                        betterCin("# Please enter the score: ", score, Tcolors::RED + "Invalid score. Please try again." + Tcolors::RESET, false);
                                        cout << endl;

                                        if (score < 0 || score > 100) {
                                            lerror("Invalid score.", fmt);
                                            cout << endl;
                                            fail = 1;
                                            break;
                                        }

                                        scores.push_back(score);
                                    }

                                    if (!fail) {
                                        char confirm;
                                        cout << "ID: " << id << endl;
                                        cout << "Term: " << term << endl;
                                        cout << "Scores: ";
                                        for (unsigned i = 0; i < scores.size(); i++) {
                                            cout << scores.at(i) << ", ";
                                        }
                                        cout << endl;

                                        betterCin("Are you sure that you want to add this score? (y/{n | any other key}): ", confirm, Tcolors::RED + "Invalid choice. Please try again." + Tcolors::RESET, false);

                                        if (confirm == 'y') {
                                            node->data.add_term_scores(term, scores);
                                            lsuccess("Successfully added score.", "[SUCCESS]: %(msg)s");
                                            cout << endl;
                                        }
                                        else {
                                            linfo("Score addition has been dropped.", fmt);
                                            cout << endl;
                                        }
                                    }
                                }

                                betterCin("Do you want to add another score? (y/{n | any other key}): ", repeatChoice, Tcolors::RED + "Invalid choice. Please try again." + Tcolors::RESET, false);
                                cout << endl;

                                if (repeatChoice == 'y') {
                                    continue;
                                }
                                else {
                                    break;
                                }
                            }
                        }
                        else if (choiceT == 5) {
                            // Edit a student's score for a term
                            linfo("You have selected to edit a student's score for a term.", fmt);
                            cout << endl;

                            char repeatChoice;
                            while (true) {
                                bool fail = false;
                                string id;
                                betterCin("# Please enter the ID of the student: ", id, Tcolors::RED + "Invalid ID. Please try again." + Tcolors::RESET, false);
                                cout << endl;

                                Node<Student>* node = students.search(id);

                                if (node == nullptr) {
                                    lerror("Could not find student with ID " + id, fmt);
                                    cout << endl;
                                    fail = true;
                                } else {
                                    int term;
                                    betterCin("# Please enter the term you want to edit: ", term, Tcolors::RED + "Invalid term. Please try again." + Tcolors::RESET, false);
                                    cout << endl;

                                    if (term < 1) {
                                        lerror("Invalid term.", fmt);
                                        cout << endl;
                                    }
                                    else {

                                        vector<float> *scores = node->data.getScores(term);

                                        if (scores == nullptr) {
                                            lerror("You have not taken any exams this term.", fmt);
                                            cout << endl;
                                        }
                                        else {
                                            cout << "Term: " << term << endl;
                                            cout << "Scores: ";
                                            for (unsigned i = 0; i < scores->size(); i++) {
                                                cout << "Score " << i + 1 << ": " << scores->at(i) << endl;
                                            }
                                            cout << endl;

                                            char conf;
                                            betterCin("Are you sure that you want to edit this score? (y/{n | any other key}): ", conf, Tcolors::RED + "Invalid choice. Please try again." + Tcolors::RESET, false);

                                            if (conf == 'y') {

                                                int how_many;
                                                betterCin("# Please enter how many scores you want to add: ", how_many, Tcolors::RED + "Invalid number. Please try again." + Tcolors::RESET, false);
                                                cout << endl;

                                                vector<float> scores_new;
                                                for (int i = 0; i < how_many; i++) {
                                                    float score;
                                                    betterCin("# Please enter the score: ", score, Tcolors::RED + "Invalid score. Please try again." + Tcolors::RESET, false);
                                                    cout << endl;

                                                    if (score < 0 || score > 100) {
                                                        lerror("Invalid score.", fmt);
                                                        fail = true;
                                                        cout << endl;
                                                        break;
                                                    } else {
                                                        scores_new.push_back(score);
                                                    }
                                                }

                                                if (!fail) {
                                                    char confirm;
                                                    cout << "ID: " << id << endl;
                                                    cout << "Term: " << term << endl;
                                                    cout << "Scores: ";
                                                    for (unsigned i = 0; i < scores_new.size(); i++) {
                                                        cout << scores_new.at(i) << ", ";
                                                    }
                                                    cout << endl;

                                                    betterCin("Are you sure that you want to add this score? (y/{n | any other key}): ", confirm, Tcolors::RED + "Invalid choice. Please try again." + Tcolors::RESET, false);

                                                    if (confirm == 'y') {
                                                        node->data.edit_term_scores(term, scores_new);
                                                        lsuccess("Successfully edited score.", "[SUCCESS]: %(msg)s");
                                                        cout << endl;
                                                    }
                                                    else {
                                                        linfo("Score editing has been dropped.", fmt);
                                                        cout << endl;
                                                    }
                                                }
                                            }
                                            else {
                                                linfo("Score editing has been dropped.", fmt);
                                                cout << endl;
                                            }
                                        }
                                    }
                                }

                                betterCin("Do you want to edit another score? (y/{n | any other key}): ", repeatChoice, Tcolors::RED + "Invalid choice. Please try again." + Tcolors::RESET, false);
                                cout << endl;

                                if (repeatChoice == 'y') {
                                    continue;
                                }
                                else {
                                    break;
                                }
                            }
                        }
                        else if (choiceT == 6) {
                            // Edit a student's information
                            linfo("You have selected to view a student's information.", fmt);
                            cout << endl;

                            char repeatChoice;
                            while (true) {
                                string id;
                                betterCin("# Please enter the ID of the student: ", id, Tcolors::RED + "Invalid ID. Please try again." + Tcolors::RESET, false);
                                cout << endl;

                                Node<Student>* node = students.search(id);

                                if (node == nullptr) {
                                    lerror("Could not find student with ID " + id, fmt);
                                    cout << endl;
                                }
                                else {
                                    cout << "Info: The information of the student is: " << endl;
                                    cout << "Name: " << node->data.name << endl;
                                    cout << "ID: " << node->data.id << endl;
                                    cout << "Gender: " << node->data.g << endl;
                                    cout << endl;

                                    string newName;
                                    char newGender;

                                    betterCin("# Please enter the new name: ", newName, Tcolors::RED + "Invalid name. Please try again." + Tcolors::RESET, false);
                                    betterCin("# Please enter the new Gender: ", newGender, Tcolors::RED + "Invalid Gender. Please try again." + Tcolors::RESET, false);

                                    if (newGender != 'M' && newGender != 'F') {
                                        lerror("Invalid gender.", fmt);
                                        cout << endl;
                                    }
                                    else {

                                        cout << "New name: " << newName << endl;
                                        cout << "New gender: " << newGender << endl;

                                        betterCin("Are you sure that you want to replace the old information with this one? (y/{n | any other key}: ", repeatChoice, Tcolors::RED + "Invalid choice. Please try again" + Tcolors::RESET, false);

                                        if (repeatChoice == 'y') {
                                            node->data.name = newName;
                                            node->data.g = newGender;
                                            lsuccess("Successfully edited student information.", "[SUCCESS]: %(msg)s");
                                            cout << endl;
                                        }
                                        else {
                                            linfo("Student information editing has been dropped.", fmt);
                                            cout << endl;
                                        }
                                    }
                                }

                                betterCin("Do you want to edit another student's information? (y/{n | any other key}): ", repeatChoice, Tcolors::RED + "Invalid choice. Please try again." + Tcolors::RESET, false);
                                cout << endl;

                                if (repeatChoice == 'y') {
                                    continue;
                                }
                                else {
                                    break;
                                }
                            }
                        }
                        else if (choiceT == 7) {
                            // Remove a student from the system
                            linfo("You have selected to remove a student from the system.", fmt);
                            cout << endl;

                            char repeatChoice;
                            while (true) {

                                string id;
                                betterCin("# Please enter the ID of the student: ", id, Tcolors::RED + "Invalid ID. Please try again." + Tcolors::RESET, false);
                                cout << endl;

                                Node<Student>* node = students.search(id);

                                if (node == nullptr) {
                                    lerror("Could not find student with ID " + id, fmt);
                                    cout << endl;
                                }
                                else {
                                    cout << "Name: " << node->data.name << endl;
                                    cout << "ID: " << node->data.id << endl;
                                    cout << "Gender: " << node->data.g << endl;

                                    char confirm;
                                    betterCin("Are you sure you want to delete this student? (y/{n | any other key}): ", confirm, Tcolors::RED + "Invalid choice. Please try again." + Tcolors::RESET, false);

                                    if (confirm == 'y') {
                                        students.remove(id);
                                        lsuccess("Successfully removed student.", "[SUCCESS]: %(msg)s");
                                        cout << endl;
                                    }
                                    else {
                                        linfo("Student removal has been dropped.", fmt);
                                        cout << endl;
                                    }
                                }

                                betterCin("Do you want to remove another student? (y/{n | any other key}): ", repeatChoice, Tcolors::RED + "Invalid choice. Please try again." + Tcolors::RESET, false);
                                cout << endl;

                                if (repeatChoice == 'y') {
                                    continue;
                                }
                                else {
                                    break;
                                }
                            }
                        }
                        else if (choiceT == 8) {
                            // Show teacher menu again
                            cout << endl;
                            continue;
                        }
                        else if (choiceT == 9) {
                            // Force reload the student list
                            linfo("You have selected to force reload the student list.", fmt);
                            cout << endl;

                            char confirm;
                            betterCin("Are you sure you want to reload the student list? (y/{n | any other key}): ", confirm, Tcolors::RED + "Invalid choice. Please try again." + Tcolors::RESET, false);

                            if (confirm == 'y') {
                                students.clear();
                                students.readFromFile("students.csv");
                                lsuccess("Successfully reloaded student list.", "[SUCCESS]: %(msg)s");
                                cout << endl;
                            }
                            else {
                                linfo("Student list reloading has been dropped.", fmt);
                                cout << endl;
                            }
                        }
                        else if (choiceT == 10) {
                            // Export the current list to a file
                            linfo("You have selected to export the current list to a file.", fmt);
                            cout << endl;

                            char confirm;
                            betterCin("Are you sure you want to export the current list to a file? (y/{n | any other key}): ", confirm, Tcolors::RED + "Invalid choice. Please try again." + Tcolors::RESET, false);

                            if (confirm == 'y') {
                                students.writeToFile("students.csv");
                                lsuccess("Successfully exported student list.", "[SUCCESS]: %(msg)s");
                                cout << endl;
                            }
                            else {
                                linfo("Student list exporting has been dropped.", fmt);
                                cout << endl;
                            }
                        }
                        else if (choiceT == 11) {
                            // Print all students' information
                            linfo("You have selected to print all students' information.", fmt);
                            cout << endl;

                            cout << Tcolors::BLINKPURPLE << "Students list size: " << students.getEffectiveSize() << Tcolors::RESET << endl;
                            students.print();
                            cout << Tcolors::BLINKPURPLE << "Students list size: " << students.getEffectiveSize() << Tcolors::RESET << endl;
                            cout << "\n" << endl;
                        }
                        else if (choiceT == 12) {
                            // Exit the teacher menu
                            linfo("You have selected to exit the teacher menu.", fmt);
                            cout << endl;

                            char choiceSave;
                            betterCin(Tcolors::YELLOW + "Warning: You may have unsaved changes to the list, do you want to save the list? (Y/N): " + Tcolors::RESET, choiceSave, Tcolors::RED + "Invalid choice. Please try again." + Tcolors::RESET, false);
                            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            
                            if (choiceSave == 'Y' || choiceSave == 'y') {
                                students.writeToFile("students.csv");

                                lsuccess("Successfully saved the list.", "[SUCCESS]: %(msg)s");
                                cout << endl;

                                break;
                            }
                            else if (choiceSave == 'N' || choiceSave == 'n') {
                                linfo("You have selected to not save the list.", fmt);
                                cout << endl;

                                break;
                            }
                            else {
                                lerror("Invalid choice.", fmt);
                                cout << endl;
                            }

                        }
                        // TODO: another choice for viewing all students' information sorted by GPA
                        // Reply: Can't do it, would have to implement an entire BST to do it, and I don't want to do that.
                        else {
                            lerror("Invalid choice.", fmt);
                            cout << endl;
                            continue;
                        }
                    }

                    continue;
                }
                else {
                    linfo("You have selected to exit the system.", fmt);
                    cout << endl;

                    continue;
                }
            }
        }
        else if (choice == 3) {
            string username, password, confPassword;

            betterCin("Please enter your username: ", username, Tcolors::RED + "Invalid username. Please try again." + Tcolors::RESET, false);
            betterCin("Please enter your password: ", password, Tcolors::RED + "Invalid password. Please try again." + Tcolors::RESET, true);
            cout << endl;

            betterCin("Please enter your password again: ", confPassword, Tcolors::RED + "Invalid password. Please try again." + Tcolors::RESET, true);
            cout << endl;

            if (password != confPassword) {
                lerror("Passwords do not match.", fmt);
                cout << endl;
                continue;
            }

            bool found = false;

            string line;
            ifstream file("users.txt");

            if (!file.is_open()) {
                lerror("Could not open file.", fmt);
                cout << endl;
                linfo("Creating new file...", fmt);

                ofstream ofile("users.txt");
                ofile.close();

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
                lerror("Username already exists.", fmt);
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
                    lerror("Could not open file.", fmt);
                    cout << endl;

                    linfo("Creating new file...", fmt);

                    ofstream ofile("users.txt");
                    ofile.close();

                    continue;
                }

                ofile << username << ":" << salt << "." << digest << endl;

                ofile.close();

                lblinksuccess("Successfully created student user.", "[SUCCESS]: %(msg)s");
                cout << endl;
                continue;
            }
        }
        else if (choice == 4) {
            // register as teacher
            string username, password, confPassword;

            betterCin("Please enter your username: ", username, Tcolors::RED + "Invalid username. Please try again." + Tcolors::RESET, false);
            betterCin("Please enter your password: ", password, Tcolors::RED + "Invalid password. Please try again." + Tcolors::RESET, true);
            cout << endl;

            betterCin("Please enter your password again: ", confPassword, Tcolors::RED + "Invalid password. Please try again." + Tcolors::RESET, true);
            cout << endl;

            if (password != confPassword) {
                lerror("Passwords do not match.", fmt);
                cout << endl;
                continue;
            }

            bool found = false;

            string line;
            ifstream file("users.txt");

            if (!file.is_open()) {
                lerror("Could not open file.", fmt);
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
                lerror("Username already exists.", fmt);
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
                    lerror("Could not open file.", fmt);
                    cout << endl;
                    continue;
                }

                ofile << username << ":" << salt << "." << digest << endl;

                ofile.close();

                lblinksuccess("Successfully created teacher user.", "[SUCCESS]: %(msg)s");
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
            cout << Tcolors::GREEN << "Goodbye!" << Tcolors::RESET << endl;
            cout << endl;
            continue;
        }
        else {
            lerror("Invalid choice.", fmt);
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
    cout << Tcolors::YELLOW << "1. " << Tcolors::RESET << "View basic information" << endl;
    cout << Tcolors::YELLOW << "2. " << Tcolors::RESET << "View score for a specific term" << endl;
    cout << Tcolors::YELLOW << "3. " << Tcolors::RESET << "Show the student menu again" << endl;
    cout << Tcolors::YELLOW << "4. " << Tcolors::RESET << "Exit" << endl;
    cout << endl;
}

void print_teacher_menu() {
    cout << Tcolors::YELLOW << "1. " << Tcolors::RESET << "Add a student into the system" << endl;
    cout << Tcolors::YELLOW << "2. " << Tcolors::RESET << "Add a student's score for a term" << endl;
    cout << Tcolors::YELLOW << "3. " << Tcolors::RESET << "View a student information" << endl;
    cout << Tcolors::YELLOW << "4. " << Tcolors::RESET << "View a student's score for a specific term" << endl;
    cout << Tcolors::YELLOW << "5. " << Tcolors::RESET << "Edit a student's score for a term" << endl;
    cout << Tcolors::YELLOW << "6. " << Tcolors::RESET << "Edit a student's information" << endl;
    cout << Tcolors::YELLOW << "7. " << Tcolors::RESET << "Remove a student from the system" << endl;
    cout << Tcolors::YELLOW << "8. " << Tcolors::RESET << "Show the teacher menu again" << endl;
    cout << Tcolors::YELLOW << "9. " << Tcolors::RESET << "Import student information from a file" << endl;
    cout << Tcolors::YELLOW << "10. " << Tcolors::RESET << "Export student information to a file" << endl;
    cout << Tcolors::YELLOW << "11. " << Tcolors::RESET << "Show all students' information" << endl;
    cout << Tcolors::YELLOW << "12. " << Tcolors::RESET << "Exit" << endl;
    cout << endl;
}

void print_main_menu() {
    cout << Tcolors::BOLDCYAN + "Please select an option:" + Tcolors::RESET<< endl;
    cout << Tcolors::YELLOW + "1. " << Tcolors::RESET << "Login as a student" << endl;
    cout << Tcolors::YELLOW + "2. " << Tcolors::RESET << "Login as a teacher" << endl;
    cout << Tcolors::YELLOW + "3. " << Tcolors::RESET << "Register as a student" << endl;
    cout << Tcolors::YELLOW + "4. " << Tcolors::RESET << "Register as a teacher" << endl;
    cout << Tcolors::YELLOW + "5. " << Tcolors::RESET << "Show help again" << endl;
    cout << Tcolors::YELLOW + "6. " << Tcolors::RESET << "Exit" << endl;
    cout << endl;
}