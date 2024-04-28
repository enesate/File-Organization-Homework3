#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 10

// Öğrenci yapısı
typedef struct Student {
    int id;
    char name[50];
    char department[50];
    struct Student* next;
} Student;

// HashTable yapısı
typedef struct {
    Student* array[SIZE];
} HashTable;

// Yeni bir öğrenci oluştur
Student* createStudent(int id, const char* name, const char* department) {
    Student* newStudent = (Student*)malloc(sizeof(Student));
    newStudent->id = id;
    strcpy(newStudent->name, name);
    strcpy(newStudent->department, department);
    newStudent->next = NULL;
    return newStudent;
}

// Öğrenci eklemek için hash fonksiyonu
int hash(int key) {
    return key % SIZE;
}

// Öğrenci bilgisini güncelle
void updateStudent(HashTable* table, int id, const char* newName, const char* newDepartment) {
    int index = hash(id);
    Student* temp = table->array[index];
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
void deleteStudent(HashTable* table, int id) {
    int index = hash(id);
    Student* temp = table->array[index];
    if (temp != NULL && temp->id == id) {
        table->array[index] = temp->next;
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
// HashTable'e öğrenci ekle
void addStudent(HashTable* table, int id, const char* name, const char* department) {
    int index = hash(id);
    Student* newStudent = createStudent(id, name, department);
    if (table->array[index] == NULL) {
        table->array[index] = newStudent;
        return;
    }
    Student* temp = table->array[index];
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newStudent;
}

// Öğrenci ekle veya güncelle
void addOrUpdateStudent(HashTable* table, int id, const char* name, const char* department) {
    int index = hash(id);
    Student* temp = table->array[index];
    while (temp != NULL) {
        if (temp->id == id) {
            updateStudent(table, id, name, department);
            return;
        }
        temp = temp->next;
    }
    addStudent(table, id, name, department);
}



// Öğrencileri ekrana yazdır
void printStudents(HashTable* table) {
    for (int i = 0; i < SIZE; i++) {
        Student* temp = table->array[i];
        while (temp != NULL) {
            printf("ID: %d, Name: %s, Department: %s\n", temp->id, temp->name, temp->department);
            temp = temp->next;
        }
    }
}

// Dosyadan öğrenci bilgilerini oku ve HashTable'e ekle
void readFromFile(HashTable* table, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }
    int id;
    char name[50];
    char department[50];
    while (fscanf(file, "%d %s %s", &id, name, department) == 3) {
        addStudent(table, id, name, department);
    }
    fclose(file);
}

// HashTable'i dosyaya yaz
void writeToFile(HashTable* table, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }
    for (int i = 0; i < SIZE; i++) {
        Student* temp = table->array[i];
        while (temp != NULL) {
            fprintf(file, "%d %s %s\n", temp->id, temp->name, temp->department);
            temp = temp->next;
        }
    }
    fclose(file);
}

int main() {
    HashTable table = { .array = { NULL } };
    readFromFile(&table, "students.txt");

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
                    addOrUpdateStudent(&table, id, name, department);
                    writeToFile(&table, "students.txt");
                    break;
                }
            case 2:
                {
                    int id;
                    printf("Enter ID of student to delete: ");
                    scanf("%d", &id);
                    deleteStudent(&table, id);
                    writeToFile(&table, "students.txt");
                    break;
                }
            case 3:
                printf("\nStudents:\n");
                printStudents(&table);
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
