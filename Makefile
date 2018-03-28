
LIB := libmpc-utils.a
SOURCES := $(shell find . -type f -name '*.cpp')
OBJECTS := $(patsubst %.cpp, %.o, $(SOURCES))

$(LIB): $(OBJECTS)
	ar rcs $@ $^

# extract dependencies
%.d: %.cpp $(LIBRARIES)
	@set -e; rm -f $@; \
	$(CXX) -MM $(CXXFLAGS) -MT "$*.o $@" $< > $@;

-include $(SOURCES:.cpp=.d)

.PHONY: clean
clean:
	$(RM) $(OBJECTS) $(OBJECTS:.o=.d) $(LIB)
