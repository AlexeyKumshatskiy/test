#include "passwords.h"
#include "aes.h"


/* Генерация случайного ключа заданного размера в байтах */
static void RandomKey(uint8_t* key, uint8_t len){

    srand(time(NULL));
    for (int i = 0; i < len; i ++) {
        key[i] = (uint8_t)(rand() % 256);
    }
}

/* Создание и заполнение массива для ключа AES */
static uint8_t* CreateAesKey(uint8_t size){

    uint8_t *aesKey = NULL;

    aesKey = (uint8_t*)calloc(size, sizeof(uint8_t));
    if(aesKey == NULL){
        return NULL;
    }

    RandomKey(aesKey, size);

    return aesKey;
}



/*  Обработка введенного ключа. 
    Ключ может быть только 16, 24 и 32 байта
*/
int GetKeyLength(const unsigned char* inputBuf){

    uint16_t keySize;
    uint16_t dataSize = strlen(inputBuf);

    for(int i = 0; i < dataSize; i ++){
        if((inputBuf[i] < 0x30) || (inputBuf[i] > 0x39)){
            printf("Error: incorrect format\n"); 
            return -1;
        }
    }

    keySize = atoi(inputBuf);

    if((keySize != AES_KEY_SIZE_128) && (keySize != AES_KEY_SIZE_192) && (keySize != AES_KEY_SIZE_256)){
        printf("Error: invalid key size\n"); 
        return -2;
    }

    return keySize;
}

/*  Обработка введенного логина */
unsigned char* GetLogin(const unsigned char* inputBuf){

    uint16_t dataSize = strlen(inputBuf);
    unsigned char *login = NULL;

    if(dataSize == 0){
        printf("Error: login is not entered\n"); 
        return NULL;
    }

    //Проверка на отсутствие управляющих символов
    for(int i = 0; i < dataSize; i ++){
        if(((inputBuf[i] > 0x01) && (inputBuf[i] <= 0x1F)) || (inputBuf[i] == 0x7F)){
            printf("Error: incorrect format\n"); 
            return NULL;
        }
    }

    login = (unsigned char*)calloc(dataSize+1, sizeof(char));
    if(login == NULL){
        printf("System error: memory not allocated\n");  
        return NULL;   
    }

    strncpy(login, inputBuf, dataSize);
    login[dataSize] = '\0';

    printf("Success! Login added\n"); 

    return login;
}

/*  Обработка введенного пароля */
unsigned char* GetPasswords(const unsigned char* inputBuf){

    uint16_t dataSize = strlen(inputBuf);
    unsigned char *pass = NULL;

    if(dataSize == 0){
        printf("Error: password is not entered\n"); 
        return NULL;
    }

    //Проверка на отсутствие управляющих символов
    for(int i = 0; i < dataSize; i ++){
        if(((inputBuf[i] > 0x01) && (inputBuf[i] <= 0x1F)) || (inputBuf[i] == 0x7F)){
            printf("Error: incorrect format\n"); 
            return NULL;
        }
    }

    pass = (unsigned char*)calloc(dataSize+1, sizeof(char));
    if(pass == NULL){
        printf("System error: memory not allocated\n");  
        return NULL;   
    }


    strncpy(pass, inputBuf, dataSize);
    pass[dataSize] = '\0';

    printf("Success! Login added\n"); 

    return pass;
}


/*  Шифровка и расшифровка введенных данных */
int EncryptData(unsigned char* login, unsigned char* pass, uint16_t keySize){

    uint8_t *aesKey;          
    unsigned char *tmpLogin, *tmpPass;
    uint16_t loginLen, passLen;
    uint16_t expLoginLen, expPassLen;
    struct AES_ctx ctx = {0};


    if((keySize != AES_KEY_SIZE_128) && (keySize != AES_KEY_SIZE_192) && (keySize != AES_KEY_SIZE_256)){
        printf("Encrypt error: invalid key size\n");
        return -1;
    }
    if((login == NULL) || (pass == NULL)){
        printf("Encrypt error: data is not entered\n");
        return -2;
    }
  
    //Формирование ключа для шифрования 
    aesKey = CreateAesKey(keySize);
    if(aesKey == NULL){
        printf("System error: memory not allocated\n");  
        return -3;
    }

    loginLen = strlen(login);
    passLen = strlen(pass);

    //Находим размеры массивов, которые будут кратны 128 битам (т.к AES шифрует блоками по 16 байт)
    for(expLoginLen = loginLen; expLoginLen % AES_BLOCKLEN; expLoginLen ++);
    for(expPassLen = passLen; expPassLen % AES_BLOCKLEN; expPassLen ++);
    
    //Выделяем память для массивов, кратных 128 битам
    tmpLogin = (unsigned char*)calloc(expLoginLen, sizeof(char));
    tmpPass = (unsigned char*)calloc(expPassLen, sizeof(char));

    if((tmpLogin == NULL) || (tmpPass == NULL)){
        free(aesKey);
        free(tmpLogin);
        free(tmpPass);
        printf("System error: memory not allocated\n"); 
        return -4;
    }

    //Копируем данные пользователя в расширенные массивы
    memcpy(tmpLogin, login, loginLen);
    memcpy(tmpPass, pass, passLen);

    //------------------------------------// 
    printf("Login/password:\n");
    for(int i = 0; i < loginLen; i ++){
        printf(" %x", login[i]);
    }
    printf("\n");
    for(int i = 0; i < passLen; i ++){
        printf(" %x", pass[i]);
    }
    printf("\n");
    //------------------------------------// 

    //Инициализация алгоритма
    if(AES_init_ctx(&ctx, aesKey, keySize) != 0){
        printf("Encrypt error: aes is not initialized\n");
        free(aesKey);
        free(tmpLogin);
        free(tmpPass);
        return -5;
    }

    //Шифрование данных блоками по 16 байт
    for(int i = 0; i < expLoginLen; i += AES_BLOCKLEN){
        AES_ECB_encrypt(&ctx, &tmpLogin[i]);
    }
    for(int i = 0; i < expPassLen; i += AES_BLOCKLEN){
        AES_ECB_encrypt(&ctx, &tmpPass[i]);
    }

    //------------------------------------// 
    printf("Login/password after encrypt:\n");
    for(int i = 0; i < loginLen; i ++){
        printf(" %x", tmpLogin[i]);
    }
    printf("\n");
    for(int i = 0; i < passLen; i ++){
        printf(" %x", tmpPass[i]);
    }
    printf("\n");
    //------------------------------------// 

    //Расшифровка данных
    for(int i = 0; i < expLoginLen; i += AES_BLOCKLEN){
        AES_ECB_decrypt(&ctx, &tmpLogin[i]);
    }
    for(int i = 0; i < expPassLen; i += AES_BLOCKLEN){
        AES_ECB_decrypt(&ctx, &tmpPass[i]);
    }

    //------------------------------------// 
    printf("Login/password after decrypt:\n");
    for(int i = 0; i < loginLen; i ++){
        printf(" %x", tmpLogin[i]);
    }
    printf("\n");
    for(int i = 0; i < passLen; i ++){
        printf(" %x", tmpPass[i]);
    }
    printf("\n");
    //------------------------------------// 

    printf("----------------------------------\n");
    printf("RESULT: ");

    if((memcmp(tmpLogin,login,loginLen) == 0) && (memcmp(tmpPass,pass,passLen) == 0)){
        printf("SUCCESS!\n");
    }else{
        printf("FAIL!\n");
    }

    free(aesKey);
    free(tmpLogin);
    free(tmpPass);
}