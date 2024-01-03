#include "Grades.h"


// constructs a new grade object
Grades::Grades(){
    className = "";
    grade = 0;
    
}
// constructs a new grade object given the class name and the grade
Grades::Grades(std::string className_, int grade_){
    className = className_;
    grade = grade_;
}