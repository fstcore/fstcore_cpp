#include "ThreadManager.h"

bool ThreadManager::is_textbox = false;
bool ThreadManager::is_datagrid = false;
bool ThreadManager::thread_info = false;
bool ThreadManager::is_progress = false;
int ThreadManager::max_thread = 1;
vector<string> ThreadManager::data_list;
vector<thread> ThreadManager::thread_pool;

ThreadManager::ThreadManager() {
}

void ThreadManager::set_list(vector<string> e) {
	ThreadManager::data_list = e;
	//ThreadManager::textbox->AppendText(gcnew String(to_string(ThreadManager::data_list.size()).c_str()) + "\n");
}

void ThreadManager::set_maxthread(int m) {
	ThreadManager::max_thread = m;
}

void ThreadManager::getinfo() {
	if (ThreadManager::thread_info) {
		//std::cout << "Run: %d\n", ThreadManager::thread_pool.size() << std::endl;
	}
}

void ThreadManager::jointhread() {
	for (int i = 0; i < ThreadManager::thread_pool.size(); i++) {
		if (ThreadManager::thread_pool.at(i).joinable()) {
			ThreadManager::thread_pool.at(i).join();
		}
		ThreadManager::getinfo();
	}
	ThreadManager::thread_pool.clear();
}

void ThreadManager::run() {
	if (ThreadManager::data_list.size() > 0) {
		vector<string> tmp_;
		int count = 0;
		int total_ = int(ThreadManager::data_list.size()) / ThreadManager::max_thread;
		int totaltmp_ = ThreadManager::data_list.size() % ThreadManager::max_thread;
		for (int xinit = 1; xinit <= total_; xinit++) {
			if (ThreadManager::data_list.size() == count || count <= ThreadManager::data_list.size()) {
				tmp_.clear();
				for (int i = 0; i < ThreadManager::max_thread; i++) {
					tmp_.push_back(ThreadManager::data_list.at(count));
					count += 1;
				}
				for (int i = 0; i < tmp_.size(); i++) {
					ProcessThread pt = ProcessThread();
					pt.set_data(tmp_.at(i).c_str());
					ThreadManager::thread_pool.push_back(thread(pt.run));
				}
				ThreadManager::jointhread();
			}
			else {
				break;
			}
		}
		ThreadManager::jointhread();
		if (totaltmp_ > 0) {
			tmp_.clear();
			for (int i = 0; i < totaltmp_; i++) {
				tmp_.push_back(ThreadManager::data_list.at(count));
				count += 1;
			}
			for (int i = 0; i < tmp_.size(); i++) {
				ProcessThread pt = ProcessThread();
				pt.set_data(tmp_.at(i).c_str());
				ThreadManager::thread_pool.push_back(thread(pt.run));
			}
			ThreadManager::jointhread();
		}
	}
}