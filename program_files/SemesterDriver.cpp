#include "SemesterDriver.h"

const string SemesterDriver::mFileName = "data/semesters.data";

SemesterDriver::SemesterDriver() {
	ifstream inFile;
	inFile.open(mFileName);

	inFile >> mNumberOfSemesters;

	if (mNumberOfSemesters > 0) {
		mSemesters = new Semester*[mNumberOfSemesters];

		for (int i = 0; i < mNumberOfSemesters; i++) {
			string season;
			string year;
			int gradePointAverage;
			string courseDriverFileName;

			inFile >> season;
			inFile >> year;
			inFile >> gradePointAverage;
			inFile >> courseDriverFileName;

			mSemesters[i] = new Semester(season, year, gradePointAverage, courseDriverFileName);
		}
	} else {
		mSemesters = nullptr;
	}

	mDataChanged = false;
}

SemesterDriver::~SemesterDriver() {
	if (mDataChanged) {
		writeDataChangesToFile();
	}
	for (int i = 0; i < mNumberOfSemesters; i++) {
		delete mSemesters[i];
	}
	delete[] mSemesters;
}

void SemesterDriver::addSemester(string season, string year, string fileName) throw (invalid_argument) {
	bool uniqueSemesterName = false;
	try {
		getSemesterIndex((season + " " + year));
	} catch (invalid_argument& e) {
		uniqueSemesterName = true;
	}

	if (!uniqueSemesterName) {
		throw invalid_argument("Semester name already exists.");
	}

	Semester** newSemesters = new Semester*[mNumberOfSemesters + 1];

	for (int i = 0; i < mNumberOfSemesters; i++) {
		newSemesters[i] = mSemesters[i];
	}

	newSemesters[mNumberOfSemesters] = new Semester(season, year, 0, fileName);

	delete[] mSemesters;
	mSemesters = newSemesters;
	mNumberOfSemesters++;
	mDataChanged = true;
}

void SemesterDriver::editSemester(string fullSemesterName, string newSeason, string newYear, string newFileName) throw (runtime_error, invalid_argument) {
	int semesterIndex = -1;
	try {
		semesterIndex = getSemesterIndex(fullSemesterName);
	} catch (invalid_argument& e) {
		throw invalid_argument(e.what());
	}

	bool uniqueSemesterName = true;
	string newSemesterFullName = newSeason + " " + newYear;
	for (int a = 0; a < mNumberOfSemesters; a++) {
		if (a != semesterIndex && mSemesters[a]->getFullSemesterName() == newSemesterFullName) {
			uniqueSemesterName = false;
		}
	}

	if (!uniqueSemesterName) {
		throw invalid_argument("Cannot edit semester name to be non unique.");
	}

	double oldGradePointAverage = mSemesters[semesterIndex]->getGradePointAverage();
	string oldSemesterFullName = mSemesters[semesterIndex]->getFullSemesterName();
	string oldFileName = mSemesters[semesterIndex]->getCourseDriver()->getFileName();

	delete mSemesters[semesterIndex];

	if (newSemesterFullName != oldSemesterFullName) {
		int successfulRename = rename(oldFileName.c_str(), newFileName.c_str());
		if(successfulRename != 0) {
			throw runtime_error("File could not be renamed.");
		}
	}

	mSemesters[semesterIndex] = new Semester (newSeason, newYear, oldGradePointAverage, newFileName);

	mDataChanged = true;
}

void SemesterDriver::deleteSemester(string season, string year) throw (runtime_error, invalid_argument) {
	int semesterIndex = -1;
	string fullSemesterName = season + " " + year;
	try {
		semesterIndex = getSemesterIndex(fullSemesterName);
	} catch (invalid_argument& e) {
		throw invalid_argument(e.what());
	}

	Semester** newSemesters = nullptr;
	string fileName = mSemesters[semesterIndex]->getCourseDriver()->getFileName();

	int successfulDeletion = remove(fileName.c_str());

	if (successfulDeletion != 0) {
		throw runtime_error("File could not be deleted.");
	}

	mSemesters[semesterIndex]->getCourseDriver()->deleteAllCourses();

	if (mNumberOfSemesters > 1) {
		newSemesters = new Semester*[mNumberOfSemesters - 1];
		int index = 0;
		for (int i = 0; i < mNumberOfSemesters; i++) {
			if (i != semesterIndex) {
				newSemesters[index] = mSemesters[i];
				index++;
			}
		}
	}

	delete mSemesters[semesterIndex];
	delete[] mSemesters;

	mSemesters = newSemesters;
	mNumberOfSemesters--;
	mDataChanged = true;
}

CourseDriver* SemesterDriver::getCourseDriver(int semesterIndex) throw (invalid_argument) {
	return (mSemesters[semesterIndex]->getCourseDriver());
}

int SemesterDriver::getSemesterIndex(string fullSemesterName) throw (invalid_argument) {
	int semesterIndex = -1;
	for (int i = 0; i < mNumberOfSemesters; i++) {
		if (fullSemesterName == mSemesters[i]->getFullSemesterName()) {
			semesterIndex = i;
			break;
		}
	}
	if (semesterIndex == -1) {
		throw invalid_argument("Semester does not exist");
	}
	return (semesterIndex);
}

Semester* SemesterDriver::getSemester(int index) const {
	return (mSemesters[index]);
}

void SemesterDriver::writeDataChangesToFile() {
	fstream file;

	file.open(mFileName, ofstream::out | ofstream::trunc);
	file << mNumberOfSemesters << endl;

	for (int i = 0; i < mNumberOfSemesters; i++) {
		file << mSemesters[i]->getSeason() << " ";
		file << mSemesters[i]->getYear() << " ";
		file << mSemesters[i]->getGradePointAverage() << " ";
		file << mSemesters[i]->getCourseDriver()->getFileName() << endl;
	}

	file.close();
}
