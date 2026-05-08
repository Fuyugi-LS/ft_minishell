*This project has been created as part of the 42 curriculum by nsongsit, vasukmua.*

# Minishell: As beautiful as shell

## Description
Minishell is a minimalist implementation of a POSIX-compliant shell, inspired by bash. The goal of this project is to understand the inner workings of a shell, including parsing, process creation, file descriptor management, and signal handling. It bridges the gap between the user and the operating system by interpreting command-line inputs and executing them seamlessly.

Key features include:
* Displaying a working prompt with a command history.
* Locating and executing standard executables via the `PATH` environment variable.
* Handling single quotes `'` and double quotes `"` accurately.
* Implementing pipes `|` to connect the output of one command to the input of another.
* Managing standard redirections (`<`, `>`, `>>`) and heredocs (`<<`).
* Handling environment variables (e.g., `$USER`) and the exit status variable (`$?`).
* Interactive signal handling (`ctrl-C`, `ctrl-D`, `ctrl-\`).
* Custom built-in commands: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, and `exit`.

## Instructions

### Prerequisites
The project requires the `readline` library to handle the command prompt and history.
* **Debian/Ubuntu:** `sudo apt install libreadline-dev`
* **macOS (via Homebrew):** `brew install readline`

### Compilation
To compile the project, run the following command at the root of the repository. Our `Makefile` is configured to silently compile the source files and libraries (`libft`, `ft_fprintf`) without cluttering the terminal.

```bash
make
```

### Execution
Once compiled, you can launch the shell by running:

```bash
./minishell
```

To clean the compiled object files and the executable, you can use:
```bash
make clean
make fclean
make re
```

## Resources

* **GNU Bash Reference Manual:** The ultimate guide to understanding how bash handles edge cases, quoting, and expansions.
* **Linux Man Pages:** In-depth documentation for critical system calls (`fork`, `execve`, `pipe`, `dup2`, `waitpid`).
* **Ast (Abstract Syntax Tree):** Used for structuring the parsing logic before execution.

### AI Usage
Artificial Intelligence (LLMs) was used during the development of this project strictly as a conceptual guide and debugging assistant. Specifically:
* **System Architecture:** Used to deeply understand the behavior of `dup2` and file descriptor management when handling multiple pipes and `execve` failures.
* **Debugging:** Used to analyze complex `valgrind` logs (specifically with flags like `--trace-children=yes` and `--track-fds=all`) to isolate memory leaks in child processes and ensure no file descriptors were left open.
