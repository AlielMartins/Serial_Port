#include <serialport.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

serialport::serialport(char * name,int autenticar){
    printf("Comunicação porta serial by: Aliel Martins \n    Porta: %s \n",name);
    if(autenticar){
        printf("    Autenticação: Ativada \n");
    }else{
        printf("    Autenticação: Desativada \n");
    }
    if(open_port(name) == 0){
        printf("    [F] Falha em abrir a porta solicitada \n");
        port_flag = false;
    }else{
        printf("    Porta solicitada aberta \n");
        port_flag = true;
        if(set_configs()==0){
            printf("    [F] Falha em configurar porta \n");
            return;
        }else{
            printf("    Porta configurada com sucesso \n");
            if(autenticar == true){
                if(v_dispositivo()){
                    printf("    Dispositvo Reconhecido \n");
                    arduino_flag = true;
                }else{
                    printf("    Dispositvo desconhecido \n");
                    arduino_flag = false;
                    close_port();
                    return;
                }
            }else{
                arduino_flag = true;
            }
        }
    }
}
int serialport::open_port(char * name){
    id_port = open(name, O_RDWR | O_NOCTTY | O_NDELAY);
    if(id_port == -1){
        return 0;
    }
    return 1;
}
int serialport::close_port(){
    printf("Conexão encerrada \n");
    close(id_port);
}
int serialport::escrever(char * bytes,int zbytes){

}
char * serialport::ler(int zbytes){
    int bytes = 0,aux=0;  
    char * buffer = NULL;
    char fail_ler[] = "Arduino não inicializado";
    buffer = (char*)malloc(zbytes);
    if(arduino_flag){
        printf("Lendo arduino-> %d. \n",id_port);
        while(true){
            bytes = read(id_port,&buffer[aux],1);
            if(bytes == 1){
                //printf("Byte recebido: %s \n",buffer);
            }else{
                break;
            }
            if(aux==zbytes){
                break;
            }
            aux++;
        }
    }
    return buffer;
}

int serialport::set_configs(){
    int flag=0;
    struct termios PortSettings;
    memset(&PortSettings,0,sizeof(PortSettings));
    if(port_flag){
        flag = tcgetattr(id_port,&PortSettings);
        if(flag == -1){
            return 0;
        }
        flag = cfsetispeed(&PortSettings,B9600);
        if(flag == -1){
            return 0;
        }
        flag = cfsetospeed(&PortSettings,B9600);
        if(flag == -1){
            return 0;
        }
        flag = tcsetattr(id_port,TCSANOW,&PortSettings);
        if(flag == -1){
            return 0;
        }
    }
    return 1;
}
bool serialport::v_dispositivo(){
    int bytes=0,tentativas=0;
    char syn[] = "SYN";
    char syn_ack[] = "SYN-ACK";
    char ack[] = "ACK";
    char result[10] = "\0";
    while(true){
        sleep(3);
        bytes = write(id_port,syn,strlen(syn));
        if(bytes == strlen(syn)){
            printf("    FLAG %s ENVIADA \n",syn);
            bytes = 0;
            memset(&result[0],0,10);
            sleep(1);
            bytes = read(id_port,result,strlen(syn_ack));
            if(bytes == strlen(syn_ack)){
                //printf("%d recebidos \n",bytes);
               // printf("Dados: %s \n",result);
                if(strncmp(result,syn_ack,strlen(syn_ack))==0){
                    printf("    FLAG SYN-ACK RECEBIDA \n");
                    sleep(1);
                    bytes = write(id_port,ack,strlen(ack));
                    if(bytes == strlen(ack)){
                        printf("    FLAG ACK ENVIADA \n");
                        return true;
                    }else{
                        return false;
                    }
                }else{
                    return false;
                }
            }else{
               // printf("Bytes recebidos: %d \n",bytes);
               // printf("Dados: %s \n",result);
                if(tentativas == 5){
                    break;
                }
            }
        }
        tentativas++;
    }
    return false;
}
int serialport::get_portid(){
    return id_port;
}