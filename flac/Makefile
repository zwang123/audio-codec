CC  = clang++ -O2 -g -Wall -std=c++14 -Wextra -pedantic
INC = 
LIBS = 
SHFLAGS = -fPIC
DEPFLAGS = -M

AR = ar -rv

SRC_ALL = $(wildcard *.cpp)
#OBJ_ALL = $(SRC_ALL:.cpp=.o)
SRC = $(filter-out test.cpp, $(SRC_ALL))
OBJ = $(SRC:.cpp=.o)
DEPENDS = $(SRC_ALL:.cpp=.d)

.PHONY: all lib clean

all : test
	@:

lib : libflac.a
	@:

clean : 
	/bin/rm -f *.o
	/bin/rm -f *.d
	/bin/rm -f *.a
	/bin/rm -f test

test : libflac.a test.o
	@#/bin/rm -f test
	$(CC) $^ $(LIBS) -o $@

libflac.a : $(OBJ)
	$(AR) $@ $^

%.o : %.cpp
	$(CC) $(SHFLAGS) $(INC) -c $< -o $@

%.d : %.cpp
	set -e; /bin/rm -f $@; \
         $(CC) $(DEPFLAGS) $(INC) $< > $@.$$$$; \
         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
         rm -f $@.$$$$

ifeq (,$(MAKECMDGOALS))
sinclude $(DEPENDS)
endif
ifneq (,$(filter-out clean, $(MAKECMDGOALS)))
sinclude $(DEPENDS)
endif

