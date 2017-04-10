objs = ServerEntry.o ServerThread.o
headers = ServerThread.h

Server : $(objs)
	gcc -o server $(objs) -pthread

ServerEntry.o : ServerEntry.c $(headers)
	gcc -c ServerEntry.c 
	
ServerThread.o : ServerThread.c $(headers)
	gcc -c ServerThread.c -pthread
	
clean : 
	rm $(objs) server 
	
Client : 
	gcc -o client Client.c 
