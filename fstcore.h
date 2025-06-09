#include "datacore.h"
#include "payment.h"
#include "sdk.h"
#include "ui.h"
#include "util.h"

class fstcore{

    private:
        jsonobj config_json;
        fstcore fstcore;
        bool debug;
    
    public:
        DataCore datacore;
        Payment payment;
        Sdk sdk;
        Ui ui;
        Util util;
    
    public:
        fstcore();
        open_config();
};
