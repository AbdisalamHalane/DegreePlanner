#ifndef MODULE_H
#define MODULE_H
#include <iostream>
#include <string>
#include "Course.h"
#include <Wt/WPushButton.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WText.h>

class Module{

public:
    //constructor

    Module();
    Module(std::string moduleName_);
    ~Module();
    //get the class name
    std::string getModuleName(){ return moduleName;}
    
    // get the class year
    int getSize(){ return size;}
   // adds a new class to the module as described in the module.cpp file
    void addClass(std::string className_, int classYear_, std::string description_, Wt::WContainerWidget *courseContainer, Wt::WText *descriptionText);
   //gets a class by the index
    Course getClass(int number);
   // gets glass by name
    Course getClassByName(std::string className);
   // gets class in the module's index given their name
    int getClassIndex(std::string className);
   // sets an index of the course array of this module to a new class
    void setClassByIndex(Course newCourse,int index);
    // return the courses button given their name
    Wt::WPushButton* getCourseButton(std::string className);
   // returns thecourse button in the course array given the index at the course array of that course
    Wt::WPushButton* getCourseButton(int number);
    // sets the x and y value of a certain course as described in the module.cpp file
    void SetClassxy(int x , int y, std::string className);
    // adds a new pre requisite of a given course 
    void addPreReq(std::string course1,std::string course2);
    // adds a new reuired course
    void addReqCourse(std::string course1);
    // returns the required courses
    std::string * getReqCourses(){
        return reqCourses;
    }

    // gets the required course
    int getReqCourseSize(){return reqCourseSize;}
    // gets a required course given their index
    std::string getReqCourseIndex(int index){
        return reqCourses[index];
    }
    
    
//attributes 
private: 
    Course* courses;
    int size;
    std::string moduleName;

    std::string * reqCourses;
    int reqCourseSize;


};
#endif // MODULE_H