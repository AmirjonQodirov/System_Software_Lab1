
all: 
	$ gcc -c main.c ./partitions/*.c ./partitions/*.h ./read_be/*.c ./read_be/*.h ./hfsp/*.c ./hfsp/*.h
	$ mv ./*.o builds/
	$ gcc -o main ./builds/*.o
	$ chmod a+x main

clean: 
	$(RM) main ./builds/*.o
