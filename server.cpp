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
#include <stdlib.h>
#include <signal.h>
using namespace std;

#define size     180

typedef struct  {
         long    type;
         char    text[size];
         }msg_obj;

class Reciever {
 private :
  
    char **rec_list;
    int queue_id;
    int queue_flag;
    key_t key;
    msg_obj message;
    size_t buf_length;
    char *user_name;
    int last;
     int f;

 public :
 Reciever (){
     this->f=0;
     this->key=150130208;
     this->rec_list= new char*[50]; 
     this->last=0;
     this->queue_flag=IPC_CREAT | 0666;
      
      if ((queue_id=msgget(key,queue_flag))==-1){
        perror("error: msgget failed");   
      }
      else{
       cout<<"Message Queue generated\n";
      }

 }

 void remove_user(char *str){
  char temp[20];   
  cout<<"removing FIFO_"<<str<<endl;
  sprintf(temp,"FIFO_%s",str);
  int error=remove(temp);
  for(int i=0 ; i<this->last ; i++){  
   if(strcmp(rec_list[i],str)==0){
   strcpy(rec_list[i],"");
     for(int j=i;j<this->last-1;j++){
        strcpy(rec_list[j],rec_list[j+1]);
     }
     free(rec_list[this->last-1]);
     this->last--;
     }
   }
 }


 void remove_all_users(){
   char temp[20];
   for(int i=0;i<last;i++){
   sprintf(temp,"FIFO_%s",rec_list[i]);
   cout<<"removing "<<temp<<endl;
   int eror=remove(temp);
   free(rec_list[i]);
   }
 }

 void add_user(){
  rec_list[this->last]=new char[10];
  strcpy(rec_list[this->last],message.text+6);
   cout<<rec_list[this->last]<<" connected"<<endl;
   last++;
 }
 
 void send_message(char *str){

   int fid;
   char filename[15];
    if(this->f==0){
    for(int i=0;i<last;i++){
     sprintf(filename,"FIFO_%s",rec_list[i]);
     mkfifo(filename, 0666);
     strcpy(filename,"");   
    }
    
    this->f++;
    
    }
   int write_byte;   
   for(int i=0;i<this->last;i++){ 
       sprintf(filename,"FIFO_%s",rec_list[i]);
    while(1){
      fid=open(filename,O_WRONLY);
      char temp[10];
      strcpy(temp,filename+5);
      if(strncmp(str,temp,strlen(temp))==0)
      break;
      write_byte=write(fid,str,strlen(str));
      if (write_byte==0 || write_byte){
      
        if(write_byte==-1){
           perror("error:");
          cout<<"failed"<<endl;
      
       }
      break;
     }
   }
   strcpy(filename,""); 
   close(fid);
   } 
 }

 void recieve_message(){
  int temp;
  if (temp=msgrcv(this->queue_id,&message, 180, 1, this->queue_flag)==-1)
  perror("error: message recieving  failed");
  if(strncmp(message.text,"login",5)==0){
    this->add_user();
  }
  else if(strncmp(message.text,"logout",6)==0){
   this->remove_user(message.text+7);
  }
  else{
     this->send_message(message.text);  
  }
 }

};

void *ftn(void *a){
Reciever *r=(Reciever*)a;
char temp[10];

for( ; ;){

cin.getline(temp,10);
if(strcmp(temp,"q")==0)
break;

}
char mes[180];
strcpy(mes,"server connection closed");
r->send_message(mes);
r->remove_all_users();
int error=pthread_kill(pthread_self(),SIGSTOP);

}


int main(){
    pthread_t tid;
    Reciever *m=new Reciever();   
    pthread_create(&tid,NULL,ftn,(void*)m);
 while(1){
     m->recieve_message();
     
   }

return 0;

}



