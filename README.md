# minishell

Minishell is a minimalist shell implementation in C, created for educational purposes. The project supports basic features of a bash-like shell.

## Features

- Execute external programs with arguments
- Built-in commands: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- Support for pipes (`|`) and redirections (`>`, `<`, `>>`, `<<`)
- Heredoc (`<<`)
- Environment variables (export, modify, unset)
- Signal handling (`Ctrl+C`, `Ctrl+D`)
- Command history (via readline)
- Proper memory management

## Build

Requires GNU Make, a C compiler, and the readline library.

```sh
make
```

## Run

```sh
./minishell
```

## Project Structure

- [`src/`](src/) — minishell source code
- [`includes/`](includes/) — header files
- [`libft/`](libft/) — custom C standard library functions
- [`tests/`](tests/) — tests (if present)
- [`Makefile`](Makefile) — build rules

## Usage

Type commands as in a regular shell:

```sh
echo Hello, World!
export VAR=42
echo $VAR
ls -l | grep minishell > result.txt
cat << EOF
multi-line
heredoc
EOF
```

## License

For educational use only.
