#include "audio/Audio.h"
#include "data/Data.h"
#include "database/DataBase.h"
#include "debug/Debug_.h"
#include "encoder/Encoder.h"
#include "filesystem_/FileSystem_.h"
#include "image/Image.h"
#include "net/Host.h"
#include "net/Http.h"
#include "net/Parser.h"
#include "net/Smtp.h"
#include "net/Socket.h"
#include "net/UserAgent.h"
#include "proccess/Proccess.h"
#include "system_/System.h"
#include "system_/ThreadManager.h"
#include "text/ConsoleLog.h"
#include "text/PasswordGenerator.h"
#include "text/Text.h"

class Util{

    private:
        fstcore fstcore;
        bool debug;
    
    public:
        Util();
};
