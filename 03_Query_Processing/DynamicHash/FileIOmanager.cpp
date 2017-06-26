#include"FileIOManager.h"
#include"StudentBucket.h"
#include<cassert>
#include<direct.h>
void FileManager::hashsave(const HashTable& tlb)
{
	const vector<int>& tb = tlb.getTable();
	string hashPath;
	if (tlb.getType() == BucketFactory::Type::student) {
		hashPath = "Students.hash";
	}
	else if (tlb.getType() == BucketFactory::Type::professor) {
		hashPath = "Professors.hash";
	}
	ofstream writeHash(hashPath.data(), ios::binary);	
	for (unsigned i = 0; i < tb.size(); ++i) {
		writeHash.write((const char*)(&i), 4);
		writeHash.write(reinterpret_cast<const char*>(&tb[i]), 4);
	}
	writeHash.close();
}
vector<int> FileManager::hashload(BucketFactory::Type type)
{
	int i, temp;
	vector<int> ht;
	string hashPath;
	if (type == BucketFactory::Type::student) {
		hashPath = "Students.hash";
	}
	else if (type == BucketFactory::Type::professor) {
		hashPath = "Professors.hash";
	}
	ifstream hashl(hashPath.data(), ios::binary);
	do{
		hashl.read((char*)(&i), 4);
		hashl.read((char*)(&temp),4);
		ht.push_back(temp);
	} while (!hashl.eof());
	ht.pop_back();
	return ht;
}

Bucket* FileManager::bucketSave(const ProfessorBucket& bk, ofstream& wDB)
{	
	wDB.seekp((bk.getBlkNum() - ProfessorBucket::initNum)*BLOCK_SIZE, ios::beg);
	if (bk.getBlkNum() - ProfessorBucket::initNum == 129)
		cout << "hello";
	if (wDB.fail())
		return nullptr;
	int blkNum = bk.getBlkNum();
	int size = bk.getSize();
	int level = bk.getLevel();

	wDB.write((const char*)(&blkNum), 4);
	wDB.write((const char*)(&size), 4);
	wDB.write((const char*)(&level), 4);
	for (int j = 0; j<bk.getSize(); ++j) {
		int aid = bk[j].ProfID;
		int salary= bk[j].Salary;
		wDB.write((const char*)(&aid), 4);
		wDB.write(bk[j].name, 20);
		wDB.write((const char*)(&salary), 4);
	}
	// ���� ũ�� ��ŭ ������ ����� ä���.
	int buffer[6] = { 0,0,0,0,0,0 };
	for (int j = 0; j < bk.getCapacity() - bk.getSize(); ++j) {
		wDB.write((const char*)(buffer), 4);
		wDB.write((const char*)(buffer), 20);
		wDB.write((const char*)(buffer), 4);
	}
	wDB.write((const char*)(buffer), 24);
}

Bucket* FileManager::bucketSave(const StudentBucket& bk,ofstream& wDB)
{
	// ���ϴ� ��ġ�� �̵� (������ �̵�)	
	wDB.seekp((bk.getBlkNum()-StudentBucket::initNum)*BLOCK_SIZE,ios::beg);
	if (wDB.fail() != 0){
		return nullptr;
	}
	int beg, end;
	int blkNum = bk.getBlkNum();
	int size = bk.getSize();
	int level = bk.getLevel();
	wDB.write((const char*)(&blkNum), 4);
	wDB.write((const char*)(&size), 4);
	wDB.write((const char*)(&level), 4);
	for (int j = 0; j<bk.getSize(); ++j) {
		wDB.write((const char*)(&bk[j].advisorID),  4);
		wDB.write(bk[j].name, 20);
		wDB.write((const char*)(&bk[j].studentID), 4);
		wDB.write((const char*)(&bk[j].score), 4);
	}
	// ���� ũ�� ��ŭ ������ ����� ä���.
	int buffer[8] = { 0,0,0,0,0 ,0,0,0};
	for (int j = 0; j < bk.getCapacity() - bk.getSize(); ++j) {
		wDB.write((const char*)(buffer), 32);
	}
	wDB.write((const char*)(buffer), 20);

}
vector<StudentBucket*> FileManager::bucketLoadAll(StudentBucket* bk,ifstream& rDB)
{
	vector<StudentBucket*> buckets;
	StudentBucket* bucket=nullptr;
	int i = StudentBucket::initNum;
	check = true;
	do {
		bucketLoad(&bucket, i++, rDB);
		buckets.push_back(bucket);
	}while (bucket != nullptr);
	check = false;
	return buckets;
}
vector<ProfessorBucket*> FileManager::bucketLoadAll(ProfessorBucket* bk,ifstream & rDB)
{
	vector<ProfessorBucket*> buckets;
	ProfessorBucket* bucket = nullptr;
	int i = ProfessorBucket::initNum;
	do {
		bucketLoad(&bucket, i++, rDB);
		buckets.push_back(bucket);
	} while (bucket != nullptr);
	return buckets;
}

void FileManager::bucketLoad(ProfessorBucket** bk, int blk, ifstream &rDB)
{
	*bk = new ProfessorBucket();
	int t_aid, salary, level, size, blkNum;
	char t_n[20];
	// ���ϴ� ��ġ�� �̵� (������ �̵�)
	rDB.seekg((blk-ProfessorBucket::initNum)*BLOCK_SIZE,ios::beg);
	if (rDB.fail()){
		*bk = nullptr;
		return;
	}
	rDB.read((char*)(&blkNum), 4);
	rDB.read((char*)(&size), 4);
	rDB.read((char*)(&level), 4);
	if (rDB.fail()){
		*bk = nullptr;
		return;
	}
	assert(blk == blkNum);
	for (int i = 0; i<size; ++i) {
		rDB.read((char*)(&t_aid), 4);
		rDB.read(t_n, 20);
		rDB.read((char*)(&salary), 4);
		Professor pro;
		pro.ProfID = t_aid;
		int len = MyStrCpy(pro.name, t_n);
		if (len<20)
			pro.name[len] = '\0';
		pro.Salary = salary;
		(*bk)->insert(&pro);
	}
}
void FileManager::bucketLoad(StudentBucket** bk,int blk,ifstream &rDB)
{
	*bk = new StudentBucket();
	int t_aid, t_sid,level,size,blkNum;
	float t_score;
	char t_n[20];
	// ���ϴ� ��ġ�� �̵� (������ �̵�)
	
	rDB.seekg((blk-StudentBucket::initNum)*BLOCK_SIZE);
	if (rDB.fail()){
		*bk = nullptr;
		return;
	}
	rDB.read((char*)(&blkNum), 4);
	rDB.read((char*)(&size), 4);
	rDB.read((char*)(&level), 4);
	if (rDB.fail()){
		*bk = nullptr;
		return;
	}
	for(int i=0;i<size;++i){
		rDB.read((char*)(&t_aid), 4);
		rDB.read(t_n, 20);
		rDB.read((char*)(&t_sid), 4);
		rDB.read((char*)(&t_score), 4);
		Student stu;
		stu.advisorID = t_aid;
		int len = MyStrCpy(stu.name, t_n);
		if (len<20)
			stu.name[len] = '\0';
		stu.score = t_score;
		stu.studentID = t_sid;
		(*bk)->insert(&stu);
	}
}

vector<Student> FileManager::readStudent(HashTable& tlb, BPlusTree& tree)
{
	ifstream ifs("student_data.csv");
	int num,count=0;
	char buffer[1024];
	ifs >> num;
	ifs.getline(buffer,1024);
	vector<Student> students;
	students.resize(3000);
	for (int i = 0; i<num; ++i) {
		ifs.getline(buffer, 1024);
		char* buff = buffer;
		int subFloat;
		Student stu;
		int next = 0;
		char *data;
		
		data = strtok(buff, ",");
		strcpy(stu.name, data);
		next = strlen(data);
		buff = buff + next + 1;

		data = strtok(buff, ",");
		stu.studentID = atoi(data);
		next = strlen(data);
		buff = buff + next + 1;

		data = strtok(buff, ",");
		stu.score = atof(data);
		next = strlen(data);
		buff = buff + next + 1;
		
		subFloat = data[0] - '0';
		int j = 1;
		for (; data[j] != '\0'; j++) {
			
			if (data[j] == '.')
				continue;
			subFloat = (subFloat * 10) + (data[j] - '0');
		}

		if (j == 1)
			j = 2;
		for (; j < 10; j++)
			subFloat *= 10;
		data = strtok(buff, ",");
		stu.advisorID = atoi(data);
		tlb.insert(stu);
		tree.insert(subFloat, stu.studentID);
	}
	ifs.close();
	return students;
}

vector<Professor> FileManager::readProfessor(HashTable & tlb, BPlusTree & tree)
{
	ifstream ifs("prof_data.csv");
	int num, count = 0;
	char buffer[1024];
	ifs >> num;
	ifs.getline(buffer, 1024);
	vector<Professor> professors;
	professors.resize(3000);
	for (int i = 0; i<num; ++i) {
		ifs.getline(buffer, 1024);
		char* buff = buffer;
		Professor pro;
		int next = 0;
		char *data;

		data = strtok(buff, ",");
		strcpy(pro.name, data);
		next = strlen(data);
		buff = buff + next + 1;

		data = strtok(buff, ",");
		pro.ProfID = atoi(data);
		next = strlen(data);
		buff = buff + next + 1;

		data = strtok(buff, ",");
		pro.Salary = atoi(data);
		tlb.insert(pro);
		tree.insert(pro.Salary, pro.ProfID);
	}
	ifs.close();
	return professors;
}
