#ifndef COURSE_H
#define COURSE_H
#include <string>
#include "GradesDriver.h"
using namespace std;

class Course {
	private:
	string mCourseNumber;

	string mDepartmentCode;

	GradesDriver* mGradesDriver;

	string mGradesFileName;

	string mFinalGrade;

	int mCreditHours;

	public:
	/**
	* @pre departmentCode and courseNumber are legal names with only letter/number characters
	*/
	Course(string departmentCode, string courseNumber, string finalGrade, int creditHours, string gradesFileName);
	Course();
	Course& operator= (const Course& original);
	~Course();

	string getCourseNumber() const;
	string getDepartmentCode() const;
	string getFinalGrade() const;
	string getFullCourseName() const;
	GradesDriver* getGradesDriver();
	string getGradesFileName() const;
	bool getValidCourse() const;
	int getCreditHours() const;

	void setCourseNumber(string courseNumber);
	void setDepartmentCode(string departmentCode);
	void setGradesDriver(string fileName);
	void setFinalGrade(string finalGrade);
};
#endif
