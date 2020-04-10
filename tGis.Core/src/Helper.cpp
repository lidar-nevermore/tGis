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

	_tgis_get_next(m, next, m_len);

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

TGIS_CORE_API char * _tgis_str_trim(char * src)
{
	char *ori_src = src;
	char *begin = src;
	char *end = src;

	while (*end++);

	if (begin == end) return ori_src;

	while (*begin == ' ' || *begin == '\t')
		++begin;
	while ((*end) == '\0' || *end == ' ' || *end == '\t')
		--end;

	if (begin > end) {
		*src = '\0';
		return ori_src;
	}

	while (begin != end) {
		*src++ = *begin++;
	}

	*src++ = *end;
	*src = '\0';

	return ori_src;
}

#if defined(_MSC_VER) || defined(__MINGW32__)

TGIS_CORE_API void _tgis_get_exe_dir(char * dir, int * size)
{
	int sz = size == nullptr ? TGIS_MAX_PATH : *size;
	::GetModuleFileNameA(NULL, (LPSTR)dir, sz);
	int sep = _tgis_find_last_of(dir, TGIS_PATH_SEPARATOR_STR, 0);
	dir[sep] = '\0';
	if(size != nullptr)
	    *size = sep;
}

TGIS_CORE_API void _tgis_traverse_dir(const char * dir, const char* pat, void * usr, _tgis_on_traverse on_traverse)
{
	_finddata_t file;
	intptr_t handle;
	intptr_t flag;
	char path[TGIS_MAX_PATH] = { 0 };
	strcpy(path, dir);
	strcat(path, TGIS_PATH_SEPARATOR_STR);
	strcat(path, pat);

	flag = handle = _findfirst(path, &file);
	while (flag != -1)
	{
		if (strcmp(file.name, ".") != 0
			&& strcmp(file.name, "..") != 0)
		{
			unsigned int attrib = 0;
			if (file.attrib&_A_SUBDIR)
				attrib = attrib | _TGIS_A_SUBDIR;
			if (file.attrib&_A_NORMAL)
				attrib = attrib | _TGIS_A_NORMAL;
			if (file.attrib&_A_SYSTEM)
				attrib = attrib | _TGIS_A_SYSTEM;
			if (file.attrib&_A_HIDDEN)
				attrib = attrib | _TGIS_A_HIDDEN;
			if (file.attrib&_A_RDONLY)
				attrib = attrib | _TGIS_A_RDONLY;

			on_traverse(usr, dir, file.name, attrib);
		}

		flag = _findnext(handle, &file);
	}

	_findclose(handle);
}

#endif

