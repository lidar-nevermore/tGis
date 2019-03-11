#include "Helper.h"

void _tgis_get_next(const char* m, int* next, int len)
{
	int i = -1, j = 0, n = 0;
	int m_inx = len - 1;

	for (n = 0; n < len; n++)
		next[n] = -1;

	while (j < m_inx)
	{
		if ((i < 0) || (m[i] == m[j]))
		{
			i++; j++;
			if (m[i] != m[j])
				next[j] = i;
			else  next[j] = next[i];
		}
		else
			i = next[i];
	}
}

TGIS_CORE_API int _tgis_find_first_of(const char* s, const char* m, int offset)
{
	int j = offset, i = 0;//这里的串的第1个元素下标是0
	int s_len = (int)strlen(s);
	int m_len = (int)strlen(m);

	if ((s_len - offset) < m_len)
		return -1;

	int* next = (int*)malloc(m_len * sizeof(int));

	_tgis_get_next(m, next,m_len);

	while (i<m_len && j<s_len)
	{
		if (i < 0 || s[j] == m[i])
		{
			++i;
			++j;
		}
		else i = next[i];//j不变,i跳动
	}

	free(next);
	if (i == m_len) return j - i;//匹配成功
	else return -1;
}

TGIS_CORE_API int _tgis_find_last_of(const char* s, const char* m, int offset)
{
	int j = offset, i = 0;//这里的串的第1个元素下标是0
	int s_len = (int)strlen(s);
	int m_len = (int)strlen(m);
	int last_inx = -1;

	if ((s_len - offset) < m_len)
		return -1;

	int* next = (int*)malloc(m_len * sizeof(int));

	_tgis_get_next(m, next, m_len);

	while (true)
	{
		while (i < m_len && j < s_len)
		{
			if (i < 0 || s[j] == m[i])
			{
				++i;
				++j;
			}
			else i = next[i];//j不变,i跳动
		}

		if (i == m_len)
		{
			last_inx = j - i;//匹配成功
			j = last_inx + 1;
			i = 0;
		}
		else 
			break;
	}

	free(next);

	return last_inx;
}
