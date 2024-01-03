#ifndef COURSE_H
#define COURSE_H
#include <iostream>
#include <string>
#include <Wt/WPushButton.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WText.h>


class Course{

public:
    //constructor
    
    Course();
    // larger constructor, as descriped in the course.cpp file
    Course(std::string className_, int classYear_, std::string description_, Wt::WContainerWidget *courseContainer, Wt::WText *descriptionText);
    //get the class name
    std::string getClassName(){ return className;}
    
    // get the class year
    int getClassYear(){ return classYear;}
    // returns the x,y value of the course's button
    int getX(){ return x;}
    int getY(){ return y;}
    // the set the course button to a new button method described in course.cpp
    void setCourseButton(Wt::WPushButton *button);
    //get the class description
    std::string getDescription(){ return description;}
    // sets the css class of the course button
    void setButtonStyle(std::string css_);
    // sets the button's css attributes to a new content. 
    void setButtonCss(std::string css_);
   // return's the class button pointer
    Wt::WPushButton* getClassButton();
    //the button clicked functions as descriped in course.cpp
    void buttonClicked(Wt::WText* descriptionText);
    // sets the x and y value of the course
    void setxy(int x_, int y_);
    // adds a new prereq to the course
    void addPreReq(std::string className);
    // returns the pre req size
    int getPreReqSize(){ return preReqCounter;}
    // gets the pre req at a certain index. 
    std::string getPreReq(int index);
    // gets the button index as descriped in course.cpp
    int getButtonIndex(){
        return courseButtonIndex;
    }
    // sets the course button index to a new one, if a certain course is removed. 
    void setCourseButtonIndex(int index);
    // sets graded value of the course to 1
    void setGraded();
    // sets the graded value of the course to 0
    void setUngraded();
    // returns the graded value of the course
    int getGraded();


//attributes 
private: 
    std::string className;
    int classYear;
    std::string description;
    Wt::WPushButton *courseButton;
    std::string* preReq;
    int x;
    int y;
    int preReqCounter;
    int courseButtonIndex;
    int graded;

};
#endif // COURSE_H