#include "Semester.h"

Semester::Semester(string season, string year, int gradePointAverage, string courseDriverFileName) {
	mSeason = season;
	mYear = year;
	mGradePointAverage = gradePointAverage;
	mCourseDriver = new CourseDriver(courseDriverFileName);
}

Semester& Semester::operator=(const Semester& original) {
	delete mCourseDriver;

	mCourseDriver = original.mCourseDriver;
	mSeason = original.mSeason;
	mYear = original.mYear;
	mGradePointAverage = original.mGradePointAverage;

	return (*this);
}

Semester::Semester() {
	mSeason = "";
	mYear = "";
	mGradePointAverage = 0;
	mCourseDriver = nullptr;
}

Semester::~Semester() {
	delete mCourseDriver;
}

string Semester::getSeason() const {
	return (mSeason);
}

string Semester::getYear() const {
	return (mYear);
}

CourseDriver* Semester::getCourseDriver() {
	return (mCourseDriver);
}

int Semester::getGradePointAverage() const {
	return (mGradePointAverage*1000);
}

string Semester::getFullSemesterName() const {
	string fullSemesterName = mSeason + " " + mYear;
	return (fullSemesterName);
}

void Semester::setSeason(string season) {
	mSeason = season;
}

void Semester::setYear(string year) {
	mYear = year;
}
