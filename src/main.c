#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include <io.h>
#include <direct.h>

#include "log.h"
#include "detectFile.h"

#define _CRT_SECURE_NO_WARNINGS

/// Function that detects extension of a file
static const char* extensionDetector(const char* file)
{
    char message[1024];
    if (file == NULL || *file == '\0')
    {
        snprintf(message, sizeof(message), "[ERROR]||NO EXTENSION!");
        logger(message); /// logs error if file is empty or NULL
        return NULL; /// returns nothing if no extension
    }

    /// strrchr here checks if there is a dot in the filename
    const char* dot = strrchr(file, '.'); /// file is guaranteed non-NULL here
    if (dot != NULL && dot != file && *(dot + 1) != '\0') {
        return dot; /// returns pointer to extension
    }
    else {
        return NULL; /// returns NULL if no valid extension
    }
}

/// Function that creates folder based on file extension
static void* createFolder(const char* file)
{
    const char* extension = extensionDetector(file);
    static char message[1024];

    if (extension == NULL) {
        snprintf(message, sizeof(message), "[ERROR]||Folder cannot be made!");
        logger(message); /// logs error if no extension
        return;
    }

    char folderName[100]; /// foldername can be only up to 100 characters
    snprintf(folderName, sizeof(folderName), "%s", extension + 1); /// skips dot in extension

    if (_mkdir(folderName) == 0) { /// _mkdir creates a folder
        snprintf(message, sizeof(message), "[INFO]||Folder %s has been made!", folderName);
        logger(message);
    }
    else {
        snprintf(message, sizeof(message), "[INFO]||Folder %s hasn't been made!", folderName);
        logger(message);
    }
}

/// Function for searching if there are any files in folder
static void fileLookUp(const char* path)
{
    WIN32_FIND_DATAA fileData; /// WIN32_FIND_DATAA for ANSI version
    char searchpath[MAX_PATH]; /// MAX_PATH expands the size to 260 bytes
    char message[1024];

    snprintf(searchpath, sizeof(searchpath), "%s\\*", path);
    HANDLE Find = FindFirstFileA(searchpath, &fileData); /// FindFirstFileA() starts searching for files

    if (Find == INVALID_HANDLE_VALUE) /// folder does not exist or is empty
    {
        snprintf(message, sizeof(message), "[ERROR]||Folder doesn't exist or it's empty!");
        logger(message);
        return;
    }

    do {
        /// skip "." and ".." entries
        if (strcmp(fileData.cFileName, ".") == 0 || strcmp(fileData.cFileName, "..") == 0)
            continue;

        printf("%s\n", fileData.cFileName); /// prints file name
    } while (FindNextFileA(Find, &fileData));

    FindClose(Find); /// closes file search
}

/// Function that moves files to corresponding folder
static void fileMoving(char* file)
{
    char message[1024];
    const char* extension = extensionDetector(file);
    snprintf(message, sizeof(message), "[INFO]||Detected an extension.");
    logger(message);

    char folderName[100];
    snprintf(folderName, sizeof(folderName), "%s", extension + 1); /// skips dot
    snprintf(message, sizeof(message), "[INFO]||Detected a folder name.");
    logger(message);

    char sourcePath[200]; /// path of the file
    snprintf(sourcePath, sizeof(sourcePath), "%s", file);
    snprintf(message, sizeof(message), "[INFO]||Detected file's source path.");
    logger(message);

    /// extract only filename from full path
    const char* fileName = strrchr(file, '\\');
    if (fileName != NULL) fileName++; else fileName = file;

    char destinationPath[200]; /// path of the folder
    snprintf(destinationPath, sizeof(destinationPath), "%s\\%s", folderName, fileName);
    snprintf(message, sizeof(message), "[INFO]||Moving source path to destination path.");
    logger(message);

    MoveFileA(sourcePath, destinationPath); /// moves the file to folder
}


int main()
{
    WIN32_FIND_DATAA fileData; /// ANSI version
    HANDLE FIND = FindFirstFileA("Entry\\*", &fileData);
    char message[1024];

    if (FIND == INVALID_HANDLE_VALUE) {
        snprintf(message, sizeof(message), "[ERROR]||No files found or directory does not exist.");
        logger(message);
        return 1;
    }

    do {
        ///skip "." and ".." entries
        if (strcmp(fileData.cFileName, ".") == 0 || strcmp(fileData.cFileName, "..") == 0)
            continue;

        char fullPath[MAX_PATH];
        snprintf(fullPath, sizeof(fullPath), "Entry\\%s", fileData.cFileName);

        snprintf(message, sizeof(message), "[INFO]||Entry\\%s", fileData.cFileName);
        logger(message);

        ///detects the extension
        const char* extension = extensionDetector(fullPath);

        ///if no extension is found, try detecting file type from content

        if (extension == NULL) {
            const char* type = detectFileType(fullPath);
            if (type && strcmp(type, "unknown") != 0) {
                snprintf(message, sizeof(message), "[INFO]||File has no extension but detected as: %s", type);
                logger(message);


                char renamedPath[MAX_PATH];
                snprintf(renamedPath, sizeof(renamedPath), "%s.%s", fullPath, type);
                ///renames the file to get the extension
                // Fix C6031: check return value of 'rename'
                if (rename(fullPath, renamedPath) != 0) {
                    snprintf(message, sizeof(message), "[ERROR]||Failed to rename file: %s to %s", fullPath, renamedPath);
                    logger(message);
                    continue; ///skips further processing for this file
                }

                snprintf(fullPath, sizeof(fullPath), "%s", renamedPath);
            }
            else {
                snprintf(message, sizeof(message), "[WARNING]||File type could not be detected.");
                logger(message);
                continue;
            }
        }

        createFolder(fullPath);
        fileMoving(fullPath);

    } while (FindNextFileA(FIND, &fileData));

    FindClose(FIND);
    snprintf(message, sizeof(message), "[INFO]||Files have been moved to their corresponding folder!");
    logger(message);
    return 0;
}
