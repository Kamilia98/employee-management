# Employee Management Program

This program is a console-based application in C that manages employee records, including adding, viewing, and deleting employee information. It uses structured data, menu navigation, and console-based text and cursor manipulation to create an interactive user experience.

## Features

- **Add Employee**: Allows users to add a new employee with details like ID, name, age, salary, commission, and deduction.
- **View All Employees**: Displays a list of all employees, including calculated net salary.
- **View Employee by ID or Name**: Retrieves details of a specific employee by their ID or name.
- **Delete All Employees**: Deletes all employee records in the system.
- **Delete Employee by ID or Name**: Deletes a specific employee record based on ID or name.
- **Exit**: Closes the program.

## Program Structure

- **Constants for Keyboard Input**: Defines special keys and macros for common operations (e.g., clearing the console).
- **Employee Struct**: Contains fields for employee details such as ID, name, age, salary, commission, and deduction.
- **Function Prototypes**: Declares functions for displaying menus, adding/viewing/deleting employees, and manipulating console output.

## Files

This program is contained in a single source file, which includes all the function implementations and the main function.

## How to Run the Program

1. Compile the program with a C compiler (e.g., `gcc` on Windows).
2. Run the compiled executable. Ensure that you are on a Windows system, as the program uses Windows-specific libraries (`windows.h` and `conio.h`) for console control.

### Compilation Example

```bash
gcc employee_management.c -o employee_management
```

### Execution

```bash
./employee_management
```

## Usage

When you start the program, a menu will appear with the following options:

- Use the arrow keys to navigate through the menu.
- Press **Enter** to select an option.
- Press **ESC** to exit at any time.

### Example Workflow

- **Add Employee**: Choose "Add" from the menu, enter the employee details, and confirm.
- **View Employee**: Choose "Display By ID" or "Display By Name" and input the ID or name.
- **Delete Employee**: Select "Delete By ID" or "Delete By Name," enter the ID or name, and confirm.

## System Requirements

- **Platform**: Windows
- **Compiler**: A C compiler compatible with Windows (e.g., GCC or Visual Studio)

