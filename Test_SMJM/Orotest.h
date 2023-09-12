extern "C" __global__ void Orotest()
{

	int a = threadIdx.x;
	printf( "	thread %d running\n", a );
}