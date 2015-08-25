/*
  Name: renamefile
  Copyright: huang zeqian
  Author: huang zeqian
  Date: 16/11/11 16:44
  Description: change the original substring in the file name 
               to a target substring for files under a directory
  eg.  given files "helloworld1.txt helloworld2.txt helloworld3.txt"
       if you want to rename the filename from helloworld*.txt to foo*.txt
       Usage:  program directory helloworld foo
       the result files are "foo1.txt foo2.txt foo3.txt"
*/
#include <stdio.h>
#include <string>
#include <dirent.h>
#include <sys/stat.h>

using namespace std;

#define MAX_STR_LENGTH 256

/*
* direct : directory name
* target : target substring
* replace : replace substring
* buf : character buffer
*/
int renamefile(string &direct, string &target, string &replace, char* buf)
{
    DIR *dir = NULL;
    struct dirent *dp = NULL;
    struct stat statbuf;
    char* ptr = NULL;
    string origin; // original file name
    string result; // result file name
    int offset = 0;
    int ret = 0;
    
    dir = opendir(direct.c_str());
    if(dir == NULL){
        printf("Open dir failed. \n");
        return -1;
    }
    while((dp = readdir(dir)) != NULL){
        origin = direct + "/" + dp->d_name;
        ret = stat(origin.c_str(), &statbuf);
        if(ret != 0)
        {
            printf("Read [%s] error.\n", dp->d_name);
            continue;
        }
        if(!S_ISREG(statbuf.st_mode))
            continue;
        strcpy(buf, dp->d_name);
        ptr = strstr(buf, target.c_str());
        if(ptr == NULL){
            continue;
        }
        offset = ptr - buf;
        buf[offset] = '\0';
        offset += target.size();
        ptr = buf + offset;
        result = direct + "/" + buf + replace + strdup(ptr);
        
        ret = rename(origin.c_str(), result.c_str());
        if(ret != 0)
        {
            printf("rename [%s] to [%s] failed. \n", origin.c_str(), result.c_str());
            continue;
        }else{
            printf("rename [%s] to [%s]\n", origin.c_str(), result.c_str());
        }
    }
    closedir(dir);
    return 0;
}

int main(int argc, char* argv[])
{

    int ret = 0;
    char file[MAX_STR_LENGTH];
    string direct;  // directory name
    string target;  // target substring
    string replace; // replace substring
        
    if(! (argc == 4 || argc == 3)){
        printf("Usage: %s directory origin replace\n",argv[0]);
        return 0;
    }
    memset(file, 0, MAX_STR_LENGTH);
    direct = argv[1];
    target = argv[2];
    replace = (argc == 4) ? argv[3] : "";

    ret = renamefile(direct, target, replace, file);
    if(ret != 0){
        return -1;
    }
    return 0;
}
