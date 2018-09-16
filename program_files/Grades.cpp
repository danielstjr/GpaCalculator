#include "Grades.h"

Grades::Grades(double**& earnedPoints, double**& totalPoints, string**& gradeNames, int*& categoryWeights, string*& categoryNames, int*& categoryGradeCounts, int categoryCount) {
	mEarnedPoints = new double*[categoryCount];
	mTotalPoints = new double*[categoryCount];
	mGradeNames = new string*[categoryCount];
	mCategoryWeights = new int[categoryCount];
	mCategoryNames = new string[categoryCount];
	mCategoryGradeCounts = new int[categoryCount];

	for (int i = 0; i < categoryCount; i++) {
		mCategoryWeights[i] = categoryWeights[i];
		mCategoryNames[i] = categoryNames[i];
		mCategoryGradeCounts[i] = categoryGradeCounts[i];

		int categoryGradeCount = categoryGradeCounts[i];
		if (categoryGradeCount > 0) {
			mEarnedPoints[i] = new double[categoryGradeCount];
			mTotalPoints[i] = new double[categoryGradeCount];
			mGradeNames[i] = new string[categoryGradeCount];
			for (int j = 0; j < categoryGradeCount; j++) {
				mGradeNames[i][j] = gradeNames[i][j];
				mEarnedPoints[i][j] = earnedPoints[i][j];
				mTotalPoints[i][j] = totalPoints[i][j];
			}
			delete[] earnedPoints[i];
			delete[] totalPoints[i];
			delete[] gradeNames[i];
		} else {
			mEarnedPoints[i] = nullptr;
			mTotalPoints[i] = nullptr;
			mGradeNames[i] = nullptr;
		}
	}

	mCategoryCount = categoryCount;

	delete[] earnedPoints;
	delete[] totalPoints;
	delete[] gradeNames;
	delete[] categoryWeights;
	delete[] categoryNames;
	delete[] categoryGradeCounts;
}

Grades::Grades() {
	mEarnedPoints = nullptr;
	mTotalPoints = nullptr;
	mGradeNames = nullptr;
	mCategoryWeights = nullptr;
	mCategoryNames = nullptr;
	mCategoryGradeCounts = nullptr;
	mCategoryCount = 0;
	mCurrentGrade = -1;
}

Grades& Grades::operator= (const Grades& original) {
	for (int a = 0; a < mCategoryCount; a++) {
		delete[] mEarnedPoints[a];
		delete[] mTotalPoints[a];
		delete[] mGradeNames[a];
	}

	delete[] mEarnedPoints;
	delete[] mTotalPoints;
	delete[] mGradeNames;
	delete[] mCategoryWeights;
	delete[] mCategoryNames;
	delete[] mCategoryGradeCounts;

	if (mCategoryCount > 0) {
		mCategoryCount = original.mCategoryCount;

		mEarnedPoints = new double*[mCategoryCount];
		mTotalPoints = new double*[mCategoryCount];
		mGradeNames = new string*[mCategoryCount];
		mCategoryNames = new string[mCategoryCount];
		mCategoryWeights = new int[mCategoryCount];
		mCategoryGradeCounts = new int[mCategoryCount];

		for (int b = 0; b < mCategoryCount; b++) {
			int categoryGradeCount = original.mCategoryGradeCounts[b];

			mCategoryNames[b] = original.mCategoryNames[b];
			mCategoryWeights[b] = original.mCategoryWeights[b];
			mCategoryGradeCounts[b] = categoryGradeCount;

			if (categoryGradeCount > 0) {
				mEarnedPoints[b] = new double[categoryGradeCount];
				mTotalPoints[b] = new double[categoryGradeCount];
				mGradeNames[b] = new string[categoryGradeCount];
				for (int c = 0; c < categoryGradeCount; c++) {
					mEarnedPoints[b][c] = original.mEarnedPoints[b][c];
					mTotalPoints[b][c] = original.mTotalPoints[b][c];
					mGradeNames[b][c] = original.mGradeNames[b][c];
				}
			} else {
				mEarnedPoints[b] = nullptr;
				mTotalPoints[b] = nullptr;
				mGradeNames[b] = nullptr;
			}
		}

		mCurrentGrade = original.mCurrentGrade;
	} else {
		mEarnedPoints = nullptr;
		mTotalPoints = nullptr;
		mGradeNames = nullptr;
		mCategoryNames = nullptr;
		mCategoryWeights = nullptr;
		mCategoryGradeCounts = nullptr;
		mCategoryCount = 0;
		mCurrentGrade = -1;
	}

	return(*this);
}

Grades::~Grades() {
	for (int i = 0; i < mCategoryCount; i++) {
		if (mCategoryGradeCounts[i] > 0) {
			delete[] mEarnedPoints[i];
			delete[] mTotalPoints[i];
			delete[] mGradeNames[i];
		}
	}
	delete[] mEarnedPoints;
	delete[] mTotalPoints;
	delete[] mGradeNames;
	delete[] mCategoryWeights;
	delete[] mCategoryNames;
	delete[] mCategoryGradeCounts;
}


int Grades::getCategoryCount() const {
	return (mCategoryCount);
}

int Grades::getCategoryGradeCount (int index) const {
	return (mCategoryGradeCounts[index]);
}

string Grades::getCategoryName (int index) const {
	return (mCategoryNames[index]);
}

int Grades::getCategoryWeight(int index) const {
	return (mCategoryWeights[index]);
}

int Grades::getCurrentGrade() const {
	return (mCurrentGrade);
}

double Grades::getEarnedPoints(int index1, int index2) const {
	return (mEarnedPoints[index1][index2]);
}

string Grades::getGradeName(int index1, int index2) const {
	return (mGradeNames[index1][index2]);
}

double Grades::getTotalPoints(int index1, int index2) const {
	return(mTotalPoints[index1][index2]);
}

void Grades::setCategoryGrades(double*& earnedPoints, double*& totalPoints, string*& gradeNames, int gradeCategoryIndex, int categoryGradeCount) {
	if (mCategoryGradeCounts[gradeCategoryIndex]) {
		delete[] mEarnedPoints[gradeCategoryIndex];
		delete[] mTotalPoints[gradeCategoryIndex];
		delete[] mGradeNames[gradeCategoryIndex];
	}

	if (categoryGradeCount > 0) {
		mEarnedPoints[gradeCategoryIndex] = earnedPoints;
		mTotalPoints[gradeCategoryIndex] = totalPoints;
		mGradeNames[gradeCategoryIndex] = gradeNames;
		mCategoryGradeCounts[gradeCategoryIndex] = categoryGradeCount;
	} else {
		mEarnedPoints[gradeCategoryIndex] = nullptr;
		mTotalPoints[gradeCategoryIndex] = nullptr;
		mGradeNames[gradeCategoryIndex] = nullptr;
		mCategoryGradeCounts[gradeCategoryIndex] = 0;
	}
}

void Grades::setCurrentGrade(int currentGrade) {
	mCurrentGrade = currentGrade;
}

void Grades::setGradeWeightData(double**& earnedPoints,double**& totalPoints,string**& gradeNames, int*& categoryWeights, string*& categoryNames, int*& categoryGradeCounts, int categoryCount) {
	for (int a = 0; a < mCategoryCount; a++) {
		if (mCategoryGradeCounts[a] > 0) {
			delete[] mEarnedPoints[a];
			delete[] mTotalPoints[a];
			delete[] mGradeNames[a];
		}
	}
	delete[] mEarnedPoints;
	delete[] mTotalPoints;
	delete[] mGradeNames;
	delete[] mCategoryWeights;
	delete[] mCategoryNames;
	delete[] mCategoryGradeCounts;

	if (categoryCount > 0) {
		mCategoryCount = categoryCount;

		mCategoryNames = categoryNames;
		mCategoryWeights = categoryWeights;
		mCategoryGradeCounts = categoryGradeCounts;

		mEarnedPoints = new double*[categoryCount];
		mTotalPoints = new double*[categoryCount];
		mGradeNames = new string*[categoryCount];
		for (int i = 0; i < categoryCount; i++) {
				setCategoryGrades(earnedPoints[i], totalPoints[i], gradeNames[i], i, categoryGradeCounts[i]);
		}

		delete[] earnedPoints;
		delete[] totalPoints;
		delete[] gradeNames;
	} else {
		mEarnedPoints = nullptr;
		mTotalPoints = nullptr;
		mGradeNames = nullptr;
		mCategoryWeights = nullptr;
		mCategoryNames = nullptr;
		mCategoryGradeCounts = nullptr;
		mCategoryCount = 0;
	}
}
