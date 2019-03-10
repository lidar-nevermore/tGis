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
	** 从配置文件中读取一个节的信息，并返回是否成功。
	**
	**     第一个参数表示文件的完整路径，路径只能由ASCII字符组成。
	**     第二个参数为指向conf_session_t的指针，作为输入和输出参数。无论该方法返回成功和失败都需要
	** 对传入的session调用free_conf_session。
	**     返回1-执行成功，0-没有找到名称为session->name的session，-1-文件打开失败，-2-内存错误。
	*/
	int read_conf_session(const char *file,conf_session_t *session);


	/*
	** 释放conf_session_t占用的资源。
	*/
	void free_conf_session(conf_session_t *session);


	/*
	** 从配置节中获取long类型的值，如果获取失败将会dv返回。
	**
	**     第一个参数session，必须不为NULL。为经read_conf_session初始化的session传入后会导致默认值
	** dv，被返回。
	**     第二个参数key，表示配置项的名称，必须不为NULL。
	**     第三个参数dv，作为配置项key的默认值。如果获取配置项为key的值失败，将会返回dv。
	*/
	long get_conf_long(const conf_session_t *session,const char *key, const long dv);


	/*
	** 从配置节中获取int类型的值，如果获取失败将会dv返回。
	**
	**     第一个参数session，必须不为NULL。为经read_conf_session初始化的session传入后会导致默认值
	** dv，被返回。
	**     第二个参数key，表示配置项的名称，必须不为NULL。
	**     第三个参数dv，作为配置项key的默认值。如果获取配置项为key的值失败，将会返回dv。
	*/
	int get_conf_int(const conf_session_t *session,const char *key, const int dv);


	/*
	** 从配置节中获取字符串，如果获取失败将会返回NULL。
	**
	**     第一个参数session，必须不为NULL。为经read_conf_session初始化的session传入后会导致默认值
	** dv，被返回。
	**     第二个参数key，表示配置项的名称，必须不为NULL。
	**     此方法直接返回的是指向session中key对应的value的指针，其值不能修改。也不能对其调用free函
	** 数。
	*/
	const char* get_conf_str(const conf_session_t *session,const char *key);


#ifdef __cplusplus
}  /* end of the 'extern "C"' block */
#endif


#endif

