#ifndef COURSE_DRIVER_H
#define COURSE_DRIVER_H
#include "Grades.h"
#include "Course.h"
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <stdexcept>
using namespace std;

class CourseDriver {
	private:
	Course** mCourses;
	int mCourseCount;
	bool mDataChanged;
	string mFileName;

	void writeDataChangesToFile();

	public:
	CourseDriver(string fileName);
	~CourseDriver();

	string getFileName() const;

	void addCourse(string departmentCode, string courseNumber, int creditHours, string gradeFileName) throw (invalid_argument);
	void editDepartmentCode(string oldFullCourseName, string departmentCode, string newGradesFileName) throw (runtime_error, invalid_argument);
	void editCourseNumber(string oldFullCourseName, string courseNumber, string newGradesFileName) throw (runtime_error, invalid_argument);
	void editCreditHours(string oldFullCourseName, int creditHours) throw (invalid_argument);
	void editFinalGrade(string oldFullCourseName, string finalGrade) throw (invalid_argument);
	void deleteCourse(string fullCourseName) throw (runtime_error, invalid_argument);
	void deleteAllCourses();

	GradesDriver* getCourseGradesDriver(int courseIndex) throw (invalid_argument);
	string getFullCourseName(int courseIndex);

	int getCourseIndex(string fullCourseName) throw (runtime_error, invalid_argument);

	string printCourse(int courseIndex);
	string printAllCourses();
};
#endif
