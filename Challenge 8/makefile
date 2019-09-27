PP=Challenge8
AUX=BigInteger

OBJ=obj
INC=include


all: $(BIN)/$(PP)

clean:
	rm $(OBJ)/*.o
	rm $(PP)

$(OBJ):
	mkdir -p $@

$(OBJ)/%.o: %.cpp
	g++ -c -I$(INC) -o $@ $<

$(OBJ)/%.o: $(INC)/%.cpp
	g++ -c -I$(INC) -o $@ $<

$(BIN)/$(PP): $(OBJ) $(OBJ)/$(PP).o $(OBJ)/$(AUX).o
	g++ -std=c++0x -I$(INC) $(OBJ)/$(PP).o $(OBJ)/$(AUX).o -o $(PP)
