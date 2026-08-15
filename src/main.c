/*
 * ============================================================================
 * Project      : Heap-Allocated Contact Database Directory Manager
 * Author       : Muhammad Ali Haider
 * Date         : August 2026
 * Language     : C
 *
 * Description  :
 * A console-based contact management system that stores contacts dynamically
 * on the heap. The program supports adding, viewing, searching, editing,
 * deleting, sorting, saving, and loading contacts.
 *
 * Key Concepts Demonstrated:
 *  - Structures
 *  - Pointers and pointer-to-pointer parameters
 *  - Dynamic memory allocation using malloc() and realloc()
 *  - File handling using fopen(), fprintf(), and fgets()
 *  - String handling
 *  - Input validation
 *  - Searching
 *  - Bubble sort
 *  - Persistent data storage
 *
 * Data Persistence:
 * Contacts are automatically saved to "contacts.txt" after add, edit,
 * and delete operations. Existing contacts are loaded when the program starts.
 *
 * Memory Management:
 * The contact array is allocated dynamically. Its capacity is doubled
 * whenever the current array becomes full.
 *
 * ID Management:
 * Contact IDs are generated automatically in the format:
 *
 *      CONTACT-001
 *      CONTACT-002
 *      CONTACT-003
 *
 * IDs are not reused after deletion. The next available ID is determined
 * from the highest ID stored in the file.
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/*
 * ============================================================================
 * STRUCTURE DEFINITION
 * ============================================================================
 *
 * Contact represents one complete contact record.
 *
 * Each contact contains:
 *  - id    : Automatically generated unique identifier
 *  - name  : Contact's name
 *  - phone : Contact's phone number
 *  - email : Contact's email address
 */
typedef struct {
    char id[50];
    char name[50];
    char phone[20];
    char email[100];
} Contact;


/*
 * ============================================================================
 * FUNCTION PROTOTYPES
 * ============================================================================
 *
 * Function prototypes allow functions to be called before their complete
 * definitions appear in the source file.
 */

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


/*
 * ============================================================================
 * FUNCTION: get_menu_choice()
 * ============================================================================
 *
 * Purpose:
 * Safely reads and validates the user's menu selection.
 *
 * Why fgets() + strtol()?
 * scanf("%d") accepts partial numeric input such as "2abc". That would leave
 * "abc" in the input buffer and could cause unexpected behavior.
 *
 * fgets() reads the complete input line first, while strtol() converts the
 * numeric portion. We then verify that nothing invalid remains.
 *
 * Valid choices:
 *      0 - 5
 */
int get_menu_choice(void)
{
    char input[20];
    char *end;
    long choice;

    while (1) {

        printf("\nEnter your choice: ");

        /*
         * Read the complete input line.
         * If input fails or EOF is reached, exit the menu safely.
         */
        if (fgets(input, sizeof(input), stdin) == NULL) {
            return 0;
        }

        /*
         * Explicitly reject + and - because menu choices must be plain
         * non-negative integers.
         */
        if (input[0] == '+' || input[0] == '-') {
            printf("Invalid choice. Please enter a number from 0 to 5.\n");
            continue;
        }

        /*
         * Convert the string to a long integer.
         * 'end' points to the first character that was not converted.
         */
        choice = strtol(input, &end, 10);

        /*
         * Allow spaces or tabs after the number.
         */
        while (*end == ' ' || *end == '\t') {
            end++;
        }

        /*
         * The input is valid only if:
         * 1. No unexpected characters remain.
         * 2. The value is between 0 and 5.
         */
        if ((*end == '\n' || *end == '\0') &&
            choice >= 0 && choice <= 5) {

            return (int)choice;
        }

        printf("Invalid choice. Please enter a number from 0 to 5.\n");
    }
}


/*
 * ============================================================================
 * FUNCTION: menu()
 * ============================================================================
 *
 * Purpose:
 * Main control interface of the application.
 *
 * This function does not perform contact operations itself. Instead, it
 * delegates each operation to its appropriate function.
 *
 * Pointer-to-pointer:
 *      Contact **contacts
 *
 * A double pointer is required because add_contact() and load_contacts()
 * may use realloc(), which can change the address of the allocated array.
 */
void menu(Contact **contacts, int *count, int *capacity, int *next_id)
{
    int choice;

    do {

        /* Display program header. */
        for(int i = 0; i < 55; i++){
            printf("=");
        }
        printf("\n");

        printf("\t    CONTACT DATABASE DIRECTORY MANAGER\n");

        for(int i = 0; i < 55; i++){
            printf("=");
        }
        printf("\n");

        /* Display available operations. */
        printf("\n1. Add Contact");
        printf("\n2. View All Contacts");
        printf("\n3. Search Contact");
        printf("\n4. Edit Contact");
        printf("\n5. Delete Contact");
        printf("\n0. Exit");

        choice = get_menu_choice();

        switch (choice) {

            case 1:

                /*
                 * Make sure enough heap memory exists before writing the
                 * new contact.
                 */
                if (add_contact(contacts, count, capacity)) {

                    /*
                     * Store user information in the newly available array
                     * position.
                     */
                    input_contacts(&(*contacts)[*count]);

                    /*
                     * Generate the ID automatically instead of asking the
                     * user to enter it.
                     */
                    generate_id(&(*contacts)[*count], next_id);

                    (*count)++;

                    /*
                     * Keep the in-memory database sorted and immediately
                     * synchronize it with the text file.
                     */
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
                /*
                 * This should normally never execute because
                 * get_menu_choice() already validates the input.
                 */
                printf("Invalid choice.\n");
        }

    } while (choice != 0);
}


/*
 * ============================================================================
 * FUNCTION: input_contacts()
 * ============================================================================
 *
 * Purpose:
 * Reads the user's contact information.
 *
 * fgets() is used instead of scanf("%s") because it safely limits the number
 * of characters read and allows spaces in names.
 *
 * strcspn() removes the newline character that fgets() stores.
 *
 * The ID is deliberately NOT requested here because it is generated
 * automatically by generate_id().
 */
void input_contacts(Contact *contact)
{
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


/*
 * ============================================================================
 * FUNCTION: generate_id()
 * ============================================================================
 *
 * Purpose:
 * Automatically generates a unique sequential contact ID.
 *
 * Example:
 *      next_id = 1  -> CONTACT-001
 *      next_id = 2  -> CONTACT-002
 *
 * The ID counter is incremented after assigning the current ID so that the
 * next contact receives a different identifier.
 */
void generate_id(Contact *contact, int *next_id)
{
    sprintf(contact->id, "CONTACT-%03d", *next_id);

    (*next_id)++;
}


/*
 * ============================================================================
 * FUNCTION: add_contact()
 * ============================================================================
 *
 * Purpose:
 * Ensures that enough heap memory exists for a new contact.
 *
 * Dynamic array strategy:
 *      Initial capacity = 2
 *      When full       -> capacity *= 2
 *
 * realloc() may move the entire array to a new memory location, which is why
 * Contact **contacts is used.
 *
 * Return value:
 *      1 -> memory is available
 *      0 -> allocation failed
 */
int add_contact(Contact **contacts, int *count, int *capacity)
{
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

    /*
     * The array is full when the number of contacts equals its capacity.
     */
    if (*count == *capacity) {

        Contact *temp;

        /*
         * Double the capacity instead of increasing it by one.
         * This significantly reduces the number of reallocations.
         */
        new_capacity = *capacity * 2;

        temp = realloc(*contacts, new_capacity * sizeof(Contact));

        /*
         * Always check realloc() before replacing the original pointer.
         * If realloc() fails, the original memory is still valid.
         */
        if (temp == NULL){
            printf("\nMemory reallocation failed.\n");
            return 0;
        }

        *contacts = temp;
        *capacity = new_capacity;
    }

    return 1;
}


/*
 * ============================================================================
 * FUNCTION: print_contacts()
 * ============================================================================
 *
 * Purpose:
 * Displays every contact currently stored in memory.
 */
void print_contacts(Contact *contacts, int count)
{
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


/*
 * ============================================================================
 * FUNCTION: find_contact()
 * ============================================================================
 *
 * Purpose:
 * Searches the contact array for an exact ID match.
 *
 * Return value:
 *      >= 0 -> array index of the matching contact
 *      -1   -> contact was not found
 *
 * This function is reused by search, edit, and delete operations to avoid
 * duplicating the same search logic.
 */
int find_contact(Contact *contacts, int count, char *id)
{
    for (int i = 0; i < count; i++){

        if (strcmp(id, contacts[i].id) == 0){
            return i;
        }
    }

    return -1;
}


/*
 * ============================================================================
 * FUNCTION: search_contacts()
 * ============================================================================
 *
 * Purpose:
 * Allows the user to search for a contact using its automatically generated
 * ID.
 */
void search_contacts(Contact *contacts, int count)
{
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

    /*
     * find_contact() returns the position of the contact in the array.
     */
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


/*
 * ============================================================================
 * FUNCTION: update_contact_details()
 * ============================================================================
 *
 * Purpose:
 * Replaces the editable information of an existing contact.
 *
 * The ID is intentionally not changed because it is the permanent identifier
 * assigned when the contact was created.
 */
void update_contact_details(Contact *contact)
{
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


/*
 * ============================================================================
 * FUNCTION: edit_contact()
 * ============================================================================
 *
 * Purpose:
 * Finds an existing contact by ID and updates its name, phone number,
 * and email address.
 *
 * The ID remains unchanged.
 */
void edit_contact(Contact *contacts, int count)
{
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

    /*
     * Editing the name does not normally change ID order, but sorting here
     * guarantees the database remains consistently ordered.
     */
    sort_contacts(contacts, count);

    /* Immediately persist the modified database. */
    save_contacts(contacts, count);

    printf("\nContact updated successfully.\n");
}


/*
 * ============================================================================
 * FUNCTION: delete_contact()
 * ============================================================================
 *
 * Purpose:
 * Removes a contact from the dynamic array.
 *
 * After finding the contact, every element after it is shifted one position
 * toward the beginning of the array.
 *
 * Example:
 *
 *      Before:
 *      [A] [B] [C] [D]
 *
 *      Delete B:
 *
 *      [A] [C] [D]
 *
 * The allocated memory itself is not reduced because the array capacity is
 * intentionally retained for future additions.
 */
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

    /*
     * Shift all contacts after the deleted contact one position left.
     */
    for (int i = index; i < *count - 1; i++) {
        contacts[i] = contacts[i + 1];
    }

    (*count)--;

    /*
     * Re-sort and immediately update the persistent file.
     *
     * Notice that the deleted ID is NOT reused.
     */
    sort_contacts(contacts, *count);
    save_contacts(contacts, *count);

    printf("\nContact deleted successfully.\n");
}


/*
 * ============================================================================
 * FUNCTION: sort_contacts()
 * ============================================================================
 *
 * Purpose:
 * Sorts contacts by their automatically generated IDs.
 *
 * Algorithm:
 *      Bubble Sort
 *
 * Bubble sort repeatedly compares neighboring elements and swaps them when
 * they are in the wrong order.
 *
 * Since the IDs follow CONTACT-001, CONTACT-002, etc., lexicographical
 * comparison using strcmp() produces the correct order.
 */
void sort_contacts(Contact *contacts, int count)
{
    for (int i = 0; i < count - 1; i++) {

        for (int j = 0; j < count - i - 1; j++) {

            if (strcmp(contacts[j].id, contacts[j + 1].id) > 0) {

                /*
                 * Swap the complete Contact structure rather than swapping
                 * each field separately.
                 */
                Contact temp = contacts[j];

                contacts[j] = contacts[j + 1];

                contacts[j + 1] = temp;
            }
        }
    }
}


/*
 * ============================================================================
 * FUNCTION: save_contacts()
 * ============================================================================
 *
 * Purpose:
 * Writes all contacts currently stored in memory to contacts.txt.
 *
 * The file is opened with "w", meaning the old file contents are replaced
 * with the current complete database.
 *
 * This is intentional: the text file is treated as a persistent snapshot
 * of the current in-memory database.
 */
void save_contacts(Contact *contacts, int count)
{
    FILE *file = fopen("contacts.txt", "w");

    if (file == NULL) {
        printf("File could not be opened.\n");
        return;
    }

    /*
     * Write a human-readable header.
     */
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


/*
 * ============================================================================
 * FUNCTION: load_contacts()
 * ============================================================================
 *
 * Purpose:
 * Loads contacts from contacts.txt when the program starts.
 *
 * Why is this necessary?
 *
 * Heap memory disappears when the program terminates. Therefore, the
 * contacts must be stored in a file if they are expected to survive
 * program restarts.
 *
 * This function also determines the next available ID by finding the
 * highest existing CONTACT number.
 *
 * Example:
 *
 *      File contains:
 *          CONTACT-001
 *          CONTACT-003
 *          CONTACT-004
 *
 *      Result:
 *          next_id = 5
 *
 * CONTACT-002 is intentionally not reused after deletion.
 */
void load_contacts(Contact **contacts, int *count, int *capacity, int *next_id)
{
    FILE *file = fopen("contacts.txt", "r");

    /*
     * The file may not exist during the first program execution.
     * That is not an error; it simply means the database is initially empty.
     */
    if (file == NULL) {
        return;
    }

    char line[150];
    int highest_id = 0;

    while (fgets(line, sizeof(line), file) != NULL) {

        /*
         * A contact record begins with the ID line.
         * Ignore all other lines such as headers and separators.
         */
        if (strncmp(line, "ID            : ", 16) != 0) {
            continue;
        }

        /*
         * Make sure there is enough heap space before loading another
         * contact.
         */
        if (*count == *capacity) {

            int new_capacity = *capacity * 2;

            Contact *temp = realloc(
                *contacts,
                new_capacity * sizeof(Contact)
            );

            if (temp == NULL) {
                printf("Memory reallocation failed while loading contacts.\n");
                fclose(file);
                return;
            }

            *contacts = temp;
            *capacity = new_capacity;
        }

        Contact *contact = &(*contacts)[*count];

        /*
         * Read the ID.
         *
         * line + 16 skips:
         *
         *      "ID            : "
         *
         * leaving only the actual ID.
         */
        sscanf(line + 16, "%49s", contact->id);

        int current_id;

        /*
         * Extract the numeric part of CONTACT-XXX.
         */
        if (sscanf(contact->id, "CONTACT-%d", &current_id) != 1) {
            continue;
        }

        /*
         * Keep the largest ID encountered.
         *
         * We need the maximum, not simply the last ID read.
         */
        if (current_id > highest_id) {
            highest_id = current_id;
        }

        /*
         * Read the three remaining fields belonging to this contact.
         */
        if (fgets(line, sizeof(line), file) == NULL)
            break;

        sscanf(line + 16, "%49[^\n]", contact->name);

        if (fgets(line, sizeof(line), file) == NULL)
            break;

        sscanf(line + 16, "%19[^\n]", contact->phone);

        if (fgets(line, sizeof(line), file) == NULL)
            break;

        sscanf(line + 16, "%99[^\n]", contact->email);

        (*count)++;
    }

    /*
     * Continue ID generation from the highest existing ID.
     *
     * If highest_id = 4:
     *      next_id = 5
     */
    *next_id = highest_id + 1;

    fclose(file);
}


/*
 * ============================================================================
 * FUNCTION: main()
 * ============================================================================
 *
 * Purpose:
 * Program entry point.
 *
 * Responsibilities:
 *  1. Initialize database state.
 *  2. Allocate initial heap memory.
 *  3. Load previously saved contacts.
 *  4. Start the menu.
 *  5. Release allocated memory before termination.
 */
int main()
{
    Contact *contacts = NULL;

    /*
     * count:
     * Number of contacts currently stored.
     */
    int count = 0;

    /*
     * capacity:
     * Number of Contact structures currently allocated in the heap.
     */
    int capacity = 2;

    /*
     * next_id:
     * ID number assigned to the next newly created contact.
     */
    int next_id = 1;

    /*
     * Allocate the initial contact array on the heap.
     */
    contacts = malloc(capacity * sizeof(Contact));

    if (contacts == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    /*
     * Restore the previous database from disk.
     *
     * This may also increase capacity through realloc() if the file contains
     * more contacts than the initial capacity.
     */
    load_contacts(&contacts, &count, &capacity, &next_id);

    /*
     * Start the main application interface.
     */
    menu(&contacts, &count, &capacity, &next_id);

    /*
     * Release all heap memory before the program terminates.
     */
    free(contacts);

    return 0;
}