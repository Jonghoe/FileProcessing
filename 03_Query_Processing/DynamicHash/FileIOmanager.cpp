#include"FileIOManager.h"
#include"StudentBucket.h"
#include<cassert>
#include<direct.h>

void FileManager::hashsave(const HashTable& tlb)
{
	const vector<int>& tb = tlb.getTable();
	string hashPath = "Students.hash";
	ofstream writeHash(hashPath.data(), ios::binary);	
	for (unsigned i = 0; i < tb.size(); ++i) {
		writeHash.write(reinterpret_cast<char*>(&i), 4);
		writeHash.write(reinterpret_cast<const char*>(&tb[i]), 4);
	}
	writeHash.close();
}
vector<int> FileManager::hashload()
{
	int i, temp;
	vector<int> ht;
	string hashPath = "Students.hash";
	ifstream hashl(hashPath.data(), ios::binary);
	do{
		hashl.read(reinterpret_cast<char*>(&i), 4);
		hashl.read(reinterpret_cast<char*>(&temp),4);
		ht.push_back(temp);
	} while (!hashl.eof());
	ht.pop_back();
	return ht;
}

Bucket* FileManager::bucketSave(const ProfessorBucket& bk, ofstream& wDB)
{
	return nullptr;
}

Bucket* FileManager::bucketSave(const StudentBucket& bk,ofstream& wDB)
{
	// ���ϴ� ��ġ�� �̵� (������ �̵�)	
	wDB.seekp(bk.getBlkNum()*BLOCK_SIZE);
	if (wDB.fail() != 0)
		return nullptr;
	int blkNum = bk.getBlkNum();
	int size = bk.getSize();
	int level = bk.getLevel();

	wDB.write(reinterpret_cast<char*>(&blkNum), 4);
	wDB.write(reinterpret_cast<char*>(&size), 20);
	wDB.write(reinterpret_cast<char*>(&level), 4);
	for (int j = 0; j<bk.getSize(); ++j) {
		int aid = bk[j].advisorID;
		float score = bk[j].score;
		int sid = bk[j].studentID;
		wDB.write(reinterpret_cast<char*>(&aid), 4);
		wDB.write(bk[j].name, 20);
		wDB.write(reinterpret_cast<char*>(&sid), 4);
		wDB.write(reinterpret_cast<char*>(&score), 4);
	}
	// ���� ũ�� ��ŭ ������ ����� ä���.
	int buffer[5]={ 0,0,0,0,0 };
	for (int j = 0; j < bk.getCapacity() - bk.getSize(); ++j) {
		wDB.write(reinterpret_cast<char*>(buffer), 4);
		wDB.write(reinterpret_cast<char*>(buffer), 20);
		wDB.write(reinterpret_cast<char*>(buffer), 4);
		wDB.write(reinterpret_cast<char*>(buffer), 4);
	}
}
void FileManager::bucketLoad(ProfessorBucket* bk, int blk, ifstream &rDB)
{

}
void FileManager::bucketLoad(StudentBucket* bk,int blk,ifstream &rDB)
{
	bk = new StudentBucket();
	int t_aid, t_sid,level,size,blkNum;
	float t_score;
	char t_n[20];
	// ���ϴ� ��ġ�� �̵� (������ �̵�)
	rDB.seekg(blk*BLOCK_SIZE);
	if (rDB.fail())
		bk = nullptr;
	rDB.read(reinterpret_cast<char*>(&blkNum), 4);
	rDB.read(reinterpret_cast<char*>(&size), 4);
	rDB.read(reinterpret_cast<char*>(&level), 4);
	if (rDB.fail())
		bk = nullptr;
	assert(blk == blkNum);
	for(int i=0;i<size;++i){
		rDB.read(reinterpret_cast<char*>(&t_aid), 4);
		rDB.read(t_n, 20);
		rDB.read(reinterpret_cast<char*>(&t_sid), 4);
		rDB.read(reinterpret_cast<char*>(&t_score), 4);
		Student stu;
		stu.advisorID = t_aid;
		int len = MyStrCpy(stu.name, t_n);
		if (len<20)
			stu.name[len] = '\0';
		stu.score = t_score;
		stu.studentID = t_sid;
		bk->insert(&stu);
	}
}

vector<Student> FileManager::readStudent(HashTable& tlb, BPlusTree& tree)
{
	ifstream ifs("SampleData_edited.csv");
	int num,count=0;
	char buffer[1024];
	ifs >> num;
	ifs.getline(buffer,1024);
	vector<Student> students;
	students.resize(3000);
	for (int i = 0; i<num; ++i) {
		ifs.getline(buffer, 1024);
		char* buff = buffer;
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

		data = strtok(buff, ",");
		stu.advisorID = atoi(data);
		tlb.insert(stu);
		tree.insert(stu.score * 100000, stu.studentID);		
	}
	ifs.close();
	tree.print();
	return students;
}

vector<Professor> FileManager::readProfessor(HashTable & tlb, BPlusTree & tree)
{
	return vector<Professor>();
}
