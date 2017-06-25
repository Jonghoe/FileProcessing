#pragma once
#include"Bucket.h"
class BucketFactory {
public:
	enum Type{student,professor};
	static Bucket* createBucket(Type t,int level=1);
};