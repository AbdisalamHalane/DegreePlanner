
#include "DegreePlanner.h"
#include <sqlite3.h>

Wt::WContainerWidget *reqCoursesContainer ;
Wt::WText* avergaeText ;
Wt::WContainerWidget *adminPageContainer;

bool createUserTable() {
    sqlite3* db; 

    int rc; 

    rc = sqlite3_open("Degreedatabase.db", &db);
    if (rc) {
        return false;
    }


    std::string sql = "CREATE TABLE IF NOT EXISTS user ("
                      "email TEXT PRIMARY KEY, "
                      "password TEXT NOT NULL, "
                      "name TEXT NOT NULL);";


    rc = sqlite3_exec(db, sql.c_str(), 0, 0, 0);
    if (rc != SQLITE_OK) {


        sqlite3_close(db);
        return false;
    }


    sqlite3_close(db);
    return true;
}

bool createCoursesTable() {
    sqlite3* db;
    char* errMsg = 0;
    int rc;


    rc = sqlite3_open("Degreedatabase.db", &db);


    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    std::string sql = "CREATE TABLE IF NOT EXISTS Courses ("
                      "coursename TEXT PRIMARY KEY, "
                      "courseindex INT, "
                      "prereqsize INT, "
                      "courseYear INT, "
                      "graded INT, "
                      "description TEXT, "
                      "prereq_names TEXT);"; 

    rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return false;
    }

    sqlite3_close(db);
    return true;
}

bool createGradesTable() {
    sqlite3* db;
    char* errMsg = 0;
    int rc;

    rc = sqlite3_open("Degreedatabase.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Execute the SQL statement

    std::string sql = "CREATE TABLE IF NOT EXISTS Grades ("
                      "UserEmail TEXT , "
                      "Grade INT, "
                      "Course TEXT);";

    rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return false;
    }

    sqlite3_close(db);
    return true;
}

bool createModuleTable() {
    sqlite3* db;
    char* errMsg = 0;
    int rc;

    rc = sqlite3_open("Degreedatabase.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    std::string sql = "CREATE TABLE IF NOT EXISTS Modules ("
                      "ModuleName TEXT, "
                      "ModuleCourses TEXT, "
                      "ModuleReqCourses TEXT, "
                      "ModuleReqCourseSize INT, "
                      "ModuleCourseSize INT);";

    rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return false;
    }

    sqlite3_close(db);
    return true;
}



bool addUser(const std::string& email, const std::string& name, const std::string& password) {
    sqlite3* db;
    char* errMsg = 0;
    int rc;

    rc = sqlite3_open("Degreedatabase.db", &db);

    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    std::ostringstream sql;
    sql << "INSERT INTO user (email, name, password) VALUES ("
        << "'" << email << "', "
        << "'" << name << "', "
        << "'" << password << "');";

    rc = sqlite3_exec(db, sql.str().c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return false;
    }

    sqlite3_close(db);
    return true;
}

bool addGrade1(const std::string& userEmail, int grade, const std::string& course) {
    sqlite3* db;
    char* errMsg = 0;
    int rc;

    rc = sqlite3_open("Degreedatabase.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    std::string sql = "INSERT INTO Grades (UserEmail, Grade, Course) VALUES (?, ?, ?);";

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }


    sqlite3_bind_text(stmt, 1, userEmail.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, grade);
    sqlite3_bind_text(stmt, 3, course.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Insertion failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }


    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}

bool removeGrade1(const std::string& userEmail, const std::string& course) {
    sqlite3* db;
    char* errMsg = 0;
    int rc;

    
    rc = sqlite3_open("Degreedatabase.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    
    std::string sql = "DELETE FROM Grades WHERE UserEmail = ? AND Course = ?;";

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    
    sqlite3_bind_text(stmt, 1, userEmail.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, course.c_str(), -1, SQLITE_STATIC);

    
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Deletion failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}

std::string getPassword(const std::string& email) {
    sqlite3* db;
    sqlite3_stmt* stmt;
    int rc;

    rc = sqlite3_open("Degreedatabase.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        throw std::runtime_error("Can't open database");
    }

    std::string sql = "SELECT password FROM user WHERE email = ?";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        throw std::runtime_error("Failed to prepare statement");
    }

    sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_STATIC);
    std::string password;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    } else {
        std::cerr << "Email not found." << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return password;
}
int getGrade(const std::string& email, const std::string& course) {
    int grade = 0 ;
    sqlite3* db;
    sqlite3_stmt* stmt;
    int rc;

    rc = sqlite3_open("Degreedatabase.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        throw std::runtime_error("Can't open database");
    }

    std::string sql = "SELECT Grade FROM Grades WHERE UserEmail = ? AND Course = ?";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        throw std::runtime_error("Failed to prepare statement");
    }

    sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, course.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        grade = sqlite3_column_int(stmt, 0);
    } else {
        std::cerr << "Email not found." << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return grade;
}

bool getGradesAndCourses(const std::string& userEmail, std::vector<int>& grades, std::vector<std::string>& courses) {
    sqlite3* db;
    char* errMsg = 0;
    int rc;

    
    rc = sqlite3_open("Degreedatabase.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    std::string sql = "SELECT Grade, Course FROM Grades WHERE UserEmail = ?;";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

   
    sqlite3_bind_text(stmt, 1, userEmail.c_str(), -1, SQLITE_STATIC);

    
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int grade = sqlite3_column_int(stmt, 0);
        std::string course = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

        grades.push_back(grade);
        courses.push_back(course);
    }

    if (rc != SQLITE_DONE) {
        std::cerr << "Query failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}



Wt::WText *description ;
Wt::WText *description1 ;
Wt::WText *courseName ;
Wt::WLineEdit* gradeInput;
Wt::WTable* table;
std::string preReqs ="";

int currentRequiredMark = 50;


std::string honSpecString = "This is the program that will give you the most concentrated background and deep knowledge of almost all areas of computer science. In order to receive the Honors designation, you must maintain a 70% average and you must get at least a 60% in every required course.";
std::string specString = "This program will give a strong background in almost all areas of computer science. You could learn about graphics, networks, security, databases, the internet, computer law. This module offers a wealth of possiblities!";
std::string majorString ="This program provides a solid background in computer science, covering the foundational elements of the discipline. While perhaps best used as part of a Double Major (see Add-On Modules for Other Degrees), this module can be used as a basis for a degree in Computer Science as well.";
std::string minorString ="Consider this program if you are planning another degree and you want a solid background in computer science principles.";
unsigned gradeCounter;
unsigned removed;

std::string courseTake__ = "Course Taken? Add the course with your grade, make sure it is over : ";


std::string honSpecName ="Honors Specialization in Computer Science";
std::string specName = "Specialization in Computer Science";
std::string majorName = "Major in Computer Science";
std::string minorName = "Minor in Computer Science";
std::string requiredC ="";


std::string newD; 
std::string* newD_ = new std::string[100];


//
namespace {
    struct Employee {
        std::string firstName;
        std::string lastName;
        double pay;

        Employee(const std::string& aFirstName,
                 const std::string& aLastName,
                 double aPay)
          : firstName(aFirstName),
            lastName(aLastName),
            pay(aPay) { }
    };

    Employee employees[] = {
      Employee("CS1027", "0.5", 100),
      Employee("CS1033", "0.5", 50),
      Employee("CS2210", "0.5", 10)
    };
}
using namespace Wt;
//







User* admin_ = new User("MAINADMIN","$$1212");



Module module("MAIN MODULE");

Module honSpecM("HonSpec");
Module specM("Spec");
Module majorM("Major");
Module minorM("Minor");



// initial button click handler, not used anymore
void addButtonWithClickHandler(Wt::WContainerWidget* container, std::string ButtonName,std::string css_, std::string tempD,Wt::WText* descriptionText) {
    

    auto button = container->addWidget(std::make_unique<Wt::WPushButton>(ButtonName));
    button ->setAttributeValue("style",css_);
    button->clicked().connect([=] {
        descriptionText -> setText(tempD);
        return;
    });
}


//sets button to green if take
void DegreePlanner::buttonClickHandler(int x, Wt::WContainerWidget* con){

    auto button = con->widget(x);
    button->setStyleClass("courseButtonReq");

}


//sets buttons to yello
void DegreePlanner::defaultButton(int x, Wt::WContainerWidget* con){
    auto button = con->widget(x);
    button->setStyleClass("courseButton");
}


//sets buttons to orange 
void DegreePlanner::courseTakenButton(int x, Wt::WContainerWidget* con){

    auto button = con->widget(x);
    button->setStyleClass("courseButtonTaken");
}

// updates all teh courses that are already taken 

void DegreePlanner::autoCourseTakenButton(Wt::WContainerWidget * con){
    int courseSize = currentUser -> returnGradeSize();
    for(int i = 0; i < courseSize; i++){

        courseTakenButton((module.getClassByName((currentUser -> returnGradeByIndex(i)).getClassName())).getButtonIndex(),con);
    }
}

void moduleButtonHandler(Wt::WPushButton* button, Wt::WContainerWidget* con1, Wt::WContainerWidget* con2){
    button->clicked().connect([=]() {
        for(int i = 0; i<honSpecM.getReqCourseSize();i++){
        }
        con1 -> setHidden(false);
        con2 -> setHidden(true);
    });
}

int graded = 0;

// if the course button is pressed, show the add grade button only if the course is not taken, else show the course already taken, remove it button. Also update the description text widget contained in the descripton pop up

void courseButtonOnClick(Wt::WPushButton* button,Wt::WText* descriptionText, std::string tempD, std::string className,Wt::WContainerWidget* container, int index,Wt::WPushButton * grade, Wt::WPushButton * removeGrade ) {
    
    button->clicked().connect([=] {

        preReqs ="";
        
        for(int i = 0; i< (module.getClassByName(className)).getPreReqSize(); i++){
            preReqs = preReqs + " " + (module.getClassByName(className)).getPreReq(i);
        }
        if((module.getClassByName(className)).getGraded() == 0){
                removeGrade -> setHidden(true);
                grade -> setHidden(false);
                gradeInput -> setHidden(false);
                
        }
        if((module.getClassByName(className)).getGraded() == 1){
            grade -> setHidden(true);
            removeGrade ->setHidden(false);
            gradeInput -> setHidden(true);
        }
        courseName -> setText(className);
        descriptionText -> setText(tempD);
        description1 -> setText(preReqs);
        container -> setHidden(false);


    });
}

// if a course is already taken, once user logs in, it updates the graded course information to those courses
void DegreePlanner::setGradedCourses(){

    int tempAverage = 0 ;
    for(int i = 0; i< currentUser ->returnGradeSize();i++ ){


        Course temp = module.getClassByName((currentUser -> returnGradeByIndex(i)).getClassName());
            
        temp.setGraded();
        module.setClassByIndex(temp,module.getClassIndex((currentUser -> returnGradeByIndex(i)).getClassName()));

        /**/
        std::string currClassName = (currentUser -> returnGradeByIndex(i)).getClassName();
        std::string currClassGrade = std::to_string((currentUser -> returnGradeByIndex(i)).getGrade());
        table->elementAt(i+1, 0)
                ->addNew<Wt::WText>(Wt::WString("{1}").arg("-"));
            table->elementAt(i+1, 1)
                ->addNew<Wt::WText>(currClassName);
            table->elementAt(i+1, 2)
                ->addNew<Wt::WText>("0.5");
            table->elementAt(i+1, 3)
                ->addNew<Wt::WText>(currClassGrade);
        ++gradeCounter;
        tempAverage = tempAverage + (currentUser -> returnGradeByIndex(i)).getGrade();


    }

    if(gradeCounter != 1){
        avergaeText->setText("Average: " + std::to_string(tempAverage/(gradeCounter-1)));
    }

}



//adds the lines connecting prereqs
void makeWeb(Wt::WContainerWidget* container, Course course1, Course course2) {
    
    //gets the difference between the x and y values of the two courses, (the prereq being course 1 in this case)
    int x = course2.getX()+10 - course1.getX();
    int y = course2.getY()+10 - course1.getY();

    // stores the hypotenues of the x and y value into the length variable
    int length = ceil(sqrt(pow(x,2)+pow(y,2)));
    // gets the arc sin of the y value divided by the length and stores into the angle object, this would contain the angle
    float angle = asin(float(y)/length);
    auto web = container->addWidget(std::make_unique<Wt::WContainerWidget>());   // initializes the line connecting the two courses
    web -> setStyleClass("honspec");
    std::string width = std::to_string(length);

    
    std::string angle_ = std::to_string(angle);

    std::string top = std::to_string(course1.getY()+50);    //offsets the top positon of the line by 50, starting at the y positon of course 1 
    std::string left = std::to_string(course1.getX()+150); //offsets the left positon of the line by 150 , starting at the x positon of course 2
    // sets the attributes to all the gathered information
    web -> setAttributeValue("style", "transform-origin: top left;position:fixed; width:"+width+"px; top:"+top+"px; left:"+left+"px;transform: rotate("+angle_+"rad); height: 1px; background-color: black;");
    
}



// LOGIN METHOD   

void DegreePlanner::login(Wt::WContainerWidget * con){


    // Title

}


//global variables. 
std::vector<int> memory;

std::vector<std::string> words; 

std::string correctWord ;

DegreePlanner::DegreePlanner(const Wt::WEnvironment& env)
  : WApplication(env)
{ 

    admin_ -> setAdmin();
        
    Wt::WApplication::instance()->useStyleSheet("main.css");
    createGradesTable();
    createUserTable();
    createModuleTable();
    createCoursesTable();




    // initializes the main Container (private instance of Degree planner)
    mainContainer = root()->addNew<Wt::WContainerWidget>();
    mainContainer -> setStyleClass("mainContainer");

    
    setTitle("Degree Planner");  
    Wt::WApplication::instance()->useStyleSheet("main.css");
    


    /*
    *
    *
    *
    *
    * 
    * 
    * 
    */

    // CODE FOR MODULES 


    // Code for user input


    Wt::WContainerWidget* container = root() ->addNew<Wt::WContainerWidget>();
    container->setStyleClass("logs-container");

    auto title = container->addNew<Wt::WText>("Western Login");
    title->setStyleClass("title");

    // Username input
    auto userNameEdit = container->addNew<Wt::WLineEdit>();
    userNameEdit->setStyleClass("form-group");
    userNameEdit->setPlaceholderText("Username or email");

    // Password input
    auto passwordEdit = container->addNew<Wt::WLineEdit>();
    passwordEdit->setStyleClass("form-group");
    passwordEdit->setPlaceholderText("Password");
    passwordEdit->setEchoMode(Wt::EchoMode::Password);

    auto loginButton = container->addNew<Wt::WPushButton>("Log in");
    Wt::WContainerWidget *SignUpContainer = root()->addNew<Wt::WContainerWidget>();
    SignUpContainer-> setStyleClass("signs-container");
    auto SignUpButton = container->addNew<Wt::WPushButton>("Sign Up");
    auto emailLabel = SignUpContainer->addWidget(std::make_unique<Wt::WText>("Email: "));
    auto emailEdit = SignUpContainer->addWidget(std::make_unique<Wt::WLineEdit>());
    auto usernameWarning = SignUpContainer->addWidget(std::make_unique<Wt::WText>(""));
    emailEdit->setPlaceholderText("Enter your email");

// Create and add username input field
    auto usernameLabel = SignUpContainer->addWidget(std::make_unique<Wt::WText>("Username: "));
    auto usernameEdit = SignUpContainer->addWidget(std::make_unique<Wt::WLineEdit>());

    usernameEdit->setPlaceholderText("Enter your username");



// Create and add password input field
    auto passwordLabel = SignUpContainer->addWidget(std::make_unique<Wt::WText>("Password: "));
    auto passwordEdit2 = SignUpContainer->addWidget(std::make_unique<Wt::WLineEdit>());
    passwordEdit2->setPlaceholderText("Enter your password");
    passwordEdit2->setEchoMode(Wt::EchoMode::Password);
    SignUpContainer->setHidden(true);
    currentUser = new User("","");

    // jumpt to other login section



    auto inSignUpButton = SignUpContainer->addWidget(std::make_unique<Wt::WPushButton>("Sign Up"));
    auto CloseButton = SignUpContainer->addWidget(std::make_unique<Wt::WPushButton>("Close"));



    SignUpButton->clicked().connect([=] {
        usernameWarning->setText("");


        // Click handler for the newly created inner button

        inSignUpButton->clicked().connect([=] {
            //usernameWarning->setText("");

            std::string email = emailEdit->text().toUTF8();
            std::string user = usernameEdit->text().toUTF8();
            std::string pass = passwordEdit2->text().toUTF8();

            bool success = addUser(email, user, pass);

            if (success){
                //usernameWarning->setText("");
                emailEdit->setText("");
                usernameEdit->setText("");
                passwordEdit2->setText("");
                SignUpContainer->setHidden(true);
            }else{
                emailEdit->setText("");
                usernameEdit->setText("");
                passwordEdit2->setText("");
                usernameWarning->setText("Email Already Exists");


            }

        });

        CloseButton->clicked().connect([=] {
            usernameWarning->setText("");
            SignUpContainer->setHidden(true);

        });



        SignUpContainer->setHidden(false);

    });


    //login(root());
   

    Wt::WContainerWidget * moduleContainer = mainContainer->addNew<Wt::WContainerWidget>();
    

    moduleContainer -> setStyleClass("moduleContainer");

    Wt::WPushButton *honSpec = moduleContainer->addNew<Wt::WPushButton>("Honor Specialization");
    Wt::WPushButton *spec = moduleContainer->addNew<Wt::WPushButton>("Specialization");
    Wt::WPushButton *major = moduleContainer->addNew<Wt::WPushButton>("Major");
    Wt::WPushButton *minor = moduleContainer->addNew<Wt::WPushButton>("Minor");


    

    honSpec -> setStyleClass("honspec");
    spec -> setStyleClass("spec");
    major -> setStyleClass("majorButton");
    minor -> setStyleClass("minorButton");



    

    mainContainer->setHidden(true);


    // iniialize courses taken if already existing 



    
 

    // MODULE CODE ENDS
    /*
    *
    *
    * 
    * 
    * 
    * 
    * 
    *
    */
    //REQUIRED COURSES CONATINER


    // This following section sets up the different Year buttons




    // This is the end of the section above


    // This follows section sets up the display the courses in the left navigation bar as well as the main course display

    gradeCounter = 0;
    int average = 0 ; 


    reqCoursesContainer = mainContainer->addNew<Wt::WContainerWidget>();

    reqCoursesContainer -> setStyleClass("reqCoursesContainer");

    //Course Description Section  pop up

    Wt::WContainerWidget *coursesDescription = mainContainer->addNew<Wt::WContainerWidget>();

    coursesDescription -> setStyleClass("coursesDescription");

    Wt::WContainerWidget *coursesDescription_ = coursesDescription->addNew<Wt::WContainerWidget>();
    coursesDescription_ -> setStyleClass("coursesDescription_");

    coursesDescription -> setAttributeValue("style", "position: fixed; top: 50%; left: 50%; transform: translate(-50%, -50%);");
    coursesDescription -> setHidden(true);
    //coursesDescription -> setHidden(true);
    Wt::WPushButton *back__ = coursesDescription->addNew<Wt::WPushButton>("Back");
    back__ -> setStyleClass("courseYearButton");
    back__ ->setAttributeValue("style", "position:fixed; top:0%; left:90%; height:5%; width: 10%; font-size: 10px;");
    back__->clicked().connect([=]() {
        coursesDescription -> setHidden(true);
    });
    courseName = coursesDescription_ -> addNew<Wt::WText>("#");
    courseName -> setAttributeValue("style","top:20%");

    coursesDescription -> addNew <Wt::WBreak>();

    Wt::WText *addText1 = coursesDescription->addNew<Wt::WText>("Prerequisites");
    addText1 -> setAttributeValue("style","postion:relative;transform: translate(-50%, 50%);");
    coursesDescription -> addNew <Wt::WBreak>();


    Wt::WContainerWidget *preReqList = coursesDescription->addNew<Wt::WContainerWidget>();

    preReqList -> setStyleClass("coursesDescription_");
    description1 = preReqList -> addNew<Wt::WText>("PREREQS");


    coursesDescription -> addNew <Wt::WBreak>();
    coursesDescription -> addNew <Wt::WBreak>();

    Wt::WText *addText = coursesDescription->addNew<Wt::WText>("Course Taken? Add the course with your grade, make sure it is over : 50");

    coursesDescription_ -> addNew <Wt::WBreak>();
    description = coursesDescription_ -> addNew<Wt::WText>("#");

    gradeInput = coursesDescription->addNew<Wt::WLineEdit>("0");

    gradeInput -> setStyleClass("gradeInput");
    gradeInput -> setAttributeValue("style","position:relative;");
    coursesDescription -> addNew <Wt::WBreak>();

    coursesDescription -> addNew <Wt::WBreak>();

    Wt::WPushButton *addGrade = coursesDescription->addNew<Wt::WPushButton>("Add Course and Grade");
    addGrade -> setStyleClass ("addGradeButton");
    addGrade -> setAttributeValue("style","position:relative;");

    Wt::WPushButton *removeGrade = coursesDescription->addNew<Wt::WPushButton>("Remove Course and Grade");
    removeGrade -> setStyleClass ("addGradeButton");
    removeGrade -> setAttributeValue("style","position:relative;");


    addText -> setAttributeValue("style","postion:relative;transform: translate(-50%, 50%);");

    removeGrade -> setHidden(true);
    addGrade -> setHidden(true);



    // 


    //Containers for each Module's courses separated by year

    // The Following is the code for the courses in the computer science program

// The Following is the code for the courses in the computer science program

    module.addClass("CS1026",1,"The nature of Computer Science as a discipline; the design and analysis of algorithms and their implementation as modular, reliable, well-documented programs written in a modern programming language. Intended for students with little or no background in programming. ",reqCoursesContainer,description);
    module.addClass("CS1027",1,"A continuation for both Computer Science 1025A/B and Computer Science 1026A/B. Data organization and manipulation; abstract data types and their implementations in a modern programming language; lists, stacks, queues, trees; recursion; file handling and storage. ",reqCoursesContainer,description);
    module.addClass("CALC1000",1,"Review of limits and derivatives of exponential, logarithmic and rational functions. Trigonometric functions and their inverses. The derivatives of the trig functions and their inverses. L'Hospital's rules. The definite integral. Fundamental theorem of Calculus. Simple substitution. Applications including areas of regions and volumes of solids of revolution. ",reqCoursesContainer,description);
    module.addClass("MATH1600",1," Properties and applications of vectors; matrix algebra; solving systems of linear equations; determinants; vector spaces; orthogonality; eigenvalues and eigenvectors.",reqCoursesContainer,description);
    module.addClass("CALC1500",1,"An enriched version of Calculus 1000A/B. Basic set theory and an introduction to mathematical rigour. The precise definition of limit. Derivatives of exponential, logarithmic, rational trigonometric functions. L'Hospital's rule. The definite integral. Fundamental theorem of Calculus. Integration by substitution. Applications.",reqCoursesContainer,description);
    module.addClass("CALC1501",1,"Students who intend to pursue a degree in Actuarial Science, Applied Mathematics, Astronomy, Mathematics, Physics, or Statistics should take this course. Techniques of integration; The Mean Value Theorem and its consequences; series, Taylor series with applications; parametric and polar curves with applications; first order linear and separable differential equations with applications.",reqCoursesContainer,description);
        //updated - matia

    module.addClass("CS2208",2,"This course gives an understanding of what a modern computer can do. It covers the internal representation of various data types and focuses on the architectural components of computers (how these components are interconnected and the nature of the information flow between them). Assembly language is used to reinforce these issues. ",reqCoursesContainer,description);
    module.addClass("CS2209",2,"Propositional and predicate logic; representing static and dynamic properties of real-world systems; logic as a tool for representation, reasoning and calculation; logic and programming. ",reqCoursesContainer,description);
    module.addClass("CS2210",2,"Lists, stacks, queues, priority queues, trees, graphs, and their associated algorithms; file structures; sorting, searching, and hashing techniques; time and space complexity. ",reqCoursesContainer,description);    
    module.addClass("CS2211",2,"An introduction to software tools and systems programming. Topics include: understanding how programs execute (compilation, linking and loading); an introduction to a complex operating system (UNIX); scripting languages; the C programming language; system calls; memory management; libraries; multi-component program organization and builds; version control; debuggers and profilers.",reqCoursesContainer,description);
    module.addClass("CS2212",2,"A team project course that provides practical experience in the software engineering field. Introduction to the structure and unique characteristics of large software systems, and concepts and techniques in the design, management and implementation of large software systems.", reqCoursesContainer, description); 
    module.addClass("CS2214",2,"This course presents an introduction to the mathematical foundations of computer science, with an emphasis on mathematical reasoning, combinatorial analysis, discrete structures, applications and modeling, and algorithmic thinking. Topics include sets, functions, relations, algorithms, number theory, matrices, mathematical reasoning, counting, graphs and trees. ",reqCoursesContainer,description);
    module.addClass("W2101",2,"An introduction to the basic principles and techniques of good writing. The course will emphasize practical work and the development of writing skills for a variety of subjects and disciplines. ",reqCoursesContainer,description);
    module.addClass("W2111",2,"This course will introduce students to various genres of workplace writing such as letters, memos, and reports. Topics include: employment communications (application letters and resumes); business writing style; positive, negative, and persuasive messages; cross-cultural communication; oral communication. ",reqCoursesContainer,description);
    module.addClass("W2125",2,"An intensive, practical study of exposition in discursive prose, this course aims to develop students' abilities to think critically and write persuasively using argumentation, visual rhetoric, and relevant source materials. Students consider diverse types of prose across disciplines and focus on essay development through writing, rewriting, and revision. ",reqCoursesContainer,description);
    module.addClass("W2131",2," Writing in the Sciences introduces students to the basic principles and genres of writing required for science students in their undergraduate careers: lab reports, summaries of scientific research, and scientific review essays. The course will focus on drafting and revising various reports written on scientific topics.",reqCoursesContainer,description);
    module.addClass("MATH2155",2," This course provides an introduction to logical reasoning and proofs. Topics include sets, counting (permutations and combinations), mathematical induction, relations and functions, partial order relations, equivalence relations, binary operations, elementary group theory and applications to error-correcting codes.",reqCoursesContainer,description);
    module.addClass("STATS2857",2,"Probability axioms, conditional probability, Bayes' theorem. Random variables motivated by real data and examples. Parametric univariate models as data reduction and description strategies. Multivariate distributions, expectation and variance. Likelihood function will be defined and exploited as a means of estimating parameters in certain simple situations.",reqCoursesContainer,description);
    module.addClass("STATS2244",2,"An introductory course in the application of statistical methods, intended for students in departments other than Statistical and Actuarial Sciences, Applied Mathematics, Mathematics, or students in the Faculty of Engineering. Topics include sampling, confidence intervals, analysis of variance, regression and correlation. ",reqCoursesContainer,description);


    module.addClass("CS3305",3,"Survey of major operating systems; interprocess communication; multi-tasking; scheduling; memory management; performance and measurement issues; trade-offs in operating system design; concurrency and deadlock. ",reqCoursesContainer,description);
    module.addClass("CS3307",3,"Software design and analysis techniques with particular emphasis on object-oriented design and analysis; a team project will be developed using an object-oriented language such as Java, C++ or Smalltalk.",reqCoursesContainer,description);
    module.addClass("CS3331",3,"Languages as sets of strings over an alphabet; operations on languages; finite automata, regular expressions; language hierarchy; Turing machines; models of computation. ",reqCoursesContainer,description);
    module.addClass("CS3319",3,"This course introduces student to database management systems. The course discusses how to design a database for a real application. Design of good relational tables is followed by the use of these tables. The standard SQL data manipulation language will be used. Following the logical design phase, we discuss issues of physical storage structures and indexes. Finally, issues of backup and recovery, concurrency control and data distribution will be introduced.",reqCoursesContainer,description);
    module.addClass("CS3340",3,"Upper and lower time and space bounds; levels of intractability; graph algorithms; greedy algorithms; dynamic algorithms; exhaustive search techniques; parallel algorithms.",reqCoursesContainer,description);
    module.addClass("CS3342",3," Specification and analysis of programming languages; data types and structures; bindings and access structures; run-time behavior of programs; compilation vs. interpretation. Comparative presentation of at least three programming languages addressing the above concepts.",reqCoursesContainer,description);
    module.addClass("CS3350",3,"Topics include: semiconductor technologies, gates and circuits, buses, semiconductor memories, peripheral interfaces, I/O techniques, A/D conversion, standards, RISC. ",reqCoursesContainer,description);
    module.addClass("DATA3000",3, "Basic principles of machine learning (estimation, optimization, prediction, generalization, bias-variance trade-off, regularization) in the context of supervised (linear models, decision trees, deep neuronal networks) and unsupervised (clustering and dimensionality reduction) statistical learning techniques. The course emphasizes the ability to apply techniques to real data sets and critically evaluate their performance. ", reqCoursesContainer, description);

    module.addClass("CS4490Z",4,"",reqCoursesContainer,description);
    module.addClass("CS4470Y",4,"MANDATIRY COURSE FOR SOFTWARE ENGINEERING MINOR - An examination of industrial-style software development issues related to managing and maintaining large-scale software systems; in a group project, students will examine software maintenance and configuration management concepts, tools, techniques, risks and benefits; case studies. ",reqCoursesContainer,description);
    module.addClass("CS4480Y",4,"MANDATIRY COURSE FOR GAME DEVELOPMENT MINOR - Industrial-style development issues related to the creation of games of commercial scale and quality, both for entertainment and serious game applications; in a group project, students will examine concepts, theories, tools, technologies, and techniques for code and content generation for modern games.",reqCoursesContainer,description);
    module.addClass("Additional 4th Year CS Course", 4, "Students must have taken a total of 1.0 course from {4000s level CS, DATASCI3000}",reqCoursesContainer,description);

    module.addPreReq("CS1027","CS1026");

    module.addPreReq("CS2208","CS1027");
    module.addPreReq("CS2209","CS1027"); module.addPreReq("CS2209","CALC1000"); module.addPreReq("CS2209","CALC1500"); module.addPreReq("CS2209","CALC1501");  module.addPreReq("CS2209","MATH1600");
    module.addPreReq("CS2210","CS1027"); module.addPreReq("CS2210","CALC1000"); module.addPreReq("CS2210","CALC1500"); module.addPreReq("CS2210","CALC1501");  module.addPreReq("CS2210","MATH1600");
    module.addPreReq("CS2211","CS1027");
    module.addPreReq("CS2212","CS2210"); module.addPreReq("CS2212","CS2211");

    module.addPreReq("CS2214","CALC1500"); module.addPreReq("CS2214","CALC1501");  module.addPreReq("CS2214","MATH1600");
    module.addPreReq("STATS2857","CALC1500"); module.addPreReq("STATS2857","CALC1501");  module.addPreReq("STATS2857","MATH1600");
    module.addPreReq("STATS2244","CALC1500"); module.addPreReq("STATS2244","CALC1501");  module.addPreReq("STATS2244","MATH1600");
    module.addPreReq("MATH2155","CALC1500"); module.addPreReq("MATH2155","CALC1501");  module.addPreReq("MATH2155","MATH1600");

    module.addPreReq("CS3331","CS2214"); module.addPreReq("CS3331","MATH2155");
    module.addPreReq("CS3307","CS2212"); module.addPreReq("CS3307","CS2210");  module.addPreReq("CS3307","CS2211");

    module.addPreReq("CS3305","CS2208"); module.addPreReq("CS3305","CS2210");  module.addPreReq("CS3305","CS2211");
    module.addPreReq("CS3340","CS2210"); module.addPreReq("CS3340","CS2211");  module.addPreReq("CS3340","CS2214"); module.addPreReq("CS3340","MATH2155");


    module.addPreReq("CS3342","CS2211");

    module.addPreReq("CS3350","CS2210"); module.addPreReq("CS3350","CS2211");  module.addPreReq("CS3350","CS2209"); module.addPreReq("CS3350","CS2208");
  
    
      
    honSpecM.addReqCourse("CS2209"); honSpecM.addReqCourse("CS2208"); honSpecM.addReqCourse("CS2210"); honSpecM.addReqCourse("CS2211"); honSpecM.addReqCourse("CS2212"); honSpecM.addReqCourse("CS2214"); honSpecM.addReqCourse("CS3331"); honSpecM.addReqCourse("CS3305");
    honSpecM.addReqCourse("CS3307"); honSpecM.addReqCourse("CS3340"); honSpecM.addReqCourse("CS3342"); honSpecM.addReqCourse("CS3350"); honSpecM.addReqCourse("CS4490Z"); honSpecM.addReqCourse("W2101"); honSpecM.addReqCourse("STATS2244"); 

    specM.addReqCourse("CS2209"); specM.addReqCourse("CS2208"); specM.addReqCourse("CS2210"); specM.addReqCourse("CS2211"); specM.addReqCourse("CS2212");   specM.addReqCourse("CS3305"); specM.addReqCourse("CS3307"); specM.addReqCourse("CS3331"); specM.addReqCourse("CS3342"); specM.addReqCourse("CS3350"); 
    specM.addReqCourse("CS2214"); specM.addReqCourse("W2101"); specM.addReqCourse("STATS2244"); 

    majorM.addReqCourse("CS2208"); majorM.addReqCourse("CS2209"); majorM.addReqCourse("CS2210"); majorM.addReqCourse("CS2211"); majorM.addReqCourse("CS2212"); majorM.addReqCourse("CS3305"); majorM.addReqCourse("CS3307"); majorM.addReqCourse("CS2214"); 

    minorM.addReqCourse("CS2208");  minorM.addReqCourse("CS2208"); minorM.addReqCourse("CS2208"); minorM.addReqCourse("CS2210"); minorM.addReqCourse("CS2211"); minorM.addReqCourse("CS2214"); 
    // End of the courses section
    


    // The following is the code for the positoning of each course button in the main screen

    int first_ = 0;
    int second_ = 0;
    int third_ = 0;
    int fourth_ = 0;

    for (int i = 0; i<module.getSize(); i++){

        int year = (module.getClass(i)).getClassYear();
        if ( year%4 == 1){
            ++first_;
            module.SetClassxy(200,first_*200,(module.getClass(i)).getClassName());
           }
        if ( year%4 == 2){
            ++second_;
            module.SetClassxy(700,second_*200,(module.getClass(i)).getClassName());
        }

        if ( year%4 == 3){
            ++third_ ;
            module.SetClassxy(1200,third_*200,(module.getClass(i)).getClassName());
            }

        if ( year%4 == 0){
            ++fourth_;
            module.SetClassxy(1700,fourth_*200,(module.getClass(i)).getClassName());

        }

    }

  // the following code makes the webs for each prereq to course 
    for(int i = 0 ; i<module.getSize(); i++){
        for(int j = 0; j<  (module.getClass(i)).getPreReqSize() ;j++){
                makeWeb(reqCoursesContainer,module.getClassByName((module.getClass(i)).getPreReq(j)),module.getClass(i));
        }
    }

    autoCourseTakenButton(reqCoursesContainer);
   
    // the followin code initalizes all the buttons


    for (int i = 0; i<module.getSize(); i++){

        
        int year = (module.getClass(i)).getClassYear();


        if ( year%4 == 1){
            std::string space_ = std::to_string((module.getClass(i)).getY());
            std::string css_ =  "position:fixed; left: 200px;top:"+space_+"px";
            Wt::WPushButton* tempButton = reqCoursesContainer ->addNew<Wt::WPushButton>((module.getClass(i)).getClassName());
            tempButton -> setStyleClass("courseButton");
            tempButton -> setAttributeValue("style",css_);

            Course tempCourse = (module.getClass(i));
            tempCourse.setCourseButton(tempButton);

            tempCourse.setCourseButtonIndex((reqCoursesContainer->count())-1);    

            module.setClassByIndex(tempCourse,i);
            courseButtonOnClick(tempButton,description,(module.getClass(i)).getDescription(),(module.getClass(i)).getClassName(),coursesDescription,(module.getClass(i)).getGraded(),addGrade,removeGrade);
        }

        (module.getClass(i)).getClassYear();
        if ( year%4 == 2){
            std::string space_ = std::to_string((module.getClass(i)).getY());
            std::string css_ =  "position:fixed;left: 700px; top:"+space_+"px;";

            Wt::WPushButton* tempButton = reqCoursesContainer ->addNew<Wt::WPushButton>((module.getClass(i)).getClassName());
            tempButton -> setStyleClass("courseButton");
            tempButton -> setAttributeValue("style",css_);
            Course tempCourse = (module.getClass(i));
            tempCourse.setCourseButton(tempButton);

            tempCourse.setCourseButtonIndex((reqCoursesContainer->count())-1);    

            module.setClassByIndex(tempCourse,i);


            courseButtonOnClick(tempButton,description,(module.getClass(i)).getDescription(),(module.getClass(i)).getClassName(),coursesDescription,(module.getClass(i)).getGraded(),addGrade,removeGrade);
        }

        if ( year%4 == 3){
            std::string space_ = std::to_string((module.getClass(i)).getY());
            std::string css_ =  "position:fixed; left: 1200px; top:"+space_+"px;";
            Wt::WPushButton* tempButton =reqCoursesContainer ->addNew<Wt::WPushButton>((module.getClass(i)).getClassName());
            tempButton -> setStyleClass("courseButton");
            tempButton -> setAttributeValue("style",css_);
            Course tempCourse = (module.getClass(i));
            tempCourse.setCourseButton(tempButton);

            tempCourse.setCourseButtonIndex((reqCoursesContainer->count())-1);    

            module.setClassByIndex(tempCourse,i); //

            courseButtonOnClick(tempButton,description,(module.getClass(i)).getDescription(),(module.getClass(i)).getClassName(),coursesDescription,(module.getClass(i)).getGraded(),addGrade,removeGrade);
        }

        if ( year%4 == 0){
            std::string space_ = std::to_string((module.getClass(i)).getY());
            std::string css_ =  "position:fixed; left: 1700px; top:"+space_+"px;";
            Wt::WPushButton* tempButton = reqCoursesContainer->addNew<Wt::WPushButton>((module.getClass(i)).getClassName());
            tempButton -> setStyleClass("courseButton");
            tempButton -> setAttributeValue("style",css_);
            Course tempCourse = (module.getClass(i));
            tempCourse.setCourseButton(tempButton);

            tempCourse.setCourseButtonIndex((reqCoursesContainer->count())-1);    

            module.setClassByIndex(tempCourse,i);

            courseButtonOnClick(tempButton,description,(module.getClass(i)).getDescription(),(module.getClass(i)).getClassName(),coursesDescription,(module.getClass(i)).getGraded(),addGrade,removeGrade);

        }

    }

    // End of the years //


    //Module Function Code
    
    Wt::WContainerWidget *coursesContainer = mainContainer->addNew<Wt::WContainerWidget>();
    coursesContainer -> setStyleClass("coursesContainer");
    coursesContainer -> setHidden(false);
    Wt::WContainerWidget *moduleDescription = coursesContainer->addNew<Wt::WContainerWidget>();
    moduleDescription -> setStyleClass("moduleDescription");
    Wt::WContainerWidget *moduleDescription_ = moduleDescription->addNew<Wt::WContainerWidget>();
    moduleDescription_ -> setStyleClass("moduleDescription_");

    Wt::WPushButton *back_ = coursesContainer->addNew<Wt::WPushButton>("Back");
    back_ -> setStyleClass("courseYearButton");
    back_ ->setAttributeValue("style", "position:fixed; top:95%; left:0%; height: 5%; width: 100%;");

    Wt::WPushButton *gradesButton = coursesContainer->addNew<Wt::WPushButton>("See Your Grades");

    gradesButton -> setStyleClass("courseYearButton");
    gradesButton ->setAttributeValue("style", "position:fixed; top:80%; left:0%; height: 10%; width: 100%;");


        // Nor hence hoped her after other known defer his. For county now sister engage had season

    Wt::WText *addText3 = moduleDescription_->addNew<Wt::WText>("HELLO");
    moduleDescription_ -> addNew <Wt::WBreak>();
    moduleDescription_ -> addNew <Wt::WBreak>();

    addText3 -> setAttributeValue("style","postion:relative;transform: translate(-50%, 50%);font-size:30px; color: black;");

    Wt::WText *addText2 = moduleDescription_->addNew<Wt::WText>("");
    addText2 -> setAttributeValue("style","postion:relative;transform: translate(-50%, 50%);");

    moduleDescription_ -> addNew <Wt::WBreak>();
    moduleDescription_ -> addNew <Wt::WBreak>();

    Wt::WText *addText5 = moduleDescription_->addNew<Wt::WText>("Required Courses");
    addText5 -> setAttributeValue("style","postion:relative;transform: translate(-50%, 50%);font-size:30px; color:black;");

    moduleDescription_ -> addNew <Wt::WBreak>();
    moduleDescription_ -> addNew <Wt::WBreak>();

    Wt::WText *addText4 = moduleDescription_->addNew<Wt::WText>("");
    addText4 -> setAttributeValue("style","postion:relative;transform: translate(-50%, 50%);");



    coursesContainer -> setHidden(true);
    // the module button functions, sets the required courses to green if not taken when pressed
    honSpec->clicked().connect([=]() {
        for(int i = 0; i<honSpecM.getReqCourseSize();i++){
                        

            buttonClickHandler((module.getClassByName((honSpecM.getReqCourses())[i])).getButtonIndex(),reqCoursesContainer);
        }
        addText2 ->setText(honSpecString);
        addText3 ->setText(honSpecName);


        requiredC ="";
        
        for(int i = 0; i< honSpecM.getReqCourseSize(); i++){
            requiredC = requiredC + " " + honSpecM.getReqCourseIndex(i);
        }

        std::string tempTEXT = courseTake__ + std::to_string(70)+ "%.";

        addText -> setText(tempTEXT);
        currentRequiredMark = 70;

        addText4 ->setText(requiredC);
        coursesContainer -> setHidden(false);
        moduleContainer -> setHidden(true);
        autoCourseTakenButton(reqCoursesContainer);
        
    });
    spec->clicked().connect([=]() {
        for(int i = 0; i<specM.getReqCourseSize();i++){
            buttonClickHandler((module.getClassByName((specM.getReqCourses())[i])).getButtonIndex(),reqCoursesContainer);
        }
        addText2 ->setText(specString);
        addText3 ->setText(specName);

        requiredC ="";
        
        for(int i = 0; i< specM.getReqCourseSize(); i++){
            requiredC = requiredC + " " + specM.getReqCourseIndex(i);
        }

        std::string tempTEXT = courseTake__ + std::to_string(60)+ "%.";
        currentRequiredMark = 60;

        addText -> setText(tempTEXT);
        addText4 ->setText(requiredC);
        coursesContainer -> setHidden(false);
        moduleContainer -> setHidden(true);
        autoCourseTakenButton(reqCoursesContainer);
    });

    major->clicked().connect([=]() {
        for(int i = 0; i< majorM.getReqCourseSize();i++){
            buttonClickHandler((module.getClassByName((majorM.getReqCourses())[i])).getButtonIndex(),reqCoursesContainer);
        }
        addText2 ->setText(majorString);
        addText3 ->setText(majorName);

        requiredC ="";
        
        for(int i = 0; i< majorM.getReqCourseSize(); i++){
            requiredC = requiredC + " " + majorM.getReqCourseIndex(i);
        }
        

        currentRequiredMark = 60;
        addText4 ->setText(requiredC);
        coursesContainer -> setHidden(false);
        moduleContainer -> setHidden(true);
        autoCourseTakenButton(reqCoursesContainer);
        std::string tempTEXT = courseTake__ + std::to_string(60)+ "%.";

        addText -> setText(tempTEXT);
    });

    minor ->clicked().connect([=]() {
        for(int i = 0; i< minorM.getReqCourseSize();i++){
            buttonClickHandler((module.getClassByName((minorM.getReqCourses())[i])).getButtonIndex(),reqCoursesContainer);
        }
        addText2 ->setText(minorString);
        addText3 ->setText(minorName);

        requiredC ="";
        
        for(int i = 0; i< minorM.getReqCourseSize(); i++){
            requiredC = requiredC + " " +  minorM.getReqCourseIndex(i);
        }

        addText4 ->setText(requiredC);
        coursesContainer -> setHidden(false);
        moduleContainer -> setHidden(true);
        currentRequiredMark = 60;
        autoCourseTakenButton(reqCoursesContainer);
        std::string tempTEXT = courseTake__ + std::to_string(60)+ "%.";

        addText -> setText(tempTEXT);
    });
    // makes not taken courses to their default colors and keeps the graded courses as is, back button for module container
    back_->clicked().connect([=]() {
        coursesContainer -> setHidden(true);
        moduleContainer -> setHidden(false);
        coursesContainer -> setHidden(true);
        for(int i = 0; i<module.getSize();i++){
            defaultButton((module.getClass(i)).getButtonIndex(),reqCoursesContainer);
        }
        autoCourseTakenButton(reqCoursesContainer);


        std::string tempTEXT = courseTake__ +  "%.";

        addText -> setText(tempTEXT);
    });

    





    //Year Button Code Ends


    

    //REQUIRED COURSES CONTAINER
    /*
    *
    *
    * 
    *
    *
    * 
    * 
    * 
    * 
    * 
    * 
    */
    //Grades Container 
    Wt::WContainerWidget *gradeContainer = mainContainer->addNew<Wt::WContainerWidget>();

    gradeContainer -> setStyleClass("gradeContainer");
    gradeContainer -> setAttributeValue("style", "position: fixed; top: 50%; left: 50%; transform: translate(-50%, -50%);");

    gradeContainer -> setHidden(true);

    Wt::WPushButton *backGrade = gradeContainer->addNew<Wt::WPushButton>("Back");
    backGrade -> setStyleClass("courseYearButton");
    backGrade ->setAttributeValue("style", "position:fixed; top:0%; left:80%; height:10%; width: 20%; font-size: 10px; color: white;");
    backGrade->clicked().connect([=]() {
        gradeContainer -> setHidden(true);
    });

    gradesButton->clicked().connect([=]() {

        gradeContainer -> setHidden(false);
    });


    removed = 00;

    avergaeText = gradeContainer->addNew<Wt::WText>("Average : 0");

    avergaeText ->setStyleClass("averageText");
    table = gradeContainer -> addNew<Wt::WTable>();
    table->setHeaderCount(1);
    table->setWidth(Wt::WLength("100%"));

    table->elementAt(0, 0)->addNew<Wt::WText>("#");
    table->elementAt(0, 1)->addNew<Wt::WText>("Course");
    table->elementAt(0, 2)->addNew<Wt::WText>("Weight");
    table->elementAt(0, 3)->addNew<Wt::WText>("Grade");

    gradeCounter = 1;
    int totalcount ;


    

    avergaeText->setText("Average: " + std::to_string(0));

    // adds a new grade if the add grade button of the course is clicked and updates average
    addGrade->clicked().connect([=]()mutable {

        int num = std::stoi(gradeInput ->text());
        if(num>=currentRequiredMark && 100>=num){
            table->elementAt(gradeCounter, 0)
                ->addNew<Wt::WText>(Wt::WString("{1}").arg("-"));
            table->elementAt(gradeCounter, 1)
                ->addNew<Wt::WText>(courseName ->text());
            table->elementAt(gradeCounter, 2)
                ->addNew<Wt::WText>("0.5");
            table->elementAt(gradeCounter, 3)
                ->addNew<Wt::WText>(gradeInput -> text());
    
            std::string str = (avergaeText->text()).toUTF8();
            str = str.substr(str.length() - 2);
            int num_ = std::stoi(str);
            totalcount = (num_)*(gradeCounter-removed) +num ;
            
            average = totalcount /(gradeCounter-removed) ; 
            
            // Update the text of the WText widget
            avergaeText->setText("Average: " + std::to_string(average));
            gradeCounter = gradeCounter +1;
            

            std::string changeGrade_ = (courseName ->text()).toUTF8();
        
            Course temp = module.getClassByName(changeGrade_);
            
            temp.setGraded();
            currentUser -> addNewGrade(changeGrade_,num);
            module.setClassByIndex(temp,module.getClassIndex(changeGrade_));
            coursesDescription -> setHidden(true);
            courseTakenButton((module.getClassByName(changeGrade_)).getButtonIndex(),reqCoursesContainer);

            if(currentUser -> getAdmin() == 0){addGrade1(currentUser -> getUsername(),num,changeGrade_);}
            std::cout<<currentUser -> getUsername()<<std::endl;
        }



    });

    // removes a grade if the remove grade button is clicked
    removeGrade -> clicked().connect([=]()mutable {
        
        for (int i = 0 ; i< gradeCounter; i++){

            Wt::WText* gradedClassName = dynamic_cast<Wt::WText*>((table->elementAt(i, 1))->widget(0));
            std::string changeGrade_ = (gradedClassName ->text()).toUTF8();
            if((courseName ->text())== changeGrade_){
                Wt::WTableRow *row_ = table->rowAt(i);
                row_->hide();
                gradedClassName->setText("");
                gradedClassName = dynamic_cast<Wt::WText*>((table->elementAt(i, 3))->widget(0));
                
                changeGrade_ = (gradedClassName ->text()).toUTF8();

                std::string str = (avergaeText->text()).toUTF8();
                str = str.substr(str.length() - 2);
                ++removed;
                //std::cout<<removed<<std::endl;
                
                average = ((std::stoi(str)*(gradeCounter-removed))-std::stoi(changeGrade_))/(gradeCounter-removed);
                totalcount = totalcount-std::stoi(changeGrade_);
                if(average <10){

                    avergaeText->setText("Average: " +std::to_string(0)+ std::to_string(average));
                    
                }
                else{
                    avergaeText->setText("Average: " + std::to_string(average));
                }
                currentUser -> deleteGrade(changeGrade_);
                std::cout<< currentUser -> returnGradeSize()<<std::endl;
                std::cout<< (currentUser -> returnGradeByIndex(0)).getClassName()<<std::endl;
                if(currentUser -> getAdmin() == 0){
                    removeGrade1("user",changeGrade_);
                }

            }
        }
        //gradeCounter = gradeCounter -1;
        std::string changeGrade_ = (courseName ->text()).toUTF8();
    
        Course temp = module.getClassByName(changeGrade_);
        
        temp.setUngraded();
        module.setClassByIndex(temp,module.getClassIndex(changeGrade_));
        coursesDescription -> setHidden(true);
        defaultButton((module.getClassByName(changeGrade_)).getButtonIndex(),reqCoursesContainer);


    });

    //gradeContainer->addWidget(std::move(table));

    


    //Grades Container 
    /*
    *
    *
    *
    *
    * 
    * 
    * 
    * 
    * 
    * 
    * 
    * 
    */

    // Nav Bar

    // log out button
    Wt::WContainerWidget *navContainer = mainContainer->addNew<Wt::WContainerWidget>();
    navContainer -> setStyleClass("navContainer");

    Wt::WContainerWidget *navSep = navContainer->addNew<Wt::WContainerWidget>();
    navSep -> setStyleClass("navSep");

    Wt::WPushButton *logout = navContainer->addNew<Wt::WPushButton>("Logout");
    logout ->setStyleClass("logout");
    // logoout functions
    logout -> setAttributeValue("style", "position:fixed; top:0%; left:0%; height:100%; width: 15%; font-size: 10px; color: white; background-color: black");
    logout->clicked().connect([=] {
           
              mainContainer->setHidden(true);
              container ->setHidden(false);

              
              Wt::WTable* temptable = new Wt::WTable();
              temptable->setHeaderCount(1);
              temptable->setWidth(Wt::WLength("100%"));

              temptable->elementAt(0, 0)->addNew<Wt::WText>("#");
              temptable->elementAt(0, 1)->addNew<Wt::WText>("Course");
              temptable->elementAt(0, 2)->addNew<Wt::WText>("Weight");
              temptable->elementAt(0, 3)->addNew<Wt::WText>("Grade");
              table = temptable;
            
        });
    
    // Nav Bar
    /*
    *
    *
    * 
    * 
    * 
    *
    * 
    */
    // admin buttons, doesn't show admin button if the user is not an admin
    adminPageContainer = navContainer->addNew<Wt::WContainerWidget>(); 
    adminPageContainer->setStyleClass("adminContainer"); 
    Wt::WText *warningMessage = adminPageContainer->addNew<Wt::WText>("WARNING! ADMINISTRATOR MODE: ANY CHANGES MADE HERE WILL BE APPLIED TO ALL USERS\t");
    warningMessage->setStyleClass("adminText");
    adminPageContainer->addNew<Wt::WBreak>(); 
    Wt::WPushButton *addClassAdmin = adminPageContainer->addNew<Wt::WPushButton>("Add Class");
    addClassAdmin ->setStyleClass("adminButton");
    Wt::WPushButton *removeClassAdmin = adminPageContainer->addNew<Wt::WPushButton>("Remove Class");
    removeClassAdmin ->setStyleClass("adminButton"); 
    adminPageContainer->setHidden(true);
    //help Container

    

        Wt::WPushButton *help = moduleContainer->addNew<Wt::WPushButton>("Help");
        help -> setStyleClass("helpButton");
        Wt::WContainerWidget *helpContainer = mainContainer->addNew<Wt::WContainerWidget>();
        Wt::WText *helpText = helpContainer->addNew<Wt::WText>("HELP: Welcome to our web application, which was created to help Western University students with their academic planning. Here's how to easily manage your academic journey: Begin by choosing your module: Select the module in which you are enrolled, and we will display all relevant courses as interactive buttons connected by webs to show their links. Customize Your Course Path: Click on any course to see more information. You can enter your completed or expected grade for each subject. Track Your Progress: Courses with a grade of 50 or higher can be recognized as 'Passed,' allowing you to keep track of your achievements and plan forward successfully. Our application strives to make academic preparation easier for you, resulting in a more structured and effective educational experience at Western University.");
        helpContainer -> setStyleClass("helpContainer");
        helpContainer -> addNew <Wt::WBreak>();

        Wt::WPushButton *helpClose = helpContainer->addNew<Wt::WPushButton>("Close");

        helpContainer-> setHidden(true);


        help->clicked().connect([=]() {
            helpContainer-> setHidden(false);
            
        });

        helpClose->clicked().connect([=]() {
            helpContainer-> setHidden(true);
        
        });

    // help container ends


    // login section
    
    loginButton->clicked().connect([=] {
        std::string email = userNameEdit->text().toUTF8();
        Wt::WString password = passwordEdit->text();
        if (getPassword(email) == password) { 
            currentUser -> setName(userNameEdit->text().toUTF8());
            currentUser -> setPassword(passwordEdit->text().toUTF8());

            userNameEdit->setText("");
            passwordEdit->setText("");
            mainContainer->setHidden(false);
            container ->setHidden(true);

            std::vector<int> tempGrades_;
            std::vector<std::string> tempCourses_;
            std::cout<<currentUser -> getUsername()<<std::endl;


            getGradesAndCourses(email,tempGrades_,tempCourses_);

            std::cout<<tempGrades_.size()<<std::endl;

            for (size_t i = 0; i < tempGrades_.size(); ++i) {

            currentUser -> addNewGrade(tempCourses_[i],tempGrades_[i]);    
            }
            autoCourseTakenButton(reqCoursesContainer);
            setGradedCourses();
            adminPageContainer->setHidden(true);

        } 
        // if the log in content is the admin, set the user to admin and show admin buttons
        if ( email == "MAINADMIN" &&"$$1212" == password) {
            currentUser = admin_;

            userNameEdit->setText("");
            passwordEdit->setText("");
            mainContainer->setHidden(false);
            container ->setHidden(true);
            adminPageContainer->setHidden(false);
            
            for(int i = 0; i<module.getSize();i++){
                defaultButton((module.getClass(i)).getButtonIndex(),reqCoursesContainer);
            }
        }
    });


    // login section ends

}

void DegreePlanner::greet()
{
}