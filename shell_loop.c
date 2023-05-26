#include "shell.h"

/**
 * hsh - main shell loop
 * @infos: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int hsh(info_t *infos, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		clear_info(infos);
		if (interactive_mode(infos))
			_puts("$ ");
		_iputchar(BUF_FLUSH);
		r = get_input(infos);
		if (r != -1)
		{
			set_info(infos, av);
			builtin_ret = find_builtin(infos);
			if (builtin_ret == -1)
				find_cmd(infos);
		}
		else if (interactive_mode(infos))
			_putchar('\n');
		free_info(infos, 0);
	}
	write_history(infos);
	free_info(infos, 1);
	if (!interactive_mode(infos) && infos->status)
		exit(infos->status);
	if (builtin_ret == -2)
	{
		if (infos->err_num == -1)
			exit(infos->status);
		exit(infos->err_num);
	}
	return (builtin_ret);
}

/**
 * find_builtin - finds a builtin command
 * @infos: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int find_builtin(info_t *infos)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", my_exit},
		{"env", my_env},
		{"help", _myhelp},
		{"history", my_history},
		{"setenv", my_setenv},
		{"unsetenv", my_nonsetenv},
		{"cd", my_cd},
		{"alias", my_alias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (_strcmp(infos->argv[0], builtintbl[i].type) == 0)
		{
			infos->line_count++;
			built_in_ret = builtintbl[i].func(infos);
			break;
		}
	return (built_in_ret);
}

/**
 * find_cmd - finds a command in PATH
 * @infos: the parameter & return info struct
 *
 * Return: void
 */
void find_cmd(info_t *infos)
{
	char *path = NULL;
	int i, k;

	infos->path = infos->argv[0];
	if (infos->linecount_flag == 1)
	{
		infos->line_count++;
		infos->linecount_flag = 0;
	}
	for (i = 0, k = 0; infos->arg[i]; i++)
		if (!_isdelim(infos->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = find_path(infos, get_env(infos, "PATH="), infos->argv[0]);
	if (path)
	{
		infos->path = path;
		fork_cmd(infos);
	}
	else
	{
		if ((interactive_mode(infos) || get_env(infos, "PATH=")
			|| infos->argv[0][0] == '/') && is_cmd(infos, infos->argv[0]))
			fork_cmd(infos);
		else if (*(infos->arg) != '\n')
		{
			infos->status = 127;
			print_error(infos, "not found\n");
		}
	}
}

/**
 * fork_cmd - forks a an exec thread to run cmd
 * @infos: the parameter & return info struct
 *
 * Return: void
 */
void fork_cmd(info_t *infos)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(infos->path, infos->argv, get_environ(infos)) == -1)
		{
			free_info(infos, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(infos->status));
		if (WIFEXITED(infos->status))
		{
			infos->status = WEXITSTATUS(infos->status);
			if (infos->status == 126)
				print_error(infos, "Permission denied\n");
		}
	}
}
