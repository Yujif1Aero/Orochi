#include <fstream>
#include <Orochi/Orochi.h>
#include <vector>
#include <iostream>

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
		f.close();
	}
	else
	{
		printf( "File not found\n" );
	}
}


int main( int argc, char** argv )
{
	oroApi api = []( int& argc, char** argv )
	{
		oroApi api = ORO_API_HIP;
		if( argc >= 2 )
		{
			if( strcmp( argv[1], "hip" ) == 0 ) api = ORO_API_HIP;
			if( strcmp( argv[1], "cuda" ) == 0 ) api = ORO_API_CUDA;
		}
		return api;
	}( argc, argv );
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
	// try kernel execution
	oroFunction function;

	std::vector<char> code;
	const char* funcName = "test_kernel2";
	loadFile( "../Test_SMJM/Test_SMJM2/test_kernel2.h", code );
	//const char* code = "extern \"C\" __global__ "
	//				   "void test_kernel2()"
	//				   "{ int a = threadIdx.x; printf(\"	thread %d running\\n\", a); }";
	code.push_back( '\0' ); 

	for( const char c : code )
	{
		std::cout << c;
	}

	std:: cout << std::endl;
	std::cout << code.data();
	

	std::cout << std::endl;
	//exit( 0 );

	orortcProgram prog;
	orortcResult rtc_e;
	rtc_e = orortcCreateProgram( &prog, code.data(), funcName, 0, 0, 0 );
	//rtc_e = orortcCreateProgram( &prog, code, funcName, 0, 0, 0 );
	std::vector<const char*> opts;
	opts.push_back( "-I ../" );




		// Compile the  kernel
	printf( "Compiling  kernel...\n" );
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

	void** args = {};
	e = oroModuleLaunchKernel( function, 1, 1, 1, 32, 1, 1, 0, 0, args, 0 );
	oroDeviceSynchronize();

}
