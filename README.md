Algorithm Project: Student Management System
============================================

- [Algorithm Project: Student Management System](#algorithm-project-student-management-system)
  - [Description](#description)
  - [How to build the program](#how-to-build-the-program)
  - [After building the program](#after-building-the-program)

Description
-----------
This project is a student management system created in C++ that
allows the user to add, view, modify, and delete the students in the system.

How to build the program
------------------------
To build the program/project, you need to have `make` or `mingw32-make` installed.

`make` comes with the GNU C toolchain by default on Linux and other operating systems.
`mingw32-make` comes with the MinGW toolchain by default.

To install `make` on Windows, install [`Chocolatey`](https://chocolatey.org/) first, and run the following command as administrator:
```
choco install make
```

or you can install `mingw32-make` by getting MinGW toolchain from [MSYS2](https://www.msys2.org/).

After you install `make` or `mingw32-make`, you can run the following command in the root directory of the project to build the program:
```
make
```

If you prefer to not use `make` or `mingw32-make` to build the program, you can manually execute the following command to build it:
```
g++ -std=gnu++17 src/main.cpp modules/secrets.cpp -o bin/main.exe -I"./modules" -lstdc++
```

This will build the program and create a `bin` directory with the executable file `main.exe` in it. That is the executable program of the project.

After building the program
--------------------------

After you build the program, you can run the executable file inside of `bin`.

**IMPORTANT**: The current working directory `cwd` should be `<path>/bin` in order to ensure that files are created and read in the correct place.