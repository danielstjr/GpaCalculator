#ifndef COURSE_DRIVER_H
#define COURSE_DRIVER_H
#include "GradesDriver.h"
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
	int mNumberOfCourses;
	bool mDataChanged;
	string mFileName;

	void writeDataChangesToFile();

	public:
	CourseDriver(string fileName);
	CourseDriver& operator= (const CourseDriver& original);
	~CourseDriver();

	string getFileName() const;

	void addCourse(string departmentName, string courseNumber, int creditHours, string gradeFileName) throw (invalid_argument);
	void editCourse(string oldFullCourseName, string departmentName, string courseNumber, string finalGrade, int creditHours, string newGradesFileName) throw (runtime_error, invalid_argument);
	void deleteCourse(string fullCourseName) throw (runtime_error, invalid_argument);
	void deleteAllCourses();

	GradesDriver* getCourseGradesDriver(int courseIndex) throw (invalid_argument);
	string getFullCourseName(int courseIndex);

	int getCourseIndex(string fullCourseName) throw (invalid_argument);
};
#endif
