#include "p6.h"

void shared_memory_maker();
void sig_handler(int signal);


struct Shmem* shmem;
key_t skey, mkey;
int sid, mqid;


std::string error_msg;
std::string exe_name;

int main(int argc, char* argv[]) {
    signal(SIGTERM, sig_handler); 
    exe_name = argv[0];
    error_msg += exe_name;

    void shared_memory_maker();


    printf("hello from %s!\n", "user");


    shmdt(shmem);

    return 0;
}


void shared_memory_maker() {
    if ((skey = ftok("makefile", 'a')) == (key_t)-1) {
        error_msg += ":skey/ftok - creating::";
        perror(error_msg.c_str());
        exit(EXIT_FAILURE);
    }

    if ((sid = shmget(skey, sizeof(struct Shmem), 0)) == -1) {
        error_msg += "sid/shmget - allocation failure::";
        perror(error_msg.c_str());
        exit(EXIT_FAILURE);
    }

    shmem = (struct Shmem*)shmat(sid, NULL, 0);

    if ((mkey = ftok("makefile", 'b')) == (key_t)-1) {
        error_msg = ":mkey/ftok - creating msg queue";
        perror(error_msg.c_str());
        exit(EXIT_FAILURE);
    }

    if ((mqid = msgget(mkey, 0644)) == -1) {
        error_msg = "mqid/msgget - allocate msg que failure::";
        perror(error_msg.c_str());
        exit(EXIT_FAILURE);
    }
}


void sig_handler(int signal) { 
    if (signal == 15) { // ctrl c;
        exit(EXIT_FAILURE);
    }
}