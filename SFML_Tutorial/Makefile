# =====================
# Directories
# =====================
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include
LIB_DIR = lib

DEBUG_DIR = bin/debug
RELEASE_DIR = bin/release

# =====================
# Compiler
# =====================
CXX = g++
CXXFLAGS = -std=c++17 -Wall -MMD -MP
USE_INC = -I$(INCLUDE_DIR)
USE_LIB = -L$(LIB_DIR)
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# =====================
# File Names
# =====================
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC))
DEP = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.d,$(SRC))
RELEASE = $(RELEASE_DIR)/game.exe
DEBUG = $(DEBUG_DIR)/game_debug.exe

# =====================
# Make rules
# =====================
all: $(RELEASE)

release: $(RELEASE)

debug: $(DEBUG)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(USE_INC) -c $< -o $@

-include $(DEP)

$(RELEASE): $(OBJ)
	@mkdir -p $(RELEASE_DIR)
	$(CXX) $(CXXFLAGS) -O2 -DNDEBUG $^ -o $@ $(USE_LIB) $(LIBS)

$(DEBUG): $(OBJ)
	@mkdir -p $(DEBUG_DIR)
	$(CXX) $(CXXFLAGS) -g $^ -o $@ $(USE_LIB) $(LIBS)

clean:
	rm -r $(BUILD_DIR) $(DEBUG_DIR) $(RELEASE_DIR)