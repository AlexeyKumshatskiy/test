#ifndef _MENU_H_
#define _MENU_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>



enum eCmdNums{
    _CMD_START = 0,
    _CMD_EXIT,
    _CMD_UNDEFINED
};

extern int CommandBLock(const char* header, unsigned char* outputBuf, int bufSize);
extern enum eCmdNums CmdMenu(unsigned char* inputBuf, uint16_t bufSize);

















#endif