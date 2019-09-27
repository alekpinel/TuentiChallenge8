PP=Challenge1


all: $(BIN)/$(PP)

clean:
	rm *.o
	rm $(PP)

$(OBJ)/$(PP).o: $(PP).cpp
	g++ -c -o $(PP).o $(PP).cpp

$(BIN)/$(PP): $(PP).o 
	g++ -o $(PP) $(PP).o
	rm *.o
	
