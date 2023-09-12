#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include <Orochi/Orochi.h>
#include "test.h"

void loadFile( const char* path, std::vector<char>& dst )
{
	std::fstream f( path, std::ios::in );
	if( f.is_open() )
	{
		size_t sizeFile;
		f.seekg( 0, std::fstream::end );
		size_t size = sizeFile = (size_t)f.tellg();
		dst.resize( size );
		f.seekg( 0, std::fstream::beg );
		f.read( dst.data(), size );
		dst.push_back('\0' );
		f.close();
	}else{
		std ::cout << "no such file" << std:: endl;
		exit( 1 );
	}
}

int main(int argc, char **argv)
{

	/*clock_t start = clock();
	srand( (unsigned int)time( NULL ) );*/
	oroApi api = []( int& argc, char** argv ) {
		oroApi api = ORO_API_HIP;
		if( argc >= 2 )
		{
			if( strcmp( argv[1], "hip" ) == 0 ) api = ORO_API_HIP;
			if( strcmp( argv[1], "cuda" ) == 0 ) api = ORO_API_CUDA;
		}
		return api;
	}(argc, argv);



	int a = oroInitialize( api, 0 );
	if( a != 0 )
	{
		printf( "initialization failed\n" );
		return 0;
	}
	printf( ">> executing on %s\n", ( api == ORO_API_HIP ) ? "hip" : "cuda" );

	printf( ">> testing initialization\n" );
	oroError e;
	e = oroInit( 0 );
	oroDevice device;
	e = oroDeviceGet( &device, 0 );
	oroCtx ctx;
	e = oroCtxCreate( &ctx, 0, device );
	printf( ">> testing device props\n" );
	{
		oroDeviceProp props;
		oroGetDeviceProperties( &props, device );
		printf( "executing on %s (%s)\n", props.name, props.gcnArchName );
		int v;
		oroDriverGetVersion( &v );
		printf( "running on driver: %d\n", v );
	}
	oroFunction function;
	std::vector<char> code;
	const char* funcName = "MatrixMultiplicationKernel";
	loadFile( "../Test_SMJM/test_kernel.h", code );
	//const char* funcName = "Orotest";
	//loadFile( "../Test_SMJM/Orotest.h", code );
	//const char* code = "extern \"C\" __global__ "
	//				   "void MatrixMultiplicationKerne()"
	//			   "{ int a = threadIdx.x; printf(\"	thread %d running\\n\", a); }";
	//std::cout << code.data() << std::endl;
	//exit( 0 );
	orortcProgram prog;
	orortcResult rtc_e;
	rtc_e = orortcCreateProgram( &prog, code.data(), funcName, 0, 0, 0 );
	//rtc_e = orortcCreateProgram( &prog, code, funcName, 0, 0, 0 );
	std::vector<const char*> opts;

	opts.push_back( "-I ../" );

		// Compile the  kernel
	printf( "Compiling kernel...\n" );
	rtc_e = orortcCompileProgram( prog, opts.size(), opts.data() );
	if( rtc_e != ORORTC_SUCCESS )
	{
		size_t logSize;
		orortcGetProgramLogSize( prog, &logSize );
		if( logSize )
		{
			std::string log( logSize, '\0' );
			orortcGetProgramLog( prog, &log[0] );
			std::cout << log << '\n';
		};
		return -1;
	}
	size_t codeSize;
	rtc_e = orortcGetCodeSize( prog, &codeSize );

	std::vector<char> codec( codeSize );
	rtc_e = orortcGetCode( prog, codec.data() );
	rtc_e = orortcDestroyProgram( &prog );
	oroModule module;
	e = oroModuleLoadData( &module, codec.data() );
	e = oroModuleGetFunction( &function, module, funcName );

	/*void** args = {};
	e = oroModuleLaunchKernel( function, 1, 1, 1, 32, 1, 1, 0, 0, args, 0 );
	oroDeviceSynchronize();*/


	
	clock_t start = clock();
	srand( (unsigned int)time( NULL ) );

	// 行列のサイズを設定します
	int m = 1;
	int n = 10000;
	int p = n;
	int q = 1;
	 
	// 行列を作成します
	Matrix<T> A( m, n );
	Matrix<T> B( p, q );
	Matrix<T> C_GPU( m, q );
	Matrix<T> C_CPU( m, q );

	//// 行列AとBに値を設定します（例としてランダムな値を使用）
	for( int i = 0; i < m; ++i )
	{
		for( int j = 0; j < n; ++j )
		{
			A.at( i, j ) = static_cast<T>( rand() ) / RAND_MAX - (T)0.5;
		}
	}

	for( int i = 0; i < p; ++i )
	{
		for( int j = 0; j < q; ++j )
		{
			B.at( i, j ) = static_cast<T>( rand() ) / RAND_MAX - (T)0.5;
		}
	}

	//// 行列の積を計算します
	GPUMatrixMultiplication( A, B, C_GPU, &function );
	CPUMatrixMultiplication( A, B, C_CPU );

	//// 結果を出力します
	
	/*std::cout << "行列C (A * B):" << std::endl;
	for( int i = 0; i < m; ++i )
	{
		for( int j = 0; j < p; ++j )
		{
			std::cout << C.at( i, j ) << " ";
			std::cout << std::endl;
		}
	}*/
	/*clock_t end = clock();
	std::cout << "duration = " << (float)( end - start ) / CLOCKS_PER_SEC << "sec.\n";*/

	return 0;
}

//int main()
//{
//	// 行列のサイズを設定します
//	int m = 3;
//	int n = 4;
//	int p = 2;
//
//	// 行列を作成します
//	matrix<float> a( m, n );
//	matrix<float> b( n, p );
//	matrix<float> c( m, p );
//
//	// 行列aとbに値を設定します（例としてランダムな値を使用）
//	for( int i = 0; i < m; ++i )
//	{
//		for( int j = 0; j < n; ++j )
//		{
//			a.at( i, j ) = static_cast<float>( rand() ) / rand_max;
//		}
//	}
//
//	for( int i = 0; i < n; ++i )
//	{
//		for( int j = 0; j < p; ++j )
//		{
//			b.at( i, j ) = static_cast<float>( rand() ) / rand_max;
//		}
//	}
//
//	// 行列の積を計算します
//	matrixmultiplication( a, b, c );
//
//	// 結果を出力します
//	std::cout << "行列a:" << std::endl;
//	for( int i = 0; i < m; ++i )
//	{
//		for( int j = 0; j < n; ++j )
//		{
//			std::cout << a.at( i, j ) << " ";
//		}
//		std::cout << std::endl;
//	}
//
//	std::cout << "行列b:" << std::endl;
//	for( int i = 0; i < n; ++i )
//	{
//		for( int j = 0; j < p; ++j )
//		{
//			std::cout << b.at( i, j ) << " ";
//		}
//		std::cout << std::endl;
//	}
//
//	std::cout << "行列c (a * b):" << std::endl;
//	for( int i = 0; i < m; ++i )
//	{
//		for( int j = 0; j < p; ++j )
//		{
//			std::cout << c.at( i, j ) << " ";
//		}
//		std::cout << std::endl;
//	}
//
//	return 0;
//}
