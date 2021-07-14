/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : BaseStorageService.h
 * Author   : TangYao
 * Date     : 2021/02/04
 * Descript : 数据库操作接口
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#ifndef __BASESTORAGESERVICE__
#define __BASESTORAGESERVICE__

#include <iostream>
#include "sqlite3.h"
#include "RunEnv.h"
#define SQLCMDLEN 512//sql语句最大长度
#define BINCOUNT 10//每个sql能拥有的BIN文件数
#define BINSIZE  1000//每个bin文件的最大条目数
#define MAXCOUNT 10000//内存数据库最大条目数
#define FLUSHTIMEOUT 600000//两次FLUSH到Bin文件行为的最大间隔时间
using namespace std;

namespace Storage
{
	class BaseStorageService
	{
	protected:
		BaseStorageService(const string & dbPath, const string & tbName, int maxCnt = 10000);
		virtual ~BaseStorageService();

	protected:
		// 打开数据库
		int Open();
		// 关闭数据库
		void Close();
		// 执行sql语句
		int ExecuteSql(const char * sql);
		// 创建内存数据库
		virtual int Create() = 0;
		// 附加文件数据库
		int Attach();

	public:
		// 备份至文件数据库
		int Flush();
		// 备份指定数量数据
		int Flush(int count);
		// 获取内存中数据数量
		int Count();

	protected:
		sqlite3 * db;	// 数据库句柄
		string dbPath;	// 数据库路径
		string tbName;	// 数据表名称
		bool isOpened;	// 是否打开数据库
	};
}

#endif // !__BASESTORAGESERVICE__

