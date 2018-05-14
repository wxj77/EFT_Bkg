CC=g++
CFLAGS=-Wall -g -O3 -I`root-config --incdir` 
LDFLAGS=`root-config --ldflags --libs`

BIN1 = ReadtestNESTToROOT
BIN2 = ReadROOTToTH2F

all: ${BIN1} ${BIN2}

${BIN1}:
	$(CC) $(CFLAGS) ${BIN1}.cpp -o ${BIN1} $(LDFLAGS)

${BIN2}:
	$(CC) $(CFLAGS) ${BIN2}.cpp -o ${BIN2} $(LDFLAGS)

clean:
	rm -f ${BIN1} 
	rm -f ${BIN2}
