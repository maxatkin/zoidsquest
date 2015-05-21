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
#include <ctype.h>
#include "parser.h"


string PackageInXML(char *tag, map<string, string> *attributes, char type, bool newline){
  map<string, string>::iterator i;
  string retstr;
  if(type == Close_Tag) retstr = "</";
  else retstr = "<";
  retstr = retstr + tag;
  if(attributes != NULL){
    for(i = attributes->begin(); i != attributes->end(); i++){
      retstr = retstr + " " + (*i).first + " = \"" + (*i).second + "\"";
    }
  }
  if(type == Single_Tag) retstr = retstr + "/>";
  else retstr = retstr + ">";
  if(newline) retstr = retstr + "\n";
  return retstr;
}


Parser::Parser(){
  doc = NULL;
}

Parser::~Parser(){
  xmlFreeDoc(doc);
}

int Parser::LoadData(const char *data, const char *type){
  doc = xmlParseFile(data);
  if(doc == NULL){
    fprintf(stderr,"Level file not parsed successfully.\n");
    return Error;
  }
  cur = xmlDocGetRootElement(doc);
  if(cur == NULL){
    fprintf(stderr,"empty document\n");
    return Error;
  }
  if(xmlStrcmp(cur->name, (const xmlChar *)type)){
    fprintf(stderr,"Sorry, this is not a valid %s file\n", type);
    return Error;
  }
  cur = cur->xmlChildrenNode;
  start_cur = cur;
  return Ok;
}

int Parser::FindElement(char *element){
  xmlNodePtr i = start_cur;
  if(!xmlStrcmp(cur->name, (const xmlChar *)element)) i = cur->next;
  while(i != NULL){
    if(!xmlStrcmp(i->name, (const xmlChar *)element)){
      cur = i;
      return Ok;
    }
    i = i->next;
  }
  return Error;  
}

void Parser::ResetCursor(){
  cur = start_cur;
}

int Parser::FindNextElement(char *element){
  cur = cur->next;
  while(cur != NULL){
    if((!xmlStrcmp(cur->name, (const xmlChar *)element))) return Ok;
    cur = cur->next;
  }
  return Error;
}

int Parser::EnterElement(){
  if(cur->xmlChildrenNode == NULL) return Error;

  struct Node *lastnode = new struct Node;
  lastnode->cur = cur;
  lastnode->start_cur = start_cur;

  node_stack.push_back(lastnode);
  cur = cur->xmlChildrenNode;
  start_cur = cur;
  return Ok;
}

void Parser::ExitElement(){
  struct Node *lastnode;
  lastnode = node_stack.back();
  cur = lastnode->cur;
  start_cur = lastnode->start_cur;
  delete lastnode;
  node_stack.pop_back();
}

char *Parser::GetElementAttribute(char *attribute){
  return (char *)xmlGetProp(cur, (const xmlChar *)attribute);
}

char *Parser::GetElementData(){
  return KillWhiteSpace((char *)xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
}

char *Parser::KillWhiteSpace(char *str){
  if(str == NULL) return NULL;
  unsigned int i, size = strlen(str);
  char *start = str;
  
  for(i = 0; i < size; i++){
    if(!isspace(str[i])){
      start = &str[i];
      break;
    }
  }

  for(i = size - 1; i >= 0; i--){
    if(!isspace(str[i])){
      str[i+1] = 0x00;
      break;
    }
  }
  return start;
}
