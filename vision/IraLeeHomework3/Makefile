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

Cpp_OBJ=image.o h1.o

PROGRAM_NAME=h1

$(PROGRAM_NAME): $(Cpp_OBJ)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(Cpp_OBJ) $(INCLUDES) $(LIBS_ALL)
	
Cpp_OBJ2=image.o h2.o

PROGRAM_NAME2=h2

$(PROGRAM_NAME2): $(Cpp_OBJ2)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(Cpp_OBJ2) $(INCLUDES) $(LIBS_ALL)

Cpp_OBJ3=image.o h3.o

PROGRAM_NAME3=h3

$(PROGRAM_NAME3): $(Cpp_OBJ3)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(Cpp_OBJ3) $(INCLUDES) $(LIBS_ALL)

Cpp_OBJ4=image.o h4.o

PROGRAM_NAME4=h4

$(PROGRAM_NAME4): $(Cpp_OBJ4)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(Cpp_OBJ4) $(INCLUDES) $(LIBS_ALL)
	


all: 
	make $(PROGRAM_NAME) $(PROGRAM_NAME2) $(PROGRAM_NAME3) $(PROGRAM_NAME4)
clean:
	(rm -f *.o;)

(:
