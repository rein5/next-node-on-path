CXX = g++
CXXFLAGS = -std=c++11
TARGET = main
SRCS = main.cpp RMQ.cpp LCA.cpp NextNodeOnPath.cpp TestUtils.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean