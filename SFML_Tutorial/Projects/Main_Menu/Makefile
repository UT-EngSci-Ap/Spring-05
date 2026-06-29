# =====================
# Directories
# =====================
SRC_DIR = src
BUILD_DIR = build
MY_INCLUDE_DIR = include

DEBUG_DIR = bin/debug
RELEASE_DIR = bin/release

# =====================
# SFML (via pkg-config)
# =====================
# Works with Homebrew SFML on macOS and system SFML on Linux.
# Requires: brew install sfml  (macOS)  /  apt install libsfml-dev (Linux)
SFML_MODULES = sfml-graphics sfml-window sfml-system
SFML_CFLAGS := $(shell pkg-config --cflags $(SFML_MODULES))
SFML_LIBS   := $(shell pkg-config --libs $(SFML_MODULES))

# =====================
# Compiler
# =====================
CXX = c++
CXXFLAGS = -std=c++17 -Wall -MMD -MP
INCFLAGS = $(SFML_CFLAGS) -I$(MY_INCLUDE_DIR)
LIBFLAGS = $(SFML_LIBS)

# =====================
# File Names
# =====================
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC))
DEP = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.d,$(SRC))
RELEASE = $(RELEASE_DIR)/game
DEBUG = $(DEBUG_DIR)/game_debug

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

run: release
	./$(RELEASE)

clean:
	rm -rf $(BUILD_DIR) $(RELEASE) $(DEBUG)

.PHONY: all release debug run clean
