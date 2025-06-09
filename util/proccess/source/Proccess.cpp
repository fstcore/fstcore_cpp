#include "Proccess.h"

Proccess::Proccess(){

}

Proccess::cmd(char cmd){
    const char* command = cmd;
    FILE* pipe = popen(command, "r");
    if (!pipe) {
        std::cerr << "popen failed!" << std::endl;
        return 1;
    }
    const int bufferSize = 128;
    char buffer[bufferSize];
    std::string result;
    while (fgets(buffer, bufferSize, pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);
    return result;
}
