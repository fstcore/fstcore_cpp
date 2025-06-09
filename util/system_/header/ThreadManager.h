#include "stdafx.h"

using namespace std;

class ThreadManager {

private:
	static bool thread_info;
	static bool is_progress;
	static bool is_textbox;
	static bool is_datagrid;
	static int max_thread;
	static vector<string> data_list;
	static vector<thread> thread_pool;

public:
	ThreadManager();
	static void set_list(vector<string> e);
	static void set_maxthread(int m);
	static void getinfo();
	static void jointhread();
	static void run();
};
