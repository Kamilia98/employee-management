#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>

// Constants for keyboard input
#define EXTENDED -32
#define ENTER 13
#define ESC 27
#define TAB 9
#define UP 72
#define HOME 71
#define DOWN 80
#define END 79
#define CLEAR system("cls") // Macro to clear the console screen

// Struct Definitions
struct Employee
{
    int id;           // Employee ID
    char name[18];    // Employee name
    int age;          // Employee age
    float salary;     // Employee salary
    float commission; // Employee commission
    float deduction;  // Employee deduction
};

// Function Prototypes
void displayMenu(struct Employee *employees, int length);
void displayEmployees(struct Employee *employees, int length);
void addEmployee(struct Employee *employees, int length);
int getEmployeeId();
void displayEmployee(struct Employee *employees, int length, int id);
void clearEmployeeData(struct Employee *employee);
void deleteEmployees(struct Employee *employees, int length);
void deleteEmployee(struct Employee *employees, int length, int id);
void gotoxy(int column, int row); // Set cursor position
void textattr(int i);             // Set text color

// Function Implementations

// Function to set the cursor position in the console
void gotoxy(int column, int row)
{
    COORD coord;
    coord.X = column;
    coord.Y = row;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Function to set the text color in the console
void textattr(int i)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), i);
}

// Function to display the main menu
void displayMenu(struct Employee *employees, int length)
{
    const char *menu[] = {"Add", "Display All", "Display By ID", "Delete All", "Delete By ID", "Exit"};
    int inx = 0; // Index for the menu selection
    char ch;

    do
    {
        CLEAR; // Clear the console
        textattr(7);
        printf("----------------------------\n");
        printf("          MAIN MENU         \n");
        printf("----------------------------\n");

        // Display menu options
        for (int i = 0; i < 6; i++)
        {
            gotoxy(7, i * 2 + 3);
            textattr(i == inx ? 14 : 7); // Highlight selected option
            printf("%s %s", (i == inx) ? ">>" : "   ", menu[i]);
        }

        ch = getche(); // Get user input
        switch (ch)
        {
        case EXTENDED: // Handle special key inputs
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
                return; // Exit the program
            }
            break;

        case ESC:
            return; // Exit on ESC key
        case TAB:
            inx = (inx + 1) % 6; // Cycle through menu options
            break;
        }
    } while (1);
}

// Function to display all employees
void displayEmployees(struct Employee *employees, int length)
{
    CLEAR;
    printf("+---------+-------+------------------+-----+--------------+---------------+---------------+----------------+\n");
    printf("| Index   | ID    | Name             | Age | Salary       | Commission    | Deduction     | Net Salary     |\n");
    printf("+---------+-------+------------------+-----+--------------+---------------+---------------+----------------+\n");

    // Loop through employees and display their details
    for (int i = 0; i < length; i++)
    {
        if (employees[i].id != 0)
        {                                                                                             // Only display employees with valid IDs
            float netSalary = employees[i].salary + employees[i].commission - employees[i].deduction; // Calculate net salary
            printf("| %-7d | %-5d | %-16s | %-3d | %-12.2f | %-13.2f | %-13.2f | %-14.2f |\n",
                   i + 1, employees[i].id, employees[i].name, employees[i].age, employees[i].salary,
                   employees[i].commission, employees[i].deduction, netSalary);
        }
    }

    printf("+---------+-------+------------------+-----+--------------+---------------+---------------+----------------+\n");
}

// Function to get an employee ID from user input
int getEmployeeId()
{
    int id;
    printf("Choose ID: ");
    _flushall(); // Clear input buffer
    scanf("%d", &id);
    return id;
}

// Function to display details of a specific employee by ID
void displayEmployee(struct Employee *employees, int length, int id)
{
    for (int i = 0; i < length; i++)
    {
        if (employees[i].id == id)
        { // Check if employee ID matches
            CLEAR;
            float netSalary = employees[i].salary + employees[i].commission - employees[i].deduction; // Calculate net salary
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
            return; // Exit after displaying employee details
        }
    }
    printf("Employee with ID %d not found.\n", id); // Error message if ID not found
}

// Function to add an employee
void addEmployee(struct Employee *employees, int length)
{
    int inx;
    do
    {
        printf("Choose an index (1-%d): ", length);
        scanf("%d", &inx);
    } while (inx < 1 || inx > length); // Validate index input
    inx -= 1; // Adjust index for zero-based array

    // Check if an employee already exists at the index
    if (employees[inx].id != 0)
    {
        CLEAR;
        char choice;
        printf("An employee already exists at this index. Replace? (y/n): ");
        fflush(stdin);
        choice = getche();
        if (choice == 'n' || choice == 'N') // If user chooses not to replace, exit
            return;
    }

    CLEAR;
    gotoxy(12, 2);
    printf("=== Enter Employee Details ===");

    // Display input fields for employee details
    gotoxy(10, 4);
    printf("+-------------------------------+");
    for (int i = 5; i <= 17; i++)
    {
        gotoxy(10, i);
        printf("|                               |");
    }
    gotoxy(10, 18);
    printf("+-------------------------------+");

    // Prompt for employee details
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

    // Get employee details from user input
    gotoxy(25, 6);
    scanf("%d", &employees[inx].id);
    gotoxy(25, 8);
    fflush(stdin);
    fgets(employees[inx].name, sizeof(employees[inx].name), stdin);
    employees[inx].name[strcspn(employees[inx].name, "\n")] = 0; // Remove newline character
    gotoxy(25, 10);
    scanf("%d", &employees[inx].age);
    gotoxy(25, 12);
    scanf("%f", &employees[inx].salary);
    gotoxy(25, 14);
    scanf("%f", &employees[inx].commission);
    gotoxy(25, 16);
    scanf("%f", &employees[inx].deduction);
}

// Function to clear an employee's data
void clearEmployeeData(struct Employee *employee)
{
    employee->id = 0;
    strcpy(employee->name, ""); // Clear name
    employee->age = 0;
    employee->salary = 0.0;
    employee->commission = 0.0;
    employee->deduction = 0.0;
}

// Function to delete all employees
void deleteEmployees(struct Employee *employees, int length)
{
    char choice;
    printf("Delete all employees? (y/n): ");
    fflush(stdin);
    choice = getche();
    if (choice == 'y' || choice == 'Y')
    {
        for (int i = 0; i < length; i++)
        {
            clearEmployeeData(&employees[i]); // Clear each employee's data
        }
        printf("All employee data deleted.\n");
    }
    else
    {
        printf("Deletion cancelled.\n");
    }
}

// Function to delete a specific employee by ID
void deleteEmployee(struct Employee *employees, int length, int id)
{
    for (int i = 0; i < length; i++)
    {
        if (employees[i].id == id)
        {                                     // Check for matching ID
            clearEmployeeData(&employees[i]); // Clear employee data
            printf("Employee with ID %d deleted.\n", id);
            return; // Exit after deletion
        }
    }
    printf("Employee with ID %d not found.\n", id); // Error message if ID not found
}

// Main Function
int main()
{
    struct Employee employees[100] = {0}; // Array to hold employee records
    displayMenu(employees, 100);          // Start the menu display
    return 0;                             // Indicate successful execution
}
