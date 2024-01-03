#include "Module.h"


//constructor 1
Module::Module(): moduleName(""),size(0){
    courses = new Course[100];
}

//constructor 2
Module::Module(std::string moduleName_): moduleName(moduleName_), size(0){
    courses = new Course[100];
    reqCourses = new std::string[100];
    reqCourseSize = 0;
}
  

// deletes the courses list
Module::~Module(){
    delete[] courses;
}


//returns class By number
Course Module::getClass(int number){


    return courses[number];
}


// sets the class at a certain index to a new class
void Module::setClassByIndex(Course newCourse,int index){
    courses[index] = newCourse;
}

//returns class by name
Course Module::getClassByName(std::string className){
    Course toReturn = courses[0];
    for(int i = 0; i < size+1; i++){
        std::string currCourseName = (courses[i]).getClassName();

        if (currCourseName == className){
            toReturn = courses[i];
        }
    }
    return toReturn;
}

// get the index the class is contained in for this module given that className is the argument. 
int Module::getClassIndex(std::string className){
    int toReturn = 0;
    for(int i = 0; i < size+1; i++){
        std::string currCourseName = (courses[i]).getClassName();

        if (currCourseName == className){
            toReturn = i;
        }
    }
    return toReturn;
}

//adds new class, same the constructor of the course object, into the list of courses of the module object.
void Module::addClass(std::string className_, int classYear_, std::string description_, Wt::WContainerWidget *courseContainer, Wt::WText *descriptionText){
    Course tempCourse(className_, classYear_, description_,courseContainer, descriptionText);
    courses[size] = tempCourse;
    size = size + 1;
}

// returns the courses button pointer
Wt::WPushButton* Module::getCourseButton(std::string className){
    Course tempCourse = this -> getClassByName(className);
    return tempCourse.getClassButton();
}

// returns the courses button index number in the stack containing the button
Wt::WPushButton* Module::getCourseButton(int number){
    Course tempCourse = this -> getClass(number);
    return tempCourse.getClassButton();
}

// sets the classes x and y values given the class name as the argument.
void Module::SetClassxy(int x, int y, std::string className){
    int classIndex = getClassIndex(className);

    courses[classIndex].setxy(x,y);
}

// adds a new prerequisite to a course, given the course name course to the moudle.
void Module::addPreReq(std::string course1,std::string course2){

        courses[getClassIndex(course1)].addPreReq(course2);
    }


// adds a new required course to the module
void Module::addReqCourse(std::string course1){

        reqCourses[reqCourseSize] = course1;
        ++reqCourseSize;
    }

    

    