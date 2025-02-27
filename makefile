#Compilation
CC = gcc
LINKER_FLAGS = -lws2_32
API_FLAGS = -ladvapi32 

#File path
SOURCE = ./src
BUILD = ./build
OBJECT = ./obj

include $(SOURCE)/makefile

#Procedures
compile = $(CC) -c $(SOURCE)/$(1).c -o $(OBJECT)/$(1).o

compileAll = @for var in $(1); do\
				$(call compile ,$$var);\
			done;

link = $(CC)  $(patsubst %, $(OBJECT)/%.o, $1)  $(LINKER_FLAGS) $(API_FLAGS) -o $(BUILD)/$(2)


all :  compile link

link : server client

compile: server.o client.o

#%.c:
#	$(call compile ,$(patsubst %.c,%, $@));

server.o:
	$(call compileAll , $(SERVER_LIST_PATH))

client.o:
	$(call compileAll , $(CLIENT_LIST_PATH))

server :
	$(call link ,$(SERVER_LIST_PATH),$(SERVER_APP_NAME))

client : 
	$(call link ,$(CLIENT_LIST_PATH),$(CLIENT_APP_NAME))

clean: clean_o clean_e

clean_o : 
		rm -f $(OBJECT)/*

clean_e :		
		rm -f $(BUILD)/*.exe

count:
	find src include -name '*.*' | xargs wc -l