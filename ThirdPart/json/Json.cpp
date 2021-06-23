#include <stdio.h>
#include "Json.h"
#include "rapidjson/filereadstream.h"
#include <cstdio>
#include <memory>

CJson::CJson()
	: m_pDoc(new Document())
{
}

CJson::~CJson()
{
	delete m_pDoc;
	m_pDoc = NULL;
}

int CJson::ParseFile(const char * fileName)
{
	FILE* fp = fopen(fileName, "rb");
	if (NULL == fp)
		return -1;

	fseek(fp, 0, SEEK_END);
	size_t filesize = (size_t)ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* buffer = new char[filesize + 1];
	std::unique_ptr<char[]> p(buffer);
	size_t readlen = fread(p.get(), 1, filesize, fp);
	p.get()[readlen] = '\0';
	int ret = ParseString(buffer);
	fclose(fp);
    return ret;
}

int CJson::ParseString(const char * string)
{
	m_pDoc->Parse(string);
    return m_pDoc->HasParseError() ? 0 : 1;
}

int CJson::getInt(Value &v, const char *prop, int def) const
{
    if (v.HasMember(prop))
    {
        Value& value = v[prop];
        if (value.IsInt())
            return value.GetInt();
    }
    return def;
}

bool CJson::getBoolean(Value &v, const char* prop, bool def) const
{
	if (v.HasMember(prop))
	{
		Value& value = v[prop];
		if (value.IsBool())
			return value.GetBool();
	}
	return def;
}

unsigned CJson::getUint(Value &v, const char *prop, unsigned def) const
{
    if (v.HasMember(prop))
    {
        Value& value = v[prop];
        if (value.IsUint())
            return value.GetUint();
    }
    return def;
}

float CJson::getFloat(Value &v, const char *prop, float def) const
{
    if (v.HasMember(prop))
    {
        Value& value = v[prop];
        if (value.IsFloat())
            return value.GetFloat();
    }
    return def;
}

const char* CJson::getUtf8String(Value& v, const char* prop, const char* def) const
{
	if (v.HasMember(prop))
	{
		Value& value = v[prop];
		if (value.IsString())
			return value.GetString();
	}
	return def;
}

Value &CJson::getValue(Value &v, const char *prop) const
{
	return v[prop];
}



