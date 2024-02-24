OUT := out/
SRC := src/
FILES := $(addprefix $(OUT),LogicStr.o main.o)

all: $(FILES)
	g++ -g -Wall $(FILES) -o main.exe 

$(OUT)%.o: $(SRC)%.cpp
	g++ -g -c $< -o $@

clean:
	rm -f $(OUT)*.o $(OUT)*.exe