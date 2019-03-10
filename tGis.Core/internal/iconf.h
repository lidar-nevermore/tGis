#ifndef __ICONF_H__
#define __ICONF_H__

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct __conf_item_t
	{
		char *key;
		char *value;
		struct __conf_item_t *next;
	}
	conf_item_t;

	typedef struct __conf_session_t
	{
		const char *name;
		conf_item_t *head;
	}
	conf_session_t;


	/*
	** �������ļ��ж�ȡһ���ڵ���Ϣ���������Ƿ�ɹ���
	**
	**     ��һ��������ʾ�ļ�������·����·��ֻ����ASCII�ַ���ɡ�
	**     �ڶ�������Ϊָ��conf_session_t��ָ�룬��Ϊ�����������������۸÷������سɹ���ʧ�ܶ���Ҫ
	** �Դ����session����free_conf_session��
	**     ����1-ִ�гɹ���0-û���ҵ�����Ϊsession->name��session��-1-�ļ���ʧ�ܣ�-2-�ڴ����
	*/
	int read_conf_session(const char *file,conf_session_t *session);


	/*
	** �ͷ�conf_session_tռ�õ���Դ��
	*/
	void free_conf_session(conf_session_t *session);


	/*
	** �����ý��л�ȡlong���͵�ֵ�������ȡʧ�ܽ���dv���ء�
	**
	**     ��һ������session�����벻ΪNULL��Ϊ��read_conf_session��ʼ����session�����ᵼ��Ĭ��ֵ
	** dv�������ء�
	**     �ڶ�������key����ʾ����������ƣ����벻ΪNULL��
	**     ����������dv����Ϊ������key��Ĭ��ֵ�������ȡ������Ϊkey��ֵʧ�ܣ����᷵��dv��
	*/
	long get_conf_long(const conf_session_t *session,const char *key, const long dv);


	/*
	** �����ý��л�ȡint���͵�ֵ�������ȡʧ�ܽ���dv���ء�
	**
	**     ��һ������session�����벻ΪNULL��Ϊ��read_conf_session��ʼ����session�����ᵼ��Ĭ��ֵ
	** dv�������ء�
	**     �ڶ�������key����ʾ����������ƣ����벻ΪNULL��
	**     ����������dv����Ϊ������key��Ĭ��ֵ�������ȡ������Ϊkey��ֵʧ�ܣ����᷵��dv��
	*/
	int get_conf_int(const conf_session_t *session,const char *key, const int dv);


	/*
	** �����ý��л�ȡ�ַ����������ȡʧ�ܽ��᷵��NULL��
	**
	**     ��һ������session�����벻ΪNULL��Ϊ��read_conf_session��ʼ����session�����ᵼ��Ĭ��ֵ
	** dv�������ء�
	**     �ڶ�������key����ʾ����������ƣ����벻ΪNULL��
	**     �˷���ֱ�ӷ��ص���ָ��session��key��Ӧ��value��ָ�룬��ֵ�����޸ġ�Ҳ���ܶ������free��
	** ����
	*/
	const char* get_conf_str(const conf_session_t *session,const char *key);


#ifdef __cplusplus
}  /* end of the 'extern "C"' block */
#endif


#endif

