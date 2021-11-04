  # Makefile

# Variables
CFLAGS = gcc -g -lm -ansi -std=c99 -Wall -c $(GTKINC)
FLAGS = gcc -ansi -g -lm -std=c99 -Wall $(GTKLIBS)
GTKLIBS = `PKG_CONFIG_PATH=/usr/share/pkgconfig pkg-config --libs gtk+-2.0`
GTKINC  = `PKG_CONFIG_PATH=/usr/share/pkgconfig pkg-config --cflags gtk+-2.0`
TXT = ./bin/ChessHistory.txt 
CLIENT = ./bin/Client_test.o
SERVER = ./bin/Server_test.o ./bin/Account.o ./bin/Contacts.o ./bin/Chess.o ./bin/Menu.o ./bin/Help.o ./bin/Board.o ./bin/CheckMove.o ./bin/IsOccupied.o ./bin/Checkmate.o
# Default target
all: ./bin/Client ./bin/Server

test: 
	@echo
	@echo "######################################## PLEASE READ ########################################"
	@echo "To run both Client and Server program, make sure to have two terminals to run both programs at the same time!"
	@echo "To run Server program, type: ./bin/Server <PortNumber>"
	@echo "  For example, './bin/Server 10000'"
	@echo "To run Client Program, type: ./bin/Client <HostName> <PortNumber>"
	@echo "  For example, './bin/Client bondi 10000'"
	@echo
	@echo "########################################### NOTES ###########################################"
	@echo "<PortNumber> is the port number specified for client and server to communicate"
	@echo "<PortNumber> must be greater than 2000"
	@echo "<HostName> is the server name the server program is running on for client to connect to"
	@echo "If the socket binding fails, please choose a different socket port number!"
	@echo
 

txt: 
	cat ./bin/ChessHistory.txt
 
tar:
	tar -zcvf P2_Beta_src.tar.gz README.md INSTALL COPYRIGHT Makefile src bin chess doc

######################### Generate object files #######################
# Target for all dependencies
./bin/%.o: ./src/%.c
	$(CFLAGS) $^ -o $@


######################### Generate the executable #####################
# Target for Client
./bin/Client: $(CLIENT)
	$(FLAGS) $^ -o $@
 
# Target for Server
./bin/Server: $(SERVER)
	$(FLAGS) $^ -o $@

###############################  others  ##############################
# Target for clean
clean:
	rm -rf ./bin/*.o $(TXT) ./bin/Client ./bin/Server #For Developer Package
	#rm -rf ./bin/*.o $(TXT) ./bin/Client ./bin/Server bin src COPYRIGHT INSTALL README.md *.tar.gz #For User Package