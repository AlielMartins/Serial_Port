class serialport{
    private:
    int id_port=0;
    bool arduino_flag=false;
    bool port_flag=false;
    public:
    serialport();
    serialport(char * name,int autenticar);
    int open_port(char * name);
    int close_port();
    int escrever(char * bytes,int zbytes);
    int set_configs();
    char * ler(int zbytes);
    bool v_dispositivo();
    int get_portid();
};