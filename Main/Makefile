

CC  = gcc 
cflags = -lmosquitto -lpthread -ldl
OBJ1 += Main.o
OBJ1 += sqlite3.o
OBJ1 += MyFunction.o

OBJ2 += MyCgi.o
OBJ2 += sqlite3.o

OBJ3 += MyCgi2.o


Main: $(OBJ1)  
	$(CC) $(OBJ1) -o $@ $(FLAGS) $(cflags)
MyCgi.cgi:$(OBJ2)  
	$(CC) $(OBJ2) -o $@ $(FLAGS) $(cflags)
MyCgi2.cgi:$(OBJ3)  
	$(CC) $(OBJ3) -o $@ $(FLAGS) $(cflags)
	
%.o:%.c
	$(CC) -c $^ -o $@ $(FLAGS) $(cflags)

.PHONY:clean
clean:
	rm  *.o -rfv	  