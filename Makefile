V = @

CXX = /opt/local/bin/clang++-mp-3.2
CXXFLAGS += \
	-std=c++11 \
	-isystem /opt/local/include/ \
	-L/opt/local/lib/ \
	-lcppunit \
	-Wall -Wextra \
	-O0 \
	-g

TESTS_BIN = ./test-bin
TESTS_SRCS = ./test.cpp $(wildcard *.hpp)

all: test

test: $(TESTS_BIN)
	$(V)echo "Running tests..."
	$(V)$(TESTS_BIN)

$(TESTS_BIN): $(TESTS_SRCS)
	$(V)echo "Building tests..."
	$(V)$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: all test
