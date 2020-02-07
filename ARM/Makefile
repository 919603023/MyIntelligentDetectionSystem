CC  = gcc 
cflags = -lmosquitto -lpthread -ldl  
OBJ1 += A53_Main.o

A53_Main:$(OBJ1)  
	$(CC) $(OBJ1) -o $@ $(FLAGS) $(cflags)
%.o:%.c
	$(CC) -c $^ -o $@ $(FLAGS) $(cflags) 
    
.PHONY:clean
clean:
	rm  *.o -rfv