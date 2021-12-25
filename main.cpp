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
    float mid_score;
    float final_score;
    // etc...
};

template <class T>
struct Node {
    T data;
    Node<T>* next;
    Node<T>* prev;
};

template <class T>
struct List {
    Node<T>* head;
    Node<T>* tail;
    int size;
};

template <class T>
struct HashTable {
    List<T>* table;
    int size;
};

template <class T>
void betterCin(string, T&, string, bool);

void disableEcho();
void enableEcho();
unsigned c_rand();

int main() {
    srand(time(NULL));

    unsigned choice;

    cout << Tcolors::BOLDGREEN << "Welcome to the student management system!" << Tcolors::RESET << endl;
    cout << "Please select an option:" << endl;
    cout << "1. Login as a student" << endl;
    cout << "2. Login as a teacher" << endl;
    cout << "3. Register as a student" << endl;
    cout << "4. Register as a teacher" << endl;
    cout << "5. Show help again" << endl;
    cout << "6. Exit" << endl;
    cout << endl;

    while (choice != 6) {
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
                    continue;
                }
            }
        }
        else if (choice == 2) {
            // login as teacher
            // Teacher features
            // createa, read , update, delete
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
    unsigned r = (unsigned)rand() % 127;

    if (!(r >= 48 && r <= 57) && !(r >= 65 && r <= 90) && !(r >= 97 && r <= 122)) {
        r = c_rand();
    }

    return r;
}
#endif