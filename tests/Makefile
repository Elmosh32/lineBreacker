CXX?=g++
CC=$(CXX)

LDLIBS   = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network -lX11

CPPFLAGS += -MMD -MP

CXXFLAGS +=  -std=c++20 -pedantic -Wall -g  $(LDLIBS) 
CPPFLAGS += -I$(INC_DIR) -I$(ONLINE_INC_DIR) 

ONLINE_SRC_DIR = ../src/multiplayer
ONLINE_INC_DIR = ../inc/multiplayer
SRC_DIR = ../src
INC_DIR = ../inc

TARGET = utest
SERVER_TARGET = server

#   $(ONLINE_SRC_DIR)/playerManager.o  
OBJECTS = utest.o  $(SRC_DIR)/game.o $(SRC_DIR)/paddle.o $(SRC_DIR)/brick.o $(SRC_DIR)/ball.o $(SRC_DIR)/collision.o $(SRC_DIR)/runLevel.o 
OBJECTS +=  $(ONLINE_SRC_DIR)/client.o  $(ONLINE_SRC_DIR)/playerManager.o 
OBJECTS +=  $(SRC_DIR)/canvas.o $(SRC_DIR)/resourceManager.o 
SERVER_OBJECTS = $(ONLINE_SRC_DIR)/server.o $(ONLINE_SRC_DIR)/ipPool.o  


check: $(TARGET)
	./$(TARGET)

server: $(SERVER_OBJECTS)
	$(CXX) $(CXXFLAGS) -o server  $(SERVER_OBJECTS) $(LDLIBS) 

server_check: $(SERVER_TARGET)
	./$(SERVER_TARGET)

$(TARGET): $(OBJECTS)

all:  $(TARGET)

debug: $(TARGET)
	gdb ./$(TARGET)

valgrind:$(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=logV1.txt ./$(TARGET)

clean:
	@$(RM) $(OBJECTS) $(DEPENDS) $(TARGET) $(SERVER_TARGET) $(SERVER_OBJECTS) 

DEPENDS = $(OBJECTS:%.o=%.d) $(OBJECTS:%.o=%.d)

-include $(DEPENDS)			
