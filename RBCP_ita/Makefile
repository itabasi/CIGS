CC=gcc
CXX=g++
CFLAGS  = -O2
BINDIR = ./bin
LIBDIR = ./lib
ROOTFLAGS = $(shell root-config --cflags)
ROOTLIBS = $(shell root-config --libs) -lMinuit
ROOTGLIBS = $(shell root-config --glibs)
CXXFLAGS = -Wall -O2 $(ROOTFLAGS)
CXXLIBS = $(ROOTLIBS)

TARGET1=     rbcp
OBJS1=       rbcp.o 

.PHONY: all
all: $(TARGET1)

$(LIBDIR)/%.o : %.c
	$(CXX) $(CFLAGS) -c -o $@ $< -MMD $(CXXFLAGS)
$(TARGET1): $(patsubst %,$(LIBDIR)/%,$(OBJS1))
	$(CXX) $(CFLAGS) -o $(BINDIR)/$@ $^ $(CXXLIBS) $(CXXFLAGS)
.PHONY: clean
clean:
	rm -f $(LIBDIR)/*.o $(LIBDIR)/*.d core $(BINDIR)/*
