#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>

int main (int argc, char* argv[]){
    int t = 0;
    int increment = 1;

//process p increments by 1 every 2second due to internal events running   
    while (t < 5){
        sleep(2);
        t = t + increment;
        printf("t = %d\n", t);
    }
//Q requests P to send a message
//generate random number between 0 and 9
    srand(time(NULL));
    int randNum = (rand() % 10) * 0;
    //int msg1[2] = {randNum, t};

//P only sends to Q if randNum is even
    if (randNum %2 == 0){
        printf ("\n randNum is even, sending msg1 to Q\n");
    //create fifo file to write the msg1 for q to read
        if (mkfifo("clock", 0777) == -1 && errno != EEXIST){
            return 1;
        }
//open the fifo file to write to Q
        int fd = open("clock", O_WRONLY);
        if (fd == -1){
            return 2;
        }
//write to the fifo file
//increment t by 1 and attach it to message
        t = t + increment;
        int msg1[2] = {t, randNum};
        if (write(fd, &msg1, sizeof(int)) == -1){
            return 3;
        }
        close(fd);

        printf("\nMessage sent to Q=%d at time t=%d\n", msg1[1], msg1[0]);
    } else {
        printf("\nrandNum is odd, not sending to Q. Run the program until random number, randNum, is even\n");
    }
    
//open the fifo file to read from Q
    int fd = open("clock", O_RDONLY);
    if (fd == -1){
        return 4;
    }
//read from the fifo file
    int msg2[2];
    if (read(fd, &msg2, sizeof(int) == -1)){
        return 5;
    }
    close(fd);
    
    printf("\nMessage received from Q=%d\n", msg2[1]);
    printf("Time t received from t=%d\n", msg2[0]);
    printf("local time at P is t = %d\n", t);
    //update time t;
    if(msg2[0] > t){
        t = msg2[0] + 1;
        printf("\nupdated time t = %d\n", t);
     }else{
        printf("\nnew t from Q is not > local t, so t is not updated. New t=%d\n", msg2[0]);
     }
    
    return 0;
}

//Processes P and Q with different logical clocks 
//Increment at different rates
//P increments every 2 seconds
//Q increments every 3 seconds
//P and Q sends each other a message at random
//When message is received, the clock is updated
//Q should only request P to send a message when random number is even 