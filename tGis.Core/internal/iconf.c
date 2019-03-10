#include "iconf.h"

int my_isspace(char c)
{
	if (c == '\t' || c == '\n' || c == '\r' || c == ' ')
		return 1;
	else
		return 0;
}

/*
** 去除字符串两端空格
** 返回的是指向字符串内第一个不为空格字符的指针
** 对于动态申请的字符串，必须保留原始指针用于释放内存
*/
char* my_strtrim(char *pstr)
{
	size_t i = 0;
	size_t j = strlen(pstr) - 1;

	while ((j >= 0) && my_isspace(pstr[j]))
		pstr[j--] = '\0';

	while ((i <= j) && my_isspace(pstr[i]))
		i++;

	return pstr + i;
}

const char* get_conf_str(const conf_session_t *session, const char *key)
{
	conf_item_t *item = NULL;

	assert(session != NULL);
	assert(key != NULL);

	item = session->head;

	while (item != NULL)
	{
		if (strcmp(key, item->key) == 0)
			return item->value;

		item = item->next;
	}

	return NULL;
}

int get_conf_int(const conf_session_t *session, const char *key, const int dv)
{
	conf_item_t *item = NULL;

	assert(session != NULL);
	assert(key != NULL);

	item = session->head;

	while (item != NULL)
	{
		if (strcmp(key, item->key) == 0)
			return atoi(item->value);

		item = item->next;
	}

	return dv;
}

long  get_conf_long(const conf_session_t *session, const char *key, const long dv)
{
	conf_item_t *item = NULL;

	assert(session != NULL);
	assert(key != NULL);

	item = session->head;

	while (item != NULL)
	{
		if (strcmp(key, item->key) == 0)
			return atol(item->value);

		item = item->next;
	}

	return dv;
}

int is_session_line(char *line, const conf_session_t *session)
{
	char *p = my_strtrim(line);
	size_t len = strlen(p);
	if ((len - strlen(session->name)) != 2) return 0;
	if (p[0] == '[' && p[len - 1] == ']'	&& strncmp(p + 1, session->name, len - 2) == 0)
		return 1;
	else
		return 0;
}

/*
*从配置文件的一行读出key或value,返回conf_item_t指针
*line--从配置文件读出的一行
*/
int  get_item_from_line(char *line, conf_item_t *item)
{
	char *p = my_strtrim(line);
	size_t len = strlen(p);

	if (len == 0) return -4;/*空行*/
	else if (p[0] == ';') return -5;/*注释*/
	else if (p[0] == '[') return -3;/*session*/
	else
	{
		char *p2 = strchr(p, '=');
		if (p2 == NULL)	return 0;
		*p2 = '\0';
		p2++;
		p2 = my_strtrim(p2);
		p = my_strtrim(p);

		item->key = (char *)malloc(strlen(p) + 1);
		if (item->key == NULL) return -2;

		item->value = (char *)malloc(strlen(p2) + 1);
		if (item->value == NULL)
		{
			free(item->key);
			return -2;
		}

		strcpy(item->key, p);
		strcpy(item->value, p2);

		return 1;/*成功*/
	}
}

int read_conf_session(const char *file, conf_session_t *session)
{
	char line[1024];
	int i = 0;
	conf_item_t **cur = NULL;
	conf_item_t *item = NULL;
	FILE *fp;

	assert(file != NULL);
	assert(session != NULL);
	assert(session->name != NULL);

	cur = &session->head;

	fp = fopen(file, "r");
	if (fp == NULL)
		return -1;/*文件打开错误*/

	while (fgets(line, 1023, fp))
	{
		if (is_session_line(line, session))
		{
			while (fgets(line, 1023, fp))
			{
				if (item == NULL)
				{
					item = (conf_item_t*)malloc(sizeof(conf_item_t));
					if (item == NULL)
					{
						fclose(fp);
						return -2;/*内存申请失败*/
					}
					item->next = NULL;
				}

				i = get_item_from_line(line, item);

				if (i == 1)
				{
					*cur = item;
					cur = &item->next;
					item = NULL;
				}
				else if (i == -2)/*如果内存申请失败*/
				{
					free(item);
					fclose(fp);
					return -2;/*内存申请失败*/
				}
				else if (i == -3)/*如果遇到下一个session*/
					break;
			}
			/*如果遇到下一个session
			或者空行，或者遇到文件结尾*/
			free(item);
			fclose(fp);
			return 1;/*成功*/
		}
	}

	fclose(fp);
	return 0;/*没有找到session*/
}

void free_conf_session(conf_session_t *session)
{
	conf_item_t *item = NULL;

	assert(session != NULL);

	while (session->head != NULL)
	{
		item = session->head;
		session->head = item->next;
		free(item->key);
		free(item->value);
		free(item);
	}
}

