# ZACLib/Makefile
CXX := g++
CXXFLAGS := -I./ \
-std=c++11 # or higher

SRC := ZACLib.cpp
OBJ := $(SRC:.cpp=.o)
TARGET := libZACLib.a

all: $(TARGET)

$(TARGET): $(OBJ)
	ar rcs $@ $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
