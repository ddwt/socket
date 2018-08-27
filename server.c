/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月23日 星期四 19时45分38秒
 ************************************************************************/

#include <stdio.h>
#include "common.h"

#ifndef DBG
#define DBG(fmt, args...) printf(fmt, ##args);
#endif

#define HOSTIP "192.168.1.107"
#define PI1_HOST "192.168.1.40"
#define PI2_HOST "192.168.1.41"
#define PI3_HOST "192.168.1.42"
#define PI4_HOST "192.168.1.43"
#define PI5_HOST "192.168.1.44"
#define PI6_HOST "192.168.1.45"
#define PI7_HOST "192.168.1.46"
#define PI8_HOST "192.168.1.47"
#define PI9_HOST "192.168.1.48"
#define PI10_HOST "192.168.1.49"

#define link_warning_port 9999

#define share_con_port 9000
#define link_cpu_port 10000
#define link_mem_port 10001
#define link_disk_port 10002
#define link_sys_port 10003
#define link_user_port 10004
#define link_proc_port 10005

#define LOGADDR "/home/ddwt/文档/HZ/海贼项目/socket通信/Tcp项目/log/info.log"

int socket_create();
int socket_connect(int, int, char *, char *);
void socket_receive(char *, char *, char *);
void get_log();
char* get_time();
void open_warning_port();
void recv_file(char *str_file, int port, char *host, char *file_type);

int main() {
    int PID;
    PID = fork();
    if (PID == 0) {
	while (1) {
	    open_warning_port();
	}
        return 0;  
    } 
    char *buffer = (char *)calloc(1000, sizeof(char));
    while (1) {
        fflush(stdout);
        scanf("%s", buffer);
        if (strcmp(buffer, "pi1") == 0) {
            char str_buff[50] = "1/";
            socket_receive(str_buff, buffer, PI1_HOST);
        } else if (strcmp(buffer, "pi1") == 0) {
            char str_buff[50] = "2/";
            socket_receive(str_buff, buffer, PI2_HOST);
        } else if (strcmp(buffer, "pi2") == 0) {
            char str_buff[50] = "3/";
            socket_receive(str_buff, buffer, PI3_HOST);
        } else if (strcmp(buffer, "pi3") == 0) {
            char str_buff[50] = "4/";
            socket_receive(str_buff, buffer, PI4_HOST);
        } else if (strcmp(buffer, "pi4") == 0) {
            char str_buff[50] = "5/";
            socket_receive(str_buff, buffer, PI5_HOST);
        } else if (strcmp(buffer, "pi5") == 0) {
            char str_buff[50] = "6/";
            socket_receive(str_buff, buffer, PI6_HOST);
        } else if (strcmp(buffer, "pi6") == 0) {
            char str_buff[50] = "7/";
            socket_receive(str_buff, buffer, PI7_HOST);
        } else if (strcmp(buffer, "pi7") == 0) {
            char str_buff[50] = "8/";
            socket_receive(str_buff, buffer, PI8_HOST);
        } else if (strcmp(buffer, "host") == 0) {
            char str_buff[50] = "0/";
            socket_receive(str_buff, buffer, HOSTIP);
        } else {
            printf("输入IP错误\n");
        }
    }
    return 0;
}

int socket_create() {
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        DBG("socket_create error!\n");
        return -1;
    }
    return sockfd;
}

int socket_connect(int sockfd, int port, char *host, char *file_type) {
    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    dest_addr.sin_addr.s_addr = inet_addr(host);
    if (connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
        DBG("connect error\n");
        //在记录中输出错误信息
        //FILE *log_f = fopen(LOGADDR, "a+");
        //char *data = get_time();
        //fprintf(log_f, "%s : connect %s  error\n", data, host);
        //free(data);
        //fclose(log_f);
        return -1;
    } else {
        DBG("connect %s ok\n", host);
        //get_log(host, port, file_type);
    }
    return sockfd;
}

void socket_receive(char *str_file, char *buffer, char *client) {
    //1 socket()
    int sockfd;
    sockfd = socket_create();
    //2 connect()
    int port = share_con_port;
    char *host = strdup(client);
    struct sockaddr_in client_addr;
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(port);
    client_addr.sin_addr.s_addr = inet_addr(host);
    socklen_t len = sizeof(client_addr);
    if (connect(sockfd, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0) {
        DBG("connect error\n");
        //printf("connect error\n");
        return ;
        //在记录中输出错误信息
    } else {
        DBG("connect %s ok\n", host);
    }
    get_log(host, port, "log");
    //printf("链接成功\n");
    //sockfd = socket_connect(sockfd, port, host); 因为要获取链接地址　所以这里要把代码拆出来写
    //3 send()
    int PID, count = 0;
    for (int i = 0; i < 6; i++) {
        count++;
        PID = fork();
        if (PID == 0) break;
    }
    if (PID > 0) {
        while (1) {
            int con_sockfd = socket_connect(sockfd, share_con_port, HOSTIP, "log");
        }
    };
    if (count == 1) {
        recv_file(str_file, link_cpu_port, host, "cpu");
    } else if (count == 2) {
        recv_file(str_file, link_mem_port, host, "mem");
    } else if (count == 3) {
        recv_file(str_file, link_disk_port, host, "disk");
    } else if (count == 4) {
        recv_file(str_file, link_sys_port, host, "sys");
    } else if (count == 5) {
        recv_file(str_file, link_user_port, host, "user");
    } else {
        return ;
    }   
    
    return ;
}


void get_log(char *ip, int port, char *file_type) {
    FILE *log_f = fopen(LOGADDR, "a+"); 
    char *time = get_time();
    fprintf(log_f, "%s: Client IP is %s, port is %d, receive file_type is %s\n", time, ip, port, file_type);
    fflush(stdout);
    free(time);
    fclose(log_f);
    return ;
}

char *get_time() {
    time_t now;
    struct tm *timenow;
    time(&now);
    timenow = localtime(&now);
    char *ret = strdup(asctime(timenow));
    return ret;
}

void open_warning_port() {
    //int warning_sockfd = socket_create();
    //warning_sockfd = socket_connect(warning_sockfd, link_warning_port, );
    int sockfd = socket_create();
    struct sockaddr_in sock_addr;
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sock_addr.sin_port = htons(link_warning_port);
    socklen_t len = sizeof(struct sockaddr_in);
    if (bind(sockfd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0) {
        DBG("bind error!\n");
        return ;
        
    }
    if (listen(sockfd, 20) < 0) {
        close(sockfd);
        DBG("listen error\n");
        return ;
    }
    int client_sockfd = accept(sockfd, (struct sockaddr *)&sock_addr, &len);
    sleep(1);
    char buffer[50] = {0};
    recv(client_sockfd, buffer, 1000, 0);
    FILE *f = fopen("/home/ddwt/文档/HZ/海贼项目/socket通信/Tcp项目/warning/warning.log", "a+");
    fscanf(f, "%[^\n]s", buffer);
    fclose(f);
    return ;
}

void recv_file(char *str_file, int port, char *host, char *file) {
    char buffer[1000] = {0};
    char str_buff[51] = {0};
    char file_type[51] = {0};
    strcpy(file_type, file);
    strcpy(str_buff, str_file);
    strcat(file_type, ".log");
    strcat(str_buff, file_type);
    //printf("str_buff : %s\n", str_buff);
    strcpy(buffer, file_type);
    int sockfd = socket_create();
    struct sockaddr_in client_addr;
    //printf("开始链接专用端口\n");
    sockfd = socket_connect(sockfd, port, host, file_type);
    if (sockfd > 0) {
        //printf("链接专用端口成功\n");
    } else {
        printf("链接专用端口失败\n");
        return ;
    }       
    
    send(sockfd, file, strlen(buffer), 0); 
    sleep(0.3);
    memset(buffer, 0, 1000);
    fflush(stdout);  //接收前必须清除缓存
    recv(sockfd, buffer, 1000, 0);
    printf("recv : %s\n", buffer);
    DBG("receive = %s\n", buffer);
    if (strcmp(buffer, "ok") == 0) {
        sleep(3); //为啥睡３秒 等待client绑定端口开启监听
        memset(buffer, 0, 1000);
        FILE *save_f = fopen(str_buff, "w"); //此处需要更具ＩＰ地址区分文件夹
        if (save_f == NULL) printf("%s : 文件打开失败", str_buff);
        fflush(stdout);
        while (recv(sockfd, buffer, 1000, 0)) {
            fprintf(save_f, "%s", buffer);
            memset(buffer, 0, 1000);
        }
        fflush(stdout);
        fclose(save_f);
        close(sockfd);
        DBG("recv and save ok\n");
    }
    return ;   
}
