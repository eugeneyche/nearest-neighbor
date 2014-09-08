CC := g++
SRCDIR := src
BUILDDIR := build
TARGET := bin/main
 
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -std=c++0x -pthread
LIB := -L lib
INC := -I include

ifdef DEBUG
CFLAGS += -g -DDEBUG=$(DEBUG)
endif
ifdef LABEL_T
CFLAGS += -DLABEL_T=$(LABEL_T)
endif
ifdef DATA_T
CFLAGS += -DDATA_T=$(DATA_T)
endif

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	$(RM) -r $(BUILDDIR) $(TARGET)

new: clean $(TARGET)

directories:
	mkdir bin
	mkdir output

.PHONY: 
	clean

run-background:
	screen -dmSL eharmony bin/main
