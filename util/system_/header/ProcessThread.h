#pragma once
#include "pch.h"

using namespace std;

#ifndef PROCESSHTREAD_H
#define PROCESSHTREAD_H

class ProcessThread {

private:
	static string e;

public:
	ProcessThread();
	static void set_data(string e);
	static void run();
};
#endif