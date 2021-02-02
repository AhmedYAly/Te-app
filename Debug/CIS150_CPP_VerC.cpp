//Put to ignore security warnings
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif // _MSC_VER

#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <stdlib.h>
#include <Windows.h>
#include <ctime>
#include <conio.h>

//Constants 
#define nocardtypes 5				// number of different prepaid cards' values from 5£ to 100£
#define nocardspertype 4			// number of available cards per type

#define max_inbox_messages 5		// number of messages in message in-box per user

#define charge 0.25					// charge for calls & messages in £

using namespace std;


struct date
{
	int day;
	int month;
}today;

struct names
{
	string first;
	string second;
};

//structure that contains all the information about the customer's subscription type
struct subscription
{
	bool is_subscribed;

	int units;
	int package;

	date expirydate;
	date startdate;
};

//structure that contains all the information about the customer's messages and mailbox
struct message_format
{
	string context;

	bool is_read = true, has_text = false;

	int sender_in_database;
};

//structure that contains all the information about the customer's mobile number
struct phonenumber
{
	float balance;

	string number;
	string pass;

	subscription subscription_type;

	message_format message[max_inbox_messages];

	int id;
};


struct userinfo
{
	names name;

	string email;
	string address;

	phonenumber numbers[3];

	int numofownedlines;
}customer[5];

//structure containing the information of the prepaid cards for recharge
struct Prepaid_card
{
	int sn;

	bool used;
}prepaidcards[nocardtypes][nocardspertype];


// the position of the logged-in user and number in their arrays
int user_in_arr, number_in_arr;

// the time used in the calling functionality
int hrs = 0, mins = 0, secs = 0;


//secondary menus' flags
bool
loged = false,

call = false,
message = false,
transfer = false,
recharge = false,
plans = false,
user_info = false,

back = false,
exit_bool = false;


int login();

// the reading and generation of info used further in the application
void staticinformation();

void readfromfile(ifstream& file);
void generate(ofstream& file);
void units_management();
void mailbox_read();


void mainmenu(userinfo user);

// the function used to display the user information after being selected in the main menu
void user_information(userinfo user);

// the calling functionality
void start_call(float& balance, int& units);
int call_ongoing(int timer_fix, int max_mins, int receiver_name, int receiver_number);

// the messaging functionality
void messaging(float& balance, int& units);

// used to recharge the user's balance in £
void rechargeing(float& balance);

// the balance transfer functionality in £
void transfering(float& balance);

// the plans renewal, subscription, changing and unsubscribing
void plans_managing(float& balance, int& bundle, int& units);

// the saving of the data after the termination of the program
void data_management();
void subscription_data_saving();
void mailbox_save();

// where most functions are used
void logic();



void main()
{
	//setting console window size and position, no functionality, to look more like a mobile application
	HWND console_window = GetConsoleWindow();
	MoveWindow(console_window, 500, 25, 450, 650, TRUE);

	//getting the information
	staticinformation();
	mailbox_read();
	units_management();

	ifstream database_in("database.txt");

	//getting balances
	if (database_in.fail())				//if the data file does not exist. it set balances, passwords, names and ids
	{
		ofstream database_out;
		generate(database_out);
	}
	else								//if the data file does exist. it reads balances, passwords, names and ids from it
	{
		readfromfile(database_in);
	}

	system("CLS");


	while (!loged)
	{
		number_in_arr = login();
	}

	if (customer[user_in_arr].numbers[number_in_arr].subscription_type.is_subscribed)
	{
		if (customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.day <= today.day && customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.month <= today.month)
		{
			if (customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.day == today.day)
			{
				system("CLS");
				cout << endl << endl << endl << endl;
				cout << "\t\tYour Bundle expires today";
				this_thread::sleep_for(1750ms);
				system("CLS");
			}

			else
			{
				customer[user_in_arr].numbers[number_in_arr].subscription_type.is_subscribed = false;

				system("CLS");
				cout << endl << endl << endl << endl;
				cout << "\t\tYour Bundle has expired";
				this_thread::sleep_for(1750ms);
				system("CLS");
			}
		}
	}

	logic();
}


int login()
{
	string loginname, password;		//login elements

	cout << endl << "\t\t    My Cyber-TE" << endl;
	cout << endl << endl << endl << endl << "\t\t       Login" << endl << endl << endl;
	cout << endl << endl << endl << endl << "\n\n\n\n\n\n\n      In the format: xxxx Please enter\n\n\n\t   Mobile number: ";
	cin >> loginname;
	cout << endl << endl << "\t\tPassword: "; cin >> password;

	int j;

	// checking the login validity
	for (int i = 0; i < 5; i++)
	{
		for (j = 0; j < customer[i].numofownedlines; j++)
		{
			if (loginname == customer[i].numbers[j].number)
			{
				if (password == customer[i].numbers[j].pass)
				{
					loged = true;
					break;
				}

				if (loged)
				{
					break;
				}
			}
		}
		if (loged)
		{
			user_in_arr = i;
			break;
		}
	}
	if (!loged)
	{
		system("CLS");
		cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n Invalid Mobile number or password, please try again" << endl;
		this_thread::sleep_for(1750ms);
	}
	system("CLS");

	return j;
}


void staticinformation()
{
	//users' names ,numbers & passwords

	//customer 1
	customer[0].name.first = "Leigh";
	customer[0].name.second = "Gray";
	customer[0].email = "LeighGray@Sheriff.com";
	customer[0].address = "Caliga Hall, Rhodes, Scarlett Meadows, USA";
	customer[0].numofownedlines = 2;



	//1st number
	customer[0].numbers[0].number = "0000";
	customer[0].numbers[0].pass = "admin";
	customer[0].numbers[0].subscription_type.is_subscribed = true;
	customer[0].numbers[0].subscription_type.expirydate.day = 30;
	customer[0].numbers[0].subscription_type.expirydate.month = 6;
	customer[0].numbers[0].subscription_type.startdate.day = 30;
	customer[0].numbers[0].subscription_type.startdate.month = 5;
	customer[0].numbers[0].subscription_type.package = 2;

	//2nd number
	customer[0].numbers[1].number = "0001";
	customer[0].numbers[1].pass = "admin";
	customer[0].numbers[1].subscription_type.is_subscribed = false;



	//customer 2
	customer[1].name.first = "James";
	customer[1].name.second = "Fraser";
	customer[1].email = "lord_brochtuarach@highlander.com";
	customer[1].address = "Lallybroch, South Queensferry, Scotland, UK";
	customer[1].numofownedlines = 1;

	//1st number
	customer[1].numbers[0].number = "0010";
	customer[1].numbers[0].pass = "admin";
	customer[1].numbers[0].subscription_type.is_subscribed = true;
	customer[1].numbers[0].subscription_type.expirydate.day = 12;
	customer[1].numbers[0].subscription_type.expirydate.month = 6;
	customer[1].numbers[0].subscription_type.startdate.day = 12;
	customer[1].numbers[0].subscription_type.startdate.month = 5;
	customer[1].numbers[0].subscription_type.package = 4;



	//customer 3
	customer[2].name.first = "Anna";
	customer[2].name.second = "Henrietta";
	customer[2].email = "Anarietta_duqessa@nilfgaard.com";
	customer[2].address = "Beauclair Palace, Sansretour valley, Toussaint";
	customer[2].numofownedlines = 1;

	//1st number
	customer[2].numbers[0].number = "0011";
	customer[2].numbers[0].pass = "admin";
	customer[2].numbers[0].subscription_type.is_subscribed = true;
	customer[2].numbers[0].subscription_type.expirydate.day = 27;
	customer[2].numbers[0].subscription_type.expirydate.month = 6;
	customer[2].numbers[0].subscription_type.startdate.day = 27;
	customer[2].numbers[0].subscription_type.startdate.month = 5;
	customer[2].numbers[0].subscription_type.package = 3;



	//customer 4
	customer[3].name.first = "Geralt";
	customer[3].name.second = "Korin";
	customer[3].email = "GeraltofRivia@wiedzmin.com";
	customer[3].address = "Kaer Morhen, Gwenllech river banks, Kaedwen";
	customer[3].numofownedlines = 3;

	//1st number
	customer[3].numbers[0].number = "0100";
	customer[3].numbers[0].pass = "admin";
	customer[3].numbers[0].subscription_type.is_subscribed = true;
	customer[3].numbers[0].subscription_type.expirydate.day = 19;
	customer[3].numbers[0].subscription_type.expirydate.month = 6;
	customer[3].numbers[0].subscription_type.startdate.day = 19;
	customer[3].numbers[0].subscription_type.startdate.month = 5;
	customer[3].numbers[0].subscription_type.package = 5;

	//2nd number
	customer[3].numbers[1].number = "0101";
	customer[3].numbers[1].pass = "admin";
	customer[3].numbers[1].subscription_type.is_subscribed = true;
	customer[3].numbers[1].subscription_type.expirydate.day = 9;
	customer[3].numbers[1].subscription_type.expirydate.month = 6;
	customer[3].numbers[1].subscription_type.startdate.day = 9;
	customer[3].numbers[1].subscription_type.startdate.month = 5;
	customer[3].numbers[1].subscription_type.package = 3;

	//3rd number
	customer[3].numbers[2].number = "0110";
	customer[3].numbers[2].pass = "admin";
	customer[3].numbers[2].subscription_type.is_subscribed = false;



	//customer 5
	customer[4].name.first = "Jennifer";
	customer[4].name.second = "Scott";
	customer[4].email = "jennykenway@ubisoft.com";
	customer[4].address = "Bristol, UK";
	customer[4].numofownedlines = 2;

	//1st number
	customer[4].numbers[0].number = "0111";
	customer[4].numbers[0].pass = "admin";
	customer[4].numbers[0].subscription_type.is_subscribed = true;
	customer[4].numbers[0].subscription_type.expirydate.day = 28;
	customer[4].numbers[0].subscription_type.expirydate.month = 6;
	customer[4].numbers[0].subscription_type.startdate.day = 28;
	customer[4].numbers[0].subscription_type.startdate.month = 5;
	customer[4].numbers[0].subscription_type.package = 4;

	//2nd number
	customer[4].numbers[1].number = "1000";
	customer[4].numbers[1].pass = "admin";
	customer[4].numbers[1].subscription_type.is_subscribed = true;
	customer[4].numbers[1].subscription_type.expirydate.day = 11;
	customer[4].numbers[1].subscription_type.expirydate.month = 6;
	customer[4].numbers[1].subscription_type.startdate.day = 11;
	customer[4].numbers[1].subscription_type.startdate.month = 5;
	customer[4].numbers[1].subscription_type.package = 2;




	//getting Rechargeable Cards' SNs
	ifstream cards_in("prepaidcards.txt");

	if (cards_in.fail())
	{
		ofstream cards_out;
		cards_out.open("prepaidcards.txt");

		if (cards_out.is_open())
		{
			for (int i = 0; i < nocardtypes; i++)
			{
				for (int j = 0; j < nocardspertype; j++)
				{
					prepaidcards[i][j].sn = abs(((abs(rand()) * 9856)) % 10000000);

					if (i == 0 && j == 0)
					{
						prepaidcards[i][j].sn += 3000000;
					}
					cards_out << prepaidcards[i][j].sn << endl;
				}
			}
		}
		cards_out.close();
	}
	else
	{
		string restore;

		if (cards_in.is_open())
		{
			if (!cards_in.eof())
			{
				for (int i = 0; i < nocardtypes; i++)
				{
					for (int j = 0; j < nocardspertype; j++)
					{
						getline(cards_in, restore);
						prepaidcards[i][j].sn = atof(restore.c_str());
					}
				}
			}
		}
		cards_in.close();
	}
}

void readfromfile(ifstream& file)
{
	string restore;

	if (file.is_open())
	{
		if (!file.eof())
		{
			for (int i = 0; i < 5; i++)
			{
				for (int j = 0; j < customer[i].numofownedlines; j++)
				{
					getline(file, restore);
					customer[i].numbers[j].balance = atof(restore.c_str());
					customer[i].numbers[j].id = 35320 + i;
				}
			}
			for (int i = 0; i < nocardtypes; i++)
			{
				for (int j = 0; j < nocardspertype; j++)
				{
					getline(file, restore);
					prepaidcards[i][j].used = atof(restore.c_str());
				}
			}
		}
	}
	file.close();
}
void generate(ofstream& file)
{
	//creating a seed for randomized balance generation
	srand((unsigned)time(0));

	file.open("database.txt");

	if (file.is_open())
	{
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < customer[i].numofownedlines; j++)
			{
				customer[i].numbers[j].id = 35320 + i;
				customer[i].numbers[j].balance = rand() / 520;
				file << customer[i].numbers[j].balance << endl;
			}
		}
		for (int i = 0; i < nocardtypes; i++)
		{
			for (int j = 0; j < nocardspertype; j++)
			{
				prepaidcards[i][j].used = false;
				file << prepaidcards[i][j].used << endl;
			}
		}
	}
	file.close();

	//setting or getting Rechargeable Cards' SNs
	ifstream cards_in("prepaidcards.txt");

	if (cards_in.fail())
	{
		ofstream cards_out;
		cards_out.open("prepaidcards.txt");

		if (cards_out.is_open())
		{
			for (int i = 0; i < nocardtypes; i++)
			{
				for (int j = 0; j < nocardspertype; j++)
				{
					prepaidcards[i][j].sn = abs(((abs(rand()) * 9856)) % 10000000);

					if (i == 0 && j == 0)
					{
						prepaidcards[i][j].sn += 3000000;
					}
					cards_out << prepaidcards[i][j].sn << endl;
				}
			}
		}
		cards_out.close();
	}
	else
	{
		string restore;

		if (cards_in.is_open())
		{
			if (!cards_in.eof())
			{
				for (int i = 0; i < nocardtypes; i++)
				{
					for (int j = 0; j < nocardspertype; j++)
					{
						getline(cards_in, restore);
						prepaidcards[i][j].sn = atof(restore.c_str());
					}
				}
			}
		}
		cards_in.close();
	}
}
void units_management()
{
	ifstream units_amount("units.txt");

	string restore_units;

	if (units_amount.fail())
	{
		ofstream unit_save;
		unit_save.open("units.txt");

		if (unit_save.is_open())
		{
			for (int i = 0; i < 5; i++)
			{
				for (int j = 0; j < customer[i].numofownedlines; j++)
				{
					unit_save << customer[i].numbers[j].subscription_type.is_subscribed << endl;
				}
			}
			for (int i = 0; i < 5; i++)
			{
				for (int j = 0; j < customer[i].numofownedlines; j++)
				{
					unit_save << customer[i].numbers[j].subscription_type.package << endl;
				}
			}

			for (int i = 0; i < 5; i++)
			{
				for (int j = 0; j < customer[i].numofownedlines; j++)
				{
					if (customer[i].numbers[j].subscription_type.is_subscribed)
					{
						if (customer[i].numbers[j].subscription_type.package == 1)
						{
							customer[i].numbers[j].subscription_type.units = (rand() * 150) % 750;
						}
						else if (customer[i].numbers[j].subscription_type.package == 2)
						{
							customer[i].numbers[j].subscription_type.units = (rand() * 320) % 1250;
						}
						else if (customer[i].numbers[j].subscription_type.package == 3)
						{
							customer[i].numbers[j].subscription_type.units = (rand() * 435) % 2000;
						}
						else if (customer[i].numbers[j].subscription_type.package == 4)
						{
							customer[i].numbers[j].subscription_type.units = (rand() * 526) % 3800;
						}
						else if (customer[i].numbers[j].subscription_type.package == 5)
						{
							customer[i].numbers[j].subscription_type.units = (rand() * 728) % 4200;
						}
					}
					else
					{
						customer[i].numbers[j].subscription_type.units = 0;
					}

					unit_save << customer[i].numbers[j].subscription_type.units << endl;
				}
			}

			for (int i = 0; i < 5; i++)
			{
				for (int j = 0; j < customer[i].numofownedlines; j++)
				{
					unit_save << customer[i].numbers[j].subscription_type.expirydate.day << endl;
					unit_save << customer[i].numbers[j].subscription_type.expirydate.month << endl;

					unit_save << customer[i].numbers[j].subscription_type.startdate.day << endl;
					unit_save << customer[i].numbers[j].subscription_type.startdate.month << endl;
				}
			}
		}
		unit_save.close();
	}
	else
	{
		if (units_amount.is_open())
		{
			if (!units_amount.eof())
			{
				for (int i = 0; i < 5; i++)
				{
					for (int j = 0; j < customer[i].numofownedlines; j++)
					{
						getline(units_amount, restore_units);
						customer[i].numbers[j].subscription_type.is_subscribed = atof(restore_units.c_str());
					}
				}
				for (int i = 0; i < 5; i++)
				{
					for (int j = 0; j < customer[i].numofownedlines; j++)
					{
						getline(units_amount, restore_units);
						customer[i].numbers[j].subscription_type.package = atof(restore_units.c_str());
					}
				}

				for (int i = 0; i < 5; i++)
				{
					for (int j = 0; j < customer[i].numofownedlines; j++)
					{
						getline(units_amount, restore_units);
						customer[i].numbers[j].subscription_type.units = atof(restore_units.c_str());
					}
				}

				for (int i = 0; i < 5; i++)
				{
					for (int j = 0; j < customer[i].numofownedlines; j++)
					{
						getline(units_amount, restore_units);
						customer[i].numbers[j].subscription_type.expirydate.day = atof(restore_units.c_str());

						getline(units_amount, restore_units);
						customer[i].numbers[j].subscription_type.expirydate.month = atof(restore_units.c_str());

						getline(units_amount, restore_units);
						customer[i].numbers[j].subscription_type.startdate.day = atof(restore_units.c_str());

						getline(units_amount, restore_units);
						customer[i].numbers[j].subscription_type.startdate.month = atof(restore_units.c_str());
					}
				}
			}
		}
		units_amount.close();
	}
}
void mailbox_read()
{
	string restore;

	ifstream mail_box_sender;
	mail_box_sender.open("messages sender.txt");

	if (!mail_box_sender.fail())
	{
		if (mail_box_sender.is_open())
		{
			for (int i = 0; i < 5; i++)
			{
				for (int j = 0; j < customer[i].numofownedlines; j++)
				{
					for (int c = 0; c < max_inbox_messages; c++)
					{
						getline(mail_box_sender, restore);
						customer[i].numbers[j].message[c].sender_in_database = atof(restore.c_str());
					}
				}
			}
		}
	}
	mail_box_sender.close();

	ifstream mail_box_read;
	mail_box_read.open("messages read-flag.txt");

	if (!mail_box_read.fail())
	{
		if (mail_box_read.is_open())
		{
			for (int i = 0; i < 5; i++)
			{
				for (int j = 0; j < customer[i].numofownedlines; j++)
				{
					for (int c = 0; c < max_inbox_messages; c++)
					{
						getline(mail_box_read, restore);
						customer[i].numbers[j].message[c].is_read = atof(restore.c_str());
					}
				}
			}
		}
	}
	mail_box_read.close();

	ifstream mail_box_has_text;
	mail_box_has_text.open("messages has-text.txt");

	if (!mail_box_has_text.fail())
	{
		if (mail_box_has_text.is_open())
		{
			if (!mail_box_has_text.eof())
			{
				for (int i = 0; i < 5; i++)
				{
					for (int j = 0; j < customer[i].numofownedlines; j++)
					{
						for (int c = 0; c < max_inbox_messages; c++)
						{
							getline(mail_box_has_text, restore);

							if (restore == "1")
							{
								customer[i].numbers[j].message[c].has_text = true;
							}
							else
							{
								customer[i].numbers[j].message[c].has_text = false;
							}
						}
					}
				}
			}
		}
	}
	mail_box_has_text.close();

	ifstream mail_box_text;
	mail_box_text.open("messages text.txt");

	if (!mail_box_text.fail())
	{
		if (mail_box_text.is_open())
		{
			for (int i = 0; i < 5; i++)
			{
				for (int j = 0; j < customer[i].numofownedlines; j++)
				{
					for (int c = 0; c < max_inbox_messages; c++)
					{
						getline(mail_box_text, restore);
						customer[i].numbers[j].message[c].context = restore;
					}
				}
			}
		}
	}
	mail_box_text.close();
}


void mainmenu(userinfo user)
{
	int mainmenu_choise;

	time_t Time_ = time(0);
	struct tm* time_info = localtime(&Time_);

	today.day = time_info->tm_mday;
	today.month = time_info->tm_mon + 1;

	cout << endl << "\t\t    My Cyber-TE" << endl;
	cout << "\t\t       " << today.day << "/" << today.month;
	cout << endl << "\t       Welcome Back, ";

	if (user_in_arr == 0 || user_in_arr == 1 || user_in_arr == 3)
	{
		cout << "Mr. " << user.name.second;
	}
	else
	{
		cout << "Mrs. " << user.name.second;
	}

	cout << "\n\t\tCurrent balance:" << user.numbers[number_in_arr].balance;

	if (user.numbers[number_in_arr].subscription_type.is_subscribed)
	{
		cout << "\n\t\tRemaining units:" << user.numbers[number_in_arr].subscription_type.units;
	}

	cout << endl << endl << endl;
	cout << "\t\t   1) Make a call ";
	cout << endl << endl << "\t\t   2) Messages ";
	cout << endl << endl << "\t\t   3) Balance transfer ";
	cout << endl << endl << "\t\t   4) Recharge ";
	cout << endl << endl << "\t\t   5) Check your plan ";
	cout << endl << endl << "\t\t   6) User info. ";
	cout << endl << endl << "\t\t   0) Exit ";
	cout << endl << endl << "\n\n\t    What would you like to do? \n\t\t        - ";

	cin >> mainmenu_choise;

	if (mainmenu_choise == 0)
	{
		exit_bool = true;
	}
	else if (mainmenu_choise == 1)
	{
		call = true;
	}
	else if (mainmenu_choise == 2)
	{
		message = true;
	}
	else if (mainmenu_choise == 3)
	{
		transfer = true;
	}
	else if (mainmenu_choise == 4)
	{
		recharge = true;
	}
	else if (mainmenu_choise == 5)
	{
		plans = true;
	}
	else if (mainmenu_choise == 6)
	{
		user_info = true;
	}
	else
	{
		system("CLS");
		cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t   Invalid operation";
		this_thread::sleep_for(1525ms);
		system("CLS");
	}
}


void user_information(userinfo user)
{
	int goback;

	system("CLS");
	cout << endl << "\t\t    My Cyber-TE" << endl;
	cout << "\t\t       " << today.day << "/" << today.month;
	cout << endl << "\n\n\n   Full name: ";

	if (user_in_arr == 0 || user_in_arr == 1 || user_in_arr == 3)
	{
		cout << "Mr. " << user.name.first << " " << user.name.second;
	}
	else
	{
		cout << "Mrs. " << user.name.first << " " << user.name.second;
	}

	cout << "\n   Use ID : " << user.numbers[number_in_arr].id;

	cout << "\n   Current balance : " << user.numbers[number_in_arr].balance;

	if (user.numbers[number_in_arr].subscription_type.is_subscribed)
	{
		cout << "\n   Subscribed Bundle : ";

		if (user.numbers[number_in_arr].subscription_type.package == 1)
		{
			cout << "25 Bundle";
		}
		else if (user.numbers[number_in_arr].subscription_type.package == 2)
		{
			cout << "35 Bundle";
		}
		else if (user.numbers[number_in_arr].subscription_type.package == 3)
		{
			cout << "60 Bundle";
		}
		else if (user.numbers[number_in_arr].subscription_type.package == 4)
		{
			cout << "80 Bundle";
		}
		else if (user.numbers[number_in_arr].subscription_type.package == 5)
		{
			cout << "105 Bundle";
		}

		cout << "\n   Remaining units : " << user.numbers[number_in_arr].subscription_type.units;

		cout << "\n   Starting date : " << user.numbers[number_in_arr].subscription_type.startdate.day << "/" << user.numbers[number_in_arr].subscription_type.startdate.month;
		cout << "\n   Expiry date : " << user.numbers[number_in_arr].subscription_type.expirydate.day << "/" << user.numbers[number_in_arr].subscription_type.expirydate.month;
	}
	cout << "\n\n\n   e-mail   :\n   " << user.email;
	cout << "\n\n   address  :\n   " << user.address;
	cout << endl << endl << "\n\n\t    type any number to go back \n\t\t       - ";
	cin >> goback;

	transfer = false;
	back = true;
	system("CLS");
}


void start_call(float& balance, int& units)
{
	hrs = 0;
	mins = 0;
	secs = 0;

	bool call_started = false, self = false;

	system("CLS");
	cout << endl << "\t\t    My Cyber-TE" << endl;
	cout << endl << "\t       Welcome Back, ";

	string call_receiver;

	int max_call_mins = 0;

	if (customer[user_in_arr].numbers[number_in_arr].subscription_type.is_subscribed)			//maximum number of calling minutes for the current balance
	{
		max_call_mins = units;
	}
	else
	{
		max_call_mins = balance / 0.25;
	}

	if (customer[user_in_arr].numbers[number_in_arr].subscription_type.is_subscribed && max_call_mins == 0)			//handling previous operation expected logical errors
	{
		max_call_mins = balance / 0.25;
	}

	if (user_in_arr == 0 || user_in_arr == 1 || user_in_arr == 3)
	{
		cout << "Mr. " << customer[user_in_arr].name.second;
	}
	else
	{
		cout << "Mrs. " << customer[user_in_arr].name.second;
	}

	cout << "\n\t\tCurrent balance:" << customer[user_in_arr].numbers[number_in_arr].balance;

	if (customer[user_in_arr].numbers[number_in_arr].subscription_type.is_subscribed)
	{
		cout << "\n\t\tRemaining units:" << customer[user_in_arr].numbers[number_in_arr].subscription_type.units;
	}

	cout << endl << endl << endl << endl;
	cout << "\n\nEnter the number you want to call, or '0' to go back \n\t\t    -  ";

	cin >> call_receiver;

	int timer_fix = clock() / 1000;

	if (call_receiver != "0")
	{
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < customer[i].numofownedlines; j++)
			{
				if (call_receiver == customer[i].numbers[j].number)
				{
					if (call_receiver != customer[user_in_arr].numbers[number_in_arr].number)
					{
						call_started = true;

						if (customer[user_in_arr].numbers[number_in_arr].subscription_type.is_subscribed && customer[user_in_arr].numbers[number_in_arr].subscription_type.units > 0)
						{
							units -= call_ongoing(timer_fix, max_call_mins, i, j);
						}
						else
						{
							balance -= (call_ongoing(timer_fix, max_call_mins, i, j) * charge);
						}

						system("CLS");
						cout << endl << endl << endl << endl;
						cout << "\n\n\n\t\t    Call ended ";
						this_thread::sleep_for(1750ms);
						system("CLS");
					}
					else
					{
						self = true;

						system("CLS");
						cout << endl << endl << endl << endl;
						cout << "\n\n\n\t\t Cannot call yourself!";
						this_thread::sleep_for(1750ms);
						system("CLS");
					}
				}
				if (call_started)
				{
					break;
				}
			}
		}

		if (!call_started && !self)
		{
			system("CLS");
			cout << endl << endl << endl << endl;
			cout << "\n\n\n\t\t This number does not exist ";
			this_thread::sleep_for(1750ms);
			system("CLS");
		}
	}
	else
	{
		call = false;
		back = true;
		system("CLS");
	}
}
int call_ongoing(int timer_fix, int max_mins, int receiver_name, int receiver_number)
{
	int call_time_in_mins = 0;

	while (!_kbhit())
	{
		secs = ((clock() / 1000) - timer_fix) % 61;
		this_thread::sleep_for(1000ms);

		if (secs > 59)
		{
			mins++;
			secs = 0;
			call_time_in_mins++;
		}
		if (mins > 59)
		{
			hrs++;
			mins = 0;
		}

		if (call_time_in_mins >= max_mins)
		{
			break;
		}

		system("CLS");
		cout << endl << "\t\t    My Cyber-TE" << endl;
		cout << "\n\n\n\t\t    " << customer[receiver_name].name.first << " " << customer[receiver_name].name.second << "\n\n\t\t\t" << customer[receiver_name].numbers[receiver_number].number << "\n\n\n\n\t\t\t" << hrs << ":" << mins << ":" << secs << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t     press any key to end the call";
	}
	system("CLS");

	if (secs > 0)
	{
		call_time_in_mins++;
	}

	return call_time_in_mins;
}


void messaging(float& balance, int& units)
{
back:

	string message_text, receiver;

	bool receiver_exists = false, new_message_written = false;

	int sub_menu, new_messages = 0;
	int i, j;

	//interface
	system("CLS");
	cout << endl << "\t\t    My Cyber-TE" << endl;
	cout << endl << "\t       Welcome Back, ";

	if (user_in_arr == 0 || user_in_arr == 1 || user_in_arr == 3)
	{
		cout << "Mr. " << customer[user_in_arr].name.second;
	}
	else
	{
		cout << "Mrs. " << customer[user_in_arr].name.second;
	}

	cout << endl << endl << endl << endl;
	cout << endl << endl << endl << endl;
	cout << "\t\t   1) Compose a message ";
	cout << endl << endl << "\t\t   2) Check in-box";

	for (int c = 0; c < max_inbox_messages; c++)
	{
		if (!customer[user_in_arr].numbers[number_in_arr].message[c].is_read)
		{
			new_messages++;
		}
	}
	if (new_messages > 0)
	{
		cout << "*";
	}

	cout << endl << endl << "\t\t   0) Exit ";
	cout << endl << endl;
	cout << endl << endl;
	cout << endl << endl << "\n\n\t    What would you like to do? \n\t\t        - ";

	cin >> sub_menu;

	if (sub_menu == 1)
	{
		system("CLS");
		cout << endl << "\t\t    My Cyber-TE" << endl;
		cout << endl << "\t      Welcome Back, ";

		if (user_in_arr == 0 || user_in_arr == 1 || user_in_arr == 3)
		{
			cout << "Mr. " << customer[user_in_arr].name.second;
		}
		else
		{
			cout << "Mrs. " << customer[user_in_arr].name.second;
		}

		cout << endl << endl << endl << endl;
		cout << "Enter the number you would like to send a message to\n\n";
		cout << "To : ";

		cin >> receiver;

		cout << "- write your message here, or '0' to go back. \n- ";

		getline(cin >> ws, message_text);

		if (message_text != "0")
		{
			for (i = 0; i < 5; i++)
			{
				for (j = 0; j < customer[i].numofownedlines; j++)
				{
					if (customer[i].numbers[j].number == receiver)
					{
						receiver_exists = true;
						break;
					}
				}

				if (receiver_exists)
				{
					break;
				}
			}
			if (receiver_exists)
			{
				if (customer[user_in_arr].numbers[number_in_arr].number == receiver)
				{
					system("CLS");
					cout << endl << endl << endl << endl;
					cout << "\n\n\n\n\n\n\t  you cannot send messages to yourself! ";
					this_thread::sleep_for(1750ms);
					system("CLS");
				}
				else
				{
					for (int c = 0; c < max_inbox_messages; c++)
					{
						if (!customer[i].numbers[j].message[c].has_text)
						{
							customer[i].numbers[j].message[c].context = message_text;
							new_message_written = true;
							customer[i].numbers[j].message[c].is_read = false;
							customer[i].numbers[j].message[c].has_text = true;
							customer[i].numbers[j].message[c].sender_in_database = user_in_arr;
							if (customer[user_in_arr].numbers[number_in_arr].subscription_type.is_subscribed && customer[user_in_arr].numbers[number_in_arr].subscription_type.units > 0)
							{
								units--;
							}
							else
							{
								balance -= charge;
							}
							break;
						}
					}
					if (!new_message_written)
					{
						for (int c = 0; c < max_inbox_messages; c++)
						{
							if (customer[i].numbers[j].message[c].is_read)
							{
								customer[i].numbers[j].message[c].context = message_text;
								new_message_written = true;
								customer[i].numbers[j].message[c].is_read = false;
								customer[i].numbers[j].message[c].has_text = true;
								customer[i].numbers[j].message[c].sender_in_database = user_in_arr;
								if (customer[user_in_arr].numbers[number_in_arr].subscription_type.is_subscribed && customer[user_in_arr].numbers[number_in_arr].subscription_type.units > 0)
								{
									units--;
								}
								else
								{
									balance -= charge;
								}
								break;
							}
						}
					}

					system("CLS");
					cout << endl << endl << endl << endl;
					cout << "\n\n\n\n\n\n\t\t Successful operation";
					this_thread::sleep_for(1750ms);
					system("CLS");
				}
			}
			else
			{
				system("CLS");
				cout << endl << endl << endl << endl;
				cout << "\n\n\n\t\t Receiver does not exist ";
				this_thread::sleep_for(1750ms);
				system("CLS");
			}
		}
		else if (message_text == "0")
		{
			goto back;
			system("CLS");
		}
		else
		{

			system("CLS");
			cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t   Invalid operation";
			this_thread::sleep_for(1525ms);
			system("CLS");
		}
	}
	else if (sub_menu == 2)
	{
	back_one:

		int message_check;

		system("CLS");
		cout << endl << "\t\t    My Cyber-TE" << endl;
		cout << endl << "\t      Welcome Back, ";

		if (user_in_arr == 0 || user_in_arr == 1 || user_in_arr == 3)
		{
			cout << "Mr. " << customer[user_in_arr].name.second;
		}
		else
		{
			cout << "Mrs. " << customer[user_in_arr].name.second;
		}

		cout << endl << endl << endl << endl;
		cout << endl << endl << endl << endl;

		int no_available_messages = 0;
		int message_with_text[5];
		int x = 0, new_messages = 0;

		for (int c = 0; c < max_inbox_messages; c++)
		{
			if (customer[user_in_arr].numbers[number_in_arr].message[c].has_text)
			{
				no_available_messages++;
				message_with_text[x] = c;
				x++;

				cout << "\t   " << no_available_messages << ") From: " << customer[customer[user_in_arr].numbers[number_in_arr].message[c].sender_in_database].name.second << ", " << customer[customer[user_in_arr].numbers[number_in_arr].message[c].sender_in_database].name.first;

				if (!customer[user_in_arr].numbers[number_in_arr].message[c].is_read)
				{
					cout << " *New!";
					new_messages++;
				}

				cout << endl << endl;
			}
		}

		if (no_available_messages > 0)
		{
			cout << endl << endl;
			cout << endl << endl << "\n\n     Which message would you like to check? \n\t\t        - ";

			cin >> message_check;
			if (message_check <= no_available_messages)
			{
				if (message_check == 1)
				{
					new_messages--;
					customer[user_in_arr].numbers[number_in_arr].message[message_with_text[0]].is_read = true;

					system("CLS");
					cout << endl << "\t\t    My Cyber-TE" << endl;
					cout << endl << "\t      Welcome Back, ";

					if (user_in_arr == 0 || user_in_arr == 1 || user_in_arr == 3)
					{
						cout << "Mr. " << customer[user_in_arr].name.second;
					}
					else
					{
						cout << "Mrs. " << customer[user_in_arr].name.second;
					}

					cout << endl << endl << endl << endl;
					cout << endl << endl << endl << endl;

					cout << "From: " << customer[customer[user_in_arr].numbers[number_in_arr].message[message_with_text[0]].sender_in_database].name.first << " " << customer[customer[user_in_arr].numbers[number_in_arr].message[message_with_text[0]].sender_in_database].name.second << endl;
					cout << "- " << customer[user_in_arr].numbers[number_in_arr].message[message_with_text[0]].context << endl << endl;
					cout << "\t\t Enter '0' to go back\n\t\t       - ";
					cin >> message_check;

					if (message_check == 0)
					{
						system("CLS");
						goto back_one;
					}
				}
				else if (message_check == 2)
				{
					new_messages--;
					customer[user_in_arr].numbers[number_in_arr].message[message_with_text[1]].is_read = true;

					system("CLS");
					cout << endl << "\t\t    My Cyber-TE" << endl;
					cout << endl << "\t      Welcome Back, ";

					if (user_in_arr == 0 || user_in_arr == 1 || user_in_arr == 3)
					{
						cout << "Mr. " << customer[user_in_arr].name.second;
					}
					else
					{
						cout << "Mrs. " << customer[user_in_arr].name.second;
					}

					cout << endl << endl << endl << endl;
					cout << endl << endl << endl << endl;

					cout << "From: " << customer[customer[user_in_arr].numbers[number_in_arr].message[message_with_text[1]].sender_in_database].name.first << " " << customer[customer[user_in_arr].numbers[number_in_arr].message[message_with_text[1]].sender_in_database].name.second << endl;
					cout << "- " << customer[user_in_arr].numbers[number_in_arr].message[message_with_text[1]].context << endl << endl;
					cout << "\t\t Enter '0' to go back\n\t\t       - ";
					cin >> message_check;

					if (message_check == 0)
					{
						goto back_one;
					}
				}
				else if (message_check == 3)
				{
					new_messages--;
					customer[user_in_arr].numbers[number_in_arr].message[message_with_text[2]].is_read = true;

					system("CLS");
					cout << endl << "\t\t    My Cyber-TE" << endl;
					cout << endl << "\t      Welcome Back, ";

					if (user_in_arr == 0 || user_in_arr == 1 || user_in_arr == 3)
					{
						cout << "Mr. " << customer[user_in_arr].name.second;
					}
					else
					{
						cout << "Mrs. " << customer[user_in_arr].name.second;
					}

					cout << endl << endl << endl << endl;
					cout << endl << endl << endl << endl;

					cout << "From: " << customer[customer[user_in_arr].numbers[number_in_arr].message[message_with_text[2]].sender_in_database].name.first << " " << customer[customer[user_in_arr].numbers[number_in_arr].message[message_with_text[2]].sender_in_database].name.second << endl;
					cout << "- " << customer[user_in_arr].numbers[number_in_arr].message[message_with_text[2]].context << endl << endl;
					cout << "\t\t Enter '0' to go back\n\t\t       - ";
					cin >> message_check;

					if (message_check == 0)
					{
						goto back_one;
					}
				}
				else if (message_check == 4)
				{
					new_messages--;
					customer[user_in_arr].numbers[number_in_arr].message[message_with_text[3]].is_read = true;

					system("CLS");
					cout << endl << "\t\t    My Cyber-TE" << endl;
					cout << endl << "\t      Welcome Back, ";

					if (user_in_arr == 0 || user_in_arr == 1 || user_in_arr == 3)
					{
						cout << "Mr. " << customer[user_in_arr].name.second;
					}
					else
					{
						cout << "Mrs. " << customer[user_in_arr].name.second;
					}

					cout << endl << endl << endl << endl;
					cout << endl << endl << endl << endl;

					cout << "From: " << customer[customer[user_in_arr].numbers[number_in_arr].message[message_with_text[3]].sender_in_database].name.first << " " << customer[customer[user_in_arr].numbers[number_in_arr].message[message_with_text[3]].sender_in_database].name.second << endl;
					cout << "- " << customer[user_in_arr].numbers[number_in_arr].message[message_with_text[3]].context << endl << endl;
					cout << "\t\t Enter '0' to go back\n\t\t       - ";
					cin >> message_check;

					if (message_check == 0)
					{
						goto back_one;
					}
				}
				else if (message_check == 5)
				{
					new_messages--;
					customer[user_in_arr].numbers[number_in_arr].message[message_with_text[4]].is_read = true;

					system("CLS");
					cout << endl << "\t\t    My Cyber-TE" << endl;
					cout << endl << "\t      Welcome Back, ";

					if (user_in_arr == 0 || user_in_arr == 1 || user_in_arr == 3)
					{
						cout << "Mr. " << customer[user_in_arr].name.second;
					}
					else
					{
						cout << "Mrs. " << customer[user_in_arr].name.second;
					}

					cout << endl << endl << endl << endl;
					cout << endl << endl << endl << endl;

					cout << "From: " << customer[customer[user_in_arr].numbers[number_in_arr].message[message_with_text[4]].sender_in_database].name.first << " " << customer[customer[user_in_arr].numbers[number_in_arr].message[message_with_text[4]].sender_in_database].name.second << endl;
					cout << "- " << customer[user_in_arr].numbers[number_in_arr].message[message_with_text[4]].context << endl << endl;
					cout << "\t\t Enter '0' to go back\n\t\t       - ";
					cin >> message_check;

					if (message_check == 0)
					{
						system("CLS");
						goto back_one;
					}
				}
				else if (message_check == 0)
				{
					goto back;
					system("CLS");
				}
			}
		}
		else
		{
			cout << endl << endl;
			cout << endl << endl << "\n\n\t\t  No new messages\n\n";
			this_thread::sleep_for(1750ms);
		}
	}
	else if (sub_menu == 0)
	{
		message = false;
		back = true;
		system("CLS");
	}
	else
	{
		system("CLS");
		cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t   Invalid operation";
		this_thread::sleep_for(1525ms);
		system("CLS");
	}
}


void rechargeing(float& balance)
{
	int recharge_temp = 0;
	int card_type;

	bool exists = false;

	//interface
	system("CLS");
	cout << endl << "\t\t    My Cyber-TE" << endl;
	cout << endl << "\t       Welcome Back, ";

	if (user_in_arr == 0 || user_in_arr == 1 || user_in_arr == 3)
	{
		cout << "Mr. " << customer[user_in_arr].name.second;
	}
	else
	{
		cout << "Mrs. " << customer[user_in_arr].name.second;
	}

	cout << endl << endl << endl << endl;
	cout << " Please note that the card SN consists of 9 numbers\n\n";
	cout << "      Enter your prepaid card's serial number,\n\t\tor '0' to go back \n\t\t - ";
	cin >> recharge_temp;

	if (recharge_temp != 0)
	{
		int j = 0;
		int i = 0;

		for (i = 0; i < nocardtypes; i++)
		{
			for (j = 0; j < nocardspertype; j++)
			{
				if (recharge_temp == prepaidcards[i][j].sn)
				{
					exists = true;
					card_type = i;
					break;
				}
			}
			if (exists)
			{
				break;
			}
		}

		if (exists && !prepaidcards[i][j].used)
		{
			if (card_type == 0)
			{
				balance += 5;
			}
			if (card_type == 1)
			{
				balance += 10;
			}
			if (card_type == 2)
			{
				balance += 25;
			}
			if (card_type == 3)
			{
				balance += 50;
			}
			if (card_type == 4)
			{
				balance += 100;
			}
			prepaidcards[i][j].used = true;

			system("CLS");
			cout << "\n\n\n\n\n\n\n\  Successful operation, current balance: " << balance;
			this_thread::sleep_for(1575ms);
			system("CLS");
		}
		else if (exists && prepaidcards[i][j].used)
		{
			system("CLS");
			cout << "\n\n\n\n\n\n\n  this card was already used, please try another.";
			this_thread::sleep_for(1575ms);
			system("CLS");
		}
		else
		{
			system("CLS");
			cout << "\n\n\n\n\n\n\n\t  card number is not valid, please retry.";
			this_thread::sleep_for(1575ms);
			system("CLS");
		}
	}
	else
	{
		recharge = false;
		back = true;
		system("CLS");
	}
}


void transfering(float& balance)
{
	float transfer_amount;

	string receiver;

	bool receiver_exists = false;

	system("CLS");
	system("CLS");
	cout << endl << "\t\t    My Cyber-TE" << endl;
	cout << endl << "\t       Welcome Back, ";

	if (user_in_arr == 0 || user_in_arr == 1 || user_in_arr == 3)
	{
		cout << "Mr. " << customer[user_in_arr].name.second;
	}
	else
	{
		cout << "Mrs. " << customer[user_in_arr].name.second;
	}

	cout << "\n\t\tCurrent balance:" << customer[user_in_arr].numbers[number_in_arr].balance;

	cout << endl << endl << endl << endl;
	cout << "Enter your the number you wish to transfer money to,\n\t\tor '0' to go back \n\t\t - ";
	cin >> receiver;

	if (receiver != "0")
	{
		int i, j;
		for (i = 0; i < 5; i++)
		{
			for (j = 0; j < customer[i].numofownedlines; j++)
			{
				if (customer[i].numbers[j].number == receiver)
				{
					receiver_exists = true;
					cout << "     Enter your the amount you wish to transfer\n\n-Please note, you must have at least 2LE remaining- \n\t\t     - ";
					break;
				}
			}

			if (receiver_exists)
			{
				break;
			}
		}
		if (receiver_exists)
		{
			cin >> transfer_amount;
		}
		else
		{
			system("CLS");
			cout << endl << endl << endl << endl;
			cout << "\t\t   This number does not exist ";
			this_thread::sleep_for(1750ms);
			system("CLS");
		}

		if (receiver_exists)
		{
			if (balance - 2 >= transfer_amount)
			{
				balance -= transfer_amount;
				customer[i].numbers[j].balance += transfer_amount;

				system("CLS");
				cout << endl << endl << endl << endl;
				cout << "\t\tSuccessful operation";
				this_thread::sleep_for(1750ms);
				system("CLS");
			}
			else
			{
				system("CLS");
				cout << endl << endl << endl << endl;
				cout << "\t\t   Unacceptable amount! ";
				this_thread::sleep_for(1750ms);
				system("CLS");
			}
		}
	}
	else
	{
		transfer = false;
		back = true;
		system("CLS");
	}
}


void plans_managing(float& balance, int& bundle, int& units)
{
	int choice;

	string confirmation;

back_one_up:

	if (customer[user_in_arr].numbers[number_in_arr].subscription_type.is_subscribed)
	{
		system("CLS");
		cout << endl << "\t\t    My Cyber-TE" << endl;
		cout << endl << "\t       Welcome Back, ";

		if (user_in_arr == 0 || user_in_arr == 1 || user_in_arr == 3)
		{
			cout << "Mr. " << customer[user_in_arr].name.second;
		}
		else
		{
			cout << "Mrs. " << customer[user_in_arr].name.second;
		}

		cout << endl << endl << "\t\t Expiry date:" << customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.day << "/" << customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.month << endl;
		cout << "\t      Current Package: ";

		if (bundle == 1)
		{
			cout << "25 Bundle";
		}
		else if (bundle == 2)
		{
			cout << "35 Bundle";
		}
		else if (bundle == 3)
		{
			cout << "60 Bundle";
		}
		else if (bundle == 4)
		{
			cout << "80 Bundle";
		}
		else if (bundle == 5)
		{
			cout << "105 Bundle";
		}


		if (customer[user_in_arr].numbers[number_in_arr].subscription_type.is_subscribed)
		{
			cout << "\n\t\tRemaining units:" << units;
		}

		cout << endl << endl << endl << endl;
		cout << "\t\t   1) Renew package ";
		cout << endl << endl << "\t\t   2) Check other packages ";
		cout << endl << endl << "\t\t   3) Unsubscribe ";
		cout << endl << endl << "\t\t   0) Exit ";
		cout << endl << endl << "\n\n\t    What would you like to do? \n\t\t        - ";
		cin >> choice;

		if (choice != 0)
		{
			if (choice == 1)
			{
				system("CLS");
				cout << endl << "\t\t    My Cyber-TE" << endl;
				cout << endl << "\t       Welcome Back, ";

				if (user_in_arr == 0 || user_in_arr == 1 || user_in_arr == 3)
				{
					cout << "Mr. " << customer[user_in_arr].name.second;
				}
				else
				{
					cout << "Mrs. " << customer[user_in_arr].name.second;
				}

				cout << "\n\n\t   Current Package: ";

				if (bundle == 1)
				{
					cout << "25 Bundle";
				}
				else if (bundle == 2)
				{
					cout << "35 Bundle";
				}
				else if (bundle == 3)
				{
					cout << "60 Bundle";
				}
				else if (bundle == 4)
				{
					cout << "80 Bundle";
				}
				else if (bundle == 5)
				{
					cout << "105 Bundle";
				}

				cout << endl << endl << endl << endl;
				cout << "\n\n\t    Please type 'confirm' to proceed \n\t\t       - ";
				cin >> confirmation;

				if (confirmation == "confirm")
				{
					if (bundle == 1)
					{
						if (balance >= 25)
						{
							units = 750;
							balance -= 25;

							customer[user_in_arr].numbers[number_in_arr].subscription_type.startdate.day = today.day;
							customer[user_in_arr].numbers[number_in_arr].subscription_type.startdate.month = today.month;

							customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.day = today.day;
							customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.month = today.month + 1;

							system("CLS");
							cout << endl << endl << endl << endl;
							cout << "\t\tSuccessful operation";
							this_thread::sleep_for(1750ms);
							system("CLS");
						}
						else
						{
							system("CLS");
							cout << endl << endl << endl << endl;
							cout << "\t\tInsufficient balance!";
							this_thread::sleep_for(1750ms);
							system("CLS");
						}
					}
					else if (bundle == 2)
					{
						if (balance >= 35)
						{
							units = 1250;
							balance -= 35;

							customer[user_in_arr].numbers[number_in_arr].subscription_type.startdate.day = today.day;
							customer[user_in_arr].numbers[number_in_arr].subscription_type.startdate.month = today.month;

							customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.day = today.day;
							customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.month = today.month + 1;

							system("CLS");
							cout << endl << endl << endl << endl;
							cout << "\t\tSuccessful operation";
							this_thread::sleep_for(1750ms);
							system("CLS");
						}
						else
						{
							system("CLS");
							cout << endl << endl << endl << endl;
							cout << "\t\tInsufficient balance!";
							this_thread::sleep_for(1750ms);
							system("CLS");
						}
					}
					else if (bundle == 3)
					{
						if (balance >= 60)
						{
							units = 2000;
							balance -= 60;

							customer[user_in_arr].numbers[number_in_arr].subscription_type.startdate.day = today.day;
							customer[user_in_arr].numbers[number_in_arr].subscription_type.startdate.month = today.month;

							customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.day = today.day;
							customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.month = today.month + 1;

							system("CLS");
							cout << endl << endl << endl << endl;
							cout << "\t\tSuccessful operation";
							this_thread::sleep_for(1750ms);
							system("CLS");
						}
						else
						{
							system("CLS");
							cout << endl << endl << endl << endl;
							cout << "\t\tInsufficient balance!";
							this_thread::sleep_for(1750ms);
							system("CLS");
						}
					}
					else if (bundle == 4)
					{
						if (balance >= 80)
						{
							units = 3800;
							balance -= 80;

							customer[user_in_arr].numbers[number_in_arr].subscription_type.startdate.day = today.day;
							customer[user_in_arr].numbers[number_in_arr].subscription_type.startdate.month = today.month;

							customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.day = today.day;
							customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.month = today.month + 1;

							system("CLS");
							cout << endl << endl << endl << endl;
							cout << "\t\tSuccessful operation";
							this_thread::sleep_for(1750ms);
							system("CLS");
						}
						else
						{
							system("CLS");
							cout << endl << endl << endl << endl;
							cout << "\t\tInsufficient balance!";
							this_thread::sleep_for(1750ms);
							system("CLS");
						}
					}
					else if (bundle == 5)
					{
						if (balance >= 105)
						{
							units = 420;
							balance -= 105;

							customer[user_in_arr].numbers[number_in_arr].subscription_type.startdate.day = today.day;
							customer[user_in_arr].numbers[number_in_arr].subscription_type.startdate.month = today.month;

							customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.day = today.day;
							customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.month = today.month + 1;

							system("CLS");
							cout << endl << endl << endl << endl;
							cout << "\t\tSuccessful operation";
							this_thread::sleep_for(1750ms);
							system("CLS");
						}
						else
						{
							system("CLS");
							cout << endl << endl << endl << endl;
							cout << "\t\tInsufficient balance!";
							this_thread::sleep_for(1750ms);
							system("CLS");
						}
					}
				}
				else
				{
					system("CLS");
					cout << endl << endl << endl << endl;
					cout << "\t\tOperation failed";
					this_thread::sleep_for(1750ms);
					system("CLS");

					goto back_one_up;
				}
			}
			else if (choice == 2)
			{
			back_once:

				system("CLS");
				cout << endl << "\t\t    My Cyber-TE" << endl;
				cout << endl << "\t       Welcome Back, ";

				if (user_in_arr == 0 || user_in_arr == 1 || user_in_arr == 3)
				{
					cout << "Mr. " << customer[user_in_arr].name.second;
				}
				else
				{
					cout << "Mrs. " << customer[user_in_arr].name.second;
				}

				cout << "\n\n\t      Current Package: ";

				if (bundle == 1)
				{
					cout << "25 Bundle";
				}
				else if (bundle == 2)
				{
					cout << "35 Bundle";
				}
				else if (bundle == 3)
				{
					cout << "60 Bundle";
				}
				else if (bundle == 4)
				{
					cout << "80 Bundle";
				}
				else if (bundle == 5)
				{
					cout << "105 Bundle";
				}

				cout << endl << endl << endl << endl;
				cout << "\t\t1) 25 Bundle -> 750 units ";
				cout << endl << endl << "\t\t2) 35 Bundle -> 1250 units";
				cout << endl << endl << "\t\t3) 60 Bundle -> 2000 units";
				cout << endl << endl << "\t\t4) 80 Bundle -> 3800 units";
				cout << endl << endl << "\t\t5) 105 Bundle -> 4200 units";
				cout << endl << endl << "\t\t0) Exit ";
				cout << endl << endl << "\n\n\t    What would you like to do? \n\t\t        - ";
				cin >> choice;

				if (choice == 1)
				{
					if (balance >= 25)
					{
						units = 750;
						bundle = 1;
						balance -= 25;

						customer[user_in_arr].numbers[number_in_arr].subscription_type.startdate.day = today.day;
						customer[user_in_arr].numbers[number_in_arr].subscription_type.startdate.month = today.month;

						customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.day = today.day;
						customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.month = today.month + 1;

						system("CLS");
						cout << endl << endl << endl << endl;
						cout << "\t\tSuccessful operation";
						this_thread::sleep_for(1750ms);
						system("CLS");
					}
					else
					{
						system("CLS");
						cout << endl << endl << endl << endl;
						cout << "\t\tInsufficient balance!";
						this_thread::sleep_for(1750ms);
						system("CLS");
					}
				}
				else if (choice == 2)
				{
					if (balance >= 35)
					{
						units = 1250;
						bundle = 2;
						balance -= 35;

						customer[user_in_arr].numbers[number_in_arr].subscription_type.startdate.day = today.day;
						customer[user_in_arr].numbers[number_in_arr].subscription_type.startdate.month = today.month;

						customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.day = today.day;
						customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.month = today.month + 1;

						system("CLS");
						cout << endl << endl << endl << endl;
						cout << "\t\tSuccessful operation";
						this_thread::sleep_for(1750ms);
						system("CLS");
					}
					else
					{
						system("CLS");
						cout << endl << endl << endl << endl;
						cout << "\t\tInsufficient balance!";
						this_thread::sleep_for(1750ms);
						system("CLS");
					}
				}
				else if (choice == 3)
				{
					if (balance >= 60)
					{
						units = 2000;
						bundle = 3;
						balance -= 60;

						customer[user_in_arr].numbers[number_in_arr].subscription_type.startdate.day = today.day;
						customer[user_in_arr].numbers[number_in_arr].subscription_type.startdate.month = today.month;

						customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.day = today.day;
						customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.month = today.month + 1;

						system("CLS");
						cout << endl << endl << endl << endl;
						cout << "\t\tSuccessful operation";
						this_thread::sleep_for(1750ms);
						system("CLS");
					}
					else
					{
						system("CLS");
						cout << endl << endl << endl << endl;
						cout << "\t\tInsufficient balance!";
						this_thread::sleep_for(1750ms);
						system("CLS");
					}
				}
				else if (choice == 4)
				{
					if (balance >= 80)
					{
						units = 3800;
						bundle = 4;
						balance -= 80;

						customer[user_in_arr].numbers[number_in_arr].subscription_type.startdate.day = today.day;
						customer[user_in_arr].numbers[number_in_arr].subscription_type.startdate.month = today.month;

						customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.day = today.day;
						customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.month = today.month + 1;

						system("CLS");
						cout << endl << endl << endl << endl;
						cout << "\t\tSuccessful operation";
						this_thread::sleep_for(1750ms);
						system("CLS");
					}
					else
					{
						system("CLS");
						cout << endl << endl << endl << endl;
						cout << "\t\tInsufficient balance!";
						this_thread::sleep_for(1750ms);
						system("CLS");
					}
				}
				else if (choice == 5)
				{
					if (balance >= 105)
					{
						units = 4200;
						bundle = 5;
						balance -= 105;

						customer[user_in_arr].numbers[number_in_arr].subscription_type.startdate.day = today.day;
						customer[user_in_arr].numbers[number_in_arr].subscription_type.startdate.month = today.month;

						customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.day = today.day;
						customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.month = today.month + 1;

						system("CLS");
						cout << endl << endl << endl << endl;
						cout << "\t\tSuccessful operation";
						this_thread::sleep_for(1750ms);
						system("CLS");
					}
					else
					{
						system("CLS");
						cout << endl << endl << endl << endl;
						cout << "\t\tInsufficient balance!";
						this_thread::sleep_for(1750ms);
						system("CLS");
					}

				}
				else if (choice == 0)
				{
					goto back_one_up;
				}
				else
				{
					system("CLS");
					cout << endl << endl << endl << endl;
					cout << "\t\tInvalid operation!";
					this_thread::sleep_for(1750ms);
					system("CLS");

					goto back_once;
				}
			}
			else if (choice == 3)
			{
				system("CLS");
				cout << endl << "\t\t    My Cyber-TE" << endl;
				cout << endl << "\t       Welcome Back, ";

				if (user_in_arr == 0 || user_in_arr == 1 || user_in_arr == 3)
				{
					cout << "Mr. " << customer[user_in_arr].name.second;
				}
				else
				{
					cout << "Mrs. " << customer[user_in_arr].name.second;
				}

				cout << endl << endl << endl << endl;
				cout << "\n\n\t    Please type 'confirm' to proceed \n\t\t       - ";
				cin >> confirmation;

				if (confirmation == "confirm")
				{
					customer[user_in_arr].numbers[number_in_arr].subscription_type.is_subscribed = false;

					system("CLS");
					cout << endl << endl << endl << endl;
					cout << "\t\tSuccessful operation";
					this_thread::sleep_for(1750ms);
					system("CLS");

					plans = false;
					back = true;
				}
				else
				{
					system("CLS");
					cout << endl << endl << endl << endl;
					cout << "\t\tOperation failed";
					this_thread::sleep_for(1750ms);
					system("CLS");

					goto back_one_up;
				}
			}
			else
			{
				system("CLS");
				cout << endl << endl << endl << endl;
				cout << "\t\tInvalid operation!";
				this_thread::sleep_for(1750ms);
				system("CLS");
			}
		}
		else
		{
			plans = false;
			back = true;
			system("CLS");
		}
	}
	else
	{
		system("CLS");
		cout << endl << "\t\t    My Cyber-TE" << endl;
		cout << endl << "\t       Welcome Back, ";

		if (user_in_arr == 0 || user_in_arr == 1 || user_in_arr == 3)
		{
			cout << "Mr. " << customer[user_in_arr].name.second;
		}
		else
		{
			cout << "Mrs. " << customer[user_in_arr].name.second;
		}

		cout << endl << endl << endl << endl;
		cout << "\t\t   1) Check available packages ";
		cout << endl << endl << "\t\t   0) Exit ";
		cout << endl << endl << "\n\n\t    What would you like to do? \n\t\t        - ";
		cin >> choice;

		if (choice != 0)
		{
			if (choice == 1)
			{
				system("CLS");
				cout << endl << "\t\t    My Cyber-TE" << endl;
				cout << endl << "\t       Welcome Back, ";

				if (user_in_arr == 0 || user_in_arr == 1 || user_in_arr == 3)
				{
					cout << "Mr. " << customer[user_in_arr].name.second;
				}
				else
				{
					cout << "Mrs. " << customer[user_in_arr].name.second;
				}

				cout << endl << endl << endl << endl;
				cout << "\t\t1) 25 Bundle -> 750 units ";
				cout << endl << endl << "\t\t2) 35 Bundle -> 1250 units";
				cout << endl << endl << "\t\t3) 60 Bundle -> 2000 units";
				cout << endl << endl << "\t\t4) 80 Bundle -> 3800 units";
				cout << endl << endl << "\t\t5) 105 Bundle -> 4200 units";
				cout << endl << endl << "\t\t0) Exit ";
				cout << endl << endl << "\n\n\t    What would you like to do? \n\t\t        - ";
				cin >> choice;

				if (choice == 1)
				{
					if (balance >= 25)
					{
						units = 750;
						bundle = 1;
						balance -= 25;
						customer[user_in_arr].numbers[number_in_arr].subscription_type.is_subscribed = true;

						customer[user_in_arr].numbers[number_in_arr].subscription_type.startdate.day = today.day;
						customer[user_in_arr].numbers[number_in_arr].subscription_type.startdate.month = today.month;

						customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.day = today.day;
						customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.month = today.month + 1;

						system("CLS");
						cout << endl << endl << endl << endl;
						cout << "\t\tSuccessful operation";
						this_thread::sleep_for(1750ms);
						system("CLS");
					}
					else
					{
						system("CLS");
						cout << endl << endl << endl << endl;
						cout << "\t\tInsufficient balance!";
						this_thread::sleep_for(1750ms);
						system("CLS");
					}
				}
				else if (choice == 2)
				{
					if (balance >= 35)
					{
						units = 1250;
						bundle = 2;
						balance -= 35;
						customer[user_in_arr].numbers[number_in_arr].subscription_type.is_subscribed = true;

						customer[user_in_arr].numbers[number_in_arr].subscription_type.startdate.day = today.day;
						customer[user_in_arr].numbers[number_in_arr].subscription_type.startdate.month = today.month;

						customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.day = today.day;
						customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.month = today.month + 1;

						system("CLS");
						cout << endl << endl << endl << endl;
						cout << "\t\tSuccessful operation";
						this_thread::sleep_for(1750ms);
						system("CLS");
					}
					else
					{
						system("CLS");
						cout << endl << endl << endl << endl;
						cout << "\t\tInsufficient balance!";
						this_thread::sleep_for(1750ms);
						system("CLS");
					}
				}
				else if (choice == 3)
				{
					if (balance >= 60)
					{
						units = 2000;
						bundle = 3;
						balance -= 60;
						customer[user_in_arr].numbers[number_in_arr].subscription_type.is_subscribed = true;

						customer[user_in_arr].numbers[number_in_arr].subscription_type.startdate.day = today.day;
						customer[user_in_arr].numbers[number_in_arr].subscription_type.startdate.month = today.month;

						customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.day = today.day;
						customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.month = today.month + 1;

						system("CLS");
						cout << endl << endl << endl << endl;
						cout << "\t\tSuccessful operation";
						this_thread::sleep_for(1750ms);
						system("CLS");
					}
					else
					{
						system("CLS");
						cout << endl << endl << endl << endl;
						cout << "\t\tInsufficient balance!";
						this_thread::sleep_for(1750ms);
						system("CLS");
					}
				}
				else if (choice == 4)
				{
					if (balance >= 80)
					{
						units = 3800;
						bundle = 4;
						balance -= 80;
						customer[user_in_arr].numbers[number_in_arr].subscription_type.is_subscribed = true;

						customer[user_in_arr].numbers[number_in_arr].subscription_type.startdate.day = today.day;
						customer[user_in_arr].numbers[number_in_arr].subscription_type.startdate.month = today.month;

						customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.day = today.day;
						customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.month = today.month + 1;

						system("CLS");
						cout << endl << endl << endl << endl;
						cout << "\t\tSuccessful operation";
						this_thread::sleep_for(1750ms);
						system("CLS");
					}
					else
					{
						system("CLS");
						cout << endl << endl << endl << endl;
						cout << "\t\tInsufficient balance!";
						this_thread::sleep_for(1750ms);
						system("CLS");
					}
				}
				else if (choice == 5)
				{
					if (balance >= 105)
					{
						units = 4200;
						bundle = 5;
						balance -= 105;
						customer[user_in_arr].numbers[number_in_arr].subscription_type.is_subscribed = true;

						customer[user_in_arr].numbers[number_in_arr].subscription_type.startdate.day = today.day;
						customer[user_in_arr].numbers[number_in_arr].subscription_type.startdate.month = today.month;

						customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.day = today.day;
						customer[user_in_arr].numbers[number_in_arr].subscription_type.expirydate.month = today.month + 1;

						system("CLS");
						cout << endl << endl << endl << endl;
						cout << "\t\tSuccessful operation";
						this_thread::sleep_for(1750ms);
						system("CLS");
					}
					else
					{
						system("CLS");
						cout << endl << endl << endl << endl;
						cout << "\t\tInsufficient balance!";
						this_thread::sleep_for(1750ms);
						system("CLS");
					}
				}
				else if (choice == 0)
				{
					goto back_one_up;
				}
				else
				{
					system("CLS");
					cout << endl << endl << endl << endl;
					cout << "\t\tInvalid operation!";
					this_thread::sleep_for(1750ms);
					system("CLS");
				}
			}
			else
			{
				system("CLS");
				cout << endl << endl << endl << endl;
				cout << "\t\tInvalid operation!";
				this_thread::sleep_for(1750ms);
				system("CLS");
			}
		}
		else
		{
			plans = false;
			back = true;
			system("CLS");
		}
	}
}


void data_management()
{
	ofstream database_rewrite;
	database_rewrite.open("database.txt");

	if (!database_rewrite.fail())
	{
		for (int i = 0; i < 5; i++)
		{
			int j = 0;
			while (j < customer[i].numofownedlines)
			{
				database_rewrite << customer[i].numbers[j].balance << endl;

				j++;
			}
		}
		for (int i = 0; i < nocardtypes; i++)
		{
			for (int j = 0; j < nocardspertype; j++)
			{
				database_rewrite << prepaidcards[i][j].used << endl;
			}
		}
		database_rewrite.close();
	}
}
void subscription_data_saving()
{
	ofstream unit_save;
	unit_save.open("units.txt");

	if (unit_save.is_open())
	{
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < customer[i].numofownedlines; j++)
			{
				unit_save << customer[i].numbers[j].subscription_type.is_subscribed << endl;
			}
		}
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < customer[i].numofownedlines; j++)
			{
				unit_save << customer[i].numbers[j].subscription_type.package << endl;
			}
		}

		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < customer[i].numofownedlines; j++)
			{
				if (customer[i].numbers[j].subscription_type.is_subscribed)
				{
					if (customer[i].numbers[j].subscription_type.package == 1)
					{
						customer[i].numbers[j].subscription_type.units = (rand() * 150) % 750;
					}
					else if (customer[i].numbers[j].subscription_type.package == 2)
					{
						customer[i].numbers[j].subscription_type.units = (rand() * 320) % 1250;
					}
					else if (customer[i].numbers[j].subscription_type.package == 3)
					{
						customer[i].numbers[j].subscription_type.units = (rand() * 435) % 2000;
					}
					else if (customer[i].numbers[j].subscription_type.package == 4)
					{
						customer[i].numbers[j].subscription_type.units = (rand() * 526) % 3800;
					}
					else if (customer[i].numbers[j].subscription_type.package == 5)
					{
						customer[i].numbers[j].subscription_type.units = (rand() * 728) % 4200;
					}
				}
				else
				{
					customer[i].numbers[j].subscription_type.units = 0;
				}

				unit_save << customer[i].numbers[j].subscription_type.units << endl;
			}
		}

		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < customer[i].numofownedlines; j++)
			{
				if (customer[i].numbers[j].subscription_type.is_subscribed)
				{
					unit_save << customer[i].numbers[j].subscription_type.expirydate.day << endl;
					unit_save << customer[i].numbers[j].subscription_type.expirydate.month << endl;

					unit_save << customer[i].numbers[j].subscription_type.startdate.day << endl;
					unit_save << customer[i].numbers[j].subscription_type.startdate.month << endl;
				}
				else
				{
					unit_save << "0" << endl;
					unit_save << "0" << endl;
				}
			}
		}
	}
	unit_save.close();
}
void mailbox_save()
{
	ofstream mail_box_sender;
	mail_box_sender.open("messages sender.txt");

	if (mail_box_sender.is_open())
	{
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < customer[i].numofownedlines; j++)
			{
				for (int c = 0; c < max_inbox_messages; c++)
				{
					mail_box_sender << customer[i].numbers[j].message[c].sender_in_database << endl;
				}
			}
		}
	}
	mail_box_sender.close();

	ofstream mail_box_read;
	mail_box_read.open("messages read-flag.txt");

	if (mail_box_read.is_open())
	{
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < customer[i].numofownedlines; j++)
			{
				for (int c = 0; c < max_inbox_messages; c++)
				{
					mail_box_read << customer[i].numbers[j].message[c].is_read << endl;
				}
			}
		}
	}
	mail_box_read.close();

	ofstream mail_box_has_text;
	mail_box_has_text.open("messages has-text.txt");

	if (mail_box_has_text.is_open())
	{
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < customer[i].numofownedlines; j++)
			{
				for (int c = 0; c < max_inbox_messages; c++)
				{
					mail_box_has_text << customer[i].numbers[j].message[c].has_text << endl;
				}
			}
		}
	}
	mail_box_has_text.close();

	ofstream mail_box_text;
	mail_box_text.open("messages text.txt");

	if (mail_box_text.is_open())
	{
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < customer[i].numofownedlines; j++)
			{
				for (int c = 0; c < max_inbox_messages; c++)
				{
					if (customer[i].numbers[j].message[c].has_text)
					{
						mail_box_text << customer[i].numbers[j].message[c].context << endl;
					}
					else
					{
						mail_box_text << 0 << endl;
					}
				}
			}
		}
	}
	mail_box_text.close();
}


void logic()
{
	//main menu
menu_helper:

	mainmenu(customer[user_in_arr]);

	if (exit_bool)
	{
		goto end_of_logic;
	}
	else if (!call && !transfer && !message && !recharge && !plans && !user_info)
	{
		goto menu_helper;
	}

	//recharging
	while (recharge)
	{
		rechargeing(customer[user_in_arr].numbers[number_in_arr].balance);

		if (back)
		{
			back = false;
			goto menu_helper;
		}
	}

	//balance transfer
	while (transfer)
	{
		transfering(customer[user_in_arr].numbers[number_in_arr].balance);

		if (back)
		{
			back = false;
			goto menu_helper;
		}
	}

	//messaging
	while (message)
	{
		messaging(customer[user_in_arr].numbers[number_in_arr].balance, customer[user_in_arr].numbers[number_in_arr].subscription_type.units);
		if (back)
		{
			back = false;
			goto menu_helper;
		}
	}

	//calling
	while (call)
	{
		start_call(customer[user_in_arr].numbers[number_in_arr].balance, customer[user_in_arr].numbers[number_in_arr].subscription_type.units);
		if (back)
		{
			back = false;
			goto menu_helper;
		}
	}

	//plan management
	while (plans)
	{
		plans_managing(customer[user_in_arr].numbers[number_in_arr].balance, customer[user_in_arr].numbers[number_in_arr].subscription_type.package, customer[user_in_arr].numbers[number_in_arr].subscription_type.units);
		if (back)
		{
			back = false;
			goto menu_helper;
		}
	}

	//user info

	while (user_info)
	{
		user_information(customer[user_in_arr]);
		if (back)
		{
			back = false;
			goto menu_helper;
		}
	}

end_of_logic:

	data_management();
	mailbox_save();
	subscription_data_saving();
}
