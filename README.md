# simple-shell


**Description:**

-  Parse Input function: parses the input and convert command line into vector of tokens by using regular expressions.
- Convert to C String: converts the vector of tokens into array of C strings corresponding to the parameters of execvp ( ) function.
- Signal stop handler: this function is called when the child process is terminated which sends a signal to the parent process. This functions write logs into a file with the time stamp when child process is terminated.
- Main module:
 **1.** Gets input from the user and parse input.
 **2.** If the command contains &amp; at its end, it sets a background variable which skips waiting for the child process.
 **3.** Check if the command contains exit or cd which handled separately from the execvp function.










