#include <stdio.h>
#include <curl/curl.h>
/* For older cURL versions you will also need 
#include <curl/types.h>
#include <curl/easy.h>
*/
#include <string>
#include <iostream>
#include <thread>

//Reference
//https://curl.haxx.se/libcurl/c/url2file.html
//http://curl.askapache.com/c/debug.html

using namespace std;

int BYTES_DOWNLOADED = 0;
int TOTAL_BYTES_DOWNLOAD = 14657001;

struct data
{
  char trace_ascii; /* 1 or 0 */ 
};
 
static int my_trace(CURL *handle, curl_infotype type,
             char *data, size_t size,
             void *userp)
{
  (void)handle; /* prevent compiler warning */ 

  if(type == CURLINFO_DATA_IN)
  {
    BYTES_DOWNLOADED+=size;
  }
  return 0;
}

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

void curlDownloadFile(string url, string download_path)
{
  CURL *curl;
  FILE *file;
  CURLcode res;

  struct data config;
 
  config.trace_ascii = 1; /* enable ascii tracing */ 

  curl = curl_easy_init();
  if(curl)
  {
    curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, my_trace);
    curl_easy_setopt(curl, CURLOPT_DEBUGDATA, &config);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    file = fopen(download_path.c_str(),"wb");
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
    res = curl_easy_perform(curl);

    double cl;
    res = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &cl);
    if(!res) {
      printf("Size: %.0f\n", cl);
    }

    /* always cleanup */
    curl_easy_cleanup(curl);
    fclose(file);
  }
}

void downloadFile(string url, string download_path)
{
  std::thread t(curlDownloadFile, url, download_path);
  t.detach();
}

int main(void)
{
  downloadFile("http://rosalilastudio.com:1337/ftp/file.zip", "file.zip");

  while(BYTES_DOWNLOADED < TOTAL_BYTES_DOWNLOAD)
  {
    int percentage_downloaded = BYTES_DOWNLOADED*100/TOTAL_BYTES_DOWNLOAD;
    cout<<percentage_downloaded<<"% downloaded"<<endl;
  }
  cout<<"Download complete"<<endl;

  return 0;
}
