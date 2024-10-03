# 42 minishell
This project was developed together with [**PauliusMinialga**](https://github.com/PauliusMinialga)

## About the project
### Task
The goal of this project was to write your own little shell with `bash` serving as a reference for most cases.
Given its nature, the shell should run in an interactive session, allowing for user input, which is then validated,
tokenized, and parsed before it is eventually executed. Of course, a shell wouldn't be too useful without its built-ins. 
Find a more extensive list of the features below:

### Features
- Input and output redirections
- Here-doc support
- Pipes
- Environment variables
- Quotes ("", '')
- Exit codes ($?)
- Signals (Ctrl-C, Ctrl-D, Ctrl-)
- Built-ins (echo, cd, pwd, export, unset, env, exit)
- History

### Responsibilities
- [**Paulius**](https://github.com/PauliusMinialga): execution, signals
- **Me**: parsing, variable expansion
- **Both**: debugging, built-ins

### Technologies
- The shell is written entirely in `C` and can be compiled using a `Makefile`
- As a team we heavily relied on `git` as a VCS


## Learnings
### Teamwork 
- Effective work division and milestone setting
- Git usage in a team (branching, merging, rebasing, preventing merge conflicts, solving merge conflicts ;) etc.)
- Considerate and effective communication

### Compilers / programming languages 
- Language definition through grammars
- Implementation of grammars through lexers and parsers
- Parsing algorithms


## Utilization
### Cloning
Run the following command to clone the repo and build the executable:
```
git clone https://github.com/michmos/42_minishell.git && cd 42_minishell && make
```
### Running
Run the executable:
```
./minishell
```
