#include <iostream>
#include <fstream>
#include <string.h>
#include <dirent.h>
#include <windows.h>
using namespace std;

void FixPath(char path[256])
{
    char slash[1];
    strcpy(slash, "\\");
    if(path[strlen(path)-1] != slash[0])
        strcat(path, "\\");
}

void FileContentToFile(char fileFrom[256], char fileTo[256])
{
    ifstream stream1(fileFrom);
    ofstream stream2(fileTo);
    stream2 << stream1.rdbuf();
    cout<<fileFrom<<" >> "<<fileTo<<endl;
}

void FileContentToFolderFiles(char file[256], char folder[256], bool includeSubfolders)
{
    FixPath(folder);
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (folder)) != NULL)
    {
      int skippoints = 0;
      while ((ent = readdir (dir)) != NULL)
      {
          if(skippoints>1)
          {
              ifstream inputfile(file);
              char fulldir[200];
              strcpy(fulldir, folder);
              strcat(fulldir, ent->d_name);

              if(GetFileAttributes(fulldir) & FILE_ATTRIBUTE_DIRECTORY)
              {
                if(includeSubfolders)
                {
                      FixPath(fulldir);
                      FileContentToFolderFiles(file, folder, true);
                }
              }
              else
                  FileContentToFile(file, fulldir);
        }
        skippoints++;
      }
      closedir (dir);
    }
    else
    {
      perror ("");
    }
}

void ReplaceStringInString(string& subject, const string& search, const string& replace)
{
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos)
    {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}

void ReplaceStringInFile(char filePath[256], string toReplace, string replaceWith)
{
    ifstream inputFile(filePath);
    
    string contents((istreambuf_iterator<char>(inputFile)), (istreambuf_iterator<char>()));
    ReplaceStringInString(contents, toReplace, replaceWith);
    
    ofstream outputFile(filePath);
    outputFile<<contents;
    outputFile.close();
}

void ReplaceStringInFolderFiles(char path[256], string toReplace, string replaceWith, bool includeSubfolders)
{
    FixPath(path);
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (path)) != NULL)
    {
      int skippoints = 0;
      while ((ent = readdir (dir)) != NULL)
      {
          if(skippoints>1)
          {
            char fulldir[256];
              strcpy(fulldir, path);
              strcat(fulldir, ent->d_name);
              if(GetFileAttributes(fulldir) & FILE_ATTRIBUTE_DIRECTORY)
              {
                if(includeSubfolders)
                {
                      FixPath(fulldir);
                      ReplaceStringInFolderFiles(fulldir, toReplace, replaceWith, true);
                }
              }
              else
                  ReplaceStringInFile(fulldir, toReplace, replaceWith);
        }
        skippoints++;
      }
      closedir (dir);
    }
    else
    {
        perror ("");
    }
}
