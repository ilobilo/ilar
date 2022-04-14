SOURCEDIR := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
BINDIR := $(SOURCEDIR)/../bin/

CPP := clang++
TARGET = $(BINDIR)/ilar

CPPFILES := $(shell find $(SOURCEDIR)/ -type f -name '*.cpp')
OBJECTS := $(CPPFILES:.cpp=.o)

INCLUDE = -I$(SOURCEDIR)
LINK = -lboost_iostreams
CPPFLAGS = $(INCLUDE) -std=gnu++20 -Wall -Werror -Wextra

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CPP) $(LINK) $^ -o $(TARGET)
	rm -rf $(OBJECTS)

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -rf $(OBJECTS)

.PHONY: distclean
distclean:
	rm -rf $(TARGET) $(OBJECTS)