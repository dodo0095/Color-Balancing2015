CC = g++
CFLAGS = -c -W
AR = ar
ARFLAGS = rcv
# -c option ask g++ to compile the source files, but do not link.
# -g option is for debugging version
# -O3 option is for optimized version
DBGFLAGS = -g -DEBUG
OPTFLAGS = -O3

all	: bin/color_balance
	@echo -n ""

# optimized version
bin/color_balance	 : graph_opt.o main_opt.o tm_usage.o 
			$(CC) $(OPTFLAGS) graph_opt.o main_opt.o tm_usage.o -o bin/color_balance

tm_usage.o 		: src/tm_usage.cpp src/tm_usage.h
			$(CC) $(CFLAGS) $< -Ilib -o $@
main_opt.o 	    : src/main.cpp
			$(CC) $(CFLAGS) $< -Ilib -o $@
graph_opt.o	    : src/graph.cpp src/graph.h 
			$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@

# DEBUG Version
dbg : bin/color_balance_dbg
	@echo -n ""

bin/color_balance_dbg	    : graph_dbg.o main_dbg.o tm_usage_dbg.o ga_dbg.o
			$(CC) $(DBGFLAGS) graph_dbg.o main_dbg.o tm_usage_dbg.o ga_dbg.o -o bin/color_balance_dbg

tm_usage_dbg.o 		: src/tm_usage.cpp src/tm_usage.h
			$(CC) $(CFLAGS) $< -Ilib -o $@
main_dbg.o 	   	: src/main.cpp
			$(CC) $(CFLAGS) $< -Ilib -o $@
graph_dbg.o	    : src/graph.cpp src/graph.h 
			$(CC) $(CFLAGS) $(DBGFLAGS) $< -o $@

# clean all the .o and executable files
clean:
		rm -rf *.o bin/*