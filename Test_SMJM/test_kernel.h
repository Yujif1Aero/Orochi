#define CALVECTODOTRPRODUCT
#ifdef CALVECTODOTRPRODUCT
#define BLOCK_SIZEx 1024 // threadsPerBlock
#define VECTORNUMx 2048*1000
#define GRID_SIZEx ( VECTORNUMx + BLOCK_SIZEx - 1 ) / BLOCK_SIZEx // blocksPerGrid
//#define iimachigae
#define try
template<typename T>
__device__ void MatrixMultiplicationKernelT( const T* A, const T* B, T* C, int* rowm, int* colummn, int* rowp, int* colummq )
{
#ifdef iimachigae
	int bx = blockIdx.x;
	// int by = blockIdx.y;
	int tx = threadIdx.x;
	// int ty = threadIdx.y;

	int blockstartA = bx * blockDim.x;
	int blockendA = bx * blockDim.x + blockDim.x - 1;

	int indexA = bx * blockDim.x + tx;
	// int indexB = indexA;

//	printf( " rowm,	colummn,	rowp,	colummq	 =	%d,	%d,	%d,	%d\n", *rowm, *colummn, *rowp, *colummq );

	// printf( "blockdx.y,	blockDim.y	threadIdx.y	gridDim.y	=	%d,	%d,	%d,	%d\n", blockIdx.y, blockDim.y, threadIdx.y, gridDim.y );
	printf( "blockIdx.x,	blockDim.x	threadIdx.x	gridDim.x	=	%d,	%d,	%d,	%d\n", bx, blockDim.x, tx, gridDim.x );
	printf( "indexA,	blockstartA,	blockendA=	%d,	%d,	%d \n", indexA, blockstartA, blockendA );
	T sum;
	// T Cresult[GRID_SIZEx];
	__shared__ T Cresult[BLOCK_SIZEx]; // blockDim.x is no available to make arry
	printf( "GRID_SIZEx=%d\n", GRID_SIZEx );
	//printf( " A[%d]= %f, B[%d] =%f\n", tx, A[tx], tx, B[tx] );

	// for( int k = blockstartA ; k < blockendA ; k++ )
	//{
	__syncthreads();
	//  Cresult[0] += A[tx] * B[tx];
	Cresult[bx] += A[indexA] * B[indexA];
    //sum = A[indexA] * B[indexA];
	// printf( "k= %d   blockldx.x=%d  results= %f \n", k, bx, Cresult[bx] );
	//printf( "threadIdx.x=%d,  sum= %f \n", tx, sum );
//	printf( "bx * blockDim.x + tx =%d,  sum= %f \n", indexA, sum );

	// printf( "k= %d  \n",bx );
	//	}

	//	for( int j = 0; j < blockIdx.x; ++j )
	//	{
	//Cresult[bx] = +sum;
	//printf( "bx  =%d,   Cresult[%d]= %f \n", bx,  bx ,Cresult[bx] );
	printf( "indexA  =%d,   Cresult[%d]= %f \n", indexA, bx, Cresult[bx] );
	//	}
	__syncthreads();
	for( int k = 0; k < GRID_SIZEx; ++k )
	{   
        printf( "Cresult[%d]= %f \n", k, Cresult[k] );
		printf( "blockIdx.x,	blockDim.x	threadIdx.x	gridDim.x	=	%d,	%d,	%d,	%d\n", bx, blockDim.x, tx, gridDim.x );
		printf( "indexA,	blockstartA,	blockendA=	%d,	%d,	%d \n", indexA, blockstartA, blockendA );
		C[0] += Cresult[k];
	}
	//
	//	printf( "sum=%f\n", sum );

#endif // iimachigae

#ifdef try

	int bx = blockIdx.x;
	// int by = blockIdx.y;
	int tx = threadIdx.x;
	// int ty = threadIdx.y;

	//int blockstartA = bx * blockDim.x;
	//int blockendA = bx * blockDim.x + blockDim.x - 1;

	int indexA = bx * blockDim.x + tx;
	// int indexB = indexA;


	//printf( "blockIdx.x,	blockDim.x	threadIdx.x	gridDim.x	=	%d,	%d,	%d,	%d\n", bx, blockDim.x, tx, gridDim.x );
	//printf( "indexA,	blockstartA,	blockendA=	%d,	%d,	%d \n", indexA, blockstartA, blockendA );
	T sum= (T) 0.0;

	__shared__ T Cresult[BLOCK_SIZEx]; // blockDim.x is no available to make arry
	//printf( "GRID_SIZEx=%d\n", GRID_SIZEx );
	//printf( " A[%d]= %f, B[%d] =%f\n", indexA, A[indexA], indexA, B[indexA] );


	//__syncthreads();
	while( indexA < *colummn ) 
	{
		sum += A[indexA] * B[indexA];
		//printf( "Before blockIdx.x,	blockDim.x	threadIdx.x	gridDim.x,  indexA	A[indexA]   B[indexA]  =	%d,	%d,	%d,	%d, %d, %f, %f \n", bx, blockDim.x, tx, gridDim.x, indexA, A[indexA], B[indexA] );
		indexA += blockDim.x * gridDim.x;
		//printf( "After blockIdx.x,	blockDim.x	threadIdx.x	gridDim.x,  indexA	A[indexA]   B[indexA]  =	%d,	%d,	%d,	%d, %d, %f, %f \n", bx, blockDim.x, tx, gridDim.x, indexA, A[indexA], B[indexA] );

	}
	Cresult[tx] = sum;
	__syncthreads();
    

	//printf( "bx * blockDim.x + tx =%d,  sum= %f \n", indexA, sum );
    //printf( "bx  =%d,   Cresult[%d]= %f \n", bx,  bx ,Cresult[bx] );
	//__syncthreads();
	if( tx == 0 )
	{

		T CresultBlock = (T) 0.0;
		for( int k = 0; k < blockDim.x; ++k )
		{
			//printf( "Cresult[%d]= %f \n", k, Cresult[k] );
			//printf( "blockIdx.x,	blockDim.x	threadIdx.x	gridDim.x	=	%d,	%d,	%d,	%d\n", bx, blockDim.x, tx, gridDim.x );
			//printf( "indexA,	blockstartA,	blockendA=	%d,	%d,	%d \n", indexA, blockstartA, blockendA );
			CresultBlock+= Cresult[k];
		}
        printf( "CresultBlock[%d]=%f\n", bx, CresultBlock );
		atomicAdd( C, CresultBlock ); // double is not available
	}
	//
	

    #endif //try

}
#endif // CALVECTODOTRPRODUCT

extern "C" __global__ void MatrixMultiplicationKernel( const void* A, const void* B, void* C, int* rowm, int* colummn, int* rowp, int* colummq, int* size )
{
	//	printf( "%d \n", sizeof( float ) );
	//	printf( "%d \n", *size );
	if( *size == sizeof( float ) )
	{
		MatrixMultiplicationKernelT<float>( (const float*)A, (const float*)B, (float*)C, rowm, colummn, rowp, colummq );
		//printf( "float size is %d\n", *size );
	}
	/*if( *size == sizeof( double ) )
	{
		MatrixMultiplicationKernelT<double>( (const double*)A, (const double*)B, (double*)C, rowm, colummn, rowp, colummq );
		printf( "double size is %d\n", *size );
	}*/
}