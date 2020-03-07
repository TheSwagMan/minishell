/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotier <tpotier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/04 17:43:49 by tpotier           #+#    #+#             */
/*   Updated: 2020/03/04 18:20:07 by tpotier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		strcat_realloc(char **str, char *cat)
{
	char	*tmp;
	int		size;

	if (!str)
		return (0);
	size = 0;
	if (*str)
		size = ft_strlen(*str);
	if (!(tmp = malloc(size + ft_strlen(cat) + 1)))
		return (0);
	*tmp = 0;
	if (*str)
	{
		ft_strcpy(tmp, *str);
		free(*str);
	}
	ft_strcat(tmp, cat);
	*str = tmp;
	return (1);
}

void	handle_sigint(int sig)
{
	(void)sig;
	ft_putstr("\n$> ");
}

char	*get_next_command(void)
{
	char	buff[BUFFER_SIZE + 1];
	static char	*rest = NULL;
	char		*cmd;
	int			n;
	char		*end;

	cmd = NULL;
	while (rest || (n = read(STDIN_FILENO, buff, BUFFER_SIZE)) > 0)
	{
		if (rest)
			ft_strcpy(buff, rest);
		else
			buff[n] = 0;
		free(rest);
		rest = NULL;
		end = ft_strchr(buff, ';');
		end = end ? end : ft_strchr(buff, '\n');
		if (end)
			*end = '\0';
		strcat_realloc(&cmd, buff);
		if (end && *end && *(++end))
			rest = ft_strdup(end);
		break;
	}
	return (cmd);
}

void	exec_app(char *app)
{
	char* argv[] = {app, NULL};
	char* envp[] = {"ok", NULL };
	(void)app;
	if (fork() == 0)
	{
		execve(app, argv, envp);
		ft_putendl("Error");
		exit(0);
	}
	else
		wait(NULL);
}

int		main(int ac, char **av)
{
	char	*cmd;

	(void)ac;
	(void)av;
	signal(SIGINT, handle_sigint);
	while (1)
	{
		ft_putstr("$> ");
		cmd = get_next_command();
		if (!cmd)
		{
			ft_putchar('\n');
			return (0);
		}
		if (ft_strcmp(cmd, "exit") == 0)
			return (0);
		else
			exec_app(cmd);
		free(cmd);
	}
	return (0);
}
