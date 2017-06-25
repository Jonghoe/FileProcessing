#include"FileIOManager.h"
#include<cassert>
#include<direct.h>

void FileManager::hashsave(const HashTable& tlb)
{
	const vector<int>& tb = tlb.getTable();
	string hashPath = "Students.hash";
	ofstream writeHash(hashPath.data(), ios::binary);	
	for (int i = 0; i < tb.size(); ++i) {
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
// 디비 저장
void FileManager::DBsave(HashTable& tlb)
{
	vector<Bucket*> buck = tlb.getBucket();
	string filePath = "Student.DB";
	ofstream writeFile(filePath.data(), ios::binary);
	for (auto it = buck.begin(); it != buck.end(); ++it) {
		bucketSave(**it,tlb.getDBOStream());
	}
	writeFile.close();
}
Bucket* FileManager::bucketSave(const Bucket& bk,ofstream& wDB)
{
	// 원하는 위치로 이동 (블럭으로 이동)	
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
	// 블럭의 크기 만큼 데이터 빈공간 채우기.
	int buffer[5]={ 0,0,0,0,0 };
	for (int j = 0; j < bk.getCapacity() - bk.getSize(); ++j) {
		wDB.write(reinterpret_cast<char*>(buffer), 4);
		wDB.write(reinterpret_cast<char*>(buffer), 20);
		wDB.write(reinterpret_cast<char*>(buffer), 4);
		wDB.write(reinterpret_cast<char*>(buffer), 4);
	}
}
Bucket* FileManager::bucketLoad(int blk,ifstream &rDB)
{
	Bucket* bk = new Bucket();
	int t_aid, t_sid,level,size,blkNum;
	float t_score;
	char t_n[20];
	// 원하는 위치로 이동 (블럭으로 이동)
	rDB.seekg(blk*BLOCK_SIZE);
	if (rDB.fail())
		return nullptr;
	rDB.read(reinterpret_cast<char*>(&blkNum), 4);
	rDB.read(reinterpret_cast<char*>(&size), 4);
	rDB.read(reinterpret_cast<char*>(&level), 4);
	if (rDB.fail())
		return nullptr;
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
		bk->insert(stu);
	}
	return bk;
}
vector<Bucket*> FileManager::bucketload()
{
	unsigned t_bln, t_size, t_l, t_aid, t_sid;
	float t_score;
	char t_n[21];
	vector<Bucket*> bks;
	int i = 0;
	Bucket* bk = nullptr;
	while ((bk=bucketLoad(i, ifstream("Student.DB", ios::binary))) != nullptr){
		if (bk != nullptr){
			bks.push_back(bk);
			++i;
		}
	}	
	return bks;
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

vector<Student> FileManager::readProfessor(HashTable & tlb, BPlusTree & tree)
{
	return vector<Student>();
}
