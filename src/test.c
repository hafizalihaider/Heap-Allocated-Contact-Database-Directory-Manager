#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct {
    int id;
    char name[50];
    char phone[20];
    char email[100];
} Contact;

int main(){

    Contact person;

    Contact *p = &person;
    Contact **pp = &p;

    (*pp)->id = 2;

    printf("ID %d",p->id);
    printf("ID %d",(*pp)->id);
}