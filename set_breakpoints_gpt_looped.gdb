# set_breakpoints_gpt_looped.gdb

# Start the program
file main

# Set the starting line number
set $line_number = 16

# Set breakpoints in a loop for each line in the switch statement
while $line_number <= 20
  break main.c:$line_number
  set $line_number = $line_number + 1
end

# Run the program
run
