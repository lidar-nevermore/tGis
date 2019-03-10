#include "iconf.h"

int my_isspace(char c)
{
	if (c == '\t' || c == '\n' || c == '\r' || c == ' ')
		return 1;
	else
		return 0;
}

/*
** ȥ���ַ������˿ո�
** ���ص���ָ���ַ����ڵ�һ����Ϊ�ո��ַ���ָ��
** ���ڶ�̬������ַ��������뱣��ԭʼָ�������ͷ��ڴ�
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
*�������ļ���һ�ж���key��value,����conf_item_tָ��
*line--�������ļ�������һ��
*/
int  get_item_from_line(char *line, conf_item_t *item)
{
	char *p = my_strtrim(line);
	size_t len = strlen(p);

	if (len == 0) return -4;/*����*/
	else if (p[0] == ';') return -5;/*ע��*/
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

		return 1;/*�ɹ�*/
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
		return -1;/*�ļ��򿪴���*/

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
						return -2;/*�ڴ�����ʧ��*/
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
				else if (i == -2)/*����ڴ�����ʧ��*/
				{
					free(item);
					fclose(fp);
					return -2;/*�ڴ�����ʧ��*/
				}
				else if (i == -3)/*���������һ��session*/
					break;
			}
			/*���������һ��session
			���߿��У����������ļ���β*/
			free(item);
			fclose(fp);
			return 1;/*�ɹ�*/
		}
	}

	fclose(fp);
	return 0;/*û���ҵ�session*/
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

