#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char id[50];
    char name[50];
    char phone[20];
    char email[100];
} Contact;

int get_menu_choice(void);
void menu(Contact **contacts, int *count, int *capacity, int *next_id);
void input_contacts(Contact *contact);
void generate_id(Contact *contact, int *next_id);
int add_contact(Contact **contacts, int *count, int *capacity);
void print_contacts(Contact *contacts, int count);
int find_contact(Contact *contacts, int count, char *id);
void search_contacts(Contact *contacts, int count);
void update_contact_details(Contact *contact);
void edit_contact(Contact *contacts, int count);
void delete_contact(Contact *contacts, int *count);
void sort_contacts(Contact *contacts, int count);
void save_contacts(Contact *contacts, int count);
void load_contacts(Contact **contacts, int *count, int *capacity, int *next_id);

int get_menu_choice(void)
{
    char input[20];
    char *end;
    long choice;

    while (1) {

        printf("\nEnter your choice: ");

        if (fgets(input, sizeof(input), stdin) == NULL) {
            return 0;
        }

        choice = strtol(input, &end, 10);

        while (*end == ' ' || *end == '\t') {
            end++;
        }

        if (*end == '\n' || *end == '\0') {

            if (choice >= 0 && choice <= 5) {
                return (int)choice;
            }
        }

        printf("Invalid choice. Please enter a number from 0 to 5.\n");
    }
}

void menu(Contact **contacts, int *count, int *capacity, int *next_id)
{
    int choice;

    do {

        for(int i = 0; i < 55; i++){
            printf("=");
        }
        printf("\n");

        printf("\t    CONTACT DATABASE DIRECTORY MANAGER\n");

        for(int i = 0; i < 55; i++){
            printf("=");
        }
        printf("\n");

        printf("\n1. Add Contact");
        printf("\n2. View All Contacts");
        printf("\n3. Search Contact");
        printf("\n4. Edit Contact");
        printf("\n5. Delete Contact");
        printf("\n0. Exit");

        choice = get_menu_choice();

        switch (choice) {

            case 1:

                if (add_contact(contacts, count, capacity)) {

                    input_contacts(&(*contacts)[*count]);
                    generate_id(&(*contacts)[*count], next_id);

                    (*count)++;

                    sort_contacts(*contacts, *count);
                    save_contacts(*contacts, *count);

                    printf("Contact added successfully.\n");
                }

                break;

            case 2:
                print_contacts(*contacts, *count);
                break;

            case 3:
                search_contacts(*contacts, *count);
                break;

            case 4:
                edit_contact(*contacts, *count);
                break;

            case 5:
                delete_contact(*contacts, count);
                break;

            case 0:
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 0);
}

void input_contacts(Contact *contact){

    printf("\nEnter the Name: ");
    if (fgets(contact->name, sizeof(contact->name), stdin) != NULL){
        contact->name[strcspn(contact->name, "\n")] = '\0';
    }

    printf("\nEnter the Phone Number: ");
    if (fgets(contact->phone, sizeof(contact->phone), stdin) != NULL){
        contact->phone[strcspn(contact->phone, "\n")] = '\0';
    }

    printf("\nEnter the Email ID: ");
    if (fgets(contact->email, sizeof(contact->email), stdin) != NULL){
        contact->email[strcspn(contact->email, "\n")] = '\0';
    }
}

void generate_id(Contact *contact, int *next_id)
{
    sprintf(contact->id, "CONTACT-%03d", *next_id);
    (*next_id)++;
}


int add_contact(Contact **contacts, int *count, int *capacity){

    for(int i = 0; i < 55; i++){
        printf("=");
    }
    printf("\n");

    printf("\t\t\tADD CONTACT\n");

    for(int i = 0; i < 55; i++){
        printf("=");
    }
    printf("\n");

    int new_capacity;

    if (*count == *capacity) {

        Contact *temp;
        new_capacity = *capacity * 2;

        temp = realloc(*contacts, new_capacity * sizeof(Contact));

        if (temp == NULL){
            printf("\nMemory reallocation failed.\n");
            return 0;
        }

        *contacts = temp;
        *capacity = new_capacity;
    }
    return 1;
}

void print_contacts(Contact *contacts, int count){

    for(int i = 0; i < 55; i++){
        printf("=");
    }
    printf("\n");

    printf("\t\t\tALL CONTACTS\n");

    for(int i = 0; i < 55; i++){
        printf("=");
    }
    printf("\n");

    if (count == 0) {
        printf("\nNo contacts available.\n");
        return;
    }

    for(int i = 0; i < count; i++){

        printf("\nID            : %s\n", contacts[i].id);
        printf("Name          : %s\n", contacts[i].name);
        printf("Phone Number  : %s\n", contacts[i].phone);
        printf("Email ID      : %s\n", contacts[i].email);
        printf("-------------------------------------------------------");

        printf("\n");

    }
}

int find_contact(Contact *contacts, int count, char *id){

    for (int i = 0; i < count; i++){
        if (strcmp(id, contacts[i].id) == 0){
            return i;
        }
    }
    return -1;
}

void search_contacts(Contact *contacts, int count){

    for(int i = 0; i < 55; i++){
        printf("=");
    }
    printf("\n");

    printf("\t\t\tSEARCH CONTACT\n");

    for(int i = 0; i < 55; i++){
        printf("=");
    }
    printf("\n");

    char id[50];

    printf("\nEnter the ID: ");
    if (fgets(id, sizeof(id), stdin) != NULL){
        id[strcspn(id, "\n")] = '\0';
    }

    int index = find_contact(contacts, count, id);

    if (index == -1) {
        printf("Contact not found.\n");
    }
    else {
        printf("Contact found!\n");
        printf("Name: %s\n", contacts[index].name);
        printf("ID: %s\n", contacts[index].id);
        printf("Phone Number: %s\n", contacts[index].phone);
        printf("Email ID: %s\n", contacts[index].email);
    }
}

void update_contact_details(Contact *contact){

    printf("\nEnter new Name: ");
    fgets(contact->name, sizeof(contact->name), stdin);
    contact->name[strcspn(contact->name, "\n")] = '\0';

    printf("\nEnter new Phone Number: ");
    fgets(contact->phone, sizeof(contact->phone), stdin);
    contact->phone[strcspn(contact->phone, "\n")] = '\0';

    printf("\nEnter new Email ID: ");
    fgets(contact->email, sizeof(contact->email), stdin);
    contact->email[strcspn(contact->email, "\n")] = '\0';
}

void edit_contact(Contact *contacts, int count){

    for(int i = 0; i < 55; i++){
        printf("=");
    }
    printf("\n");

    printf("\t\t\tEDIT CONTACT\n");

    for(int i = 0; i < 55; i++){
        printf("=");
    }
    printf("\n");

    char id[50];

    printf("\nEnter the ID: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = '\0';

    int index = find_contact(contacts, count, id);

    if (index == -1){
        printf("Contact not found.\n");
        return;
    }

    update_contact_details(&contacts[index]);

    sort_contacts(contacts, count);
    save_contacts(contacts, count);

    printf("\nContact updated successfully.\n");
}

void delete_contact(Contact *contacts, int *count)
{

    for(int i = 0; i < 55; i++){
        printf("=");
    }
    printf("\n");

    printf("\t\t    DELETE CONTACT\n");

    for(int i = 0; i < 55; i++){
        printf("=");
    }
    printf("\n");

    char id[50];

    printf("\nEnter the ID: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = '\0';

    int index = find_contact(contacts, *count, id);

    if (index == -1) {
        printf("Contact not found.\n");
        return;
    }

    for (int i = index; i < *count - 1; i++) {
        contacts[i] = contacts[i + 1];
    }

    (*count)--;

    sort_contacts(contacts, *count);
    save_contacts(contacts, *count);

    printf("\nContact deleted successfully.\n");
}

void sort_contacts(Contact *contacts, int count)
{
    for (int i = 0; i < count - 1; i++) {

        for (int j = 0; j < count - i - 1; j++) {

            if (strcmp(contacts[j].id, contacts[j + 1].id) > 0) {

                Contact temp = contacts[j];
                contacts[j] = contacts[j + 1];
                contacts[j + 1] = temp;
            }
        }
    }
}

void save_contacts(Contact *contacts, int count)
{
    FILE *file = fopen("contacts.txt", "w");

    if (file == NULL) {
        printf("File could not be opened.\n");
        return;
    }

    fprintf(file, "=======================================================\n");
    fprintf(file, "            CONTACT DATABASE DIRECTORY\n");
    fprintf(file, "=======================================================\n\n");

    for (int i = 0; i < count; i++) {

        fprintf(file, "Contact %d\n", i + 1);
        fprintf(file, "-------------------------------------------------------\n");
        fprintf(file, "ID            : %s\n", contacts[i].id);
        fprintf(file, "Name          : %s\n", contacts[i].name);
        fprintf(file, "Phone Number  : %s\n", contacts[i].phone);
        fprintf(file, "Email ID      : %s\n", contacts[i].email);
        fprintf(file, "=======================================================\n\n");
    }

    fclose(file);

    printf("Contacts saved successfully.\n");
}

void load_contacts(Contact **contacts, int *count, int *capacity, int *next_id)
{
    FILE *file = fopen("contacts.txt", "r");

    if (file == NULL) {
        return;
    }

    char line[150];
    Contact contact;

    while (fgets(line, sizeof(line), file) != NULL) {

        if (strncmp(line, "ID            : ", 16) == 0) {

            sscanf(line + 16, "%49[^\n]", contact.id);

            fgets(line, sizeof(line), file);
            sscanf(line + 16, "%49[^\n]", contact.name);

            fgets(line, sizeof(line), file);
            sscanf(line + 16, "%19[^\n]", contact.phone);

            fgets(line, sizeof(line), file);
            sscanf(line + 16, "%99[^\n]", contact.email);

            if (*count == *capacity) {

                int new_capacity = *capacity * 2;

                Contact *temp = realloc(
                    *contacts,
                    new_capacity * sizeof(Contact)
                );

                if (temp == NULL) {
                    printf("Memory reallocation failed.\n");
                    fclose(file);
                    return;
                }

                *contacts = temp;
                *capacity = new_capacity;
            }

            (*contacts)[*count] = contact;
            (*count)++;
        }
    }

    fclose(file);

    int max_id = 0;

    for (int i = 0; i < *count; i++) {

        int id_number;

        if (sscanf((*contacts)[i].id, "CONTACT-%d", &id_number) == 1) {

            if (id_number > max_id) {
                max_id = id_number;
            }
        }
    }

    *next_id = max_id + 1;
}

int main()
{
    Contact *contacts = NULL;
    int count = 0;
    int capacity = 2;
    int next_id = 1;

    contacts = malloc(capacity * sizeof(Contact));

    if (contacts == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    load_contacts(&contacts, &count, &capacity, &next_id);

    menu(&contacts, &count, &capacity, &next_id);

    free(contacts);

    return 0;
}