TARGET_NAME = phylan
CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++14 -O2

LDFLAGS = -L../mikayuu/build/ `pkg-config --libs bullet`
ifeq ($(OS), Windows_NT)
	LIBS = -lmikayuu -lglfw3 -lopengl32 -lglu32
else
	LIBS = -lmikayuu -lglfw3 -lgl -lGLU
endif

INCLUDE = -I./include `pkg-config --cflags bullet`

SRCDIR = ./src
SRC = $(wildcard $(SRCDIR)/*.cpp)

BUILD_DIR = ./build
TARGET = $(BUILD_DIR)/$(TARGET_NAME)

OBJ = $(addprefix $(BUILD_DIR)/obj/, $(notdir $(SRC:.cpp=.o)))
DEPEND = $(OBJ:.o=.d)


$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)


$(BUILD_DIR)/obj/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<


.PHONY: clean
clean:
	-rm -f $(OBJ) $(DEPEND) $(TARGET)

run: $(TARGET)
	$(TARGET)

