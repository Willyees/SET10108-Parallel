__kernel void vecadd(__global int* A, __global int* B, __global int* C){
	//get the work item's unique ID
	int idx = get_global_id(0);
	//add corresponding locations of A nd B and store into C 
	C[idx] = A[idx] + B[idx];

}