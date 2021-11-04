# Makefile

# Variables
CFLAGS = gcc -g -lm -ansi -std=c99 -Wall -c $(GTKINC)
FLAGS = gcc -ansi -g -lm -std=c99 -Wall $(GTKLIBS)
GTKLIBS = `PKG_CONFIG_PATH=/usr/share/pkgconfig pkg-config --libs gtk+-2.0`
GTKINC  = `PKG_CONFIG_PATH=/usr/share/pkgconfig pkg-config --cflags gtk+-2.0`
DESIGN = Chess

# Default target
all: Chess Client Server 

test: Chess
	./Chess

txt: Chess
	cat ChessHistory.txt

######################### Generate object files #######################
# Target for Board.o
Board.o: Board.h Board.c
	$(CFLAGS) Board.c -o Board.o

# Target for Chess_Pieces.o
CheckMove.o: CheckMove.h Board.h CheckMove.c 
	$(CFLAGS) CheckMove.c -o CheckMove.o

# Target for IsOccupied.o
IsOccupied.o: IsOccupied.h IsOccupied.c
	$(CFLAGS) IsOccupied.c -o IsOccupied.o 
 
# Target for Checkmate.o
Checkmate.o: Checkmate.h CheckMove.h Checkmate.c
	$(CFLAGS) Checkmate.c -o Checkmate.o

# Target for Help.o
Help.o: Help.h Help.c
	$(CFLAGS) Help.c -o Help.o

# Target for Menu.o
Menu.o: Menu.h Menu.c
	$(CFLAGS) Menu.c -o Menu.o

# Target for Main.o
Main.o: Main.c Menu.h Help.h Board.h 
	$(CFLAGS) Main.c -o Main.o
 
# Target for AI_1.o
AI_1.o: AI_1.c AI_1.h Checkmate.h
	$(CFLAGS) AI_1.c -o AI_1.o

# Target for Account.o
Account.o: Account.c Account.h
	$(CFLAGS) Account.c -o Account.o

# Target for Contacts.o
Contacts.o: Contacts.c Contacts.h Account.h
	$(CFLAGS) Contacts.c -o Contacts.o

Server_test.o: Server_test.c Account.h Contacts.h
	$(CFLAGS) Server_test.c -o Server_test.o

Client_test.o: Client_test.c Contacts.h
	$(CFLAGS) Client_test.c -o Client_test.o

Chess.o: Chess.c Chess.h Menu.h Help.h Board.h
	gcc -ansi -std=c99 -Wall -c Chess.c -o Chess.o

######################### Generate the executable #####################
# Target for Chess
Chess: Main.o Menu.o Help.o Board.o CheckMove.o IsOccupied.o Checkmate.o AI_1.o
	$(FLAGS) Main.o Menu.o Help.o Board.o CheckMove.o IsOccupied.o Checkmate.o AI_1.o -o Chess

########################### Client/Server ##############################
#Client: Client.c
#	$(FLAGS) Client.c -g -o Client

#Server: Server.c
#	$(FLAGS) Server.c -g -o Server

#Client2: Client2.o Account.o
#	$(FLAGS) Client2.o Account.o -o Client2

#Server2: Server2.o Account.o
#	$(FLAGS) Server2.o Account.o -o Server2

#Client3: Client3.c
#	gcc Client3.c -Wall -ansi -g -o Client3

#Server3: Server3.c
#	gcc Server3.c -Wall -ansi -g -o Server3

Client: Client_test.o Contacts.o
	$(FLAGS) Client_test.o Contacts.o -o Client

Server: Server_test.o Account.o Contacts.o Chess.o Menu.o Help.o Board.o CheckMove.o IsOccupied.o Checkmate.o
	$(FLAGS) Server_test.o Account.o Contacts.o Chess.o Menu.o Help.o Board.o CheckMove.o IsOccupied.o Checkmate.o -o Server


###############################  others  ##############################
# Target for clean
clean:
	rm -f *.o $(DESIGN) Client Server 


