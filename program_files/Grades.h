#ifndef GRADES_H
#define GRADES_H
#include <iostream>
#include <string>
using namespace std;

class Grades {
	private:
	/** A double multiplied by 100 and typecast to an int to represnt 2 digits of precision for the current grade */
	int mCurrentGrade;

	/** Array of double array pointers that correspond to each category of grades and the points earned for each assignment */
	double** mEarnedPoints;

	/** Array of double array pointers that correspond to each category of grades and the total poitns earned for each assignment */
	double** mTotalPoints;

	/** Array of string names for each grade */
	string** mGradeNames;

	/** Array of the weight of each grade category */
	int* mCategoryWeights;

	/** Array of the names of each grade category */
	string* mCategoryNames;

	/** Array of the count of each grade category */
	int* mCategoryGradeCounts;

	/** Integer of the total number of Grade Categories */
	int mCategoryCount;

	public:
	/**
	* @pre The arrays passed in have data in them, grades and gradeNames are the same size and the category arrays are the same size, and there is at least one category
	* @param grades double nested array of grade values
	* @param gradeNames double nested array of grade names for each grade in the grades array
	* @param categoryWeights int array of weights of each category
	* @param categoryNames string array of the name of each category
	* @param categoryGradeCount int array of the number of grades in each category
	* @param categoryCount int representing the number of categories possible for grades, and the number of double arrays in grades
	* @post Initializes all Grades member variables to the values passed in
	*/
	Grades(double**& earnedPoints, double**& totalPoints, string**& gradeNames, int*& categoryWeights, string*& categoryNames, int*& categoryGradeCounts, int categoryCount);

	/**
	* @post Initializes all arrays to nullptr and all size integers to 0
	*/
	Grades();

	/**
	* @post Deletes all the arrays of this Grades object
	*/
	~Grades();

	/**
	* @return int The number of categories in the grades object
	*/
	int getCategoryCount() const;

	/**
	* @pre int index is a valid category index
	* @param int index of a grade category
	* @return int The number of grades in a category at the given index
	*/
	int getCategoryGradeCount (int index) const;

	/**
	* @pre int index is a valid category index
	* @param int index of a grade category
	* @return string The name of the grade category at the given index
	*/
	string getCategoryName (int index) const;

	/**
	* @pre int index is a valid category index
	* @param int index of a grade category
	* @return int Weight of the category at the given index
	*/
	int getCategoryWeight(int index) const;

	/**
	* @return int Four digit current grade, needs to be divided by 100 to put it to convert it to a double
	*/
	int getCurrentGrade() const;
	/**
	* @pre index1 is a valid category index and index2 is a valid grade index in the mEarnedPoints/mTotalPoints array
	* @param int index1 A grade category index
	* @param int index2 A grade value index
	* @return double earned points for an assignment in the category at index1 and the grade at index2
	*/
	double getEarnedPoints(int index1, int index2) const;

	/**
	* @pre index1 is a valid category index and index2 is a valid grade index in the mGrades/mGradeNames arrays
	* @param int index1 A grade category index
	* @param int index2 A grade value index
	* @return string grade at the category at index1 and the grade at index2
	*/
	string getGradeName(int index1, int index2) const;

	/**
	* @pre index1 is a valid category index and index2 is a valid grade index in the mEarnedPoints/mTotalPoints array
	* @param int index1 A grade category index
	* @param int index2 A grade value index
	* @return double total points for an assignment in the category at index1 and the grade at index2
	*/
	double getTotalPoints(int index1, int index2) const;

	/**
	* @pre gradeCategoryIndex is a valid grade category index
	* @param grades double array of grades for the category in gradeCategoryIndex
	* @param gradeNames string array of grade names for each grade in the grades array
	* @param gradeCategoryIndex int representing the index of the category for which the grades are being set
	* @param gradeCount int representing how many grades are in the grades array and names in the gradeNames array
	* @post Sets all grades of the given category to the values in the grades array their associated names in gradeNames
	*/
	void setCategoryGrades(double*& earnedPoints, double*& totalPoints, string*& gradeNames, int gradeCategoryIndex, int categoryGradeCount);

	/**
	* @post sets mCurrent grade to the integer passed in
	*/
	void setCurrentGrade(int currentGrade);

	/**
	* @param grades double nested array of grade values
	* @param gradeNames double nested array of grade names for each grade in the grades array
	* @param categoryWeights int array of weights of each category
	* @param categoryNames string array of the name of each category
	* @param categoryGradeCount int array of the number of grades in each category
	* @param categoryCount int representing the number of categories possible for grades, and the number of double arrays in grades
	* @post Sets all data for all categories
	*/
	void setGradeWeightData(double**& earnedPoints,double**& totalPoints,string**& gradeNames, int*& categoryWeights, string*& categoryNames, int*& categoryGradeCounts, int categoryCount);
};
#endif
