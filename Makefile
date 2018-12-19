
LIB := libmpc-utils.a
SOURCES := $(shell find . -type f -name '*.cpp')
OBJECTS := $(patsubst %.cpp, %.o, $(SOURCES))
TEST_SOURCES := $(shell find . -type f -name '*_test.cpp')
TEST_OBJECTS := $(patsubst %.cpp, %.o, $(TEST_SOURCES))
TESTS := $(basename $(TEST_OBJECTS))

CXXFLAGS += -I/usr/include/eigen3

$(LIB): $(OBJECTS)
	ar rcs $@ $^

$(TESTS): $(TEST_OBJECTS) $(LIB)
	$(CC) -o $@ $^ -lgtest -lgtest_main -lstdc++ -lboost_serialization -lntl -lgmp

.PHONY: test
test: $(TESTS) $(LIB)
	@set -e; for test_binary in $(TESTS); do \
		./$${test_binary}; \
	done

# extract dependencies
%.d: %.cpp $(LIBRARIES)
	@set -e; rm -f $@; \
	$(CXX) -MM $(CXXFLAGS) -MT "$*.o $@" $< > $@;

-include $(SOURCES:.cpp=.d)

.PHONY: clean
clean:
	$(RM) $(OBJECTS) $(OBJECTS:.o=.d) $(LIB) $(TEST_OBJECTS) \
	$(TEST_OBJECTS:.o=.d) $(TESTS)
