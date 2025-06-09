#include "datamanager/source/DeleteData.cpp"
#include "datamanager/source/DeleteData.cpp"
#include "datamanager/source/DeleteData.cpp"

fstcore DataCore::fstcore;
bool DataCore::debug;
DeleteData DataCore::deletedata;
InsertData DataCore::insertdata;
ListData DataCore::listdata;

DataCore::DataCore(){
    //INIT CLASS
    this->deletedata = DeleteData();
    this->insertdata = InsertData();
    this->listdata = ListData();
    //INIT FSTCORE
    this->deletedata->fstcore = this->fstcore;
    this->insertdata->fstcore = this->fstcore;
    this->listdata->fstcore = this->fstcore;
    //INIT DEBUG
    this->deletedata->debug = this->debug;
    this->insertdata->debug = this->debug;
    this->listdata->debug = this->debug;
}
