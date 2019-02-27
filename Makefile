all:
	$(CC) $(CFLAGS) -g -fsanitize=address -o demo demo.c -lcjson 
