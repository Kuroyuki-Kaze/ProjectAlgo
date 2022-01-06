IDIR=./modules
SDIR=./src
ODIR=./obj
BDIR=./bin
CPPC=g++
CPPFLAGS=-Wall -Wextra -std=c++17

output: $(ODIR)/main.o $(ODIR)/secrets.o
	$(CPPC) $(CPPFLAGS) $(ODIR)/main.o $(ODIR)/secrets.o -o $(BDIR)/main 

$(ODIR)/main.o: $(SDIR)/main.cpp
	$(CPPC) $(CPPFLAGS) -c $(SDIR)/main.cpp -o $(ODIR)/main.o

$(ODIR)/secrets.o: $(IDIR)/secrets.cpp $(IDIR)/secrets.hpp
	$(CPPC) $(CPPFLAGS) -c $(IDIR)/secrets.cpp -o $(ODIR)/secrets.o

.PHONY: clean, prepare

clean:
	del /s *.o main.exe


prepare:
	-mkdir bin
	-mkdir obj
