#include "shell.h"

/**
 * my_exit - exits the shell
 * @infos: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: exits with a given exit status
 *         (0) if info.argv[0] != "exit"
 */
int my_exit(info_t *infos)
{
	int exitcheck;

	if (infos->argv[1])  /* If there is an exit arguement */
	{
		exitcheck = _erritoa(infos->argv[1]);
		if (exitcheck == -1)
		{
			infos->status = 2;
			print_error(infos, "Illegal number: ");
			_iputs(infos->argv[1]);
			_iputchar('\n');
			return (1);
		}
		infos->err_num = _erritoa(infos->argv[1]);
		return (-2);
	}
	infos->err_num = -1;
	return (-2);
}

/**
 * my_cd - changes the current directory of the process
 * @infos: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int my_cd(info_t *infos)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		_iputs("TODO: >>getcwd failure emsg here<<\n");
	if (!infos->argv[1])
	{
		dir = get_env(infos, "HOME=");
		if (!dir)
			chdir_ret = /* TODO: what should this be? */
				chdir((dir = get_env(infos, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (_strcmp(infos->argv[1], "-") == 0)
	{
		if (!get_env(infos, "OLDPWD="))
		{
			_iputs(s);
			_iputchar('\n');
			return (1);
		}
		_iputs(get_env(infos, "OLDPWD=")), _iputchar('\n');
		chdir_ret = /* TODO: what should this be? */
			chdir((dir = get_env(infos, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(infos->argv[1]);
	if (chdir_ret == -1)
	{
		print_error(infos, "can't cd to ");
		_iputs(infos->argv[1]), _iputchar('\n');
	}
	else
	{
		_setenv(infos, "OLDPWD", get_env(infos, "PWD="));
		_setenv(infos, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * _myhelp - changes the current directory of the process
 * @infos: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _myhelp(info_t *infos)
{
	char **arg_array;

	arg_array = infos->argv;
	_iputs("help call works. Function not yet implemented \n");
	if (0)
		_iputs(*arg_array); /* temp att_unused workaround */
	return (0);
}
