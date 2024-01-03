#ifndef DEGREEPLANNER_H
#define DEGREEPLANNER_H

#include <Wt/WApplication.h>
#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>

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

#include <cmath>
#include <math.h> 
#include "User.h"
#include "Grades.h"



class DegreePlanner : public Wt::WApplication
{
public:
  DegreePlanner(const Wt::WEnvironment& env);  // constructs the degree planner object
  void buttonClickHandler(int x, Wt::WContainerWidget* con);  // a button click handler for courses, given their container, described more in the degreeplanner.cpp file
  void login(Wt::WContainerWidget * con);  // a login method 
  void defaultButton(int x, Wt::WContainerWidget* con);  // returns a button to their default color 
  void courseTakenButton(int x, Wt::WContainerWidget* con);   //updates the display of the description if the course is taken
  void autoCourseTakenButton(Wt::WContainerWidget* con);      // if the course is taken, make it brown 

  void setGradedCourses();
  //void logout(Wt::WContainerWidget * con);


private:
  Wt::WLineEdit *nameEdit_;
  Wt::WText     *greeting_;
  User * currentUser;
  Wt::WContainerWidget* mainContainer; // main container

  void greet();
};

#endif // DEGREEPLANNER_H