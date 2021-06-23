#include "Serialize_Test_Memory.h"
#include "HMIProject.h"
#include "ProjectDataTables.h"

Serialize_Test_Memory& Serialize_Test_Memory::Ins()
{
	static Serialize_Test_Memory ins;
	return ins;
}

void TestRamSerilaze()
{
	Project::HMIProject prj;

	map<string, Project::StringStatusItem>& dt = prj.Tables.StrStatus.StringStatusMap;
	//vector<Project::StringStatusItem>& dt = prj.tables.strStatusDT.stringStatusDT;

	Project::StringStatusItem item;
	item.Tag = "name";
	vector<string> st1;
	st1.push_back("s1-zhang-lang1");
	st1.push_back("s1-zhang-lang2");
	vector<string> st2;
	st2.push_back("s2-zhang-lang1");
	st2.push_back("s2-zhang-lang2");
	item.StateLangs.push_back(st1);
	item.StateLangs.push_back(st2);
	//dt.push_back(item);
	dt.insert(pair<string, Project::StringStatusItem>("name", item));

	//序列化到内存

	ostringstream outs0(std::ios::binary);
	{
		cereal::BinaryOutputArchive archive(outs0);
		archive(item);
	}
	/*outs0.seekp(0, std::ios_base::beg);
	outs0.seekp(0, std::ios_base::end);
	len = outs0.tellp();*/

	int len = outs0.tellp();
	char buf[200];
	memcpy(buf, outs0.str().c_str(), len);

	//反序列化
	Project::StringStatusItem itemex;

	/*string strbuf="12";
	istringstream ins0(strbuf);
	iostream iostr();
	std::streambuf istringbuf(buf, len);

	for (size_t i = 0; i < len; i++)
	{
		ins0.putback(buf[i]);
	}*/
	StreamReadBuf osrb(buf, len);
	istream ins0(&osrb);
	//ins0.;
	{
		cereal::BinaryInputArchive archive(ins0);
		archive(itemex);
	}


#if 0
	prj.ToBinFile("D:\\1.bin");


	Project::HMIProject prj2;
	prj2.LoadBinFile("D:\\1.bin");
	map<string, Project::StringStatusItem>& dtt = prj2.tables.strStatusDT.stringStatusDT;
	cout << dtt["name"].statelangs[0][0] << endl;
#endif
}



struct WinTestST
{
	string name;
	vector<char> data;
	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(name, data);
	}
};
#include <map>
struct TestST
{
	string testname;
	map<int, WinTestST> maps;
	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(testname, maps);
	}
};
TestST testSt;
void TestMapSerialize()
{
	testSt.testname = "ABC";
	testSt.maps.insert(std::pair<int, WinTestST>(1, WinTestST()));
	testSt.maps[1].data.push_back(1);
	testSt.maps[1].data.push_back(2);
	testSt.maps[1].data.push_back(3);
	testSt.maps[1].data.push_back(4);
	testSt.maps.insert(std::pair<int, WinTestST>(2, WinTestST()));
	testSt.maps[2].data.push_back(10);
	testSt.maps[2].data.push_back(11);
	testSt.maps[2].data.push_back(12);
	testSt.maps[2].data.push_back(13);

	testSt.maps.insert(std::pair<int, WinTestST>(3, WinTestST()));
	testSt.maps[3].data.push_back(20);
	testSt.maps[3].data.push_back(21);
	testSt.maps[3].data.push_back(22);
	testSt.maps[3].data.push_back(23);


	ostringstream outs0(std::ios::binary);
	{
		cereal::BinaryOutputArchive archive(outs0);
		archive(testSt);
	}

	char buf[200];
	int len = outs0.tellp();
	memcpy(buf, outs0.str().c_str(), len);


	TestST intestSt;

	StreamReadBuf osrb(buf, len);
	istream ins0(&osrb);
	//ins0.;
	{
		cereal::BinaryInputArchive archive(ins0);
		archive(intestSt);
	}
	cout << intestSt.testname << endl;
}

bool Serialize_Test_Memory::Run(int argc, char ** argv)
{
	//TestRamSerilaze();
	TestMapSerialize();
	return true;
}
