all:
	gcc -I $(HEIF_INCLUDE_DIR) -c heif_env.c
	gcc -I $(HEIF_INCLUDE_DIR) -c main.c 
	gcc main.o heif_env.o -o a.out 
	gcc -I $(HEIF_INCLUDE_DIR) -I $(JAVA_HEADERS) -I $(JAVA_LINUX_HEADERS) -I $(WRAPPER_HEADERS) -c heif_wrapper.c
	gcc -shared -o libockaheif.so heif_wrapper.o heif_env.o
clean:
	rm -f *.o
	rm -f *.so
	rm -f a.out
