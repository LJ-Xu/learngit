#pragma once

#include <string>

namespace base
{
//"r" ：以只读方式打开文件，该文件必须存在。
//"w" ：打开只写文件，若文件存在则文件长度清为0，即该文件内容会消失。
//若文件不存在则建立该文件。
//"a" ：以附加的方式打开只写文件。若文件不存在，则会建立该文件，
//如果文件存在，写入的数据会被加到文件尾，即文件原先的内容会被保留。（EOF符保留）
//"r+" ：以可读写方式打开文件，该文件必须存在。
//"w+" ：打开可读写文件，若文件存在则文件长度清为零，即该文件内容会消失。
//若文件不存在则建立该文件。
//"a+"：以附加方式打开可读写的文件。若文件不存在，则会建立该文件，
//如果文件存在，写入的数据会被加到文件尾后，即文件原先的内容会被保留。（原来的EOF符不保留）
//"ab+"读写打开一个二进制文件，允许读，或在文件末追加数据。

enum FILE_SEEK_TYPE
{
    FILE_SEEK_CUR = 1,
    FILE_SEEK_END = 2,
    FILE_SEEK_SET = 0,
};

class file 
{
public:
    file();
    ~file();

    bool open(const char* file_path, const char* open_mode);
    void close();
    size_t file_size();
    int seek(int pos, FILE_SEEK_TYPE type);
    bool null() const;
    int read(char* buf, size_t size);
	int write(const char* buf, size_t size);
	void flush();
	FILE* get() { return fp_; }
	const FILE* get() const { return fp_;}
	void read_all(std::string &data);

public:
    static bool is_file_exist(const char* file_path);
    static bool is_directory_exist(const char* file_path);
    static bool rename_director(const char* src_path, const char* des_path);
    static bool rename_file(const char* src_file_path, const char* des_file_path);
    static bool remove_file(const char* file_path);
    static bool make_sure_directory_exist(const char* dir_path);
    static std::string file_directory(const char* file_path);
private:
#ifndef _MSC_VER
	static int make_dirs(const char* dir_path);
#endif
private:
    FILE* fp_;
    std::string path_;
};

}
