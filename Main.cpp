#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
using namespace std;

class database
{
public:
	string name;
	string date_of_birth;
	string address;
	string gender;
	int is_deleted;
	int id;

	database():name("") , date_of_birth("") , address("") , gender("") , is_deleted(0) , id(0) {}
	
	database(string inputName, string inputDateOfBirth, string inputAddress,
			string inputGender,int inputIsDeleted , int inputId) : 
			name(inputName),date_of_birth(inputDateOfBirth), address(inputAddress),gender(inputGender),
			is_deleted(inputIsDeleted),id(inputId) 
			{}
	
	database(const database& inputDB) : 
			name(inputDB.name),date_of_birth(inputDB.date_of_birth),address(inputDB.address),
			gender(inputDB.gender),is_deleted(inputDB.is_deleted),id(inputDB.id) 
			{}
	
	~database() {};
};

void init_terminal();
void db_menu(database*& input_dbArr, int& size); // show menu
void increase_dbArr_size_by_one(database*& input_dbArr, int obj_szamlalo);
void init_database_and_size(database*& input_dbArr, string fileName, int&size); // database init
void make_database_array(database*& input_dbArr, string, int); 
bool input_number_validation(int choose_menu, string input_number, int& size); // input check(menu, delete, modify)
void add_user(database*& input_dbArr,int&size);
void write_to_file(database*& input_dbArr, int&size); // save, write to file
bool check_for_file_open_failure(string fileName); 
void list_users(database*& input_dbArr, int list_active_or_deleted_users,int& size); 
void delete_user(database*& input_dbArr, int& size); 
void modify_user(database*& input_dbArr, int& size); 
string date_of_birth_validation(string);


int main()
{
	init_terminal();

	string fileName = "database.txt";
	check_for_file_open_failure(fileName);

	int size; // database array size
	database* dbArr = new database[1]; // work on database(upload,edit,save)

	init_database_and_size(dbArr, fileName, size);
	cout << endl;
	db_menu(dbArr, size);

	cout << endl << endl;
	return 0;
}

//dynamic array size automatically increasing if new user added
void increase_dbArr_size_by_one(database*& input_dbArr, int obj_counter)
{
	database* tmp = new database[obj_counter];
	int i_copy_the_objects = 0;
	while (i_copy_the_objects < obj_counter)
	{
		tmp[i_copy_the_objects] = input_dbArr[i_copy_the_objects];
		i_copy_the_objects++;
	}

	delete[]input_dbArr;
	input_dbArr = new database[obj_counter + 1];
	i_copy_the_objects = 0;
	while (i_copy_the_objects < obj_counter)
	{
		input_dbArr[i_copy_the_objects] = tmp[i_copy_the_objects];
		i_copy_the_objects++;
	}
	input_dbArr[obj_counter] = database();
	delete[]tmp;
}

void init_database_and_size(database*& input_dbArr, string fileName,int&size)
{
	int obj_counter = 0;
	string current_line_from_txt_file;

	ifstream readFile;
	readFile.open(fileName);
	while (getline(readFile, current_line_from_txt_file))
	{
		if (obj_counter > 0) { increase_dbArr_size_by_one(input_dbArr, obj_counter); } //if get a new line the array size increasing

		make_database_array(input_dbArr, current_line_from_txt_file, obj_counter); //upload the line into the dynamic array
		obj_counter++;
	}
	readFile.close();

	size = obj_counter;
}

void make_database_array(database* &input_dbArr, string current_line_from_txt_file, int obj_counter)
{
	size_t comma_counter = 0; // or field counter
	size_t i_go_through_the_characters_in_the_line = 0;
	size_t i_go_to_the_comma = 0;
	size_t last_comma_position = 100; // initialization before assigning a value, you will later get the last comma in the line

	string nameTmp;
	string dateOfBirthTmp;
	string addressTmp;
	string genderTmp;
	string isDeletedTmp;
	string idTmp;

	while (i_go_through_the_characters_in_the_line < current_line_from_txt_file.length())
	{
		if (current_line_from_txt_file.at(i_go_through_the_characters_in_the_line) == 44)// 44 = ',' - ASCII 44 decimal value is a comma
		{
			while (i_go_to_the_comma < i_go_through_the_characters_in_the_line)
			{
				if (comma_counter == 0)
				{
					nameTmp += current_line_from_txt_file.at(i_go_to_the_comma);
				}

				if (comma_counter == 1)
				{
					dateOfBirthTmp += current_line_from_txt_file.at(i_go_to_the_comma);
				}

				if (comma_counter == 2)
				{
					addressTmp += current_line_from_txt_file.at(i_go_to_the_comma);
				}

				if (comma_counter == 3)
				{
					genderTmp += current_line_from_txt_file.at(i_go_to_the_comma);
				}

				if (comma_counter == 4)
				{
					last_comma_position = i_go_through_the_characters_in_the_line;
					isDeletedTmp += current_line_from_txt_file.at(i_go_to_the_comma);
				}

				if (i_go_to_the_comma < current_line_from_txt_file.length())
				{
					i_go_to_the_comma++;
				}
			}
			i_go_to_the_comma++; // you get the value after the comma in a line
			comma_counter++;
		}
		else if (i_go_through_the_characters_in_the_line > last_comma_position) 
		{
			idTmp += current_line_from_txt_file.at(i_go_through_the_characters_in_the_line);
		}
		i_go_through_the_characters_in_the_line++;
	}
	
	input_dbArr[obj_counter] = database(nameTmp, dateOfBirthTmp, addressTmp, genderTmp, stoi(isDeletedTmp), stoi(idTmp));
}

void db_menu(database*& input_dbArr, int&size)
{
	cout << "Database program v0.2\n\n";
	string str_menu;
	do {

		string menu_items = "Please choose from the following menu items:\n"
			"1......List users\n"
			"2......Delete user\n"
			"3......Add new user\n"
			"4......Modify\n"
			"5......Clear window\n"
			"6......Save - Write to file\n"
			"7......List deleted user(s)\n"
			"9......Quit\n\n";

		cout << menu_items;
		cin >> str_menu;

		if (!input_number_validation(1,str_menu, size)) {
			do
			{
				cout << "Incorrect menu(" << str_menu << ") please re-enter:";
				cin >> str_menu;
			} while (!input_number_validation(1,str_menu, size));
		}

		cout << "The selected menu:" << str_menu << endl << endl;

		int int_menu = stoi(str_menu);
		switch (int_menu) {
		case 1:
			cout << "Current menu: list users\n";
			list_users(input_dbArr,0,size);
			break;
		case 2:
			cout << "Current menu: delete user\n";
			delete_user(input_dbArr, size);
			break;
		case 3:
			cout << "Current menu: add new user\n";
			add_user(input_dbArr,size);
			break;
		case 4:
			cout << "Current menu: modify user\n";
			modify_user(input_dbArr, size);
			break;
		case 5:
			system("CLS");
			cout << "Current menu: clear window\n";
			break;
		case 6:
			cout << "Current menu: write to file\n";
			write_to_file(input_dbArr, size);
			break;
		case 7:
			cout << "Current menu : list deleted user(s)\n";
			list_users(input_dbArr, 1, size);
			break;
		case 9:
			cout << "Quit.\n";
			break;
		default:
			cout << "no menu here" << endl;
			break;
		}
	} while (stoi(str_menu) != 9);
}

bool input_number_validation(int choose_validation, string input_number, int&size)
{
	int i_go_through_the_characters = 0;

	// 1 -> check input menu
	// 2 -> check deleted user id
	// 3 -> modify user input number validation
	switch (choose_validation)
	{
	case 1:
		// check input menu
		if (input_number.length() == 1 &&
			(input_number.at(0) == '1' || input_number.at(0) == '2' || input_number.at(0) == '3' ||
				input_number.at(0) == '4' || input_number.at(0) == '5' || input_number.at(0) == '6' ||
				input_number.at(0) == '7' || input_number.at(0) == '9')
			) return true; 

		return false;
		break;
	case 2:
		//check input user id
		i_go_through_the_characters = 0;
		if ( input_number.length() <= to_string(size).length() )
		{
			while (i_go_through_the_characters < input_number.length())
			{
				if (input_number.at(i_go_through_the_characters) == '1' || input_number.at(i_go_through_the_characters) == '2' ||
					input_number.at(i_go_through_the_characters) == '3' || input_number.at(i_go_through_the_characters) == '4' ||
					input_number.at(i_go_through_the_characters) == '5' || input_number.at(i_go_through_the_characters) == '6' ||
					input_number.at(i_go_through_the_characters) == '7' || input_number.at(i_go_through_the_characters) == '8' ||
					input_number.at(i_go_through_the_characters) == '9' || input_number.at(i_go_through_the_characters) == '0')
				{ }
				else
				{
					cout << "Helytelen karakter: " << input_number.at(i_go_through_the_characters) << endl;
					return false;
				}
				i_go_through_the_characters++;
			}
		}
		else
		{
			return false;
		}

		if (stoi(input_number) > size || stoi(input_number) < 1)
		{
			return false;
		}

		return true;
		break;
	case 3:
		//change user menu item check
		if (input_number.length() == 1 &&
			(input_number.at(0) == '1' || input_number.at(0) == '2' ||
				input_number.at(0) == '3' || input_number.at(0) == '4' )
			) return true;
		return false;
		break;
	case 4:
		return false;
		break;
	default:
		break;
	}
}

void add_user(database*& input_dbArr, int&size)
{
	//reinit the dynamic array( increase_dbArr_size_by_one() )
	// copy data temporarily
	database* tmp = new database[size];
	size_t obj_counter = 0;

	while (obj_counter < size)
	{

		tmp[obj_counter] = input_dbArr[obj_counter]; 
		obj_counter++;
	}

	//delete and reinitialize a dynamic array
	delete[] input_dbArr;
	input_dbArr = new database[size + 1];

	//restore data to the original dynamic array
	obj_counter = 0;
	while (obj_counter < size)
	{
		input_dbArr[obj_counter] = tmp[obj_counter];
		obj_counter++;
	}
	// end of copy

	string name;
	cout << "Please enter a username:";
	getline(cin >> ws, name);
	input_dbArr[size].name = name;

	string year;
	string month;
	string day;
	cout << "Please enter the date of birth(YEAR/MONTH/DAY):\n";
	cout << "Year:";
	cin >> year;
	cout << "Month:";
	cin >> month;
	cout << "Day:";
	cin >> day;

	string date_of_birth = year;
	date_of_birth += ".";
	date_of_birth += month;
	date_of_birth += ".";
	date_of_birth += day;
	date_of_birth += ".";

	input_dbArr[size].date_of_birth = date_of_birth_validation(date_of_birth);

	string address;
	cout << "Address:";
	getline(cin >> ws, address);
	input_dbArr[size].address = address;

	string gender;
	cout << "Gender:";
	getline(cin >> ws, gender);
	input_dbArr[size].gender = gender;
	cout << endl;

	input_dbArr[size].id = size + 1;
	size = size + 1;
//	cout << "add_user() - size:" << size << endl;
}

void init_terminal()
{
	system("chcp 1250"); //console window to display accented characters correctly
	SetConsoleTextAttribute
	(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_RED |
		FOREGROUND_GREEN |
		FOREGROUND_INTENSITY |
		BACKGROUND_BLUE);
	system("CLS");
	cout << "Terminal setup successful!\n";
}

void write_to_file(database*& input_dbArr, int &size)
{
	ofstream writeFile;
	writeFile.open("database.txt");
	
	size_t obj_counter = 0;
	while (obj_counter < size)
	{
		writeFile << input_dbArr[obj_counter].name << "," <<
			input_dbArr[obj_counter].date_of_birth << "," <<
			input_dbArr[obj_counter].address << "," <<
			input_dbArr[obj_counter].gender << "," <<
			input_dbArr[obj_counter].is_deleted << "," <<
			input_dbArr[obj_counter].id << "\n";
			
		obj_counter++;
	}
	writeFile.close();
	cout << "Successfully writing to file!!\n\n";
}

bool check_for_file_open_failure(string fileName)
{
	ifstream readFile;
	readFile.open(fileName);
	if (readFile.fail())
	{
		cout << "ERROR: Unable to open file(" << fileName << ")!\n\n";
		readFile.close();
		return false;
	}

	cout << "Successful opening of the file:" << fileName << "\n";
	readFile.close();
	return true;
}

void list_users(database*& input_dbArr, int list_active_or_deleted_users, int&size)
{
	string tab;
	size_t obj_counter = 0;
	while (obj_counter < size)
	{	
		
		//TO-DO: set tab instead of comma after the fields!
/*
		if (input_dbArr[obj_counter].gender.length() > 2)
		{
			tab = "\t";
		}
		else if(input_dbArr[obj_counter].gender.length() <= 2)
		{
			tab = "\t\t";
		}
*/
		//0 == active, 1 == deleted user
		if(list_active_or_deleted_users == 0)
		{ 
			if (input_dbArr[obj_counter].is_deleted == 0)
			{
				cout << "Name:" << input_dbArr[obj_counter].name << ","
					<< "Age:" << input_dbArr[obj_counter].date_of_birth << ","
					<< "Address:" << input_dbArr[obj_counter].address << ","
					<< "Gender:" << input_dbArr[obj_counter].gender << ","
					<< "id:" << input_dbArr[obj_counter].id << "\n";
			}

		}
		else if(list_active_or_deleted_users == 1)
		{
			if (input_dbArr[obj_counter].is_deleted == 1)
			{
				cout << "Name:" << input_dbArr[obj_counter].name << ","
					<< "Age:" << input_dbArr[obj_counter].date_of_birth << ","
					<< "Address:" << input_dbArr[obj_counter].address << ","
					<< "Gender:" << input_dbArr[obj_counter].gender << ","
					<< "id:" << input_dbArr[obj_counter].id << "\n";
			}
		}
		obj_counter++;
	}  
	cout << endl << endl;
}

void delete_user(database*& input_dbArr, int&size)
{
	cout << "Please provide the id of the user to be deleted:" << endl;
	string str_id_to_be_deleted;
	cin >> str_id_to_be_deleted;

	if(!input_number_validation(2, str_id_to_be_deleted, size))
	{
		do
		{
			cout << "The id you entered is incorrect!\nPlease re-enter:";
			cin >> str_id_to_be_deleted;
		}
		while (!input_number_validation(2, str_id_to_be_deleted, size));
	}
	int int_id_to_be_deleted = stoi(str_id_to_be_deleted)-1;
	cout << endl;

	if (input_dbArr[int_id_to_be_deleted].is_deleted == 1)
	{ 
		cout << "User has already been deleted! Return to the main menu.\n\n";
	}
	else 
	{
		input_dbArr[int_id_to_be_deleted].is_deleted = 1;
		cout << "Delete successfully!\n\n";
	}
}

void modify_user(database*& input_dbArr, int&size)
{
	cout << "Please provide the id of the user to be modified:";
	string str_user_id;
	cin >> str_user_id;
	cout << endl;
	//TO-DO: fix conditions
	if( ( (int)str_user_id.at(0) > 57 || (int)str_user_id.at(0) < 49 ) && ( stoi(str_user_id) > size || stoi(str_user_id) < 1 ) ) // ascii dec57 = '9', dec58 = ':', dec49 = 1
	{
		do
		{
			cout << "The id you entered is incorrect!\nPlease re-enter:";
			cin >> str_user_id;

		} while (stoi(str_user_id) > size  || stoi(str_user_id) < 1 && ( (int)str_user_id.at(0) > 57  || (int)str_user_id.at(0) < 49 ) );
	}
	int user_id = stoi(str_user_id) - 1;

	cout << "Selected user:\n";
	cout << "Name:" << input_dbArr[user_id].name << "\n"
		<< "Age:" << input_dbArr[user_id].date_of_birth << "\n"
		<< "Address:" << input_dbArr[user_id].address << "\n"
		<< "Gender:" << input_dbArr[user_id].gender << "\n"
		<< "id:" << input_dbArr[user_id].id << "\n\n";

	cout << "What would you like to modify ?\n"
			"1...Name\n2...Age\n3...Address\n4...Gender\n";
	string str_menu_to_change;
	cin >> str_menu_to_change;
	int tmp = 0; // not used for a size of the case
	if (!input_number_validation(3, str_menu_to_change, tmp)) {
		do {
			cout << "Incorrect menu(" << str_menu_to_change << ") please re-enter:";
			cin >> str_menu_to_change;
		} while (!input_number_validation(3, str_menu_to_change, tmp));
	}

	int int_menu_to_change = stoi(str_menu_to_change);

	if (int_menu_to_change == 1)
	{
		cout << "What do you want to change at the name?("<< input_dbArr[user_id].name << ")\n";
		string new_name;
		getline(cin >> ws, new_name);
		input_dbArr[user_id].name = new_name;
	}
		
	if (int_menu_to_change == 2)
	{
		cout << "What do you want to change the date of birth for?("<< input_dbArr[user_id].date_of_birth <<")\n";
		string new_date_of_birth;
		cin >> new_date_of_birth;
		new_date_of_birth = date_of_birth_validation(new_date_of_birth);
		input_dbArr[user_id].date_of_birth = new_date_of_birth;
	}

	if (int_menu_to_change == 3)
	{
		cout << "What do you want to change the address for?("<< input_dbArr[user_id].address << ")\n";
		string new_address;
		getline(cin >> ws, new_address);
		input_dbArr[user_id].address = new_address;
	}

	if (int_menu_to_change == 4)
	{
		cout << "What do you want to change the gender for?\n";
		string new_gender;
		getline(cin >> ws, new_gender);
		input_dbArr[user_id].gender = new_gender;
	}
	cout << "Successful modification!\n\n";
}

string date_of_birth_validation(string date_of_birth)
{
	string str_year;
	string str_month;
	string str_day;

	int year;
	int month;
	int day;

	int i_go_to_the_point = 0;
	int point_counter = 0;
	int i_go_through_the_characters = 0;
	while (i_go_through_the_characters < date_of_birth.length())
	{
		if (date_of_birth.at(i_go_through_the_characters) == '.')
		{
			while (i_go_to_the_point < i_go_through_the_characters)
			{
				if (point_counter == 0) 
				{
					str_year += date_of_birth.at(i_go_to_the_point);
				}				
				
				if (point_counter == 1) 
				{
					str_month += date_of_birth.at(i_go_to_the_point);
				}				
				
				if (point_counter == 2) 
				{
					str_day += date_of_birth.at(i_go_to_the_point);
				}

				i_go_to_the_point++;
			}
			i_go_to_the_point++;
			point_counter++;
		}
		i_go_through_the_characters++;
	}

	year = stoi(str_year);
	month = stoi(str_month);
	day = stoi(str_day);

	if (year < 1900 || year > 2020)
	{
		string yearCheck;
		do 
		{
			cout << "Entering the year is incorrect!\nPlease re-enter (1900-2020):";
			cin >> yearCheck;

		} while (stoi(yearCheck) < 1900 || stoi(yearCheck) > 2020);
		str_year = yearCheck;
	}

	if (month < 1 || month > 12)
	{
		string monthCheck;
		do
		{
			cout << "Entering the month is incorrect!\nPlease re-enter:";
			cin >> monthCheck;

		} while (stoi(monthCheck) < 1 || stoi(monthCheck) > 12);
		str_month = monthCheck;
	}

	if (day < 1 || day > 31)
	{
		string dayCheck;
		do
		{
			cout << "Entering the day is incorrect!\nPlease re-enter:";
			cin >> dayCheck;

		} while (stoi(dayCheck) < 1 || stoi(dayCheck) > 31);
		str_day = dayCheck;
	}

	string dateofbirth = str_year;
	dateofbirth += ".";
	dateofbirth += str_month;
	dateofbirth += ".";
	dateofbirth += str_day;
	dateofbirth += ".";

	return dateofbirth;
}