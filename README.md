Loony - a simple text editor.
=============================
Compilation For Linux
---------------------

First Method
--

1. Install the curses library by typing the following into a linux terminal: sudo apt-get install libncurses5-dev
2. Clone the repository by entering the following in a terminal:
git clone https://github.com/dreamyeyed/loony.git
(assuming you have git installed)
3. then do:
cd /loony/src
4. Compile using gcc in a terminal using the following:
gcc main.c textbuf.c util.c cursesio.c -lncurses -o loony
5. Then do: ./loony

Second Method
--
1. cd into the loony folder.
2. Install autoreconf via: sudo apt-get install dh-autoreconf
3. autoreconf --install
4. ./configure
5. make
6. If step 5 gives you an error with something like main.c:13:20: fatal error: curses.h: No such file or directory
compilation terminated.', then do: sudo apt-get install libncurses5-dev
	
Usage
----
So far, loony is similar to vim, so use the h, j, k, l keys to navigate while in command mode (press Esc)

---
Work in progress...

Loony is © dreamyeyed 2014.
Released under GNU GPL v3.0 or later.
