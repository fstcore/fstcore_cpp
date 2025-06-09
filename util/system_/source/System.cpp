#include "System.h"

using namespace std;

System::System(){

}

System::get_arch(){
    #if defined(__x86_64__) || defined(_M_X64)
        return "x86-64";
    #elif defined(__i386) || defined(_M_IX86)
        return "x86";
    #elif defined(__arm__) || defined(_M_ARM)
        return "ARM";
    #elif defined(__aarch64__) || defined(_M_ARM64)
        return "ARM64";
    #else
        return null;
    #endif
}

System::get_os(){
    #if defined(__linux__) || defined(__linux)
        return "Linux";
    #elif defined(_WIN32) || defined(_WIN64)
        return "Windows";
    #elif defined(__APPLE__) && defined(__MACH__)
        return "macOS";
    #elif defined(__FreeBSD__)
        return "FreeBSD";
    #elif defined(__ANDROID__)
        return "Android";
    #else
        return null;
    #endif
}

System::get_computername(){
    if(System::get_os() == "Windows"){
        char computerName[MAX_COMPUTERNAME_LENGTH + 1];
        DWORD size = sizeof(computerName);
        if (GetComputerNameA(computerName, &size)) {
            return computerName;
        } else {
            return null;
        }
    }else{
        char computerName[256];
        if (gethostname(computerName, sizeof(computerName)) == 0) {
            return computerName;
        } else {
            return null;
        }
    }
}
