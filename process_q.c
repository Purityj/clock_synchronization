#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h> //
#include <sys/types.h> 
#include <errno.h> 
#include <fcntl.h> //for the open() function
#include <time.h>

int main (int argc, char* argv[]){
    int t = 0;
    int increment = 1;

//process Q increments by 1 every 5 second    
    while (t < 5){
        sleep(3);
        t = t + increment;
        printf("t = %d\n", t);    
    }

//read msg1 from P
//since process P created fifo, no need to create it again
//open fifo to read msg1 from P
     int fd = open("clock", O_RDONLY);
     if (fd == -1){
        return 1;
     } 
//read from the fifo file
    int msg1[2];
    if (read(fd, &msg1, sizeof(int)) == -1){
        return 2;
    } 
    close(fd);
    printf("Message received from P: %d\n", msg1[1]);
    printf("\nTime t received from P is t=%d\n", msg1[0]);
    printf("local time at Q is t = %d\n", t);
//update the time t
     if(msg1[0] > t){
        t = msg1[0] + increment;
        printf("\nupdated time t = %d\n", t);
     }else{
        printf("\nnew t from P is not > local t, so t is not updated. New t=%d\n", msg1[0]);
     }


//P requests Q to send a message
//generate random number between 0 and 9 to establish randomness
    srand(time(NULL));
    int randNum = (rand() % 10) * 0;
printf("\nrandNum = %d\n", randNum);
//Q only sends to P if randNum is odd
    if (randNum % 2 == 0){
        printf ("\nrandNum is odd, sending msg2 to Q\n");
 
//write msg2 to P
//fifo file is already created, no need to create it again 
//open the fifo to write msg2 to P
        int fd = open("clock", O_WRONLY);
        if (fd == -1){
            return 3;
        }
//write to the fifo file
        t = t + increment;
        int msg2[2] = {t, randNum};
        if (write(fd, &msg2, sizeof(int)) == -1){
            return 3;
        }
        close(fd);

        printf("Message sent to P=%d at time t=%d\n", msg2[1], msg2[0]);

    } else {
        printf("\nrandNum is even, not sending to P. Run the program until randNum is odd\n");
    }

    return 0;
}


//STEPS
//Processes P and Q with different logical clocks 
//Increment at different rates
//P increments every 2 seconds
//Q increments every 3 seconds
//P and Q sends each other a message at random
//When message is received, the clock is updated
//Q should only request P to send a message when random number is even 