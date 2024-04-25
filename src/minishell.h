
#ifndef MINISHELL_H
# define MINISHELL_H

# include "../external_libs/42_libs/ft_libs.h"
# include "../src/utils/utils.h"

typedef struct	s_token
{
	t_tag			tag;
	char			*lexeme;
	struct s_token	*next;
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

#endif
