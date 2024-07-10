
#ifndef MINISHELL_H
# define MINISHELL_H

# include "../external_libs/42_libs/ft_libs.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include <errno.h>
# include <signal.h>
# include <stdbool.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <linux/limits.h>

# define ERROR -1

typedef enum e_error
{
	// No error
	NO_ERR,
	// Syscall fails
	SYS_ERR,
	// Syntax error
	SYN_ERR,
	// Bad function input
	INP_ERR
}	t_error;

// -------------------------------------------------------------------------- //
// ---------------------------- PARSING/ ------------------------------------ //
// -------------------------------------------------------------------------- //

typedef enum e_tag
{
	WHITESPACE,
	I_RD,
	I_RD_HD,
	O_RD,
	O_RD_APP,
	PIPE,
	SI_QUO,
	DU_QUO,
	WORD
}	t_tag;

typedef struct s_token
{
	t_tag	tag;
	char	*lexeme;
}	t_token;

// parsing.c ---------------------------------------------------------------- //
t_error	parsing(t_list **cmd_lst, char *str, t_list *env_lst);

// -------------------------- PARSING/lexer/ -------------------------------- //

# define VEC_START_SZ 4

typedef struct s_parse_str
{
	char	*buf;
	size_t	buf_len;
	size_t	cursor_pos;
}	t_parse_str;

typedef struct s_vec
{
	char	*buf;
	size_t	size;
}	t_vec;

// parse_literals.c --------------------------------------------------------- //
t_error	parse_str_dq(char **lexeme, t_parse_str *cmd_line);
t_error	parse_str_sq(char **lexeme, t_parse_str *cmd_line);
t_error	parse_word(char **lexeme, t_parse_str *cmd_line);

// add_char.c --------------------------------------------------------------- //
t_error	add_char(char c, t_vec *vector);

// char_ops.c --------------------------------------------------------------- //
char	peek_char(t_parse_str *str);
char	cur_char(t_parse_str *str);
void	advance_char(t_parse_str *str, size_t n);

// lexer.c ------------------------------------------------------------------ //
t_error	create_token_lst(t_list **head, t_parse_str *cmd_line);

// -------------------------- PARSING/parser/ ------------------------------- //

typedef struct s_cmd
{
	char	**args;
	t_list	*redir_lst;
	int		*fd_array;
	int		redir_count;
	int		builtin;
	char	*path;
	int		last_input;
	int		last_output;
}	t_cmd;

typedef struct s_redir
{
	t_tag	type;
	char	*filename;
}	t_redir;

// utils.c ------------------------------------------------------------------ //
t_token	pop_token(t_list **rem_tokens);
void	consume_token(t_list **rem_tokens);
t_tag	get_token_tag(t_list *tokens);
bool	is_literal(t_tag tag);
bool	is_redir(t_tag tag);

// expand_env_var.c --------------------------------------------------------- //
t_error	expand_env_var(char **str, t_list *env_lst);

// get_cmd_args.c ----------------------------------------------------------- //
t_error	extend_arg_lst(t_list **args_lst, t_list **rem_tokens, t_list *env_lst);

// get_redir_lst.c ---------------------------------------------------------- //
t_error	extend_redir_lst(t_list **head, t_list **rem_tokens);

// parser.c ----------------------------------------------------------------- //
t_error	create_cmd_lst(t_list **cmd_lst, t_list **token_lst, t_list *env_lst);

// -------------------------------------------------------------------------- //
// --------------------------- EXECUTION/ ----------------------------------- //
// -------------------------------------------------------------------------- //

// ----------------------- EXECUTION/builtins/ -------------------------------//
typedef struct s_env_var
{
	char			*key; //before equal sign
	char			*value; //after equal sign
	bool			equal;
}	t_env_var;

typedef struct s_parse_env
{
	char	*buffer;
	size_t	buffer_len;
	size_t	cursor_pos;
}	t_parse_env;

// builtins.c --------------------------------------------------------------- //
void	print_envlst(t_list *head);
void	check_builtins(char **argv, char **env);

// env.c -------------------------------------------------------------------- //
char	*get_key(t_env_var *env_var, t_parse_env *env_parse);
char	*get_value(t_env_var *env_var, t_parse_env *env_parse);
t_env_var	*get_env_var(char *env);
t_list	*create_envlst(char **env);
void	free_env_var(void *node);

// ordered_env.c ------------------------------------------------------------ //
t_list	*create_ordered_envlst(t_list *env);
char	*key(t_list *node);

// add_to_list.c ------------------------------------------------------------ //
t_list	*add_to_ordered_envlst(t_list *head, char *argv);
t_list	*add_to_envlst(t_list *head, char *argv);

// print_pwd.c -------------------------------------------------------------//
void	print_pwd(void);

// unset.c ----------------------------------------------------------------//
t_list	*unset_envlst(t_list *head, char *argv);

// cd.c -------------------------------------------------------------------//
int		change_directory(char **argv);

// echo.c -----------------------------------------------------------------//
int		echo(char **argv);

// exit.c -----------------------------------------------------------------//
int		exit_bash(char **argv);

// prompt.c ---------------------------------------------------------------//
void	show_prompt(void);

// ----------------------- EXECUTION/execution/ ---------------------------//
typedef struct s_info
{
	int num_cmd;
	int **fd;
	int *pid;
}	t_info;

// execution.c ------------------------------------------------------------//
int		pipex(t_list *head, char **env);

// pipex_helper.c ---------------------------------------------------------//
void	err_exit(char *str);
int		i_o_redirection(int input_fd, int output_fd);
t_cmd	*get_cmd(t_list *lst);
t_redir	*get_redir(t_list *lst);

// pipex_paths.c ----------------------------------------------------------//
char	*find_command_path(char *command, char **env);
char	*get_env_path(char **env);

// pipex.c ----------------------------------------------------------------//
pid_t	cmd_pipeline(t_list *head, int num_childs, char **env);

// redir_list.c -----------------------------------------------------------//
t_list	*return_here_doc_in(t_list *head);
t_list	*return_here_doc_out(t_list *head);
char	*in_file(t_list *head);
char	*out_file(t_list *head);

// -------------------------------------------------------------------------- //
// ----------------------------- UTILS/ ------------------------------------- //
// -------------------------------------------------------------------------- //

// utils.c ------------------------------------------------------------------ //
char	**store_ptrs_in_arr(t_list *lst);
char	*get_env_val_ptr(char *key, t_list *env_list);
//
// free.c ------------------------------------------------------------------- //
void	free_token(void *token);
void	free_cmd(void *cmd);
void	free_redir(void	*redir);
#endif
