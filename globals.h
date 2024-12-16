#pragma once
inline uintptr_t mesh;
inline __int64 va_text;
namespace globals {

    inline bool is_visible;
    static bool dpi_changed = false;
    inline bool debug = true;
    inline float n_width = 0;
    inline float n_height = 0;
    inline float n_half_width = 0;
    inline float n_half_height = 0;

    inline bool has_clicked;
    inline std::chrono::steady_clock::time_point tb_begin;
    inline std::chrono::steady_clock::time_point tb_end;
    inline int tb_time_since;

    inline ImColor menu_color_dark;
    inline ImColor menu_color_light;
    inline ImColor misc_color;
    inline ImColor text_color;
    inline ImColor visible_color;
    inline ImColor visible_color_filled;
    inline ImColor weapon_text_color;

    namespace memory {
        inline uintptr_t base_address;
        inline uint32_t process_id;

        
    }

    namespace threads {
        inline std::mutex mtx;
        inline std::condition_variable cv;
        inline bool thread_running = true;
        inline bool set_highest_priority( ) {
            BOOL result = SetThreadPriority( GetCurrentThread( ), THREAD_PRIORITY_HIGHEST );
            if ( result == 0 ) {
                return false;
            }

            return true;
        }
    }

}

namespace settings {
    inline bool show_menu = false;
    inline bool show_render = true;

    namespace combat {
        inline bool combat_visible_check = true;
        inline bool softaim = true;
        inline bool prediction = true;
        inline bool magic_bullet = true;
        inline bool pred = false;
        inline bool prediction_sniper_only = false;
        inline float bullet_velocity_offset = 6.0f;
        inline float bullet_drop_offset = 0.31f;
        inline bool ignore_knocked = true;
        inline float smoothness = 19.0f;
        inline float normal_smoothness = 19.0f;
        inline float fov_value = 10.0f;
        inline float normal_fov_value = 10.0f;
        inline int target_bone = 0;
        inline int normal_target_bone = 1;
        inline bool randomize_target_bone = false;
        inline const char* bone_names[ 5 ]{ ( "Head" ), ( "Neck" ), ( "Chest" ), ( "Pelvis" ), ( "Hitscan" ) };
        // inline bool show_target_line = true;
        inline bool triggerbot = true;
        inline bool triggerbot_always_on = false;
        inline bool triggerbot_shotgun_only = true;
        inline bool triggerbot_sniper_only = true;
        inline float triggerbot_delay = 0;
        inline float combat_render_distance = 200.0f;
        inline bool dead_zone = false;
        inline float custom_dead_zone = 0.1f;
        inline float multiplier = 2.990f;

        inline bool weapon_configs = true;

        inline float normal_bullet_velocity_offset = 6.0f;
        inline float rifle_bullet_velocity_offset = 6.0f;
        inline float smg_bullet_velocity_offset = 6.0f;
        inline float pistol_bullet_velocity_offset = 6.0f;
        inline float sniper_bullet_velocity_offset = 6.0f;

        inline float rifle_fov = 10.0f;
        inline float shotgun_fov = 7.0f;
        inline float smg_fov = 9.0f;
        inline float pistol_fov = 10.0f;
        inline float sniper_fov = 6.5f;

        inline float rifle_smoothness = 20.0f;
        inline float shotgun_smoothness = 2.0f;
        inline float smg_smoothness = 10.0f;
        inline float pistol_smoothness = 12.0f;
        inline float sniper_smoothness = 6.0f;

        inline int rifle_target_bone = 1;
        inline int shotgun_target_bone = 0;
        inline int smg_target_bone = 2;
        inline int pistol_target_bone = 1;
        inline int sniper_target_bone = 0;

        inline int current_weapon_config = 0;
        inline static const char* weapon_config_array[ ] = { "Rifle", "Shotgun", "SMG", "Pistol", "Sniper" };
    }

    namespace visuals {
        inline bool visuals_visible_check = true;
        inline bool perfect_visuals_visible_check = true;
        inline bool show_bounding_box = true;
        inline int box_type = 1;
        inline const char* box_names[ 6 ]{ ( "2D" ), ( "2D Outlined" ), ( "2D Cornered" ), ( "2D Cornered Outlined" ), ( "2D Filled" ), ( "2D Outlined Filled" ) };
        inline float box_thickness = 1.0f;
        inline float box_rounding = 0.0f;
        inline float box_outline_thickness = 2.0f;
        inline bool show_distance = true;
        inline bool show_weapon = false;
        inline bool show_ammo = false;
        inline bool show_name = false;
        inline bool show_kills = true;
        inline bool show_level = true;
        inline bool show_line = false;
        inline int line_type = 1;
        inline const char* line_names[ 2 ]{ ( "2D" ), ( "2D Outlined" ) };
        inline int line_start_type = 1;
        inline const char* line_start_names[ 3 ]{ ( "Top" ), ( "Center" ), ( "Bottom" ) };
        inline int line_end_type = 1;
        inline const char* line_end_names[ 3 ]{ ( "Top" ), ( "Center" ), ( "Bottom" ) };
        inline float view_angle_distance = 150.0f;
        inline bool show_skeleton = false;
        inline int skeleton_type = 0;
        inline const char* skeleton_names[ 2 ]{ ( "2D" ), ( "2D Outlined" ) };
        inline float skeleton_thickness = 0.5f;
        inline float skeleton_outline_thickness = 2.0f;
        inline float line_thickness = 0.5f;
        inline float line_outline_thickness = 2.0f;
        inline float visual_render_distance = 200.0f;
        inline bool text_outline = true;
        inline float text_font_size = 11.0f;
    }

    namespace radar {
        inline int radar_type = 2;
        inline bool draw_radar = true;
        inline const char* radar_names[ 3 ]{ ( "Circle" ), ( "Rectangle" ), ( "Minimap" ) };
        inline bool draw_radar_distance = true;
        inline bool draw_radar_to_minimap = true;
        inline float radar_font_size = 9.0f;
        inline float radar_circle_size = 3.0f;
        inline float radar_position_x = 15.f;
        inline float radar_position_y = 60.f;
        inline float radar_size = 270.f;
        inline float radar_distance = 60.f;
    }

    namespace world {
        inline bool enable = false;
        inline bool show_chests = false;
        inline bool show_ammo_box = false;
        inline bool show_traps = false;
        inline bool show_fishing_rod_barrels = false;
        inline bool show_campfires = false;
        inline bool show_llamas = false;
        inline bool show_pickups = false;
        inline bool show_common = false;
        inline bool show_uncommon = false;
        inline bool show_rare = false;
        inline bool show_epic = false;
        inline bool show_legendary = false;
        inline bool show_mythic = false;
        inline bool show_vehicles = false;
        inline float level_render_distance = 100.0f;
        inline float llama_render_distance = 300.0f;
        inline float pickup_render_distance = 70.0f;
        inline float chest_render_distance = 70.0f;
    }

    namespace other {
        inline int config_selection = 0;
        static const char* config_names[ ] =
        {
            "Config 1",
            "Config 2",
            "Config 3",
            "Config 4",
            "Config 5",
            "Config 6",
            "Config 7",
            "Config 8",
        };
        inline bool streamproof = false;
        inline bool streamproof_once = false;
        inline bool draw_quickbar = true;
        inline bool show_fov_circle = true;
        inline bool show_crosshair = false;
        inline bool show_fps = true;
        inline bool ignore_team = true;
        inline bool ignore_bots = true;
        inline bool ignore_dying = true;
        inline bool vsync = true;
        inline bool misc_rgb = false;
        inline bool misc_outline = true;
        inline const int actor_loop_time = 300;
        inline float menu_color_dark[ 4 ] = { 253 / 255.0f, 244 / 255.0f, 210 / 255.0f, 255 / 255.0f };
        inline float menu_color_light[ 4 ] = { 121 / 255.0f, 131 / 255.0f, 207 / 255.0f, 255 / 255.0f };
        inline float normal_color[ 4 ] = { 253 / 255.0f, 244 / 255.0f, 210 / 255.0f, 255 / 255.0f };
        inline float visible_color[ 4 ] = { 255 / 255.0f, 159 / 255.0f, 0 / 255.0f, 255 / 255.0f };
        inline float not_visible_color[ 4 ] = { 173 / 255.0f, 0 / 255.0f, 0 / 255.0f, 255 / 255.0f };
        inline float filled_color[ 4 ] = { 253 / 255.0f, 244 / 255.0f, 210 / 255.0f, 20 / 255.0f };

        inline float text_color[ 4 ] = { 253 / 255.0f, 244 / 255.0f, 210 / 255.0f, 255 / 255.0f };
        inline float text_visible_color[ 4 ] = { 255 / 255.0f, 159 / 255.0f, 0 / 255.0f, 255 / 255.0f };
        inline float text_not_visible_color[ 4 ] = { 173 / 255.0f, 0 / 255.0f, 0 / 255.0f, 255 / 255.0f };

        inline float misc_color[ 4 ] = { 253 / 255.0f, 244 / 255.0f, 210 / 255.0f, 255 / 255.0f };

        inline float knocked_color[ 4 ] = { 253 / 255.0f, 244 / 255.0f, 210 / 255.0f, 255 / 255.0f };
        inline float team_color[ 4 ] = { 0 / 255.0f, 255 / 255.0f, 0 / 255.0f, 255 / 255.0f };
        inline float bot_color[ 4 ] = { 0 / 255.0f, 0 / 255.0f, 255 / 255.0f, 255 / 255.0f };
    }
}

#define n_print( fmt ) do { \
    if ( globals::debug ) { \
        printf("[%s(Line: %d)] -> " fmt "\n", __FILE__, __LINE__); \
    } \
} while ( 0 )

#define n_log( fmt, ... ) do { \
    if ( globals::debug ) { \
        printf("[%s(Line: %d)] -> " fmt "\n", __FILE__, __LINE__); \
    } \
} while ( 0 )

#define PI 3.14159265358979323846f
#define M_PI 3.14159265358979323846264338327950288419716939937510
#define FortPTR reinterpret_cast< uint64_t >