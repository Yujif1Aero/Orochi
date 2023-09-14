#define CALVECTODOTRPRODUCT
#ifdef CALVECTODOTRPRODUCT
#define GRID_SIZEx 1
#define BLOCK_SIZEx 1024
template<typename T>
__device__ void MatrixMultiplicationKernelT( const T* A, const T* B, T* C, int* rowm, int* colummn, int* rowp, int* colummq )
{

	int bx = blockIdx.x;
	// int by = blockIdx.y;
	int tx = threadIdx.x;
	// int ty = threadIdx.y;

	int blockstartA = bx * blockDim.x;
	int blockendA = bx * blockDim.x + blockDim.x - 1;

	int indexA = bx * blockDim.x + tx;
	// int indexB = indexA;

	printf( " rowm,	colummn,	rowp,	colummq	 =	%d,	%d,	%d,	%d\n", *rowm, *colummn, *rowp, *colummq );

	// printf( "blockdx.y,	blockDim.y	threadIdx.y	gridDim.y	=	%d,	%d,	%d,	%d\n", blockIdx.y, blockDim.y, threadIdx.y, gridDim.y );
	printf( "blockIdx.x,	blockDim.x	threadIdx.x	gridDim.x	=	%d,	%d,	%d,	%d\n", bx, blockDim.x, tx, gridDim.x );
	printf( "indexA,	blockstartA,	blockendA=	%d,	%d,	%d \n", indexA, blockstartA, blockendA );
	T sum;
	// T Cresult[GRID_SIZEx];
	__shared__ T Cresult[BLOCK_SIZEx]; // blockDim.x is no available to make arry
	printf( "GRID_SIZEx=%d\n", GRID_SIZEx );
	printf( " A[%d]= %f, B[%d] =%f\n", tx, A[tx], tx, B[tx] );

	// for( int k = blockstartA ; k < blockendA ; k++ )
	//{
	__syncthreads();
	//  Cresult[0] += A[tx] * B[tx];
	sum = A[tx] * B[tx];
	// printf( "k= %d   blockldx.x=%d  results= %f \n", k, bx, Cresult[bx] );
	printf( "threadIdx.x=%d,  sum= %f \n", tx, sum );

	// printf( "k= %d  \n",bx );
	//	}

	//	for( int j = 0; j < blockIdx.x; ++j )
	//	{
	Cresult[tx] = sum;
	//	}
	__syncthreads();
	for( int k = 0; k < BLOCK_SIZEx; ++k )
	{
		C[0] += Cresult[k];
	}
	//
	//	printf( "sum=%f\n", sum );
}
#endif // CALVECTODOTRPRODUCT

extern "C" __global__ void MatrixMultiplicationKernel( const void* A, const void* B, void* C, int* rowm, int* colummn, int* rowp, int* colummq, int* size )
{
	//	printf( "%d \n", sizeof( float ) );
	//	printf( "%d \n", *size );
	if( *size == sizeof( float ) )
	{
		MatrixMultiplicationKernelT<float>( (const float*)A, (const float*)B, (float*)C, rowm, colummn, rowp, colummq );
		printf( "float size is %d\n", *size );
	}
	if( *size == sizeof( double ) )
	{
		MatrixMultiplicationKernelT<double>( (const double*)A, (const double*)B, (double*)C, rowm, colummn, rowp, colummq );
		printf( "double size is %d\n", *size );
	}
}