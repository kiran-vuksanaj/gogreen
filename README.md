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

#### Files & Function Headers
(we mightve split up our files a little tooo much)\
`main.c`

`cmdstack.c`

`entry_exit.c`

`execute.c`

`parseargs.c`

`prompt.c`

`redirect.c`

`shortcut.c`

`stry_util.c`
