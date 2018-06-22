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

Cpp_OBJ=image.o s1.o

PROGRAM_NAME=s1
$(PROGRAM_NAME): $(Cpp_OBJ)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(Cpp_OBJ) $(INCLUDES) $(LIBS_ALL)
	
Cpp_OBJ1=image.o s2.o

PROGRAM_NAME1=s2
$(PROGRAM_NAME1): $(Cpp_OBJ1)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(Cpp_OBJ1) $(INCLUDES) $(LIBS_ALL)

	
Cpp_OBJ2=image.o s3.o

PROGRAM_NAME2=s3
$(PROGRAM_NAME2): $(Cpp_OBJ2)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(Cpp_OBJ2) $(INCLUDES) $(LIBS_ALL)
	
Cpp_OBJ3=image.o s4.o

PROGRAM_NAME3=s4
$(PROGRAM_NAME3): $(Cpp_OBJ3)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(Cpp_OBJ3) $(INCLUDES) $(LIBS_ALL)


all: 
	make $(PROGRAM_NAME) 
	make $(PROGRAM_NAME1) 
	make $(PROGRAM_NAME2) 
	make $(PROGRAM_NAME3) 


clean:
	(rm -f *.o;)

(:
