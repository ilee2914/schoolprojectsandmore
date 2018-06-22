########################################
##
## Makefile
## LINUX compilation 
##
##############################################





#FLAGS
C++FLAG = -g -std=c++11

MATH_LIBS = -lm

EXEC_DIR=.


.cc.o:
	g++ $(C++FLAG) $(INCLUDES)  -c $< -o $@


#Including
INCLUDES=  -I. 

#-->All libraries (without LEDA)
LIBS_ALL =  -L/usr/lib -L/usr/local/lib 


#First Program (ListTest)

Cpp_OBJ=image.o p4.o

PROGRAM_NAME=p4

$(PROGRAM_NAME): $(Cpp_OBJ)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(Cpp_OBJ) $(INCLUDES) $(LIBS_ALL)


all: 
	make $(PROGRAM_NAME) 


clean:
	(rm -f *.o;)

(:
