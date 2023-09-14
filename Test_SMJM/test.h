#pragma once
#define BLOCK_SIZEx 1024 // threadsPerBlock 
#define VECTORNUMx 1024 
#define GRID_SIZEx  (VECTORNUMx + BLOCK_SIZEx -1 ) / BLOCK_SIZEx //blocksPerGrid  
using T = float;
template<typename T>
class Matrix
{

  private:
	int rows_;
	int cols_;
	std::vector<T> data_;

  public:
	Matrix( int rows, int cols ) : rows_( rows ), cols_( cols ), data_( rows * cols ) {}

	// T& operator()( int row, int col ) { return data_[row * cols_ + col]; }
	// const T& operator()( int row, int col ) const { return data_[row * cols_ + col]; }
	const T& at( int row, int col ) const { return data_[row * cols_ + col]; }
	T& at( int row, int col ) { return data_[row * cols_ + col]; }

	const T* atp() const { return data_.data(); }
	T* atp() { return data_.data(); }

	int getRows() const { return rows_; }

	int getCols() const { return cols_; }
};

template<typename T>
void GPUMatrixMultiplication( const Matrix<T>& A, const Matrix<T>& B, Matrix<T>& C, oroFunction* function )
{
	oroError e;
	const int m = A.getRows();
	const int n = A.getCols();
	const int p = B.getRows();
	const int q = B.getCols();
	const size_t size = sizeof( T );
	//std::cout << size << std::endl;
	// const float* Ap = A.atp();
	// const float* Bp = B.atp();
	// const float* Cp = C.atp();
	const T* Ap = A.atp();
	const T* Bp = B.atp();
	const T* Cp = C.atp();

	//  デバイス用のメモリを確保
	T* d_A;
	T* d_B;
	T* d_C;
	// float* d_A;
	// float* d_B;
	// float* d_C;
	int* d_m;
	int* d_n;
	int* d_p;
	int* d_q;
	int* d_size;

    std::cout << " BLOCK_SIZEx,    VECTORNUMx,   GRID_SIZEx=     " << BLOCK_SIZEx << "    " << VECTORNUMx << "    " << GRID_SIZEx << std::endl;
	exit( 0 );

	oroMalloc( (oroDeviceptr*)&d_A, sizeof( T ) * m * n );
	oroMalloc( (oroDeviceptr*)&d_B, sizeof( T ) * p * q );
	oroMalloc( (oroDeviceptr*)&d_C, sizeof( T ) * m * q );
	// oroMalloc( (oroDeviceptr*)&d_A, sizeof( float ) * m * n );
	// oroMalloc( (oroDeviceptr*)&d_B, sizeof( float ) * p * q );
	// oroMalloc( (oroDeviceptr*)&d_C, sizeof( float ) * m * q );
	oroMalloc( (oroDeviceptr*)&d_m, sizeof( int ) );
	oroMalloc( (oroDeviceptr*)&d_n, sizeof( int ) );
	oroMalloc( (oroDeviceptr*)&d_p, sizeof( int ) );
	oroMalloc( (oroDeviceptr*)&d_q, sizeof( int ) );
	oroMalloc( (oroDeviceptr*)&d_size, sizeof( T ) );


	// データをデバイスにコピー
	oroMemcpyHtoD( (oroDeviceptr)d_A, (void*)Ap, sizeof( T ) * m * n );
	oroMemcpyHtoD( (oroDeviceptr)d_B, (void*)Bp, sizeof( T ) * p * q );
	// oroMemcpyHtoD( (oroDeviceptr)d_C, (void*)Cp, sizeof( T ) * m * p );
	// oroMemcpyHtoD( (oroDeviceptr)d_A, (void*)Ap, sizeof( float ) * m * n );
	// oroMemcpyHtoD( (oroDeviceptr)d_B, (void*)Bp, sizeof( float ) * p * q );
	// oroMemcpyHtoD( (oroDeviceptr)d_C, (void*)Cp, sizeof( float ) * m * q );
	oroMemcpyHtoD( (oroDeviceptr)d_m, (void*)&m, sizeof( int ) );
	oroMemcpyHtoD( (oroDeviceptr)d_n, (void*)&n, sizeof( int ) );
	oroMemcpyHtoD( (oroDeviceptr)d_p, (void*)&p, sizeof( int ) );
	oroMemcpyHtoD( (oroDeviceptr)d_q, (void*)&q, sizeof( int ) );
	oroMemcpyHtoD( (oroDeviceptr)d_size, (void*)&size, sizeof( T ) );
	// oroMemset( (oroDeviceptr)d_C, 0.0f, sizeof( float ) * m * q );
	oroMemset( (oroDeviceptr)d_C, 0.0f, sizeof( T ) * m * q );

	////std::cout << "行列A:" << std::endl;
	// for( int i = 0; i < m; ++i )
	//{
	//	for( int j = 0; j < n; ++j )
	//	{
	//		std::cout << A.at( i, j ) << " ";
	//	}
	//	std::cout << std::endl;
	// }

	// std::cout << "行列B:" << std::endl;
	// for( int i = 0; i < p; ++i )
	//{
	//	for( int j = 0; j < q; ++j )
	//	{
	//		std::cout << B.at( i, j ) << " ";
	//	}
	//	std::cout << std::endl;
	// }
	//  グリッドとブロックの設定
	// int threadsPerBlock = 256;
	// int blocksPerGrid = ( n + threadsPerBlock - 1 ) / threadsPerBlock;
	// int blocksPerGrid = 1;
	// const char *type= typeid( d_A ).name();
	// std::cout << type << std::endl;
	//  カーネルを呼び出し
	// exit( 1 );

	const void* args[] = { &d_A, &d_B, &d_C, &d_m, &d_n, &d_p, &d_q, &d_size };
	// void** args = {};
	// e = oroModuleLaunchKernel( *function, blocksPerGrid, 1, 1, threadsPerBlock, 1, 1, 0, 0, (void**)args, 0 );
	oroDeviceSynchronize();
	clock_t start = clock();
	srand( (unsigned int)time( NULL ) );
	size_t sharedata = GRID_SIZEx * BLOCK_SIZEx * sizeof( T );
	e = oroModuleLaunchKernel( *function, GRID_SIZEx, 1, 1, BLOCK_SIZEx, 1, 1, sharedata, 0, (void**)args, 0 ); //
	oroDeviceSynchronize();
	clock_t end = clock();
	std::cout << "GPU duration = " << (float)( end - start ) / CLOCKS_PER_SEC << "sec.\n";
	// clock stop
	// std::cout << d_C[0] << std::endl;
	//  結果をデバイスからホストにコピー
	oroMemcpyDtoH( (void*)Cp, (oroDeviceptr)d_C, sizeof( T ) * m * q );
	// oroMemcpyDtoH( (void*)k, (oroDeviceptr)d_k, sizeof( int )  );

	std::cout << "GPU 行列C (A * B):" << std::endl;
	for( int i = 0; i < m; ++i )
	{
		for( int j = 0; j < q; ++j )
		{
			std::cout << C.at( i, j ) << " ";
		}
		std::cout << std::endl;
	}

	// デバイスメモリの解放
	oroFree( (oroDeviceptr)d_A );
	oroFree( (oroDeviceptr)d_B );
	oroFree( (oroDeviceptr)d_C );
	oroFree( (oroDeviceptr)d_m );
	oroFree( (oroDeviceptr)d_n );
	oroFree( (oroDeviceptr)d_p );
	oroFree( (oroDeviceptr)d_q );
	oroFree( (oroDeviceptr)d_size );
}
template<typename T>
void CPUMatrixMultiplication( const Matrix<T>& A, const Matrix<T>& B, Matrix<T>& C )
{
	int m = A.getRows();
	int n = A.getCols();
	int p = B.getCols();
	clock_t start = clock();
	for( int i = 0; i < m; ++i )
	{
		for( int j = 0; j < p; ++j )
		{
			T sum = 0;
			for( int k = 0; k < n; ++k )
			{
				sum += A.at( i, k ) * B.at( k, j );
			}
			C.at( i, j ) = sum;
		}
	}
	clock_t end = clock();
	std::cout << "CPU duration = " << (float)( end - start ) / CLOCKS_PER_SEC << "sec.\n";
	std::cout << "CPU 行列C (A * B):" << std::endl;
	for( int i = 0; i < m; ++i )
	{
		for( int j = 0; j < p; ++j )
		{
			std::cout << C.at( i, j ) << " ";
			std::cout << std::endl;
		}
	}
}
// template<typename T>
// void MatrixMultiplication( const Matrix<T>& A, const Matrix<T>& B, Matrix<T>& C )
//{
//	int m = A.getRows();
//	int n = A.getCols();
//	int p = B.getCols();
//
//	for( int i = 0; i < m; ++i )
//	{
//		for( int j = 0; j < p; ++j )
//		{
//			T sum = 0;
//			for( int k = 0; k < n; ++k )
//			{
//				sum += A.at( i, k ) * B.at( k, j );
//			}
//			C.at( i, j ) = sum;
//		}
//	}
// }

// #include <iostream>
// #include <vector>
//
// template<typename T>
// class Matrix
//{
//   public:
//	Matrix( int rows, int cols ) : rows_( rows ), cols_( cols ), data_( rows * cols ) {}
//
//	T& at( int row, int col ) { return data_[row * cols_ + col]; }
//
//	const T& at( int row, int col ) const { return data_[row * cols_ + col]; }
//
//	int getRows() const { return rows_; }
//
//	int getCols() const { return cols_; }
//
//   private:
//	int rows_;
//	int cols_;
//	std::vector<T> data_;
// };
//
// template<typename T>
// void MatrixMultiplication( const Matrix<T>& A, const Matrix<T>& B, Matrix<T>& C )
//{
//	int m = A.getRows();
//	int n = A.getCols();
//	int p = B.getCols();
//
//	for( int i = 0; i < m; ++i )
//	{
//		for( int j = 0; j < p; ++j )
//		{
//			T sum = 0;
//			for( int k = 0; k < n; ++k )
//			{
//				sum += A.at( i, k ) * B.at( k, j );
//			}
//			C.at( i, j ) = sum;
//		}
//	}
// }
//