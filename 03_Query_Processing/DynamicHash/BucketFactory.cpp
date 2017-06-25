#pragma once
#include "BucketFactory.h"
#include "StudentBucket.h"
#include "ProfcessorBucket.h"

Bucket * BucketFactory::createBucket(Type t,int level)
{
	if (t == student)
		return new StudentBucket(level);
	else if (t == professor)
		return new ProfessorBucket(level);
}
