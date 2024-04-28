#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Öğrenci yapısı
typedef struct Student {
    int id;
    char name[50];
    char department[50];
    struct Student* next;
} Student;

// LinkedList yapısı
typedef struct {
    Student* head;
} LinkedList;

// Öğrenci bilgisini güncelle
void updateStudent(LinkedList* list, int id, const char* newName, const char* newDepartment) {
    Student* temp = list->head;
    while (temp != NULL) {
        if (temp->id == id) {
            strcpy(temp->name, newName);
            strcpy(temp->department, newDepartment);
            return;
        }
        temp = temp->next;
    }
    printf("Student with ID %d not found.\n", id);
}

// Öğrenci bilgisini sil
void deleteStudent(LinkedList* list, int id) {
    Student* temp = list->head;
    if (temp != NULL && temp->id == id) {
        list->head = temp->next;
        free(temp);
        return;
    }
    Student* prev = NULL;
    while (temp != NULL && temp->id != id) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) {
        printf("Student with ID %d not found.\n", id);
        return;
    }
    prev->next = temp->next;
    free(temp);
}
// Yeni bir öğrenci oluştur
Student* createStudent(int id, const char* name, const char* department) {
    Student* newStudent = (Student*)malloc(sizeof(Student));
    newStudent->id = id;
    strcpy(newStudent->name, name);
    strcpy(newStudent->department, department);
    newStudent->next = NULL;
    return newStudent;
}
// LinkedList'e öğrenci ekle
void addStudent(LinkedList* list, int id, const char* name, const char* department) {
    Student* newStudent = createStudent(id, name, department);
    if (list->head == NULL) {
        list->head = newStudent;
        return;
    }
    Student* temp = list->head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newStudent;
}

// Öğrenci ekle veya güncelle
void addOrUpdateStudent(LinkedList* list, int id, const char* name, const char* department) {
    Student* temp = list->head;
    while (temp != NULL) {
        if (temp->id == id) {
            updateStudent(list, id, name, department);
            return;
        }
        temp = temp->next;
    }
    addStudent(list, id, name, department);
}



// Öğrencileri ekrana yazdır
void printStudents(LinkedList* list) {
    Student* temp = list->head;
    while (temp != NULL) {
        printf("ID: %d, Name: %s, Department: %s\n", temp->id, temp->name, temp->department);
        temp = temp->next;
    }
}

// Dosyadan öğrenci bilgilerini oku ve LinkedList'e ekle
void readFromFile(LinkedList* list, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }
    int id;
    char name[50];
    char department[50];
    while (fscanf(file, "%d %s %s", &id, name, department) == 3) {
        addStudent(list, id, name, department);
    }
    fclose(file);
}

// LinkedList'i dosyaya yaz
void writeToFile(LinkedList* list, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }
    Student* temp = list->head;
    while (temp != NULL) {
        fprintf(file, "%d %s %s\n", temp->id, temp->name, temp->department);
        temp = temp->next;
    }
    fclose(file);
}
int main() {
    LinkedList list = { .head = NULL };
    readFromFile(&list, "students.txt");

    int choice;
    do {
        printf("\n1. Add or update a student\n");
        printf("2. Delete a student\n");
        printf("3. Print all students\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                {
                    int id;
                    char name[50];
                    char department[50];
                    printf("Enter student ID: ");
                    scanf("%d", &id);
                    printf("Enter student name: ");
                    scanf("%s", name);
                    printf("Enter student department: ");
                    scanf("%s", department);
                    addOrUpdateStudent(&list, id, name, department);
                    writeToFile(&list, "students.txt");
                    break;
                }
            case 2:
                {
                    int id;
                    printf("Enter ID of student to delete: ");
                    scanf("%d", &id);
                    deleteStudent(&list, id);
                    writeToFile(&list, "students.txt");
                    break;
                }
            case 3:
                printf("\nStudents:\n");
                printStudents(&list);
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 4.\n");
                break;
        }
    } while (choice != 4);

    return 0;
}
