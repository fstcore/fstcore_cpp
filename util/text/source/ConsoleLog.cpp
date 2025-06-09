#include "pch.h"
#include "ConsoleLog.h"

void ConsoleLog::alert(string title, string d) {
	MessageBox::Show(gcnew String(d.c_str()), gcnew String(title.c_str()));
}

void ConsoleLog::print(string e) {
	string tmp = "echo \""+e+"\" && pause";
	system((const char *) tmp.c_str());
}
