#include "pch.h"
#include "ProcessThread.h"

string ProcessThread::e;

ProcessThread::ProcessThread() {
}

void ProcessThread::set_data(string e) {
	ProcessThread::e = e;
}

void ProcessThread::run() {
	//msclr::interop::marshal_context ctx;
	//string dt = ctx.marshal_as<string>(line_data->c_str());
	HttpRequestCurl r = HttpRequestCurl();
	r.set_url(to_string(ProcessThread::e.c_str()));
	r.set_timeout(10000);
	map<string, string> res = r.request("GET");
	//ProcessThread::textbox->AppendText(gcnew String(res["uri"].c_str()) + "\n");
}