#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <serialport.h>

serialport MinhaPorta("/dev/ttyUSB0",false);
int main(){
    printf("Conteudo: %s \n",MinhaPorta.ler(5000));
    return 0;
}