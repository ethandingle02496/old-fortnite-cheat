
namespace n_ue5
{
	
	class render
	{		
	public:
		struct TArray
		{
			std::uintptr_t Array;
			std::uint32_t Count;
			std::uint32_t MaxCount;

			std::uintptr_t Get(std::uint32_t Index)
			{
				return read<std::uintptr_t>(Array + (Index * 0x8));
			}
		};
		// Configs
		static void save_config( LPCSTR path );
		static void load_config( LPCSTR path );
		static void clear_config( LPCSTR path );
		bool open_file_dialog( );

		// Overlay
		void hide_overlay( );
		void show_overlay( );

		// Rendering
		render( HMODULE hModule );
		void run_render_loop( );
		void process_messages( );
		void handle_inputs( );
		void start_scene( );
		void end_scene( );
		void clean_up( );
		void render_levels( );
		void render_radar( );
		bool render_menu( );

		// Utility / SDK
		bool WorldToScreen( FVector Position, FVector2D* Output );

		float WorldGravityZ = -2800.0f;

		FVector predict_location(FVector target, FVector target_velocity, float distance, float speed)
		{
			speed *= 100;
			float gravity = 9.81f * 100.0f;
			float time = distance / speed;

			FVector target_positsion = target /* + (target_velocity * time)*/;

			return target_positsion;
		}

		bool is_shotgun( const std::string& weapon ) {
			return weapon.find( "otgun" ) != std::string::npos;
		}

		void perform_triggerbot( bool shotgun_only, bool sniper_only ) {
			if ( shotgun_only && is_shotgun( this->CurrentPlayerWeapon ) ) {
				mouse_click( );
				globals::has_clicked = true;
			}
			if ( sniper_only && strcmp( this->CurrentPlayerWeapon.c_str( ), "Reaper Sniper Rifle" ) == 0 ) {
				std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
				mouse_click( );
				globals::has_clicked = true;
			}
			
			if ( !shotgun_only && !sniper_only ) {
				mouse_click( );
				globals::has_clicked = true;
			}
		}


		//void chams(  )
		//{
		//	auto PawnMaterials = request.read<TArray<uintptr_t>>(Player + 0x5C40);
		//	for (int i = 0; i < PawnMaterials.Size(); i++)
		//	{
		//		auto MaterialInstance = PawnMaterials[i];

		//		auto Material = Memory::Read<uintptr_t>(MaterialInstance + 0x18);

		//		Memory::WriteBit(Material + 0x1b8, 6); //Wireframe is the 6th index
		//	}
		//}

		bool skeletons( USkeletalMeshComponent* Mesh, UObject* BoneArray, bool Outline, ImColor Color )
		{
			std::list<int> upper_part = { bones::HumanNeck,bones::HumanHead };
			std::list<int> right_arm = { bones::HumanNeck,bones::HumanChest,bones::HumanRUpperarm,bones::HumanRForearm1,bones::HumanRForearm23,bones::HumanRPalm };
			std::list<int> left_arm = { bones::HumanNeck,bones::HumanChest,bones::HumanLUpperarm,bones::HumanLForearm1,bones::HumanLForearm23,bones::HumanLPalm };
			std::list<int> spine = { bones::HumanChest,bones::HumanSpine1,bones::HumanSpine2,bones::HumanSpine3 };

			std::list<int> lower_right = { bones::HumanPelvis,bones::HumanRThigh1,bones::HumanRThigh2,bones::HumanRThigh3,bones::HumanRCalf,bones::HumanRFoot };
			std::list<int> lower_left = { bones::HumanPelvis,bones::HumanLThigh1,bones::HumanLThigh2,bones::HumanLThigh3,bones::HumanLCalf,bones::HumanLFoot };
			std::list<std::list<int>> skeleton = { /*upper_part,*/ right_arm, left_arm, spine, lower_right, lower_left };

			FVector previous( 0, 0, 0 );
			FVector current;
			auto p1 = FVector2D( 0, 0 );
			auto c1 = FVector2D( 0, 0 );
			auto neckpos = Mesh->GetBoneLocation( BoneArray, bones::HumanChest );
			auto pelvispos = Mesh->GetBoneLocation( BoneArray, bones::HumanPelvis );

			for ( auto a : skeleton ) {
				previous = FVector( 0, 0, 0 );
				for ( int bone : a ) {
					current = current = bone == bones::HumanChest ? neckpos : ( bone == bones::HumanPelvis ? pelvispos : Mesh->GetBoneLocation( BoneArray, bone ) );
					if ( previous.x == 0.f ) {
						previous = current;
						continue;
					}

					if ( this->WorldToScreen( current, &c1 ) && this->WorldToScreen( previous, &p1 ) ) {
						if ( Outline ) {
							this->line( ImVec2( p1.x, p1.y ), ImVec2( c1.x, c1.y ), ImColor( 0, 0, 0, 255 ), settings::visuals::skeleton_outline_thickness + settings::visuals::skeleton_thickness );
						}

						this->line( ImVec2( p1.x, p1.y ), ImVec2( c1.x, c1.y ), Color, settings::visuals::skeleton_thickness );

						previous = current;
					}
				}
			}
		}

		uintptr_t rootcomp( uintptr_t actor ) {

			return read<uintptr_t>( actor + offsets::RootComponent );
		}
		FVector get_location( uintptr_t actor ) {

			return read<FVector>( rootcomp( actor ) + offsets::RelativeLocation );
		}

		bool GetPlayerBoundings( USkeletalMeshComponent* Mesh, UObject* BoneArray, Bounds::PlayerBounds* Boundings ) {
			const auto get_bounds = [ & ]( Bounds::PlayerBounds& out, float expand = 0.f ) -> bool {
				Bounds::PlayerBounds Bounds = { FLT_MAX, FLT_MIN, FLT_MAX, FLT_MIN };

				for ( auto& bone : Bounds::BonesArray ) {
					FVector bone_pos = Mesh->GetBoneLocation( BoneArray, bone.index );

					bone.screen = FVector2D{  };

					if ( bone.index == bones::HumanHead )
						bone_pos.z += 15;

					if ( bone.index == bones::HumanLCalf ||
						bone.index == bones::HumanRCalf ) {
						bone_pos.z -= 8.5;
					}

					if ( bone.index == bones::HumanRPalm ||
						bone.index == bones::HumanRForearm23 ) {
						bone_pos.x -= 10.5;
					}

					if ( bone.index == bones::HumanLPalm ||
						bone.index == bones::HumanLForearm23 ) {
						bone_pos.x += 11;
					}

					if ( WorldToScreen( bone_pos, &bone.screen ) ) {
						if ( bone.screen.x < Bounds.left )
							Bounds.left = bone.screen.x;
						else if ( bone.screen.x > Bounds.right )
							Bounds.right = bone.screen.x;
						if ( bone.screen.y < Bounds.top )
							Bounds.top = bone.screen.y;
						else if ( bone.screen.y > Bounds.bottom )
							Bounds.bottom = bone.screen.y;

						bone.on_screen = true;
					}
				}

				if ( Bounds.left == FLT_MAX )
					return false;
				if ( Bounds.right == FLT_MIN )
					return false;
				if ( Bounds.top == FLT_MAX )
					return false;
				if ( Bounds.bottom == FLT_MIN )
					return false;

				Bounds.left -= expand;
				Bounds.right += expand;
				Bounds.top -= expand;
				Bounds.bottom += expand;

				out = Bounds;

				return true;
				};

			Bounds::PlayerBounds Bounds;

			if ( get_bounds( Bounds, 4.f ) ) {
				*Boundings = Bounds;
				return true;
			}

			return false;

		}

		std::string read_object_name_internal( int fname );
		std::string get_name_from_fname( int fname );

		// Drawing
		std::string global_tooltip = _("N/A");

		void add_to_radar( FVector word_location, float distance, bool visible );
		void calc_radar( FVector vOrigin, int& screenx, int& screeny );

		void add_to_minimap( FVector WorldLocation, FVector LocalLocation, ImColor Color, FVector2D RadarPos, FVector2D RadarSize, float Distance );
		FVector2D RotatePoint( FVector2D radar_pos, FVector2D radar_size, FVector LocalLocation, FVector TargetLocation );

		void text( const std::string& text, const ImVec2& pos, float size, uint32_t color, bool center );

		void filled_box( const ImVec2& position, const float width, const float height, const uint32_t color ) {
			ImGui::GetBackgroundDrawList( )->AddRectFilled( position, ImVec2( position.x + width, position.y + height ), color, settings::visuals::box_rounding, 0 );
		}

		void normal_box( const ImVec2& position, const float width, const float height, const uint32_t color, float thickness ) {
			ImGui::GetBackgroundDrawList( )->AddRect( position, ImVec2( position.x + width, position.y + height ), color, settings::visuals::box_rounding, 0, thickness );
		}

		void outlined_box( const ImVec2& position, const float width, const float height, const uint32_t color, float thickness, float outline_thickness ) {
			const ImVec2 top_left = position;
			const ImVec2 bottom_right = ImVec2( position.x + width, position.y + height );

			// Outline
			ImGui::GetBackgroundDrawList( )->AddRect( top_left, bottom_right, ImColor( 0, 0, 0 ), settings::visuals::box_rounding, 0, thickness + outline_thickness );

			// Inline
			ImGui::GetBackgroundDrawList( )->AddRect( top_left, bottom_right, color, settings::visuals::box_rounding, 0, thickness );
		}

		void draw_cornered_box( int x, int y, int w, int h, ImColor color, float thickness ) {
			ImGui::GetBackgroundDrawList( )->AddLine( ImVec2( x, y ), ImVec2( x, y + (h / 3) ), color, thickness );
			ImGui::GetBackgroundDrawList( )->AddLine( ImVec2( x, y ), ImVec2( x + (w / 3), y ), color, thickness );
			ImGui::GetBackgroundDrawList( )->AddLine( ImVec2( x + w - (w / 3), y ), ImVec2( x + w, y ), color, thickness );
			ImGui::GetBackgroundDrawList( )->AddLine( ImVec2( x + w, y ), ImVec2( x + w, y + (h / 3) ), color, thickness );
			ImGui::GetBackgroundDrawList( )->AddLine( ImVec2( x, y + h - (h / 3) ), ImVec2( x, y + h ), color, thickness );
			ImGui::GetBackgroundDrawList( )->AddLine( ImVec2( x, y + h ), ImVec2( x + (w / 3), y + h ), color, thickness );
			ImGui::GetBackgroundDrawList( )->AddLine( ImVec2( x + w - (w / 3), y + h ), ImVec2( x + w, y + h ), color, thickness );
			ImGui::GetBackgroundDrawList( )->AddLine( ImVec2( x + w, y + h - (h / 3) ), ImVec2( x + w, y + h ), color, thickness );
		}

		void circle( const FVector2D& pos, int radius, uint32_t color, int segments, float thickness ) {
			ImGui::GetBackgroundDrawList( )->AddCircle( ImVec2( pos.x, pos.y ), radius, color, segments, thickness );
		}

		ImU32 gradient_color( float hue ) {
			float saturation = 1.0f;
			float value = 1.0f;

			ImVec4 color;
			ImGui::ColorConvertHSVtoRGB( hue, saturation, value, color.x, color.y, color.z );
			color.w = 1.0f;

			// Convert ImVec4 color to ImU32
			return IM_COL32( int( color.x * 255 ), int( color.y * 255 ), int( color.z * 255 ), 255 );
		}

		void draw_fov_circle( float x, float y, float radius, ImColor color, int segments, float thickness ) {
			if ( settings::other::misc_outline ) {
				this->circle( FVector2D( x, y ), radius, ImColor( 0, 0, 0 ), 99, 3.0f );
			}

			this->circle( FVector2D( x, y ), radius, color, 99, thickness );
		}

		// Function to get CPU usage of a thread in percentage
		double GetThreadCpuUsage( HANDLE hThread ) {
			FILETIME creationTime, exitTime, kernelTime, userTime;

			if ( !GetThreadTimes( hThread, &creationTime, &exitTime, &kernelTime, &userTime ) ) {
				std::cerr << "Error getting thread times: " << GetLastError( ) << std::endl;
				return -1.0;
			}

			ULARGE_INTEGER kernelTimeValue;
			kernelTimeValue.LowPart = kernelTime.dwLowDateTime;
			kernelTimeValue.HighPart = kernelTime.dwHighDateTime;

			ULARGE_INTEGER userTimeValue;
			userTimeValue.LowPart = userTime.dwLowDateTime;
			userTimeValue.HighPart = userTime.dwHighDateTime;

			// Calculate total CPU time
			ULONGLONG totalTime = kernelTimeValue.QuadPart + userTimeValue.QuadPart;

			// Calculate CPU usage as a percentage
			double cpuUsage = static_cast< double >( totalTime ) / 10000000.0; // Convert to seconds

			// Calculate the percentage of CPU usage
			int numProcessors = std::thread::hardware_concurrency( ); // Get the number of processors
			double percentage = ( cpuUsage / ( numProcessors * 100.0 ) ) * 100.0;

			return percentage;
		}

		void draw_spinning_gradient_circle( float x, float y, float radius, int segments, float thickness, float rotation_speed ) {
			if ( settings::other::misc_outline ) {
				this->circle( FVector2D( x, y ), radius, ImColor( 0, 0, 0 ), 99, 3.0f );
			}

			ImDrawList* draw_list = ImGui::GetBackgroundDrawList( );

			// Calculate rotation angle based on time
			float rotation = ImGui::GetTime( ) * rotation_speed;

			ImVec2 wheel_center( x, y );

			// Render spinning gradient circle
			const float aeps = 0.5f / radius; // Half a pixel arc length in radians (2pi cancels out).
			const int segment_per_arc = ImMax( 4, static_cast< int >( radius ) / 12 );

			for ( int n = 0; n < 6; n++ ) {
				const float a0 = ( n ) / 6.0f * 2.0f * IM_PI - aeps + rotation;
				const float a1 = ( n + 1.0f ) / 6.0f * 2.0f * IM_PI + aeps + rotation;

				const int vert_start_idx = draw_list->VtxBuffer.Size;
				draw_list->PathArcTo( wheel_center, radius, a0, a1, segment_per_arc );
				draw_list->PathStroke( gradient_color( n / 6.0f ), false, thickness );
				const int vert_end_idx = draw_list->VtxBuffer.Size;

				// Paint colors over existing vertices
				ImVec2 gradient_p0( wheel_center.x + ImCos( a0 ) * radius, wheel_center.y + ImSin( a0 ) * radius );
				ImVec2 gradient_p1( wheel_center.x + ImCos( a1 ) * radius, wheel_center.y + ImSin( a1 ) * radius );
				ImGui::ShadeVertsLinearColorGradientKeepAlpha( draw_list, vert_start_idx, vert_end_idx, gradient_p0, gradient_p1, gradient_color( n / 6.0f ), gradient_color( ( n + 1.0f ) / 6.0f ) );
			}
		}

		void render_crosshair( int width, int height, ImColor misc_color ) {
			ImVec2 Center = ImVec2( width, height );
			ImVec2 offset1 = ImVec2( 5, 5 );  // Adjusted offset for the X shape
			ImVec2 offset2 = ImVec2( 6, -5 ); // Adjusted offset for the X shape

			float LineThickness = 1.0f;

			ImGui::GetBackgroundDrawList( )->AddLine( Center - offset1, Center + offset1, misc_color, LineThickness );
			ImGui::GetBackgroundDrawList( )->AddLine( Center - offset2, Center + offset2, misc_color, LineThickness );

		}

		void render_fps( float framerate ) {
			char fps[ 256 ];
			sprintf_s( fps, _("fps: %.0f"), framerate );
			this->text( fps, ImVec2( 15, 30), 15.0f, ImColor( 255, 255, 255, 245 ), false );
		}

		void circle_filled( const FVector2D& pos, int radius, uint32_t color, int segments ) {
			ImGui::GetBackgroundDrawList( )->AddCircleFilled( ImVec2( pos.x, pos.y ), radius, color, segments );
		}
		void rec_filled( float x0, float y0, float x1, float y1, ImColor color, float rounding, int rounding_corners_flags ) {
			ImGui::GetBackgroundDrawList( )->AddRectFilled( ImVec2( x0, y0 ), ImVec2( x1, y1 ), color, rounding, rounding_corners_flags );

		}
		void progress_bar( float x, float y, float w, float h, int value, int v_max ) {
			float progress_height = (h / static_cast<float>(v_max)) * static_cast<float>(value);
			float bar_y = y + h - progress_height;

			ImColor bar_color = ImColor {
				min( 510 * (v_max - value) / v_max, 255 ),
				min( 510 * value / v_max, 255 ),
				25,
				255
			};
			rec_filled( x, bar_y, x + w, y + h, bar_color, 0.0f, 0 );
		}

		void line( const ImVec2& from, const ImVec2& to, ImColor color, float thickness );
	
		// Helpers
		void render_elements( );
		bool filter_players( const PlayerInformation& Player );
		float calculate_text_size( float distance );
		void set_player_colors( const PlayerInformation& Player );
		PlayerInformation* find_target( );

		// Other funcs
		
		void run_mouse_loop();
		bool initialize_mouse();
		void move_mouse(int x, int y);
		void mouse_click();
		

		int calculate_hitbox( PlayerInformation* Player, UObject* BoneArray, bool RandomizedHitbox, int Hitbox );
		void set_colors( const float color1[ 4 ], const float color2[ 4 ], const float color3[ 4 ] )
		{
			globals::text_color = ImColor( { color1[ 0 ], color1[ 1 ], color1[ 2 ], color1[ 3 ] } );
			globals::visible_color = ImColor( { color2[ 0 ], color2[ 1 ], color2[ 2 ], color2[ 3 ] } );
			globals::visible_color_filled = ImColor( { color2[ 0 ], color2[ 1 ], color2[ 2 ], color3[ 3 ] } );
		}
		void set_color_from_array( ImColor target_color, const float color_array[ 4 ] ) {
			target_color = ImColor( color_array[ 0 ], color_array[ 1 ], color_array[ 2 ], color_array[ 3 ] );
		}

		// Quickbar
		bool options_tab = true;
		int selected_bool_index = 0;
		bool show_body = true;
		float opacity = 0.45f;
		float color_opacity = 255.0f;

		void draw_text_on_background( const ImVec2& pos, const char* text, ImColor color );
		void handle_input( );
		void render_quickbar( );

		// Inits and loops
		bool initialize_overlay( );
		bool hijack_overlay( );
		bool create_swapchain( );
		bool create_render_target( );
		bool init_imgui( );
		void run_actor_loop( );
		void render_actors( );
		void run_aimbot_loop( );
		void run_level_loop( );
		CameraInformation run_camera_loop( );

		// Data members for rendering
		MARGINS margin{ -1 };
		ID3D11Device* device;
		ID3D11DeviceContext* device_context;
		IDXGISwapChain* swapchain;
		ID3D11RenderTargetView* render_target;
		DWORD picker_flags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreview;
		RECT rect{ };
		HWND hwnd{ };
		MSG msg = { nullptr };
		ImFont* font{ };

		// Game Vars
		UWorld* GWorld;
		UFortLocalPlayer* LocalPlayer;
		AFortPlayerController* PlayerController;
		AFortPlayerPawnAthena* ClosestPawn;
		ACharacter* AcknowledgedPawn;
		AFortWeapon* CurrentWeapon;
		char TeamIndex;
		float ClosestDistance;
		FVector2D ClosestBone;
		bool InLobby;
		std::mutex DataMutex;

		// Player Information Vars
		std::string CurrentPlayerWeapon;


		float speed;
		float gravity;

	};
}
enum InjectedInputMouseOptions
{
	move = 1,
	left_up = 4,
	left_down = 2,
	right_up = 8,
	right_down = 16
};

struct InjectedInputMouseInfo
{
	int move_direction_x;
	int move_direction_y;
	unsigned int mouse_data;
	InjectedInputMouseOptions mouse_options;
	unsigned int time_offset_in_miliseconds;
	void* extra_info;
};

inline bool(*_NtUserInjectMouseInput)(InjectedInputMouseInfo*, int) = nullptr;

namespace ntmouseinject
{
	static BOOLEAN WINAPI Init()
	{

		auto win32uStringLibrary = _("win32u.dll");
		auto InjectMouseInputFunctionName = _("NtUserInjectMouseInput");

		auto win32u = LI_FN(LoadLibraryA).safe_cached()(win32uStringLibrary);
		void* NtUserInjectMouseInputAddress = (void*)LI_FN(GetProcAddress).safe_cached()(win32u, InjectMouseInputFunctionName);
		if (!NtUserInjectMouseInputAddress) return false;
		*(void**)&_NtUserInjectMouseInput = NtUserInjectMouseInputAddress;
		LI_FN(FreeLibrary).get()(win32u);


		return TRUE;
	}


	static BOOLEAN WINAPI move(int x, int y)
	{

		InjectedInputMouseInfo temp;
		temp.mouse_data = 0;
		temp.mouse_options = InjectedInputMouseOptions::move;
		temp.move_direction_x = x;
		temp.move_direction_y = y;
		temp.time_offset_in_miliseconds = 0;
		return _NtUserInjectMouseInput(&temp, 1);
	}
}
