#include <time.h>
#include <stdio.h>
#include <stdlib.h> 

int main() {
    int guess = 0;
    int answer = 0;

    srand(time(NULL)); 
    answer = rand() % 10 + 1;

    while(1) {
        scanf("%d", &guess);
        if(guess == answer) {
            printf("you won the answer is: %d", answer);
            break;
        } else if(guess < answer) {
            printf("guess higher!");
        }else if(guess > answer) {
            printf("guess lower!") ;
        }
    }

    return 0;


}