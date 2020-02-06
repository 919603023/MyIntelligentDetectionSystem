CC  = gcc 
cflags = -lmosquitto -lpthread -ldl  
OBJ1 += MyCgi.o
OBJ1 += sqlite3.o
MyCgi.cgi:$(OBJ1)  
	$(CC) $(OBJ1) -o $@ $(FLAGS) $(cflags)
%.o:%.c
	$(CC) -c $^ -o $@ $(FLAGS) $(cflags)

.PHONY:clean
clean:
	rm  *.o -rfv