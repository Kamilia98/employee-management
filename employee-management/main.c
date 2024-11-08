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
#define BACK 8
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
void getEmployeeData(struct Employee *employee);
int getEmployeeId();
char *getEmployeeName();
int employeeExists(struct Employee *employees, int length, int id);
void displayEmployeeByID(struct Employee *employees, int length, int id);
void displayEmployeeByName(struct Employee *employees, int length, char *name);
void clearEmployeeData(struct Employee *employee);
void deleteEmployees(struct Employee *employees, int length);
void deleteEmployeeByID(struct Employee *employees, int length, int id);
void deleteEmployeeByName(struct Employee *employees, int length, char *name);
void gotoxy(int column, int row); // Set cursor position
void textattr(int i);             // Set text color
char **MultipleLineEditor(int dataCount, int col, int row, int *types, int *sizes);

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
    const char *menu[] = {"Add", "Display All", "Display By ID", "Display By Name", "Delete All", "Delete By ID", "Delete By Name", "Exit"};
    int inx = 0; // Index for the menu selection
    char ch;
    int screenWidth = 28; //

    do
    {
        CLEAR; // Clear the console
        textattr(7);
        printf("----------------------------\n");
        printf("          MAIN MENU         \n");
        printf("----------------------------\n");

        // Display menu options
        for (int i = 0; i < 8; i++)
        {
            int itemWidth = strlen(menu[i]) + 6;         // Width of menu item including ">> <<" for selected
            int padding = (screenWidth - itemWidth) / 2; // Calculate padding for centering
            gotoxy(padding, i * 2 + 3);                  // Move cursor to start position with padding

            textattr(i == inx ? 14 : 7); // Highlight selected option
            printf("%s %s %s", (i == inx) ? ">>" : "  ", menu[i], (i == inx) ? "<<" : "  ");
        }

        ch = getche(); // Get user input
        switch (ch)
        {
        case EXTENDED: // Handle special key inputs
            ch = getch();
            inx = (ch == UP || ch == LEFT)      ? (inx == 0 ? 7 : inx - 1)
                  : (ch == DOWN || ch == RIGHT) ? (inx + 1) % 8
                  : (ch == HOME)                ? 0
                  : (ch == END)                 ? 7
                                                : inx;

            break;

        case ENTER:
            CLEAR;
            int id;
            char *name;
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
                displayEmployeeByID(employees, length, id);
                getch();
                break;
            case 3:
                name = getEmployeeName();
                displayEmployeeByName(employees, length, name);
                free(name); // Free the allocated memory
                getch();
                break;
            case 4:
                deleteEmployees(employees, length);
                getch();
                break;
            case 5:
                id = getEmployeeId();
                CLEAR;
                deleteEmployeeByID(employees, length, id);
                getch();
                break;
            case 6:
                name = getEmployeeName();
                deleteEmployeeByName(employees, length, name);
                free(name); // Free the allocated memory
                getch();
                break;
            case 7:
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

// Function to capture multi-line user input based on data types and size constraints
char **MultipleLineEditor(int dataCount, int col, int row, int *types, int *sizes)
{
    // Allocate memory for each input field and supporting pointers
    char **arrs = (char **)malloc(dataCount * sizeof(char *));      // Stores user inputs
    char **startptrs = (char **)malloc(dataCount * sizeof(char *)); // Start positions
    char **currptrs = (char **)malloc(dataCount * sizeof(char *));  // Current cursor positions
    char **lastptrs = (char **)malloc(dataCount * sizeof(char *));  // Last character positions

    // Track cursor and editing positions for each input field
    int *start = malloc(dataCount * sizeof(int)); // Initial column positions
    int *curr = malloc(dataCount * sizeof(int));  // Current cursor positions
    int *last = malloc(dataCount * sizeof(int));  // Last filled positions
    int hasDecimalPoint = 0;                      // Track if a decimal point has been entered for float fields

    // Initialize input fields and set up memory for each entry
    for (int i = 0; i < dataCount; i++)
    {
        arrs[i] = (char *)malloc(sizes[i] * sizeof(char));  // Allocate memory per field size
        memset(arrs[i], ' ', sizes[i]);                     // Fill each field with spaces
        startptrs[i] = currptrs[i] = lastptrs[i] = arrs[i]; // Initialize all pointers to the start
        start[i] = curr[i] = last[i] = col;                 // Set initial positions to start column
    }

    char ch;
    int flag = 1;  // Flag to control loop
    int dcurr = 0; // Current data entry index (used for navigation across fields)

    do
    {
        gotoxy(curr[dcurr], row); // Move cursor to the current position
        ch = getch();             // Get user input

        // Handle various input cases
        switch (ch)
        {
        case EXTENDED: // Handle extended (arrow) keys
            ch = getch();
            switch (ch)
            {
            case RIGHT: // Move cursor right if possible
                if (currptrs[dcurr] < lastptrs[dcurr])
                {
                    currptrs[dcurr]++;
                    curr[dcurr]++;
                }
                break;
            case LEFT: // Move cursor left if possible
                if (currptrs[dcurr] > startptrs[dcurr])
                {
                    currptrs[dcurr]--;
                    curr[dcurr]--;
                }
                break;
            case UP: // Navigate to the previous field
                if (dcurr > 0)
                {
                    dcurr--;
                    row -= 2; // Move row up by 2
                    curr[dcurr] = currptrs[dcurr] - startptrs[dcurr] + col;
                }
                break;
            case DOWN: // Navigate to the next field
                if (dcurr < dataCount - 1)
                {
                    dcurr++;
                    row += 2; // Move row down by 2
                    curr[dcurr] = currptrs[dcurr] - startptrs[dcurr] + col;
                }
                break;
            }
            break;

        case ENTER:
        case ESC:     // Exit the editor on ENTER or ESC
            flag = 0; // End the loop
            break;

        case BACK: // Handle backspace
            if (currptrs[dcurr] > startptrs[dcurr])
            {
                // Reset decimal flag if decimal point is removed
                if (*(currptrs[dcurr] - 1) == '.')
                {
                    hasDecimalPoint = 0;
                }

                // Remove last character, reset cursor, and display space to clear
                currptrs[dcurr]--;
                *currptrs[dcurr] = ' ';
                curr[dcurr]--;
                last[dcurr]--;
                gotoxy(curr[dcurr], row);
                printf(" ");
                gotoxy(curr[dcurr], row); // Set cursor to the updated position
            }
            break;

        default:
            // Define acceptable key ranges based on field type
            int startKey = 0, endKey = 127;
            if (types[dcurr] == 0 || types[dcurr] == 1) // int or float
            {
                startKey = '0';
                endKey = '9';
            }
            else if (types[dcurr] == 2) // string (lowercase letters)
            {
                startKey = 'a';
                endKey = 'z';
            }

            // Check if the character is valid for the field type and within length limits
            if (isprint(ch) &&
                ((ch >= startKey && ch <= endKey) ||
                 (types[dcurr] == 1 && ch == '.' && hasDecimalPoint == 0)))
            {
                // Allow decimal point for float type only once
                if (ch == '.' && types[dcurr] == 1)
                {
                    hasDecimalPoint = 1;
                }

                // Add character to field if it fits within the size limit
                if (currptrs[dcurr] < startptrs[dcurr] + sizes[dcurr] - 1)
                {
                    *currptrs[dcurr] = ch;
                    printf("%c", ch); // Display the character
                    currptrs[dcurr]++;
                    curr[dcurr]++;
                    if (currptrs[dcurr] > lastptrs[dcurr]) // Update last position if moved forward
                    {
                        lastptrs[dcurr]++;
                        last[dcurr]++;
                    }
                }
                else
                {
                    printf("\a"); // Beep if field size limit reached
                }
            }
            break;
        }

    } while (flag);

    // Null-terminate each input field after editing is complete
    for (int i = 0; i < dataCount; i++)
    {
        *(currptrs[i]) = '\0';
    }
    getch();

    // Free temporary memory allocations
    free(start);
    free(curr);
    free(last);
    free(startptrs);
    free(currptrs);
    free(lastptrs);

    return arrs; // Return edited inputs
}

// Function to display all employees
void displayEmployees(struct Employee *employees, int length)
{
    CLEAR;
    int count = 0;

    // Loop through employees and display their details
    for (int i = 0; i < length; i++)
    {
        if (employees[i].id != 0)
        {
            count++;
            if (count == 1)
            {
                printf("+---------+-------+-------------------+-----+--------------+---------------+---------------+----------------+\n");
                printf("| Index   | ID    | Name              | Age | Salary       | Commission    | Deduction     | Net Salary     |\n");
                printf("+---------+-------+-------------------+-----+--------------+---------------+---------------+----------------+\n");
            }
            // Only display employees with valid IDs
            float netSalary = employees[i].salary + employees[i].commission - employees[i].deduction; // Calculate net salary
            printf("| %-7d | %-5d | %-17s | %-3d | %-12.2f | %-13.2f | %-13.2f | %-14.2f |\n",
                   i + 1, employees[i].id, employees[i].name, employees[i].age, employees[i].salary,
                   employees[i].commission, employees[i].deduction, netSalary);
        }
    }

    if (count)
    {
        printf("+---------+-------+-------------------+-----+--------------+---------------+---------------+----------------+\n");
    }
    else
        printf("There is no employees!");
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

// Function to get an employee name from user input
char *getEmployeeName()
{
    char *name = malloc(18 * sizeof(char)); // Allocate memory for name
    if (name == NULL)
    {
        perror("Memory allocation failed");
        exit(1);
    }

    printf("Choose Name: ");
    fflush(stdin);
    fgets(name, 18, stdin);           // Read input with size limit
    name[strcspn(name, "\n")] = '\0'; // Remove trailing newline

    return name;
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
void displayEmployeeByID(struct Employee *employees, int length, int id)
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

// Function to display details of a specific employee by ID
void displayEmployeeByName(struct Employee *employees, int length, char *name)
{
    int count = 0; // To count the number of matching employees

    // Create a temporary array to store matching employees
    struct Employee matchingEmployees[length];

    for (int i = 0; i < length; i++)
    {
        if (strcmpi(employees[i].name, name) == 0)
        {
            matchingEmployees[count++] = employees[i];
        }
    }
    if (!count)
    {
        CLEAR;
        printf("There are no employees with this name!");
    }
    else
    {
        // Call the displayEmployees function with the filtered array
        displayEmployees(matchingEmployees, count);
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
        scanf("%c", &choice);
        if (choice == 'n' || choice == 'N') // If user chooses not to replace, exit
            return;
    }

    getEmployeeData(&employees[inx]);
}

// Function to collect and stores employee data
void getEmployeeData(struct Employee *e)
{
    CLEAR; // Clear the screen
    gotoxy(3, 0);
    printf("=== Enter Employee Details ==="); // Title for input section

    // Display boundaries for the input fields
    gotoxy(0, 1);
    printf("+-----------------------------------+"); // Top boundary
    gotoxy(0, 15);
    printf("+-----------------------------------+\n"); // Bottom boundary

    // Field labels for employee data entry
    gotoxy(2, 3);
    printf("ID:");
    gotoxy(2, 5);
    printf("Name:");
    gotoxy(2, 7);
    printf("Age:");
    gotoxy(2, 9);
    printf("Salary:");
    gotoxy(2, 11);
    printf("Commission:");
    gotoxy(2, 13);
    printf("Deduction:");
    fflush(stdin); // Clear any leftover input

    int types[6] = {0, 2, 0, 1, 1, 1};  // Field data types: 0=int, 1=float, 2=string
    int sizes[6] = {5, 15, 3, 6, 6, 6}; // Field sizes for input constraints

    // Collect input for each field using MultipleLineEditor
    char **emp = MultipleLineEditor(6, 15, 3, types, sizes);

    // Store inputs in the Employee struct
    e->id = atoi(emp[0]);
    strcpy(e->name, emp[1]);
    e->age = atoi(emp[2]);
    e->salary = atof(emp[3]);
    e->commission = atof(emp[4]);
    e->deduction = atof(emp[5]);
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
        int count = 0;
        for (int i = 0; i < length; i++)
        {
            if (employees[i].id != 0)
            {
                count++;
                clearEmployeeData(&employees[i]); // Clear each employee's data
            }
        }
        CLEAR;
        if (!count)
        {
            printf("There is no employees!");
        }
        else
        {
            printf("All employee data deleted.");
        }
    }
    else
    {
        CLEAR;
        printf("Deletion cancelled.\n");
    }
}

// Function to delete a specific employee by ID
void deleteEmployeeByID(struct Employee *employees, int length, int id)
{
    int index = employeeExists(employees, length, id); // Get the index of the employee

    if (index != -1) // Check if the employee exists
    {
        // Clear employee data
        clearEmployeeData(&employees[index]); // Clear the employee data
        printf("Employee with ID %d deleted.\n", id);
    }
    else
        printf("Employee with ID %d not found.\n", id); // Error message if ID not found
}

// Function to delete a specific employee by ID
void deleteEmployeeByName(struct Employee *employees, int length, char *name)
{
    int count = 0; // To count the number of matching employees

    for (int i = 0; i < length; i++)
    {
        if (strcmpi(employees[i].name, name) == 0)
        {
            count++;
            clearEmployeeData(&employees[i]);
        }
    }
    CLEAR;
    if (!count)
        printf("There are no employees with this name!");
    else
        printf("%d employees deleted successfully!", count);
}

// Main Function
int main()
{
    struct Employee employees[100] = {0}; // Array to hold employee records
    displayMenu(employees, 100);          // Start the menu display
    return 0;                             // Indicate successful execution
}
