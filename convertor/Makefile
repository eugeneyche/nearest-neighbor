CC := g++
SRCDIR := src
BUILDDIR := build
TARGET := bin/convertor
 
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -std=c++11
LIB := -L lib
INC := -I include

ifdef DEBUG
CFLAGS += -g -DDEBUG=$(DEBUG)
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
	if [ ! -d bin ]; then mkdir bin; fi
	if [ ! -d data ]; then mkdir data; fi
	make -C data/mnist directories
	make -C data/cifar10 directories

mnist: $(TARGET)
	make -C data/mnist
	$(TARGET) mnist

cifar10: $(TARGET)
	make -C data/cifar10
	$(TARGET) cifar10
	

.PHONY: 
	clean
