# @file Makefile
# @author 11796444 - Giovanni Shibaki Camargo 
# @author 11796451 - Pedro Kenzo Muramatsu Carmo
# @author 11796472 - Lucas Keiti Anbo Mihara
# 
# @copyright Copyright (c) 2021

ELECT=./lib/electricity.hpp
GAD=./lib/gadget.hpp
GADGETS=./lib/blender.hpp ./lib/grill.hpp ./lib/iceCream.hpp ./lib/order.hpp ./lib/colours.hpp ./lib/orderManager.hpp ./lib/printManager.hpp
MAIN=./src/main.cpp
BINARY= OvercookedOS

FLAGS = -Werror 
FLAGS2 = -Werror -Wall -Wextra -Wpedantic -Wpointer-arith -O3 -march=native
VFLAGS= --leak-check=full --show-leak-kinds=all --track-origins=yes

all:
	@g++ -pthread -g  $(MAIN) $(GAD) $(ELECT) $(GADGETS) -o $(BINARY) -lm -I lib/ -lncurses

run:
	./$(BINARY)

helgrind: 
	valgrind -v --tool=helgrind ./main

clean:
	@echo "Cleaning the mess!"
	@rm $(BINARY)
	@rm *.zip

zip:
	@echo "Creating a zip folder with all the important files!"
	@zip -r overcookedOS.zip lib/* src/* $(MAIN) Makefile README.md