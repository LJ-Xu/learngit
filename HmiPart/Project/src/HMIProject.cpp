/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name		: HMIProject.cs
 * Author   : zhanglinbo
 * Date     : 2020/08/11
 * Descript	: �û������Ĺ�������,Ҳ��Ӧ�ó������ݡ��ýṹ�����л���Ķ������ļ�
 * Version	: 1.0
 * modify	:
 *			:
 *******************************************************************************/
#include "HMIProject.h"

namespace Project
{
	HMIProject* HMIProject::Ins;
	ofstream& operator<<(ofstream& out, const BinHeader& h)
	{
		out.write((char*)&h, sizeof(BinHeader));
		//out << h.size << h.resOffset << h.crc;
		return out;
	}
	ifstream& operator>>(ifstream& in, BinHeader& h)
	{

		//in >> h.size >> h.resOffset >> h.crc;
		in.read((char*)&h, sizeof(BinHeader));
		return in;
	}
	static void FileEmptyFill(ofstream& os, int size)
	{
		char buf[512] = { 0 };
		while (size > 0)
		{

			if (size >= sizeof(buf))
			{
				os.write(buf, sizeof(buf));
				size -= sizeof(buf);
			}
			else
			{
				os.write(buf, size);
				size = 0;
			}
		}
	}
	/*******************************************************************************
	 * Name	    : ToBinFile
	 * Descript : ���л��������ļ���ָ�����ļ�·��
	 * Input	: path - �����·��
	 * Output	: int  - ���л�����ļ���С
	 * Note	    :
	*******************************************************************************/
	int HMIProject::ToBinFile(const char* path)
	{
		BinHeader header;
		ofstream out0;
		{
			out0.open(path, std::ios::binary);
			out0 << header;
			cereal::BinaryOutputArchive archive(out0);
			archive(*this);
		}
		header.ResOffset = (unsigned int)out0.tellp();
		//out0.write(this->reses.GetDataPtr(), this->reses.size);
		FileEmptyFill(out0, this->Reses.Size);
		header.Size = (unsigned int)out0.tellp();
		out0.seekp(std::ios_base::beg);
		out0 << header;
		out0.close();
		return 0;
	}
	/*******************************************************************************
	 * Name	    : LoadBinFile
	 * Descript : �������л���Ķ������ļ�����ָ���ṹ����
	 * Input	: path - �������ļ�·��
	 * Output	: int  - ���л�����ļ���С
	 * Note	    :
	*******************************************************************************/
	int HMIProject::LoadBinFile(const char* path)
	{
		BinHeader header;
		ifstream in0;
		{
			in0.open(path, ios::binary);
			//in0.open(path, ios::in);
			in0 >> header;
			cereal::BinaryInputArchive archive(in0);
			archive(*this);
			Ins = this;
		}
		this->Reses.Size = header.Size - header.ResOffset;
		//TODO �����ж�
		this->Reses.SetData(new char[this->Reses.Size], this->Reses.Size);
		in0.read(this->Reses.GetDataPtr(), this->Reses.Size);
		in0.close();
		RecipeDT::SetRecipeDTIns(&this->Tables.Recipe);
		
		return 0;
	}
	/*
	int HMIProject::LoadMem()
	{
		// output to memory.
		string strbuf0;
		ostringstream outs0;
		{
			cereal::BinaryOutputArchive archive(outs0);
			archive(*this);
		}
		cout << "outs0.str()=" << endl << outs0.str() << endl;
		cout << "strbuf0=" << endl << strbuf0 << endl;
		strbuf0 = outs0.str();
		outs0.clear();
		cout << "strbuf0=" << endl << strbuf0 << endl;
	}
	int HMIProject::ToMem()
	{

		string strbuf0;
		istringstream ins0(strbuf0);
		{
			cereal::BinaryInputArchive archive(ins0);
			archive(*this);
		}
		//cout << "ttts0=" << endl << ttts0 << endl;
	}
	*/
}






