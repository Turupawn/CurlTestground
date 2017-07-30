#include "CurlWrapper.h"

int BYTES_DOWNLOADED = 0;
int FILE_SIZE = 99999999;

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

void curlGetFileSize(string url)
{
  CURL *curl;
  curl = curl_easy_init();

  curl_easy_setopt(curl, CURLOPT_HEADER, 1);
  curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_perform(curl);

  double result;
  curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &result);
  FILE_SIZE = result;
  curl_easy_cleanup(curl);
}

void curlDownloadFile(string url, string download_path)
{
  CURL *curl;
  FILE *file;

  struct data config;

  config.trace_ascii = 1; /* enable ascii tracing */
  curlGetFileSize(url);
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

    curl_easy_perform(curl);

    /* always cleanup */
    curl_easy_cleanup(curl);
    fclose(file);
  }
}

int getBytesDownloaded()
{
  return BYTES_DOWNLOADED;
}

int getFileSize()
{
  return FILE_SIZE;
}

void downloadFile(string url, string download_path)
{
  std::thread t(curlDownloadFile, url, download_path);
  t.detach();
}
