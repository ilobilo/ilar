CPP := clang++

SRCDIR := src
TARGET := bin/ilar

CPPFILES := $(shell find $(SRCDIR) -type f -name *.cpp)
OBJECTS := $(CPPFILES:.cpp=.o)
CPPFLAGS := -I$(SRCDIR) -std=gnu++20 -Wall -Werror -Wextra

$(TARGET): $(OBJECTS)
	$(CPP) $^ -o $(TARGET)
	rm -rf $(OBJECTS)

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -rf $(OBJECTS)

.PHONY: distclean
distclean:
	rm -rf $(TARGET) $(OBJECTS)