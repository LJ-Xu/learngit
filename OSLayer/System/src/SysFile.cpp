#include "SysFile.h"
#include <stdio.h>
#include <sys/stat.h>
#ifdef _MSC_VER
#include <windows.h>
#include <ImageHlp.h>
#pragma comment(lib, "imagehlp.lib")
#else
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#endif
namespace base
{


	file::file() 
		: fp_(nullptr)
		, path_("")
	{

	}

	file::~file()
	{
		close();
	}

	bool file::open(const char* file_path, const char* open_mode)
	{
		close();
		fp_ = ::fopen(file_path, open_mode);
		if (null()) return false;
		path_ = file_path;
		return true;
	}

	void file::close()
	{
		if (fp_ != nullptr)
		{
			::fclose(fp_);
			fp_ = nullptr;
		}
	}

	size_t file::file_size()
	{
		struct stat stat;
		if (::stat(path_.c_str(), &stat) == 0)
		{
			return stat.st_size;
		}
		return 0;
	}

	int file::seek(int pos, FILE_SEEK_TYPE type)
	{
		int seek_type;
		switch (type) {
		case FILE_SEEK_CUR:
			seek_type = SEEK_CUR;
			break;
		case FILE_SEEK_END:
			seek_type = SEEK_END;
			break;
		case FILE_SEEK_SET:
		default:
			seek_type = SEEK_SET;
		}

		int ret = (int)fseek(fp_, pos, seek_type);
		if (ret >= 0) return 0;

		return -1;
	}

	bool file::null() const
	{
		return fp_ == nullptr;
	}

	int file::read(char* buf, size_t size)
	{
		return ::fread(buf, 1, size, fp_);
	}

	int file::write(const char* buf, size_t size)
	{
		return ::fwrite(buf, 1, size, fp_);
	}

	void file::flush()
	{
		::fflush(fp_);
	}

	void file::read_all(std::string &data)
	{
		uint32_t size = file_size();
		if (size > 0)
		{
			data.resize(size);
			read(&data[0], size);
		}
	}

	bool file::is_file_exist(const char* file_path)
	{
#ifdef _MSC_VER
		//wchar_t ws[512];
		//swprintf(ws, 512, L"%hs", file_path);
		DWORD attr = GetFileAttributes(file_path);
		if (attr != INVALID_FILE_ATTRIBUTES &&
			attr != FILE_ATTRIBUTE_DIRECTORY)
		{
			return true;
		}
#else
		struct stat stat;
		if (lstat(file_path, &stat) == 0)
		{
			return S_ISREG(stat.st_mode);
		}
#endif

		return false;
	}

	bool file::is_directory_exist(const char* file_path)
	{
#ifdef _MSC_VER
		//wchar_t ws[512];
		//swprintf(ws, 512, L"%hs", file_path);
		DWORD attr = GetFileAttributes(file_path);
		if (attr != INVALID_FILE_ATTRIBUTES &&
			attr == FILE_ATTRIBUTE_DIRECTORY)
		{
			return true;
		}
#else
		struct stat stat;
		if (lstat(file_path, &stat) == 0)
		{
			return S_ISDIR(stat.st_mode);
		}
#endif

		return false;
	}

	bool file::rename_director(const char* src_path, const char* des_path)
	{
		if (!is_directory_exist(src_path))
		{
			return false;
		}
		return (rename(src_path, des_path) == 0);
	}

	bool file::rename_file(const char* src_file_path, const char* des_file_path)
	{
		if (!is_file_exist(src_file_path))
		{
			return false;
		}
		return (rename(src_file_path, des_file_path) == 0);
	}

	bool file::remove_file(const char* file_path)
	{
		return (remove(file_path) == 0);
	}

	bool file::make_sure_directory_exist(const char* dir_path)
	{
		if (is_directory_exist(dir_path))
		{
			return true;
		}
#ifdef _MSC_VER
		return (MakeSureDirectoryPathExists(dir_path) == TRUE);
#else
		if (make_dirs(dir_path) == 0)
		{
			return true;
		}
#endif

		return false;
	}

	std::string file::file_directory(const char* file_path)
	{
		std::string dir(file_path);
		int pos = -1;
#ifdef _MSC_VER
		pos = dir.rfind('\\');
#else
		pos = dir.rfind('/');
#endif
		if (pos >= 0)
		{
			return dir.substr(0, pos + 1);
		}

		return "";
	}
#ifndef _MSC_VER
	int file::make_dirs(const char* dir_path)
	{
		char *saved_path, *cp;
		int saved_ch;
		struct stat st;
		int rc;

		cp = saved_path = strdup(dir_path);
		while (*cp && *cp == '/') {
			++cp;
		}
		while (1)
		{
			while (*cp && *cp != '/') ++cp;
			if ((saved_ch = *cp) != 0) {
				*cp = 0;
			}
			if ((rc = stat(saved_path, &st)) >= 0) {
				if (!S_ISDIR(st.st_mode)) {
					errno = ENOTDIR;
					rc = -1;
					break;
				}
			}
			else {
				if (errno != ENOENT) {
					break;
				}

				if ((rc = mkdir(saved_path, 0755)) < 0) {
					if (errno != EEXIST)
						break;

					if ((rc = stat(saved_path, &st)) < 0)
						break;

					if (!S_ISDIR(st.st_mode)) {
						errno = ENOTDIR;
						rc = -1;
						break;
					}
				}
			}

			if (saved_ch != 0) {
				*cp = saved_ch;
			}

			while (*cp && *cp == '/') {
				++cp;
			}
			if (*cp == 0) {
				break;
			}
		}

		free(saved_path);
		return rc;
	}
#endif
}

