#ifndef GRADES_DRIVER_H
#define GRADES_DRIVER_H
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <string>
#include "Grades.h"
using namespace std;

class GradesDriver {
	private:
	/** Boolean used to determine if the data needs to be written to file */
	bool mDataChanged;

	/** File name string used to write serialized data to */
	string mFileName;

	/** Pointer to a Grades object */
	Grades* mGrades;

	/**
	* @param string original String that has underscores instead of spaces
	* @return original with underscores replaced by spaces
	*/
	string deserializeString(string original);

	/**
	* @param string categoryName Name of the category whose index is being searched for
	* @throw runtime_error if no categories are initialized
	* @throw invalid_argument if category does not exist
	* @return int index of the cateogry with the given categoryName
	*/
	int getCategoryIndex(string categoryName) const throw (runtime_error, invalid_argument);

	/**
	* @param string gradeName string of the grade name being searched for in the given category
	* @param int gradeCategoryIndex index of the category to search in
	* @param int categoryGradeCount integer of how many grades are in the grade arrays of this category
	* @throw invalid_argument if grade does not exist
	* @return index of grade with the given gradeName
	*/
	int getGradeIndex(string gradeName, int gradeCategoryIndex, int categoryGradeCount) throw (invalid_argument);

	/**
	* @param string original String that has spaces instead of underscores
	* @return original with spaces replaced by underscores
	*/
	string serializeString(string original);

	/**
	* @post Calculates the current grade based off of all grades in the grades array
	*/
	void updateCurrentGrade() const;

	/**
	* @pre mFileName is a valid file name
	* @post Writes grade information to the mFileName
	*/
	void writeDataChangesToFile();

	public:
	/**
	* @pre fileName is a valid file with data in it, formatted correctly
	* @post Gets all data from the file and initializes mGrades with it
	*/
	GradesDriver(string fileName);

	/**
	* @post Creates an empty GradesDriver object
	*/
	GradesDriver();

	/**
	* @pre initialized GradesDriver object
	* @return Creates a new GradesDriver object that is a deep copy of original
	*/
	GradesDriver& operator= (const GradesDriver& original);

	/**
	* @post Calls the function to write data changes to file if mDataChanged is true then calls delete on the mGrades object
	*/
	~GradesDriver();

	/**
	* @pre Assumes that categoryWeight is a valid weight and categoryName does not contain non-letter/number characters
	* @param int categoryWeight The weight of the grade category being added
	* @param string categoryName The name of the grade category being added
	* @throw invalid_argument if the categoryName passed in already exists as a categoryName
	* @post Creates the category of the given name and weight in the mGrades object and updates mDataChanged to true
	*/
	void addCategory(string categoryName, int categoryWeight) throw (invalid_argument);

	/**
	* @pre gradeName is a valid name without any non-letter/number characters
	* @param string categoryName String name of a category
	* @param double grade Grade to be inserted into the given category
	* @param string gradeName Name of the grade to be inserted into the given category
	* @throw runtime_error if no categories are initialized in the mGrades object
	* @throw invalid_argument if the given category or grade do not exist
	* @post Adds a grade of the given value and name to the given category and updates mDataChanged to true
	*/
	void addGrade(string categoryName, string gradeName, double earnedPoints, double totalPoints) throw (runtime_error, invalid_argument);


	/**
	* @param string categoryName
	* @throw runtime_error if no categories are initialized right now
	* @throw invalid_argument if the categoryName passed in does not exist
	* @post Deletes the categoryName passed in and all associated category data and updates mDataChanged to true
	*/
	void deleteCategory(string categoryName)  throw (runtime_error, invalid_argument);

	/**
	* @param string categoryName Name of the category from which to delete the grade
	* @param string gradeName Name of the grade to delete from the given category
	* @throw runtime_error if no categories are initialized
	* @throw invalid_argument if the category or grade do not exist
	* @post deletes the given grade from the given category and updates mDataChanged to true
	*/
	void deleteGrade(string categoryName, string gradeName) throw (runtime_error, invalid_argument);


	/**
	* @pre newCategoryName is a valid name without any non-letter/number characters
	* @param string oldCategoryName Name of category to edit
	* @param string newCategoryName Replacement name for edited category
	* @param int newCategoryWeight Replacement weight for edited category
	* @throw runtime_error if no categories are initialized right now
	* @throw invalid_argument if the categoryName passed in does not exist or the categoryName is edited to be non unique
	* @post remakes the category at the index of oldCategoryName with the given newCategoryName and newCategoryWeight and updates mDataChanged to true
	*/
	void editCategory (string oldCategoryName, string newCategoryName, int newCategoryWeight) throw (runtime_error, invalid_argument);

	/**
	* @pre replacementGradeName is a valid name without any non-letter/number characters
	* @param string categoryName Name of the category that holds the grade to edit
	* @param string oldGradeName Name of the grade that is being edited
	* @param string replacementGradeName New name for the grade in the given category
	* @param double replacementGrade New grade for the grade in the given category
	* @throw runtime_error if no categories are initialized
	* @throw invalid_argument if given category or grade do not exist, or the new grade name is non unique
	* @post resets grade in the given category and name with the replacement grade and grade name
	*/
	void editGradeName(string categoryName, string oldGradeName, string newGradeName) throw (runtime_error, invalid_argument);

	/**
	* @pre replacementGradeName is a valid name without any non-letter/number characters
	* @param string categoryName Name of the category that holds the grade to edit
	* @param string oldGradeName Name of the grade that is being edited
	* @param string replacementGradeName New name for the grade in the given category
	* @param double replacementGrade New grade for the grade in the given category
	* @throw runtime_error if no categories are initialized
	* @throw invalid_argument if given category or grade do not exist, or the new grade name is non unique
	* @post resets grade in the given category and name with the replacement grade and grade name
	*/
	void editGradeValues(string categoryName, string oldGradeName, double inEarnedPoints, double inTotalPoints) throw (runtime_error, invalid_argument);

	/**
	* @return The current grade from the mGrades object as a double
	*/
	double getCurrentGrade() const;

	/**
	* @return The file name used to construct this object
	*/
	string getFileName() const;

	/**
	*
	*/
	int getTotalCategoryWeight() const;
};
#endif
