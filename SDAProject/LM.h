#include <iostream>
#include <string>
#include <Windows.h>
#include <Time.h>
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

int calculateDaysBetween( const Date& D1, const Date& D2)  const
{
int days1 = calculateDaysSince(D1);
int days2 = calculateDaysSince(D2);
return abs(days1 - days2);
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
};

class AttendanceRecord
{
Date date; 
int hoursWorked;
bool isPresent;    
public:
AttendanceRecord(Date d = Date(), int hours = 0, bool present = false) : date(d), hoursWorked(hours), isPresent(present) {}
//markAttendance()
int GetHours() const{  return hoursWorked; }
bool GetPresent() const{ return isPresent;}
};

class LeaveApplication
{
int ID; 
int employeeID;
Date leaveStart;
Date leaveEnd;
string leaveAddress; 
string reason;
Date dateOfApplication;
public:
LeaveApplication(int id = 0, int empID = 0, Date start = Date(), Date end = Date(), string address = "", string reason = "", Date appDate = Date()) : ID(id), employeeID(empID), leaveStart(start), leaveEnd(end), leaveAddress(address), reason(reason),dateOfApplication(appDate){}
virtual bool requiresApproval() const {};  
virtual int getMaxAllowedDays() const {};
//applyLeave(), updateStatus(), notifyEmployee()
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
};

class LeaveBalance
{
int casualLeaveBalance;
int earnedLeaveBalance;
int  officialLeaveCount;
int unpaidLeaveCount;
public:
LeaveBalance(int casual = 15, int earned = 21, int official = 0, int unpaid = 0) : casualLeaveBalance(casual), earnedLeaveBalance(earned), officialLeaveCount(official), unpaidLeaveCount(unpaid) {}


//updateLeaveBalance(), checkLeaveAvailability()

};

class Director//only one object to be made
{
string name;
public:
// approveLongLeave(), rejectLeave()
};

class Employee
{
int ID ;
string name;
string department;
AttendanceRecord** attendanceRecords;
public:
Employee(int id = 0, string name = "Unknown", string dept = "General", AttendanceRecord** records= nullptr): ID(id), name(name), department(dept), attendanceRecords(records) {}
//viewAttendance(), viewLeaveDetails(), applyForLeave(), viewLeaveBalance()
virtual double calculateMonthlyAttendancePercentage()=0;
 ~Employee() 
{
if (attendanceRecords) 
{
for (int i = 0; attendanceRecords[i] != nullptr; i++) 
{
delete attendanceRecords[i];
attendanceRecords[i]=nullptr;
}
delete[] attendanceRecords;
attendanceRecords=nullptr;
}
}
};

class Supervised;
class Supervisor: public Employee
{
Supervised** subordinates;
public:
Supervisor(int id = 0, string name = "Unknown", string dept = "General",  AttendanceRecord** records = nullptr, Supervised** subs = nullptr) : Employee(id, name, dept, records), subordinates(subs) {}


// approveLeave(), rejectLeave(), forwardToDirector()

 ~Supervisor() 
 {
if (subordinates) 
{
for (int i = 0; subordinates[i] != nullptr; i++) 
{
 subordinates[i]=nullptr;
}
subordinates=nullptr;
}
}
};

class Supervised: public Employee
{
Supervisor* superior;
public:
Supervised(int id = 0, string name = "Unknown", string dept = "General", AttendanceRecord** records = nullptr, Supervisor* sup = nullptr) : Employee(id, name, dept, records), superior(sup) {}
//double calculateMonthlyAttendancePercentage(int month, int year) {
//         int totalHoursWorked = 0;
//         int daysPresent = 0;
//         int expectedWorkingDays = 20; // Assuming 20 workdays in a month for simplicity
//         int expectedHours = expectedWorkingDays * 8; // Assuming 8 hours per day
//         for (const auto& record : attendanceRecords) {
//             if (record.date.getMonth() == month && record.date.getYear() == year) {
//                 totalHoursWorked += record.hoursWorked;
//                 if (record.isPresent) daysPresent++;
//             }
//         }
//         // Calculate percentage based on hours or days
//         double attendancePercentage = (totalHoursWorked / static_cast<double>(expectedHours)) * 100;
//         // Alternatively, if using days:
//         // double attendancePercentage = (daysPresent / static_cast<double>(expectedWorkingDays)) * 100;
//         return attendancePercentage;
//     }

~Supervised() 
{
superior=nullptr;     
}
};