# set_breakpoints.gdb

# Start the program
file main

# Set breakpoints for each line in the switch statement
break main.c:16
break main.c:17
break main.c:18
break main.c:20

# Run the program
run
