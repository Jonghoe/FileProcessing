#include"FileIOManager.h"

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
void FileManager::DBsave(const HashTable& tlb)
{
	const vector<Bucket*> buck = tlb.getBucket();
	string filePath = "Student.DB";
	ofstream writeFile(filePath.data(), ios::binary);
	for (int i = 0; i<buck.size(); ++i) {
		bucketSave(*buck[i]);
	}
	writeFile.close();
}
Bucket* FileManager::bucketSave(const Bucket& bk)
{
	string hashPath = "Student.DB";
	ofstream wDB(hashPath.data(), ios::binary);
		
	// 원하는 위치로 이동 (블럭으로 이동)	
	wDB.seekp(bk.getBlkNum()*BLOCK_SIZE);
	if (wDB.fail() != 0)
		return nullptr;

	wDB.write(reinterpret_cast<char*>(bk.getBlkNum()), 4);
	wDB.write(reinterpret_cast<char*>(bk.getSize()), 20);
	wDB.write(reinterpret_cast<char*>(bk.getLevel()), 4);
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
	for (int j = 0; j < bk.getCapacity() - bk.getSize(); ++j) {
		wDB.write(reinterpret_cast<char*>(0), 4);
		wDB.write(reinterpret_cast<char*>(0), 20);
		wDB.write(reinterpret_cast<char*>(0), 4);
		wDB.write(reinterpret_cast<char*>(0), 4);
	}
}
Bucket* FileManager::bucketLoad(int blk)
{
	string hashPath = "Student.DB";
	ifstream rdb(hashPath.data(), ios::binary);
	Bucket* bk = new Bucket();
	int t_aid, t_sid;
	float t_score;
	char t_n[20];
	// 원하는 위치로 이동 (블럭으로 이동)
	rdb.seekg(blk*BLOCK_SIZE);
	rdb.read(reinterpret_cast<char*>(&t_aid), 4);
	rdb.read(t_n, 20);
	rdb.read(reinterpret_cast<char*>(&t_sid), 4);
	rdb.read(reinterpret_cast<char*>(&t_score), 4);
	Student stu;
	stu.advisorID = t_aid;
	int len = MyStrCpy(stu.name, t_n);
	if (len<20)
		stu.name[len] = '\0';
	stu.score = t_score;
	stu.studentID = t_sid;
	bk->insert(stu);
}
vector<Bucket*> FileManager::bucketload()
{
	unsigned t_bln, t_size, t_l, t_aid, t_sid;
	float t_score;
	char t_n[21];
	vector<Bucket*> bks;
	string hashPath = "Student.DB";
	ifstream rdb(hashPath.data(), ios::binary);		
	do {			
		rdb.read(reinterpret_cast<char*>(&t_bln), 4);
		rdb.read(reinterpret_cast<char*>(&t_size), 4);
		rdb.read(reinterpret_cast<char*>(&t_l), 4);
		Bucket* bk = new Bucket();
		for (int i = 0; i<t_size; ++i) {
			rdb.read(reinterpret_cast<char*>(&t_aid), 4);
			rdb.read(t_n,20);
			rdb.read(reinterpret_cast<char*>(&t_sid), 4);
			rdb.read(reinterpret_cast<char*>(&t_score), 4);
			Student stu;
			stu.advisorID = t_aid;
			int len =MyStrCpy(stu.name, t_n);
			if(len<20)
				stu.name[len] = '\0';
			stu.score = t_score;
			stu.studentID = t_sid;
			bk->insert(stu);
		}
		bks.push_back(bk);
		rdb.seekg(BLOCK_SIZE*bks.size());
	} while (!rdb.eof());
	bks.pop_back();
	return bks;
}
vector<Student> FileManager::readcsv(HashTable& tlb, BPlusTree& tree)
{
	ifstream ifs("SampleData.csv");
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

		students[count]=stu;
		++count;
		if (count > 2999 || i == num - 1) {				
			for (int s = 0; s < count; ++s) {
				tlb.insert(students[s]);
				tree.insert(students[s].score, students[s].studentID);
			}
			count = 0;
			cout << i << endl;
		}
	}
	ifs.close();
	tree.print();
	return vector<Student>();
}
