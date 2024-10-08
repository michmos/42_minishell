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

typedef enum e_error
{
	// No error
	NO_ERR,
	// Shell cannot continue running - like failed malloc
	DEADLY_ERR = 200,
	// Shell can continue running - like syntax error
	ERR = 1,
	// Bad function input
	INP_ERR,
}	t_error;

// information specific to each command line
typedef struct s_cmdline
{
	t_list	*cmdlst;
	int		open_pipe_end;
	char	*hd_str;
} t_cmdline;

// information about the shell
typedef struct s_shell
{
	char		*cwd;		// cur working dir
	char		*old_wd;	// last working dir
	char		*home_dir;	// home dir
	t_list		*env_lst;	// env_lst
	char		**env;		// env array
	int			ex_code;	// ex_code of last command
	int			std_in;		// stdin backup
	int			std_out;	// stdout backup
	int			std_err;	// stderr backup
	t_cmdline	cur_cmdline;// cur cmdline data
}	t_shell;

// main.c ------------------------------------------------------------------- //
int	main(int argc, char *argv[], char **env);

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
void		advance_char(t_parse_str *str);

// lexer.c ------------------------------------------------------------------ //
t_error		create_token_lst(t_list **head, char *str);


// -------------------------- PARSING/parser/ ------------------------------- //
typedef struct s_cmd
{
	char	**args;
	t_list	*redir_lst;
}	t_cmd;

typedef struct s_redir
{
	t_tag	type;
	char	*filename;
}	t_redir;

// expand_all_env_vars.c --------------------------------------------------- //
t_error		expand_all_env_vars(char **str_ptr);

// expand_env_var.c --------------------------------------------------------- //
t_error		expand_env_var(char **str_ptr, size_t *cur_pos);

// extend_arg_lst.c --------------------------------------------------------- //
t_error		extend_arg_lst(t_list **args_lst, t_list **rem_tokens);

// extend_redir_lst.c ------------------------------------------------------- //
t_error		extend_redir_lst(t_list **head, t_list **rem_tokens);

// parser.c ----------------------------------------------------------------- //
t_error		create_cmd_lst(t_list **cmd_lst, t_list **token_lst);

// utils.c ------------------------------------------------------------------ //
t_token		pop_token(t_list **rem_tokens);
void		consume_token(t_list **rem_tokens);
t_tag		get_token_tag(t_list *tokens);
bool		is_literal(t_tag tag);
bool		is_redir(t_tag tag);


// -------------------------------------------------------------------------- //
// --------------------------- EXECUTION/ ----------------------------------- //
// -------------------------------------------------------------------------- //

// execution.c -------------------------------------------------------------- //
t_error		execution(t_list *pars_out);

// cmd_pipeline.c ----------------------------------------------------------- //
void		wait_for_childs(pid_t last_child, int *status);
t_error		cmd_pipeline(t_list *cmd_lst);

// get_cmd_path.c ----------------------------------------------------------- //
t_error		init_cmd_path(char **cmd_path, char *cmd, char **env);

// heredoc.c ---------------------------------------------------------------- //
t_error		exec_hd(t_list *redir_lst);

// fds.c -------------------------------------------------------------------- //
t_error		reset_io(void);
t_error		redir(int old_fd, int new_fd);
t_error		set_io_pipes(size_t child_i, size_t num_childs);
t_error		set_io_redirs(t_list	*redir_lst, char *hd_str);

// cmd_check.c -------------------------------------------------------------- //
void		check_cmd(char *path, char *arg);

// signals.c ---------------------------------------------------------------- //
int			init_signals(void);
void		signal_ctrl_d(char *line);
void		handle_sig(int signal);
void		signal_sigquit(int signal);
void		handle_sig_child(int signal);
void		handle_sig_hd(int signal);

// utils.c ------------------------------------------------------------------ //
t_cmd		*get_cmd(t_list *lst);
t_redir		*get_redir(t_list *lst);
void		err_exit(char *str);
char		**converter(t_list *head);
t_error		close_fd(int fd);


// ------------------------ EXECUTION/builtins/ ----------------------------- //
typedef struct s_env_var
{
	char	*key;
	char	*value;
	bool	equal;
}	t_env_var;

typedef struct s_parse_env
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

// builtins.c --------------------------------------------------------------- //
t_error		execute_builtin(char **args);
t_error		exec_one_builtin(t_cmd *cmd);
t_builtins	get_builtin_type(char *cmd);

// cd.c --------------------------------------------------------------------- //
t_error		cd(char *argv[]);
bool		is_dir(char *pathname);

// cd_sec_1_to_6.c ---------------------------------------------------------- //
t_error		init_curpath(char **curpath, char *arg);

// cd_sec_7_to_9.c ---------------------------------------------------------- //
void 		del_char(char *ptr);
t_error		cnvrt_to_rltv_path(char **curpath, char *cwd);
t_error		modify_curpath(char **curpath);

// env.c -------------------------------------------------------------------- //
t_error		env(char **args);
t_list		*create_envlst(char **env);
t_env_var	*create_env_var(char *env);

// echo.c ------------------------------------------------------------------- //
void		echo(char *argv[]);

// exit.c ------------------------------------------------------------------- //
t_error		exec_exit(char **args);

// export.c ----------------------------------------------------------------- //
t_error		exec_export(char **args);

// ordered_env.c ------------------------------------------------------------ //
void		print_ordered_lst(void);
char		*key(t_list *node);

// pwd.c -------------------------------------------------------------------- //
void		pwd(void);

// unset.c ------------------------------------------------------------------ //
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


// -------------------------------------------------------------------------- //
// ----------------------------- UTILS/ ------------------------------------- //
// -------------------------------------------------------------------------- //

// cleanup.c ---------------------------------------------------------------- //
void	clean_exit(int exit_code);

// free.c ------------------------------------------------------------------- //
void	free_redir(void	*redir);
void	free_cmd(void *cmd);
void	free_token(void *token);
void	free_env_var(void *var);

// utils.c ------------------------------------------------------------------ //
char	*get_env_val_ptr(char *key);
bool	has_key(void *var, char *key);
char	**store_ptrs_in_arr(t_list *lst);

#endif
