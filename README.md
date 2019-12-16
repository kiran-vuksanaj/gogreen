### SHASH
Shell? a Shell! \
by Kiran Vuksanaj and Bryan Monge-Serrano

#### Features
- Forks and executes commands!
- Parses multiple commands one one line, separated by `;`
- Parses redirects using `<`, `>`, `>>`, and `&>` followed by a filename (including multiple redirects within one command)
- Forks and executes pipes, indicated by `<cmd>|<cmd>` recursively (aka multi-pipe!)
  - also we did that without `popen()` :smile:
- Succeeds regardless of leading/tailing whitespace, excessive middle whitespace, or no whitespace (surrounding a pipe, redirect, or semicolon specifically)
- Replaces the `~` symbol with a users home directory before execution
- Modifies terminal io settings in order to enter "raw mode," allowing for execution of code before the user clicks the ENTER key
- Stores a history of entered commands, which can be cycled through using the UP_ARROW and DOWN_ARROW keys
- Proper handling of heap memory clearing, valgrind sends her regards :heart:

#### Attempted
- **tab autocomplete:** an attempt at automatically filling in a filename based on the files in the current working directory failed, but the code attempt is visible on the branch `tabcomplete`

#### Bugs
- previously, typing tab (pre-autocomplete attempts) caused strange issues with typing input; this has been lazily covered up by replacing any tab typed with a `%` character
- setting the input of the program as a whole to be a series of commands not including any `exit` command results in a forever repeating loop printing the prompt
- Exceeding 512 characters entered (including ~ home directories) causes strange behavior
- We broke the delete/insert buttons; backspace is fine, just no delete/insert

#### Files & Function Headers
(we mightve split up our files a little tooo much)\
`main.c` -- *home of the prompt loop*
- dependencies: execute, prompt, entry_exit, cmdstack, shortcut
- `int main();`
  - runs a forever loop switching between get prompt and execute command

`cmdstack.c` -- *methods for managing command history*
Methods for editing, accessing, and clearing
- `void init_cstack()`
  - allocates memory for an empty node in the static variable for future usage
- `void clear_stack()`
  - frees all memory allocated to command history
- `void push_cmd()`
  - copies data stored in the prompt input buffer into an allocated memory
  - frees all memory associated with a command in the 'future' compared to the written command
- `void link_cstack(char *buf)`
  - copies address of string buffer `buf` into the active stack command (generally the buffer to which a user is writing)
  - should only be called when stack's stored command is NULL

- `void fwd_cstack(char *buf)`
  - writes the current contents of string buffer `buf` into an allocated memory block for the current stack command ('saves')
  - increments to the next command into the future, copies the commmand into `buf`, and links the command to `buf`
- `void bk_cstack(char *buf)` (see above, replace future with past)

`entry_exit.c` -- *clean entry and exit using terminal IO settings*
- dependencies: cmdstack
- `void enable_rawmode()`
  - uses terminal ios to disable canonical blocking/flushing standards, and input io echos
  - stores the original state of blocking afterwardsto user lmao

- `void reset_terminos()`
  - restores state of the terminal to the former status in terms of IO standards

- `void config_exit()`
  - use the `atexit()` function to configure the actions that should occur upon an exit from the program of any sort-clearing the search history and resetting terminal IOs

`execute.c` -- *code to execute and pipe functions*
- dependencies: parseargs, execute, redirect, stry_util
- `void printerr()` - print contents of errno alongside matching error string
- `int exec_cmds(char *cmd)`
  - `char *cmd`: string as would be entered into the prompt, including any separator
  - parses input by semicolons and newlines into distinct commands, and passes these smaller commands on to the next step
  - Returns -1 on `exit`, 0 otherwise
- `int exec_cmd(char *cmd)`
  - `char *cmd` - string of command excluding semicolons or pipes, just a statement + redirects
  - parses statement on whitespace to format it as execvp would like
  - checks for `cd` or `exit` and handles the special cases of functions that must execute in the original process
  - if a special case is found, passes on to runchild()
  - Returns -1 on exit, 0 on cd, or the return value of the child
- `int exec_cmd_p(char *cmd)`
  - `char *cmd` - string of command between two semicolons, including any separator except semicolons
  - recursively handles `|` pipes in a command
  - Base case: when no pipes are present, executes `exec_cmd()` like normal
  - Recursive case: splits the command on the last pipe; forks a program with an outward pipe that recursively handles everything before the last pipe, and then runs everything after the last pipe with the pipe redirected into `stdin`
- `int runchild(char **args)`
  - `char **args` - NULL-terminated array of null-terminated strings representing args of the command
  - calls function to configure file redirects, forks and execvp's a child to run the args passed, and then returns the WEXITSTATUS or errno of the child

`parseargs.c` -- *just a command to parse on a delimiter (wrapping strsep)*
- `char **parseargs(char *cmd,char *delim)`
  - `char *cmd` - string to be split into an array
  - `char *delim` - array of characters on which it should be split
  - allocates memory for an array of strings, and populates it with substrings from the command, by running strsep repeatedly
  - terminates said array with a NULL pointer
  - automatically skips over empty strings, does not add them to the array :exclamation:

`prompt.c` -- *code to print the prompt and to handle input while in raw mode*
- `void print_prompt()` (it does what it says on the tin)
- `void get cmd(char *buf)`
  - `char *buf` - buffer into which input should be saved
  - takes the places of `fgets()` while in raw mode, saving and printing characters as they're written to `stdin`, with extra features using live input rather than post-newline blocking
  - handles backspaces, but not delete character
  - when an escape character (27) is detected, handles a ANSI escape sequence following it
  - reprints contents of the save buffer after each new character
  - (bug/missing) - lacking `n` variable, there's the potential for overwrite

- `int handleansi(char *s,int i)`
  - `char *s` - string on which getcmd is operating; save buffer for stdin
  - `int i` - current cursor position
  - examines the ansi escape sequence that appears next in `stdin`, and when it is an arrow key, handles such cases properly
  - horizontal arrow keys simply move the cursor, while vertical arrow keys issue commands to change position in history
  - Returns a new value for `i`, the next cursor position

- `int restrictindex(char *s,int i)`
  - replaces any value in `i` outside valid indices of string `s` with closes index in range, and returns this new value for `i`
- `int insshift(char *s,int i,char c)`
  - shifts characters after `s[i]` to the right and inserts character `c` at index `i`
  - returns new cursor position after insertion
- `int delshift(char *s,int i)`
  - shifts characters after `s[i]` to the left and deletes the character at index `i`
  - returns new cursor position after deletion

`redirect.c` -- *handling of file redirects*
- `int redirect(int src,int dest)`
  - `int src,dest` - file descriptors on which the redirect should be carried out
  - wrapper for `dup2` which copies contents of `fd1` into `fd2` while also creating a backup of fd2
  - returns a file descriptor for the backup, or -1 on failure
  - turning this into a function might've been the best decision made in this entire project
- `int redirect_filename(int fd_std, char *filename, int flags)`
  - opens file with name `filename` and `flags`, and redirects this newly opened file into the file descriptor specified int `fd_std` (presumably in/out/err)
  - returns backup file descriptor for the former contents of `fd_std`, or -1 on failure
- `char *getfilename(char **args, int i,char *symbol)`
  - returns a string representing the token immediately following the string `symbol` in either `args[i]` or `args[i+1]`, which should be the filename
- `void clear_used_args(char **args,int i,char *symbol)`
  - eliminates `symbol` and the filename immediately following it by adding terminating null characters (does NOT remove elements from args)
- `int parse_redirects(int *bk_pointer,char **args)`
  - `int *bk_pointer` - array of three blank ints, in which the three backup file descriptors will be written
  - `char **args` - args to scan for redirects
  - scans through the elements of args to find symbols representing file redirects, and configures the standard file streams to match the demands of these redirects
  - on exit, `bk_pointer` will be populated with file descriptors representing backups of the file streams, or -1 if they were not modified
  - returns 0 on success, -1 on failure
  
`shortcut.c` -- *replacing tildas with home directories*
- `void strinsert(char *dest, char *src)`
  - copies `src` into `dest` and shifts contents of `dest` to the right len(src) characters
- `void insert_shortcuts(char *buf)`
  - replaces all instances of `~` in `buf` with the user's home directory, as found by `getenv("HOME")`

`stry_util.c` -- *some utility functions for operations on NULL-terminated string arrays*
- `void remove_blanks(char **args)`
  - scans through `args` and deletes any elements that are empty strings, shifting subsequent elements to the left
- `char **strrystr(char **stry,char *str)`
  - returns a pointer to an element in `stry` which exactly matches `str`, or NULL if the element is not found
