objs = ServerEntry.o ServerThread.o Auth.o CSProtocol.o log.o
headers = ServerThread.h Auth.h CSProtocol.h log.h

Server : $(objs)
	gcc -o server $(objs) -lpthread

ServerEntry.o : ServerEntry.c $(headers)
	gcc -c ServerEntry.c 
	
ServerThread.o : ServerThread.c $(headers)
	gcc -c ServerThread.c -lpthread 

CSProtocol.o : CSProtocol.c CSProtocol.h
	gcc -c CSProtocol.c 

log.o : log.c log.h
	gcc -c log.c
	
Auth.o : Auth.c Auth.h
	gcc -c Auth.c
	
clean : 
	rm $(objs) server client Client.o
	
Client : Client.o CSProtocol.o log.o
	gcc -o client Client.o CSProtocol.o log.o -lncurses
	
Client.o : Client.c CSProtocol.h
	gcc -c Client.c -lncurses
