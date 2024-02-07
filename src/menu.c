#include "menu.h"


enum eCmdNums CmdMenu(unsigned char* inputBuf, uint16_t bufSize){

    do{
        printf("----------------------------------\n");
        printf("Commands:\n");
        printf("/start\n");
        printf("/exit\n");
        printf("----------------------------------\n");
    }while(CommandBLock("Enter comand: ", inputBuf, bufSize) != 0);

    if(strncmp(inputBuf, "/exit", bufSize) == 0){
        return _CMD_EXIT;
    }
    if(strncmp(inputBuf, "/start", bufSize) == 0){
        return _CMD_START;
    }

    return _CMD_UNDEFINED;
}


int CommandBLock(const char* header, unsigned char* outputBuf, int bufSize){

    unsigned char *data;
    uint16_t dataSize;

    memset(outputBuf, 0, bufSize);

    printf(header);

    fflush(stdin);
    data = fgets(outputBuf, bufSize, stdin);

    if(data == NULL){
        printf("System error: stdin "); 
        printf("%d\n", ferror(stdin)); 
        return -1;
    }

    dataSize = strlen(outputBuf);

    if((dataSize > 0) && (outputBuf[dataSize-1] == '\n')){
        data[dataSize-1] = '\0';
        dataSize --;
    }
    /*  Необходимо отнять 2 байта, чтобы понять, что мы привысили максимальный заданный размер входных данных.
        Т.к. fgets за один раз возвращает заданное кол-во байт, учитывая обязательный финальный \0,
        а \n может отстаться в потоке до следующей вычитки
    */
    if(dataSize > (bufSize-2)){
        printf("Error: invalid data legth\n"); 
        fflush(stdin); //очищаем воходной поток от оставшихся данных
        return -2;
    }

    

    return 0;
}