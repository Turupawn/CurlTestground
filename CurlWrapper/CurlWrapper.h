#include <stdio.h>
#include <curl/curl.h>
#include <string>
#include <iostream>
#include <thread>

using namespace std;

//Reference
//https://curl.haxx.se/libcurl/c/url2file.html
//http://curl.askapache.com/c/debug.html

double getBytesDownloaded();
double getFileSize();
void downloadFile(string url, string download_path);
double getDownloadPercentage();
