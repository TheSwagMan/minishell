/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotier <tpotier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/04 17:43:49 by tpotier           #+#    #+#             */
/*   Updated: 2020/03/09 17:01:18 by tpotier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_blank(char c)
{
	return (c == ' ' || c == '\t');
}

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

char		*path_cat(char *dir, char *file)
{
	char	*res;

	if (!(res = malloc(ft_strlen(dir) + ft_strlen(file) + 2)))
		return (NULL);
	*res = '\0';
	ft_strcat(res, dir);
	ft_strcat(res, "/");
	ft_strcat(res, file);
	return (res);
}

char	*get_executable(char *name)
{
	char	*pth;
	t_lst	*path;
	char	*p;
	int		n;

	path = NULL;
	n = 0;
	while (environ[n])
	{
		if (ft_strncmp("PATH=", environ[n], 5) == 0)
		{
			pth = ft_strdup(environ[n] + 5);
			p = pth;
			lst_append(&path, p);
			while ((p = ft_strchr(p, ':')))
			{
				*p = '\0';
				lst_append(&path, ++p);
			}
			lst_goto_n(&path, 0);
			while (path)
			{
				p = path_cat(path->data, name);
				if (access(p, X_OK) == 0)
					return (p);
				free(p);
				path = path->next;
			}
		}
		n++;
	}
	return (NULL);
}

void	prompt_show(void)
{
	ft_putstr("$> ");
}

void	handle_sigint(int sig)
{
	(void)sig;
	ft_putstr("\n");
	prompt_show();
}

t_lst	*split_command(char *cmd)
{
	t_lst	*l;

	l = NULL;
	while (*cmd)
	{
		while (*cmd && is_blank(*cmd))
			*(cmd++) = '\0';
		lst_append(&l, cmd);
		while (*cmd && !is_blank(*cmd))
			cmd++;
	}
	return (l);
}

void	disp_lst(t_lst *l)
{
	lst_goto_n(&l, 0);
	while (l)
	{
		ft_putendl(l->data);
		l = l->next;
	}
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

void	exec_app(t_lst *app)
{
	char	**argv;
	char	*executable;
	int		n;

	n = 0;
	if (!(argv = malloc(sizeof(*argv) * (1 + lst_size(app)))))
		return ;
	lst_goto_n(&app, 0);
	while (app)
	{
		argv[n++] = app->data;
		app = app->next;
	}
	argv[n] = NULL;
	executable = get_executable(argv[0]);
	if (fork() == 0)
	{
		execve(executable, argv, environ);
		ft_putendl("Error");
		exit(0);
	}
	else
		wait(NULL);
	free(argv);
}

int		main(int ac, char **av)
{
	char	*cmd;

	(void)ac;
	(void)av;
	signal(SIGINT, handle_sigint);
	while (1)
	{
		prompt_show();
		cmd = get_next_command();
		if (!cmd)
		{
			ft_putchar('\n');
			return (0);
		}
		if (ft_strcmp(cmd, "exit") == 0)
			return (0);
		else
			exec_app(split_command(cmd));
		free(cmd);
	}
	return (0);
}
