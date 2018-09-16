#include "Course.h"

Course::Course(string departmentCode, string courseNumber, string finalGrade, int creditHours, string gradesFileName) {
	mDepartmentCode = departmentCode;
	mCourseNumber = courseNumber;
	mGradesFileName = gradesFileName;
	mFinalGrade = finalGrade;
	mCreditHours = creditHours;
	mGradesDriver = new GradesDriver(gradesFileName);
}

Course::Course() {
	mDepartmentCode = "";
	mCourseNumber = "";
	mFinalGrade = "-1";
	mGradesFileName = "";
	mCreditHours = 0;
	mGradesDriver = nullptr;
}

Course& Course::operator= (const Course& original) {
	delete mGradesDriver;

	mCourseNumber = original.mCourseNumber;
	mDepartmentCode = original.mDepartmentCode;
	mGradesFileName = original.mGradesFileName;
	mCreditHours = original.mCreditHours;
	mFinalGrade = original.mFinalGrade;
	mGradesDriver = original.mGradesDriver;

	return(*this);
}

Course::~Course() {
	delete mGradesDriver;
}

string Course::getCourseNumber() const {
	return (mCourseNumber);
}

string Course::getDepartmentCode() const {
	return (mDepartmentCode);
}

string Course::getFinalGrade() const {
	return (mFinalGrade);
}

string Course::getFullCourseName() const {
	return (mDepartmentCode + " " + mCourseNumber);
}

GradesDriver* Course::getGradesDriver() {
	return (mGradesDriver);
}

string Course::getGradesFileName() const {
	return (mGradesFileName);
}

bool Course::getValidCourse() const {
	return (mGradesDriver->getTotalCategoryWeight() == 100);
}

int Course::getCreditHours() const {
	return (mCreditHours);
}

void Course::setCourseNumber(string courseNumber) {
	mCourseNumber = courseNumber;
}

void Course::setDepartmentCode(string departmentCode) {
	mDepartmentCode = departmentCode;
}

void Course::setGradesDriver(string fileName) {
	delete mGradesDriver;
	mGradesDriver = new GradesDriver(fileName);
}
