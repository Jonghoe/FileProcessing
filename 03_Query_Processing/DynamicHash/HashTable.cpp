#include"StudentBucket.h"
#include"ProfcessorBucket.h"

#include "HashTable.h"
#include "FileIOManager.h"
#include<cassert>
#include<algorithm>
#include<fstream>
#include<iostream>


HashTable::HashTable(BucketFactory::Type t) :type(t),table(2), MASK(1), maxLevel(1), buckets()
{
	if (type == BucketFactory::Type::professor) {
		out = ofstream("Professors.DB");
		in = ifstream("Professors.DB");
	}
	else if (type == BucketFactory::Type::student) {
		out = ofstream("Students.DB");
		in = ifstream("Students.DB");
	}
	createBucket(0);
	createBucket(1);
	table[0] = 0;
	table[1] = 1;
}


HashTable::HashTable(BucketFactory::Type t,const vector<int>& tb, const vector<Bucket*>& b) :type(t)
{
	if (type == BucketFactory::Type::professor) {
		out = ofstream("Professors.DB");
		in = ifstream("Professors.DB");
	}
	else if (type == BucketFactory::Type::student) {
		out = ofstream("Students.DB");
		in = ifstream("Students.DB");
	}
	table = tb;
	maxLevel = 0;

	for (; pow(2, maxLevel) < tb.size(); ++maxLevel)
		;
	for (unsigned i = 0; i < b.size(); ++i) {
		buckets.push_back(b[i]);
	}
}


bool HashTable::modifyBlock(int blk)
{
	
	auto it = buckets.begin();
	for (; it != buckets.end() && (*it)->getBlkNum() != blk; ++it);
	if (it != buckets.end())
		return true;
	FileManager fm;
	// 메모리에서 빼기전에 저장
	if (buckets.size() >= 500){		
		FileManager fm;
		if (type == BucketFactory::Type::student) {
			StudentBucket* sBuck = (StudentBucket*)buckets.back();
			fm.bucketSave(*sBuck, out);
		}
		
		buckets.pop_back();
	}
	Bucket* newBlk=nullptr;
	if (type == BucketFactory::Type::student) {
		StudentBucket* temp = ((StudentBucket*)newBlk);
		fm.bucketLoad(&temp, blk, in);
	}
	else if (type == BucketFactory::Type::professor) {
		ProfessorBucket* temp = ((ProfessorBucket*)newBlk);
		fm.bucketLoad(&temp, blk, in);
	}
	if (newBlk != nullptr) {
		buckets.push_back(newBlk);
		return true;
	}
	else {
		return false;
	}
}

void HashTable::bucketSave(Bucket* buck)
{
	FileManager fm;
	if (type == BucketFactory::Type::student)
		fm.bucketSave(*(StudentBucket*)buck, out);
	else if (type == BucketFactory::Type::professor)
		fm.bucketSave(*(ProfessorBucket*)buck, out);
}
void HashTable::createBucket(int blk)
{
	buckets.push_back(BucketFactory::createBucket(type));
	bucketSave(buckets.back());
}


void HashTable::createBucket(int blk,int level)
{	
	buckets.push_back(BucketFactory::createBucket(type,level));
	bucketSave(buckets.back());
}


Bucket* HashTable::getBlock(int blk)const
{
	int block = blk;
	if (type == BucketFactory::Type::professor)
		block -= ProfessorBucket::initNum;
	if (buckets[blk]->getBlkNum() == block) {
		return buckets[blk];
	}
	CheckBlk func(blk);
	auto it = find_if(buckets.begin(), buckets.end(),func);
	if (it == buckets.end())
		return nullptr;
	return *it;
}


Bucket* HashTable::getBlock(int blk)
{
	int block = blk;
	if (type == BucketFactory::Type::professor)
		block += ProfessorBucket::initNum;
	if (buckets[blk]->getBlkNum() == block) {
		return buckets[blk];
	}
	CheckBlk func(blk);
	auto it = find_if(buckets.begin(), buckets.end(), func);
	if(it==buckets.end())
		return nullptr;
	sort(buckets.begin(),buckets.end());
	return *it;
}
void HashTable::insert(Professor& record)
{
	assert(type == BucketFactory::Type::professor);
	// hash값을 찾고
	int hash = findHash(record.ProfID);
	// hash값을 이용하여 bucket Num을 찾는다
	int blkNum = table[hash];
	// 입력때 오버플로우 발생 시 err 는 -1이 됨
	int err = getBlock(blkNum)->insert(&record);
	if (err == -1) {
		// 오버플로우난 bucket의 레벨에 맞게금 mask와 hash 조정
		int fitMask = (1 << (getBlock(blkNum)->getLevel() - 1)) - 1;
		int fitHash = (fitMask)&hash;
		// 오버플로우 발생하여 새로 생길 버킷에 대한 키 값(최상위 비트에 1추가)
		int half = fitHash | (fitMask + 1);
		// 오버플로우 발생한 버킷의 레벨이 테이블 레벨보다 크면
		// 테이블의 크기를 2배로 확장시키고 마스크 값과 테이블의 레벨을 조정한다.
		bool needChange = maxLevel < getBlock(blkNum)->getLevel();
		if (needChange) {
			modifyTable();
			maxLevel = getBlock(blkNum)->getLevel();
			modifyMask();
		}
		// 버킷을 생성
		createBucket(blkNum, getBlock(blkNum)->getLevel());
		// 테이블에 버킷number 저장
		int alpha = 0;
		if (type == BucketFactory::Type::professor)
			alpha = ProfessorBucket::initNum;
		table[half] = buckets.back()->getBlkNum()-alpha;
		// 테이블 조정
		backTable(half, buckets.size() - 1, buckets.back()->getLevel());
		// 새로운 키값에 대한 이동
		move(fitHash, half);
		// 다시 삽입.
		insert(record);
	}
}

void HashTable::insert(Student& record)
{

	assert(type == BucketFactory::Type::student);
	// hash값을 찾고
	int hash = findHash(record.studentID);
	// hash값을 이용하여 bucket Num을 찾는다
	int blkNum = table[hash];
	// 입력때 오버플로우 발생 시 err 는 -1이 됨
	int err = getBlock(blkNum)->insert(&record);	
	if (err == -1 ) {
		// 오버플로우난 bucket의 레벨에 맞게금 mask와 hash 조정
		int fitMask = (1 << (getBlock(blkNum)->getLevel() - 1)) - 1;
		int fitHash = (fitMask)&hash;
		// 오버플로우 발생하여 새로 생길 버킷에 대한 키 값(최상위 비트에 1추가)
		int half = fitHash | (fitMask + 1);	
		// 오버플로우 발생한 버킷의 레벨이 테이블 레벨보다 크면
		// 테이블의 크기를 2배로 확장시키고 마스크 값과 테이블의 레벨을 조정한다.
		bool needChange = maxLevel < getBlock(blkNum)->getLevel();
		if (needChange) {
			modifyTable();
			maxLevel = getBlock(blkNum)->getLevel();
			modifyMask();
		}
		// 버킷을 생성
		createBucket(blkNum, getBlock(blkNum)->getLevel());
		// 테이블에 버킷number 저장
		table[half] = buckets.back()->getBlkNum();
		// 테이블 조정
		backTable(half, buckets.size() - 1, buckets.back()->getLevel());
		// 새로운 키값에 대한 이동
		move(fitHash, half);
		// 다시 삽입.
		insert(record);
	}
}

int HashTable::findHash(unsigned key)const
{
	return key & MASK;
}

void HashTable::printTable()const
{
	if (type == BucketFactory::Type::student)
		cout << "Student Table" << endl;
	else if(type == BucketFactory::Type::professor)
		cout << "Professor Table" << endl;
	for (unsigned i = 0; i < table.size(); ++i) {
		cout << "Idx[" << i << "] ="<<table[i]<<endl;		
	}	
}



const vector<Bucket*>& HashTable::getBucket()const
{
	return buckets;
}


const vector<int>& HashTable::getTable()const
{
	return table;
}


unsigned HashTable::getBlkNum(int key) const
{
	return table[findHash(key)];
}


bool HashTable::check(unsigned key)const
{
	int hash = findHash(key);
	int blkNum = table[hash];
	for (int i = 0; i < getBlock(blkNum)->getSize(); ++i) {
		if(type==BucketFactory::student){
			StudentBucket* sBucket = (StudentBucket*)getBlock(blkNum);
			if((*sBucket)[i].studentID ==key){
				return true;
			}
		}
	}
	return false;
}

HashTable::~HashTable()
{
	for (unsigned i = 0; i < buckets.size(); ++i)
		delete getBlock(i);
}

void HashTable::printBucket() const
{
	if (type == BucketFactory::Type::student) {
		cout << "StudentsData" << endl;
	}
	else {
		cout << "ProfessorData" << endl;
	}
	for (int i = 0; i < buckets.size(); ++i) {
		for (int b = 0; b < buckets[i]->getSize(); ++b) {
			if (type == BucketFactory::Type::student) {
				StudentBucket& buck = *(StudentBucket*)buckets[i];
				cout << buck[b].name << " " << buck[b].studentID << " " << buck[b].score << " " << buck[b].advisorID << endl;
			}
			else {
				ProfessorBucket& buck = *(ProfessorBucket*)buckets[b];
				cout << buck[b].name << " " << buck[b].ProfID << " " << buck[b].Salary << endl;
			}
		}
	}
}


void HashTable::modifyTable()
{
	//테이블의 크기를 2배로 늘리고 최상위 비트아래 비트들만 mask값을 이용
	table.resize(table.size() * 2);
	for (unsigned i = table.size() / 2; i < table.size(); ++i) {
		table[i] = table[i & MASK];
	}	
}


void HashTable::backTable(int hash,int blkNum,int level)
{	
	if (maxLevel <= level)
		return;	
	table[(1 << (level)) + hash] = blkNum;
	backTable(hash, blkNum, level + 1);
	backTable((1 << (level)) + hash, blkNum, level + 1);
}


void HashTable::modifyMask()
{
	MASK = (1 << maxLevel) - 1;
}



void HashTable::move(int first, int second)
{
	int src = table[first];
	int dst = table[second];
	if (type==BucketFactory::Type::student){
		int i = 0;
		StudentBucket& a = *(StudentBucket*)getBlock(src);
		StudentBucket& b = *(StudentBucket*)getBlock(dst);

		while( i < a.getSize()) {
			int localKey = a[i].studentID&( (1 << a.getLevel()) - 1);
			if (localKey == second) {
				b.insert(&a[i]);
				a.erase(i);
			}
			else {
				++i;
			}
		}
	}
	else if (type == BucketFactory::Type::professor) {
		int i = 0;
		ProfessorBucket& a = *(ProfessorBucket*)getBlock(src);
		ProfessorBucket& b = *(ProfessorBucket*)getBlock(dst);

		while (i < a.getSize()) {
			int localKey = a[i].ProfID&((1 << a.getLevel()) - 1);
			if (localKey == second) {
				b.insert(&a[i]);
				a.erase(i);
			}
			else {
				++i;
			}
		}
	}
}
int MyStrCpy(char* dst, const char* src)
{
	int i = 0;
	for (; *src != '\0'&&i<20; ++src, ++dst,++i) {
		*dst = *src;
	}
	return i;
}