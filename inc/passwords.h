#ifndef _PASSWORDS_H_
#define _PASSWORDS_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>



extern int GetKeyLength(const unsigned char* inputBuf);
extern unsigned char* GetLogin(const unsigned char* inputBuf);
extern unsigned char* GetPasswords(const unsigned char* inputBuf);

extern int EncryptData(unsigned char* login, unsigned char* pass, uint16_t keyBytes);















#endif
