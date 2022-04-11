CPP := clang++

SRCDIR := src
BUILDDIR := build
TARGET := bin/ilfs

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CPPFLAGS := -std=gnu++20 -Wall -Werror -Wextra

INC := -I$(SRCDIR)

$(TARGET): $(OBJECTS)
	$(CPP) $^ -o $(TARGET)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	mkdir -p $(BUILDDIR)
	$(CPP) $(CPPFLAGS) $(INC) -c -o $@ $<

clean:
	$(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: clean