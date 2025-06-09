#include "fstcore.h"

DataCore fstcore::datacore;
Payment fstcore::payment;
Sdk fstcore::sdk;
Ui fstcore::ui;
Util fstcore::util;
jsonobj fstcore::config_json;
fstcore fstcore::fstcore;
bool fstcore::debug;

fstcore::fstcore(){
    //INIT CLASS
    this->datacore = DataCore();
    this->payment = Payment();
    this->sdk = Sdk();
    this->ui = Ui();
    this->util = Util();
    fstcore::open_config();
    //INIT FSTCORE
    this->datacore.fstcore = this->fstcore;
    this->payment.fstcore = this->fstcore;
    this->sdk.fstcore = this->fstcore;
    this->ui.fstcore = this->fstcore;
    this->util.fstcore = this->fstcore;
    //INIT DEBUG
    this->datacore.debug = this->debug;
    this->payment.debug = this->debug;
    this->sdk.debug = this->debug;
    this->ui.debug = this->debug;
    this->util.debug = this->debug;
}

fstcore::open_config(){

}
