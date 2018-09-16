#ifndef SEMESTER_DRIVER_H
#define SEMESTER_DRIVER_H
#include "Semester.h"
#include <fstream>
#include <string>
#include <stdexcept>
using namespace std;

class SemesterDriver {
	private:
	Semester** mSemesters;
	int mNumberOfSemesters;
	bool mDataChanged;
	static const string mFileName;

	void writeDataChangesToFile();

	public:
	SemesterDriver();
	~SemesterDriver();

	void addSemester(string season, string year, string fileName) throw (invalid_argument);
	void editSemester(string fullSemesterName, string newSeason, string newYear, string newFileName) throw (runtime_error, invalid_argument);
	void deleteSemester(string season, string year) throw (runtime_error, invalid_argument);

	CourseDriver* getCourseDriver(int courseIndex) throw (invalid_argument);

	int getSemesterIndex(string fullSemesterName) throw (invalid_argument);

	Semester* getSemester(int index) const;
};
#endif
