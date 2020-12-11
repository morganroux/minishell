/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroux <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/09 12:35:37 by mroux             #+#    #+#             */
/*   Updated: 2019/11/14 19:27:48 by mroux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <unistd.h>
# include <stdlib.h>
# include "libftprintf.h"
# include "get_next_line.h"

typedef struct	        s_command
{
	char		        *cmd;
    int                 *argc;
	char		        **argv;
	int			        *fd_in;
	int			        *fd_out;
    int                 *fd_err;
    struct s_command    *next;
}				        t_command;

//Parsing
t_command               *parse();

//Exec
int                     exec(t_command *cmds);

//Utils
void                    print_strs(char **strs);
#endif
