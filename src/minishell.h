
#ifndef MINISHELL_H
# define MINISHELL_H

# include "../external_libs/42_libs/ft_libs.h"
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
} t_error;


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
} t_token;


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

{
	t_tag			tag;
	char			*buf;
	struct s_AST	**next;

}	t_AST;

// --------------------------- PARSING/ --------------------------------------//
// lex_add_ops.c -------------------------------------------------------------//
t_error		add_word(t_parse_str *cmd_line, t_vec *lexeme);
t_error		add_str_literal(t_parse_str *cmd_line, t_vec *lexeme);
t_error		add_char(char c, t_vec *vector);

// lex_char_ops.c ------------------------------------------------------------//
char		peek_char(t_parse_str *cmd_line);
char		next_char(t_parse_str *cmd_line);
char		cur_char(t_parse_str *str);

// lexer.c -------------------------------------------------------------------//
t_list		*lexer(t_parse_str *cmd_line);

// ---------------------------- UTILS/ ---------------------------------------//
// utils.c -------------------------------------------------------------------//

// ---------------------------- BUILTINS/ ------------------------------------//
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

// builtins.c ---------------------------------------------------------------//
void		print_envlst(t_list *head);

// env.c --------------------------------------------------------------------//
char		*get_key(t_env_var *env_var, t_parse_env *env_parse);
char		*get_value(t_env_var *env_var, t_parse_env *env_parse);
t_env_var	*get_env_var(char *env);
t_list		*create_envlst(char **env);
void		free_env_var(void *node);

// ordered_env.c -----------------------------------------------------------//
t_list		*create_ordered_envlst(t_list *env);
char		*key(t_list *node);

// add_to_list.c -----------------------------------------------------------//
t_list		*add_to_ordered_envlst(t_list *head, char *argv);
t_list		*add_to_envlst(t_list *head, char *argv);
#endif
