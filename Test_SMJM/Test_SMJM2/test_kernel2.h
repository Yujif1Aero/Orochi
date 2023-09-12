extern "C" __global__ void test_kernel2( void )
{
	int a = threadIdx.x;
	printf("thread %d running\n", a);
	
}