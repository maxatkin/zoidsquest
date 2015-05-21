#ifndef _IO_H_
#define _IO_H_

#include "main.h"
#define End 0

class File;

class File{
  public:
    File();
    int Open(char *file, char *mode = NULL);
    char ReadChar( );
    char *ReadLine(char *line, int size);
    int WriteLine(char *line);
    int WriteChar(char c);
    int WriteChunk(char *chunk);
    long Length();
   ~File( );
  private:
    FILE *fd;
};


#endif
