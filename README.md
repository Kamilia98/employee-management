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

## Multi-line Editor

The **Multi-line Editor** is a custom input method used in the program to collect employee details in a structured, user-friendly manner. Instead of prompting for each field individually, this editor allows the user to enter multiple fields (ID, Name, Age, Salary, Commission, and Deduction) in one continuous workflow, enhancing the user experience.

### How It Works:

1. **Initialization**: 
   - The editor is initialized with multiple fields, each associated with a specific data type (integer, string, or float) and a predefined size limit.
   - The editor uses dynamic arrays to manage user inputs, and each field has a corresponding cursor position and boundary limit.

2. **User Input**:
   - The user can navigate between fields and input characters using keyboard keys:
     - **Arrow Keys**: Move the cursor between fields or within the current field.
     - **Enter**: Submit the current field's data and move to the next one.
     - **Backspace**: Delete characters from the current field.
     - **Escape**: Exit the input process early.

3. **Validations**:
   - The editor ensures the input is valid based on the field type:
     - **Numeric Fields (ID, Age)**: Only numeric characters (`0-9`) are allowed.
     - **Floating-point Fields (Salary, Commission, Deduction)**: Only numbers and one decimal point are allowed.
     - **String Field (Name)**: Only alphabetic characters (`a-z`, `A-Z`) are allowed.
   - The editor restricts the user from entering more characters than the predefined size for each field, ensuring data integrity and preventing buffer overflows.

4. **Cursor Management**:
   - The editor dynamically adjusts the cursor position based on the user's actions, ensuring a smooth and intuitive input experience.

5. **Submission**:
   - Upon completion, the entered data is collected and stored in the corresponding employee fields (e.g., `e->id`, `e->name`).

### Validations

The program ensures that only valid data is accepted by using input validations at various stages. This helps to maintain data integrity and prevent errors caused by incorrect user input.

1. **Index Validation (Add Employee)**:
   - When adding an employee, the user is prompted to choose an index within the valid range (1 to `length` of the employee array). The program will keep asking for a valid input if the user provides an invalid index.
   - Once the index is valid, the program checks whether an employee already exists at that index. If an employee is already present, the user is asked whether they want to replace the existing record.

2. **Field Validations (Multi-line Editor)**:
   - **ID and Age Fields**: Only numeric values are allowed.
   - **Salary, Commission, Deduction Fields**: Only numeric values and one decimal point are allowed to ensure they are treated as floating-point numbers.
   - **Name Field**: Only alphabetic characters are allowed.
   - **Field Size Limitations**: Each field has a size limit, and the program prevents the user from entering more characters than the allocated space.

## Files

This program is contained in a single source file, which includes all the function implementations and the main function.

## How to Run the Program

1. Compile the program with a C compiler (e.g., `gcc` on Windows).
2. Run the compiled executable. Ensure that you are on a Windows system, as the program uses Windows-specific libraries (`windows.h` and `conio.h`) for console control.

### Compilation Example

``` # bash
gcc employee_management.c -o employee_management
```

### Execution

``` # bash
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

