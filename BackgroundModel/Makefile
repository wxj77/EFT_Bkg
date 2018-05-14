#OS := $(shell uname)
#ifeq ($(OS),Darwin)
# CC=clang++
#else
# CC=g++
#endif

CC=g++
ifeq ($(NESTPATH),)
        NESTPATH=/cvmfs/lz.opensciencegrid.org/fastNEST/release-2.10.0/libNEST/
endif
ifeq ($(NESTDATA),)
        NESTDATA=/cvmfs/lz.opensciencegrid.org/fastNEST/release-2.10.0/LCEAndEFieldMaps/
endif

CFLAGS=-Wall -g -O3 -I`root-config --incdir` -I$(NESTPATH)
LDFLAGS=`root-config --ldflags --libs`

NESTCXX=$(NESTPATH)NEST.cxx

all:
	$(CC) $(CFLAGS) generateFastBkg.cc $(NESTCXX) -o generateFastBkg $(LDFLAGS)

clean:
	rm -f generateFastBkg
