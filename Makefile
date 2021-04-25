# Author        : Shivani Sanjay Tergaonkar
# File          : Makefile
# Creation Date : 01/10/2021
# Description   : This file contains command line 
#                 options to build shell project

OBJS  = shell
CC    = gcc
SRC   = src/main.c

all:	
	@ echo --------------------------------------------
	@ echo   Building shell program for COEN-283 class 
	$(CC) -o $(OBJS) $(SRC)
	@ echo   Build successful                         
	@ echo   © Property of Shivani Sanjay Tergaonkar
	@ echo --------------------------------------------

clean:
	@ echo ---------------------------------------------
	@ echo   Deleting object files                      
	rm -rf $(OBJS)
	@ echo   Object files deleted                      
	@ echo ---------------------------------------------
