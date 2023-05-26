#include "shell.h"

/**
 * my_history - displays the history list, one command by line, preceded
 *              with line numbers, starting at 0.
 * @infos: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int my_history(info_t *infos)
{
	print_list(infos->history);
	return (0);
}

/**
 * nonset_alias - sets alias to string
 * @infos: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int nonset_alias(info_t *infos, char *str)
{
	char *p, c;
	int ret;

	p = _strchr(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = delete_node_at_index(&(infos->alias),
		get_node_index(infos->alias, node_starts_with(infos->alias, str, -1)));
	*p = c;
	return (ret);
}

/**
 * set_alias - sets alias to string
 * @infos: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int set_alias(info_t *infos, char *str)
{
	char *p;

	p = _strchr(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (nonset_alias(infos, str));

	nonset_alias(infos, str);
	return (add_node_end(&(infos->alias), str, 0) == NULL);
}

/**
 * print_alias - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int print_alias(list_t *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = _strchr(node->str, '=');
		for (a = node->str; a <= p; a++)
			_putchar(*a);
		_putchar('\'');
		_puts(p + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * my_alias - mimics the alias builtin (man alias)
 * @infos: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int my_alias(info_t *infos)
{
	int i = 0;
	char *p = NULL;
	list_t *node = NULL;

	if (infos->argc == 1)
	{
		node = infos->alias;
		while (node)
		{
			print_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; infos->argv[i]; i++)
	{
		p = _strchr(infos->argv[i], '=');
		if (p)
			set_alias(infos, infos->argv[i]);
		else
			print_alias(node_starts_with(infos->alias, infos->argv[i], '='));
	}

	return (0);
}
