#include "CurlWrapper/CurlWrapper.h"
#include "ZlibWrapper/ZlibWrapper.h"

using namespace std;

int main(void)
{
  downloadFile("http://rosalilastudio.com:1337/ftp/file.zip", "file.zip");
  while(getDownloadPercentage()!=100.00)
  {
    cout<<getDownloadPercentage()<<"% downloaded"<<endl;
  }
  cout<<"Download complete"<<endl;

  unzip("file.zip");

  return 0;
}
