CXX = g++
CXXFLAGS = -O2 -Wall -Werror -Wextra  -std=c++17 -Iinclude
LDFLAGS =

EXE = huffman_archiver
TEST = huffman_test
SRCDIR = src
OBJDIR = obj
TESTSRC = test/test.cpp src/huffman_tree.cpp src/encoding.cpp

OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(wildcard $(SRCDIR)/*.cpp))

all: $(EXE)

test: $(TEST)

$(EXE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXE) $(LDFLAGS)

$(TEST):
	$(CXX) $(CXXFLAGS) $(TESTSRC) -o $(TEST)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c -MMD -o $@ $<

include $(wildcard $(OBJDIR)/*.d)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(EXE) $(TEST)

.PHONY: clean all