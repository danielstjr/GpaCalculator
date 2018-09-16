#ifndef SEMESTER_H
#define SEMESTER_H
#include "CourseDriver.h"
using namespace std;

class Semester {
	private:
	CourseDriver* mCourseDriver;
	string mSeason;
	string mYear;
	double mGradePointAverage;

	public:
	Semester(string season, string year, int gradePointAverage, string courseDriverFileName);
	Semester();
	Semester& operator=(const Semester& original);
	~Semester();

	string getSeason() const;
	string getYear() const;
	string getFullSemesterName() const;
	CourseDriver* getCourseDriver();
	int getGradePointAverage() const;

	void setSeason(string season);
	void setYear(string year);
};
#endif
