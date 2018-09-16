#include "CourseDriver.h"

CourseDriver::CourseDriver (string fileName) {
	ifstream inFile;
	inFile.open(fileName);

	if(inFile.is_open()) {
		inFile >> mNumberOfCourses;

		if (mNumberOfCourses > 0) {
			mCourses = new Course*[mNumberOfCourses];
			for (int i = 0; i < mNumberOfCourses; i++) {
				string departmentCode;
				string courseNumber;
				string gradesFileName;
				string finalGrade;
				int creditHours;

				inFile >> departmentCode;
				inFile >> courseNumber;
				inFile >> finalGrade;
				inFile >> creditHours;
				inFile >> gradesFileName;

				mCourses[i] = new Course(departmentCode, courseNumber, finalGrade, creditHours, gradesFileName);
			}
		} else {
			mCourses = nullptr;
		}

		inFile.close();
	} else {
		mNumberOfCourses = 0;
		mCourses = nullptr;

		ofstream file;
		file.open(fileName, ofstream::out | ofstream::trunc);
		file << 0;
		file.close();
	}

	mFileName = fileName;
	mDataChanged = false;
}

CourseDriver& CourseDriver::operator= (const CourseDriver& original) {
	for (int i = 0; i < mNumberOfCourses; i++) {
		delete mCourses[i];
	}
	delete[] mCourses;

	mCourses = original.mCourses;
	mNumberOfCourses = original.mNumberOfCourses;
	mDataChanged = original.mDataChanged;
	mFileName = original.mFileName;

	return (*this);
}

CourseDriver::~CourseDriver() {
	if (mDataChanged) {
		writeDataChangesToFile();
	}
	for (int i = 0; i < mNumberOfCourses; i++) {
		delete mCourses[i];
	}
	delete[] mCourses;
}

void CourseDriver::addCourse(string departmentName, string courseNumber, int creditHours, string gradeFileName) throw (invalid_argument) {
	string fullCourseName = departmentName + " " + courseNumber;
	bool uniqueCourseName = false;

	try {
		getCourseIndex(fullCourseName);
	} catch (invalid_argument& e) {
		uniqueCourseName = true;
	}

	if(!uniqueCourseName) {
		throw invalid_argument("Given course name is not unique to this semester.");
	}

	Course** newCourses = new Course*[mNumberOfCourses + 1];
	for (int i = 0; i < mNumberOfCourses; i++) {
		newCourses[i] = mCourses[i];
	}

	ofstream file;
	file.open(gradeFileName);
	file << 0;
	file.close();

	newCourses[mNumberOfCourses] = new Course(departmentName, courseNumber, "-1", creditHours, gradeFileName);
	delete[] mCourses;

	mCourses = newCourses;
	mNumberOfCourses++;
	mDataChanged = true;
}

void CourseDriver::deleteCourse(string fullCourseName) throw (runtime_error, invalid_argument) {
	int courseIndex = -1;
	try {
		courseIndex = getCourseIndex(fullCourseName);
	} catch (invalid_argument& e) {
		throw invalid_argument("Course does not exist.");
	}

	string gradesFileName = mCourses[courseIndex]->getGradesFileName();

	int deleteSuccessful = remove(gradesFileName.c_str());

	if (deleteSuccessful != 0) {
		throw runtime_error ("Grades file could not be deleted");
	}

	Course** newCourses = nullptr;

	if (mNumberOfCourses > 1) {
		newCourses = new Course*[mNumberOfCourses - 1];
		int index = 0;
		for (int i = 0; i < mNumberOfCourses; i++) {
			if(i != courseIndex) {
				newCourses[index] = mCourses[i];
				index++;
			}
		}
	}

	delete mCourses[courseIndex];
	delete[] mCourses;

	mCourses = newCourses;
	mNumberOfCourses--;
	mDataChanged = true;
}

void CourseDriver::deleteAllCourses() {
	for (int i = 0; i < mNumberOfCourses; i++) {
		string courseName = mCourses[i]->getFullCourseName();
		deleteCourse(courseName);
	}
	delete[] mCourses;
	mCourses = nullptr;
	mNumberOfCourses = 0;
}

void CourseDriver::editCourse(string oldFullCourseName, string departmentName, string courseNumber, string finalGrade, int creditHours, string newGradesFileName) throw (runtime_error, invalid_argument) {
	int courseIndex = -1;
	try {
		courseIndex = getCourseIndex(oldFullCourseName);
	} catch (invalid_argument& e) {
		throw invalid_argument("Course does not exist.");
	}

	string oldGradesFileName = mCourses[courseIndex]->getGradesFileName();

	// Delete this here so that any data changes are written to file before it is renamed
	delete mCourses[courseIndex];

	if (oldGradesFileName != newGradesFileName) {
		int renameSuccessful = rename(oldGradesFileName.c_str(), newGradesFileName.c_str());

		if (renameSuccessful != 0) {
			throw runtime_error("File could not be renamed.");
		}
	}


	mCourses[courseIndex] = new Course(departmentName, courseNumber, finalGrade, creditHours, newGradesFileName);
	mDataChanged = true;
}

GradesDriver* CourseDriver::getCourseGradesDriver(int courseIndex) throw (invalid_argument) {
	return (mCourses[courseIndex]->getGradesDriver());
}

string CourseDriver::getFileName() const {
	return (mFileName);
}

int CourseDriver::getCourseIndex(string fullCourseName) throw (invalid_argument) {
	int courseIndex = -1;
	for (int i = 0; i < mNumberOfCourses; i++) {
		if (fullCourseName == mCourses[i]->getFullCourseName()) {
			courseIndex = i;
			break;
		}
	}

	if(courseIndex == -1) {
		throw invalid_argument("Course does not exist.");
	}

	return (courseIndex);
}

void CourseDriver::writeDataChangesToFile() {
	fstream file;

	file.open(mFileName, ofstream::out | ofstream::trunc);

	file << mNumberOfCourses << endl;

	for (int i = 0; i < mNumberOfCourses; i++) {
		file << mCourses[i]->getFullCourseName() << " ";
		file << mCourses[i]->getFinalGrade() << " ";
		file << mCourses[i]->getCreditHours() << " ";
		file << mCourses[i]->getGradesFileName() << endl;
	}

	file.close();
}
