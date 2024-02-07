#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "menu.h"
#include "passwords.h"


#define MAX_SIZE_INPUT_STIRNG       257     //максимальный размер принимаемых данных с учетом \n и \0


static unsigned char input_data[MAX_SIZE_INPUT_STIRNG];   





int main(void){

    enum eCmdNums cmdNum;
    int keySize = 0;
    unsigned char *login = NULL;
    unsigned char *pass = NULL;


    while(1){

        cmdNum = CmdMenu(input_data, sizeof(input_data));

        switch(cmdNum){   

        case _CMD_START:

            do{
                while(CommandBLock("Enter key size in bytes(16,24,32): ", input_data, sizeof(input_data)) != 0);
                keySize = GetKeyLength(input_data);
            }while(keySize <= 0);

            do{
                while(CommandBLock("Enter login: ", input_data, sizeof(input_data)) != 0);
                free(login);
                login = GetLogin(input_data);
            }while(login == NULL);

            do{
                while(CommandBLock("Enter password: ", input_data, sizeof(input_data)) != 0);
                free(pass);
                pass = GetPasswords(input_data);
            }while(pass == NULL);
            
            printf("----------------------------------\n");
            printf("Press enter to encrypt");
            getchar();
            fflush(stdin);
            printf("----------------------------------\n");

            EncryptData(login, pass, keySize);

            printf("Press enter to continue");
            getchar();
            fflush(stdin);
            break;

        case _CMD_EXIT:
            return 0;

        default:
            printf("Error: unknown command\n");  
            break;  
        }
    }

}