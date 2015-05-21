//  ZoidsQuest
//  Copyright (C) 2004 Max Atkin <g_8pit@users.sourceforge.net>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
//  02111-1307, USA.



#include <stdio.h>
#include "io.h"

File::File( ){
  fd = NULL;
}   

File::~File( ){
  if(fd != NULL) fclose(fd);
}   
    
int File::Open(char *file, char *mode){
    if(mode == NULL) fd = fopen(file, "r");
    else fd = fopen(file, mode);
    if(fd == NULL){
        return Error;
    }
    return Ok;
}

char *File::ReadLine(char *line, int size){
    return(fgets(line, size, fd));
}

char File::ReadChar(){
    char c = fgetc(fd);
    if(c != EOF) return(c);
    return End;
}

int File::WriteLine(char *line){
  if(fputs(line, fd) == EOF) return Error;
  if(fputc('\n', fd) == EOF) return Error;
  return Ok;
}

int File::WriteChunk(char *chunk){
  if(fputs(chunk, fd) == EOF) return Error;
}

int File::WriteChar(char c){
  return(fputc(c, fd));
}

long File::Length(){
  long position = ftell(fd);
  fseek(fd, 0L, SEEK_END);
  long length = ftell(fd);
  fseek(fd, position, SEEK_SET);
  return length;
}
