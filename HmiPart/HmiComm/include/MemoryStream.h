#pragma once
#ifndef _MEM_STREAM_H_
#define _MEM_STREAM_H_

#include <string>

class CMemoryStream
{
public:
	explicit CMemoryStream(std::size_t size)
	{
		m_pBuffer = new unsigned char[size];
		memset(m_pBuffer, 0, size);
		m_nSize = size;
		m_nCurrentWritePos = 0;
		m_nCurrentReadPos = 0;
		m_bManageMemory = true;
	}

	CMemoryStream(void* buffer, std::size_t size)
	{
		m_pBuffer = (unsigned char*)buffer;
		m_nSize = size;
		m_nCurrentWritePos = 0;
		m_nCurrentReadPos = 0;
		m_bManageMemory = false;
	}

	~CMemoryStream()
	{
		if (m_bManageMemory)
		{
			delete[] m_pBuffer;
		}
		m_pBuffer = NULL;
	}

public:
	template<typename T>
	CMemoryStream& operator <<(const T& value)
	{
		if (m_pBuffer &&
			m_nCurrentWritePos < m_nSize)
		{
			*(T*)(m_pBuffer + m_nCurrentWritePos) = value;
			m_nCurrentWritePos += sizeof(T);
		}

		return *this;
	}

	template<typename T>
	CMemoryStream& operator >>(T& value)
	{
		if (m_pBuffer &&
			m_nCurrentReadPos < m_nSize)
		{
			value = *(T*)(m_pBuffer + m_nCurrentReadPos);
			m_nCurrentReadPos += sizeof(T);
		}

		return *this;
	}

	CMemoryStream& operator <<(const char* pBuffer)
	{
		if (m_pBuffer &&
			m_nCurrentWritePos < m_nSize)
		{
			std::size_t size = strlen(pBuffer) + 1;
			memcpy(m_pBuffer + m_nCurrentWritePos, pBuffer, size);
			m_nCurrentWritePos += size;
		}

		return *this;
	}

	CMemoryStream& operator <<(const std::string& str)
	{
		return operator <<(str.c_str());
	}

	CMemoryStream& operator >>(const char* &pBuffer)
	{
		if (m_pBuffer &&
			m_nCurrentReadPos < m_nSize)
		{
			const char* p = (const char*)(m_pBuffer + m_nCurrentReadPos);
			std::size_t size = strlen(p);
			if (size + m_nCurrentReadPos < m_nSize)
			{
				pBuffer = p;
				m_nCurrentReadPos += size + 1;
			}
		}

		return *this;
	}

	CMemoryStream& operator >>(std::string& str)
	{
		if (m_pBuffer &&
			m_nCurrentReadPos < m_nSize)
		{
			const char* p = (const char*)(m_pBuffer + m_nCurrentReadPos);
			std::size_t size = strlen(p);
			if (size + m_nCurrentReadPos < m_nSize)
			{
				str.assign(p, p + size);
				m_nCurrentReadPos += size + 1;
			}
		}

		return *this;
	}

	CMemoryStream& operator <<(const wchar_t* pBuffer)
	{
		if (m_pBuffer &&
			m_nCurrentWritePos < m_nSize)
		{
			std::size_t size = (wcslen(pBuffer) + 1) * sizeof(wchar_t);
			memcpy(m_pBuffer + m_nCurrentWritePos, pBuffer, size);
			m_nCurrentWritePos += size;
		}

		return *this;
	}

	CMemoryStream& operator <<(const std::wstring& str)
	{
		return operator <<(str.c_str());
	}

	CMemoryStream& operator >>(const wchar_t*& pBuffer)
	{
		if (m_pBuffer &&
			m_nCurrentReadPos < m_nSize)
		{
			const wchar_t* p = (const wchar_t*)(m_pBuffer + m_nCurrentReadPos);
			std::size_t size = wcslen(p) * sizeof(wchar_t);
			if (size + m_nCurrentReadPos < m_nSize)
			{
				pBuffer = p;
				m_nCurrentReadPos += size + sizeof(wchar_t);
			}
		}

		return *this;
	}

	CMemoryStream& operator >>(std::wstring& str)
	{
		if (m_pBuffer &&
			m_nCurrentReadPos < m_nSize)
		{
			const wchar_t* p = (const wchar_t*)(m_pBuffer + m_nCurrentReadPos);
			std::size_t size = wcslen(p) * sizeof(wchar_t);
			if (size + m_nCurrentReadPos < m_nSize)
			{
				str.assign(p, p + size);
				m_nCurrentReadPos += size + sizeof(wchar_t);
			}
		}

		return *this;
	}

private:
	CMemoryStream(const CMemoryStream& memstream);

	const CMemoryStream& operator=(const CMemoryStream&);

private:
	unsigned char* m_pBuffer;
	std::size_t m_nSize;
	std::size_t m_nCurrentWritePos;
	std::size_t m_nCurrentReadPos;

	bool m_bManageMemory;
};

#endif