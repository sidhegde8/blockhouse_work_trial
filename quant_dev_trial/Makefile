CXX = g++
CXXFLAGS = -O3 -std=c++17 -Wall

TARGET = reconstruction
TEST_TARGET = test

SRCS = main.cpp orderbook.cpp
OBJS = $(SRCS:.cpp=.o)

TEST_SRCS = test_orderbook.cpp orderbook.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

test: $(TEST_SRCS)
	$(CXX) -std=c++17 -o $(TEST_TARGET) $(TEST_SRCS)

clean:
	del /Q reconstruction.exe test.exe *.o output.csv 2>nul

