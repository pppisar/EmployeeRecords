#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>

using namespace std;
#endif /* __PROGTEST__ */

/* An additional class that contains information about an employee.
 * */
class CPerson {
public:
    // Constructor and destructor
    CPerson(const string &name, const string &surname,
            const string &email, unsigned int salary) {
        m_name = name;
        m_surname = surname;
        m_email = email;
        m_salary = salary;
    }

    ~CPerson() = default;


    // gets functions
    string getName() const {
        return m_name;
    }

    string getSurname() const {
        return m_surname;
    }

    string getEmail() const {
        return m_email;
    }

    unsigned int getSalary() const {
        return m_salary;
    }


    // sets functions
    void setFullName(const string & name, const string &surname) {
        m_name = name;
        m_surname = surname;
    }

    void setEmail(const string &email) {
        m_email = email;
    }

    void setSalary(const unsigned int salary) {
        m_salary = salary;
    }


private:
    // Fields that describe an employee
    string m_name;
    string m_surname;
    string m_email;
    unsigned int m_salary;
};

/* The CPersonalAgenda class implements a database of employees
 * who are identified by first and last name, or email.
 * The database has the functionality of adding, deleting
 * an employee, and changing his information.
 * */
class CPersonalAgenda {
public:
    // Constructor and destructor
    CPersonalAgenda(void) = default;

    ~CPersonalAgenda(void) = default;

    /* Method that adds a new employee to the database.
     * Returns true if the employee was added successfully.
     * Otherwise, returns false.
     * */
    bool add(const string &name, const string &surname,
             const string &email, unsigned int salary) {
        CPerson p(name, surname, email, salary);

        if (m_databaseSize == 0) {
            // If there are no records in the database,
            // then add the employee to the end
            m_databaseByEmail.push_back(p);
            m_databaseByFullName.push_back(p);
            m_databaseSize++;
            return true;
        }
        else {
            // Find positions where we need to add a new employee to keep the records organized.
            int posByEmail = binSearch(email, 0, m_databaseSize - 1);
            int posByFullName = binSearch(surname, name,0, m_databaseSize - 1);

            /* If the found position refers to an existing employee,
             * then we check whether we are adding a copy of him.
             * If there is a match in the email or full name,
             * we cannot add the employee.
             * */
            if (posByEmail != m_databaseSize) {
                if (compare(m_databaseByEmail[posByEmail].getEmail(),
                            email) == 0) {
                    return false;
                }
            }
            if (posByFullName != m_databaseSize) {
                if (compare(m_databaseByFullName[posByFullName].getSurname(),
                            m_databaseByFullName[posByFullName].getName(),
                            surname, name) == 0) {
                    return false;
                }
            }

            // If there is no user with the same full name or email,
            // then add them to the found positions.
            m_databaseByEmail.insert(m_databaseByEmail.begin() + posByEmail, p);
            m_databaseByFullName.insert(m_databaseByFullName.begin() + posByFullName, p);
            m_databaseSize++;
            return true;
        }
    }

    /* Method that deletes an employee by full name.
     * Returns true if the employee was deleted successfully.
     * Otherwise, returns false.
     * */
    bool del(const string &name, const string &surname) {
        if (m_databaseSize == 0) {
            return false;
        }
        else {
            int posByFullName = findPosition(name, surname);
            if (posByFullName == -1) {
                return false;
            }
            // The employee with the given full name has been found.
            // Find his position in the vector sorted by emails and delete the employee.
            int posByEmail = findPosition(m_databaseByFullName[posByFullName].getEmail());
            m_databaseByFullName.erase(m_databaseByFullName.begin() + posByFullName);
            m_databaseByEmail.erase(m_databaseByEmail.begin() + posByEmail);
            m_databaseSize--;
            return true;
        }
    }

    /* Method that deletes an employee by email.
     * Returns true if the employee was deleted successfully.
     * Otherwise, returns false.
     * */
    bool del(const string &email) {
        if (m_databaseSize == 0) {
            return false;
        }
        else {
            int posByEmail = findPosition(email);
            if (posByEmail == -1) {
                return false;
            }
            // The employee with the given email has been found.
            // Find his position in the vector sorted by full name and delete the employee.
            int posByFullName = findPosition(m_databaseByEmail[posByEmail].getName(),
                                             m_databaseByEmail[posByEmail].getSurname());
            m_databaseByFullName.erase(m_databaseByFullName.begin() + posByFullName);
            m_databaseByEmail.erase(m_databaseByEmail.begin() + posByEmail);
            m_databaseSize--;
            return true;
        }
    }

    /* A method that changes the employee's full name by the given email.
     * Returns true if the full name were successfully changed.
     * Otherwise, returns false.
     * */
    bool changeName(const string &email, const string &newName, const string &newSurname) {
        if (m_databaseSize == 0) {
            return false;
        }
        else {
            // Check whether the employee with the given new
            // full name already exists in the database.
            int posByFullName = findPosition(newName, newSurname);
            if (posByFullName != -1) {
                return false;
            }
            // Find an employee by a given email.
            int posByEmail = findPosition(email);
            if (posByEmail == -1) {
                return false;
            }
            // If the employee has been found, then delete the record with
            // outdated data and add a record with the new full name.
            unsigned int salary = m_databaseByEmail[posByEmail].getSalary();
            del(email);
            add(newName, newSurname, email, salary);
            return true;
        }
    }

    /* A method that changes the employee's email by the given full name.
     * Returns true if the email was successfully changed.
     * Otherwise, returns false.
     * */
    bool changeEmail(const string &name, const string &surname, const string &newEmail) {
        if (m_databaseSize == 0) {
            return false;
        }
        else {
            // Check whether the employee with the given email already exists in the database
            int posByEmail = findPosition(newEmail);
            if (posByEmail != -1) {
                return false;
            }
            // Find an employee by a given full name.
            int posByFullName = findPosition(name, surname);
            if (posByFullName == -1) {
                return false;
            }
            // If the employee has been found, then delete the record with
            // outdated data and add a record with the new email.
            unsigned int salary = m_databaseByFullName[posByFullName].getSalary();
            del(name, surname);
            add(name, surname, newEmail, salary);
            return true;
        }
    }

    /* A method that changes the employee's salary by the given full name.
     * Returns true if the salary was successfully changed.
     * Otherwise, returns false.
     * */
    bool setSalary(const string &name, const string &surname, unsigned int salary) {
        if (m_databaseSize == 0) {
            return false;
        }
        else {
            int posByFullName = findPosition(name, surname);
            if (posByFullName == -1) {
                return false;
            }
            // The employee with the given full name has been found.
            // Find his position in the vector sorted by emails and set a new salary.
            int posByEmail = findPosition(m_databaseByFullName[posByFullName].getEmail());

            m_databaseByFullName[posByFullName].setSalary(salary);
            m_databaseByEmail[posByEmail].setSalary(salary);
            return true;
        }
    }

    /* A method that changes the employee's salary by the given email.
     * Returns true if the salary was successfully changed.
     * Otherwise, returns false.
     * */
    bool setSalary(const string &email, unsigned int salary) {
        if (m_databaseSize == 0) {
            return false;
        }
        else {
            int posByEmail = findPosition(email);
            if (posByEmail == -1) {
                return false;
            }
            // The employee with the given email has been found.
            // Find his position in the vector sorted by full name and set a new salary.
            int posByFullName = findPosition(m_databaseByEmail[posByEmail].getName(),
                                             m_databaseByEmail[posByEmail].getSurname());

            m_databaseByFullName[posByFullName].setSalary(salary);
            m_databaseByEmail[posByEmail].setSalary(salary);
            return true;
        }
    }

    /* A method that returns the employee's salary, according to the given full name.
     * Returns 0 if the employee with the given full name does not exist in the database.
     * */
    unsigned int getSalary(const string &name, const string &surname) const {
        if (m_databaseSize == 0) {
            return 0;
        }
        else {
            int posByFullName = findPosition(name, surname);
            if (posByFullName == -1) {
                return 0;
            }
            return m_databaseByFullName[posByFullName].getSalary();
        }
    }

    /* A method that returns the employee's salary, according to the given email.
     * Returns 0 if the employee with the given email does not exist in the database.
     * */
    unsigned int getSalary(const string &email) const {
        if (m_databaseSize == 0) {
            return 0;
        }
        else {
            int posByEmail = findPosition(email);
            if (posByEmail == -1) {
                return 0;
            }
            return m_databaseByEmail[posByEmail].getSalary();
        }
    }

    /* The method determines the employee's salary rating specified by full name.
     * In the output parameters rankMin and rankMax,
     * the lower and upper salary boundaries are written.
     * Returns true if the operation was successful. Otherwise, returns false.
     * */
    bool getRank(const string &name, const string &surname, int &rankMin, int &rankMax) const {
        if (m_databaseSize == 0) {
            return false;
        }
        int posByFullName = findPosition(name, surname);
        if (posByFullName == -1) {
            return false;
        }
        unsigned int salary = m_databaseByFullName[posByFullName].getSalary();
        // rMin contains the number of employees who have a salary less
        // than the employee specified by the name and surname fields.
        int rMin = 0;
        /* rMax contains the number of employees who have a salary less or equal
         * than the employee specified by the name and surname fields.
         *  The initial value is -1, because the calculation will count
         *  the employee, relative to whom the calculation is carried out.
         * */
        int rMax = -1;
        for (int i = 0; i < m_databaseSize; i++) {
            if (m_databaseByFullName[i].getSalary() < salary) {
                rMin++;
                rMax++;
            }
            else if (m_databaseByFullName[i].getSalary() == salary) {
                rMax++;
            }
        }
        rankMin = rMin;
        rankMax = rMax;
        return true;
    }

    /* The method determines the employee's salary rating specified by email.
     * In the output parameters rankMin and rankMax,
     * the lower and upper salary boundaries are written.
     * Returns true if the operation was successful. Otherwise, returns false.
     * */
    bool getRank(const string &email, int &rankMin, int &rankMax) const {
        if (m_databaseSize == 0) {
            return false;
        }
        int posByEmail = findPosition(email);
        if (posByEmail == -1) {
            return false;
        }
        unsigned int salary = m_databaseByEmail[posByEmail].getSalary();
        // rMin contains the number of employees who have a salary less
        // than the employee specified by the email field.
        int rMin = 0;
        /* rMax contains the number of employees who have a salary less or equal
         * than the employee specified by the email field.
         * The initial value is -1, because the calculation will count
         * the employee, relative to whom the calculation is carried out.
         * */
        int rMax = -1;
        for (int i = 0; i < m_databaseSize; i++) {
            if (m_databaseByEmail[i].getSalary() < salary) {
                rMin++;
                rMax++;
            }
            else if (m_databaseByEmail[i].getSalary() == salary) {
                rMax++;
            }
        }
        rankMin = rMin;
        rankMax = rMax;
        return true;
    }

    /* The method writes the first name and last name of the first employee in the
     * vector sorted by full name to the outName and outSurname output parameters.
     * Returns true if there is at least one record in the database.
     * Otherwise, returns false.
     * */
    bool getFirst(string &outName, string &outSurname) const {
        if (m_databaseSize == 0) {
            return false;
        }
        else {
            outName = m_databaseByFullName[0].getName();
            outSurname = m_databaseByFullName[0].getSurname();
            return true;
        }
    }

    /* Method finds the next employee in the sorted vector of employees
     * who follows the employee with the full name given by name and surname.
     * Method writes the full name of the next employee to the outName and outSurname output parameters.
     * Returns true if the operation was successful. Otherwise, returns false.
     * */
    bool getNext(const string &name, const string &surname, string &outName, string &outSurname) const {
        if (m_databaseSize == 0) {
            return false;
        }
        else {
            int posByFullName = findPosition(name, surname);
            if (posByFullName == -1 || posByFullName == m_databaseSize - 1) {
                // If the employee was not found or is the last element in the vector,
                // then the full name of the next employee cannot be obtained.
                return 0;
            }
            // Write the output values of the next employee
            outName = m_databaseByFullName[posByFullName + 1].getName();
            outSurname = m_databaseByFullName[posByFullName + 1].getSurname();
            return true;
        }
    }

private:
    /* For binary search, two vectors have been implemented
     * that store ordered records by full name and email.
     * */
    vector<CPerson> m_databaseByFullName; // Employee database sorted by name and surname
    vector<CPerson> m_databaseByEmail; // Employee database sorted by email

    /* A variable that stores the current number of records in the database.
     * It is needed to reduce program execution time
     * when using the size() method on a vector.
     * */
    int m_databaseSize = 0;


    // Additional functions

    /* Functions that find an employee by email or full name
     * and control the presence of this employee in the database.
     * If successful, returns the position in the vector.
     * Otherwise, returns -1.
     * */
    int findPosition(const string &email) const {
        // Find a potential database record that can match the given email.
        int posByEmail = binSearch(email, 0, m_databaseSize - 1);
        if (posByEmail == m_databaseSize) {
            // If the found position points to a non-existent record,
            // then no employee with the given email was found.
            return -1;
        }
        if (compare(m_databaseByEmail[posByEmail].getEmail(), email) != 0) {
            // If the found employee has different email.
            return -1;
        }
        return posByEmail;
    }

    int findPosition(const string &name, const string &surname) const {
        // Find a potential database record that can match the given full name.
        int posByFullName = binSearch(surname, name, 0, m_databaseSize - 1);
        if (posByFullName == m_databaseSize) {
            // If the found position points to a non-existent record,
            // then no employee with the given FullName was found.
            return -1;
        }
        if (compare(m_databaseByFullName[posByFullName].getSurname(),
                    m_databaseByFullName[posByFullName].getName(),
                    surname, name) != 0) {
            // If the found employee has different FullName.
            return -1;
        }
        return posByFullName;
    }

    // Functions that compare email or full name
    int compare(const string &email1, const string &email2) const {
        return email1.compare(email2);
    }

    int compare(const string &surname1, const string &name1, const string &surname2, const string &name2) const {
        int compareSurname = surname1.compare(surname2);
        if (compareSurname == 0) {
            return name1.compare(name2);
        }
        return compareSurname;
    }

    // Functions that implement binary search in sorted vectors.
    int binSearch(const string &email, int left, int right) const {
        if (left > right) {
            return left;
        }

        int middle = (left + right) / 2;

        int compareRes = compare(email, m_databaseByEmail[middle].getEmail());

        if (compareRes == 0) {
            return middle;
        }
        else if (compareRes > 0) {
            return binSearch(email, middle + 1, right);
        }
        else {
            return binSearch(email, left, middle - 1);
        }
    }

    int binSearch(const string &surname, const string &name, int left, int right) const {
        if (left > right) {
            return left;
        }

        int middle = (left + right) / 2;

        int compareRes = compare(surname, name,
                                 m_databaseByFullName[middle].getSurname(),
                                 m_databaseByFullName[middle].getName());
        if (compareRes == 0) {
            return middle;
        }
        else if (compareRes > 0) {
            return binSearch(surname, name, middle + 1, right);
        }
        else {
            return binSearch(surname, name, left, middle - 1);
        }
    }
};

#ifndef __PROGTEST__

int main(void) {
    // Tests of the whole implementation
    string outName, outSurname;
    int lo, hi;

    CPersonalAgenda b1;
    assert (b1.add("John", "Smith", "john", 30000));
    assert (b1.add("John", "Miller", "johnm", 35000));
    assert (b1.add("Peter", "Smith", "peter", 23000));
    assert (b1.getFirst(outName, outSurname)
            && outName == "John"
            && outSurname == "Miller");
    assert (b1.getNext("John", "Miller", outName, outSurname)
            && outName == "John"
            && outSurname == "Smith");
    assert (b1.getNext("John", "Smith", outName, outSurname)
            && outName == "Peter"
            && outSurname == "Smith");
    assert (!b1.getNext("Peter", "Smith", outName, outSurname));
    assert (b1.setSalary("john", 32000));
    assert (b1.getSalary("john") == 32000);
    assert (b1.getSalary("John", "Smith") == 32000);
    assert (b1.getRank("John", "Smith", lo, hi)
            && lo == 1
            && hi == 1);
    assert (b1.getRank("john", lo, hi)
            && lo == 1
            && hi == 1);
    assert (b1.getRank("peter", lo, hi)
            && lo == 0
            && hi == 0);
    assert (b1.getRank("johnm", lo, hi)
            && lo == 2
            && hi == 2);
    assert (b1.setSalary("John", "Smith", 35000));
    assert (b1.getSalary("John", "Smith") == 35000);
    assert (b1.getSalary("john") == 35000);
    assert (b1.getRank("John", "Smith", lo, hi)
            && lo == 1
            && hi == 2);
    assert (b1.getRank("john", lo, hi)
            && lo == 1
            && hi == 2);
    assert (b1.getRank("peter", lo, hi)
            && lo == 0
            && hi == 0);
    assert (b1.getRank("johnm", lo, hi)
            && lo == 1
            && hi == 2);
    assert (b1.changeName("peter", "James", "Bond"));
    assert (b1.getSalary("peter") == 23000);
    assert (b1.getSalary("James", "Bond") == 23000);
    assert (b1.getSalary("Peter", "Smith") == 0);
    assert (b1.getFirst(outName, outSurname)
            && outName == "James"
            && outSurname == "Bond");
    assert (b1.getNext("James", "Bond", outName, outSurname)
            && outName == "John"
            && outSurname == "Miller");
    assert (b1.getNext("John", "Miller", outName, outSurname)
            && outName == "John"
            && outSurname == "Smith");
    assert (!b1.getNext("John", "Smith", outName, outSurname));
    assert (b1.changeEmail("James", "Bond", "james"));
    assert (b1.getSalary("James", "Bond") == 23000);
    assert (b1.getSalary("james") == 23000);
    assert (b1.getSalary("peter") == 0);
    assert (b1.del("james"));
    assert (b1.getRank("john", lo, hi)
            && lo == 0
            && hi == 1);
    assert (b1.del("John", "Miller"));
    assert (b1.getRank("john", lo, hi)
            && lo == 0
            && hi == 0);
    assert (b1.getFirst(outName, outSurname)
            && outName == "John"
            && outSurname == "Smith");
    assert (!b1.getNext("John", "Smith", outName, outSurname));
    assert (b1.del("john"));
    assert (!b1.getFirst(outName, outSurname));
    assert (b1.add("John", "Smith", "john", 31000));
    assert (b1.add("john", "Smith", "joHn", 31000));
    assert (b1.add("John", "smith", "jOhn", 31000));

    CPersonalAgenda b2;
    assert (!b2.getFirst(outName, outSurname));
    assert (b2.add("James", "Bond", "james", 70000));
    assert (b2.add("James", "Smith", "james2", 30000));
    assert (b2.add("Peter", "Smith", "peter", 40000));
    assert (!b2.add("James", "Bond", "james3", 60000));
    assert (!b2.add("Peter", "Bond", "peter", 50000));
    assert (!b2.changeName("joe", "Joe", "Black"));
    assert (!b2.changeEmail("Joe", "Black", "joe"));
    assert (!b2.setSalary("Joe", "Black", 90000));
    assert (!b2.setSalary("joe", 90000));
    assert (b2.getSalary("Joe", "Black") == 0);
    assert (b2.getSalary("joe") == 0);
    assert (!b2.getRank("Joe", "Black", lo, hi));
    assert (!b2.getRank("joe", lo, hi));
    assert (!b2.changeName("joe", "Joe", "Black"));
    assert (!b2.changeEmail("Joe", "Black", "joe"));
    assert (!b2.del("Joe", "Black"));
    assert (!b2.del("joe"));
    assert (!b2.changeName("james2", "James", "Bond"));
    assert (!b2.changeEmail("Peter", "Smith", "james"));
    assert (!b2.changeName("peter", "Peter", "Smith"));
    assert (!b2.changeEmail("Peter", "Smith", "peter"));
    assert (b2.del("Peter", "Smith"));
    assert (!b2.changeEmail("Peter", "Smith", "peter2"));
    assert (!b2.setSalary("Peter", "Smith", 35000));
    assert (b2.getSalary("Peter", "Smith") == 0);
    assert (!b2.getRank("Peter", "Smith", lo, hi));
    assert (!b2.changeName("peter", "Peter", "Falcon"));
    assert (!b2.setSalary("peter", 37000));
    assert (b2.getSalary("peter") == 0);
    assert (!b2.getRank("peter", lo, hi));
    assert (!b2.del("Peter", "Smith"));
    assert (!b2.del("peter"));
    assert (b2.add("Peter", "Smith", "peter", 40000));
    assert (b2.getSalary("peter") == 40000);

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
