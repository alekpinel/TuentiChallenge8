# Mention default target.
all:

OBJ=obj
INC=include

# Implicit rule to compile C++ files.  Modify to your taste.
$(OBJ)/%.o: %.cpp
	g++ -c -o $@ -I$(INC) -O2 -Wall -Wextra -pedantic $<

$(OBJ)/%.o: $(INC)/%.cc
	g++ -c -o $@ -I$(INC) -O2 -Wall -Wextra -pedantic $<

# Components of the library.
library-objects = \
	$(OBJ)/BigUnsigned.o \
	$(OBJ)/BigInteger.o \
	$(OBJ)/BigIntegerAlgorithms.o \
	$(OBJ)/BigUnsignedInABase.o \
	$(OBJ)/BigIntegerUtils.o \

library-headers = \
	$(INC)/NumberlikeArray.hh \
	$(INC)/BigUnsigned.hh \
	$(INC)/BigInteger.hh \
	$(INC)/BigIntegerAlgorithms.hh \
	$(INC)/BigUnsignedInABase.hh \
	$(INC)/BigIntegerLibrary.hh \

# To ``make the library'', make all its objects using the implicit rule.
library: $(library-objects)

# Conservatively assume that all the objects depend on all the headers.
$(library-objects): $(library-headers)

# The rules below build a program that uses the library.  They are preset to
# build ``sample'' from ``sample.cc''.  You can change the name(s) of the
# source file(s) and program file to build your own program, or you can write
# your own Makefile.

# Components of the program.
program = Challenge10
program-objects = $(OBJ)/$(program).o

# Conservatively assume all the program source files depend on all the library
# headers.  You can change this if it is not the case.
$(program-objects) : $(library-headers)

# How to link the program.  The implicit rule covers individual objects.
$(program) : $(program-objects) $(library-objects)
	g++ $^ -o $@

# Delete all generated files we know about.
clean :
	rm -f $(library-objects) $(program-objects) $(program)

# I removed the *.tag dependency tracking system because it had few advantages
# over manually entering all the dependencies.  If there were a portable,
# reliable dependency tracking system, I'd use it, but I know of no such;
# cons and depcomp are almost good enough.

# Come back and define default target.
all : library $(program)
