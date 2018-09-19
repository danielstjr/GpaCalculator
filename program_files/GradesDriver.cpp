#include "GradesDriver.h"

GradesDriver::GradesDriver(string fileName) {
	ifstream inFile;
	inFile.open(fileName);

	if(inFile.is_open()) {
		int categoryCount = 0;
		inFile >> categoryCount;

		if (categoryCount > 0) {
			string* categoryNames = new string[categoryCount];
			int* categoryWeights = new int[categoryCount];
			int* categoryGradeCounts = new int[categoryCount];
			double** earnedPoints = new double*[categoryCount];
			double** totalPoints = new double*[categoryCount];
			string** gradeNames = new string*[categoryCount];

			for (int i = 0; i < categoryCount; i++) {
				int categoryGradeCount;
				string serializedCategoryName;

				inFile >> serializedCategoryName;
				inFile >> categoryWeights[i];
				inFile >> categoryGradeCount;

				// Category Names are serialized to remove spaces and replace them with '_', this removes those characters and replaces them with spaces
				categoryNames[i] = deserializeString(serializedCategoryName);
				categoryGradeCounts[i] = categoryGradeCount;
				if (categoryGradeCount > 0) {
					earnedPoints[i] = new double[categoryGradeCount];
					totalPoints[i] = new double[categoryGradeCount];
					gradeNames[i] = new string[categoryGradeCount];

					for (int j = 0; j < categoryGradeCount; j++) {
						string serializedGradeName;
						double inEarnedPoints;
						double inTotalPoints;

						inFile >> serializedGradeName;
						inFile >> inEarnedPoints;
						inFile >> inTotalPoints;

						//Grade Names are serialized to remove spaces and replace them with '_', this removes those characters and replaces them with spaces
						gradeNames[i][j] = deserializeString(serializedGradeName);

						// Grades are multiplied by 100 then cast to an int to ensure 2 digits of precision and
						// to avoid weird precision errors when converting them to string and storing them,
						// so this undoes that and stores them again as a double with 2 digits of precision
						earnedPoints[i][j] = inEarnedPoints/100;
						totalPoints[i][j] = inTotalPoints/100;
					}
				} else {
					earnedPoints[i] = nullptr;
					totalPoints[i] = nullptr;
					gradeNames[i] = nullptr;
				}

			}

			mGrades = new Grades(earnedPoints, totalPoints, gradeNames, categoryWeights, categoryNames, categoryGradeCounts, categoryCount);
		} else {
			mGrades = new Grades;
		}

		inFile.close();
	} else {
		ofstream file;
		file.open(fileName, ofstream::out | ofstream::trunc);
		file << 0;
		file.close();

		mGrades = new Grades;
	}

	updateCurrentGrade();
	mFileName = fileName;
	mDataChanged = false;
}

GradesDriver::GradesDriver() {
	mGrades = nullptr;
}

GradesDriver::~GradesDriver() {
	if (mDataChanged) {
		writeDataChangesToFile();
	}
	delete mGrades;
}


void GradesDriver::addCategory(string categoryName, int categoryWeight) throw (invalid_argument) {
	bool categoryAlreadyExists = true;

	// This series of try/catch blocks verify that the given categoryName does not already exist
	try {
		getCategoryIndex(categoryName);
	} catch (runtime_error& e) {
		categoryAlreadyExists = false;
	} catch (invalid_argument& e) {
		categoryAlreadyExists = false;
	}

	if(categoryAlreadyExists) {
		throw invalid_argument("Category of the given name already exists.");
	}

	int categoryCount = mGrades->getCategoryCount();

	double** newEarnedPoints = new double*[categoryCount + 1];
	double** newTotalPoints = new double*[categoryCount + 1];
	string** newGradeNames = new string*[categoryCount + 1];
	int* newCategoryWeights = new int[categoryCount + 1];
	string* newCategoryNames = new string[categoryCount + 1];
	int* newCategoryGradeCounts = new int[categoryCount + 1];

	for (int i = 0; i < categoryCount; i++) {
		int categoryGradeCount = mGrades->getCategoryGradeCount(i);

		newCategoryWeights[i] = mGrades->getCategoryWeight(i);
		newCategoryNames[i] = mGrades->getCategoryName(i);
		newCategoryGradeCounts[i] = categoryGradeCount;
		if (categoryGradeCount > 0) {
			newEarnedPoints[i] = new double[categoryGradeCount];
			newTotalPoints[i] = new double[categoryGradeCount];
			newGradeNames[i] = new string[categoryGradeCount];
			for (int j = 0; j < categoryGradeCount; j++) {
				newEarnedPoints[i][j] = mGrades->getEarnedPoints(i,j);
				newTotalPoints[i][j] = mGrades->getTotalPoints(i,j);
				newGradeNames[i][j] = mGrades->getGradeName(i,j);
			}
		} else {
			newEarnedPoints[i] = nullptr;
			newTotalPoints[i] = nullptr;
			newGradeNames[i] = nullptr;
		}

	}

	// Creates the arrays for grades of the new category but does not initialize them with any values, as that is mixing concerns
	newEarnedPoints[categoryCount] = nullptr;
	newTotalPoints[categoryCount] = nullptr;
	newGradeNames[categoryCount] = nullptr;

	newCategoryWeights[categoryCount] = categoryWeight;
	newCategoryNames[categoryCount] = categoryName;
	newCategoryGradeCounts[categoryCount] = 0;

	mGrades->setGradeWeightData(newEarnedPoints, newTotalPoints, newGradeNames, newCategoryWeights, newCategoryNames, newCategoryGradeCounts, (categoryCount + 1));
	mDataChanged = true;
}

void GradesDriver::addGrade(string categoryName, string gradeName, double earnedPoints, double totalPoints) throw (runtime_error, invalid_argument) {
	int categoryIndex = -1;

	// This series of try/catch blocks verifies the given categoryName exists
	try {
		categoryIndex = getCategoryIndex(categoryName);
	} catch (runtime_error& e) {
		throw runtime_error(e.what());
	} catch (invalid_argument& e) {
		throw invalid_argument(e.what());
	}

	int categoryGradeCount = mGrades->getCategoryGradeCount(categoryIndex);
	bool gradeNameExists = true;
	// string gradeName, int categoryIndex, int categoryGradeCount
	try {
		getGradeIndex(gradeName, categoryIndex, categoryGradeCount);
	} catch (invalid_argument& e) {
		gradeNameExists = false;
	}

	if (gradeNameExists) {
		throw invalid_argument("Grade name already exists.");
	}

	double* newEarnedPoints = new double[categoryGradeCount + 1];
	double* newTotalPoints = new double[categoryGradeCount + 1];
	string* newGradeNames = new string[categoryGradeCount + 1];

	for (int i = 0; i < categoryGradeCount; i++) {
		newEarnedPoints[i] = mGrades->getEarnedPoints(categoryIndex, i);
		newTotalPoints[i] = mGrades->getTotalPoints(categoryIndex, i);
		newGradeNames[i] = mGrades->getGradeName(categoryIndex, i);
	}

	newEarnedPoints[categoryGradeCount] = earnedPoints;
	newTotalPoints[categoryGradeCount] = totalPoints;
	newGradeNames[categoryGradeCount] = gradeName;

	mGrades->setCategoryGrades(newEarnedPoints, newTotalPoints, newGradeNames, categoryIndex, (categoryGradeCount + 1));
	updateCurrentGrade();
	mDataChanged = true;
}

void GradesDriver::deleteCategory(string categoryName) throw (runtime_error, invalid_argument) {
	int categoryIndex = -1;

	// This series of try/catch blocks verifies the given categoryName exists
	try {
		categoryIndex = getCategoryIndex(categoryName);
	} catch (runtime_error& e) {
		throw runtime_error(e.what());
	} catch (invalid_argument& e) {
		throw invalid_argument(e.what());
	}

	int categoryCount = mGrades->getCategoryCount();

	double** newEarnedPoints = nullptr;
	double** newTotalPoints = nullptr;
	string** newGradeNames = nullptr;
	int* newCategoryWeights = nullptr;
	string* newCategoryNames = nullptr;
	int* newCategoryGradeCounts = nullptr;

	if (categoryCount > 1) {
		newEarnedPoints = new double*[categoryCount - 1];
		newTotalPoints = new double*[categoryCount - 1];
		newGradeNames = new string*[categoryCount - 1];
		newCategoryWeights = new int[categoryCount - 1];
		newCategoryNames = new string[categoryCount - 1];
		newCategoryGradeCounts = new int[categoryCount - 1];

		int index = 0;
		for (int i = 0; i < categoryCount; i++) {
			if ((i != categoryIndex)) {
				int categoryGradeCount = mGrades->getCategoryGradeCount(index);

				newCategoryWeights[index] = mGrades->getCategoryWeight(index);
				newCategoryNames[index] = mGrades->getCategoryName(index);
				newCategoryGradeCounts[index] = categoryGradeCount;

				if (categoryGradeCount > 0) {
					newEarnedPoints[index] = new double[categoryGradeCount];
					newTotalPoints[index] = new double[categoryGradeCount];
					newGradeNames[index] = new string[categoryGradeCount];
					for (int j = 0; j < categoryGradeCount; j++) {
						newEarnedPoints[index][j] = mGrades->getEarnedPoints(index, j);
						newTotalPoints[index][j] = mGrades->getTotalPoints(index,j);
						newGradeNames[index][j] = mGrades->getGradeName(index,j);
					}
				} else {
					newEarnedPoints[index] = nullptr;
					newTotalPoints[index] = nullptr;
					newGradeNames[index] = nullptr;
				}
				index++;
			}
		}
	}

	mGrades->setGradeWeightData(newEarnedPoints, newTotalPoints, newGradeNames, newCategoryWeights, newCategoryNames, newCategoryGradeCounts, (categoryCount-1));

	mDataChanged = true;
}

void GradesDriver::deleteGrade(string categoryName, string gradeName) throw (runtime_error, invalid_argument) {
	int categoryIndex = -1;

	// This series of try/catch blocks verifies the given categoryName exists
	try {
		categoryIndex = getCategoryIndex(categoryName);
	} catch (runtime_error& e) {
		throw runtime_error(e.what());
	} catch (invalid_argument& e) {
		throw invalid_argument(e.what());
	}

	int categoryGradeCount = mGrades->getCategoryGradeCount(categoryIndex);

	if (categoryGradeCount > 1) {
		int gradeIndex = -1;

		// This try/catch block makes sure the grade of the given name and category exists
		try {
			gradeIndex = getGradeIndex(gradeName, categoryIndex, categoryGradeCount);
		} catch (invalid_argument& e) {
			throw invalid_argument(e.what());
		}

		double* newEarnedPoints = new double[categoryGradeCount - 1];
		double* newTotalPoints = new double[categoryGradeCount - 1];
		string* newGradeNames = new string[categoryGradeCount - 1];

		int index = 0;
		for (int b = 0; b < categoryGradeCount; b++) {
			if (!(b == gradeIndex)) {
				newEarnedPoints[index] = mGrades->getEarnedPoints(categoryIndex, b);
				newTotalPoints[index] = mGrades->getTotalPoints(categoryIndex, b);
				newGradeNames[index] = mGrades->getGradeName(categoryIndex, b);
				index++;
			}
		}

		mGrades->setCategoryGrades(newEarnedPoints, newTotalPoints, newGradeNames, categoryIndex, (categoryGradeCount - 1));
		mDataChanged = true;
		updateCurrentGrade();
	} else if (categoryGradeCount == 1) {
		double* newEarnedPoints = nullptr;
		double* newTotalPoints = nullptr;
		string* newGradeNames = nullptr;

		mGrades->setCategoryGrades(newEarnedPoints, newTotalPoints, newGradeNames, categoryIndex, 0);
		mDataChanged = true;
	} else {
		throw invalid_argument("Grade does not exist.");
	}
}

void GradesDriver::editCategory (string oldCategoryName, string newCategoryName, int newCategoryWeight) throw (runtime_error, invalid_argument) {
	int categoryIndex = -1;

	// This series of try/catch blocks verifies the given categoryName exists
	try {
		categoryIndex = getCategoryIndex(oldCategoryName);
	} catch (runtime_error& e) {
		throw runtime_error(e.what());
	} catch (invalid_argument& e) {
		throw invalid_argument(e.what());
	}

	int categoryCount = mGrades->getCategoryCount();

	bool uniqueCategoryName = true;
	for (int a = 0; a < categoryCount; a++) {
		if(newCategoryName == mGrades->getCategoryName(a)) {
			if (a != categoryIndex) {
				uniqueCategoryName = false;
			}
		}
	}

	if (!uniqueCategoryName) {
		throw invalid_argument("Category name already exists.");
	}

	double** newEarnedPoints = new double*[categoryCount];
	double** newTotalPoints = new double*[categoryCount];
	string** newGradeNames = new string*[categoryCount];
	int* newCategoryWeights = new int[categoryCount];
	string* newCategoryNames = new string[categoryCount];
	int* newCategoryGradeCounts = new int[categoryCount];

	for (int i = 0; i < categoryCount; i++) {
		int categoryGradeCount = mGrades->getCategoryGradeCount(i);

		if (i != categoryIndex) {
			newCategoryWeights[i] = mGrades->getCategoryWeight(i);
			newCategoryNames[i] = mGrades->getCategoryName(i);
		} else {
			newCategoryWeights[i] = newCategoryWeight;
			newCategoryNames[i] = newCategoryName;
		}

		newCategoryGradeCounts[i] = categoryGradeCount;

		if (categoryGradeCount > 0) {
			newEarnedPoints[i] = new double[categoryGradeCount];
			newTotalPoints[i] = new double[categoryGradeCount];
			newGradeNames[i] = new string[categoryGradeCount];
			for (int j = 0; j < categoryGradeCount; j++) {
				newEarnedPoints[i][j] = mGrades->getEarnedPoints(i,j);
				newTotalPoints[i][j] = mGrades->getTotalPoints(i,j);
				newGradeNames[i][j] = mGrades->getGradeName(i,j);
			}
		} else {
			newEarnedPoints[i] = nullptr;
			newTotalPoints[i] = nullptr;
			newGradeNames[i] = nullptr;
		}

	}

	mGrades->setGradeWeightData(newEarnedPoints, newTotalPoints, newGradeNames, newCategoryWeights, newCategoryNames, newCategoryGradeCounts, categoryCount);
	mDataChanged = true;
}

void GradesDriver::editGradeName(string categoryName, string oldGradeName, string newGradeName) throw (runtime_error, invalid_argument) {
	int categoryIndex = -1;

	// This series of try/catch blocks verifies the given categoryName exists
	try {
		categoryIndex = getCategoryIndex(categoryName);
	} catch (runtime_error& e) {
		throw runtime_error(e.what());
	} catch (invalid_argument& e) {
		throw invalid_argument(e.what());
	}

	int categoryGradeCount = mGrades->getCategoryGradeCount(categoryIndex);

	if (categoryGradeCount == 0) {
		throw invalid_argument("Grade does not exist.");
	}

	int gradeIndex = -1;
	try {
		gradeIndex = getGradeIndex(oldGradeName, categoryIndex, categoryGradeCount);
	} catch (invalid_argument& e) {
		// If grade does not exist
		throw invalid_argument(e.what());
	}

	bool uniqueGradeName = true;
	for (int a = 0; a < categoryGradeCount; a++) {
		if (newGradeName == mGrades->getGradeName(categoryIndex, a)) {
			if (a != gradeIndex) {
				uniqueGradeName = false;
			}
		}
	}

	if(!uniqueGradeName) {
		throw invalid_argument("New grade name is not unique to the category.");
	}

	double* newEarnedPoints = new double[categoryGradeCount];
	double* newTotalPoints = new double[categoryGradeCount];
	string* newGradeNames = new string[categoryGradeCount];
	for (int b = 0; b < categoryGradeCount; b++) {
		if (!(b == gradeIndex)) {
			newEarnedPoints[b] = mGrades->getEarnedPoints(categoryIndex, b);
			newTotalPoints[b] = mGrades->getTotalPoints(categoryIndex, b);
			newGradeNames[b] = mGrades->getGradeName(categoryIndex, b);
		} else {
			newEarnedPoints[b] = mGrades->getEarnedPoints(categoryIndex, b);
			newTotalPoints[b] = mGrades->getTotalPoints(categoryIndex, b);
			newGradeNames[b] = newGradeName;
		}
	}

	mGrades->setCategoryGrades(newEarnedPoints, newTotalPoints, newGradeNames, categoryIndex, categoryGradeCount);
	updateCurrentGrade();
	mDataChanged = true;
}

void GradesDriver::editGradeValues(string categoryName, string oldGradeName, double inEarnedPoints, double inTotalPoints) throw (runtime_error, invalid_argument) {
	int categoryIndex = -1;

	// This series of try/catch blocks verifies the given categoryName exists
	try {
		categoryIndex = getCategoryIndex(categoryName);
	} catch (runtime_error& e) {
		throw runtime_error(e.what());
	} catch (invalid_argument& e) {
		throw invalid_argument(e.what());
	}

	int categoryGradeCount = mGrades->getCategoryGradeCount(categoryIndex);

	if (categoryGradeCount == 0) {
		throw invalid_argument("Grade does not exist.");
	}

	int gradeIndex = -1;
	try {
		gradeIndex = getGradeIndex(oldGradeName, categoryIndex, categoryGradeCount);
	} catch (invalid_argument& e) {
		// If grade does not exist
		throw invalid_argument(e.what());
	}

	double* newEarnedPoints = new double[categoryGradeCount];
	double* newTotalPoints = new double[categoryGradeCount];
	string* newGradeNames = new string[categoryGradeCount];
	for (int b = 0; b < categoryGradeCount; b++) {
		if (!(b == gradeIndex)) {
			newEarnedPoints[b] = mGrades->getEarnedPoints(categoryIndex, b);
			newTotalPoints[b] = mGrades->getTotalPoints(categoryIndex, b);
			newGradeNames[b] = mGrades->getGradeName(categoryIndex, b);
		} else {
			newEarnedPoints[b] = inEarnedPoints;
			newTotalPoints[b] = inTotalPoints;
			newGradeNames[b] = mGrades->getGradeName(categoryIndex, b);
		}
	}

	mGrades->setCategoryGrades(newEarnedPoints, newTotalPoints, newGradeNames, categoryIndex, categoryGradeCount);
	updateCurrentGrade();
	mDataChanged = true;
}

double GradesDriver::getCurrentGrade() const {
	double currentGrade = mGrades->getCurrentGrade();
	if (currentGrade != -1) {
		return (currentGrade/100);
	} else {
		return (-1);
	}
}

string GradesDriver::getFileName() const {
	return (mFileName);
}

string GradesDriver::deserializeString(string original) {
	int size = original.length();
	string output = "";
	for (int i = 0; i < size; i++) {
		if (original[i] == '_') {
			output += ' ';
		} else {
			output += original[i];
		}
	}

	return (output);
}


int GradesDriver::getCategoryIndex(string categoryName) const throw (runtime_error, invalid_argument) {
	int categoryCount = mGrades->getCategoryCount();
	if (categoryCount == 0) {
		throw runtime_error("No categories are initialized yet.");
	}

	int categoryIndex = -1;
	for (int i = 0; i < categoryCount; i++) {
		if (mGrades->getCategoryName(i) == categoryName) {
			categoryIndex = i;
			break;
		}
	}

	if (categoryIndex == -1) {
		throw invalid_argument("Category does not exist.");
	}

	return categoryIndex;
}

int GradesDriver::getGradeIndex(string gradeName, int categoryIndex, int categoryGradeCount) throw (invalid_argument) {
	int gradeIndex = -1;
	for (int a = 0; a < categoryGradeCount; a++) {
		if (gradeName == mGrades->getGradeName(categoryIndex, a)) {
			gradeIndex = a;
		}
	}

	if(gradeIndex == -1) {
		throw invalid_argument("Grade does not exist.");
	}

	return (gradeIndex);
}

string GradesDriver::serializeString(string original) {
	int size = original.length();
	string output = "";
	for (int i = 0; i < size; i++) {
		if (original[i] == ' ') {
			output += '_';
		} else {
			output += original[i];
		}
	}

	return (output);
}

void GradesDriver::updateCurrentGrade() const {
	int categoryCount = mGrades->getCategoryCount();
	if (categoryCount > 0) {
		int totalWeight = 0;
		double weightedGrade = 0;
		bool hasGrades = false;
		for (int i = 0; i < categoryCount; i++) {
			int categoryGradeCount = mGrades->getCategoryGradeCount(i);
			if (categoryGradeCount > 0) {
				hasGrades = true;
				double categoryEarnedPoints = 0;
				double categoryTotalPoints = 0;
				for (int j = 0; j < categoryGradeCount; j++) {
					categoryEarnedPoints += mGrades->getEarnedPoints(i,j);
					categoryTotalPoints += mGrades->getTotalPoints(i,j);
				}
				int categoryWeight = mGrades->getCategoryWeight(i);
				totalWeight += categoryWeight;
				weightedGrade += (categoryEarnedPoints/categoryTotalPoints)*categoryWeight;
			}
		}
		if (hasGrades) {
			int currentGrade = weightedGrade/totalWeight*10000;
			mGrades->setCurrentGrade(currentGrade);
		} else {
			mGrades->setCurrentGrade(-1);
		}
	} else {
		mGrades->setCurrentGrade(-1);
	}
}

void GradesDriver::writeDataChangesToFile() {
	fstream file;

	file.open(mFileName, ofstream::out | ofstream::trunc);
	int categoryCount = mGrades->getCategoryCount();

	if (categoryCount > 0) {
		file << categoryCount << endl;
		for (int i = 0; i < categoryCount; i++) {
			int categoryGradeCount = mGrades->getCategoryGradeCount(i);

			file << serializeString(mGrades->getCategoryName(i)) << " ";
			file << mGrades->getCategoryWeight(i) << " ";
			file << categoryGradeCount << endl;

			for (int j = 0; j < categoryGradeCount; j++) {
				file << serializeString(mGrades->getGradeName(i,j)) << " ";
				file << (int)(mGrades->getEarnedPoints(i,j)*100) << " ";
				file << (int)(mGrades->getTotalPoints(i,j)*100) << endl;
			}
		}
	} else {
		file << 0;
	}
	file.close();
}

int GradesDriver::getTotalCategoryWeight() const {
	int totalWeight = 0;
	int categoryCount = mGrades->getCategoryCount();

	for (int i = 0; i < categoryCount; i++) {
		totalWeight += mGrades->getCategoryWeight(i);
	}

	return (totalWeight);
}

string GradesDriver::print() {
	string output = "";
	int categoryCount = mGrades->getCategoryCount();
	for (int i = 0; i < categoryCount; i++) {
		int categoryGradeCount = mGrades->getCategoryGradeCount(i);
		output += mGrades->getCategoryName(i) + ":\n";
		for (int j = 0; j < categoryGradeCount; j++) {
			//string earnedPoints, totalPoints;
			stringstream ss;

			ss << fixed << setprecision(2) << mGrades->getEarnedPoints(i,j);
			string earnedPoints = ss.str();

			ss.clear();
			ss.str("");

			ss << fixed << setprecision(2) << mGrades->getTotalPoints(i,j);
			string totalPoints = ss.str();

			output += "\t" + mGrades->getGradeName(i, j) + ": ";
			output += earnedPoints + "\\" + totalPoints + "\n";
		}
		output += "\n";
	}

	stringstream ss;
	ss << fixed << setprecision(2) << ((double)mGrades->getCurrentGrade())/100.0;

	output += "Overall Grade: " + ss.str() + "%";

	return (output);
}

Grades* GradesDriver::getGrades() {
	return (mGrades);
}
