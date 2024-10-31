#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>

// Constants
#define EXTENDED -32
#define ENTER 13
#define ESC 27
#define TAB 9
#define UP 72
#define HOME 71
#define DOWN 80
#define END 79
#define CLEAR system("cls")

// Struct Definitions
struct Employee
{
    int id;
    char name[18];
    int age;
    float salary;
    float commission;
    float deduction;
};

// Function Prototypes
void displayMenu(struct Employee *employees, int);
void displayEmployees(struct Employee *employees, int);
void addEmployee(struct Employee *employees, int);
int getEmployeeId();
void displayEmployee(struct Employee *employees, int, int);
void clearEmployeeData(struct Employee *employee);
void deleteEmployees(struct Employee *employees, int);
void deleteEmployee(struct Employee *employees, int, int);
void gotoxy(int column, int row);
void textattr(int i);

// Function Implementations
void gotoxy(int column, int row)
{
    COORD coord;
    coord.X = column;
    coord.Y = row;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void textattr(int i)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), i);
}

void displayMenu(struct Employee *employees, int length)
{
    const char *menu[] = {"Add", "Display All", "Display By ID", "Delete All", "Delete By ID", "Exit"};
    int inx = 0;
    char ch;

    do
    {
        CLEAR;
        textattr(7);
        printf("----------------------------\n");
        printf("          MAIN MENU         \n");
        printf("----------------------------\n");

        for (int i = 0; i < 6; i++)
        {
            gotoxy(7, i * 2 + 3);
            textattr(i == inx ? 14 : 7);
            printf("%s %s", (i == inx) ? ">>" : "   ", menu[i]);
        }

        ch = getche();
        switch (ch)
        {
        case EXTENDED:
            ch = getch();
            inx = (ch == UP) ? (inx == 0 ? 5 : inx - 1) : (ch == DOWN) ? (inx + 1) % 6
                                                      : (ch == HOME)   ? 0
                                                      : (ch == END)    ? 5
                                                                       : inx;
            break;

        case ENTER:
            CLEAR;
            int id;
            switch (inx)
            {
            case 0:
                addEmployee(employees, length);
                break;
            case 1:
                displayEmployees(employees, length);
                getch();
                break;
            case 2:
                id = getEmployeeId();
                CLEAR;
                displayEmployee(employees, length, id);
                getch();
                break;
            case 3:
                deleteEmployees(employees, length);
                getch();
                break;
            case 4:
                id = getEmployeeId();
                CLEAR;
                deleteEmployee(employees, length, id);
                getch();
                break;
            case 5:
                return;
            }
            break;

        case ESC:
            return;
        case TAB:
            inx = (inx + 1) % 6;
            break;
        }
    } while (1);
}

void displayEmployees(struct Employee *employees, int length)
{
    CLEAR;
    printf("+---------+-------+------------------+-----+--------------+---------------+---------------+----------------+\n");
    printf("| Index   | ID    | Name             | Age | Salary       | Commission    | Deduction     | Net Salary     |\n");
    printf("+---------+-------+-------------------+-----+--------------+---------------+---------------+----------------+\n");

    for (int i = 0; i < length; i++)
    {
        if (employees[i].id != 0)
        {
            float netSalary = employees[i].salary + employees[i].commission - employees[i].deduction;
            printf("| %-7d | %-5d | %-16s | %-3d | %-12.2f | %-13.2f | %-13.2f | %-14.2f |\n",
                   i + 1, employees[i].id, employees[i].name, employees[i].age, employees[i].salary,
                   employees[i].commission, employees[i].deduction, netSalary);
        }
    }

    printf("+---------+-------+------------------+-----+--------------+---------------+---------------+----------------+\n");
}

int getEmployeeId()
{
    int id;
    printf("Choose id: ");
    _flushall();
    scanf("%d", &id);
    return id;
}

void displayEmployee(struct Employee *employees, int length, int id)
{

    for (int i = 0; i < length; i++)
    {
        if (employees[i].id == id)
        {
            CLEAR;
            float netSalary = employees[i].salary + employees[i].commission - employees[i].deduction;
            printf("+-------------+--------------------+\n");
            printf("| Field       | Value              |\n");
            printf("+-------------+--------------------+\n");
            printf("| Index       | %-18d |\n", i + 1);
            printf("| ID          | %-18d |\n", employees[i].id);
            printf("| Name        | %-18s |\n", employees[i].name);
            printf("| Age         | %-18d |\n", employees[i].age);
            printf("| Salary      | %-18.2f |\n", employees[i].salary);
            printf("| Commission  | %-18.2f |\n", employees[i].commission);
            printf("| Deduction   | %-18.2f |\n", employees[i].deduction);
            printf("| Net Salary  | %-18.2f |\n", netSalary);
            printf("+-------------+--------------------+\n");
            return;
        }
    }
    printf("Employee with ID %d not found.\n", id);
}

void addEmployee(struct Employee *employees, int length)
{
    int inx;
    do
    {
        printf("Choose an index (1-%d): ", length);
        scanf("%d", &inx);
    } while (inx < 1 || inx > length);
    inx -= 1;

    if (employees[inx].id != 0)
    {
        CLEAR;
        char choice;
        printf("An employee already exists at this index. Replace? (y/n): ");
        fflush(stdin);
        choice = getche();
        if (choice == 'n' || choice == 'N')
            return;
    }

    CLEAR;
    gotoxy(12, 2);
    printf("=== Enter Employee Details ===");

    gotoxy(10, 4);
    printf("+-------------------------------+");
    for (int i = 5; i <= 17; i++)
    {
        gotoxy(10, i);
        printf("|                               |");
    }
    gotoxy(10, 18);
    printf("+-------------------------------+");

    gotoxy(12, 6);
    printf("ID:");
    gotoxy(12, 8);
    printf("Name:");
    gotoxy(12, 10);
    printf("Age:");
    gotoxy(12, 12);
    printf("Salary:");
    gotoxy(12, 14);
    printf("Commission:");
    gotoxy(12, 16);
    printf("Deduction:");

    gotoxy(25, 6);
    scanf("%d", &employees[inx].id);
    gotoxy(25, 8);
    fflush(stdin);
    fgets(employees[inx].name, sizeof(employees[inx].name), stdin);
    employees[inx].name[strcspn(employees[inx].name, "\n")] = 0;
    gotoxy(25, 10);
    scanf("%d", &employees[inx].age);
    gotoxy(25, 12);
    scanf("%f", &employees[inx].salary);
    gotoxy(25, 14);
    scanf("%f", &employees[inx].commission);
    gotoxy(25, 16);
    scanf("%f", &employees[inx].deduction);
}

void clearEmployeeData(struct Employee *employee)
{
    employee->id = 0;
    strcpy(employee->name, "");
    employee->age = 0;
    employee->salary = 0;
    employee->commission = 0;
    employee->deduction = 0;
}

void deleteEmployees(struct Employee *employees, int length)
{
    for (int i = 0; i < length; i++)
    {
        clearEmployeeData(&employees[i]);
    }
    printf("All employees have been successfully deleted.\n");
}

void deleteEmployee(struct Employee *employees, int length, int id)
{
    for (int i = 0; i < length; i++)
    {
        if (employees[i].id == id)
        {
            clearEmployeeData(&employees[i]);
            printf("Successfully deleted employee with ID %d.\n", id);
            return;
        }
    }
    printf("Employee with ID %d not found.\n", id);
}

// Main function
int main()
{
    struct Employee employees[10] = {0};
    displayMenu(employees, sizeof(employees) / sizeof(employees[0]));
    return 0;
}
