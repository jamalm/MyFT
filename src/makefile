objs = ServerEntry.o ServerThread.o
headers = ServerThread.h

Server : $(objs)
	gcc -o server $(objs) -lpthread

ServerEntry.o : ServerEntry.c $(headers)
	gcc -c ServerEntry.c 
	
ServerThread.o : ServerThread.c $(headers)
	gcc -c ServerThread.c -lpthread
	
clean : 
	rm $(objs) server 
	
Client : 
	gcc -o client Client.c 
