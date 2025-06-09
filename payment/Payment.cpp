#include "bank/header/Bank.cpp"
#include "credit/header/Credit.cpp"
#include "crypto/header/Crypto.cpp"

fstcore Payment::fstcore;
bool Payment::debug;
Bank Payment::Bank;
Credit Payment::Credit;
Crypto Payment::Crypto;

Payment::Payment(){
    //INIT CLASS
    this->Bank = Bank();
    this->Credit = Credit();
    this->Crypto = Crypto();
    //INIT FSTCORE
    this->Bank->fstcore = this->fstcore;
    this->Credit->fstcore = this->fstcore;
    this->Crypto->fstcore = this->fstcore;
    //INIT DEBUG
    this->Bank->debug = this->debug;
    this->Credit->debug = this->debug;
    this->Crypto->debug = this->debug;
}
