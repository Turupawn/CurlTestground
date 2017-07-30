#include "CurlWrapper/CurlWrapper.h"

//Reference
//https://curl.haxx.se/libcurl/c/url2file.html
//http://curl.askapache.com/c/debug.html

using namespace std;

int main(void)
{
  downloadFile("http://rosalilastudio.com:1337/ftp/file.zip", "file.zip");

  while(getBytesDownloaded() < getFileSize())
  {
    int percentage_downloaded = getBytesDownloaded()*100/getFileSize();
    cout<<percentage_downloaded<<"% downloaded"<<endl;
  }
  cout<<"Download complete"<<endl;
  return 0;
}
