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
 
	void addCourse();
	void addGrade();
	void addGradeCategory();
	void addSemester();

	void changeCourses();
	void changeGrades();
	void changeSemesters();

	void deleteCourse();
	void deleteGrade();
	void deleteGradeCategory();
	void deleteSemester();

	void editCourse();
	void editGrade();
	void editGradeCategory();
	void editSemester();

	int getMenuInput(int upperBound, string persistentMessage);

	string generateFileName(string season, string year, string departmentCode, string courseNumber, int fileType);

	int getValidSemesterIndex();
	int getValidCourseIndex(int semesterIndex);

	int getValidInt(string inputMessage, string failureMessage);
	string getValidString(string inputMessage, string failureMessage, bool allowSpaces, bool allowNumbers);

	bool validString(string input, bool allowSpaces, bool allowNumbers);

	template <typename number>
	number getValidNumber(string inputMessage, string failureMessage);
	public:
	UserInterface();
	~UserInterface();

	void run();
};
#endif
