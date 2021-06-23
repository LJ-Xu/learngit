#pragma once
#include <string>
#ifdef _MSC_VER
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif
class System
{
public:
	static void Init();

	/******************ʱ�����***************************/
	//��ȡ��ǰʱ�������λ��
	static unsigned long long GetCurrentTimeStamp();
	//��ȡ��ǰʱ�������λ����
	static unsigned long long GetCurrentTimeStampMs();
	//��ȡ��Init()���õ���ǰ���ѵ�ʱ�䣬��λ����
	static unsigned long long GetElapseTickMs();
	//��ȡ��ǰ�����գ�ֱ�ӽ���ǰ������д�����
	static void GetCurrentDate(int& year, int& month, int& day, int& week);
	//��ȡ��ǰʱ���룬ֱ�ӽ���ǰ��ʱ��д�����
	static void GetCurrentClock(int& hour, int& minute, int& sec, int& milsec);

	//��ȡ��ǰ���ڵĸ�ʽ���ַ�������ʽ��YYYY-MM-DD
	static std::string GetCurrentDateToString();
	//��ȡ��ǰʱ��ĸ�ʽ���ַ�������ʽ��hh:mm:ss
	static std::string GetCurrentTimeToString();
	//��ȡ��ǰ����ʱ��ĸ�ʽ���ַ�������ʽYYYY-MM-DD hh:mm:ss
	static std::string GetCurrentDateTimeToString();

	//��ȡָ��ʱ��������ڵĸ�ʽ���ַ���,��ʽ��YYYY-MM-DD
	static std::string GetDateToString(time_t time);
	//��ȡָ��ʱ�����ʱ��ĸ�ʽ���ַ�������ʽ��hh:mm:ss
	static std::string GetTimeToString(time_t time);
	//��ȡָ��ʱ���������ʱ��ĸ�ʽ���ַ�������ʽYYYY-MM-DD hh:mm:ss
	static std::string GetDateTimeToString(time_t time);
	//��ȡָ�����ڵĸ�ʽ���ַ�����ʱ���,��ʽ��YYYY-MM-DD
	static time_t GetTimeStampFromDateString(const std::string& str);
	//��ȡָ��ʱ��ĸ�ʽ���ַ�����ʱ�������ʽ��hh:mm:ss
	static time_t GetTimeStampFromTimeString(const std::string& str);
	//��ȡָ������ʱ��ĸ�ʽ���ַ�����ʱ�������ʽYYYY-MM-DD hh:mm:ss
	static time_t GetTimeStampFromDateTimeString(const std::string& str);

	//��ȡ������һ��ڵ�ĺ�����
	static unsigned long long GetNextDayNodeMs();
	//��ȡ������һСʱ�ڵ�ĺ�����
	static unsigned long long GetNextHourNodeMs();
	//��ȡ������һ���ӽڵ�ĺ�����
	static unsigned long long GetNextMinuteNodeMs();
	//��ȡ������һ��ڵ�ĺ�����
	static unsigned long long GetNextSecondNodeMs();

	//����������ȡ��Ӧʱ������
	static tm GetFormatTimeBySec(time_t time);



	/******************�ļ����***************************/
	//��ȡ��ǰִ�г�������Ŀ¼+realpath�ľ���·����
	static char* GetFilePath(const char* realpath,char* abspath);
	//�ж��ļ��Ƿ����
	static bool IsFileExist(const char* file_path);
	//�ж�Ŀ¼�Ƿ����
	static bool IsDirectoryExist(const char* file_path);
	//������·�������ڵ�Ŀ¼
	static bool RenameDirector(const char* src_path, const char* des_path);
	//�������ļ�
	static bool RenameFile(const char* src_file_path, const char* des_file_path);
	//ɾ���ļ�
	static bool RemoveFile(const char* file_path);
	//Ŀ¼����ֱ�ӷ���TURE,���򴴽�
	static bool MakeSureDirectoryExist(const char* dir_path);
	//��ȡ�ļ�·����Ŀ¼
	static std::string GetFileDirectory(const char* file_path);

	static std::string GetCurDir();
	static bool IsAbsPath(const char* path);
	static std::string GetAbsPath(const char* path);
	static std::string CombinPath(std::string dir, std::string name);
	static std::string GetLibExt();

	/******************UI���***************************/
	static int ScreenHeight();
	static int ScreenWidth();
	



private:
	static unsigned long long appStartTick_;
};

