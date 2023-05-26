#include "shell.h"

/**
 * get_environ - returns the string array copy of our environ
 * @infos: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
char **get_environ(info_t *infos)
{
	if (!infos->environ || infos->env_changed)
	{
		infos->environ = list_to_strings(infos->env);
		infos->env_changed = 0;
	}

	return (infos->environ);
}

/**
 * _nonsetenv - Remove an environment variable
 * @infos: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: 1 on delete, 0 otherwise
 * @var: the string env var property
 */
int _nonsetenv(info_t *infos, char *var)
{
	list_t *node = infos->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = starts_with(node->str, var);
		if (p && *p == '=')
		{
			infos->env_changed = delete_node_at_index(&(infos->env), i);
			i = 0;
			node = infos->env;
			continue;
		}
		node = node->next;
		i++;
	}
	return (infos->env_changed);
}

/**
 * _setenv - Initialize a new environment variable,
 *             or modify an existing one
 * @infos: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string env var property
 * @value: the string env var value
 *  Return: Always 0
 */
int _setenv(info_t *infos, char *var, char *value)
{
	char *buf = NULL;
	list_t *node;
	char *p;

	if (!var || !value)
		return (0);

	buf = malloc(_strlen(var) + _strlen(value) + 2);
	if (!buf)
		return (1);
	_strcpy(buf, var);
	_strcat(buf, "=");
	_strcat(buf, value);
	node = infos->env;
	while (node)
	{
		p = starts_with(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buf;
			infos->env_changed = 1;
			return (0);
		}
		node = node->next;
	}
	add_node_end(&(infos->env), buf, 0);
	free(buf);
	infos->env_changed = 1;
	return (0);
}
