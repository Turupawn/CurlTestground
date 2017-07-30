#include <stdio.h>
#include <curl/curl.h>
#include <string>
#include <iostream>
#include <thread>

using namespace std;

int getBytesDownloaded();
int getFileSize();
void downloadFile(string url, string download_path);
