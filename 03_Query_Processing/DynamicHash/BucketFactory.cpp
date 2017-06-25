#include "BucketFactory.h"
#include "StudentBucket.h"
#include "ProfcessorBucket.h"
#pragma once

Bucket * BucketFactory::createBucket(Type t)
{
	if (t == Student)
		return new StudentBucket();
	else if (t == Profcessor)
		return new ProfessorBucket();
	
}
