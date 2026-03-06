# ZACLib/Makefile
CXX := g++
CXXFLAGS := -I./ \
-std=c++11 # or higher

SRC := ZACLib.cpp
OBJ := $(SRC:.cpp=.o)
TARGET := ZACLib

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)