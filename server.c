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

#define share_con_port 9000
#define link_cpu_port 10000
#define link_mem_port 10001
#define link_disk_port 10002

#define LOGADDR "/home/ddwt/文档/HZ/海贼项目/socket通信/Tcp项目/log/info.log"

int socket_create();
int socket_connect(int, int, char *, char *);
void socket_receive(char *, char *, char *);
void get_log();
char* get_time();

int main() {
    
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

void socket_receive(char *str_buff, char *buffer, char *client) {
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
    if (connect(sockfd, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0) {
        DBG("connect error\n");
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
            int a = 1;
            scanf("%d", &a);
            printf("父进程不能死!\n");
        }
    };
    if (count == 1) {
        char buffer1[1000] = {0};
        strcat(str_buff, "cpu.log");
        strcpy(buffer1, "cpu");

        int cpu_sockfd = socket_create();
        struct sockaddr_in client_addr;
        printf("开始链接专用端口\n");
        cpu_sockfd = socket_connect(cpu_sockfd, link_cpu_port, host, "cpu");
        if (cpu_sockfd > 0) {
            printf("链接专用端口成功\n");
        } else {
            printf("链接专用端口失败\n");
        }       
        
        int ans = send(cpu_sockfd, buffer1, strlen(buffer1), 0);
        sleep(0.3);
        memset(buffer1, 0, 1000);
        fflush(stdout);  //接收前必须清除缓存
        recv(cpu_sockfd, buffer1, 1000, 0);
        DBG("receive = %s\n", buffer1);
        if (strcmp(buffer1, "ok") == 0) {
            sleep(3); //为啥睡３秒 等待client绑定端口开启监听
            memset(buffer1, 0, 1000);
            FILE *save_cpu = fopen(str_buff, "w"); //此处需要更具ＩＰ地址区分文件夹
            fflush(stdout);
            while (recv(cpu_sockfd, buffer1, 1000, 0)) {
                fprintf(save_cpu, "%s", buffer1);
                memset(buffer1, 0, 1000);
            }
            fflush(stdout);
            fclose(save_cpu);
            close(cpu_sockfd);
            DBG("recv and save ok\n");
        }
        return ;
    } else if (count == 2) {
        char buffer2[1000] = {0};
        strcat(str_buff, "mem.log");
        strcpy(buffer2, "mem");
        int mem_sockfd = socket_create();
        mem_sockfd = socket_connect(mem_sockfd, link_mem_port, host, "mem");
        if (mem_sockfd > 0) {
            printf("链接专用端口成功\n");
        } else {
            printf("链接专用端口失败\n");
        }              
        send(mem_sockfd, buffer2, strlen(buffer2), 0);
        memset(buffer2, 0, 1000);
        fflush(stdout);
        recv(mem_sockfd, buffer2, 1000, 0);
        DBG("recvive = %s\n", buffer2);
        if (strcmp(buffer2, "ok") == 0) {
            sleep(0.3);
            memset(buffer2, 0, 1000);

            FILE *save_mem = fopen(str_buff, "w");
            fflush(stdout);
            while (recv(mem_sockfd, buffer2, 1000, 0)) {
                fprintf(save_mem, "%s", buffer2);
                memset(buffer2, 0, 1000);
            }
            fflush(stdout);
            fclose(save_mem);
            close(mem_sockfd);
            DBG("recv and save ok\n");
        }
        return ;
    } else if (count == 3) {
        char buffer3[1000] = {0};
        strcat(str_buff, "disk.log");
        strcmp(buffer3, "disk");
        int disk_sockfd = socket_create();
        disk_sockfd = socket_connect(disk_sockfd, link_disk_port, host, "disk");
        if (disk_sockfd > 0) {
            printf("链接专用端口成功\n");
        } else {
            printf("链接专用端口失败\n");
        }       
        send(disk_sockfd, buffer3, strlen(buffer3), 0);
        memset(buffer3, 0, 1000);
        fflush(stdout);
        recv(disk_sockfd, buffer3, 1000, 0);
        DBG("receive = %s\n", buffer3);
        if (strcmp(buffer3, "ok") == 0) {
            sleep(0.3);
            memset(buffer3, 0, 1000);

            FILE *save_disk = fopen(str_buff, "w");
            while (recv(disk_sockfd, buffer3, 1000, 0)) {
                fprintf(save_disk, "%s", buffer3);
            }
            fflush(stdout);
            fclose(save_disk);
            close(disk_sockfd);
            DBG("recv and save ok\n");
        }
        return ;
    } else if (count == 4) {
        return ;
    } else if (count == 5) {
        return ;
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
