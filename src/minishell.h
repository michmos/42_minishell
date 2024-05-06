
#ifndef MINISHELL_H
# define MINISHELL_H

# include "../external_libs/42_libs/ft_libs.h"

# define VEC_START_SZ 4

typedef enum e_tag
{
	I_RD,
	I_RD_HD,
	O_RD,
	O_RD_APP,
	PIPE,
	SI_QUO,
	DU_QUO,
	WORD
} t_tag;

typedef enum e_error
{
	// No error
	NO_ERR,

	// Syscall fails
	SYS_ERR
} t_error;

typedef struct	s_token
{
	t_tag			tag;
	char			*lexeme;
} t_token;

typedef struct s_parse_str
{
	char	*buf;
	size_t	buf_len;
	size_t	cursor_pos;
} t_parse_str;

typedef struct s_vec
{
	char	*buf;
	size_t	size;
} t_vec;

typedef struct s_AST
{
	t_tag			tag;
	char			*buf;
	struct s_AST	**next;

} t_AST;

// --------------------------- PARSING/ --------------------------------------//
// lex_add_ops.c -------------------------------------------------------------//
t_error	add_word(t_parse_str *cmd_line, t_vec *lexeme);
t_error	add_str_literal(t_parse_str *cmd_line, t_vec *lexeme);
t_error	add_char(char c, t_vec *vector);

// lex_char_ops.c ------------------------------------------------------------//
char	peek_char(t_parse_str *cmd_line);
char	next_char(t_parse_str *cmd_line);
char	cur_char(t_parse_str *str);

// lexer.c -------------------------------------------------------------------//
t_list	*lexer(t_parse_str *cmd_line);

// ---------------------------- UTILS/ ---------------------------------------//
// utils.c -------------------------------------------------------------------//

#endif
