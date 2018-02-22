#include <iostream>
#include <fstream>
#include <string.h>
#include <dirent.h>
#include <windows.h>

#define MAX_PATH_LENGTH 256

using namespace std;

void FixPath(char* path)
{
    if (path[strlen(path)-1] != '\\')
        strcat(path, "\\");
}

void FileContentToFile(char* fileFrom, char* fileTo)
{
    ifstream stream1(fileFrom);
    ofstream stream2(fileTo);
    stream2 << stream1.rdbuf();
    cout<<fileFrom<<" >> "<<fileTo<<endl;
}

void FileContentToFolderFiles(char* file, char* folder, bool includeSubfolders)
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
              char fulldir[MAX_PATH_LENGTH];
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

void ReplaceStringInFile(char* filePath, string toReplace, string replaceWith)
{
    ifstream inputFile(filePath);

    string contents((istreambuf_iterator<char>(inputFile)), (istreambuf_iterator<char>()));
    ReplaceStringInString(contents, toReplace, replaceWith);

    ofstream outputFile(filePath);
    outputFile<<contents;
    outputFile.close();
}

void ReplaceStringInFolderFiles(char* path, string toReplace, string replaceWith, bool includeSubfolders)
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
              char fulldir[MAX_PATH_LENGTH];
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
        perror ("Couldn't open folder");
    }
}
