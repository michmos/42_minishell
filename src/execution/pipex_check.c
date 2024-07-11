#include "../minishell.h"

void check_dir(t_cmd *cmd, t_info *info)
{
    /*
        basically more checks
        1. starts with stat again
        2. depending on stats output which is an int, we write if statements
            and if they're true we return error messages and exit with relevant status
    */
}

void check_cmd(t_cmd *cmd, t_info *info)
{
    /*
        1. with stat check the first arg in args list, check command
        2. get command path
            - go through michas functions where he gets the comman path
        3. check command utils
            - check the command with access function and in case of error
                write error messages and exit with relevant status
        4. execve in if statement, and as we know if code after execve continues
            it means execve failed, so inside the we can write an error message, like command not found
    */
}