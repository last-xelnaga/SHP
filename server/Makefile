OBJDIR = ./obj
TARGET = gtest_server

CFLAGS = -Wall -O2 -fno-omit-frame-pointer -std=c++11
INC	= -I. -I../src/external/gtest/include
LIB	= -L../src/external/gtest -lgtest -lpthread

SRC = \
    core_class.cpp \
    tests/gtest_core_class.cpp

OBJ = $(SRC:%.cpp=$(OBJDIR)/%.o)

.PHONY: all clean test

all: $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)/tests

$(OBJDIR)/%.o: %.cpp
	g++ $(CFLAGS) $(INC) -c -o $@ $<

$(TARGET): $(OBJDIR) $(OBJ)
	g++ -o $@ $(OBJ) $(LIB)

clean:
	rm -rf $(OBJDIR) $(TARGET)

$(OBJDIR)/core_class.o: core_class.hpp Makefile
$(OBJDIR)/tests/gtest_core_class.o: core_class.hpp Makefile
