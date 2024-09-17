/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/11 10:17:01 by pminialg      #+#    #+#                 */
/*   Updated: 2024/09/12 12:39:21 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// # include "../external_libs/42_libs/ft_libs.h"
// # include "readline/readline.h"
// # include "readline/history.h"
// # include <stdio.h>
// # include <sys/stat.h>

# define SHELLNAME "minishell"
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
# include <sys/stat.h>

# define ERROR -1

// maybe i don't need this global signal, cause i have one in signals.c
extern int	g_signal;

typedef enum e_error
{
	// No error
	NO_ERR,
	// Syscall fails
	DEADLY_ERR = 200,
	// Syntax error
	ERR = 1,
	// Bad function input
	INP_ERR,
	// Other error
	OTH_ERR,
	// OPEN_ERR
}	t_error;

typedef struct s_shell
{
	char	*cwd;		// cur working dir
	char	*old_wd;	// last working dir
	t_list	*env_lst;	// env_lst
	char	**env;		// env array
	int		ex_code;	// ex_code of last command
	int		std_in;		// stdin backup
	int		std_out;	// stdout backup
	int		std_err;	// stderr backup
	int		open_fd;	// additional fd
	t_list	*cur_cmdlst;// cur command list
}	t_shell;

// init_shell.c ------------------------------------------------------------- //
void		init_shell(t_shell **ptr, char **env);

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
t_error	parsing(t_list **cmd_lst, char **str);

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
t_error		parse_str_dq(char **lexeme, t_parse_str *cmd_line);
t_error		parse_str_sq(char **lexeme, t_parse_str *cmd_line);
t_error		parse_word(char **lexeme, t_parse_str *cmd_line);

// add_char.c --------------------------------------------------------------- //
t_error		add_char(char c, t_vec *vector);

// char_ops.c --------------------------------------------------------------- //
char		peek_char(t_parse_str *str);
char		cur_char(t_parse_str *str);
void	advance_char(t_parse_str *str);

// lexer.c ------------------------------------------------------------------ //
t_error	create_token_lst(t_list **head, char *str);

// -------------------------- PARSING/parser/ ------------------------------- //

typedef struct s_cmd
{
	char	**args;
	t_list	*redir_lst;
}	t_cmd;

// TODO: we already have token - adapt naming and remove double
typedef struct s_redir
{
	t_tag	type;
	char	*filename;
}	t_redir;

// utils.c ------------------------------------------------------------------ //
t_token		pop_token(t_list **rem_tokens);
void		consume_token(t_list **rem_tokens);
t_tag		get_token_tag(t_list *tokens);
bool		is_literal(t_tag tag);
bool		is_redir(t_tag tag);

// expand_all_env_vars.c --------------------------------------------------- //
t_error		expand_all_env_vars(char **str_ptr);

// expand_env_var.c --------------------------------------------------------- //
t_error		expand_env_var(char **str_ptr, size_t *cur_pos);

// get_cmd_args.c ----------------------------------------------------------- //
t_error		extend_arg_lst(t_list **args_lst, t_list **rem_tokens);

// get_redir_lst.c ---------------------------------------------------------- //
t_error		extend_redir_lst(t_list **head, t_list **rem_tokens);

// parser.c ----------------------------------------------------------------- //
t_error		create_cmd_lst(t_list **cmd_lst, t_list **token_lst);

// -------------------------------------------------------------------------- //
// --------------------------- EXECUTION/ ----------------------------------- //
// -------------------------------------------------------------------------- //
typedef struct s_env_var
{
	char	*key;	// before equal sign
	char	*value; // after equal sign
	bool	equal;
}	t_env_var;

typedef struct s_parse_env // TODO: we already have parse_str - remove double
{
	char	*buffer;
	size_t	buffer_len;
	size_t	cursor_pos;
}	t_parse_env;

typedef enum e_builtins
{
	NO_BUILTIN,
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT
}	t_builtins;

// add_to_list.c ------------------------------------------------------------ //
t_error		add_to_envlst(t_list *head, char **argv);

// builtins.c --------------------------------------------------------------- //
void		print_ordered_lst(void);
void		print_envlst(t_list *head, int order);
t_builtins	get_builtin_type(char *cmd);
t_error		execute_builtin(char **args);
t_error		exec_one_builtin(t_cmd *cmd);

// fds.c--------------------------------------------------------------------//
t_error		reset_io(void);
t_error		set_io_redirs(t_list	*redir_lst, char *hd_str);
t_error		set_io_pipes(size_t child_i, size_t num_childs);

// fds2.c -------------------------------------------------------------------//
t_error		redir(int old_fd, int new_fd);
// cd_sec_1_to_6.c ---------------------------------------------------------- //
t_error		init_curpath(char **curpath, char *arg);

// cd_sec_7_to_8.c ---------------------------------------------------------- //
t_error		modify_curpath(char **curpath);

// cd_sec_9.c --------------------------------------------------------------- //
t_error		cnvrt_to_rltv_path(char **curpath, char *cwd);

// cd_utils.c --------------------------------------------------------------- //
bool		is_dir(char *pathname);
t_error		add_slash(char **str);
void		del_char(char *ptr);

// cd.c --------------------------------------------------------------------- //
t_error		cd(char *argv[]);

// echo.c ------------------------------------------------------------------ //
void		echo(char *argv[]);

// env.c -------------------------------------------------------------------- //
char		*get_key(t_env_var *env_var, t_parse_env *env_parse);
char		*get_value(t_env_var *env_var, t_parse_env *env_parse);
t_env_var	*get_env_var(char *env);
t_list		*create_envlst(char **env);
void		free_env_var(void *var);

// exit.c ---------------------------------------------------------------- //
int			str_is_num(char *str);
void		execute_exit(char **args);

// ordered_env.c ------------------------------------------------------------ //
t_list		*create_ordered_envlst(t_list *env);
char		*key(t_list *node);

// pwd.c ------------------------------------------------------------ //
void		pwd(void);

// unset.c ------------------------------------------------------------ //
t_error		unset(char *argv[]);

// -------------------------------------------------------------------------- //
// ------------------------ SETTERS_GETTERS/ -------------------------------- //
// -------------------------------------------------------------------------- //

// shell_struct.c ----------------------------------------------------------- //
t_shell		*get_shell_struct(void);
void		set_shell_struct(t_shell *new_shell);

// exit_code.c -------------------------------------------------------------- //
int			get_exit_code(void);
void		set_exit_code(int code);

// env_lst.c ---------------------------------------------------------------- //
t_list		*get_env_lst(void);
void		set_env_lst(t_list *new_lst);

// pwd.c -------------------------------------------------------------------- //
t_error		set_pwd(char *new_path);

// ----------------------- EXECUTION/execution/ ---------------------------//

// convert_tlist_2d.c ----------------------------------------------------//
char		**converter(t_list *head);

// execution.c ------------------------------------------------------------//
t_error		execution(t_list *pars_out);

// heredoc.c ---------------------------------------------------------//
t_error		exec_hd(char **hd_str, t_list *redir_lst);

// pipex_check.c ---------------------------------------------------------//
void		check_cmd(char *path, char *arg);

// pipex_free.c ---------------------------------------------------------//
void		free_ar2(void **array);
void		wait_free_exit(int exit_status);

// pipex_helper.c ---------------------------------------------------------//
t_cmd		*get_cmd(t_list *lst);
t_redir		*get_redir(t_list *lst);
void		err_exit(char *str);
t_error		close_fd(int fd);

// pipex_paths.c ----------------------------------------------------------//
t_error		init_cmd_path(char **cmd_path, char *command, char **env);
char		*get_env_path(char **env);
// static char *concat_path(char *dir, char *command);

// pipex.c ----------------------------------------------------------------//
void		wait_for_childs(pid_t last_child, int *status);
t_error		cmd_pipeline(t_list *cmd_lst);

// signals.c -----------------------------------------------------------//
void		handle_sig(int signal);
void		handle_sig_child(int signal);
void		handle_sig_hd(int signal);
void		signal_ctrl_d(char *line);
void		signal_sigquit(int signal);
int			init_signals(void);

// -------------------------------------------------------------------------- //
// ----------------------------- UTILS/ ------------------------------------- //
// -------------------------------------------------------------------------- //

// utils.c ------------------------------------------------------------------ //
char		**store_ptrs_in_arr(t_list *lst);
char		*get_env_val_ptr(char *key);
bool		has_key(void *var, char *key);

// free.c ------------------------------------------------------------------- //
void		free_token(void *token);
void		free_cmd(void *cmd);
void		free_redir(void *redir);

// exit_code.c -------------------------------------------------------------- //
int			get_exit_code(void);
void		set_exit_code(int code);
// cleanup.c --------------------------------------------------------------- //
void		clean_exit(int exit_code);

#endif
