/*  Author: Michael Trani
    May 2022
    Process Master      */

#include "p6.h"

void shared_memory_maker();
void free_memory();


static std::string logfile = "logfile";
static FILE* fptr;

struct Shmem* shmem;
key_t skey, mkey;
int sid;
int mqid;



std::string error_msg; 
std::string exe_name; 

int main(int argc, char* argv[]) {

    exe_name = argv[0];
    error_msg += exe_name;
    fptr = fopen(logfile.c_str(), "w+");

    int option;
    while ((option = getopt(argc, argv, "m:h")) != -1) {
        switch (option) {
        case 'm':
            break;
        case 'h':
            std::cout << "Your program should take in a command line optionion -m x, which accepts either a 0 or a 1, which determines how child processes will perform their memory access\n";
            return 0;
        default:
            break;
        }
    }

    shared_memory_maker();

    pid_t childpid;

    switch (childpid = fork()) {
    case -1:
        error_msg += "Failed to fork";
        perror(error_msg.c_str());
        exit(EXIT_FAILURE);

    case 0:
        execl("./user", "user",  '\0');
        error_msg = exe_name + ": Error: failed to execl";
        perror(error_msg.c_str());
        exit(EXIT_FAILURE);
    }



    sleep(1);


    if (fptr)
        fclose(fptr);
    fptr = NULL;
    free_memory();

    return 0;
}


// builds shared memory
void shared_memory_maker() {

    // ALLOCATE OR ATTACH TO SHARED MEMORY
    if ((skey = ftok("makefile", 'a')) == (key_t)-1) {
        error_msg += ":Shared Memory/skey/ftok - creating::";
        perror(error_msg.c_str());
        exit(EXIT_FAILURE);
    }
    if ((sid = shmget(skey, sizeof(struct Shmem), IPC_CREAT | 0666)) == -1) {
        error_msg += ":Shared Memory/sid/shmget - creating::";
        perror(error_msg.c_str());
        exit(EXIT_FAILURE);
    }

    shmem = (struct Shmem*)shmat(sid, NULL, 0);


    // SET UP MESSAGE QUEUE
    if ((mkey = ftok("makefile", 'b')) == (key_t)-1) {
        error_msg += ":Message Queue/mkey/ftok - creating";
        perror(error_msg.c_str());
        free_memory();
        exit(EXIT_FAILURE);
    }
    // Create the queue
    if ((mqid = msgget(mkey, 0644 | IPC_CREAT)) == -1) {
        error_msg += ":Creating queue/mqid/msgget - ";
        perror(error_msg.c_str());
        free_memory();
        exit(EXIT_FAILURE);
    }


}


void free_memory() {

    // Performs control operations on the message queue
    if (msgctl(mqid, IPC_RMID, NULL) == -1) { // Removes the message queue
        error_msg += " Could not remove the  message queue";
        perror(error_msg.c_str());
        exit(EXIT_FAILURE);
    }

    shmdt(shmem); 
    shmctl(sid, IPC_RMID, NULL);
    
    printf("[OSS]: Memory cleared.\n");
}