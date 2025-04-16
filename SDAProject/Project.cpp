#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <ctime>
#include <random>
using namespace std;
//utility class date
class Date
{
int day;
int month;
int year;
public:
Date(int d=0, int m=0, int y=0) : day(d), month(m), year(y) {}

int getDay() const { return day; }
int getMonth() const { return month; }
int getYear() const { return year; }

static Date now() 
{
auto now = chrono::system_clock::now();
time_t currentTime = chrono::system_clock::to_time_t(now);
tm localTime = *localtime(&currentTime);
return Date(localTime.tm_mday, localTime.tm_mon + 1, localTime.tm_year + 1900);
}
   
bool isWithinRange(const Date& startDate, const Date& endDate)  const
{
if (*this >= startDate && *this <= endDate) 
{
return true;
}
return false;
}

bool isLeapYear(int year) const 
{
return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int daysInMonth(int month, int year) const 
{
if (month == 4 || month == 6 || month == 9 || month == 11)
 {
return 30;
        } 
else if (month == 2)
{
return isLeapYear(year) ? 29 : 28;
} else 
{
return 31;
}
}

int calculateDaysSince(const Date& date) const 
{
int totalDays = date.year * 365 + date.day;


for (int i = 1; i < date.month; i++) 
{
totalDays += daysInMonth(i, date.year);
}


for (int i = 0; i < date.year; i++)
{
if (isLeapYear(i)) 
{
totalDays++;
}
}

return totalDays;
}

int operator-(const Date& other) const 
{
int days1 = calculateDaysSince(*this);
int days2 = calculateDaysSince(other);
return abs(days1 - days2);
}


bool operator>=(const Date& other) const 
{
return (year > other.year) || (year == other.year && month > other.month) || 
               (year == other.year && month == other.month && day >= other.day);
}

bool operator<=(const Date& other) const 
{
return (year < other.year) || (year == other.year && month < other.month) || 
               (year == other.year && month == other.month && day <= other.day);
}

bool operator==(const Date& other) const 
{
return day == other.day && month == other.month && year == other.year;
}

Date& operator++()
{
day++; 
if (day > daysInMonth(month, year))
{
day = 1; 
month++; 
if (month > 12)
{
month = 1;
year++;
}
}
return *this;
}
Date operator++(int)
{
Date temp = *this; 
++(*this);
return temp;
}


friend istream& operator>>(istream& in, Date& date)
{
char separator1, separator2;
in >> date.day >> separator1 >> date.month >> separator2 >> date.year;
if (separator1 != '/' || separator2 != '/') 
{
in.setstate(ios::failbit); 
}
return in;
}

static void displayDate(const Date& date) 
{
cout << date.day << "/" << date.month << "/" << date.year << endl;
}
static Date parse(const string& dateStr, const string& format = "dd/mm/yyyy")
{
        int d, m, y;

        if (format == "dd/mm/yyyy") {
            char delimiter;
            stringstream ss(dateStr);
            ss >> d >> delimiter >> m >> delimiter >> y;

            if (ss.fail() || delimiter != '/') {
                cerr << "Invalid date format! Expected dd/mm/yyyy\n";
                return Date(); // Return default date if there's an error
            }

            return Date(d, m, y);
        }

        cerr << "Unsupported date format\n";
        return Date(); // Return default date if the format is unsupported
}

string toString() const 
{
ostringstream oss;
oss << (day < 10 ? "0" : "") << day << "/"<< (month < 10 ? "0" : "") << month << "/"<< year;
return oss.str();
}

};


class AttendanceRecord
{
Date date; 
int hoursWorked;
bool isPresent;    
public:
AttendanceRecord(Date d = Date(), int hours = 0, bool present = false) : date(d), hoursWorked(hours), isPresent(present) {}
//markAttendance()
int getHoursWorked() const{  return hoursWorked; }
bool getPresent() const{ return isPresent;}
Date getDate() const{ return date;}
void display(ostream& output)
{  output << "Date: " << date.getDay() << "-" << date.getMonth() << "-" << date.getYear()<< endl;
   output <<"Hours Worked: " << hoursWorked<<endl;
   output << "Attendance: " << (isPresent ? "Yes" : "No") << endl;
}
};

class LeaveBalance
{
int casualLeaveBalance;//Since theres a limit we use balance
int earnedLeaveBalance;//Since theres a limit we use balance
int officialLeaveCount;//Since theres no limit we simply track count
int unpaidLeaveCount;//Since theres no limit we simply track count
public:
LeaveBalance(int casual = 15, int earned = 21, int official = 0, int unpaid = 0) : casualLeaveBalance(casual), earnedLeaveBalance(earned), officialLeaveCount(official), unpaidLeaveCount(unpaid) {}

bool checkLeaveAvailability(const string& type, const Date& startDate, const Date& endDate) const 
{
int leaveDays = endDate - startDate; 
if (type == "CasualLeave") 
{
return leaveDays <= casualLeaveBalance && leaveDays <= 4;
} 
else if (type == "EarnedLeave") 
{
return leaveDays <= earnedLeaveBalance;
} 
else if (type == "OfficialLeave") 
{
return true; //No restriction on official leaves
} 
else if (type == "UnpaidLeave") 
{
return true; // No balance restriction for unpaid leaves
}
return false;
}

void updateLeaveBalance(const string& type, const Date& startDate, const Date& endDate) 
{
int leaveDays = endDate - startDate; // Calculate the number of leave days
if (type == "CasualLeave") 
{
casualLeaveBalance -= leaveDays;
} else if (type == "EarnedLeave") 
{
earnedLeaveBalance -= leaveDays;
} 
else if (type == "OfficialLeave") 
{
officialLeaveCount += 1;
} 
else if (type == "UnpaidLeave")
{
unpaidLeaveCount += 1;
}
}

void displayLeaveBalances(ostream& output) const 
{
output << "Leave Balances:\n";
output << "Casual Leave Balance: " << casualLeaveBalance << "\n";
output << "Earned Leave Balance: " << earnedLeaveBalance << "\n";
output << "Official Leaves Taken: " << officialLeaveCount << "\n";
output << "Unpaid Leaves Taken: " << unpaidLeaveCount << "\n";
output << "-------------------------------------------------------\n";
}


};


class Director // Assuming company has single director Singleton class
{
static Director* obj;
string name;
Director(const string& n = "Unknown") : name(n) {}

public:
static Director* getObj(const string& n = "Unknown")
{
if (obj == nullptr)
{
obj = new Director(n);
}
return obj;
}
bool forwardToDirector() const
{
static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_int_distribution<> dist(0, 1); // Generates 0 or 1 randomly
return dist(gen) == 1; // 50% chance of returning true or false
}

string getName() const { return name; }
~Director()
{
delete obj;
obj = nullptr;
}
};
Director* Director::obj = nullptr;

class Supervised;
//Strategy Desgin Pattern
class LeaveUpdateStrategy
{
public:
virtual void updateAttendance(Supervised* sup, const Date& startDate, const Date& endDate) const = 0;
virtual ~LeaveUpdateStrategy() {}
};

class CasualLeaveUpdateStrategy : public LeaveUpdateStrategy
{
public:
void updateAttendance(Supervised* sup, const Date& startDate, const Date& endDate) const override
{
Date start = startDate;
while (start <= endDate)
{
//sup->markAttendance(start, 0, false); // 0 hours worked, absent
++start;
}
}
};

class EarnedLeaveUpdateStrategy : public LeaveUpdateStrategy
{
public:
void updateAttendance(Supervised* sup, const Date& startDate, const Date& endDate) const override
{
Date start = startDate;
while (start <= endDate)
{
//sup->markAttendance(start, 8, false); // 8 hours worked, absent
++start;
}
}
};

class OfficialLeaveUpdateStrategy : public LeaveUpdateStrategy
{
public:
void updateAttendance(Supervised* sup, const Date& startDate, const Date& endDate) const override
{
Date start = startDate;
while (start <= endDate)
{
//sup->markAttendance(start, 8, true); // 8 hours worked, present
++start;
}
}
};

class UnpaidLeaveUpdateStrategy : public LeaveUpdateStrategy
{
public:
void updateAttendance(Supervised* sup, const Date& startDate, const Date& endDate) const override
{
Date start = startDate;
while (start <= endDate)
{
//sup->markAttendance(start, 0, false); // 0 hours worked, absent
++start;
}
}
};
//Observer Design Pattern 
class Observer 
{
public:
virtual void update(const string& message) = 0; // Notification method
virtual ~Observer() = default;
};

class LeaveApplication
{
protected:
int ID; 
int employeeID;
Date leaveStart;
Date leaveEnd;
string leaveAddress; 
string reason;
Date dateOfApplication;
vector<Observer*> observers;
public:
LeaveApplication(int id = 0, int empID = 0, Date start = Date(), Date end = Date(), string address = "", string reason = "", Date appDate = Date()) : ID(id), employeeID(empID), leaveStart(start), leaveEnd(end), leaveAddress(address), reason(reason),dateOfApplication(appDate){}
virtual bool requiresApproval() const {return 0;}
virtual int getMaxAllowedDays() const {return 0;}
Date getLeaveStart() const { return leaveStart;}
Date getLeaveEnd() const { return leaveEnd;}
int getEmployeeID() const { return employeeID;}
int getID() const { return ID;}
virtual void updateStatus() {return;}
virtual void updateAttendanceRecord(Supervised* sup){return;}
void addObserver(Observer* observer) 
{
observers.push_back(observer);
}
void notifyObservers(const string& message) 
{
for (Observer* observer : observers) 
{
observer->update(message);
}
}
virtual void viewApplications (ostream& output) 
{
output << "Leave ID: " << ID
<< ", Employee ID: " << employeeID
<< ", Application Date: " << dateOfApplication.toString()
<< ", Start Date: " << leaveStart.toString()
<< ", End Date: " << leaveEnd.toString()
<< ", Address: " << leaveAddress
<< ", Reason: " << reason << endl;
}
virtual string isApproved(){ return "";}
virtual ~LeaveApplication() = default; 
};

class CasualLeave : public LeaveApplication 
{
public:
CasualLeave(int id = 0, int empID = 0, Date start = Date(), Date end = Date(), string address = "", string reason = "", Date appDate = Date()) : LeaveApplication(id, empID, start, end, address, reason, appDate) {}
bool requiresApproval() const override 
{
return false;
}
int getMaxAllowedDays() const override
{
return 4; 
}

void updateAttendanceRecord(Supervised* sup)
{
//CasualLeaveUpdateStrategy::updateAttendance(sup,getLeaveStart(),getLeaveEnd() );
}

};

class EarnedLeave : public LeaveApplication 
{
string status;
Date approvalDate;
Date DirectorApprovalDate;        
public:
EarnedLeave(int id = 0, int empID = 0, Date start = Date(), Date end = Date(), string address = "", string reason = "", Date appDate = Date(), string stats = "Pending", Date approvDate = Date(), string type = "Earned", Date dirApprovDate = Date()) : LeaveApplication(id, empID, start, end, address, reason, appDate),status(stats),approvalDate(approvDate),DirectorApprovalDate(dirApprovDate){}
bool requiresApproval() const override
{
return true;
}
int getMaxAllowedDays() const override
{
return 21; 
}

void updateStatus()
{
status="Approved";
approvalDate=Date::now();
notifyObservers(status);
}

void updateAttendanceRecord(Supervised* sup)
{
//EarnedLeaveUpdateStrategy::updateAttendance(sup,getLeaveStart(),getLeaveEnd() );
}
void viewApplications (ostream& output) 
{
output << "Leave ID: " << ID
<< ", Employee ID: " << employeeID
<< ", Application Date: " << dateOfApplication.toString()
<< ", Start Date: " << leaveStart.toString()
<< ", End Date: " << leaveEnd.toString()
<< ", Address: " << leaveAddress
<< ", Reason: " << reason 
<< ", Status: " << status<< endl;
}

string isApproved(){ return status;}

};

class OfficialLeave : public LeaveApplication 
{
public:
OfficialLeave(int id = 0, int empID = 0, Date start = Date(), Date end = Date(), string address = "", string reason = "", Date appDate = Date()) : LeaveApplication(id, empID, start, end, address, reason, appDate) {}
bool requiresApproval() const override 
{
return false;
}
int getMaxAllowedDays() const override 
{
return 0; 
}
void updateAttendanceRecord(Supervised* sup)
{
//OfficialLeaveUpdateStrategy::updateAttendance(sup,getLeaveStart(),getLeaveEnd() );
}

};

class UnpaidLeave : public LeaveApplication 
{
string status;
Date approvalDate;
Date DirectorApprovalDate;        
public:
UnpaidLeave(int id = 0, int empID = 0, Date start = Date(), Date end = Date(), string address = "", string reason = "", Date appDate = Date(), string stats = "Pending", Date approvDate = Date(), string type = "Unpaid", Date dirApprovDate = Date()) : LeaveApplication(id, empID, start, end, address, reason, appDate),status(stats),approvalDate(approvDate),DirectorApprovalDate(dirApprovDate){}
bool requiresApproval() const override 
{
return true;
}
int getMaxAllowedDays() const override 
{
return 365;  
}

void updateStatus()
{
status="Approved";
approvalDate=Date::now();
notifyObservers(status);
}
void updateAttendanceRecord(Supervised* sup)
{
//UnpaidLeaveUpdateStrategy::updateAttendance(sup,getLeaveStart(),getLeaveEnd() );
}

void viewApplications (ostream& output) 
{
output << "Leave ID: " << ID
<< ", Employee ID: " << employeeID
<< ", Application Date: " << dateOfApplication.toString()
<< ", Start Date: " << leaveStart.toString()
<< ", End Date: " << leaveEnd.toString()
<< ", Address: " << leaveAddress
<< ", Reason: " << reason 
<< ", Status: " << status<< endl;
}

string isApproved(){ return status;}

};

//Factory method(design pattern)
class LeaveFactory 
{
public:
static LeaveApplication* createLeave(int leaveType, int id, int empID, const Date& startDate, const Date& endDate, const string& address, const string& reason, const Date& applicationDate) 
{
switch (leaveType) 
{
case 1:
return new CasualLeave(id, empID, startDate, endDate, address, reason, applicationDate);
case 2:
return new EarnedLeave(id, empID, startDate, endDate, address, reason, applicationDate);
case 3:
return new OfficialLeave(id, empID, startDate, endDate, address, reason, applicationDate);
case 4:
return new UnpaidLeave(id, empID, startDate, endDate, address, reason, applicationDate);
default:
return nullptr;
}
}
};

class Employee
{
int ID ;
string name;
string department;
AttendanceRecord* attendanceRecords;
static int recordcapacity;
int recordcount;
public:
Employee(int id = 0, string name = "Unknown", string dept = "General",AttendanceRecord* records=nullptr,int reccount=0): ID(id), name(name), department(dept),attendanceRecords(records),recordcount(reccount) {}
bool search(const Date& date)
{
for (int i = 0; i < recordcount; i++) 
{
if (attendanceRecords[i].getDate()==date) 
{
return true;
}
}
return false;
}
int getID()const {return ID;}
AttendanceRecord* getAtt()const { return attendanceRecords;}
int getCount() const {return recordcount;}
string getName() const {return name;} 
int getHours()
{
int sum=0;
for (int i = 0; i < recordcount; i++) 
{
sum+=attendanceRecords[i].getHoursWorked(); 
}
return sum;
}

void markAttendance(const Date& date, int hoursWorked,bool isPresent) 
{
if (attendanceRecords == nullptr) 
{
attendanceRecords = new AttendanceRecord[recordcapacity];
recordcount = 0;
}

if(search(date))
{
cout << "Attendance for this date is already marked!" << endl;
return;
}

if(getHours()+hoursWorked>=(40*4*12))//40 hours per week*4 weeks per month* 12 months per year
{
cout << "Working hours for this year are accounted for." << endl;
return;
}

if (recordcount >= recordcapacity) 
{
cout << "Cannot mark attendance: Maximum record capacity reached!" << endl;
return;
}

if (hoursWorked>8)
{
cout << "Cannot Stay on premises for longer than 8 hours!" << endl;
return;
}
attendanceRecords[recordcount++] = AttendanceRecord(date, hoursWorked, isPresent);
cout << "Attendance marked successfully for " << date.getDay() << "-"
     << date.getMonth() << "-" << date.getYear() << "." << endl;
}

void viewAttendance(ostream& output)
{
if (recordcount == 0 || attendanceRecords == nullptr)
{
output << "Employee has just started attendance, not recorded yet." << endl;
return;
}

output << "Attendance Records for Employee ID: " << ID << ", Name: " << name << endl;
output << "-------------------------------------------------------" << endl;
output << "Date         | Hours Worked | Present (Yes/No)" << endl;
output << "-------------------------------------------------------" << endl;

for (int i = 0; i < recordcount; i++)
{
attendanceRecords[i].display(output);
}
output << "-------------------------------------------------------" << endl;
}


double calculateMonthlyAttendancePercentage(int month, int year)
{
if(recordcount == 0 || attendanceRecords == nullptr)
{
cout << "No attendance records available for employee " << name << "." << endl;
return 0.0;
}

int totalHoursWorked = 0;
int totalWorkingDays = 0;
int daysPresent = 0;

// Each month is assumed to have 4 weeks, 5 working days per week, and 8 hours per day
int expectedWorkingDays = 4 * 5;  // 4 weeks * 5 days per week = 20 working days
int expectedHours = expectedWorkingDays * 8;  // 20 days * 8 hours = 160 hours

for(int i = 0; i < recordcount; i++)
{
Date recordDate = attendanceRecords[i].getDate();
if(recordDate.getMonth() == month && recordDate.getYear() == year)
{
totalHoursWorked += attendanceRecords[i].getHoursWorked();
if(attendanceRecords[i].getPresent())
daysPresent++;
totalWorkingDays++;
}
}

if(totalWorkingDays == 0)
{
cout << "No attendance records for the specified month and year." << endl;
return 0.0;
}

double attendancePercentage = (static_cast<double>(totalHoursWorked) / expectedHours) * 100;
cout << "Monthly attendance percentage for employee " << name << " is: " 
<< attendancePercentage << "%" << endl;
return attendancePercentage;
}

 ~Employee() 
{
if (attendanceRecords) 
{
delete[] attendanceRecords;
attendanceRecords=nullptr;
}
}
};
//if there are 40 working hours a week there must be 5 days of work per week so total working days anually=5*4*12=240-10(miscellaneous holidays)=230 days
int Employee::recordcapacity=230;

class Supervised;
class Supervisor : public Employee
{
static const int MAX_SUBORDINATES = 10;
Supervised* subordinates[MAX_SUBORDINATES];
int subordinateCount = 0;

public:
Supervisor(int id = 0, string name = "Unknown", string dept = "General", AttendanceRecord* records = nullptr) : Employee(id, name, dept, records) {}

void assignSubordinate(Supervised* subordinate)
{
if (subordinate && subordinateCount < MAX_SUBORDINATES)
{
subordinates[subordinateCount++] = subordinate;
}
else
{
cout << "Cannot assign more subordinates or subordinate is invalid.\n";
}
}

bool processLeave(LeaveApplication* leave)
{
static int leaveCounter = 0;
if (++leaveCounter % 5 == 0)
{
cout << "Leave application rejected due to policy (every fifth leave is rejected)."<<endl;
leave->notifyObservers("rejected");
return false;
}
cout << "Leave for Employee ID " << leave->getEmployeeID() << " is approved by Supervisor."<<endl;
if((leave->getLeaveEnd()-leave->getLeaveStart())>=10) // assuming a long leave is  a leave that requires prior approval and is longer than 10 days so forwading to director
{
cout<<"Since leave is longer than 10 consecutive days(Long leave) forwarding to director for approval"<<endl;
if(!Director::getObj("John Doe")->forwardToDirector())
{
cout << "Long Leave application rejected by Company Director."<<endl;
leave->notifyObservers("rejected");
return false;
}
cout << "Long Leave application approved by Company Director."<<endl;
}
leave->updateStatus();
return true;      
}

~Supervisor() 
{
for (int i = 0; i < subordinateCount; ++i)
{
subordinates[i] = nullptr;
}
}
Supervised** getSubordinates() { return subordinates; }
int getSubordinateCount() const { return subordinateCount; }

};

class Supervised: public Employee, public Observer
{
Supervisor* superior;
vector<LeaveApplication*> leaveApplications;
LeaveBalance* LeaveRecord;
public:
Supervised(int id = 0, string name = "Unknown", string dept = "General", AttendanceRecord* records = nullptr, Supervisor* sup = nullptr,LeaveBalance* LeaveRec = nullptr) : Employee(id, name, dept, records), superior(sup),LeaveRecord(LeaveRec) {}

Supervisor* getSupervisor()const { return superior;}

LeaveBalance* getBalance()const {return LeaveRecord;} 

vector<LeaveApplication*> getLeaveApplications() const {return leaveApplications;} 

void ApplyLeave(int id,int empID,int leaveType,string address,string reason,Date startDate,Date endDate)
{
Date applicationDate=Date::now();
if(LeaveRecord==nullptr)
{
LeaveRecord= new LeaveBalance();
}
string leaveTypeName;
switch (leaveType) 
{
case 1: leaveTypeName = "CasualLeave"; break;
case 2: leaveTypeName = "EarnedLeave"; break;
case 3: leaveTypeName = "OfficialLeave"; break;
case 4: leaveTypeName = "UnpaidLeave"; break;
default: 
cout << "Invalid leave type selected.\n";
return;
}
if(LeaveRecord->checkLeaveAvailability(leaveTypeName,startDate,endDate))
{
LeaveApplication* leave = LeaveFactory::createLeave(leaveType, id, empID, startDate, endDate, address, reason, applicationDate);
if(!leave)
{
cout << "Invalid leave type selected.\n";
return;
}
leave->addObserver(this);
leaveApplications.push_back(leave);
cout << "Leave application submitted successfully.\n";
processApplication(leaveTypeName,leave); 
}
else
{
cout<<"You have availed all of your "<<leaveTypeName<<" Leaves or You have applied wrong type of leaves. Cannot apply."<<endl;
return;
}
}

void updateAttendanceRecord(const string& Type, const Date& startDate, const Date& endDate)
{
Date start = startDate;
int hoursWorked = 0;
bool isPresent = false;

if (Type == "Earned Leave")
{
hoursWorked = 8; // 8 hours worked, absent
}
else if (Type == "Official Leave")
{
hoursWorked = 8; // 8 hours worked, present
isPresent = true;
}
else if (Type == "Unpaid Leave" || Type == "Casual Leave")
{
hoursWorked = 0; // 0 hours worked, absent
}

while (start <= endDate)
{
markAttendance(start, hoursWorked, isPresent);
++start;
}
}

void processApplication(const string& Type,LeaveApplication* leave) 
{
if (!leave->requiresApproval())
{
//cout << "Processing "<<Type<<" Leave for Employee ID: " << employeeID << endl;
//cout << "No prior approval required and Leaves are not exhausted.Leave Granted." << endl;
}
else
{
if (superior != nullptr) 
{

if(!superior->processLeave(leave))
{
cout << "Your Application was not approved by Supervisor.Leave not Granted." << endl;
return;
}
cout << "Your Application was approved.Leave Granted." << endl;
} 
else 
{
cout << "No supervisor assigned to process the leave.\n";
return;
}
}
//leave->updateAttendanceRecord(this);
updateAttendanceRecord(Type,leave->getLeaveStart(),leave->getLeaveEnd() );
LeaveRecord->updateLeaveBalance(Type,leave->getLeaveStart(),leave->getLeaveEnd()); 
cout<<"Records updates successfully"<<endl;
}

void update(const string& message) override 
{
cout << "Application of Employee with ID: " << getID() << " has been "<<message<<"."<< endl;
}

~Supervised() 
{
for (auto leave : leaveApplications) 
{
delete leave;
}
leaveApplications.clear(); 
superior=nullptr;  
delete LeaveRecord;
LeaveRecord=nullptr;    
}

};

void readDataFromFile(const string& fileName, vector<Supervisor*>& supervisors, vector<Supervised*>& supervisedEmployees) 
{
ifstream file(fileName);

if (!file.is_open()) {
cerr << "Error: Could not open file " << fileName << endl;
return;
}

vector<Supervised*> tempEmployees(100, nullptr); // Temporary storage for quick lookup
vector<Supervisor*> tempSupervisors(50, nullptr); // Temporary storage for supervisors

string line;

while (getline(file, line)) {
if (line.empty() || line[0] == '#') 
continue; // Skip empty lines and comments

char type = line[0]; // First character determines type of entry
line = line.substr(2); // Remove type prefix and delimiter (e.g., "S|")

if (type == 'S') { // Supervisor entry
int id;
string name, department;

size_t delim1 = line.find('|');
size_t delim2 = line.find('|', delim1 + 1);

id = stoi(line.substr(0, delim1));
name = line.substr(delim1 + 1, delim2 - delim1 - 1);
department = line.substr(delim2 + 1);

Supervisor* supervisor = new Supervisor(id, name, department);
supervisors.push_back(supervisor);
tempSupervisors[id] = supervisor;

} else if (type == 'E') { // Employee entry
int id, supervisorID;
string name, department;

size_t delim1 = line.find('|');
size_t delim2 = line.find('|', delim1 + 1);
size_t delim3 = line.find('|', delim2 + 1);

id = stoi(line.substr(0, delim1));
name = line.substr(delim1 + 1, delim2 - delim1 - 1);
department = line.substr(delim2 + 1, delim3 - delim2 - 1);
supervisorID = stoi(line.substr(delim3 + 1));

Supervisor* supervisor = tempSupervisors[supervisorID];
if (!supervisor) {
cerr << "Error: Supervisor ID " << supervisorID << " not found for employee ID " << id << endl;
continue;
}

Supervised* employee = new Supervised(id, name, department, nullptr, supervisor);
supervisedEmployees.push_back(employee);
supervisor->assignSubordinate(employee);
tempEmployees[id] = employee;

} else if (type == 'A') { // Attendance entry
int empID, hoursWorked, present;
string dateStr;

size_t delim1 = line.find('|');
size_t delim2 = line.find('|', delim1 + 1);
size_t delim3 = line.find('|', delim2 + 1);

empID = stoi(line.substr(0, delim1));
dateStr = line.substr(delim1 + 1, delim2 - delim1 - 1);
hoursWorked = stoi(line.substr(delim2 + 1, delim3 - delim2 - 1));
present = stoi(line.substr(delim3 + 1));

Date date = Date(stoi(dateStr.substr(0, 2)), stoi(dateStr.substr(3, 2)), stoi(dateStr.substr(6, 4)));
if (tempEmployees[empID]) {
tempEmployees[empID]->markAttendance(date, hoursWorked, present == 1);
} else {
cerr << "Error: Employee ID " << empID << " not found for attendance entry.\n";
}

} else if (type == 'L') { // Leave entry
int leaveID, empID, leaveType;
string startDateStr, endDateStr, address, reason;

size_t delim1 = line.find('|');
size_t delim2 = line.find('|', delim1 + 1);
size_t delim3 = line.find('|', delim2 + 1);
size_t delim4 = line.find('|', delim3 + 1);
size_t delim5 = line.find('|', delim4 + 1);

leaveID = stoi(line.substr(0, delim1));
empID = stoi(line.substr(delim1 + 1, delim2 - delim1 - 1));
leaveType = stoi(line.substr(delim2 + 1, delim3 - delim2 - 1));
startDateStr = line.substr(delim3 + 1, delim4 - delim3 - 1);
endDateStr = line.substr(delim4 + 1, delim5 - delim4 - 1);
address = line.substr(delim5 + 1, line.find_last_of('|') - delim5 - 1);
reason = line.substr(line.find_last_of('|') + 1);

Date startDate = Date(stoi(startDateStr.substr(0, 2)), stoi(startDateStr.substr(3, 2)), stoi(startDateStr.substr(6, 4)));
Date endDate = Date(stoi(endDateStr.substr(0, 2)), stoi(endDateStr.substr(3, 2)), stoi(endDateStr.substr(6, 4)));

if (tempEmployees[empID]) 
{
tempEmployees[empID]->ApplyLeave(leaveID, empID, leaveType, address, reason, startDate, endDate);
} else {
cerr << "Error: Employee ID " << empID << " not found for leave entry.\n";
}
}
}

file.close();
}

class AttendanceManager 
{
vector<Supervised*> employees;
public:
AttendanceManager(const vector<Supervised*>& employeeList) : employees(employeeList) {}

void generateMonthlyAttendanceReport(int year,double thresholdPercentage, const string& fileName) 
{
ofstream report(fileName);
if (!report.is_open()) 
{
cerr << "Error: Unable to open file: " << fileName << "\n";
return;
}

report << "Employees with monthly attendance percentages for the year " << year << ":\n\n";

for (auto emp : employees) 
{
report << "Employee ID: " << emp->getID() << ", Name: " << emp->getName() << ":\n";
bool belowThreshold = false;

for (int month = 1; month <= 12; ++month) 
{
double attendancePercentage = emp->calculateMonthlyAttendancePercentage(month, year);
report << "  Month " << month << ":   " << " " << attendancePercentage << "%";

if (attendancePercentage < thresholdPercentage) 
{
report << " (Below Threshold)";
belowThreshold = true;
}

report << "\n";
}

if (!belowThreshold) 
{
report << "  No months below the threshold (" << thresholdPercentage << "%).\n";
}

report << "\n";
}

report.close();
cout << "Monthly attendance report generated in " << fileName << "\n";
}

void generateAttendanceDetailsReport(int employeeID, const string& fileName)
{
ofstream report(fileName);
if (!report.is_open()) 
{
cerr << "Error: Unable to open file: " << fileName << "\n";
return;
}

for (auto emp : employees) 
{
if (emp->getID() == employeeID) 
{
report << "Attendance details for Employee ID: " << emp->getID() << ", Name: " << emp->getName() << ":\n";
emp->viewAttendance(report);
report.close();
cout << "Attendance details report generated in " << fileName << "\n";
return;
}
}

cerr << "Error: Employee with ID " << employeeID << " not found.\n";
report.close();
}

void generateOutstandingLeavesReport(const string& fileName) 
{
ofstream report(fileName);
if (!report.is_open()) 
{
cerr << "Error: Unable to open file: " << fileName << "\n";
return;
}
report << "Employees with outstanding leave issues:\n";

for (auto emp : employees) 
{
const AttendanceRecord* attendanceRecords = emp->getAtt();
int recordCount = emp->getCount(); // Get the number of attendance records
if (attendanceRecords == nullptr || recordCount == 0) continue; // Handle empty or null attendance records

const vector<LeaveApplication*>& leaveApplications = emp->getLeaveApplications();

for (int i = 0; i < recordCount; ++i) 
{
const AttendanceRecord& record = attendanceRecords[i];

if (!record.getPresent() && !isDateCoveredByApprovedLeave(leaveApplications, record.getDate())) 
{
report << "Employee ID: " << emp->getID()
<< ", Name: " << emp->getName()
<< ", Date Absent: " << record.getDate().toString()
<< " - No approved leave application for this date.\n";
}
}

for (const auto& leave : leaveApplications) 
{
if (leave->isApproved() != "Approved") 
{
report << "Employee ID: " << emp->getID()
<< ", Name: " << emp->getName()
<< ", Leave ID: " << leave->getID()
<< " - Rejected leave application from "
<< leave->getLeaveStart().toString() << " to "
<< leave->getLeaveEnd().toString()
<< ". Employee was absent during this period.\n";
}
}
}

report.close();
cout << "Outstanding leave report generated in " << fileName << "\n";
}


bool isDateCoveredByApprovedLeave(const vector<LeaveApplication*>& leaveApplications, const Date& date)
 {
for (const auto& leave : leaveApplications) 
{
if (leave->isApproved() == "Approved" && date >= leave->getLeaveStart() && date <= leave->getLeaveEnd()) 
{
return true;
}
}
return false;
}

void generateLeaveDetailsReport(const string& fileName) 
{
ofstream report(fileName);
if (!report.is_open()) 
{
cerr << "Error: Unable to open file: " << fileName << "\n";
return;
}

for (auto emp : employees) 
{
report << "Leave details for Employee ID: " << emp->getID() << ", Name: " << emp->getName() << ":\n";
report << "-------------------------------------------------------\n";
vector<LeaveApplication*> applications = emp->getLeaveApplications();

if (applications.empty()) 
{
report << "No leave applications found for this employee.\n";
} 
else 
{
for (const auto& leave : applications) 
{
leave->viewApplications(report);
}
}

// Including leave balance details
report << "\nLeave Balances:\n";
emp->getBalance()->displayLeaveBalances(report);
report << "=======================================================\n";
}
report.close();
cout << "Leave details report generated in " << fileName << "\n";
}

};

int main() 
{
vector<Supervisor*> supervisors;
vector<Supervised*> supervisedEmployees;

// Reading data from file
cout << "Reading data from file...\n";
string fileName = "employee_data.txt"; 
readDataFromFile(fileName, supervisors, supervisedEmployees);

// Initializing AttendanceManager
AttendanceManager attendanceManager(supervisedEmployees);

// Generating monthly attendance report
cout << "\nGenerating monthly attendance report...\n";
attendanceManager.generateMonthlyAttendanceReport(2024, 80.0, "MonthlyAttendanceReport.txt");

// Generating attendance details for a specific employee
cout << "\nGenerating attendance details for Employee ID: 101...\n";
attendanceManager.generateAttendanceDetailsReport(101, "AttendanceDetails.txt");

// Generating outstanding leaves report
cout << "\nGenerating outstanding leaves report...\n";
attendanceManager.generateOutstandingLeavesReport("OutstandingLeavesReport.txt");

// Generating leave details report
cout << "\nGenerating leave details report...\n";
attendanceManager.generateLeaveDetailsReport("LeaveDetailsReport.txt");

// Cleanup dynamically allocated memory
for (auto supervisor : supervisors) 
{
delete supervisor;
}

for (auto supervised : supervisedEmployees) 
{
delete supervised;
}

cout << "\nAll operations completed successfully!\n";
return 0;
}



