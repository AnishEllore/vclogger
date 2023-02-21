CXX       := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb

BIN     := bin
SRC     := src
INCLUDE := include
INTERFACE := include/interface
SINK := include/sinks
LOGGERS := include/loggers
CONFIG  := include/configs
LIB     := lib
LIBRARIES   := 
EXECUTABLE  := main

all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	@echo "🚀 Executing..."
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	@echo "🚧 Building..."
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -I$(INTERFACE) -I$(SINK) -I$(LOGGERS) -I$(CONFIG) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	@echo "🧹 Clearing..."
	-rm -rf $(BIN)/*