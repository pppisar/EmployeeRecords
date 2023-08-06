# Database of employees


## Disclamer

This repository contains the code of the task, that I have made for the subject [Programming and Algorithms 2](https://courses.fit.cvut.cz/BI-PA2/) studied during the second semester at the Czech Technical University in Prague.


## Description

To facilitate the work of the HR department, we have implemented an employee database. For monitored employees, we store their first name, last name, email, and salary. During the employment records, these details may change, and we want to be able to search for the data and have an overview of salaries.

An employee is identified by their first name and last name or their email. Company procedures ensure that the email is unique across the entire database. Furthermore, first names and last names may be repeated, but the combination of (first name, last name) is unique again in the database. Therefore, there may be many people with the last name Smith, and many people may have the first name John, but there can only be one employee with the name John Smith in the database at any given moment. When comparing names, last names, and emails, we distinguish between lowercase and uppercase letters (case sensitive).


## Requirements for the functionality:

The following is a public interface that includes methods:

- [X] Constructor without parameters
    - This constructor initializes an instance of the class so that the created instance is currently empty (contains no records).
- [X] Destructor
    - It releases the resources allocated by the instance.
- [X] `add(name, surname, email, salary)`
    - Adds another record to the existing database. The parameters name and surname represent the first name and last name, the parameter email specifies the email address, and salary represents the salary. The method returns true if the record was added successfully, or false if it was not added (because a record with the same name and surname or the same email already existed in the database).
- [X] `del(name, surname)` **/** `del(email)`
    - Removes a record from the database. The parameter uniquely identifies the record, either by name and surname (first variant) or by email (second variant). If the record is successfully removed, the method returns true. If the record was not removed (because there was no employee with that identification), the method returns false.
- [X] `changeName(email, newName, newSurname)`
    - Changes the employee's first name and last name. The employee is identified by their email (email), and the name is changed to newName/newSurname. The method returns true for success or false for an error (no employee with such email exists, or the newly assigned name/surname is not unique).
- [X] `changeEmail(name, surname, newEmail)`
    - Changes the employee's email. The employee is identified by name and surname (name/surname), and the email is changed to newEmail. The method returns true for success or false for an error (no employee with such name and surname exists, or the newly assigned email is not unique).
- [X] `setSalary(name, surname, salary)` **/** `setSalary(email, salary)`
    - Changes the salary for the specified employee. There are two variants - the employee can be identified either by name and surname or by their email address. If the method succeeds, it returns true; for failure (non-existent employee), it returns false.
- [X] `getSalary(name, surname)` **/** `getSalary(email)`
    - Retrieves the salary of the specified employee. There are two variants - the employee can be identified either by name and surname or by their email address. If the method succeeds, it returns the employee's salary; for failure (non-existent employee), it returns the value 0.
- [X] `getRank(name, surname, rankMin, rankMax)` **/** `getRank(email, rankMin, rankMax)`
    - Determines how well the employee is paid relative to others. The result is the position of the employee's salary in the hypothetical salary ranking, from worst (lowest) to best (highest). The employee is identified by name and surname (first variant) or by email address (second variant), and the parameters rankMin/rankMax are output parameters where the function writes the position of the employee's salary in the ranking. As multiple employees can have the same salary, the output is a pair of values - the min-max interval. For example, if the searched employee receives a salary of 20,000, 37 employees receive a lower salary, and 21 other employees receive the same salary (thus, a total of 22 employees receive a salary of 20,000), then the result would be rankMin=37 and rankMax=37+22-1=58. The return value of the function is true for success (the specified employee was found, and output parameters are filled) or false (the specified employee was not found, output parameters remain unchanged).
- [X] `getFirst(outName, outSurname)`
    - Used for browsing the database. We want to browse employees alphabetically (sorted in ascending order by last name, and in case of the same last names, by first name). The method returns the first employee in the sorted list, and their name and surname are written into the provided output parameters outName/outSurname. The return value is true for success (the database was not empty) or false (empty database, output parameters remain unchanged).
- [X] `getNext(name, surname, outName, outSurname)`
    - Used for browsing employees similarly to the getFirst method. This method returns the next employee who follows the employee with the given name/surname in the sorted list of employees (as obtained from getFirst). The name of the following employee is written into the provided output parameters outName/outSurname. The return value is true for success (the employee with the given name/surname was found and is not the last in the sorted list) or false for failure (the employee with the given name/surname was not found or is the last in the list). In case of failure, the method will not change the output parameters outName/outSurname.