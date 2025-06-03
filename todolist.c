#include<stdio.h> 
#include<stdlib.h>

#define MAX_TASKS 10
#define MAX_LENGTH 100

char toDoList[MAX_TASKS][MAX_LENGTH];
int taskCount = 0;

void addtask();
void printlist();

int main() {

    int choice = 0;
    printf("Welcome to a todolist app!\n");

    while(1) {
        printf("Please enter a number from the following choices:\n");
        printf("1.)Add a Task\n2.)Show all tasks\n3.)quit\n");
        scanf("%d", &choice);

        switch(choice) {
        case 1:
            addtask();
            break;
        case 2:
            printlist();
            break;
        case 3:
            return 0;
            break;
        default:
            printf("NOT A VALID CHOICE!");
        } 
       
    }
    return 0;
}

void addtask() {
    if(taskCount >= MAX_TASKS) {
        printf("The task list is full!\n");
        return;
    }
    printf("Enter your task: ");
    getchar();
    fgets(toDoList[taskCount], MAX_LENGTH, stdin);
    taskCount++;
}

void printlist() {

    for(int i = 0; i < taskCount;  i++) {
       printf("%s", toDoList[i]);
    }
}