#include "CourseDriver.h"

CourseDriver::CourseDriver (string fileName) {
	ifstream inFile;
	inFile.open(fileName);

	if(inFile.is_open()) {
		inFile >> mCourseCount;

		if (mCourseCount > 0) {
			mCourses = new Course*[mCourseCount];
			for (int i = 0; i < mCourseCount; i++) {
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
		ofstream file;
		file.open(fileName, ofstream::out | ofstream::trunc);
		file << 0;
		file.close();

		mCourseCount = 0;
		mCourses = nullptr;
	}

	mFileName = fileName;
	mDataChanged = false;
}

CourseDriver::~CourseDriver() {
	if (mDataChanged) {
		writeDataChangesToFile();
	}
	for (int i = 0; i < mCourseCount; i++) {
		delete mCourses[i];
	}
	delete[] mCourses;
}

void CourseDriver::addCourse(string departmentCode, string courseNumber, int creditHours, string gradeFileName) throw (invalid_argument) {
	string fullCourseName = departmentCode + " " + courseNumber;
	bool uniqueCourseName = false;

	try {
		getCourseIndex(fullCourseName);
	} catch (...) {
		uniqueCourseName = true;
	}

	if(!uniqueCourseName) {
		throw invalid_argument("Given course name is not unique to this semester.");
	}

	Course** newCourses = new Course*[mCourseCount + 1];
	for (int i = 0; i < mCourseCount; i++) {
		newCourses[i] = mCourses[i];
	}

	ofstream file;
	file.open(gradeFileName);
	file << 0;
	file.close();

	newCourses[mCourseCount] = new Course(departmentCode, courseNumber, "-1", creditHours, gradeFileName);
	delete[] mCourses;

	mCourses = newCourses;
	mCourseCount++;
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

	if (mCourseCount > 1) {
		newCourses = new Course*[mCourseCount - 1];
		int index = 0;
		for (int i = 0; i < mCourseCount; i++) {
			if(i != courseIndex) {
				newCourses[index] = mCourses[i];
				index++;
			}
		}
	}

	delete mCourses[courseIndex];
	delete[] mCourses;

	mCourses = newCourses;
	mCourseCount--;
	mDataChanged = true;
}

void CourseDriver::deleteAllCourses() {
	for (int i = 0; i < mCourseCount; i++) {
		string courseName = mCourses[i]->getFullCourseName();
		deleteCourse(courseName);
	}
	delete[] mCourses;
	mCourses = nullptr;
	mCourseCount = 0;
}

void CourseDriver::editDepartmentCode(string oldFullCourseName, string departmentCode, string newGradesFileName) throw (runtime_error, invalid_argument) {
	int courseIndex = -1;
	try {
		courseIndex = getCourseIndex(oldFullCourseName);
	} catch (invalid_argument& e) {
		throw invalid_argument("Course does not exist.");
	}

	string oldGradesFileName = mCourses[courseIndex]->getGradesFileName();
	string courseNumber = mCourses[courseIndex]->getCourseNumber();
	string finalGrade = mCourses[courseIndex]->getFinalGrade();
	int creditHours = mCourses[courseIndex]->getCreditHours();

	// Delete this here so that any data changes are written to file before it is renamed
	delete mCourses[courseIndex];

	if (oldGradesFileName != newGradesFileName) {
		int renameSuccessful = rename(oldGradesFileName.c_str(), newGradesFileName.c_str());

		if (renameSuccessful != 0) {
			throw runtime_error("File could not be renamed.");
		}
	}

	mCourses[courseIndex] = new Course(departmentCode, courseNumber, finalGrade, creditHours, newGradesFileName);
	mDataChanged = true;
}

void CourseDriver::editCourseNumber(string oldFullCourseName, string courseNumber, string newGradesFileName) throw (runtime_error, invalid_argument) {
	int courseIndex = -1;
	try {
		courseIndex = getCourseIndex(oldFullCourseName);
	} catch (invalid_argument& e) {
		throw invalid_argument("Course does not exist.");
	}

	string oldGradesFileName = mCourses[courseIndex]->getGradesFileName();
	string departmentCode = mCourses[courseIndex]->getDepartmentCode();
	string finalGrade = mCourses[courseIndex]->getFinalGrade();
	int creditHours = mCourses[courseIndex]->getCreditHours();

	// Delete this here so that any data changes are written to file before it is renamed
	delete mCourses[courseIndex];

	if (oldGradesFileName != newGradesFileName) {
		int renameSuccessful = rename(oldGradesFileName.c_str(), newGradesFileName.c_str());

		if (renameSuccessful != 0) {
			throw runtime_error("File could not be renamed.");
		}
	}

	mCourses[courseIndex] = new Course(departmentCode, courseNumber, finalGrade, creditHours, newGradesFileName);
	mDataChanged = true;
}

void CourseDriver::editCreditHours (string oldFullCourseName, int creditHours) throw (invalid_argument) {
	int courseIndex = -1;
	try {
		courseIndex = getCourseIndex(oldFullCourseName);
	} catch (invalid_argument& e) {
		throw invalid_argument("Course does not exist.");
	}

	string departmentCode = mCourses[courseIndex]->getDepartmentCode();
	string courseNumber = mCourses[courseIndex]->getCourseNumber();
	string finalGrade = mCourses[courseIndex]->getFinalGrade();
	string gradesFileName = mCourses[courseIndex]->getGradesFileName();

	delete mCourses[courseIndex];

	mCourses[courseIndex] = new Course(departmentCode, courseNumber, finalGrade, creditHours, gradesFileName);
	mDataChanged = true;
}

void CourseDriver::editFinalGrade(string oldFullCourseName, string finalGrade) throw (invalid_argument) {
	int courseIndex = -1;
	try {
		courseIndex = getCourseIndex(oldFullCourseName);
	} catch (invalid_argument& e) {
		throw invalid_argument("Course does not exist.");
	}

	string departmentCode = mCourses[courseIndex]->getDepartmentCode();
	string courseNumber = mCourses[courseIndex]->getCourseNumber();
	int creditHours = mCourses[courseIndex]->getCreditHours();
	string gradesFileName = mCourses[courseIndex]->getGradesFileName();

	delete mCourses[courseIndex];

	mCourses[courseIndex] = new Course(departmentCode, courseNumber, finalGrade, creditHours, gradesFileName);
	mDataChanged = true;
}

GradesDriver* CourseDriver::getCourseGradesDriver(int courseIndex) throw (invalid_argument) {
	return (mCourses[courseIndex]->getGradesDriver());
}

string CourseDriver::getFileName() const {
	return (mFileName);
}

int CourseDriver::getCourseIndex(string fullCourseName) throw (runtime_error, invalid_argument) {
	if (mCourseCount == 0) {
		throw runtime_error ("No courses are initialized yet.");
	}
	int courseIndex = -1;
	for (int i = 0; i < mCourseCount; i++) {
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

string CourseDriver::printCourse(int courseIndex) {
	string output = "";

	Grades* grades = mCourses[courseIndex]->getGradesDriver()->getGrades();
	cout << "Categories and weights: ";
	for (int i = 0; i < grades->getCategoryCount(); i++) {
		output += "\t" + grades->getCategoryName(i) + ": ";
		output += to_string(grades->getCategoryWeight(i)) + "\n";
	}

	return (output);
}

string CourseDriver::printAllCourses() {
	string output = "\n";

	for (int i = 0; i < mCourseCount; i++) {
		output += "\t" + mCourses[i]->getDepartmentCode() + " ";
		output += mCourses[i]->getCourseNumber() + "\n";
	}

	return (output);
}

string CourseDriver::getFullCourseName(int courseIndex) {
	return (mCourses[courseIndex]->getFullCourseName());
}

void CourseDriver::writeDataChangesToFile() {
	fstream file;

	file.open(mFileName, ofstream::out | ofstream::trunc);

	file << mCourseCount << endl;

	for (int i = 0; i < mCourseCount; i++) {
		file << mCourses[i]->getFullCourseName() << " ";
		file << mCourses[i]->getFinalGrade() << " ";
		file << mCourses[i]->getCreditHours() << " ";
		file << mCourses[i]->getGradesFileName() << endl;
	}

	file.close();
}
