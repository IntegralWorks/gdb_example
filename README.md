# gdb_example
The goal of the code/information in this repository is to explain how to use GDB at a very simple level.
Here, "very simple level" means that the user-developer/student should be able to:
* Run a toy example with a straightfoward but simple segfault
* Run a toy example with straightforward "steps" (i.e. step through)
* Run a GDB Script so that it's clear how to set breakpoints and step them
* (Optional) add a .gdbinit file to change how GDB presents information.

0. Finding a segfault toy example
https://en.wikipedia.org/wiki/GNU_Debugger has a great one.
You can just follow along with their example. In summary, the segfault code example on-purposely runs `strlen` on a `NULL` pointer, inciting a Segmentation Fault.
The key takeaways from this article:
* A good "sane default" for compiling with debug symbols is
    * `gcc example.c -Og -g -o example`
* Running GDB is straightforward
* We still "don't know" how to "just step through" 
Now that we've seen a segfault "resolved" by GDB, we can move on to stepping through.

1. Making a toy example for stepping through
Because I've used a debugger in embedded (ex. STM32CubeIDE, Code Composer Studio) environments before, and those programs automate debugging immensely, I wanted to recreate that "convenience" purely [partially] in CLI because one can't be guaranteed a GUI environment for embedded targets.
I figured it should not be hard to make a simple switch-case function and step through it for every state:

```c
1. #include <stdio.h>
2. #include <stdlib.h>
3. #include <string.h>
4.
5. void step(int current_step)
6. {
7.  printf("step %i executed\n", current_step);
8. }
9.
10. void steps()
11. {
12.  int current_step = 0;
13.
14.  switch(current_step)
15.  {
16.    case 0: step(current_step); current_step++; //break statements are missing on purpose so that "we're sure" the breakpoints are really working
17.    case 1: step(current_step); current_step++;
18.    case 2: step(current_step); current_step++;
19.    case 3: step(current_step);
20.    default: break;
21.  }
22. }
23.
24. int main( int argc, char *argv[] )
25. {
26.  steps();
27.   exit( 0 );
28. }
```
(note: don't copy-paste this code because line numbers were manually added; just use `steps.c`; the line numbers are important to explain the GDB script)
Luckily, this was correct; you can indeed declare breakpoints for the above code quite easily.
I decided to consult the GDB manual at this point, but I'm hasty so I skipped to the "Stepping" portion: 

https://sourceware.org/gdb/current/onlinedocs/gdb.html/Continuing-and-Stepping.html#Continuing-and-Stepping

I got the main idea from it, and from there decided to do this:
* `gcc steps.c -Og -g -o steps`
* `gdb ./steps`
* `set step-mode on`
* `next` ... `next` (where `next`...`next` means typing `next`,`<Enter Key>`, `next`,`<Enter Key>`...until the program terminates)

Realizing that I needed to figure out how to actually set the breakpoint(s).
I will spare you the fumbling around I did in CLI and say that after also consulting

* https://darkdust.net/files/GDB%20Cheat%20Sheet.pdf
* https://users.ece.utexas.edu/~adnan/gdb-refcard.pdf

2. Running a GDB Script
I figured I could make this script:
```
set step-mode on
break 16
break 17
break 18
break 19
break 28
start
```
to *then* enter `next`...`next` properly. This yields the desired result.
Now all one needs to do is:
* `gcc steps.c -Og -g -o steps`
* `gdb ./steps`
* `source gdb_script`
* `next` ... `next`
It's *highly* recommended at this point that you experiment with some of the commands in at least https://darkdust.net/files/GDB%20Cheat%20Sheet.pdf, ex.
* Running `backtrace` before each `next` call 
* Running the different `info` arguments, ex. `info locals`, `info threads`

3. Adding a .gdbinit file.
GDB has a feature in which you can add a .gdbinit file and it will apply some defaults/commmands etc.
There are man pages about it (https://man7.org/linux/man-pages/man5/gdbinit.5.html) but .gdbinit usage from scratch is outside the scope of this document.

Instead, we can visit https://github.com/cyrus-and/gdb-dashboard and follow the instructions.
Because we are on a Raspberry Pi, we run into an issue during `pip install pygments` that is remedied by:

* `python3 -m venv .venv`
* `source .venv/bin/activate`
* `python3 -m pip install pygments`
(source: https://stackoverflow.com/questions/75602063/pip-install-r-requirements-txt-is-failing-this-environment-is-externally-mana)

Aside: The gdb-dashboard developer says `pygments` is necessary for syntax highlighting, but I found such highlighting worked outside of the `.venv`. YMMV. Also: don't forget the command to leave a venv is `deactivate`

Now (*in your home directory*) run
`wget -P ~ https://github.com/cyrus-and/gdb-dashboard/raw/master/.gdbinit`
Then just repeat "2. Running a GDB Script" and you should see that now a detailed menu appears with each `next` call.

# Troubleshooting for root
One last note: sometimes code of interest needs to be run as root (example: https://github.com/siewertsmooc/RTES-ECEE-5623/blob/main/simplethread-affinity/pthread.c)
Depending on your shell config(s), gdb-dashboard may not necessarily "just work" in root/sudo.
To make things brief: you should do this:
* `sudo -s`
* `cd $(echo ~root)`
* `ls -la` and confirm you can see `.bashrc`; note: this directory is essentially the "home" of the root user. So you can consider that in the same way it has a `.bashrc` distinct from the home user, it too has a distinctive `.gdbinit` file so we can easily install it for `root`.
* `wget -P ~ https://github.com/cyrus-and/gdb-dashboard/raw/master/.gdbinit`
Then just repeat "2. Running a GDB Script" and you should see that now a detailed menu appears with each `next` call.

# Can AI do this for us?
Sort of. At this point I realized I knew enough about GDB to test AI output.
I fed it these prompts: https://chat.openai.com/share/9ae0a8cf-43c5-4f71-880f-b8c0842901f1
and then combined the output of the prompts with the knowledge documented here to come up with `set_breakpoints_gpt.gdb` and `set_breakpoints_gpt_looped.gdb`. You can run these directly with 
`gdb -x set_breakpoints_gpt_looped.gdb`

Insights:
* It's a good idea to do things manually at first 
    * I would have had zero clue at all how to ask ChatGPT how to write what was needed
* Intuitively, GDB command files can have the file extension `.gdb`
* You can run a command file directly with the `-x` option
    * Formal source: 
        * `man gdb` yields no manual; consulting https://stackoverflow.com/questions/23684642/how-to-fix-no-manual-entry-for-gcc inspires
        * `sudo apt install gdb-doc -y`
        * Then try `man gdb` again. (also consult https://unix.stackexchange.com/questions/767480/how-do-i-add-line-numbers-to-the-man-page)
        * Line 116: -x <u>file</u> 
        Execute GDB commands from file
* **[!]** You can specify line numbers like this: *<source_code>*.c:*<line_number>*
    * This was tricker to track down. https://sourceware.org/gdb/current/onlinedocs/gdb.html/Source-Path.html is a good source and https://alex.dzyoba.com/blog/gdb-source-path/ could be a decent explanation.
