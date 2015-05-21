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
#include <string.h>
#include "werror.h"

void ErrorLog::Record(char *errormsg){
  char *newerror = new char[strlen("Error: ") + strlen(errormsg) + 1];
  sprintf(newerror, "Error: %s\n", errormsg);
  DebugMsg(newerror);
}

void ErrorLog::DebugMsg(char *msg){
  char *newmsg = new char[strlen(msg)];
  fprintf(stderr, msg);   printf("\n");
  strcpy(newmsg, msg);
  errorlog.push_back(newmsg);
}

void ErrorLog::Fatal(char *errormsg){
  fprintf(stderr, "Fatal Error: %s\n", errormsg);
  CleanExit();
}

ErrorLog::~ErrorLog(){
  vector<char *>::iterator error_iter;
  for(error_iter = errorlog.begin(); error_iter != errorlog.end(); error_iter++){
    delete *error_iter;
  }  
}

void CleanExit(){
  fprintf(stderr, "Exiting...\n");
  exit(0);
}
