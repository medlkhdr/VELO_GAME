CXX       := g++
CXXFLAGS  := -std=c++17 -g -O0 -I/ucrt64/include -Iinclude
LDFLAGS   := -L/ucrt64/lib -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system

SRC_DIR   := src
BIN_DIR   := bin
SRC       := $(wildcard $(SRC_DIR)/*.cpp)
OBJ       := $(patsubst $(SRC_DIR)/%.cpp,$(SRC_DIR)/%.o,$(SRC))

TARGET    := $(BIN_DIR)/main

.PHONY: all clean

all: $(BIN_DIR) $(TARGET)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(TARGET): $(OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


make re: fclean all 
clean:
	rm -f $(SRC_DIR)/*.o $(TARGET)
fclean : clean 
	rm -rf