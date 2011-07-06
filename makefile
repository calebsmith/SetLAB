CC = g++

CFLAGS = -Wall			 	
         

NAME = SETLAB.exe

SOURCES = main.cpp command.cpp output.cpp script.cpp setclass.cpp pageclass.cpp 
	 	

LIBS = 
      

$(NAME): $(SOURCES)
	$(CC) -o $(NAME) $(SOURCES) $(CFLAGS) $(LIBS)

clean: 
	rm -f *.o 
