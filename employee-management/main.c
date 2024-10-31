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
#define LEFT 75
#define RIGHT 77
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
int employeeExists(struct Employee *employees, int length, int id);
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
            inx = (ch == UP || ch == LEFT)      ? (inx == 0 ? 5 : inx - 1)
                  : (ch == DOWN || ch == RIGHT) ? (inx + 1) % 6
                  : (ch == HOME)                ? 0
                  : (ch == END)                 ? 5
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
        {
            // Only display employees with valid IDs
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

int employeeExists(struct Employee *employees, int length, int id)
{
    for (int i = 0; i < length; i++)
    {
        if (employees[i].id == id)
        {
            return i; // Return the employee if found
        }
    }
    return -1;
}

// Function to display details of a specific employee by ID
void displayEmployee(struct Employee *employees, int length, int id)
{
    int index = employeeExists(employees, length, id); // Get the index of the employee

    if (index != -1) // Check if the employee exists
    {
        CLEAR;
        struct Employee e = employees[index];                    // Access the employee using the index
        float netSalary = e.salary + e.commission - e.deduction; // Calculate net salary
        printf("+-------------+--------------------+\n");
        printf("| Field       | Value              |\n");
        printf("+-------------+--------------------+\n");
        printf("| ID          | %-18d |\n", e.id);
        printf("| Name        | %-18s |\n", e.name);
        printf("| Age         | %-18d |\n", e.age);
        printf("| Salary      | %-18.2f |\n", e.salary);
        printf("| Commission  | %-18.2f |\n", e.commission);
        printf("| Deduction   | %-18.2f |\n", e.deduction);
        printf("| Net Salary  | %-18.2f |\n", netSalary);
        printf("+-------------+--------------------+\n");
    }
    else
    {
        printf("Employee with ID %d not found.\n", id); // Error message if ID not found
    }
}

// Function to add a new employee
void addEmployee(struct Employee *employees, int length)
{
    int inx;
    do
    {
        printf("Choose an index (1-%d): ", length);
        scanf("%d", &inx);
    } while (inx < 1 || inx > length); // Validate index input
    inx--; // Adjust index for zero-based array

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
    gotoxy(18, 2);
    printf("=== Enter Employee Details ===");

    // Display input fields for employee details
    gotoxy(10, 4);
    printf("+----------------------------------------+");
    gotoxy(10, 18);
    printf("+----------------------------------------+");

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

    // Get employee ID from user input and check for uniqueness
    int employeeID;

    // Loop until a valid unique ID is entered
    do
    {
        // Prompt user for the employee ID
        gotoxy(25, 6);
        scanf("%d", &employeeID); // Use & to store input in the variable

        // Check if the entered ID already exists (not the same employee)
        if (employeeExists(employees, length, employeeID) != -1 &&
            employeeExists(employees, length, employeeID) != inx)
        {
            gotoxy(12, 7);
            // Inform the user that the ID already exists
            printf("ID %d already exists. Please enter a unique ID.\n", employeeID);

            // Clear the ID input
            gotoxy(25, 6);
            printf("                    "); // Clear the input line by overwriting it with spaces

            // Optionally, provide additional instructions or leave the cursor at the same position
        }
        else
        {
            // If the ID is unique or the same as the current employee's ID, assign it
            employees[inx].id = employeeID;
            gotoxy(12, 7);
            // clear error message if exists
            printf("                                              ");
            break; // Exit the loop after a valid ID is entered
        }
    } while (1); // Keep looping until a valid unique ID is provided

    // Get employee details from user input
    gotoxy(25, 8);
    fflush(stdin);
    gets(employees[inx].name);
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
    scanf("%c", &choice);
    if (choice == 'y' || choice == 'Y')
    {
        for (int i = 0; i < length; i++)
        {
            clearEmployeeData(&employees[i]); // Clear each employee's data
        }
        CLEAR;
        printf("All employee data deleted.\n");
    }
    else
    {
        CLEAR;
        printf("Deletion cancelled.\n");
    }
}

// Function to delete a specific employee by ID
void deleteEmployee(struct Employee *employees, int length, int id)
{
    int index = employeeExists(employees, length, id); // Get the index of the employee

    if (index != -1) // Check if the employee exists
    {
        // Clear employee data
        clearEmployeeData(&employees[index]); // Clear the employee data
        printf("Employee with ID %d deleted.\n", id);
    }
    else
    {
        printf("Employee with ID %d not found.\n", id); // Error message if ID not found
    }
}

// Main Function
int main()
{
    struct Employee employees[100] = {0}; // Array to hold employee records
    displayMenu(employees, 100);          // Start the menu display
    return 0;                             // Indicate successful execution
}
