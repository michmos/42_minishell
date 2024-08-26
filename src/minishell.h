
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
// # include <linux/limits.h>
# include <sys/stat.h>

# define ERROR -1

extern int g_signal;

typedef enum e_error
{
	// No error
	NO_ERR,
	// Syscall fails
	SYS_ERR,
	// Syntax error
	SYN_ERR,
	// Bad function input
	INP_ERR,
	// Other error
	OTH_ERR,
	ERR_PIPE,
	ERR_MALLOC,
	ERR_OPEN,
	ERR_CLOSE,
	ERR_DUP,
	ERR_EXEC,
	ERR_FORK,
	ERR_PRINT,
	ERR_CWD,
	CHILD,
	PARENT
} t_error;

typedef struct s_shell
{
	char *cwd;
	char *old_wd;
	t_list *env_lst;
	int ex_code;
	t_error error;
} t_shell;

// init_shell.c ------------------------------------------------------------- //
void init_shell(t_shell **ptr, char **env);

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
} t_tag;

typedef struct s_token
{
	t_tag tag;
	char *lexeme;
} t_token;

// parsing.c ---------------------------------------------------------------- //
t_error parsing(t_list **cmd_lst, char *str);

// -------------------------- PARSING/lexer/ -------------------------------- //

#define VEC_START_SZ 4

typedef struct s_parse_str
{
	char *buf;
	size_t buf_len;
	size_t cursor_pos;
} t_parse_str;

typedef struct s_vec
{
	char *buf;
	size_t size;
} t_vec;

// parse_literals.c --------------------------------------------------------- //
t_error parse_str_dq(char **lexeme, t_parse_str *cmd_line);
t_error parse_str_sq(char **lexeme, t_parse_str *cmd_line);
t_error parse_word(char **lexeme, t_parse_str *cmd_line);

// add_char.c --------------------------------------------------------------- //
t_error add_char(char c, t_vec *vector);

// char_ops.c --------------------------------------------------------------- //
char peek_char(t_parse_str *str);
char cur_char(t_parse_str *str);
void advance_char(t_parse_str *str, size_t n);

// lexer.c ------------------------------------------------------------------ //
t_error create_token_lst(t_list **head, t_parse_str *cmd_line);

// -------------------------- PARSING/parser/ ------------------------------- //

typedef struct s_cmd
{
	char **args;
	t_list *redir_lst;
} t_cmd;

typedef struct s_redir // TODO: we already have token - adapt naming and remove double
{
	t_tag type;
	char *filename;
} t_redir;

// utils.c ------------------------------------------------------------------ //
t_token pop_token(t_list **rem_tokens);
void consume_token(t_list **rem_tokens);
t_tag get_token_tag(t_list *tokens);
bool is_literal(t_tag tag);
bool is_redir(t_tag tag);

// expand_all_env_vars.c ----------------------------------------------------- //
t_error expand_all_env_vars(char **str_ptr);

// expand_env_var.c --------------------------------------------------------- //
t_error expand_env_var(char **str_ptr, size_t *cur_pos);

// get_cmd_args.c ----------------------------------------------------------- //
t_error extend_arg_lst(t_list **args_lst, t_list **rem_tokens);

// get_redir_lst.c ---------------------------------------------------------- //
t_error extend_redir_lst(t_list **head, t_list **rem_tokens);

// parser.c ----------------------------------------------------------------- //
t_error create_cmd_lst(t_list **cmd_lst, t_list **token_lst);

// -------------------------------------------------------------------------- //
// --------------------------- EXECUTION/ ----------------------------------- //
// -------------------------------------------------------------------------- //
typedef struct s_env_var
{
	char *key;	 // before equal sign
	char *value; // after equal sign
	bool equal;
} t_env_var;

typedef struct s_parse_env // TODO: we already have parse_str - remove double
{
	char *buffer;
	size_t buffer_len;
	size_t cursor_pos;
} t_parse_env;

typedef struct s_cmd_data
{
	t_cmd *pars_out;
	int *fd_array;
	int redir_count;
	int builtin;
	char *path; // 1-------
	int last_input;
	int last_output;
	char *hd_str;
	char **hd_array;
	int hd_count;
	char *cmd_path; //1--------

} t_cmd_data;
// why do i have path and cmd_path, what is the difference and where do i use it???

typedef struct s_info
{
	int num_cmd;
	int **fd;
	int *pid;
	int error;
	int prev_error;
	char *cur_dir;
	char *old_dir;
	int std_in;
	int std_out;
	t_list *env_lst;
	t_list *ordered_lst;
	char **our_env;
} t_info;

/*
	1------
	all three things marked with 1----- aren't they the same thing?
	why do i have all of them?
*/

// add_to_list.c ------------------------------------------------------------ //
t_list *add_to_ordered_envlst(t_list *head, char **argv);
t_list *add_to_envlst(t_list *head, char **argv);

// builtins.c --------------------------------------------------------------- //
void	print_ordered_lst(void);
void print_envlst(t_list *head, int order);
int	check_builtins(t_cmd	*cmd);
int execute_builtin(t_cmd_data *cmd, char *line, t_info *info);
t_error	exec_one_builtin(t_cmd *cmd, char *line, t_info *info);
t_error set_io_files(t_list	*redir_lst);

// cd_sec_1_to_6.c ---------------------------------------------------------- //
t_error init_curpath(char **curpath, char *arg);

// cd_sec_7_to_8.c ---------------------------------------------------------- //
t_error modify_curpath(char **curpath);

// cd_sec_9.c --------------------------------------------------------------- //
t_error cnvrt_to_rltv_path(char **curpath, char *cwd);

// cd_utils.c --------------------------------------------------------------- //
bool is_dir(char *pathname);
t_error add_slash(char **str);
void del_char(char *ptr);

// cd.c --------------------------------------------------------------------- //
t_error cd(char *argv[]);

// echo.c --------------------------------------------------------------------- //
void echo(char *argv[]);

// env.c -------------------------------------------------------------------- //
char *get_key(t_env_var *env_var, t_parse_env *env_parse);
char *get_value(t_env_var *env_var, t_parse_env *env_parse);
t_env_var *get_env_var(char *env);
t_list *create_envlst(char **env);
void free_env_var(void *node);

// exit.c -------------------------------------------------------------------- //
void exit_err(char *str, char *line, t_info *info);
void print_num_arg_err(char *str);
int str_is_num(char *str);
long long exit_value(char *str, char *line, t_info *info);
int execute_exit(t_cmd_data *cmd, char *line, t_info *info);

// ordered_env.c ------------------------------------------------------------ //
t_list *create_ordered_envlst(t_list *env);
char *key(t_list *node);

// pwd.c ------------------------------------------------------------ //
void pwd(void);

// unset.c ------------------------------------------------------------ //
t_list *unset_envlst(t_list *head, char **argv);

// -------------------------------------------------------------------------- //
// ------------------------ SETTERS_GETTERS/ -------------------------------- //
// -------------------------------------------------------------------------- //

// shell_struct.c ----------------------------------------------------------- //
t_shell *get_shell_struct(void);
void set_shell_struct(t_shell *new_shell);

// exit_code.c -------------------------------------------------------------- //
int get_exit_code(void);
void set_exit_code(int code);

// env_lst.c ---------------------------------------------------------------- //
t_list *get_env_lst(void);
void set_env_lst(t_list *new_lst);

// pwd.c -------------------------------------------------------------------- //
t_error set_pwd(char *new_path);

// ----------------------- EXECUTION/execution/ ---------------------------//

// convert_tlist_2d.c ----------------------------------------------------//
char	**converter(t_list *head);

// error.c ------------------------------------------------------------//
void	free_info(t_info *info);
void error(int error, t_info *info);
int error_open(t_cmd_data *cmd, int process, t_info *info);
void free_info_line(t_info *info, char *line);

// execution.c ------------------------------------------------------------//
int execution(t_list *head, t_info *info, char *line);

// finalize_cmd.c ------------------------------------------------------------//
void finalize_cmd(t_info *info, t_cmd_data *cmd);

// heredoc.c ---------------------------------------------------------//
void get_hd_str(t_cmd_data *cmd, t_info *info);
void init_heredoc(t_cmd_data *cmd, t_info *info);
void get_hd_fd(t_cmd_data *cmd, t_info *info);
void heredoc(t_cmd_data *cmd, t_info *info, int i);

// heredoc2.c ---------------------------------------------------------//
char *hd_strjoin(char *s1, char *s2, t_info *info);
void hd_utils(char **str, char *input, t_info *info, char **result);

// pipex_check.c ---------------------------------------------------------//
void check_dir_utils(t_cmd_data *cmd, t_info *info, struct stat file_stat);
void check_dir(t_cmd_data *cmd, t_info *info);
void check_cmd_utils(t_cmd_data *cmd, t_info *info, struct stat file_stat);
void check_cmd(t_cmd_data *cmd, t_info *info);
int check_executable(t_cmd_data *cmd, t_info *info);

// pipex_free.c ---------------------------------------------------------//
void free_ar2(void **array);
void wait_free_exit(t_list *head, int exit_status);

// pipex_helper.c ---------------------------------------------------------//
t_cmd *get_cmd(t_list *lst);
t_cmd_data	*get_cmd_data(t_cmd *cmd);
t_redir *get_redir(t_list *lst);
void err_exit(char *str);
void close_pipes(t_info *info);
t_error	close_fd_array(int *fd_array, size_t size);

// pipex_open_files.c -----------------------------------------------------//
int open_append(t_cmd_data *cmd, t_info *info, int i, int proc);
int open_input_output(t_cmd_data *cmd, t_info *info, int i, int proc);
t_error	open_files(int *fd_array, t_list *redir_lst);

// pipex_paths.c ----------------------------------------------------------//
char *find_command_path(char *command, char **env);
char *get_env_path(char **env);
// static char *concat_path(char *dir, char *command);

// pipex_pipe.c ----------------------------------------------------------------//
void dup2_copy(int old_fd, int new_fd, t_info *info);
void pipe_cmd(t_cmd_data *cmd, t_info *info, int i);

// pipex.c ----------------------------------------------------------------//
int cmd_pipeline(t_list *head, t_info *info, char *line);
int parent_process(t_info *info);

// redir_list.c -----------------------------------------------------------//
int	in_file(t_list *redir_lst);
int out_file(t_list *redir_lst);
void check_last_in_out(t_cmd_data *cmd);
void check_type(t_cmd_data *cmd, t_tag type, int i);
// signals.c -----------------------------------------------------------//
void handle_sig(int signal);
void	signal_ctrl_d(t_info *info, char **line);
void	sigint_handle(int signal);
void	sigquit_handle(int signal);
int	init_signals(void);

// -------------------------------------------------------------------------- //
// ----------------------------- UTILS/ ------------------------------------- //
// -------------------------------------------------------------------------- //

// utils.c ------------------------------------------------------------------ //
char **store_ptrs_in_arr(t_list *lst);
char *get_env_val_ptr(char *key);
bool has_key(void *var, char *key);

// free.c ------------------------------------------------------------------- //
void free_token(void *token);
void free_cmd(void *cmd);
void free_redir(void *redir);

// exit_code.c -------------------------------------------------------------- //
int get_exit_code(void);
void set_exit_code(int code);
// cleanup.c ----------------------------------------------------------------- //
void clean_exit(int exit_code);

/*for testing puroses my own struct init and etc...*/
// init_info.c -------- //
void	init_info(t_info **ptr, char **env);

// info_struct.c -------- //
t_info	*get_info_struct(void);
void	set_info_struct(t_info *new_shell);

void	free_cmd_lst(void *node);

#endif
