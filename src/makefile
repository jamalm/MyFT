objs = ServerEntry.o ServerThread.o Auth.o CSProtocol.o 
headers = ServerThread.h Auth.h CSProtocol.h

Server : $(objs)
	gcc -o server $(objs) -lpthread

ServerEntry.o : ServerEntry.c $(headers)
	gcc -c ServerEntry.c 
	
ServerThread.o : ServerThread.c $(headers)
	gcc -c ServerThread.c -lpthread

CSProtocol.o : CSProtocol.c CSProtocol.h
	gcc -c CSProtocol.c 
	
Auth.o : Auth.c Auth.h
	gcc -c Auth.c
	
clean : 
	rm $(objs) server client Client.o
	
Client : Client.o CSProtocol.o
	gcc -o client Client.o CSProtocol.o
	
Client.o : Client.c CSProtocol.h
	gcc -c Client.c
