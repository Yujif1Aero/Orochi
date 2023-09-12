#include <iostream>
#include <vector>
#include <time.h>

template<typename T>
class Matrix
{
  public:
	Matrix( int rows, int cols ) : rows_( rows ), cols_( cols ), data_( rows * cols ) {}

	T& at( int row, int col ) { return data_[row * cols_ + col]; }

	const T& at( int row, int col ) const { return data_[row * cols_ + col]; }

	int getRows() const { return rows_; }

	int getCols() const { return cols_; }

  private:
	int rows_;
	int cols_;
	std::vector<T> data_;
};

template<typename T>
void MatrixMultiplication( const Matrix<T>& A, const Matrix<T>& B, Matrix<T>& C )
{
	int m = A.getRows();
	int n = A.getCols();
	int p = B.getCols();

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
}

using T = float;
int main()
{

	clock_t start = clock();
	srand( (unsigned int)time( NULL ) );

	int m = 1;
	int n = 1e4;
	int p =1;


	Matrix<T> A( m, n );
	Matrix<T> B( n, p );
	Matrix<T> C( m, p );

	
	for( int i = 0; i < m; ++i )
	{
		for( int j = 0; j < n; ++j )
		{
			A.at( i, j ) = static_cast<T>( rand() ) / RAND_MAX -(T)0.5;
		}
	}

	for( int i = 0; i < n; ++i )
	{

		for( int j = 0; j < p; ++j )
		{
			B.at( i, j ) = static_cast<T>( rand() ) / RAND_MAX -(T)0.5 ;
		}
	}

	
	MatrixMultiplication( A, B, C );


	//std::cout << "s—ñA:" << std::endl;
	//for( int i = 0; i < m; ++i )
	//{
	//	for( int j = 0; j < n; ++j )
	//	{
	//		std::cout << A.at( i, j ) << " ";
	//	}
	//	std::cout << std::endl;
	//}

	//std::cout << "s—ñB:" << std::endl;
	//for( int i = 0; i < n; ++i )
	//{
	//	for( int j = 0; j < p; ++j )
	//	{
	//		std::cout << B.at( i, j ) << " ";
	//	}
	//	std::cout << std::endl;
	//}

	std::cout << "s—ñC (A * B):" << std::endl;
	for( int i = 0; i < m; ++i )
	{
		for( int j = 0; j < p; ++j )
		{
			std::cout << C.at( i, j ) << " ";
		}
		std::cout << std::endl;
	}

	clock_t end = clock();
	std::cout << "duration = " << (double)( end - start ) / CLOCKS_PER_SEC << "sec.\n";

	return 0;
}
