#include "minishell.h"

void    close_descriptors(int num_fd, ...)
{
    va_list fds;
    int     i;
    int     curr_fd;

    i = -1;
    va_start(fds, num_fd);
    while (++i < num_fd)
    {
        curr_fd = va_arg(fds, int);
        close(curr_fd);
    }
    va_end(fds);
}