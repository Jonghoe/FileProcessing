# [Assignment # 2]
Given two tables as follows;
 
Students(char name[20], unsigned studentID, float score, unsigned advisorID)
 
Note that studentID is a key of the table. We assume that the block size is 4096 bytes.
 
We insert records of Students into files Students.DB. Each record is inserted in a block without overlapping into two blocks.

## Sequences
1] Implement dynamic hashing on Students with studentID where the hash table is stored in Students.hash. Note that this hash table should be stored in binary. You may use any kind of hashing function for this dynamic hashing.
 
2] Implement B+-tree on Students with score, where this index is stored in Students_score.idx. Note that this index should be stored in binary and the block number is determined by the dynamic hashing. 
 
3] After having inserted certain number of students, print Students.hash and k-th leaf node of B+-tree in ASCII format to console, where k is given from keyboard.
 
The format of input data:
N // number of student records
Julius Cesare, 201512345, 3.6, 123945
…
Immanuel Kant, 190234952, 2.4, 123921