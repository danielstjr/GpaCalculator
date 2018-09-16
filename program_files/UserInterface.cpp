#include "UserInterface.h"

UserInterface::UserInterface () {
	mSemesterDriver = new SemesterDriver;
}

UserInterface::~UserInterface() {
	delete mSemesterDriver;
}

void UserInterface::run() {
	int intUserInput = 0;
	string menu = "";
	menu += "\nMenu:\n-------------------\n";
	menu += "1) Change Grades\n2) Change Courses\n";
	menu += "3) Change of Semesters\n4) Exit\n";

	while (intUserInput != 4) {
		intUserInput = getMenuInput(5, menu);
		if (intUserInput == 1) {
			changeGrades();
		} else if (intUserInput == 2) {
			changeCourses();
		} else if (intUserInput == 3) {
			changeSemesters();
		} else if (intUserInput == 4) {
			cout << "\nGoodbye!\n\n";
		}
	}
}

void UserInterface::changeGrades() {
	int intUserInput = 0;
	string menu = "";
	menu += "\nGrades Menu:\n-------------------\n";
	menu += "1) Add Grade\n2) Add Grade Category\n";
	menu += "3) Edit Grade\n4) Edit Grade Category\n";
	menu += "5) Delete Grade\n6) Delete Grade Category\n";
	menu += "7) Exit\n";

	while (intUserInput != 7) {
		intUserInput = getMenuInput(8, menu);

		if (intUserInput == 1) {
			addGrade();
		} else if (intUserInput == 2) {
			addGradeCategory();
		} else if (intUserInput == 3) {
			editGrade();
		} else if (intUserInput == 4) {
			editGradeCategory();
		} else if (intUserInput == 5) {
			deleteGrade();
		} else if(intUserInput == 6) {
			deleteGradeCategory();
		} else if (intUserInput == 7) {
			cout << "\nExiting Grades Menu...\n\n";
		}
	}
}

void UserInterface::addGrade() {
	int semesterIndex = getValidSemesterIndex();
	int courseIndex = getValidCourseIndex(semesterIndex);
	GradesDriver* courseGradesDriver = mSemesterDriver->getCourseDriver(semesterIndex)->getCourseGradesDriver(courseIndex);
	bool gradeAdded = false;
	while (!gradeAdded) {
		string categoryName = getValidString("Enter the name of the grade category you want to add the grade to: ", "Category names can only contain letters and spaces.", true, true);
		string gradeName = getValidString("Enter the name of the grade you want to add: ", "Grade names can only contain letters and spaces.", true, true);
		double earnedPoints = getValidNumber<double>("Enter the points earned for this assignment: ", "Invalid input.");
		double totalPoints = getValidNumber<double>("Enter the total points for this assignment: ", "Invalid input.");

		try {
			// string categoryName, string gradeName, double earnedPoints, double totalPoints
			courseGradesDriver->addGrade(categoryName, gradeName, earnedPoints, totalPoints);
			gradeAdded = true;
			cout << "Grade successfully added!" << endl;
		} catch (runtime_error& e) {
			cout << e.what() << endl;
			break;
		} catch (invalid_argument& e) {
			cout << e.what() << endl;
		}
	}
}

void UserInterface::addGradeCategory() {
	int semesterIndex = getValidSemesterIndex();
	int courseIndex = getValidCourseIndex(semesterIndex);
	GradesDriver* courseGradesDriver = mSemesterDriver->getCourseDriver(semesterIndex)->getCourseGradesDriver(courseIndex);
	bool gradeCategoryAdded = false;
	while (!gradeCategoryAdded) {
		string categoryName = getValidString("Enter the name of the grade category you want to add: ", "Category names can only contain letters and spaces.", true, true);
		int categoryWeight = getValidNumber<int>("Enter the weight of the grade category you want to add: ", "Category weights can only be whole numbers.");

		try {
			courseGradesDriver->addCategory(categoryName, categoryWeight);
			gradeCategoryAdded = true;
			cout << "Grade category successfully added!" << endl;
		} catch (invalid_argument& e) {
			cout << e.what() << endl;
		}
	}
}

void UserInterface::deleteGrade() {
	int semesterIndex = getValidSemesterIndex();
	int courseIndex = getValidCourseIndex(semesterIndex);
	GradesDriver* courseGradesDriver = mSemesterDriver->getCourseDriver(semesterIndex)->getCourseGradesDriver(courseIndex);
	bool gradeDeleted = false;

	while (!gradeDeleted) {
		string categoryName = getValidString("Enter the name of the grade category you want to delete the grade from: ", "Category names can only contain letters and spaces.", true, true);
		string gradeName = getValidString("Enter the name of the grade you want to delete: ", "Grade names can only contain letters and spaces.", true, true);

		try {
			courseGradesDriver->deleteGrade(categoryName, gradeName);
			gradeDeleted = true;
			cout << "Grade successfully deleted!" << endl;
		} catch (runtime_error& e) {
			cout << e.what() << endl;
			break;
		} catch (invalid_argument& e) {
			cout << e.what() << endl;
		}
	}
}

void UserInterface::deleteGradeCategory() {
	int semesterIndex = getValidSemesterIndex();
	int courseIndex = getValidCourseIndex(semesterIndex);
	GradesDriver* courseGradesDriver = mSemesterDriver->getCourseDriver(semesterIndex)->getCourseGradesDriver(courseIndex);
	bool gradeCategoryDeleted = false;

	while (!gradeCategoryDeleted) {
		string categoryName = getValidString("Enter the name of the grade category you want to delete: ", "Category names can only contain letters and spaces.", true, true);

		try {
			courseGradesDriver->deleteCategory(categoryName);
			gradeCategoryDeleted = true;
			cout << "Grade category successfully deleted!" << endl;
		} catch (runtime_error& e) {
			cout << e.what() << endl;
			break;
		} catch (invalid_argument& e) {
			cout << e.what() << endl;
		}
	}
}

void UserInterface::editGrade() {
	int semesterIndex = getValidSemesterIndex();
	int courseIndex = getValidCourseIndex(semesterIndex);
	GradesDriver* courseGradesDriver = mSemesterDriver->getCourseDriver(semesterIndex)->getCourseGradesDriver(courseIndex);
	bool gradeEdited = false;

	while (!gradeEdited) {
		string categoryName = getValidString("Enter the name of the grade category you want to add the grade to: ", "Category names can only contain letters and spaces.", true, true);
		string oldGradeName = getValidString("Enter the name of the grade you want to edit: ", "Grade names cano only contain letters and spaces", true, true);
		string newGradeName = getValidString("Enter the name of the new grade name: ", "Grade names can only contain letters and spaces.", true, true);
		double earnedPoints = getValidNumber<double>("Enter the new earned points for this assignment: ", "Invalid input.");
		double totalPoints = getValidNumber<double>("Enter the new total points for this assignment: ", "Invalid input.");

		try {
			//string categoryName, string oldGradeName, string newGradeName, double inEarnedPoints, double inTotalPoints
			courseGradesDriver->editGrade(categoryName, oldGradeName, newGradeName, earnedPoints, totalPoints);
			gradeEdited = true;
			cout << "Grades successfully edited!" << endl;
		} catch (runtime_error& e) {
			cout << e.what() << endl;
			break;
		} catch (invalid_argument& e) {
			cout << e.what() << endl;
		}
	}
}

void UserInterface::editGradeCategory() {
	int semesterIndex = getValidSemesterIndex();
	int courseIndex = getValidCourseIndex(semesterIndex);
	GradesDriver* courseGradesDriver = mSemesterDriver->getCourseDriver(semesterIndex)->getCourseGradesDriver(courseIndex);
	bool gradeCategoryEdited = false;

	while (!gradeCategoryEdited) {
		string oldCategoryName = getValidString("Enter the name of the grade category you want to edit: ", "Category names can only contain letters and spaces.", true, true);

		string newCategoryName = getValidString("Enter the name of the new grade category name: ", "Category names can only contain letters and spaces.", true, true);
		int newCategoryWeight = getValidNumber<int>("Enter the weight of the new grade category weight: ", "Category weights can only be whole numbers.");

		try {
			courseGradesDriver->editCategory(oldCategoryName, newCategoryName, newCategoryWeight);
			gradeCategoryEdited = true;
			cout << "Grade category sucessfully edited!" << endl;
		} catch (runtime_error& e) {
			cout << e.what() << endl;
			break;
		} catch (invalid_argument& e) {
			cout << e.what() << endl;
		}
	}
}

void UserInterface::changeCourses() {
	int intUserInput = 0;
	string menu = "";
	menu += "\nCourse Menu:\n-------------------\n";
	menu += "1) Add Course\n2) Edit Course\n";
	menu += "3) Delete Course\n4) Exit\n";

	while (intUserInput != 4) {
		intUserInput = getMenuInput(5, menu);

		if (intUserInput == 1) {
			addCourse();
		} else if (intUserInput == 2) {
			editCourse();
		} else if (intUserInput == 3) {
			deleteCourse();
		} else if (intUserInput == 4) {
			cout << "\nExiting Course Menu...\n\n";
		}
	}
}

void UserInterface::addCourse() {
	string season, year, departmentCode, courseNumber, fileName;
	int creditHours, semesterIndex;
	bool courseAdded = false;
	while (!courseAdded) {
		season = getValidString("Enter the semester's season: ", "Seasons can only contain letters.", false, false);
		year = to_string(getValidNumber<int>("Enter the semester's year: ", "Years can only be whole numbers"));

		string fullSemesterName = season + " " + year;

		try {
			semesterIndex = mSemesterDriver->getSemesterIndex(fullSemesterName);
		} catch (invalid_argument& e) {
			cout << e.what() << endl;
			continue;
		}

		departmentCode = getValidString("Enter the department code: ",  "Department Codes can only contain letters.", false, false);
		courseNumber = to_string(getValidNumber<int>("Enter course number: ", "Course numbers can only be whole numbers."));
		creditHours = getValidNumber<int>("Enter the credit hours for the course: ", "Credit hours have to be a whole number.");
		fileName = generateFileName(season, year, departmentCode, courseNumber, 2);

		try {
			mSemesterDriver->getSemester(semesterIndex)->getCourseDriver()->addCourse(departmentCode, courseNumber, creditHours, fileName);
			courseAdded = true;
			cout << "Course successfully added!" << endl;
		} catch (invalid_argument& e) {
			cout << e.what() << endl;
		}
	}
}

void UserInterface::deleteCourse() {
	string season, year, departmentCode, courseNumber, fullCourseName;
	bool courseDeleted = false;
	while (!courseDeleted) {
		bool validCourseName = false;
		int semesterIndex = getValidSemesterIndex();

		while (!validCourseName) {
			departmentCode = getValidString("Enter the department code for the course you want to delete: ", "Department Codes can only contain letters.", false, false);
			courseNumber = to_string(getValidNumber<int>("Enter the course number for the course you want to delete: ", "Course numbers can only be whole numbers."));

			fullCourseName = departmentCode + " " + courseNumber;

			try {
				mSemesterDriver->getSemester(semesterIndex)->getCourseDriver()->getCourseIndex(fullCourseName);
				validCourseName = true;
			} catch (invalid_argument& e) {
				cout << e.what() << endl;
			}
		}

		try {
			mSemesterDriver->getSemester(semesterIndex)->getCourseDriver()->deleteCourse(fullCourseName);
			courseDeleted = true;
			cout << "Course successfully deleted!" << endl;
		} catch (invalid_argument& e) {
			cout << e.what() << endl;
		}
	}
}

void UserInterface::editCourse() {
	string season, year, oldDepartmentCode, newDepartmentCode, oldCourseNumber, newCourseNumber, oldFullCourseName, finalGrade, fileName;
	int creditHours;
	bool courseEdited = false;
	while (!courseEdited) {
		bool validCourseName = false;
		bool validFinalGrade = false;
		int semesterIndex = getValidSemesterIndex();

		while (!validCourseName) {
			oldDepartmentCode = getValidString("Enter the department code for the course you want to edit: ", "Department Codes can only contain letters.", false, false);
			oldCourseNumber = to_string(getValidNumber<int>("Enter the course number for the course you want to edit: ", "Course numbers can only be whole numbers."));

			oldFullCourseName = oldDepartmentCode + " " + oldCourseNumber;

			try {
				mSemesterDriver->getSemester(semesterIndex)->getCourseDriver()->getCourseIndex(oldFullCourseName);
				validCourseName = true;
			} catch (invalid_argument& e) {
				cout << e.what() << endl;
			}
		}

		newDepartmentCode = getValidString("Enter the new department code for the old course: ",  "Department Codes can only contain letters.", false, false);
		newCourseNumber = to_string(getValidNumber<int>("Enter the new course number for the old course: ", "Course numbers can only be whole numbers."));
		creditHours = getValidNumber<int>("Enter the new credit hours for the course: ", "Credit hours have to be a whole number.");
		fileName = generateFileName(season, year, newDepartmentCode, newCourseNumber, 2);

		while(!validFinalGrade) {
			stringstream ss;
			string stringUserInput;
			char charFinalGrade;

			cout << "Enter the Final Grade: ";
			getline (cin, stringUserInput);

			ss << stringUserInput;
			ss >> charFinalGrade;

			if (ss.fail()||!ss.eof()) {
				if(stringUserInput == "-1") {
					finalGrade = "-1";
					validFinalGrade = true;
				} else {
					cout << "\nInvalid Input.\n\n";
				}
				continue;
			}
			if ((int(charFinalGrade) > 65 && int(charFinalGrade) < 69) || int(charFinalGrade) == 70) {
				finalGrade = charFinalGrade;
				validFinalGrade = true;
			}
		}

		try {
			mSemesterDriver->getSemester(semesterIndex)->getCourseDriver()->editCourse(oldFullCourseName, newDepartmentCode, newCourseNumber, finalGrade, creditHours, fileName);
			courseEdited = true;
			cout << "Course successfully edited!" << endl;
		} catch (invalid_argument& e) {
			cout << e.what() << endl;
		}
	}
}

void UserInterface::changeSemesters() {
	int intUserInput = 0;
	string menu = "";
	menu += "\nSemester Menu:\n-------------------\n";
	menu += "1) Add Semester\n2) Edit Semester\n";
	menu += "3) Delete Semester\n4) Exit\n";

	while (intUserInput != 4) {
		intUserInput = getMenuInput(5, menu);

		if (intUserInput == 1) {
			addSemester();
		} else if (intUserInput == 2) {
			editSemester();
		} else if (intUserInput == 3) {
			deleteSemester();
		} else if (intUserInput == 4) {
			cout << "\nExiting Semester Menu...\n\n";
		}
	}
}

void UserInterface::addSemester() {
	string season, year;
	bool semesterAdded = false;
	while (!semesterAdded) {
		season = getValidString("Enter the semester's season: ", "Seasons can only contain letters.", false, false);
		year = to_string(getValidNumber<int>("Enter the semester's year: ", "Years can only be whole numbers."));

		string courseDriverFileName = generateFileName(season, year, "", "", 1);

		try {
			mSemesterDriver->addSemester(season, year, courseDriverFileName);
			semesterAdded = true;
			cout << "Semester succesfully added!" << endl;
		} catch (invalid_argument& e) {
			cout << e.what() << endl;
		}
	}
}

void UserInterface::deleteSemester() {
	string season, year;
	bool semesterDeleted = false;
	while (!semesterDeleted) {
		season = getValidString("Enter the semester's season: ", "Seasons can only contain letters.", false, false);
		year = to_string(getValidNumber<int>("Enter the semester's year: ", "Years can only be whole numbers."));

		try {
			mSemesterDriver->deleteSemester(season, year);
			semesterDeleted = true;
			cout << "Semester succesfully deleted!" << endl;
		} catch (invalid_argument& e) {
			cout << e.what() << endl;
		}
	}
}

void UserInterface::editSemester() {
	string oldSeason, oldYear, newSeason, newYear;
	bool semesterEdit = false;
	while (!semesterEdit) {
		oldSeason = getValidString("Enter the semester's season that you want to edit: ", "Seasons can only contain letters.", false, false);
		oldYear = to_string(getValidNumber<int>("Enter the semester's year that you want to edit: ", "Years can only be whole numbers."));

		newSeason = getValidString("Enter the new season for the given semester: ", "Seasons can only contain letters.", false, false);
		newYear = to_string(getValidNumber<int>("Enter the new year for the given semester: ", "Years can only be whole numbers."));

		string newCourseDriverFileName = generateFileName(newSeason, newYear, "", "", 1);

		try {
			string oldFullSemesterName = oldSeason + " " + oldYear;
			mSemesterDriver->editSemester(oldFullSemesterName, newSeason, newYear, newCourseDriverFileName);
			semesterEdit = true;
			cout << "Semester succesfully added!" << endl;
		} catch (invalid_argument& e) {
			cout << e.what() << endl;
		}
	}
}

string lowercaseString(string input) {
	int length = input.length();
	string output = "";
	for (int a = 0; a < length; a++) {
		char letter = tolower(input[a]);
		output += letter;
	}

	return (output);
}

string UserInterface::generateFileName(string season, string year, string departmentCode, string courseNumber, int fileType) {
	season = lowercaseString(season);
	departmentCode = lowercaseString(departmentCode);

	string fileName = "data/";
	//fileType 1 is to make a courseDriver file, fileType 2 is to make a gradesDriver file
	if (fileType == 1) {
		fileName += "course_lists/" + season + "_" + year + ".data";
	} else if (fileType == 2) {
		fileName += "grades/" + season + "_" + year + "_" + departmentCode + "_" + courseNumber + ".data";
	}

	return (fileName);
}

int UserInterface::getValidSemesterIndex() {
	string season, year;
	int semesterIndex;
	while (true) {
		season = getValidString("Enter the semester's season: ", "Seasons can only contain letters.", false, false);
		year = to_string(getValidNumber<int>("Enter the semester's year: ", "Years can only be whole numbers"));

		string fullSemesterName = season + " " + year;

		try {
			semesterIndex = mSemesterDriver->getSemesterIndex(fullSemesterName);
			break;
		} catch (invalid_argument& e) {
			cout << e.what() << endl;
		}
	}

	return (semesterIndex);
}

int UserInterface::getValidCourseIndex(int semesterIndex) {
	string departmentCode, courseNumber, fullCourseName;
	int courseIndex;
	while (true) {
		departmentCode = getValidString("Enter the department code: ",  "Department Codes can only contain letters.", false, false);
		courseNumber = to_string(getValidNumber<int>("Enter course number: ", "Course numbers can only be whole numbers."));

		fullCourseName = departmentCode + " " + courseNumber;
		try {
			courseIndex = mSemesterDriver->getSemester(semesterIndex)->getCourseDriver()->getCourseIndex(fullCourseName);
			break;
		} catch (invalid_argument& e) {
			cout << e.what() << endl;
		}
	}

	return (courseIndex);
}

int UserInterface::getMenuInput(int upperBound, string persistentMessage) {
	int intUserInput = 0;
	stringstream ss;
	string stringUserInput = "";
	while (true) {
		ss.clear();
		ss.str("");

        cout << persistentMessage;
		cout << "\nEnter Menu Selection: ";

		getline(cin, stringUserInput);
		ss << stringUserInput;
		ss >> intUserInput;

		if (ss.fail()||!ss.eof()) {
			intUserInput = 0;
		}
		if (intUserInput > 0 && intUserInput < upperBound) {
			break;
		} else {
			cout << "\nInvalid selection.\n\n";
		}
	}

	return(intUserInput);
}

string UserInterface::getValidString(string inputMessage, string failureMessage, bool allowSpaces, bool allowNumbers) {
	string userInput;
	while (true) {
		userInput = "";

		cout << inputMessage;
		getline(cin, userInput);

		if (validString(userInput, allowSpaces, allowNumbers) ) {
			break;
		} else {
			cout << "\n" + failureMessage + "\n\n";
		}
	}

	return (userInput);
}

template<typename number>
number UserInterface::getValidNumber(string inputMessage, string failureMessage) {
	number numberUserInput;
	while (true) {
		stringstream ss;
		string stringUserInput;
		numberUserInput = 0;

		cout << inputMessage;

		getline(cin, stringUserInput);

		ss << stringUserInput;
		ss >> numberUserInput;

		if(!(ss.fail()||!ss.eof())) {
			break;
		} else {
			cout << "\n" + failureMessage + "\n\n";
		}
	}

	return (numberUserInput);
}

bool UserInterface::validString(string input, bool allowSpaces, bool allowNumbers) {
	int inputLength = input.length();
	bool valid = true;

	for (int i = 0; i < inputLength; i++) {
		if (!((int(input[i]) > 96 && int(input[i]) < 123) || (int(input[i]) > 64 && int(input[i]) < 91))) {
			if(allowSpaces && (int(input[i]) == 32)) {
				continue;
			} else if (allowNumbers) {
				stringstream ss;
				int test;

				ss << input[i];
				ss >> test;

				if(!(ss.fail()||!ss.eof())) {
					continue;
				}
			}
			valid = false;
			break;
		}
	}

	return (valid);
}
