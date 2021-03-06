#include "ZlibWrapper.h"

// Replace this with a function that creates the directory if necessary
// and returns 1 if you want directory support
int makeDirectory(char *dir) {
    printf("mkdir(%s)\n", dir);
    return 0;
    // For MinGW (requires io.h)
    //mkdir(dir);
    //return 1;
}

void writeFile(char *filename, void *data, long bytes) {
    FILE *out;
    int i;

    // simplistic directory creation support
    for(i=0; filename[i]; i++) {
        if(filename[i] != '/')
            continue;

        filename[i] = '\0'; // terminate string at this point

        if(!makeDirectory(filename)) {
            fprintf(stderr, "Couldn't create subdirectory %s!\n", filename);
            return;
        }

        filename[i] = '/'; // Put the separator back
    }

    if(!i || filename[i-1] == '/')
        return; // empty filename or directory entry

    out = fopen(filename, "wb");

    if(out != NULL) {
        fwrite(data, 1, bytes, out); // best effort is enough here
        fclose(out);
    } else {
        fprintf(stderr, "Couldn't open %s for writing!\n", filename);
    }
}

int processFile(JZFile *zip) {
    JZFileHeader header;
    char filename[1024];
    unsigned char *data;

    if(jzReadLocalFileHeader(zip, &header, filename, sizeof(filename))) {
        printf("Couldn't read local file header!");
        return -1;
    }

    if((data = (unsigned char *)malloc(header.uncompressedSize)) == NULL) {
        printf("Couldn't allocate memory!");
        return -1;
    }

    printf("%s, %d / %d bytes at offset %08X\n", filename,
            header.compressedSize, header.uncompressedSize, header.offset);

    if(jzReadData(zip, &header, data) != Z_OK) {
        printf("Couldn't read file data!");
        free(data);
        return -1;
    }

    writeFile(filename, data, header.uncompressedSize);
    free(data);

    return 0;
}

int recordCallback(JZFile *zip, int idx, JZFileHeader *header, char *filename, void *user_data) {
    long offset;

    offset = zip->tell(zip); // store current position

    if(zip->seek(zip, header->offset, SEEK_SET)) {
        printf("Cannot seek in zip file!");
        return 0; // abort
    }

    processFile(zip); // alters file offset

    zip->seek(zip, offset, SEEK_SET); // return to position

    return 1; // continue
}

void unzip(string zip_file_path)
{
  FILE *fp;
  JZEndRecord endRecord;

  JZFile *zip;

  if(!(fp = fopen(zip_file_path.c_str(), "rb"))) {
    cout<<"Could not open "<<zip_file_path<<endl;
  }

  zip = jzfile_from_stdio_file(fp);

  if(jzReadEndRecord(zip, &endRecord)) {
    cout<<"Couldn't read ZIP file end record."<<endl;
    zip->close(zip);
    return;
  }

  if(jzReadCentralDirectory(zip, &endRecord, recordCallback, NULL)) {
    cout<<"Couldn't read ZIP file central record."<<endl;
    zip->close(zip);
    return;
  }

  zip->close(zip);

  if(remove(zip_file_path.c_str()) != 0)
    cout<<"Error deleting files"<<endl;
}
