#pragma once
#include"FileIOManager.h"

typedef int flag;
const int NESTEDBLOCKLOOPJOIN = 1000;
class JoinData {
	Student stu;
	Professor pro;
};

void PrintHead()
{
	printf("------------------------------------------------------------------------------------------------------------------------------------------\n");
	printf("||     Student name     || Student ID || Student score || Student AdvisorID ||     Professor name   || Professor ID || Professor Salary ||\n");
	printf("------------------------------------------------------------------------------------------------------------------------------------------\n");
	

}
void PrintJoinData(const Student& stu, const Professor pro)
{
	printf("|| %20s ||  %9d ||           %0.1f ||            %6d || %20s ||     %8d ||           %6d ||\n", "Julius Cesare", stu.studentID, stu.score, stu.advisorID, pro.name, pro.ProfID, pro.Salary);
	printf("------------------------------------------------------------------------------------------------------------------------------------------\n");
}
void JoinProcessing(flag f)
{
	FileManager fm;
	if (f == NESTEDBLOCKLOOPJOIN) {
		
		// student
		vector<StudentBucket*> AllStudent = fm.bucketLoadAll(&StudentBucket(), ifstream("Students.DB"));
		PrintHead();
		for (unsigned i = 0; i < AllStudent.size(); ++i) {
			ProfessorBucket* bucket=nullptr;
			fm.bucketLoad(&bucket, i + ProfessorBucket::initNum, ifstream("Professor.DB"));
			ProfessorBucket& professors=*bucket;
			StudentBucket& students = *AllStudent[i];
			for (int s = 0; students.getSize(); ++s) {
				for (int p = 0; professors.getSize(); ++p) {
					if (students[s].advisorID == professors[p].ProfID) {
						PrintJoinData(students[s], professors[p]);
						break;
					}
				}
			}
		}
	}
}

