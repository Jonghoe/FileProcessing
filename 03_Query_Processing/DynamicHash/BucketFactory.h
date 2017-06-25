#pragma once
#include"Bucket.h"
class BucketFactory {
public:
	enum Type{Student,Profcessor};
	static Bucket* createBucket(Type t);
};