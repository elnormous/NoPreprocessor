debug=0
ifeq ($(OS),Windows_NT)
platform=0
else
ifeq ($(shell uname -s),Darwin)
platform=1
endif
ifeq ($(shell uname -s),Linux)
platform=2
endif
endif

CXXFLAGS=-std=c++11 -Wall -O2 -DPLATFORM=$(platform)
LDFLAGS=-O2
SOURCES=main.cpp
BASE_NAMES=$(basename $(SOURCES))
OBJECTS=$(BASE_NAMES:=.o)
EXECUTABLE=noprep

all: $(EXECUTABLE)
ifeq ($(debug),1)
all: CXXFLAGS+=-DDEBUG -g
endif

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

.PHONY: clean
clean:
ifeq ($(platform),windows)
	-del /f /q "$(EXECUTABLE).exe" "*.o"
else
	$(RM) $(EXECUTABLE) *.o $(EXECUTABLE).exe
endif