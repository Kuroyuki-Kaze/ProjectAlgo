IDIR=./modules
SDIR=./src
ODIR=./obj
BDIR=./bin
CPPC=g++
CPPFLAGS=-Wall -Wextra -std=c++17

if [ ! -d $ODIR ]; then
	mkdir $ODIR
fi

if [ ! -d $BDIR ]; then
	mkdir $BDIR
fi

output: $(ODIR)/main.o $(ODIR)/secrets.o
	$(CPPC) $(CPPFLAGS) $(ODIR)/main.o $(ODIR)/secrets.o -o $(BDIR)/main 

$(ODIR)/main.o: $(SDIR)/main.cpp
	$(CPPC) $(CPPFLAGS) -c $(SDIR)/main.cpp -o $(ODIR)/main.o

$(ODIR)/secrets.o: $(IDIR)/secrets.cpp $(IDIR)/secrets.hpp
	$(CPPC) $(CPPFLAGS) -c $(IDIR)/secrets.cpp -o $(ODIR)/secrets.o

.PHONY: clean

clean:

ifeq ($(OS), Windows_NT)
	del /s *.o main.exe
endif