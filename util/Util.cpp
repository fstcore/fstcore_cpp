#include "Util.h"

fstcore Util::fstcore;
bool Util::debug;
Audio Util::audio;
DataBase Util::database;
Debug_ Util::debug_;
Encoder Util::encoder;
FileSystem_ Util::filesystem;
Image Util::image;
Host Util::host;
Http Util::http;
Parser Util::parser;
Smtp Util::smtp;
Socket Util::socket;
UserAgent Util::useragent;
Proccess Util::proccess;
System Util::system;
ThreadManager Util::threadmanager;
ConsoleLog Util::consolelog;
PasswordGenerator Util::passwordgenerator;
Text Util::text;

Util::Util(){
    //INIT CLASS
    this->audio = Audio();
    this->data = Data();
    this->database = DataBase();
    this->debug_ = Debug_();
    this->encoder = Encoder();
    this->filesystem = FileSystem_();
    this->image = Image();
    this->host = Host();
    this->http = Http();
    this->parser = Parser();
    this->smtp = Smtp();
    this->useragent = UserAgent();
    this->proccess = Proccess();
    this->system = System();
    this->threadmanager = ThreadManager();
    this->consolelog = ConsoleLog();
    this->passwordgenerator = PasswordGenerator();
    this->text = Text();
    //INIT FSTCORE
    this->audio.fstcore = this->fstcore;
    this->data.fstcore = this->fstcore;
    this->database.fstcore = this->fstcore;
    this->debug_.fstcore = this->fstcore;
    this->encoder.fstcore = this->fstcore;
    this->filesystem.fstcore = this->fstcore;
    this->image.fstcore = this->fstcore;
    this->host.fstcore = this->fstcore;
    this->http.fstcore = this->fstcore;
    this->parser.fstcore = this->fstcore;
    this->smtp.fstcore = this->fstcore;
    this->useragent.fstcore = this->fstcore;
    this->proccess.fstcore = this->fstcore;
    this->system.fstcore = this->fstcore;
    this->threadmanager.fstcore = this->fstcore;
    this->consolelog.fstcore = this->fstcore;
    this->passwordgenerator.fstcore = this->fstcore;
    this->text.fstcore = this->fstcore;
    //INIT DEBUG
    this->audio.debug = this->debug;
    this->data.debug = this->debug;
    this->database.debug = this->debug;
    this->debug_.debug = this->debug;
    this->encoder.debug = this->debug;
    this->filesystem.debug = this->debug;
    this->image.debug = this->debug;
    this->host.debug = this->debug;
    this->http.debug = this->debug;
    this->parser.debug = this->debug;
    this->smtp.debug = this->debug;
    this->useragent.debug = this->debug;
    this->proccess.debug = this->debug;
    this->system.debug = this->debug;
    this->threadmanager.debug = this->debug;
    this->consolelog.debug = this->debug;
    this->passwordgenerator.debug = this->debug;
    this->text.debug = this->debug;
}
