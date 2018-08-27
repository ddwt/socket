/*************************************************************************
	> File Name: client.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月23日 星期四 22时38分31秒
 ************************************************************************/

#include <stdio.h>
#include "common.h"
#ifndef DBG
#define DBG(fmt, args...) printf(fmt, ##args);
#endif

#define HOSTIP "192.168.1.107"

#define share_waring_port 9999

#define share_con_port 9000
#define share_cpu_port 10000
#define share_mem_port 10001
#define share_disk_port 10002
#define share_sys_port 10003
#define share_user_port 10004
#define share_proc_port 10005

#define CPU_FILE "/home/ddwt/文档/HZ/海贼项目/socket通信/Tcp项目/本机测试数据/cpu"
#define DISK_FILE "/home/ddwt/文档/HZ/海贼项目/socket通信/Tcp项目/本机测试数据/disk"
#define MEM_FILE "/home/ddwt/文档/HZ/海贼项目/socket通信/Tcp项目/本机测试数据/mem"
#define SYS_FILE "/home/ddwt/文档/HZ/海贼项目/socket通信/Tcp项目/本机测试数据/sys"
#define USER_FILE "/home/ddwt/文档/HZ/海贼项目/socket通信/Tcp项目/本机测试数据/user"
#define PROC_FILE "/home/ddwt/文档/HZ/海贼项目/socket通信/Tcp项目/本机测试数据/proc"
int socket_create();
void socket_bind(int, int);
void socket_listen(int);
int socket_accept(int, struct sockaddr_in *, socklen_t *);
void set_socket(int sockfd);
void get_file(char *, char *);
void send_warning(char *);
void send_file(char *bash, char *save_file, int port, char *file_type);

int main() {
    fflush(stdout);
    //1 socket()
    int sockfd = socket_create();
    //2 set_socket
    set_socket(sockfd);
    //3 bind
    socket_bind(sockfd, share_con_port);
    //4 listen
    socket_listen(sockfd);
    //5 accept 重新开个套解字　端口 一直停留在监听阻塞
    int rec_sockfd; 
    struct sockaddr_in server_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    int PID, count = 0;
    for (int i = 0; i < 5; i++) {
        count++;
        PID = fork();
        if (PID == 0) break;
    }
    
    if (PID != 0) {
        int a = 0;
        while (1) {
            rec_sockfd = socket_accept(sockfd, &server_addr, &len);
        }
	    return 0;
    }
    if (count == 1) {
        send_file("bash get_cpu.sh", CPU_FILE, share_cpu_port, "cpu");
    } else if (count == 2) {
        send_file("bash get_memory.sh", MEM_FILE, share_mem_port, "mem");
    } else if (count == 3) {
        send_file("bash get_disk.sh", DISK_FILE, share_disk_port, "disk");
    } else if (count == 4) { //SysInfo
        send_file("bash get_sys.sh", SYS_FILE, share_sys_port, "sys");
    } else if (count == 5) { //user
        send_file("bash get_user.sh", USER_FILE, share_user_port, "user");
    } else { //Proc
        send_file("bash get_proc.sh", PROC_FILE, share_proc_port, "proc");
    }
}

int socket_create() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        DBG("socket_create error!\n");
        return -1;
    }
    return sockfd;
}


void socket_bind(int sockfd, int port) {
    struct sockaddr_in sock_addr;
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sock_addr.sin_port = htons(port);
    if (bind(sockfd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0) {
        DBG("bind error!\n");
    } else {
        //printf("bind ok\n");  
    }
}

void socket_listen(int sockfd) {
    if (listen(sockfd, 20) < 0) {
        close(sockfd);
        DBG("listen error\n");
        //输出错误信息
    } else {
        DBG("listen ok\n");
    }
    return ;
}

int socket_accept(int sockfd, struct sockaddr_in *server_addr, socklen_t *len) {
    sockfd = accept(sockfd, (struct sockaddr *)server_addr, len);
    if (sockfd < 0) {
        DBG("accept error\n");
        //输出错误信息到日志
    } else {
        DBG("accept ok\n");
    }
    return sockfd;
}

void set_socket(int sockfd) {
    int Yes = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &Yes, sizeof(int)) == -1) {
        close(sockfd);
        DBG("setsockopt error\n");
    }
}

void get_file(char *bash_file, char *save_addr) {
    char buffer[1000] = {0};
    char temp[50] = {0};
    FILE *f = popen(bash_file, "r");
    while (fscanf(f, "%s", temp) != EOF) {
        strcat(buffer, temp);
        strcat(buffer, " ");
        if (strcmp(temp, "Waring") == 0) send_warning(buffer);
        memset(temp, 0, 50);
    }
    strcat(buffer, "\n");
    pclose(f);
    FILE *save_file = fopen(save_addr, "a+");
    fprintf(save_file, "%s", buffer);
    memset(buffer, 0, sizeof(buffer));
    fclose(save_file);
}


void send_file(char *bash, char *save_file, int port, char *file_type) {
    int i = 0;
    while (1) {
        i += 1;
        if (i % 10 != 0) {
            sleep(0.1);
            get_file(bash, save_file);
            continue;
        } else {
            get_file(bash, save_file);
            int sockfd = socket_create();
            set_socket(sockfd);
            struct sockaddr_in sock_addr;
            socklen_t len = sizeof(struct sockaddr_in);
            sock_addr.sin_family = AF_INET;
            sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
            sock_addr.sin_port = htons(port);
            if (bind(sockfd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0) {
                DBG("bind error!\n");
                return ;
            }
            socket_listen(sockfd);
            printf("开始监听: %s\n", file_type);
            int rec_sockfd = socket_accept(sockfd, &sock_addr, &len);   //所以这儿又得开新套解字去接收监听结果 不用create码
            char rec_buff[1000] = {0};
            fflush(stdout);
            sleep(0.3);
            recv(rec_sockfd, rec_buff, 1000, 0);
            if (strcmp(rec_buff, file_type) == 0) {
                send(rec_sockfd, "ok", 2, 0);
            } else {
                send(rec_sockfd, "notok", 7, 0);
            }
            sleep(0.3);
            memset(rec_buff, 0, 1000);
            FILE *disk_f = fopen(save_file, "r");
            flock(disk_f -> _fileno, LOCK_EX); //互斥锁
            sleep(5); //又睡？
            while (fgets(rec_buff, 1000, disk_f)) {
                send(rec_sockfd, rec_buff, strlen(rec_buff), 0);
                memset(rec_buff, 0, 1000);
            }
            fflush(stdout);
            flock(disk_f -> _fileno, LOCK_UN); //解锁
            fclose(disk_f);
            //清空文件
            FILE *del_disk_f = fopen(save_file, "w");
            fclose(del_disk_f);
            close(rec_sockfd);
            close(sockfd);
            DBG("close ok\n");
            i = 0;
            sleep(0.1);
        }
    } 
}


void send_warning(char *buffer) {
    int sockfd = socket_create();
    struct sockaddr_in client_addr;
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(share_waring_port);
    client_addr.sin_addr.s_addr = inet_addr(HOSTIP);
    int waring_sockfd = connect(sockfd, (struct sockaddr *)&client_addr, sizeof(client_addr));
    if (waring_sockfd < 0) {
        DBG("connect error\n");
        return ;
    }
    send(waring_sockfd, buffer, strlen(buffer), 0);
    return ;
}
