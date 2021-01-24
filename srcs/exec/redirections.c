#include "minishell.h"

int     *open_fds_in(t_command *content)
{
    char    *path;
    int     i;
    int     *fd;
    t_list_str  *files;

    files = content->files_in;
    i = 0;
    if(!(fd = malloc(sizeof(int) * ft_lstsize(files))))
        return NULL;
    while (files != NULL)
    {
        path = files->content;
        fd[i] = open(path, O_RDONLY, S_IRWXU);
        if (fd[i] < 0)
            return NULL;
        files = files->next;
        i++;
    }
    return fd;
}

int     *open_fds_out(t_command *content)
{
    char    *path;
    int     i;
    int     *fd;
    t_list_str *files_out; 
    t_list_str *files_append;

    files_out = content->files_out;
    files_append = content->files_append;

    i = 0;
    if(!(fd = malloc(sizeof(int) * (ft_lstsize(files_out) + ft_lstsize(files_append)))))
        return NULL;
    while (files_out != NULL)
    {
        path = files_out->content;
        fd[i] = open(path, O_WRONLY | O_CREAT, S_IRWXU);
        if (fd[i] < 0)
            return NULL;
        files_out = files_out->next;
        i++;
    }
    while (files_append != NULL)
    {
        path = files_append->content;
        fd[i] = open(path, O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);
        if (fd[i] < 0)
            return NULL;
        files_append = files_append->next;
        i++;
    }
    return fd;
}

int     open_redirections(t_list_cmd *cmd)
{
    t_command   *content;

    content = cmd->content;
    if (
        (content->fd_in = open_fds_in(content)) == NULL ||
        (content->fd_out = open_fds_out(content)) == NULL
    )
        return (-1);

    return (0);
}

int     close_fds(int *fds, int size)
{
    int i;

    i = 0;
    while(i < size)
    {
        if (close(fds[i++]) == -1)
            return -1;
    }
    return 0;
}

int     close_redirections(t_list_cmd *cmd)
{
    t_command   *content;

    content = cmd->content;
    close_fds(content->fd_in, ft_lstsize(content->files_in));
    close_fds(content->fd_out, ft_lstsize(content->files_out));
    return (0);
}

int     apply_redirections(t_command *cmd)
{
    apply_redirections_in(cmd);
    apply_redirections_out(cmd);
    return (1);
}

int     apply_redirections_in(t_command *cmd)
{
    char    buf;
    int     i;

    i = 0;
    if (is_redirection_in(cmd))
    {
        while (i < number_of_redirection_in(cmd))
        {
            while(read(cmd->fd_in[i], &buf, 1) > 0)
                write(cmd->flux_in[1], &buf, 1);
            close(cmd->fd_in[i]);
            i++;
        }
        close_fds(cmd->fd_in, number_of_redirection_in(cmd));
        close(cmd->flux_in[1]);
    }
    return (1);
}

int     apply_redirections_out(t_command *cmd)
{
    char    buf;
    int     i;

    if (is_redirection_out(cmd) || is_pipe_out(cmd))
    {
        while(read(cmd->flux_out[0], &buf, 1) > 0)
        {
            i = 0;
            while (i < number_of_redirection_out(cmd))
                write(cmd->fd_out[i++], &buf, 1);
            if (is_pipe_out(cmd))
                write(cmd->pipe[1], &buf, 1);
        }
        close_fds(cmd->fd_out, number_of_redirection_out(cmd));
        close(cmd->flux_out[0]);
        if (is_pipe_out(cmd))
            close(cmd->pipe[1]);
    }
    return (1);
}

int     number_of_redirection_out(t_command *cmd)
{
    return (ft_lstsize(cmd->files_out) + ft_lstsize(cmd->files_append));
}

int     number_of_redirection_in(t_command *cmd)
{
    return (ft_lstsize(cmd->files_in));
}

int     is_redirection_in(t_command *cmd)
{
    return (number_of_redirection_in(cmd) == 0 ? 0 : 1);
}

int     is_redirection_out(t_command *cmd)
{
    return (number_of_redirection_out(cmd) == 0 ? 0 : 1);
}