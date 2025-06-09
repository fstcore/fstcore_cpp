class FileSystem
{

public:
    FileSystem();
    char topath(char path);
    char readfile(char folder, char file);
    void writefile(char folder, char file, char data);
    void appendfile(char folder, char file, char data);
    void makefolder(char folder);
    void makefile(char folder, char file);
    void deletefolder(char folder);
    void deletefile(char folder, char file);
    void renamefolder(char folder, char foldernew);
    void renamefile(char folder, char file, char foldernew, char filenew);
    void delete_linefile(char folder, char file, char r);
    void replace_linefile(char folder, char file, char r, char replacement);
    void is_existonfile(char folder, char file, char r);
    void filetoarray(char folder, char file, char expl);
};
