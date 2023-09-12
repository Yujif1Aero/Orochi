//template<typename T>
//__global__ void MatrixMultiplicationKernel( const T* A, const T* B, T* C, int* rowm, int* colummn, int* rowp, int* colummq ) 


//extern "C" __global__ void MatrixMultiplicationKernel( const float* A, const float* B, float* C, int* rowm, int* colummn, int* rowp, int* colummq )
template<typename T> 
__device__ void MatrixMultiplicationKernelT( const T* A, const T* B, T* C, int* rowm, int* colummn, int* rowp, int* colummq )
{

	int numrow = blockDim.x * blockIdx.x + threadIdx.x;

	
	int  numcol = blockDim.y * blockIdx.y + threadIdx.y;
	
	//printf( "blockdx.y   blockDim.y  threadIdx.y   gridDim.y  =%d,  %d, %d,  %d\n", blockIdx.y, blockDim.y, threadIdx.y, gridDim.y );
	//printf( "blockdx.x   blockDim.x  threadIdx.x   gridDim.x  =%d,  %d, %d,  %d\n", blockIdx.x, blockDim.x, threadIdx.x, gridDim.x );

	//printf( " numcol   numrow =%d,  %d\n", numcol, numrow );

	// for( int j = 0; j < *rowm; ++j )
	//{
	//	for( int h = 0; h < *colummn; ++h )
	//	{
	//		printf( "A=%f \n", A[(*rowm)*j + h] );
	//	}
	// }

	// for( int i = 0; i < *rowp; ++i )
	//{
	//	for( int g = 0; g < *colummq; ++g )
	//	{
	//		printf( "B=%f\n", B[ (*rowp)*i + g ] );
	//	}
	// }

	if( numrow < *rowm && numcol < *colummq )
	{
		float sum = 0.0;
		for( int k = 0; k < *colummn; ++k )
		{
			sum += A[( *colummn ) * numrow + k] * B[( *colummq ) * k + numcol];
			//printf( "sum= %f", sum );
		}
		C[( *colummq ) * numrow + numcol] = sum;

		
	}
	


	//printf( "sum=%f\n", sum );
}

extern "C" __global__ void MatrixMultiplicationKernel( const void* A, const void* B, void* C, int* rowm, int* colummn, int* rowp, int* colummq, int *size ) {
//	printf( "%d \n", sizeof( float ) );
//	printf( "%d \n", *size );
	if( *size == sizeof( float ) )
	{
		MatrixMultiplicationKernelT<float>( (const float*)A, (const float*)B, (float*)C, rowm, colummn, rowp, colummq );
		//printf( "float size is %d\n", *size );
	}
	if( *size == sizeof( double ) )
	{
		MatrixMultiplicationKernelT<double>( (const double*)A, (const double*)B, (double*)C, rowm, colummn, rowp, colummq );
		//printf( "double size is %d\n", *size );
	}
}