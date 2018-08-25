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

#define share_con_port 9000
#define share_cpu_port 10000
#define share_mem_port 10001
#define share_disk_port 10002

#define CPU_FILE "/home/ddwt/文档/HZ/海贼项目/socket通信/Tcp项目/本机测试数据/cpu"
#define DISK_FILE "/home/ddwt/文档/HZ/海贼项目/socket通信/Tcp项目/本机测试数据/disk"
#define MEM_FILE "/home/ddwt/文档/HZ/海贼项目/socket通信/Tcp项目/本机测试数据/mem"

int socket_create();
int socket_bind(int, int);
void socket_listen(int);
int socket_accept(int, struct sockaddr_in *, socklen_t *);
void set_socket(int sockfd);
void get_file(char *, char *);


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
    for (int i = 0; i < 6; i++) {
        count++;
        PID = fork();
        if (PID == 0) break;
    }
    
    if (PID != 0) {
        int a = 0;
        while (scanf("%d", &a) != EOF) {
            printf("父进程不能死！");
        }
    };
    if (count == 1) {
        while (1) {
            get_file("bash get_cpu.sh", CPU_FILE);
            int cpu_sockfd = socket_create();
            set_socket(cpu_sockfd);
            socket_bind(cpu_sockfd, share_cpu_port);
            socket_listen(cpu_sockfd);
            printf("开启监听\n");
            int rec_cpu_sockfd = socket_accept(cpu_sockfd, &server_addr, &len);   //所以这儿又得开新套解字去接收监听结果 不用create码

            //rec_sockfd = socket_accept(sockfd, &server_addr, &len);
            char rec_buff[1000] = {0};
            fflush(stdout);
            sleep(0.3);
            recv(rec_cpu_sockfd, rec_buff, 1000, 0);
            if (strcmp(rec_buff, "cpu") == 0) {
                printf("receive cpu\n");
                int ans = send(rec_cpu_sockfd, "ok", 2, 0);
                //if (ans > 0) printf("send ok done\n"); 
            } else {
                send(rec_cpu_sockfd, "notcpu", 6, 0);
            }
            memset(rec_buff, 0, 1000);

            //FILE *cpu_f = fopen("/home/ddwt/cpu", "r");
            if (rec_cpu_sockfd > 0) printf("链接专用端口成功\n");
            else printf("链接专用端口失败\n");
            FILE *cpu_f = fopen(CPU_FILE, "r");
            flock(cpu_f -> _fileno, LOCK_EX); //互斥锁
            sleep(0.3); //又睡？
            while (fgets(rec_buff, 1000, cpu_f)) {
                send(rec_cpu_sockfd, rec_buff, strlen(rec_buff), 0);
                memset(rec_buff, 0, 1000);
            }
            fflush(stdout);
            //DBG("send ok\n");
            flock(cpu_f -> _fileno, LOCK_UN); //解锁
            fclose(cpu_f);
            //清空文件
            //FILE *del_cpu_f = fopen("/home/ddwt/cpu", "w");
            //FILE *del_cpu_f = fopen(CPU_FILE, "w");
            //fclose(del_cpu_f);
            close(rec_cpu_sockfd);
            close(cpu_sockfd);
            DBG("close ok\n");
        }
    } else if (count == 2) {
        while (1) {
            get_file("bash get_memory.sh", MEM_FILE);
            int mem_sockfd = socket_create();
            set_socket(mem_sockfd);
            socket_bind(mem_sockfd, share_mem_port);
            socket_listen(mem_sockfd);
            int rec_mem_sockfd = socket_accept(mem_sockfd, &server_addr, &len);   //所以这儿又得开新套解字去接收监听结果 不用create码       
            char rec_buff[1000] = {0};
            fflush(stdout);
            sleep(0.3);
            recv(rec_mem_sockfd, rec_buff, 1000, 0);
            if (strcmp(rec_buff, "mem") == 0) {
                send(rec_mem_sockfd, "ok", 2, 0);
            } else {
                send(rec_mem_sockfd, "notmem", 6, 0);
            }
            memset(rec_buff, 0, 1000);
            if (rec_mem_sockfd > 0) printf("链接专用端口成功\n");
            else printf("链接专用端口失败\n");
            //FILE *mem_f = fopen("/home/ddwt/mem", "r");
            FILE *mem_f = fopen(MEM_FILE, "r");
            flock(mem_f -> _fileno, LOCK_EX); //互斥锁
            sleep(0.3); //又睡？
            while (fgets(rec_buff, 1000, mem_f)) {
                send(rec_mem_sockfd, rec_buff, strlen(rec_buff), 0);
                memset(rec_buff, 0, 1000);
            }
            //DBG("send ok\n");
            fflush(stdout);
            flock(mem_f -> _fileno, LOCK_UN); //解锁
            fclose(mem_f);
            //清空文件
            //FILE *del_mem_f = fopen("/home/ddwt/mem", "w");
            //FILE *del_mem_f = fopen(MEM_FILE, "w");
            //fclose(del_mem_f);
            close(rec_mem_sockfd);
            close(mem_sockfd);
            DBG("close ok\n");
        }
    } else if (count == 3) {
        while (1) {
            get_file("bash get_disk.sh", DISK_FILE);
            int disk_sockfd = socket_create();
            set_socket(disk_sockfd);
            socket_bind(disk_sockfd, share_disk_port);
            socket_listen(disk_sockfd);
            int rec_disk_sockfd = socket_accept(disk_sockfd, &server_addr, &len);   //所以这儿又得开新套解字去接收监听结果 不用create码
            char rec_buff[1000] = {0};
            fflush(stdout);
            sleep(0.3);
            recv(rec_disk_sockfd, rec_buff, 1000, 0);
            if (strcmp(rec_buff, "disk") == 0) {
                send(rec_disk_sockfd, "ok", 2, 0);
            } else {
                send(rec_disk_sockfd, "notdisk", 7, 0);
            }
            memset(rec_buff, 0, 1000);
            if (rec_disk_sockfd > 0) printf("链接专用端口成功\n");
            else printf("链接专用端口失败\n");
            //FILE *disk_f = fopen("/home/ddwt/disk", "r");
            FILE *disk_f = fopen(DISK_FILE, "r");
            flock(disk_f -> _fileno, LOCK_EX); //互斥锁
            sleep(0.3); //又睡？
            while (fgets(rec_buff, 1000, disk_f)) {
                send(rec_disk_sockfd, rec_buff, strlen(rec_buff), 0);
                memset(rec_buff, 0, 1000);
            }
            fflush(stdout);
            //DBG("send ok\n");
            flock(disk_f -> _fileno, LOCK_UN); //解锁
            fclose(disk_f);
            //清空文件
            //FILE *del_disk_f = fopen("/home/ddwt/mem", "w");
            //FILE *del_disk_f = fopen(DISK_FILE, "w");
            //fclose(del_disk_f);
            close(rec_disk_sockfd);
            close(disk_sockfd);
            DBG("close ok\n");
        }
    } else if (count == 4) {
        return 0;
    } else if (count == 5) {
        return 0;
    }
    

 
    return 0;
}

int socket_create() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        DBG("socket_create error!\n");
        return -1;
    }
    return sockfd;
}


int socket_bind(int sockfd, int port) {
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
    while (fgets(temp, sizeof(temp), f)) {
        strcat(buffer, temp);
        //printf("%s\n", temp);
        memset(temp, 0, 50);
    }
    pclose(f);
    //printf("%s\n", buffer);
    FILE *save_file = fopen(save_addr, "a+");
    fprintf(save_file, "%s", buffer);
    memset(buffer, 0, sizeof(buffer));
    fclose(save_file);
    //printf("获取数据完成\n");
}
