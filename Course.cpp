#include "Course.h"
#include <iostream>
#include <Wt/WApplication.h>
#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>

#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WTemplate.h>

#include <Wt/WTimer.h>

#include <Wt/WAnchor.h>
#include <Wt/WMenu.h>
#include <Wt/WMessageBox.h>
#include <Wt/WNavigationBar.h>
#include <Wt/WPopupMenu.h>
#include <Wt/WPopupMenuItem.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WTextArea.h>
#include <Wt/WTable.h>

#include <Wt/WText.h>

#include "Degreeplanner.h"
#include "Course.h"
#include "Module.h"
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <cctype>
#include <Wt/WBootstrapTheme.h>

#include <Wt/WTable.h>
#include <Wt/WTableCell.h>
#include <Wt/WLineEdit.h>
#include <Wt/WText.h>

#include <Wt/WIconPair.h>
#include <Wt/WTree.h>
#include <Wt/WTreeTable.h>
#include <Wt/WTreeTableNode.h>
#include <Wt/WApplication.h>
#include <Wt/WBreak.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WAnchor.h>
#include <Wt/WApplication.h>

#include <Wt/WServer.h>


Course::Course(){
    
}
// the constructor for the course object, it takes the coursename, the year, the description, the specific countainer the button of this course will be kept in, and the the description text widget that would update when the button is initialized
Course::Course(std::string className_, int classYear_, std::string description_, Wt::WContainerWidget *courseContainer, Wt::WText *descriptionText): className(className_), classYear(classYear_),description(description_){
    //courseButton = courseContainer -> addNew<Wt::WPushButton>(className_);
    preReq = new std::string[100];
    preReqCounter = 0;

}

// sets the css class of the courses's button to the new css class
void Course::setButtonStyle(std::string css_){
    courseButton->setStyleClass(css_);
   
}
// sets the button of the course object to a new button (keep in mind this a pointer)
void Course::setCourseButton(Wt::WPushButton *button){
    courseButton = button;
}
// sets specific attribute of the button's css style
void Course::setButtonCss(std::string css_){
    courseButton ->setAttributeValue("style",css_);
}

// returns the pointer pointing at the course object's button
Wt::WPushButton* Course::getClassButton(){

    return courseButton;
}


//  a method that initialized the button pressed function for the course button, it would set the description text widget which is contained in the description container to the this's courses description
void Course::buttonClicked(Wt::WText* descriptionText){

    courseButton->clicked().connect([=] {
        descriptionText -> setText(description);
    });

}
  

// adds a new pre requisite to this course course object
void Course::addPreReq(std::string courseName){
    preReq[preReqCounter] = courseName;
    ++preReqCounter;
}

// sets the courses's button's x and y css values in this course object to contain the information..
void Course::setxy(int x_, int y_){
    this -> x = x_;
    this -> y = y_;
}
// returns a prerequisite at a certain index
std::string Course::getPreReq(int index){
    return preReq[index];
}


// a method made to contain the value of the index of the courses's button in the stack of the container that contains all of the course buttons
void Course::setCourseButtonIndex(int index){
    courseButtonIndex = index;
}

// sets the courses graded value to 1 to act as a place holder if the course is added to the graded courses
void Course::setGraded(){
    graded = 1;
}

// sets the courses graded value to 0 if the course is removed from the graded list
void Course::setUngraded(){
    graded = 0;
}

// returns the graded value of the course. , if the graded value is not yet initialized, set the graded value to 0
int Course::getGraded(){
    
    if((graded != 0)&&(graded != 1)){
        graded = 0;
    }
    return graded;
    }
//get the class name


