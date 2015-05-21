#ifndef _WERROR_H_
#define _WERROR_H_

#include <vector>

using namespace std;

class ErrorLog;
void CleanExit();

class ErrorLog{
  public:
    void DebugMsg(char *msg);
    void Record(char *errormsg);
    void Fatal(char *errormsg);
    ~ErrorLog();
  private:
    vector<char *> errorlog;
};

#endif
