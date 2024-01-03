
#include "User.h"


    // Constructor

User::User(std::string userName_, std::string password_){

    //Grades * grade_ = new Grades[100];
    grades = new Grades[100];
    //courses = new Course[100];
    userName = userName_;
    password = password_;
    gradeCounter = 0;
    isAdmin = 0;
}






// return's the user's user name
std::string User::getUsername() {return userName; };
std::string User::getPassword() {return password; }; 
// sets the admin's value of this user to 1 if the user is an admin (only one admin rigth now in the program)
void User::setAdmin(){ isAdmin = 1;}
// sets the user's user name and password
void User::setName(std::string userName_) {userName = userName_; }
void User::setPassword(std::string password_) {password = password_; }
// adds a new grade to the curren tuser
void User::addNewGrade(std::string className,int grade){
    Grades tempGerade(className,grade);
    grades[gradeCounter] = tempGerade;
    ++gradeCounter;
}

// deletes a grade given the classname
void User::deleteGrade(std::string className){
    
    int placeHolder = 0;
    for(int i = 0; i<gradeCounter;i++){
        if((grades[i].getClassName()) == className){
            placeHolder = i;
            break;
        }
    }

    for (int i = placeHolder; i < gradeCounter-1; i++)
    {   
        grades[i] = grades[i+1];
    }
    gradeCounter = gradeCounter-1;

}
// sets the email of the user
void User::setEmail(std::string email_){email = email_;}



