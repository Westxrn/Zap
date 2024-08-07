###
### Makefile for Zap Project
### Zap is a programming that takes in a file and compresses/decompresses it
### Author:  Weston Starbird


##
## Here is a special rule that removes all .o files besides the provided ones 
## (ZapUtil.o, HuffmanTreeNode.o, and BinaryIO.o), all temporary files 
## (ending with ~), and a.out produced by running unit_test. First, we find
## all .o files that are not provided files, then we run rm -f on each found
## file using the -exec find flag. Second, we delete the temporary files
## and a.out. @ is used to suppress stdout.
## 
## You do not need to modify or further comment this rule!
##

MAKEFLAGS += -L

CXX = clang++
CXXFLAGS = -g3 -Wall -Wextra -Wpedantic -Wshadow -std=c++14
LDFLAGS = -g3
# all .o objects included below 
# remember huffmantreenode.o and binaryio.o and zaputil.o givern already 


# This target links all object files into the final 'zap' executable.
zap: HuffmanCoder.o main.o ZapUtil.o HuffmanTreeNode.o BinaryIO.o 
	${CXX} $(LDFLAGS) -o $@ $^

# Compiles the minpq_example object file, dependent on HuffmanTreeNode.
minpq_example.o: minpq_example.cpp HuffmanTreeNode.h 
	$(CXX) $(CXXFLAGS) -c $<

# Compiles the ZapUtilExample object file, dependent on ZapUtil and 
# HuffmanTreeNode.
ZapUtilExample.o: ZapUtilExample.cpp ZapUtil.h HuffmanTreeNode.h
	$(CXX) $(CXXFLAGS) -c $<

# Compiles the HuffmanCoder object file, with dependencies on HuffmanCoder, 
# HuffmanTreeNode, and BinaryIO headers.
HuffmanCoder.o: HuffmanCoder.cpp HuffmanCoder.h HuffmanTreeNode.h BinaryIO.h
	$(CXX) $(CXXFLAGS) -c $<

# Compiles the main object file, dependent on the HuffmanCoder header.
main.o: main.cpp HuffmanCoder.h
	$(CXX) $(CXXFLAGS) -c $<

# Links the unit test driver with all necessary object files to create a 
# 'unit_test' executable.
unit_test: unit_test_driver.o phaseOne.o ZapUtil.o HuffmanTreeNode.o \
BinaryIO.o HuffmanCoder.o
	${CXX} $(LDFLAGS) -o $@ $^


# This target compiles and links the phaseOne executable, 
# dependent on several object files.
phase_one: phaseOne.o ZapUtil.o HuffmanTreeNode.o BinaryIO.o
	${CXX} $(LDFLAGS) -o $@ $^

# Compiles the phaseOne object file, with dependencies on phaseOne 
# and HuffmanTreeNode headers.
phaseOne.o: phaseOne.cpp phaseOne.h HuffmanTreeNode.h
	$(CXX) $(CXXFLAGS) -c $<

# The clean target removes object files and executables to clean the directory.
# Note: It preserves specific object files not recompiled frequently.
clean:
	@find . -type f \( \
		-name '*.o' ! -name 'HuffmanTreeNode.o' ! -name 'BinaryIO.o' \
		! -name 'ZapUtil.o' \) -exec rm -f {} \;
	@rm -f *~ a.out

# Marks 'clean' as a phony target to ensure it runs regardless of any 
# files named "clean."
.PHONY: clean

