#ifndef USER_H
#define USER_H

#include <string>
#include "Course.h"
#include "Grades.h"
#include "Module.h"

class User
{

// public methods
public:
    // Constructor
    User(std::string userName, std::string password);

    // getters as defined in user.cpp
    std::string getUsername();
    std::string getPassword();
    // sets the name of the of the user and the password of the user
    void setName( std::string userName_);
    void setPassword(std::string password_);
    // the add new grade method as described in user.cpp
    void addNewGrade(std::string className,int grade);
    // the delete grade method as descriped in user.cpp
    void deleteGrade(std::string className);
    // this method return the amount grades (marked courses) the user has
    int returnGradeSize(){return gradeCounter;}
    // this method returns a grade object at a certain index (given the index as an argument)
    Grades returnGradeByIndex(int i){return grades[i];}
   // sets the email of the user object
    void setEmail(std::string email_);
    // returns the email of the user
    std::string returnEmail(){
        return email;
    }
    // setAdmin method as described in user.cpp
    void setAdmin();
    // return's the user's is admin Value
    int getAdmin(){ return isAdmin;}


private:
    // Attributes
    Grades* grades;
    std::string userName;
    std::string password;
    int gradeCounter;
    std::string email;
    int isAdmin;
};

#endif // USER_H