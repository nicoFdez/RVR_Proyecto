SRC_FILES = $(wildcard *.cpp)
OBJ_FILES = $(patsubst %.cpp,%.o,$(SRC_FILES))
LDFLAGS   = -lpthread -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf
CPPFLAGS  = -g -I /usr/include/SDL2 -D_REENTRANT

main: $(OBJ_FILES)
	g++ -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean

clean: