/**
* To-Do App
**/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

typedef struct {
    int id;
    char name[100];
    int priority;
} task_node;

typedef struct {
    bool wrong_input;
    bool null;
    bool memory_sort;
    bool exit;
} error_handel;

void line();
void options();
void show_menu_ui();
bool read_user_choice(int* choice);

FILE* open_file(char file[], char mode[]);
void close_file(FILE* fptr);

task_node* read_file(int* count, FILE* fptr);
bool store_data_in_file(task_node* arr, int size);
void print_data(task_node* arr, int size);

void process_data(error_handel* error);
bool read_user_data(task_node* arr, int index);
void add_task(error_handel* error);
void view_all_task(error_handel* error);
void dlt_task(error_handel* error);

bool read_id(int* id);
void read_task(char task[]);
bool read_priority(int* priority);
bool check_similar_id(task_node* arr, int* current_id, int size);
bool shifting_to_target_element(task_node arr[], int size, int terget);

int main() {
    error_handel error;

    while(true) {
        error.wrong_input = false;
        error.null = false;
        error.memory_sort = false;
        error.exit = false;
        process_data(&error);

        if(error.wrong_input) {
            sleep(3);
            system("clear");
        }
        else if(error.null) {
            printf("Press Enter to Return to Menu...");
            fflush(stdout);
            (void)getchar();
            system("clear");
        }
        else if(error.exit) break;
    }
    printf("Exiting Program...\n");

    return 0;
}

void line() {
    printf("========================================\n");
}

void options() {
    printf("1. Add a Task\n");
    printf("2. View All Task\n");
    printf("3. Delete a Task\n");
    printf("4. Exit\n");
    printf("\n");
}

void show_menu_ui() {
    line();
    printf("%21s\n", "MENU");
    line();
    printf("\n");

    options();
}

bool read_user_choice(int* choice) {
    int attempt = 0;

    while(true) {
        attempt++;
        if(attempt > 3) {
            printf("\nToo many invalid inputs are given.\n");
            printf("So the program is closed...\n");
            return true;
        }

        printf("Choose an option: ");
        if(scanf("%d", choice) != 1) {
            if(attempt < 3) {
                printf("\nInvalid input\n");
                printf("Please enter a valid number.\n");
                printf("\n");
            }

            int ch;
            while((ch = getchar()) != '\n' && ch != EOF);
            continue;
        }

        if(*choice < 1 || *choice > 4) {
            if(attempt < 3) {
                printf("\nInvalid input!\n");
                printf("Choose must be %d and %d.\n", 1, 4);
                printf("\n");
            }
            continue;
        }

        break;
    }

    int ch;
    while((ch = getchar()) != '\n' && ch != EOF);
    return false;
}

FILE* open_file(char file[], char mode[]) {
    FILE* fptr = fopen(file, mode);
    return fptr;
}

void close_file(FILE* fptr) {
    fclose(fptr);
}

task_node* read_file(int* count, FILE* fptr) {
    char buffer[256];
    int temp_count = 0, capacity = 2;
    task_node* arr = malloc(capacity * sizeof(*arr));
    if(arr == NULL) {
        return arr;
    }

    while(fgets(buffer, sizeof(buffer), fptr) != NULL) {
        if(temp_count == capacity) {
            capacity *= 2;
            task_node* temp = realloc(arr, capacity * sizeof(*temp));
            if(temp == NULL) {
                break;
            }
            arr = temp;
        }
        int times = sscanf(buffer, "%d|%[^|]|%d",
                           &arr[temp_count].id,
                           arr[temp_count].name,
                           &arr[temp_count].priority);

        if(times == 3) temp_count++;
    }

    *count = temp_count;
    return arr;
}

bool store_data_in_file(task_node* arr, int size) {
    FILE* fptr = open_file("todo.txt", "w");
    if(fptr == NULL) {
        printf("\n[!]Error: Add Task Fail!\n");
        printf("So Please Try Again.\n");
        return true;
    }

    for(int i=0; i<size; i++) {
        fprintf(fptr, "%d|%s|%d\n", arr[i].id, arr[i].name, arr[i].priority);
    }

    close_file(fptr);
    return false;
}

void print_data(task_node* arr, int size) {
    printf("-----------------------------------------\n");
    printf("%-5s %-26s %-1s\n", "ID", "Task Name", "Priority");
    printf("-----------------------------------------\n");

    char priority_str[10];
    for(int i=0; i<size; i++) {
        if(arr[i].priority == 1) {
            strcpy(priority_str, "High");
        }
        else if(arr[i].priority == 2) {
            strcpy(priority_str, "Medium");
        }
        else if(arr[i].priority == 3) {
            strcpy(priority_str, "Low");
        }
        printf("%-5d %-26s %-1s\n", arr[i].id, arr[i].name, priority_str);
    }
    printf("\n");
}

void process_data(error_handel* error) {
    show_menu_ui();
    int choice;
    if(read_user_choice(&choice)) {
        error->wrong_input = true;
        return;
    }
    printf("\n");

    switch(choice) {
    case 1: {
        add_task(error);
        break;
    }
    case 2: {
        view_all_task(error);
        break;
    }
    case 3: {
        dlt_task(error);
        break;
    }
    case 4: {
        error->exit = true;
        break;
    }
    default: {
        printf("\nInvalid input!\n");
        printf("Please Choose the Correct Option.\n");
        break;
    }
    }
}

bool read_user_data(task_node* arr, int index) {
    if(read_id(&arr[index].id)) return true;
    if(check_similar_id(arr, &arr[index].id, index)) return true;

    read_task(arr[index].name);
    if(read_priority(&arr[index].priority)) return true;

    return false;
}

void add_task(error_handel* error) {
    FILE* fptr = open_file("todo.txt", "r");
    task_node* arr = NULL;
    int count = 0;

    if(fptr != NULL) {
        arr = read_file(&count, fptr);
        close_file(fptr);
    }
    count += 1;

    task_node* temp = realloc(arr, count * sizeof(*temp));
    if(temp == NULL) {
        printf("\nExtra Memory Alocation/Realloc is Fail.\n");
        if(arr != NULL) free(arr);
        error->null = true;
        return;
    }
    arr = temp;

    if(read_user_data(arr, count-1)) {
        free(arr);
        error->wrong_input = true;
        return;
    }

    if(store_data_in_file(arr, count)) {
        free(arr);
        error->null = true;
        return;
    }

    free(arr);
    printf("\n[✓] Add Task Completely Done.\n");
    printf("Waite 3 Second to Back to Menu...");
    fflush(stdout);

    sleep(3);
    system("clear");
}

void view_all_task(error_handel* error) {
    FILE* fptr = open_file("todo.txt", "r");
    if(fptr == NULL) {
        printf("\nFile couldn't found!\n");
        printf("Please add a task first.\n");
        error->null = true;
        return;
    }

    int count = 0;
    task_node* arr = read_file(&count, fptr);
    close_file(fptr);

    if(count != 0) {
        print_data(arr, count);
    } else {
        printf("[!]Error: File is Completely Empty.\n");
        printf("Please Add a Task First...\n");
        printf("\n");
    }

    printf("Press Enter to return to Menu...");
    free(arr);
    fflush(stdout);
    (void)getchar();
    system("clear");
}

void dlt_task(error_handel* error) {
    FILE* fptr = open_file("todo.txt", "r");
    if(fptr == NULL) {
        printf("\nFile couldn't found!\n");
        printf("Please add a task first.\n");
        error->null = true;
        return;
    }

    int count = 0;
    task_node* arr = read_file(&count, fptr);
    close_file(fptr);

    if(count != 0) {
        int terget_id;
        int attempt = 0;

        while(true) {
            attempt++;
            if(attempt > 3) {
                printf("\nToo many invalid inputs are given.\n");
                printf("So the program is closed...\n");
                error->wrong_input = true;
                free(arr);
                return;
            }

            if(read_id(&terget_id)) {
                error->wrong_input = true;
                free(arr);
                return;
            }
            if(!shifting_to_target_element(arr, count, terget_id)) {
                if(attempt < 3) {
                    printf("\nInvalid input!\n");
                    printf("This ID isn't Match. Please Try Another ID.\n");
                    printf("\n");
                }
                continue;
            }

            count--;
            break;
        }

        if(count == 0) {
            free(arr);
            arr = NULL;
        }
        else {
            arr = realloc(arr, count * sizeof(*arr));
        }
        store_data_in_file(arr, count);
        printf("\n[✓]Success: Task with ID %d has been deleted successfully!\n", terget_id);
    } else {
        printf("[!] The task list is empty.\n");
        printf("Add a task first, then try deleting it.\n");
        printf("\n");
    }

    printf("Press Enter to return to Menu...");
    if(count != 0) free(arr);
    fflush(stdout);
    (void)getchar();
    system("clear");
}

bool read_id(int* id) {
    int attempt = 0;
    long long n = 0;

    while(true) {
        attempt++;
        if(attempt > 3) {
            printf("\nToo many invalid inputs are given.\n");
            printf("So the program is closed...\n");
            return true;
        }

        printf("Enter ID: ");
        if(scanf("%lld", &n) != 1) {
            if(attempt < 3) {
                printf("\nInvalid input\n");
                printf("Please enter a valid number.\n");
                printf("\n");
            }

            int ch;
            while((ch = getchar()) != '\n' && ch != EOF);
            continue;
        }

        if(n < 1 || n > INT_MAX) {
            if(attempt < 3) {
                printf("\nInvalid input!\n");
                printf("ID must be between %d and %d.\n", 1, INT_MAX);
                printf("\n");
            }
            continue;
        }

        *id = n;
        break;
    }

    int ch;
    while((ch = getchar()) != '\n' && ch != EOF);
    return false;
}

void read_task(char task[]) {
    printf("Enter Task Name: ");
    fgets(task, 99, stdin);

    int i = 0;
    while(task[i] != '\n' && task[i] != '\0') i++;
    task[i] = '\0';
}

bool read_priority(int* priority) {
    int attempt = 0;

    while(true) {
        attempt++;
        if(attempt > 3) {
            printf("\nToo many invalid inputs are given.\n");
            printf("So the program is closed...\n");
            return true;
        }

        printf("Enter Priority(1-high, 2-mid, 3-low): ");
        if(scanf("%d", priority) != 1) {
            if(attempt < 3) {
                printf("\nInvalid input\n");
                printf("Please enter a valid number.\n");
                printf("\n");
            }

            int ch;
            while((ch = getchar()) != '\n' && ch != EOF);
            continue;
        }

        if(*priority < 1 || *priority > 3) {
            if(attempt < 3) {
                printf("\nInvalid input!\n");
                printf("Priority must be between %d and %d.\n", 1, 3);
                printf("\n");
            }
            continue;
        }

        break;
    }

    int ch;
    while((ch = getchar()) != '\n' && ch != EOF);
    return false;
}

bool check_similar_id(task_node* arr, int* current_id, int size) {
    int attempt = 0;

    while(true) {
        bool found = false;

        attempt++;
        if(attempt > 3) {
            printf("\nToo many invalid inputs are given.\n");
            printf("So the program is closed...\n");
            return true;
        }

        for(int i=0; i<size; i++) {
            if(arr[i].id == *current_id) {
                printf("\nInvalid input!\n");
                printf("This ID is already given. \nSo please try another ID.\n");
                printf("\n");

                if(read_id(current_id)) return true;
                found = true;
                break;
            }
        }

        if(!found) break;
    }

    return false;
}

bool shifting_to_target_element(task_node* arr, int size, int terget) {
    bool found = false;
    for(int i=0; i<size; i++) {
        if(arr[i].id == terget) {
            if(i != (size-1)) {
                task_node temp = arr[i];
                arr[i] = arr[i+1];
                arr[i+1] = temp;
            }
            found = true;
        }
    }

    return found;
}
    