###############################################################################
# RUSH 3
# MyGKrellm
###############################################################################

###############################################################################
# Configuration
###############################################################################

TARGET			=	MyGKrellm

CC				=	g++

CFLAGS			=	-std=c++20 \
					-Wall \
					-Wextra \
					-Werror
LFLAGS			=	-lsfml-graphics \
					-lsfml-window \
					-lsfml-system \
					-lncurses
IFLAGS			=	-Iinclude
DFLAGS			=

###############################################################################
# Sources
###############################################################################

EXTENSION		=	.cpp

SOURCES			=	source/modules/UserModule.cpp \
					source/modules/TimeModule.cpp \
					source/modules/OsModule.cpp \
					source/modules/NetworkModule.cpp \
					source/modules/CpuModule.cpp \
					source/modules/MemoryModule.cpp \
					source/IMonitorDisplay.cpp \
					source/GraphicalMonitor.cpp \
					source/TextMonitor.cpp \
					source/main.cpp

###############################################################################
# Makefile Objects
###############################################################################

SHELL			:=	/bin/bash

FLAGS			=	$(CFLAGS) $(LFLAGS) $(IFLAGS) $(DFLAGS)

OBJECTS			=	$(SOURCES:$(EXTENSION)=.o)

###############################################################################
# Makefile logics
###############################################################################

all: $(TARGET)

%.o: %$(EXTENSION)
	$(CC) -c $< -o $@ $(FLAGS)

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(FLAGS)

clean:
	find -type f -iname "*.o" -delete
	find -type f -iname "*.d" -delete
	find -type f -iname "*.gcda" -delete
	find -type f -iname "*.gcno" -delete
	find -type f -iname "*.html" -delete
	find -type f -iname "*.css" -delete

fclean: clean
	rm -f $(TARGET)
	rm -f unit_tests

re: fclean all

.PHONY: all $(TARGET) clean fclean re
