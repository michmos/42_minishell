#include "minishell.h"

void init_info(t_info *info, char **env, char **line, int first)
{
    if (first)
    {
        info->cur_dir = getcwd(NULL, 0);
        info->env_lst = create_envlst(env);
        info->std_in = dup(STDIN_FILENO);
        info->std_out = dup(STDOUT_FILENO);
        *line = NULL;
    }
    info->prev_error = info->error;
    info->error = 0;
    info->num_cmd = 0;
}

int main(int argc, char **argv, char **env)
{
    t_info info;
    t_list *head; // result of parsing? command linked list
    t_list *temp;
    char *line;

    temp = head;
    info->num_cmd = ft_lstsize(temp); // maybe these 2 lines need to go in while loop?
    init_info(&info, &env, &line, 1); // first time send 1, to fill the necessary info
    while (1)
    {
        dup2(info.std_in, STDIN_FILENO);
        dup2(info.std_out, STDOUT_FILENO);
        // deal with signals
        init_info(&info, env, &line, 0);
        // readline and parse it???
        if (line)
            add_history(line);
        finalize_cmd(info);
        pipex(head, info, line); // do i need line or it will be in redirections list?
        free_info(&info, line, 0);
    }
}