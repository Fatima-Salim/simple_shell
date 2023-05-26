#include "shell.h"

/**
 * my_env - prints the current environment
 * @infos: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int my_env(info_t *infos)
{
	print_list_str(infos->env);
	return (0);
}

/**
 * get_env - gets the value of an environ variable
 * @infos: Structure containing potential arguments. Used to maintain
 * @name: env var name
 *
 * Return: the value
 */
char *get_env(info_t *infos, const char *name)
{
	list_t *node = infos->env;
	char *p;

	while (node)
	{
		p = starts_with(node->str, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}

/**
 * my_setenv - Initialize a new environment variable,
 *             or modify an existing one
 * @infos: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int my_setenv(info_t *infos)
{
	if (infos->argc != 3)
	{
		_iputs("Incorrect number of arguements\n");
		return (1);
	}
	if (_setenv(infos, infos->argv[1], infos->argv[2]))
		return (0);
	return (1);
}

/**
 * my_nonsetenv - Remove an environment variable
 * @infos: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int my_nonsetenv(info_t *infos)
{
	int i;

	if (infos->argc == 1)
	{
		_iputs("Too few arguements.\n");
		return (1);
	}
	for (i = 1; i <= infos->argc; i++)
		_nonsetenv(infos, infos->argv[i]);

	return (0);
}

/**
 * populate_env_list - populates env linked list
 * @infos: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int populate_env_list(info_t *infos)
{
	list_t *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		add_node_end(&node, environ[i], 0);
	infos->env = node;
	return (0);
}
