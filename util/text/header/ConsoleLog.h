#pragma once
#include <stdlib.h>
#include <string>

using namespace std;
using namespace System;
using namespace System::IO;
using namespace System::Collections;
using namespace System::Collections::Specialized;
using namespace System::ComponentModel;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Threading;

ref class ConsoleLog
{
public:
	void alert(string title, string d);
	void print(string e);
};

