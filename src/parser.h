#ifndef _PARSER_H_
#define _PARSER_H_

#include <string>
#include <vector>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <map>
#include "main.h"

#define Open_Tag 0
#define Close_Tag 1
#define Single_Tag 2

string PackageInXML(char *tag, map<string, string> *attributes, char type = Open_Tag, bool newline = false);

class Parser;
struct Node{
  xmlNodePtr cur;
  xmlNodePtr start_cur;
};

class Parser{
  public:
    Parser();
    ~Parser();
    int LoadData(const char *data, const char *type);
    int FindElement(char *element);
    int FindNextElement(char *element);
    int EnterElement();
    void ExitElement();
    void ResetCursor();
    char *GetElementAttribute(char *attribute);
    char *GetElementData();

  private:
    char *KillWhiteSpace(char *str);
    vector<struct Node *> node_stack;
    xmlDocPtr doc;
    xmlNodePtr cur;
    xmlNodePtr start_cur;
};

#endif
