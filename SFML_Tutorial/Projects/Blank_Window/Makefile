# =====================
# Directories
# =====================
SRC_DIR = src
BUILD_DIR = build
MY_INCLUDE_DIR = include
SFML_INCLUDE_DIR = ../../SFML/include
SFML_LIB_DIR = ../../SFML/lib

DEBUG_DIR = bin/debug
RELEASE_DIR = bin/release

# =====================
# Compiler
# =====================
CXX = g++
CXXFLAGS = -std=c++17 -Wall -MMD -MP
INCFLAGS = -I$(SFML_INCLUDE_DIR) -I$(MY_INCLUDE_DIR)
LIBFLAGS = -L$(SFML_LIB_DIR) -lsfml-graphics -lsfml-window -lsfml-system

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
all: release

release: $(RELEASE)

debug: $(DEBUG)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCFLAGS) -c $< -o $@

-include $(DEP)

$(RELEASE): $(OBJ)
	@mkdir -p $(RELEASE_DIR)
	$(CXX) $(CXXFLAGS) -O2 -DNDEBUG $^ -o $@ $(LIBFLAGS)

$(DEBUG): $(OBJ)
	@mkdir -p $(DEBUG_DIR)
	$(CXX) $(CXXFLAGS) -g $^ -o $@ $(LIBFLAGS)

clean:
	rm -rf $(BUILD_DIR) $(RELEASE) $(DEBUG)
