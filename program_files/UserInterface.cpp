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
	menu += "1) View Grades\n2) View Course Info\n";
	menu += "3) View Courses in Semester\n4) Change Data\n5) Exit\n";

	while (intUserInput != 5) {
		intUserInput = getMenuInput(5, menu);
		if (intUserInput == 1) {
			viewGrades();
		} else if (intUserInput == 2) {
			viewCourseInfo();
		} else if (intUserInput == 3) {
			viewSemesterInfo();
		} else if (intUserInput == 4) {
			changeDataMenu();
		} else if (intUserInput == 5) {
			cout << "\nGoodbye!\n\n";
		}
	}
}

void UserInterface::viewGrades() {
	int semesterIndex, courseIndex;
	bool print = true;
	cout << "\n";
	try {
		semesterIndex = getValidSemesterIndex();
		courseIndex = getValidCourseIndex(semesterIndex);
	} catch (runtime_error& e) {
		cout << e.what() << endl;
		print = false;
	} catch (logic_error& e) {
		cout << "\nQuitting...\n\n";;
		print = false;
	}

	if (print) {
		string output = mSemesterDriver->getSemester(semesterIndex)->getCourseDriver()->getCourseGradesDriver(courseIndex)->print();
		cout << endl << output << endl;
	}
}

void UserInterface::viewCourseInfo() {
	int semesterIndex, courseIndex;
	bool print = true;
	cout << "\n";
	try {
		semesterIndex = getValidSemesterIndex();
		courseIndex = getValidCourseIndex(semesterIndex);
	} catch (runtime_error& e) {
		cout << e.what() << endl;
		print = false;
	} catch (logic_error& e) {
		cout << "\nQuitting...\n\n";;
		print = false;
	}

	if (print) {
		string output = mSemesterDriver->getSemester(semesterIndex)->getCourseDriver()->printCourse(courseIndex);
		cout << endl << output << endl;
	}
}

void UserInterface::viewSemesterInfo() {
	int semesterIndex;
	bool print = true;
	cout << "\n";
	try {
		semesterIndex = getValidSemesterIndex();
	} catch (runtime_error& e) {
		cout << e.what() << endl;
		print = false;
	} catch (logic_error& e) {
		cout << "\nQuitting...\n\n";;
		print = false;
	}

	if (print) {
		string output = mSemesterDriver->getSemester(semesterIndex)->getCourseDriver()->printAllCourses();
		cout << "\nCourses in " << mSemesterDriver->getSemester(semesterIndex)->getFullSemesterName();
		cout << ": " << output << endl;
	}
}

void UserInterface::changeDataMenu() {
	int intUserInput = 0;
	string menu = "";
	menu += "\nChange Data Menu:\n-------------------\n";
	menu += "1) Change Grades\n2) Change Courses\n";
	menu += "3) Change of Semesters\n4) Exit\n";

	while (intUserInput != 4) {
		intUserInput = getMenuInput(4, menu);
		if (intUserInput == 1) {
			changeGrades();
		} else if (intUserInput == 2) {
			changeCourses();
		} else if (intUserInput == 3) {
			changeSemesters();
		} else if (intUserInput == 4) {
			cout << "\nExiting Change Data Menu...\n";
		}
	}
}

void UserInterface::changeGrades() {
	int intUserInput = 0;
	int semesterIndex = 0;
	int courseIndex = 0;
	string menu = "";

	cout << "\n";
	try {
		semesterIndex = getValidSemesterIndex();
		courseIndex = getValidCourseIndex(semesterIndex);

		menu += "\nGrades Menu:\n-------------------\n";
		menu += "Current Selection:";
		menu += "\nSemester: " + mSemesterDriver->getSemester(semesterIndex)->getFullSemesterName();
		menu += "\nCourse: " + mSemesterDriver->getSemester(semesterIndex)->getCourseDriver()->getFullCourseName(courseIndex);
		menu += "\n-------------------\n";
		menu += "1) Add Grade\n2) Add Grade Category\n";
		menu += "3) Edit Grade Name\n4) Edit Grade Values\n5) Edit Category\n";
		menu += "6) Delete Grade\n7) Delete Grade Category\n";
		menu += "8) Select a Different Semester/Course\n9) Exit\n";
	} catch (runtime_error& e) {
		cout << e.what() << endl;
		intUserInput = 8;
	} catch (logic_error& e) {
		cout << "\nQuitting...\n\n";;
		intUserInput = 8;
	}

	while (intUserInput != 9) {
		intUserInput = getMenuInput(9, menu);

		if (intUserInput == 1) {
			addGrade(semesterIndex, courseIndex);
		} else if (intUserInput == 2) {
			addGradeCategory(semesterIndex, courseIndex);
		} else if (intUserInput == 3) {
			editGrade(semesterIndex, courseIndex, 0);
		} else if (intUserInput == 4) {
			editGrade(semesterIndex, courseIndex, 1);
		} else if (intUserInput == 5) {
			editGradeCategory(semesterIndex, courseIndex);
		} else if (intUserInput == 6) {
			deleteGrade(semesterIndex, courseIndex);
		} else if(intUserInput == 7) {
			deleteGradeCategory(semesterIndex, courseIndex);
		} else if (intUserInput == 8) {
			cout << "\n";
			int newSemesterIndex, newCourseIndex;
			try {
				newSemesterIndex = getValidSemesterIndex();
				newCourseIndex = getValidCourseIndex(semesterIndex);
			} catch (logic_error& e) {
				continue;
			}

			semesterIndex = newSemesterIndex;
			courseIndex = newCourseIndex;

			menu = "";
			menu += "\nGrades Menu:\n-------------------\n";
			menu += "Current Selection:";
			menu += "\nSemester: " + mSemesterDriver->getSemester(semesterIndex)->getFullSemesterName();
			menu += "\nCourse: " + mSemesterDriver->getSemester(semesterIndex)->getCourseDriver()->getFullCourseName(courseIndex);
			menu += "\n-------------------\n";
			menu += "1) Add Grade\n2) Add Grade Category\n";
			menu += "3) Edit Grade Name\n4) Edit Grade Values\n5) Edit Category\n";
			menu += "6) Delete Grade\n7) Delete Grade Category\n";
			menu += "8) Select a Different Semester/Course\n9) Exit\n";
		} else if (intUserInput == 9) {
			cout << "\nExiting Grades Menu...\n";
		}
	}
}

void UserInterface::addGrade(int semesterIndex, int courseIndex) {
	bool gradeAdded = false;
	try {
		while (!gradeAdded) {
			string categoryName, gradeName;
			double earnedPoints, totalPoints;
			cout << "\n";
			categoryName = getValidString("Enter the name of the grade category you want to add the grade to", "Category names can only contain letters and spaces.", true, true);
			gradeName = getValidString("Enter the name of the grade you want to add", "Grade names can only contain letters and spaces.", true, true);
			earnedPoints = getValidNumber<double>("Enter the points earned for this assignment", "Invalid input.");
			totalPoints = getValidNumber<double>("Enter the total points for this assignment", "Invalid input.");
			cout << "\n";

			try {
				mSemesterDriver->getCourseDriver(semesterIndex)->getCourseGradesDriver(courseIndex)->addGrade(categoryName, gradeName, earnedPoints, totalPoints);
				gradeAdded = true;
				cout << "Grade successfully added!" << endl;
			} catch (invalid_argument& e) {
				cout << e.what() << endl;
			}
		}
	} catch (runtime_error& e) {
		cout << e.what() << endl;
	} catch(logic_error& e) {
		cout << "\nQuitting...\n\n";;
	}
}

void UserInterface::addGradeCategory(int semesterIndex, int courseIndex) {
	GradesDriver* courseGradesDriver = mSemesterDriver->getCourseDriver(semesterIndex)->getCourseGradesDriver(courseIndex);
	bool gradeCategoryAdded = false;

	try {
		while (!gradeCategoryAdded) {
			string categoryName;
			int categoryWeight;

			cout << "\n";
			categoryName = getValidString("Enter the name of the grade category you want to add", "Category names can only contain letters and spaces.", true, true);
			categoryWeight = getValidNumber<int>("Enter the weight of the grade category you want to add", "Category weights can only be whole numbers.");
			cout << "\n";

			try {
				courseGradesDriver->addCategory(categoryName, categoryWeight);
				gradeCategoryAdded = true;
				cout << "Grade category successfully added!" << endl;
			} catch (invalid_argument& e) {
				cout << e.what() << endl;
			}
		}
	} catch (runtime_error& e) {
		cout << e.what() << endl;
	} catch(logic_error& e) {
		cout << "\nQuitting...\n\n";;
	}
}

void UserInterface::deleteGrade(int semesterIndex, int courseIndex) {
	GradesDriver* courseGradesDriver = mSemesterDriver->getCourseDriver(semesterIndex)->getCourseGradesDriver(courseIndex);
	bool gradeDeleted = false;

	try {
		while (!gradeDeleted) {
			string categoryName, gradeName;

			cout << "\n";
			categoryName = getValidString("Enter the name of the grade category you want to delete the grade from", "Category names can only contain letters and spaces.", true, true);
			gradeName = getValidString("Enter the name of the grade you want to delete", "Grade names can only contain letters and spaces.", true, true);
			cout << "\n";

			try {
				courseGradesDriver->deleteGrade(categoryName, gradeName);
				gradeDeleted = true;
				cout << "Grade successfully deleted!" << endl;
			} catch (invalid_argument& e) {
				cout << e.what() << endl;
			}
		}
	} catch (runtime_error& e) {
		cout << e.what() << endl;
	} catch(logic_error& e) {
		cout << "\nQuitting...\n\n";;
	}
}

void UserInterface::deleteGradeCategory(int semesterIndex, int courseIndex) {
	GradesDriver* courseGradesDriver = mSemesterDriver->getCourseDriver(semesterIndex)->getCourseGradesDriver(courseIndex);
	bool gradeCategoryDeleted = false;

	try {
		while (!gradeCategoryDeleted) {
			string categoryName;
			cout << "\n";
			categoryName = getValidString("Enter the name of the grade category you want to delete: ", "Category names can only contain letters and spaces.", true, true);
			cout << "\n";

			try {
				courseGradesDriver->deleteCategory(categoryName);
				gradeCategoryDeleted = true;
				cout << "Grade category successfully deleted!" << endl;
			} catch (invalid_argument& e) {
				cout << e.what() << endl;
			}
		}
	} catch (runtime_error& e) {
 		cout << e.what() << endl;
 	} catch(logic_error& e) {
		cout << "\nQuitting...\n\n";;
	}
}

void UserInterface::editGrade(int semesterIndex, int courseIndex, int changeType) {
	GradesDriver* courseGradesDriver = mSemesterDriver->getCourseDriver(semesterIndex)->getCourseGradesDriver(courseIndex);
	bool gradeEdited = false;

	try {
		while (!gradeEdited) {
			string categoryName, oldGradeName, newGradeName;
			double earnedPoints, totalPoints;

			cout << "\n";
			categoryName = getValidString("Enter the name of the grade category you want to add the grade to", "Category names can only contain letters and spaces.", true, true);
			oldGradeName = getValidString("Enter the name of the grade you want to edit", "Grade names cano only contain letters and spaces", true, true);
			if (changeType == 0) {
				newGradeName = getValidString("Enter the new grade name", "Grade names can only contain letters and spaces.", true, true);
				cout << "\n";

				try {
					courseGradesDriver->editGradeName(categoryName, oldGradeName, newGradeName);
				} catch (invalid_argument& e) {
					cout << e.what() << endl;
				}
			} else if (changeType == 1) {
				earnedPoints = getValidNumber<double>("Enter the new earned points for this assignment", "Invalid input.");
				totalPoints = getValidNumber<double>("Enter the new total points for this assignment", "Invalid input.");
				cout << "\n";

				try {
					courseGradesDriver->editGradeValues(categoryName, oldGradeName, earnedPoints, totalPoints);
				} catch (invalid_argument& e) {
					cout << e.what() << endl;
				}
			}
			gradeEdited = true;
			cout << "Grade successfully edited!" << endl;
		}
	} catch (runtime_error& e) {
		cout << e.what() << endl;
	} catch(logic_error& e) {
		cout << "\nQuitting...\n\n";;
	}
}

void UserInterface::editGradeCategory(int semesterIndex, int courseIndex) {
	GradesDriver* courseGradesDriver = mSemesterDriver->getCourseDriver(semesterIndex)->getCourseGradesDriver(courseIndex);
	bool gradeCategoryEdited = false;

	try {
		while (!gradeCategoryEdited) {
			string oldCategoryName, newCategoryName;
			int newCategoryWeight;

			cout << "\n";
			oldCategoryName = getValidString("Enter the name of the grade category you want to edit", "Category names can only contain letters and spaces.", true, true);
			newCategoryName = getValidString("Enter the name of the new grade category name", "Category names can only contain letters and spaces.", true, true);
			newCategoryWeight = getValidNumber<int>("Enter the weight of the new grade category weight", "Category weights can only be whole numbers.");
			cout << "\n";

			try {
				courseGradesDriver->editCategory(oldCategoryName, newCategoryName, newCategoryWeight);
				gradeCategoryEdited = true;
				cout << "Grade category sucessfully edited!" << endl;
			} catch (invalid_argument& e) {
				cout << e.what() << endl;
			}
		}
	} catch (runtime_error& e) {
		cout << e.what() << endl;
	} catch(logic_error& e) {
		cout << "\nQuitting...\n\n";;
	}
}

void UserInterface::changeCourses() {
	int intUserInput = 0;
	int semesterIndex = 0;
	string menu = "";

	try {
		cout << "\n";
		semesterIndex = getValidSemesterIndex();

		menu += "\nCourse Menu:\n-------------------\n";
		menu += "Current Selection:";
		menu += "\nSemester: " + mSemesterDriver->getSemester(semesterIndex)->getFullSemesterName();
		menu += "\n-------------------\n";
		menu += "1) Add Course\n2) Edit Deparment Code\n3) Edit Course Number\n";
		menu += "4) Edit Credit Hours\n5) Edit Final Grade\n6) Delete Course\n";
		menu += "7) Change Semester Selection\n8) Exit\n";
	} catch(logic_error& e) {
		cout << "\nQuitting...\n\n";;
		intUserInput = 5;
	} catch (runtime_error& e) {
		cout << e.what() << endl;
		intUserInput = 5;
	}

	while (intUserInput != 8) {
		intUserInput = getMenuInput(8, menu);

		if (intUserInput == 1) {
			addCourse(semesterIndex);
		} else if (intUserInput == 2) {
			editCourse(semesterIndex, 0);
		} else if (intUserInput == 3) {
			editCourse(semesterIndex, 1);
		} else if (intUserInput == 4) {
			editCourse(semesterIndex, 2);
		} else if (intUserInput == 5) {
			editCourse(semesterIndex, 3);
		} else if (intUserInput == 6) {
			deleteCourse(semesterIndex);
		} else if (intUserInput == 7) {
			cout << "\n";
			try {
				semesterIndex = getValidSemesterIndex();
			} catch (logic_error& e) {
				continue;
			}

			menu = "";
			menu += "\nCourse Menu:\n-------------------\n";
			menu += "Current Selection:";
			menu += "\nSemester: " + mSemesterDriver->getSemester(semesterIndex)->getFullSemesterName();
			menu += "\n-------------------\n";
			menu += "1) Add Course\n2) Edit Deparment Code\n3) Edit Course Number\n";
			menu += "4) Edit Credit Hours\n5) Edit Final Grade\n6) Delete Course\n";
			menu += "7) Change Semester Selection\n8) Exit\n";
		} else if (intUserInput == 8) {
			cout << "\nExiting Course Menu...\n";
		}
	}
}

void UserInterface::addCourse(int semesterIndex) {
	string season = mSemesterDriver->getSemester(semesterIndex)->getSeason();
	string year = mSemesterDriver->getSemester(semesterIndex)->getYear();
	bool courseAdded = false;

	try {
		while (!courseAdded) {
			string departmentCode, courseNumber, fileName;
			int creditHours;

			cout << "\n";
			departmentCode = getValidString("Enter the department code: ",  "Department Codes can only contain letters.", false, false);
			courseNumber = to_string(getValidNumber<int>("Enter course number: ", "Course numbers can only be whole numbers."));
			creditHours = getValidNumber<int>("Enter the credit hours for the course: ", "Credit hours have to be a whole number.");
			cout << "\n";

			fileName = generateFileName(season, year, departmentCode, courseNumber, 2);

			try {
				mSemesterDriver->getSemester(semesterIndex)->getCourseDriver()->addCourse(departmentCode, courseNumber, creditHours, fileName);
				courseAdded = true;
				cout << "Course successfully added!" << endl;
			} catch (invalid_argument& e) {
				cout << e.what() << endl;
			}
		}
	} catch (runtime_error& e) {
		cout << e.what() << endl;
	} catch(logic_error& e) {
		cout << "\nQuitting...\n\n";;
	}
}

void UserInterface::deleteCourse(int semesterIndex) {
	string season = mSemesterDriver->getSemester(semesterIndex)->getSeason();
	string year = mSemesterDriver->getSemester(semesterIndex)->getYear();
	bool courseDeleted = false;

	try {
		while (!courseDeleted) {
			string departmentCode, courseNumber, fullCourseName;
			bool validCourseName = false;

			while (!validCourseName) {
				cout << "\n";
				departmentCode = getValidString("Enter the department code for the course you want to delete", "Department Codes can only contain letters.", false, false);
				courseNumber = to_string(getValidNumber<int>("Enter the course number for the course you want to delete", "Course numbers can only be whole numbers."));
				cout << "\n";

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
	} catch (runtime_error& e) {
		cout << e.what() << endl;
	} catch (logic_error& e) {
		cout << "\nQuitting...\n\n";;
	}
}

void UserInterface::editCourse(int semesterIndex, int changeType) {
	string season = mSemesterDriver->getSemester(semesterIndex)->getSeason();
	string year = mSemesterDriver->getSemester(semesterIndex)->getYear();
	CourseDriver* courseDriver = mSemesterDriver->getSemester(semesterIndex)->getCourseDriver();
	bool courseEdited = false;

	try {
		while (!courseEdited) {
			string oldDepartmentCode, oldCourseNumber, oldFullCourseName;
			bool validCourseName = false;
			bool validFinalGrade = false;

			while (!validCourseName) {
				cout << "\n";
				oldDepartmentCode = getValidString("Enter the department code for the course you want to edit", "Department Codes can only contain letters.", false, false);
				oldCourseNumber = to_string(getValidNumber<int>("Enter the course number for the course you want to edit", "Course numbers can only be whole numbers."));
				cout << "\n";

				oldFullCourseName = oldDepartmentCode + " " + oldCourseNumber;

				try {
					mSemesterDriver->getSemester(semesterIndex)->getCourseDriver()->getCourseIndex(oldFullCourseName);
					validCourseName = true;
				} catch (invalid_argument& e) {
					cout << e.what() << endl;
				}
			}

			if (changeType  == 0) {
				string newDepartmentCode = getValidString("Enter the new department code for the old course",  "Department Codes can only contain letters.", false, false);
				string fileName = generateFileName(season, year, newDepartmentCode, oldCourseNumber, 2);
				try {
					courseDriver->editDepartmentCode(oldFullCourseName, newDepartmentCode, fileName);
				} catch (exception& e) {
					cout << e.what() << endl;
				}

			} else if (changeType == 1) {
				string newCourseNumber = to_string(getValidNumber<int>("Enter the new course number for the old course", "Course numbers can only be whole numbers."));
				string fileName = generateFileName(season, year, oldDepartmentCode, newCourseNumber, 2);

				try {
					courseDriver->editCourseNumber(oldFullCourseName, newCourseNumber, fileName);
				} catch (exception& e) {
					cout << e.what() << endl;
				}
			} else if (changeType == 2) {
				int newCreditHours = getValidNumber<int>("Enter the new credit hours for the course", "Credit hours have to be a whole number.");

				try {
					courseDriver->editCreditHours(oldFullCourseName, newCreditHours);
				} catch (exception& e) {
					cout << e.what() << endl;
				}
			} else if (changeType == 3) {
				string newFinalGrade;

				while(!validFinalGrade) {
					stringstream ss;
					string stringUserInput;
					char charFinalGrade;

					cout << "Enter the Final Grade: ";
					getline (cin, stringUserInput);

					if (stringUserInput == "q" || stringUserInput == "quit") {
						throw logic_error("User Quit");
					}

					ss << stringUserInput;
					ss >> charFinalGrade;

					if (ss.fail()||!ss.eof()) {
						if(stringUserInput == "-1") {
							newFinalGrade = "-1";
							validFinalGrade = true;
						} else {
							cout << "\nInvalid Final Grade.\n\n";
						}
						continue;
					}
					if ((int(charFinalGrade) > 65 && int(charFinalGrade) < 69) || int(charFinalGrade) == 70) {
						newFinalGrade = charFinalGrade;
						validFinalGrade = true;
					}
				}

				try {
					courseDriver->editFinalGrade(oldFullCourseName, newFinalGrade);
				} catch (exception& e) {
					cout << e.what() << endl;
				}
			}
			courseEdited = true;
			cout << "\nCourse succesfully edited!\n";
		}
	} catch (logic_error& e) {
		cout << "\nQuitting...\n\n";;
	}
}

void UserInterface::changeSemesters() {
	int intUserInput = 0;
	string menu = "";
	menu += "\nSemester Menu:\n-------------------\n";
	menu += "1) Add Semester\n2) Edit Semester\n";
	menu += "3) Delete Semester\n4) Exit\n";

	while (intUserInput != 4) {
		intUserInput = getMenuInput(4, menu);

		if (intUserInput == 1) {
			addSemester();
		} else if (intUserInput == 2) {
			editSemester();
		} else if (intUserInput == 3) {
			deleteSemester();
		} else if (intUserInput == 4) {
			cout << "\nExiting Semester Menu...\n";
		}
	}
}

void UserInterface::addSemester() {
	string season, year;
	bool semesterAdded = false;
	try {
		while (!semesterAdded) {
			cout << "\n";
			season = getValidString("Enter the semester's season: ", "Seasons can only contain letters.", false, false);
			year = to_string(getValidNumber<int>("Enter the semester's year: ", "Years can only be whole numbers."));
			cout << "\n";

			string courseDriverFileName = generateFileName(season, year, "", "", 1);

			try {
				mSemesterDriver->addSemester(season, year, courseDriverFileName);
				semesterAdded = true;
				cout << "Semester succesfully added!" << endl;
			} catch (invalid_argument& e) {
				cout << e.what() << endl;
			}
		}
	} catch (runtime_error& e) {
		cout << e.what() << endl;
	} catch (logic_error& e) {
		cout << "\nQuitting...\n\n";;
	}
}

void UserInterface::deleteSemester() {
	string season, year;
	bool semesterDeleted = false;
	try {
		while (!semesterDeleted) {
			cout << "\n";
			season = getValidString("Enter the semester's season: ", "Seasons can only contain letters.", false, false);
			year = to_string(getValidNumber<int>("Enter the semester's year: ", "Years can only be whole numbers."));
			cout << "\n";

			try {
				mSemesterDriver->deleteSemester(season, year);
				semesterDeleted = true;
				cout << "Semester succesfully deleted!" << endl;
			} catch (invalid_argument& e) {
				cout << e.what() << endl;
			}
		}
	} catch (runtime_error& e) {
		cout << e.what() << endl;
	} catch (logic_error& e) {
		cout << "\nQuitting...\n\n";;
	}
}

void UserInterface::editSemester() {
	string oldSeason, oldYear, newSeason, newYear;
	bool semesterEdit = false;
	try {
		while (!semesterEdit) {
			cout << "\n";
			oldSeason = getValidString("Enter the semester's season that you want to edit: ", "Seasons can only contain letters.", false, false);
			oldYear = to_string(getValidNumber<int>("Enter the semester's year that you want to edit: ", "Years can only be whole numbers."));
			cout << "\n";
			newSeason = getValidString("Enter the new season for the given semester: ", "Seasons can only contain letters.", false, false);
			newYear = to_string(getValidNumber<int>("Enter the new year for the given semester: ", "Years can only be whole numbers."));
			cout << "\n";

			string newCourseDriverFileName = generateFileName(newSeason, newYear, "", "", 1);

			try {
				string oldFullSemesterName = oldSeason + " " + oldYear;
				mSemesterDriver->editSemester(oldFullSemesterName, newSeason, newYear, newCourseDriverFileName);
				semesterEdit = true;
				cout << "Semester succesfully edited!" << endl;
			} catch (invalid_argument& e) {
				cout << e.what() << endl;
			}
		}
	} catch (runtime_error& e) {
		cout << e.what() << endl;
	} catch (logic_error& e) {
		cout << "\nQuitting...\n\n";;
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

int UserInterface::getValidSemesterIndex() throw (logic_error, runtime_error) {
	string season, year;
	int semesterIndex;

	while (true) {
		try {
			season = getValidString("Enter the semester's season", "Seasons can only contain letters.", false, false);
			year = to_string(getValidNumber<int>("Enter the semester's year", "Years can only be whole numbers"));
		} catch (logic_error& e) {
			throw logic_error("User Quit");
		}

		string fullSemesterName = season + " " + year;

		try {
			semesterIndex = mSemesterDriver->getSemesterIndex(fullSemesterName);
			break;
		} catch (runtime_error& e) {
			throw runtime_error(e.what());
		} catch (invalid_argument& e) {
			cout << e.what() << "\n\n";
		}
	}

	return (semesterIndex);
}

int UserInterface::getValidCourseIndex(int semesterIndex) throw (logic_error, runtime_error) {
	string departmentCode, courseNumber, fullCourseName;
	int courseIndex;
	while (true) {
		try {
			departmentCode = getValidString("Enter the department code",  "Department Codes can only contain letters.", false, false);
			courseNumber = to_string(getValidNumber<int>("Enter course number", "Course numbers can only be whole numbers."));
		} catch (runtime_error& e) {
			throw logic_error("User Quit");
		}

		fullCourseName = departmentCode + " " + courseNumber;
		try {
			courseIndex = mSemesterDriver->getSemester(semesterIndex)->getCourseDriver()->getCourseIndex(fullCourseName);
			break;
		} catch(runtime_error& e) {
			throw runtime_error(e.what());
		} catch (invalid_argument& e) {
			cout << endl << e.what() << "\n\n";
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
		if (intUserInput > 0 && intUserInput <= upperBound) {
			break;
		} else {
			cout << "\nInvalid selection.\n\n";
		}
	}

	return(intUserInput);
}

string UserInterface::getValidString(string inputMessage, string failureMessage, bool allowSpaces, bool allowNumbers) throw (logic_error) {
	string userInput;
	while (true) {
		userInput = "";

		cout << inputMessage << " (Enter 'q' or 'quit' to quit): ";
		getline(cin, userInput);

		if (userInput == "q" || userInput == "quit") {
			throw logic_error("User quit.");
		}

		if (validString(userInput, allowSpaces, allowNumbers)) {
			break;
		} else {
			cout << "\n" + failureMessage + "\n\n";
		}
	}

	return (userInput);
}

template<typename number>
number UserInterface::getValidNumber(string inputMessage, string failureMessage) throw (logic_error) {
	number numberUserInput;
	while (true) {
		stringstream ss;
		string stringUserInput;
		numberUserInput = 0;

		cout << inputMessage << " (Enter 'q' or 'quit' to quit): ";

		getline(cin, stringUserInput);

		if (stringUserInput == "q" || stringUserInput == "quit") {
			throw logic_error("User quit.");
		}

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
