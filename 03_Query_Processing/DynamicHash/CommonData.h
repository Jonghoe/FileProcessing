#pragma once
const int BLOCK_SIZE = 4096;
#pragma pack(push,1)
struct Student {
	unsigned advisorID;
	char name[20];	
	unsigned studentID;
	float score;
};
#pragma pack(pop)
#pragma pack(push,1)
struct Professor {
	char name[20];
	unsigned ProfID;
	int Salary;
};
#pragma pack(pop)