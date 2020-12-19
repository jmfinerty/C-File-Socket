Contained in this folder should be 5 files:

	client: 	the executable file for the client
	
	client.c: 	the code for client. Can be compiled with "gcc -o client client.c".
	
	server:		the executable file for the server
	
	server.c: 	the code for server. Can be compiled with "gcc -o server server.c".
	
	README.txt:	this file.

	
These programs are capable of sending 
any file, regardless of type/extension,
between unix machines on different networks.
	
	
The execution of these files is as follows:

	server:		"./server port", where:
	
		"port" represents the port which the server will listen for client transmissions on
		
	client:		"./client hostname port filepath", where:
	
		"hostname" represents the IP address of the server machine
		
		"port" represents the same port as the server is using
		
		"filepath" represents the path to the file to send to the server
