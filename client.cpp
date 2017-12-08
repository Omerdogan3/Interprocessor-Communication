/*
How to Run my code?
1. Compile server.cpp and client.cpp by typing
g++ client.cpp -o client.o -lpthread
g++ server.cpp -o server.o -lpthread

2. Connect server by typing
./server.o

3. Then start multiple clients and enter different
usernames for each (For same usernames program prints
empty strings as expected from us.)

4. After all, you can send and receive texts from clients.

// Development environment: Ubuntu 16.04 - ITU SSH servers

Tested in: gcc 4.9.2 & 4.8.5

Everthing Should Work Fine!
*/


#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

using namespace std;

#define size     180
char filename[50];

typedef struct  {
         long    type;
         char    text[size];
         }msg_obj;

void *fifo_reader(void *a){
int read_bytes;
char fifo_buf[500];
     for( ; ;){
     int f=open(filename,O_RDONLY);
     read_bytes=read(f,fifo_buf,500);
     strcpy(fifo_buf+read_bytes,"");
     if(strcmp(fifo_buf,"server connection closed")==0){
        cout<<fifo_buf<<endl;
        break;
       }
      cout<<fifo_buf<<endl;     
 }
int error=pthread_kill(pthread_self(),SIGSTOP);
}

class sender {
private:
    int queue_id;
    int queue_flag;
    key_t key;
    msg_obj message;
    size_t buf_length;
    char *user_name;   
public:

  sender(){
     queue_flag = IPC_CREAT | 0666;
     key=150130208;
     user_name=new char[10];
     if ((queue_id=msgget(key, queue_flag))==-1) {
        perror("error");  
     }
     else 
     fprintf(stderr,"succeeded: queue id = %d\n", queue_id);
  }

  void send_message(char *str){
     message.type=1;
     if(strncmp(str,"logout",6)==0){
     sprintf(message.text, "logout:%s",user_name);
     buf_length = strlen(message.text)+1;
       if (msgsnd(queue_id, &message, buf_length, IPC_NOWAIT) ==-1) {
        perror("error:");
        }
        else 
        printf("logout request Sent\n");
     }
     
      else{ 
        sprintf(message.text, "%s:%s",user_name,str);        
        buf_length = strlen(message.text)+1;
        if (msgsnd(queue_id, &message, buf_length, IPC_NOWAIT) ==-1) {
        perror("msgsnd");
        }
        else 
        printf("Message Sent\n");
   }
}
 void send_req(){
   cout<<"Enter a username:\n";
   cin.getline(user_name,10);
   sprintf(message.text, "login:%s",user_name);
   buf_length = strlen(message.text)+1;
   message.type=1;
   buf_length = strlen(message.text)+1;
   sprintf(filename,"FIFO_%s",user_name);
   mkfifo(filename, 0666);
   cout<<filename<<" "<<"created\n";
   pthread_t tid;
   pthread_create(&tid,NULL,fifo_reader,(void*)NULL); 
   if (msgsnd(queue_id, &message, buf_length, IPC_NOWAIT) ==-1) {
        perror("message send error:");
    }
   else 
      cout<<"login request sent\n";
 }

};

int main()
{   char mseg[180];
    sender s;
    s.send_req();
    for( ; ;){
    cin.getline(mseg,100);
      if(strcmp(mseg,"q")==0){
       strcpy(mseg,"Logout");  
       s.send_message(mseg);
       break;
      }
      else{
         
       s.send_message(mseg); 
      
       }  
    } 
  return 0;
}
 

