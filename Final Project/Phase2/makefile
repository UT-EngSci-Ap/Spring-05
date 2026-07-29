CXX = g++

# Homebrew SFML 3 location on Apple Silicon. Override on other systems, e.g.:
#   make SFML_PREFIX=/usr/local/opt/sfml
SFML_PREFIX ?= /opt/homebrew/opt/sfml

CXXFLAGS = -std=c++17 -Wall -Iinclude -I$(SFML_PREFIX)/include -MMD -MP
LDFLAGS = -L$(SFML_PREFIX)/lib -Wl,-rpath,$(SFML_PREFIX)/lib \
          -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

SRC_DIRS = $(wildcard src/*/)
SRC = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))

OBJDIR = build/obj
DEPDIR = build/dep

OBJ = $(SRC:src/%.cpp=$(OBJDIR)/%.o)
DEP = $(SRC:src/%.cpp=$(DEPDIR)/%.d)

TARGET = doodle_jump

$(TARGET): $(OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(DEPDIR)/$*.d)
	$(CXX) $(CXXFLAGS) -c $< -o $@ -MF $(DEPDIR)/$*.d

-include $(DEP)

clean:
	rm -f $(OBJ) $(DEP) doodle_jump

.PHONY: clean
