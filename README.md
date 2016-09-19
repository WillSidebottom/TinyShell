#Operating Systems : TinyShell

##Student Info
Name : Will Sidebottom
FSUID: wks12

##Design Overview
The idea behind the design of the following program begins with splitting the functionality into batch mode and interactive mode. If the user specifies a parameter (in the form of a file location), then the
program will run in batch mode, otherwise the program will run in interactive mode. If running in batch mode, the program will parse the given file line-by-line for valid commands to be executed. If 
running in interactive mode, the program will accept user input, and parse the data for commands to be executed.

If the user were to type "quit", the program will execute all other valid commands on the same line followed by exiting the program. If the user were to type in "ctrl-D", the program would exit immediately.

The main routines for this program can be described as follows:
*readFile()* 	- if the user specifies a file as a parameter, this function will parse through each line of the file for commands, waiting for either "quit" or EOF
*readInput()* 	- reads user entered input strings, parses the strings, then executes each command (arguments). Waits for either the "quit" command or ctrl-D to exit the program.
*getSize()* 	- takes an input string as its argument and returns its size
*parse()*	- parses the input string based on a given delimiter and returns and array of strings
*print()*	- loops through an array of strings and prints the data
*clean()*	- loops through an array of strings and frees the data
*execute()*	- loops through an array of strings and executes each individual command and its arguments for execution

##Complete Specification
In order to find if ctrl-D was pressed, the main loop for the input() function checked for an EOF character to be read. If the user enters commands separated by a semi-colon or a space, the program would 
ideally tokenize the entered commands, separating them by those characters as delimiters.

##Known Issues
If the user were to type in two semi colons with the no commands, an empty prompt statement would appear before quitting. Also if the user were to enter ctrl-D, the program would execute properly but issue
the wrong error statement. 
