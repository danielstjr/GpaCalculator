#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H
#include "SemesterDriver.h"
#include "CourseDriver.h"
#include "GradesDriver.h"
#include <stdexcept>
#include <string>
#include <sstream>
using namespace std;

class UserInterface {
	private:
	SemesterDriver* mSemesterDriver;

	void changeDataMenu();

	void addCourse(int semesterIndex);
	void addGrade(int semesterIndex, int courseIndex);
	void addGradeCategory(int semesterIndex, int courseIndex);
	void addSemester();

	void changeCourses();
	void changeGrades();
	void changeSemesters();

	void deleteCourse(int semesterIndex);
	void deleteGrade(int semesterIndex, int courseIndex);
	void deleteGradeCategory(int semesterIndex, int courseIndex);
	void deleteSemester();

	void editCourse(int semesterIndex, int changeType);
	void editGrade(int semesterIndex, int courseIndex, int changeType);
	void editGradeCategory(int semesterIndex, int courseIndex);
	void editSemester();

	int getMenuInput(int upperBound, string persistentMessage);

	string generateFileName(string season, string year, string departmentCode, string courseNumber, int fileType);

	int getValidSemesterIndex() throw (logic_error, runtime_error);
	int getValidCourseIndex(int semesterIndex) throw (logic_error, runtime_error);

	string getValidString(string inputMessage, string failureMessage, bool allowSpaces, bool allowNumbers) throw (logic_error);
	bool validString(string input, bool allowSpaces, bool allowNumbers);

	void viewGrades();
	void viewCourseInfo();
	void viewSemesterInfo();

	template <typename number>
	number getValidNumber(string inputMessage, string failureMessage) throw (logic_error);
	public:
	UserInterface();
	~UserInterface();

	void run();
};
#endif
