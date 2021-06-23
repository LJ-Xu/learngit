#pragma once

#include "rapidjson/document.h"
using namespace rapidjson;

class CJson
{
public:
	CJson();
	~CJson();

	int ParseFile(const char* fileName);
	int ParseString(const char* string);

	Document& doc() const { return (*m_pDoc); }

    //
    int getInt(Value& v, const char* prop, int def) const;
	bool getBoolean(Value &v, const char* prop, bool def) const;
    unsigned getUint(Value& v, const char* prop, unsigned def) const;
    float getFloat(Value& v, const char* prop, float def) const;
    Value& getValue(Value& v, const char* prop) const;
	const char* getUtf8String(Value& v, const char* prop, const char* def) const;

private:
	Document* m_pDoc;
};

