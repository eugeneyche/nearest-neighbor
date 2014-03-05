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
CFLAGS += -g -DDEBUG=VERBOSE
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
	mkdir data
	mkdir data/mnist
	mkdir data/mnist/in
	mkdir data/mnist/out

mnist: $(TARGET)
	make -C data/mnist
	$(TARGET) mnist

cifar10: $(TARGET)
	make -C data/cifar10
	$(TARGET) cifar10
	

.PHONY: 
	clean
