#include<iostream.h>
#include<fstream.h>
#include<conio.h>
#include<stdio.h>
#include<string.h>


//This structure is used to store and retrieve Employee data
struct EmployeeData
{
        char empcode[5];
        char empname[30];
        char password[9];
        char phone[12];
        char address[80];
        int CL;
        int SL;
        int EL;
};

//This structure is used to store and retrieve Employee leave data
struct LeaveData
{
        char empcode[5];
        char empname[30];
        char phone[12];
        char fromdate[11];
        char todate[11];
        int  leavesapplied;
        char leavetype[4];

};

//This class is used to operate on Employee records
class employee
{
        private:
                EmployeeData empdata;
                fstream iofile;
	        char todate[11];
	        char fromdate[11];
                int leavesApplied;
        public:
               employee()                   //constructor
               {
                        leavesApplied=0;
               }

	       ~employee()                 //destructor
               {
			iofile.close();
	       }
	       int CheckEmployee(char * code, char * pwd);
               void searchcode();
	       void searchname();
               void FormatText(char *dest, char *src, int count);
	       void employee_master();
               void LeaveHistory();
               void LeaveBalance();
	       void modifyleave();
               void LeaveCancel();
               void empLeaveHistory();
	       void form();

};

// This function will add right spacing to format output
void rightspaces(int count)
{
        for(int i=0; i<count; i++)
        {
                cout<<" ";
        }
}

void enterpassword(char * pass)
{
	char a;
	int i=0;
	while(1)
	{
	     a=getch();
	     // if user enters character or numbers
	     if((a >='a'&& a <= 'z')||(a>='A' && a <='Z')||(a>='0' && a <= '9'))
	     {
	       pass[i]=a;
	       ++i;
	       cout<<"*";
	     }
	     //if user enter back space then remove the character from password
	     if(a == '\b' && i>=1)
	     {
		cout<<"\b \b";// rub the character behind the cursor
		--i;
	     }

	     //if enter is pressed
	     if(a == '\r')
	     {
		pass[i]='\0';
		break;

	     }
	}
}

// This functions checks the validity of the employee or admin
int employee::CheckEmployee(char * code, char * pwd)
{
	iofile.open("EmplDB.dat", ios::in|ios::binary);
	if(!iofile)
	{
		clrscr();
		cout <<"Cannot Open File !!\n";  //file does not exist
		return 0;

	}

	while(!iofile.eof())
	{
		//Retrieve employee record for login
		iofile.read((char *) &empdata, sizeof(empdata));
		if((strcmp(empdata.empcode, code)==0) && (strcmp(empdata.password,pwd)==0))
		{
		   //If employee code and password matches,then login successful
		   iofile.close();
		   return 1;
		}
	}
	iofile.close();  //Invalid login
	return 0;
}

// Leave Application Form
void employee::form()
{
	int choice,ch1,leave;
	fstream leaveFile;
	long filePOS;
	EmployeeData tmpemp;
	LeaveData leaverecord;
	clrscr();
	cout<<"\n\n\n\n\t\t\t LEAVE APPLICATION FORM ";
	cout<<"\n\t\t\t~~~~~~~~~~~~~~~~~~~~~~~~";
	cout<<"\n\n\tEmployee Code:"<<empdata.empcode;
	cout<<"\t\tEmployee Name:"<<empdata.empname;
	cout<<"\n\tPhone No.:"<<empdata.phone;
	cout<<"\t\tAddress:"<<empdata.address;
	cout<<"\n\nLEAVE STATUS:"<<endl;
	cout<<"\n1.No.of Casual leave in hand:"<<empdata.CL<<endl;
	cout<<"2.No.of Sick leave in hand:"<<empdata.SL<<endl;
	cout<<"3.No.of Earned leave in hand:"<<empdata.EL<<endl;
	cout<<"\nWhat type of leave would you like to apply?(1,2,3)";
	cin>>choice;
	cout<<"\nFrom date(DD/MM/YYYY):";
	cin>>fromdate;
	cout<<"To date(DD/MM/YYYY):";
	cin>>todate;
	cout<<"\nNumber of leaves applied: ";
	cin>>leavesApplied;
	cout<<"\n\n1.Apply"<<"\t\t2.Cancel";
	cout<<"\nEnter your choice(1,2): ";
	cin>>ch1;
	strcpy(leaverecord.empcode,empdata.empcode);
	strcpy(leaverecord.empname,empdata.empname);
	strcpy(leaverecord.phone,empdata.phone);
	strcpy(leaverecord.fromdate,fromdate);
	strcpy(leaverecord.todate,todate);
	leaverecord.leavesapplied=leavesApplied;
	if(ch1==1)
	{       //Kind of Leaves Applied
		switch(choice)
		{
			case 1:leave=empdata.CL-leavesApplied;
			       if(leave<0)
			       {
					cout<<"\nInsufficient leave";
					break;
			       }
			       empdata.CL=leave;
			       strcpy(leaverecord.leavetype,"CL");
			       break;
			case 2:leave=empdata.SL-leavesApplied;
			       if(leave<0)
			       {
					cout<<"\nInsufficient leave";
					break;
			       }
			       empdata.SL=leave;
			       strcpy(leaverecord.leavetype,"SL");
			       break;
			case 3:leave=empdata.EL-leavesApplied;
			       if(leave<0)
			       {
					cout<<"\nInsufficient leave";
					break;
			       }
			       empdata.EL=leave;
			       strcpy(leaverecord.leavetype,"EL");
			       break;
			default:cout<<"Input is not defined";
		}
		//Storing balance leaves into the file
		iofile.open("EmplDB.dat", ios::in|ios::out|ios::binary);
		while(!iofile.eof())
		{
			filePOS=iofile.tellg();  //Current Position of File pointer
			iofile.read((char *) &tmpemp, sizeof(tmpemp));
			if(strcmp(tmpemp.empcode, empdata.empcode)==0)
			{       //Pointer back to starting of the matched record
				iofile.seekg(filePOS);
				iofile.write((char *) &empdata, sizeof(empdata));
			}
		}
		iofile.close();
		//Opening file LeaveDB for storing the leave information
		leaveFile.open("LeaveDB.dat",ios::app|ios::binary);
		leaveFile.write((char *) &leaverecord, sizeof(leaverecord));
		leaveFile.close();
	}


}

void employee::LeaveBalance()
{
	clrscr();
	cout<<"\n\n\t\t\t\t LEAVE BALANCE";
	cout<<"\n\t\t\t\t~~~~~~~~~~~~~~~";
	cout<<"\n\n\n\t\t1.Balanced Casual leaves:"<<empdata.CL<<endl;
	cout<<"\n\n\t\t2.Balanced Sick leaves:"<<empdata.SL<<endl;
	cout<<"\n\n\t\t3.Balanced Earned leaves:"<<empdata.EL<<endl;
	cout<<"\n\n\n\t\tPress any key to continue....";
	getch();
}


void employee::empLeaveHistory()
{
	clrscr();
	int spacecount;
	LeaveData leave;
	fstream leavefile;
	leavefile.open("LeaveDB.dat", ios::in|ios::binary);
	cout<<"\n";
	cout<<"ECode "<<"Employee Name                  "<<"Phone No.  "<<"From Date  "<<"To Date    "<<"Days "<<"Type"<<endl;
	cout<<"----- "<<"------------------------------ "<<"---------- "<<"---------- "<<"---------- "<<"---- "<<"----"<<endl;
	while(leavefile.read((char *) &leave, sizeof(leave)))
	{
		if(strcmp(leave.empcode, empdata.empcode)==0)
		{
			cout<<"\n";
			cout<<leave.empcode;
			cout<<"  ";
			cout<<leave.empname;
			rightspaces(31-strlen(leave.empname));
			cout<<leave.phone;
			rightspaces(11-strlen(leave.phone));
			cout<<leave.fromdate;
			cout<<" ";
			cout<<leave.todate;
			cout<<"  ";
			cout<<leave.leavesapplied;
			cout<<"    ";
			cout<<leave.leavetype;
			cout<<endl;
		}
	}
	cout<<"\n\n\nPress any key to continue.....";
	getch();
}

void employee::LeaveCancel()
{
	clrscr();
	cout<<"\n\n\t\t\t LEAVE CANCELLATION ";
	cout<<"\n\t\t\t~~~~~~~~~~~~~~~~~~~~";
	LeaveData leave, leave1;
	fstream leavefile;
	ofstream tempfile;
	long lastleavePOS, filePOS;
	EmployeeData tmpemp;
	tempfile.open("TempDB.dat",ios::out|ios::binary);
	leavefile.open("LeaveDB.dat", ios::in|ios::binary);
	while(leavefile.read((char *) &leave, sizeof(leave)))
	{
	     lastleavePOS=leavefile.tellg();
	     if(strcmp(leave.empcode, empdata.empcode)==0)
	     {
		  continue;
	     }

	}
	leavefile.close();
	leavefile.open("LeaveDB.dat", ios::in|ios::binary);
	while(!leavefile.eof())
	{
		leavefile.read((char *) &leave1, sizeof(leave1));
		if(lastleavePOS!=leavefile.tellg())
		{
			tempfile.write((char*)&leave1,sizeof(leave1));
		}
	}
	remove("LeaveDB.dat");
	rename("TempDB.dat","LeaveDB.dat");
	tempfile.close();
	leavefile.close();
	iofile.open("EmplDB.dat", ios::in|ios::out|ios::binary);
	while(!iofile.eof())
	{
		filePOS=iofile.tellg();  //Current Position of File pointer
		iofile.read((char *) &tmpemp, sizeof(tmpemp));
		if(strcmp(tmpemp.empcode, empdata.empcode)==0)
		{       //Pointer back to starting of the matched record
			iofile.seekg(filePOS);
			if(strcmp(leave.leavetype, "CL")==0)
			   empdata.CL=empdata.CL+leave.leavesapplied;
			else if(strcmp(leave.leavetype, "SL")==0)
			   empdata.SL=empdata.SL+leave.leavesapplied;
			else if(strcmp(leave.leavetype, "EL")==0)
			   empdata.EL=empdata.EL+leave.leavesapplied;
			iofile.write((char *) &empdata, sizeof(empdata));
		}
	}
	iofile.close();

		cout<<"\n\n\t\t\tYOUR LAST LEAVE HAS BEEN CANCELLED";
		cout<<"\nPress any key to continue";
		getch();

}

// This function is used to assign leaves by admin user
void employee::employee_master()
{
	clrscr();
	int flag=0;
	fstream iofile;
	EmployeeData e2;
	char ch='y';
	do
	{
		cout<<"\n\nEnter the Employee Code : ";
		gets(empdata.empcode);
		cout<<"\n\nEnter the Employee Name : ";
		gets(empdata.empname);
		cout<<"\n\nSet Password:";
		gets(empdata.password);
		cout<<"\n\nEnter mobile number:";
		gets(empdata.phone);
		cout<<"\nEnter address:";
		gets(empdata.address);
		cout<<"\n\nEnter the Casual Leaves : ";
		cin>>empdata.CL;
		cout<<"\n\nEnter the Sick Leaves : ";
		cin>>empdata.SL;
		cout<<"\n\nEnter the Earned Leaves : ";
		cin>>empdata.EL;
		iofile.open("EmplDB.dat",ios::binary|ios::in);
		while(iofile.read((char*)&e2,sizeof(e2)))
		{
			if(strcmp(e2.empcode,empdata.empcode)==0)
			{
				flag=1;
				break;

			}
		}
		iofile.close();
		if(flag==0)
		{
			iofile.open("EmplDB.dat",ios::binary|ios::in|ios::app);
			iofile.write((char*)&empdata,sizeof(empdata));
			cout<<"\nRecord added successfully";
			iofile.close();
		}
		else
			cout<<"\nEmployee code already exists.Please try again...";

		cout<<"\nDo you want to enter more records(Y/N)?";
		cin>>ch;
		clrscr();
	}while(ch=='Y'||ch=='y');

}

//This function will show Leave history to admin user
void employee::LeaveHistory()
{
	fstream leavefile;
	LeaveData leaverecord;
	int spacecount;
	clrscr();
	leavefile.open("LeaveDB.dat", ios::in|ios::binary);
	if(!leavefile)
	{
		clrscr();
		cout <<"LeaveDB File Not Found !!\n";  //file does not exist

	}
	else
	{
	       cout<<"\n";
	       cout<<"ECode "<<"Employee Name                  "<<"Phone No.  "<<"From Date  "<<"To Date    "<<"Days "<<"Type"<<endl;
	       cout<<"----- "<<"------------------------------ "<<"---------- "<<"---------- "<<"---------- "<<"---- "<<"----"<<endl;
	       while(leavefile.read((char *)&leaverecord, sizeof(leaverecord)))
		{
			cout<<"\n";
			cout<<leaverecord.empcode;
			cout<<"  ";
			cout<<leaverecord.empname;
			rightspaces(31-strlen(leaverecord.empname));
			cout<<leaverecord.phone;
			rightspaces(11-strlen(leaverecord.phone));
			cout<<leaverecord.fromdate;
			cout<<" ";
			cout<<leaverecord.todate;
			cout<<"  ";
			cout<<leaverecord.leavesapplied;
			cout<<"    ";
			cout<<leaverecord.leavetype;
			cout<<endl;
		}
		cout<<"\n\n\nPress any key to continue.....";
		getch();
	}
}

// This is Leave Menu
void EmployeeLeave_menu(employee *emp)
{
	clrscr();
	int ch;
	cout<<"\n\n\n\n\t\t\t\t LEAVE MENU";
	cout<<"\n\t\t\t\t~~~~~~~~~~~~";
	cout<<"\n\n\n\t\t1.Leave Application";
	cout<<"\n\n\t\t2.Leave Cancellation";
	cout<<"\n\n\t\t3.Leave Balance";
	cout<<"\n\n\t\t4.Leave History";
	cout<<"\n\n\t\t5.Exit";
	cout<<"\n\n\n\t\tEnter your choice:";
	cin>>ch;
	switch(ch)
	{
		case 1:emp->form();break;
		case 2:emp->LeaveCancel(); break;
		case 3:emp->LeaveBalance();break;
		case 4:emp->empLeaveHistory();break;
		case 5:break;
		default:cout<<"Wrong choice!";
	}
}


// This function is used for login
void login(employee * emp)
{
	char empcode[5];
	char password[11];
	char ch1='y';
	do
	{
		clrscr();
		cout<<"\n\n\n\n\n\n\t\t\t\t   LOGIN";
		cout<<"\n\t\t\t\t  ~~~~~~~";
		cout<<"\n\n\n\t\t\t Enter Employee code: ";
		cin>>empcode;
		cout<<"\n\t\t\t Enter Password     : ";
		enterpassword(password);
		if(emp->CheckEmployee(empcode, password)==1)
		{
			cout<<"\n\n\n\t\tYou are logged in successfully, Press any key...";
			getch();
			EmployeeLeave_menu(emp);
			break;
		}
		else

		{
			cout<<"\n\n\n\t\tIncorrect Employee code or password...Try again(Y/N)?:";
			cin>>ch1;
		}
	}while(ch1=='y'||ch1=='Y');

}


// this function offers choices as to what the admin can do
void admintask()
{
	int choice;
	char ch3='y';
	employee e1;
	do
	{
	cout<<"\n\n\n\n\t\t\t\t ADMIN MENU";
	cout<<"\n\t\t\t\t~~~~~~~~~~~~";
	cout<<"\n\n\t\t1)Search by Employee Code";
	cout<<"\n\n\t\t2)Search by Employee Name";
	cout<<"\n\n\t\t3)View leave history";
	cout<<"\n\n\t\t4)Assign leaves to existing employee";
	cout<<"\n\n\t\t5)Create new employee";
	cout<<"\n\n\n\t\tEnter your choice:";
	cin>>choice;
	switch(choice)
	{
		case 1:	e1.searchcode();
			break;
		case 2:e1.searchname();break;
		case 3:e1.LeaveHistory();break;
		case 4:e1.modifyleave();break;
		case 5:e1.employee_master(); break;
		default:cout<<"Not a valid choice";

	}
	cout<<"\nDo you want to keep logged as an admin?";
	cin>>ch3;
	clrscr();
	}while(ch3=='y'||ch3=='Y');
}

// this function is used to check the validity of admin
void adminmenu()
{
	char adminid[15],adminpass[9],ch2='y';
	char tempid[15],temppass[9];
	strcpy(adminid,"admin");
	strcpy(adminpass,"aj");
	do
	{
		cout<<"\n\n\n\n\n\n\t\t\t\t  ADMIN LOGIN";
		cout<<"\n\t\t\t\t  ~~~~~~~~~~~";
		cout<<"\n\n\n\t\t\t Enter User ID: ";
		gets(tempid);
		cout<<"\n\t\t\t Enter Password     : ";
		enterpassword(temppass);
		if((strcmp(tempid,adminid)||strcmp(temppass,adminpass))==0)
		{
				cout<<"\n\t\t\tyou are logged in successfully, Press any key...";
				getch();
				clrscr();
				admintask();
				break;

		}
		else
		{
			cout<<"Incorrect user id or password...Try again";
		}
			cout<<"\nDo you want try again(y/n)??";
			cin>>ch2;
	}while(ch2=='y'||ch2=='Y');
}


//this function is used to allocate/reassign leaves to an employee
void employee::modifyleave()
{
	fstream iofile;
	char sname[30];
	iofile.open("EmplDB.dat",ios::binary|ios::in|ios::out);
	int flag=0,pos;
	clrscr();
	cout<<"Enter the employee name you want to search:";
	gets(sname);

	while(!iofile.eof())
	{
		pos=iofile.tellg();
		iofile.read((char*)&empdata,sizeof(empdata));
		if(strcmpi(empdata.empname,sname)==0)
		{
			flag=1;
			iofile.seekg(pos);
			cout<<"Record found";
			cout<<"\n\nEnter the Casual Leaves : ";
			cin>>empdata.CL;
			// empdata.CL=empdatas.CL;
			cout<<"\n\nEnter the Sick Leaves : ";
			cin>>empdata.SL;
			// empdata.SL=empdatas.SL;
			cout<<"\n\nEnter the Earned Leaves : ";
			cin>>empdata.EL;             //	empdata.EL=empdatas.EL;
			iofile.write((char*)&empdata,sizeof(empdata));
			cout<<"RECORD UPDATED";
			break;
		}
	}
	if(flag==0)
	{
		cout<<"Employee does not exist";
	}
	iofile.close();
}

//this function is used to search by employee code
void employee::searchcode()
{

	fstream leavefile;
	fstream iosearch;
	char scode[5];
	LeaveData leaverecord;
	int spacecount;
	int flag=0,i=0;
	clrscr();
	cout<<"Enter the employee code you want to search:";
	gets(scode);
	leavefile.open("LeaveDB.dat", ios::in|ios::binary);

	while(leavefile.read((char *)&leaverecord, sizeof(leaverecord)))
	{
		if(strcmp(leaverecord.empcode,scode)==0)
		{
			i++;
			if(i==1)
			{
				cout<<"\nrecord found!!!"<<endl;
				cout<<"ECode "<<"Employee Name                  "<<"Phone No.  "<<"From Date  "<<"To Date    "<<"Days "<<"Type"<<endl;
				cout<<"----- "<<"------------------------------ "<<"---------- "<<"---------- "<<"---------- "<<"---- "<<"----"<<endl;
			}
			cout<<"\n";
			cout<<leaverecord.empcode;
			cout<<"  ";
			cout<<leaverecord.empname;
			rightspaces(31-strlen(leaverecord.empname));
			cout<<leaverecord.phone;
			rightspaces(11-strlen(leaverecord.phone));
			cout<<leaverecord.fromdate;
			cout<<" ";
			cout<<leaverecord.todate;
			cout<<"  ";
			cout<<leaverecord.leavesapplied;
			cout<<"    ";
			cout<<leaverecord.leavetype;
			cout<<endl;
			flag =1;
		}
	}
	leavefile.close();
	if(flag==0)
	{
		iofile.open("EmplDB.dat",ios::in|ios::binary);
		int flag1=0;
		while(iofile.read((char*)&empdata,sizeof(empdata)))
		{
			if(strcmp(empdata.empcode,scode)==0)
			{
				cout<<"\nEmployee exists but has not taken a leave yet.";
				cout<<"\nDETAILS OF THE EMPLOYEE ARE:";
				cout<<"\nEmployee Code:"<<empdata.empcode;
				cout<<"\nEmployee Name:"<<empdata.empname;
				cout<<"\nEMployee mobile number:"<<empdata.phone;
				flag1=1;
				break;
			}
		}
		iofile.close();
		if(flag1==0)
			cout<<"Employee does not exist";
	}

}

//this function is used to search employee by name
void employee::searchname()
{
	fstream leavefile;
	fstream iosearch;
	char sname[30];
	LeaveData leaverecord;
	int spacecount;
	int flag=0,i=0;
	clrscr();
	cout<<"Enter the employee name you want to search:";
	gets(sname);
	leavefile.open("LeaveDB.dat", ios::in|ios::binary);

	while(leavefile.read((char *)&leaverecord, sizeof(leaverecord)))
	{
		if(strcmp(leaverecord.empname,sname)==0)
		{
			i++;
			if(i==1)
			{
				cout<<"\n";
				cout<<"ECode "<<"Employee Name                  "<<"Phone No.  "<<"From Date  "<<"To Date    "<<"Days "<<"Type"<<endl;
				cout<<"----- "<<"------------------------------ "<<"---------- "<<"---------- "<<"---------- "<<"---- "<<"----"<<endl;
			}
			cout<<"\n";
			cout<<leaverecord.empcode;
			cout<<"  ";
			cout<<leaverecord.empname;
			rightspaces(31-strlen(leaverecord.empname));
			cout<<leaverecord.phone;
			rightspaces(11-strlen(leaverecord.phone));
			cout<<leaverecord.fromdate;
			cout<<" ";
			cout<<leaverecord.todate;
			cout<<"  ";
			cout<<leaverecord.leavesapplied;
			cout<<"    ";
			cout<<leaverecord.leavetype;
			cout<<endl;
			flag =1;
		}
	}
	leavefile.close();
	if(flag==0)
	{
		iofile.open("EmplDB.dat",ios::in|ios::binary);
		int flag1=0;
		while(iofile.read((char*)&empdata,sizeof(empdata)))
		{
			if(strcmp(empdata.empname,sname)==0)
			{
				cout<<"\nEmployee exists but has not taken a leave yet.";
				cout<<"DETAILS:"<<endl;
				cout<<"\nEmployee Code:"<<empdata.empcode;
				cout<<"\nEmployee Name:"<<empdata.empname;
				cout<<"\nEMployee mobile number:"<<empdata.phone;
				flag1=1;
				break;
			}
		}
		iofile.close();
		if(flag1==0)
			cout<<"Employee does not exist";
	}

}

//this function states the guidelines of leave policy as prescribed by govt
void notice()
{
	clrscr();
	cout<<"\n\t\t\t\tNOTICE";
	cout<<"\nLeave policy under Delhi Shops and Establishments Act, 1954 states the following provisions:";
	cout<<"\n\n\n1)Earned/Privileged Leaves (after every 12 months of continuous employment): 15";
	cout<<"\n2)Casual Leaves (leave for urgent or unforeseen contingencies): 07";
	cout<<"\n3)Sick Leaves(Leave on account of sickness or illness): 07";
	cout<<"\n4)Apart from these, 11 Festival Holidays are also given to all the employees.";
	cout<<"\n5)Casual Leave cannot be availed more than 3 continuous days except in one        occasion.";
	cout<<"\n6)Sick Leave more than 3 days should be supported by a medical Certificate and Fitness Certificate to resume the duty, given by a registered Medical Practitioner or MBBS Doctor.";
	cout<<"\n7)It is necessary to apply for the leave in advance and important to get the sanction before proceeding on leave.";
	cout<<"Especially in case of Earned leave, prior sanctioning by the Head of Department is required, if the EL is upto 3 days, it is to be applied 24 hrs.";
	cout<<"In advance and if it is more than 3 days, it should be applied 7 days in advance.";
	cout<<"\n\nPress any key to continue";
	getch();

}

//This is first screen
void WelcomeScreen(employee * emp)
{
	clrscr();
	cout<<"\n\n\n\n\n\t\t  EMPLOYEE LEAVE MANAGEMENT SYSTEM ";
	cout<<"\n\t\t ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
	cout<<"\n\n\n\t\tMADE BY:";
	cout<<"\n\t\t\t  Harshita Varshney (XII-A)";
	cout<<"\n\t\t\t  Akriti Jain       (XII-A)";
	cout<<"\n\n              Press any key to continue.....";
	getch();
	clrscr();
	char ch1='y';
	int ch;
	do
	{
		clrscr();
		//This the main menu
		cout<<"\n\n\n\n\n\t\t\t\t MAIN MENU";
		cout<<"\n\t\t\t\t~~~~~~~~~~~";
		cout<<"\n\n\n\t\t1.ADMIN LOGIN";
		cout<<"\n\n\t\t2.EMPLOYEE LOGIN";
				cout<<"\n\n\t\t3.NOTICE";
		cout<<"\n\n\t\t4.EXIT";
		cout<<"\n\n\n\t\tENTER YOUR CHOICE[1,2,3,4]: ";
		cin>>ch;
		clrscr();
		switch(ch)
		{
				case 1: adminmenu();break;
			case 2:login(emp);
			     break;
				case 3:notice(); break;
			case 4:ch1='n';break;
                        default: cout<<"\nSORRY!INVALID CHOICE!!....TRY AGAIN!!";
                        break;
                }
        }while(ch1=='y'||ch1=='Y');
}

// Main Function
void main()
{
	employee emp;
	fstream empfile,leavefile;
	empfile.open("EmplDB.dat",ios::binary|ios::app);
	leavefile.open("LeaveDB.dat",ios::binary|ios::app);
	empfile.close();
	leavefile.close();
	WelcomeScreen(&emp);
}