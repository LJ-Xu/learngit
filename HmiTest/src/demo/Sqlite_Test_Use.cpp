#include "Sqlite_Test_Use.h"
#include "sqlite3.h"
#include <iostream>  
#include <string>  
using namespace std;

Sqlite_Test_Use& Sqlite_Test_Use::Ins()
{
	Sqlite_Test_Use ins;
	return ins;
}

bool Sqlite_Test_Use::Run(int argc, char ** argv)
{
	
	sqlite3* db; 
	int nResult = sqlite3_open("test.db", &db);
	if (nResult != SQLITE_OK)
	{
		cout << "�����ݿ�ʧ�ܣ�" << sqlite3_errmsg(db) << endl;
		return 0;
	}
	else
	{
		cout << "���ݿ�򿪳ɹ�" << endl;
	}
	char* errmsg;
	nResult = sqlite3_exec(db, "create table fuck(id integer primary key autoincrement,name varchar(100))", NULL, NULL, &errmsg);
	if (nResult != SQLITE_OK)
	{
		sqlite3_close(db);
		cout << "������ʧ�ܣ�" << sqlite3_errmsg(db) << endl;
		return 0;
	}
	string strSql;
	for (int i = 0;i < 100;i++)
	{
		strSql += "insert into fuck values(null,'heh');";
	}
	cout << strSql << endl;
	nResult = sqlite3_exec(db, strSql.c_str(), NULL, NULL, &errmsg);
	if (nResult != SQLITE_OK)
	{
		sqlite3_close(db);
		cout << "��������ʧ�ܣ�" << sqlite3_errmsg(db) << endl;
		return 0;
	}
	return true;
}