#include <includes.h>


__forceinline DWORD PID( LPCWSTR processName )
{
	HANDLE handle = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );
	DWORD procID = NULL;

	if ( handle == INVALID_HANDLE_VALUE )
		return procID;

	PROCESSENTRY32W entry = { 0 };
	entry.dwSize = sizeof( PROCESSENTRY32W );

	if ( Process32FirstW( handle, &entry ) ) {
		if ( !_wcsicmp( processName, entry.szExeFile ) ) {
			procID = entry.th32ProcessID;
		}
		else while ( Process32NextW( handle, &entry ) ) {
			if ( !_wcsicmp( processName, entry.szExeFile ) ) {
				procID = entry.th32ProcessID;
			}
		}
	}

	CloseHandle( handle );
	return procID;
}

DWORD pid;
void driver_init( )
{

	if ( !Driver::Init( ) ) { }

	pid = Driver::FindProcess( _( "FortniteClient-Win64-Shipping.exe" ) );

	std::cout << "process_id -> " << pid << std::endl;

	if (!Driver::CR3()) {}

	Base = Driver::GetBase();

	std::cout << "image_base -> " << Base << std::endl;
}

auto start_ue5_renderer() -> void {
	n_ue5::render ue5_renderer( 0 );
	ue5_renderer.initialize_mouse();
	driver_init( );
	thread( [ & ]( ) { ue5_renderer.run_actor_loop( ); } ).detach( );
	thread( [ & ]( ) { ue5_renderer.run_camera_loop( ); } ).detach( );
	thread( [ & ]( ) { ue5_renderer.run_aimbot_loop( ); } ).detach( );
	//thread( [ & ]( ) { ue5_renderer.run_level_loop( ); } ).detach( );

	
	//ue5_renderer.render_levels();
	

	return ue5_renderer.run_render_loop( );
}

auto main() -> int {

	
	//ntmouseinject::Init();
	inits::initialize( );
	start_ue5_renderer();

	return 0;
}