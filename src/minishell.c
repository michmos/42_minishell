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
    char *line;

    init_info(&info, &env, &line, 1); // first time send 1, to fill the necessary info
    while (1)
    {
        dup2(data.std_in, STDIN_FILENO);
        dup2(data.std_out, STDOUT_FILENO);
        // deal with signals
        init_info(&info, env, &line, 0);
        // readline and parse it???
        if (line)
            add_history(line);
        // maybe more checks with signals and then call execute || just call execute
        free_info(&info, line, 0);
    }
}