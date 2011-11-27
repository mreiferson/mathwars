/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//      FILE:       MATH.C
//      DATE:       November 26, 1995    (start date & time **NOT** finish date)
//      TIME:       02:34:19
//
//      AUTHOR:     Matthew Reiferson
//      PROJECT:    MathWars
//      VERSION:    1.0
//
//      PURPOSE:    This is the source code for Nicolas's Math Game.
//
//      HISTORY:    First Game I Ever Made, and This is the first version
//==============================================================================
/* Nicolas's Math Game... (c) Graphics/Text Version
By: Matt Reiferson
11/26/1995 */

// Includes For Project
#include <io.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <bios.h>
#include <fcntl.h>
#include <memory.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <graphic.h>
#include <graph.h>

// DEFINES ////////////////////
// for the game_state variable
#define GAME_MENU 0
#define GAME_PLAYING 1
#define GAME_OVER 2
#define GAME_SETUP_DIF 3
// FOR THE MAIN MENU
#define MENU_PLAY 0
#define MENU_DIFFICULTY 1
#define MENU_INSTRUCTIONS 2
#define MENU_EXIT 3
// For Difficulty Setting
#define DIF_EASY 5
#define DIF_NORMAL 15
#define DIF_HARD 25
#define MENU_EASY 0
#define MENU_NORMAL 1
#define MENU_HARD 2
// For The Level Setting
#define LEVEL1 1
#define LEVEL2 2
#define LEVEL3 3
#define LEVEL4 4
#define LEVEL5 5
#define LEVEL6 6
#define LEVEL7 7
#define LEVEL8 8
#define LEVEL9 9
#define LEVEL10 10
// for the starfield
#define PLANE_1 1
#define PLANE_2 2
#define PLANE_3 3
// for the screen savers
#define NO_SAVER  0
#define SAVER_FOLLOW 1
#define SAVER_LINES 2
#define SAVER_POLY 3
#define SAVER_DOTS 4
#define SAVER_WORM 5
// for the power up cells
#define POWERUP_SHIELD 0
#define POWERUP_POINTS 1
#define POWERUP_BOMB 2
#define POWERUP_WEAPONS 3
#define NUM_OF_POWER_UPS 4
// for the ships
#define MAX_NUMBER_OF_ALIEN_SHIPS 10
#define PLAYER_DEATH_TIME    120    // how long it takes player to die
#define NUM_DEATH_PARTICLES  30     // number of explosion particles in death
#define PLAYER_DEAD 0
#define PLAYER_ALIVE 1
#define PLAYER_DYING 2
// for the worm screen saver directions
#define DOWN_RIGHT 0
#define DOWN 1
#define DOWN_LEFT 2
#define LEFT 3
#define LEFT_UP 4
#define UP 5
#define UP_RIGHT 6
#define RIGHT 7
// size of the "tech" font used in intro
#define TECH_FONT_WIDTH         4   // width of high tech font
#define TECH_FONT_HEIGHT        7   // height of high tech font
#define NUM_TECH_FONT           64  // number of characters in tech font
// for the pattern setups in math portion
#define PATTERN_X_SIZE 5
#define PATTERN_Y_SIZE 3
#define NUM_OF_PATTERNS 10
#define PATTERN_XO        132         // origin of pattern formation
#define PATTERN_YO        20
// music and sound
#define MUSIC_OFF 0
#define MUSIC_ON 1
#define SOUND_OFF 0
#define SOUND_ON 1
#define KEY_SND 0
#define SELECT_SND 1
#define NUM_OF_CONTROL_SOUNDS 2
#define CORRECT_SND 0
#define INCORRECT_SND 1
#define CRASH1_SND 2
#define CRASH2_SND 3
#define LASER_SND 4
#define POWER_SND 5
#define ATTACK_SND 6
#define NUM_OF_GAME_SOUNDS 7
// special defs
#define PI (float)3.14159
#define ALIEN_1 1
#define ALIEN_2 2
#define ALIEN_FLY 1
#define ALIEN_ATTACK 2
// for attack pattern movements
#define NUM_ATTACK_PATTERNS 10
#define MAX_ATTACK_ELEMENTS 60
#define NUM_ATTACK_DIRECTIONS 5
#define R15 4
#define R30 3
#define L15 1
#define L30 2
#define D0 0
// for the point system
#define P_SHIP_KILLED 250
#define P_LEVEL_BEAT 1500
#define P_RIGHT_ANS 300
#define P_WRONG_ANS 150
#define P_LEVEL_LOSE 500
#define P_BONUS 750
#define P_POWER_UP 100
#define P_WIN_GAME 10000
#define NUM_SHIP_EXP_FRAMES 4
#define MAX_NUMBER_OF_EXP 10
#define EXPLOSION_INACTIVE    0  // state of an inactive explosion
#define EXPLOSION_ACTIVE      1  // state of a live explosion
#define ENEMY_MISSILE  0
#define PLAYER_MISSILE 1
#define MISS_ALIVE     1
#define MISS_DEAD      0
#define NUM_MISSILES  20
#define NUM_PHOTONS 20
// for world definitions
#define NUM_ROWS 200
#define NUM_TILES_ROW 20


// structures
// for the starfield
typedef struct star_typ {
    int x, y; // position of star
    int plane; // which plane the star is in
    int color; // which color of gray the star is
    
} star, *star_ptr;

// the structure which is used for the worm saver
typedef struct pixel_typ {
    int x, y;
    int color;
} pixel, *pixel_ptr;

typedef struct player_typ {
    int power;
    long points;
    int missile_type;
} player, *player_ptr;

typedef struct particle_typ {
    int x;                   // x position
    int y;                   // y position
    int xv;                  // x velocity
    int yv;                  // y velocity
    unsigned char color;     // the color of the particle
    unsigned char back;      // the color behind the particle
    int state;               // the state of the particle
    int counter;             // use for counting
    int threshold;           // the counters threshold
    int lifetime;
    int counter_2;
    int threshold_2;
} particle, *particle_ptr;

typedef struct powerup_typ {
    int x;
    int y;
    int lifetime;
    int type;
    int state;
    int curr_frame;
    int counter_1;
    int threshold_1;
} powerup, *powerup_ptr;

typedef struct missile_typ {
    int type;
    int color;
    int length;
    int x;
    int y;
    int yv;
    int state;
} missile, *missile_ptr;

typedef struct alien_typ {
    int type;                 // type of mech 1,2,3
    int x;                    // position of mech
    int y;
    int xv;                   // velocity of mech
    int yv;
    int carry_goodie;
    int type_of_goodie;
    int bitmap_x;
    int bitmap_y;
    int state_1;              // state variables
    int state_2;
    int counter_1;            // counters
    int counter_2;
    int threshold_1;          // thresholds for counters
    int threshold_2;
    int visible;
    int aux_1;                // aux variables
    int aux_2;
    int new_state;            // the next state
    int direction;            // direction of motion when flocking
    int curr_frame;           // current animation frame
    unsigned char far *background;     // background pointer
    int x_clip;
    int y_clip;
    int x_start;
    int y_start;
    int width_clip;
    int height_clip;
    int width;
    int height;
    
} alien, *alien_ptr;

typedef struct explosion_typ {
    int type;                 // type of mech 1,2,3
    int x;                    // position of mech
    int y;
    int state_1;              // state variables
    int state_2;
    int counter_1;            // counters
    int counter_2;
    int threshold_1;          // thresholds for counters
    int threshold_2;
    int aux_1;                // aux variables
    int aux_2;
    int new_state;            // the next state
    int direction;            // direction of motion when flocking
    int curr_frame;           // current animation frame
    unsigned char far *background;     // background pointer
    int x_start;
    int y_start;
    int x_clip;
    int y_clip;
    int width_clip;
    int height_clip;
    int visible;
    int bitmap_y;
    int width;
    int height;
    int bitmap_x;
    
} explosion, *explosion_ptr;

typedef struct tile_typ {
    int x;
    int there;
    int type;
} tile, *tile_ptr;

typedef struct row_typ {
    int y;
    tile tiles[NUM_TILES_ROW];
} row, *row_ptr;

typedef struct world_typ {
    row rows[NUM_ROWS];
} world, *world_ptr;

typedef struct photon_typ {
    int state;
    int x, y;
    int move_x;
    int move_y;
} photon, *photon_ptr;


// for the starfield
star stars[NUM_STARS];

world worlds;

particle pdeath[NUM_DEATH_PARTICLES]; // the particles used for player's death

alien alien_array[MAX_NUMBER_OF_ALIEN_SHIPS];

explosion explosions[MAX_NUMBER_OF_EXP];

powerup power_array[NUM_OF_POWER_UPS];

missile missiles[NUM_MISSILES + 1];
int num_missiles = 6;

photon photon_array[NUM_PHOTONS];

// for the rotating plane in the instructions screen
polygon plane;

// Variables

int addit,
    right,
    index_num,
    level_setting;

// for the starfield
int velocity_1 = 2;
int velocity_2 = 4;
int velocity_3 = 6;
// end of starfield variables

int FX_priority = 10; // the priority tracker of the current effect
int answer;
int game_state = GAME_MENU;
int game_action_state = 0;
int dif_setting = DIF_NORMAL;
int the_dif_setting = DIF_NORMAL;
int dif_cur_setting = DIF_NORMAL;

//for the tile drawer
int t_width = 16, t_bitmap_width = 16, t_bitmap_height = 16;
int players_x_speed, players_y_speed;
int t_array[10];
int cur_missile_length;

// to redraw the screen
int cur_problem;
int first1, second1;
int answer_flag;
int wait_state = 0;
// end to redraw the screen

char numbers[10],
     report[64];

pcx_picture image;

RGB_palette game_palette;

music game_song;

int cur_music_track, fast_machine = 0, in_windows = 0;
int control_music_playing = 0;
int bonus_x = 0, bonus_y = 0;

sound control_snd[NUM_OF_CONTROL_SOUNDS],
      game_snd[NUM_OF_GAME_SOUNDS];

// Sprite Structure To Hold The Various Images
sprite button,
       picright,
       starburst,
       player_ship,
       alien_missile,
       power_line,
       power_1,
       power_2,
       power_3,
       power_4,
       alien_1,
       alien_2,
       exp_1,
       tiles;

sprite_ptr tile_p = (sprite_ptr) &tiles;
int num_aliens;

// variables for the player
player cur_player;

// color rotation variables
RGB_color color_1, color_2;

bitmap tech_font[NUM_TECH_FONT];   // the tech font bitmaps

// to save and restore the game pallete
RGB_palette game_palette;

// to test if the user wants music and/or sound
int music_enabled = MUSIC_OFF,
    sound_enabled = SOUND_OFF;

// Char Array Holding The Ending Goodbye Sayings
char *bye[] = {
    "\nHasta La Vista, Baby...\n",
    "\nSee Ya' Later...\n",
    "\nSo Long...\n",
    "\nGoodBye...\n",
    "\nPeace...\n"
};

// next 10 sets are formation setups
int pattern_0[PATTERN_X_SIZE *PATTERN_Y_SIZE] = {0, 1, 0, 1, 0,
        0, 0, 2, 0, 0,
        0, 0, 0, 0, 0
                                                };

int pattern_1[PATTERN_X_SIZE *PATTERN_Y_SIZE] = {0, 0, 2, 0, 0,
        1, 0, 1, 0, 1,
        0, 0, 0, 0, 0
                                                };

int pattern_2[PATTERN_X_SIZE *PATTERN_Y_SIZE] = {1, 0, 2, 0, 1,
        0, 0, 0, 0, 0,
        0, 0, 2, 0, 0
                                                };

int pattern_3[PATTERN_X_SIZE *PATTERN_Y_SIZE] = {2, 0, 2, 0, 0,
        0, 1, 0, 1, 0,
        0, 0, 2, 0, 1
                                                };

int pattern_4[PATTERN_X_SIZE *PATTERN_Y_SIZE] = {0, 0, 1, 0, 0,
        0, 1, 2, 1, 0,
        0, 0, 1, 0, 0
                                                };

int pattern_5[PATTERN_X_SIZE *PATTERN_Y_SIZE] = {1, 0, 0, 0, 1,
        0, 0, 2, 0, 0,
        1, 0, 2, 0, 1
                                                };

int pattern_6[PATTERN_X_SIZE *PATTERN_Y_SIZE] = { 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0,
        2, 2, 2, 2, 2
                                                };

int pattern_7[PATTERN_X_SIZE *PATTERN_Y_SIZE] = {1, 2, 0, 2, 1,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0
                                                };

int pattern_8[PATTERN_X_SIZE *PATTERN_Y_SIZE] = {1, 0, 0, 0, 1,
        0, 1, 2, 1, 0,
        0, 0, 1, 0, 0
                                                };

int pattern_9[PATTERN_X_SIZE *PATTERN_Y_SIZE] = {1, 0, 2, 0, 1,
        0, 2, 0, 2, 0,
        2, 0, 1, 0, 2
                                                };

int *patterns[NUM_OF_PATTERNS],
    *current_pattern,
    pattern_number = 0;

// next are attack patterns for enemies to follow
int attack_patterns[NUM_ATTACK_PATTERNS][MAX_ATTACK_ELEMENTS]

    =  {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 0, 0, 4, 4, 4, 4, 0, 0, 1, 2, 1, 1, 0, 0, 0, -1,
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 4, 3, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 0, 1, 2, 2, 2, 2, 2, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, -1,
        1, 1, 2, 2, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 0, 0, 4, 4, 3, 3, 4, 0, 1, 1, 0, -1,
        1, 1, 2, 2, 1, 0, 4, 3, 3, 3, 3, 3, 4, 4, 0, 0, 0, 0, 0, 1, 2, 2, 1, 0, 4, 3, 3, 3, 3, 3, 3, 4, 0, 1, 1, 1, 1, 1, 1, 2, 2, 1, 0, 0, 0, 4, 4, 4, 4, 0, 1, 2, 2, 2, 2, 2, 2, 0, 0, -1,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 4, 3, 3, 3, 3, 3, 3, 3, 3, 4, 0, -1,
        4, 4, 3, 3, 4, 3, 4, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 4, 0, 1, 2, 2, 1, 0, -1,
        1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 4, 4, 0, -1,
        1, 2, 1, 0, 4, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 0, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 0, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, -1,
        1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, -1,
        4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 4, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 4, 0, 1, 2, 1, 0, -1
       };

int dirs_x[NUM_ATTACK_DIRECTIONS] = {0, -2, -4, 4, 2};

int worldn = 0;
world_ptr the_world;


unsigned long game_clock = 0;         // how many ticks has current level been running for

long attack_time = 150;       // threshold to start attack

int players_x = 320 - 140,
    players_y = 200 - 35,
    players_sx = 320 - 140,
    players_sy = 200 - 35;

// Functions Declarations
//  Initialization and Opening Functions
void play(void);

void reportit(void);

void menu(void);
// Functions for obtaining and displaying the problems
void printproblem(int number);

void clrproblem(void);

int add();

int sub();

int get_answer(int first, int second, int mode);
// For The Opening Movies and screen savers
void dsoft_screen(void);

void opening_screen(void);

void inst_screen(void);

extern Opening_Movie(void);

int screen_saver(void);
// Star Routines
void init_stars(void);

void draw_stars(void);
// Missile functions
void Init_Missiles(void);

void Start_Missile(int x,
                   int y,
                   int x1,
                   int type);

void Draw_Missiles(void);

void Move_Missiles(void);
// Functions for drawing and creating the aliens
void Clear_Aliens(void);

void Init_Aliens(void);

void Delete_Aliens(void);

void Erase_Aliens(void);

void Behind_Aliens(void);

void Draw_Aliens(void);

void Draw_Aliens2(void);

void Move_Aliens(void);

void Move_Aliens2(void);

void Start_Alien(void);
// Ship initializations and drawing routines
void Init_Ships(void);

void Delete_Ships(void);

void Draw_Player_Ship(void);

void Draw_Player_Ship1(void);

void Erase_Player_Ship(void);

void Sprite_Under_Clip1(sprite_ptr sprite, unsigned char far *buffer, int rx, int by, int lx, int ty);

void Sprite_Draw_Clip1(sprite_ptr sprite, unsigned char far *buffer);

void Sprite_Draw_Clip2(sprite_ptr sprite);

void Sprite_Draw_Clip3(sprite_ptr sprite);

void Sprite_Draw_Clip4(sprite_ptr sprite);
// Special Effects Functions
void blit_string3(int n1, int n2, int color1, int color2, int color3, char *string, int n3);

void Do_Starburst(void);

void Init_Starburst(void);

void Show_Points(void);

void Setup_Plane_Wire(void);

void Show_Plane_Wire(void);

void Moving_Text_Y(char *string, int x, int y);

void Fill_Screen(void);

void Screen_From_Black_PCX(void);

void Panel_FX(void);
// Misc. Functions
unsigned char Get_Pixel_DB(int x, int y);

void Test_For_Warrior(void);

void redraw_screen_db(void);

int Parse_Commands(int argc, char **argv);

void V_Line_DB(int x, int y1, int y2, unsigned int color);

int Sprites_Collide1(sprite_ptr sprite_1, sprite_ptr sprite_2);
// The Font Engine
void Font_Engine_1(int x, int y,
                   int font, int color,
                   char *string, unsigned char far *destination);

void Tech_Print(int x, int y, char *string, unsigned char far *destination);
// The Power Line Functions
void Init_Power_Line(void);

void Delete_Power_Line(void);

void Draw_Power_Line(void);

void Erase_Power_Line(void);
// The Particle Death Sequence Functions
void Start_PDeath(void);

void Erase_PDeath(void);

void Animate_PDeath(void);

void Behind_PDeath(void);

void Draw_PDeath(void);
// Sound Effects Functions
void Load_Game_Sounds(void);

void Delete_Game_Sounds(void);

int FX_Play(int the_effect, int priority);
// The Patterns Setup Functions
void Start_Pattern(void);

void Init_Patterns(void);
// Inintialization Of icons
void Load_Game_Icons(void);

void Delete_Game_Icons(void);
// The Explosion Functions
void Start_Explosion(int x, int y, int speed);

void Draw_Explosions(void);

void Animate_Explosions(void);

void Init_Explosions(void);

// The World Initialization and Drawing Functions
void Load_Tiles(void);

void Delete_Tiles(void);

int Draw_World(void);

void Draw_Row(int row_num);

void Draw_Tile(int row_num, int tile_num);

void Erase_World(void);

void Move_World(void);

void Init_World(void);

void Fill_Double_Buffer_I(void);

void Play_Action(void);

void Test_For_Collisions(void);

//for the powerups

void Animate_Powerups(void);

void Init_Powerups(void);

void Draw_Powerups(void);

void Start_Powerup(int x, int y, int type, int lifetime);

int Get_Goodie(void);

//for the alien photons

void Init_Photons(void);

void Move_Photons(void);

void Draw_Photons(void);

void Start_Photon(int x, int y, int move_x, int move_y);

// for the end of the game sequence

void End_Sequence(void);

// MAIN //////////////////////////////////
void main(int argc, char **argv[])
{
    int gbye, index;
    FILE *fp;
    char name[10][15];
    char score[10][15];
    
    // Set Video Mode to 13h
    Set_Video_Mode(VGA256);
    Keyboard_Install_Driver();
    Parse_Commands(argc, argv);
    if (music_enabled) {
        Music_Load("gamemus.xmi", (music_ptr)&game_song);
        Music_Play((music_ptr)&game_song, 11);
    }
    Tech_Print(2, 8, "  MATHWARS STARTING UP...", video_buffer);
    Delay(5);
    Tech_Print(2, 21, "COMMAND PARAMETERS SET", video_buffer);
    Delay(5);
    Tech_Print(2, 34, "VIDEO MODE SET TO 320X200", video_buffer);
    Delay(5);
    // Create The Double Buffer
    Create_Double_Buffer(200);
    // Clear Out The Double Buffer
    Fill_Double_Buffer(0);
    Tech_Print(2, 47, "ANIMATION SYSTEM ACTIVATED AND INITIALIZED", video_buffer);
    Delay(5);
    Create_Tables();
    Init_Pal_Lookup();
    Tech_Print(2, 60, "ROTATION TABLES INITIALIZED", video_buffer);
    Delay(5);
    Init_Ships();
    Tech_Print(2, 73, "PLANES LOADED AND INITIALIZED", video_buffer);
    Delay(5);
    Setup_Plane_Wire();
    Tech_Print(2, 86, "WIREFRAME PLANE DISPLAY ENABLED", video_buffer);
    Delay(5);
    Init_Starburst();
    Tech_Print(2, 99, "STARBURST GRAPHICAL INTERFACE ACTIVATED", video_buffer);
    Tech_Print(2, 112, "SYSTEM CHECKS OUT O.K.", video_buffer);
    for (index = 0; index < 3; index++) {
        // draw the message and the erase the message
        Font_Engine_1(2, 112, 0, 0, "SYSTEM CHECKS OUT O.K.", video_buffer);
        Delay(5);
        
        Font_Engine_1(2, 112, 0, 0, "                      ", video_buffer);
        Delay(5);
    } // end for
    Font_Engine_1(2, 112, 0, 0, "SYSTEM CHECKS OUT O.K.", video_buffer);
    Delay(15);
    // IntroDuction
    dsoft_screen();
    clrscr();
    Opening_Movie();
    clrscr();
    opening_screen();
    inst_screen();
    // Clear Screen
    clrscr();
    // End Of Introduction
    
    // Get A Menu Choice From The User
    menu();
    
    if (music_enabled == MUSIC_ON) {
        Music_Stop();
        Music_Play((music_ptr)&game_song, 9);
    }
    clrscr();
    fp = fopen("scorelog.dat", "r");
    fscanf(fp, "%s %s", &name[0], &score[0]);
    fscanf(fp, "%s %s", &name[1], &score[1]);
    fscanf(fp, "%s %s", &name[2], &score[2]);
    fscanf(fp, "%s %s", &name[3], &score[3]);
    fscanf(fp, "%s %s", &name[4], &score[4]);
    fscanf(fp, "%s %s", &name[5], &score[5]);
    fscanf(fp, "%s %s", &name[6], &score[6]);
    fscanf(fp, "%s %s", &name[7], &score[7]);
    fscanf(fp, "%s %s", &name[8], &score[8]);
    fscanf(fp, "%s %s", &name[9], &score[9]);
    PCX_Init((pcx_picture_ptr)&image);
    PCX_Load("scorelog.pcx", (pcx_picture_ptr)&image, 1);
    Screen_From_Black_PCX();
    PCX_Delete((pcx_picture_ptr)&image);
    Tech_Print(52, 52, name[0], video_buffer);
    Tech_Print(197, 52, score[0], video_buffer);
    Tech_Print(52, 67, name[1], video_buffer);
    Tech_Print(197, 67, score[1], video_buffer);
    Tech_Print(52, 82, name[2], video_buffer);
    Tech_Print(197, 82, score[2], video_buffer);
    Tech_Print(52, 97, name[3], video_buffer);
    Tech_Print(197, 97, score[3], video_buffer);
    Tech_Print(52, 112, name[4], video_buffer);
    Tech_Print(197, 112, score[4], video_buffer);
    Tech_Print(52, 127, name[5], video_buffer);
    Tech_Print(197, 127, score[5], video_buffer);
    Tech_Print(52, 142, name[6], video_buffer);
    Tech_Print(197, 142, score[6], video_buffer);
    Tech_Print(52, 157, name[7], video_buffer);
    Tech_Print(197, 157, score[7], video_buffer);
    Tech_Print(52, 172, name[8], video_buffer);
    Tech_Print(197, 172, score[8], video_buffer);
    Tech_Print(52, 187, name[9], video_buffer);
    Tech_Print(197, 187, score[9], video_buffer);
    Delay(75);
    Sweep_Rectangle();
    clrscr();
    // Exit The Game And Print Ending Message
    // Reset The Video Mode To Text
    Delete_Double_Buffer();
    Tech_Print(2, 8, "  MATHWARS SHUTTING DOWN...", video_buffer);
    Delay(5);
    Tech_Print(2, 21, "DOUBLE BUFFER DELETED", video_buffer);
    Delay(5);
    Delete_Ships();
    Delete_Game_Icons();
    Tech_Print(2, 32, "SHIPS DELETED AND UNINITIALIZED", video_buffer);
    Delay(5);
    Keyboard_Remove_Driver();
    Tech_Print(2, 43, "KEYBOARD ISR REMOVED", video_buffer);
    Delay(5);
    Tech_Print(2, 54, "SOUND SYSTEM SHUT DOWN", video_buffer);
    Delay(20);
    if (music_enabled == MUSIC_ON) {
        Music_Stop();
        Music_Unload((music_ptr)&game_song);
    }
    fclose(fp);
    Set_Video_Mode(TEXT_MODE);
    gbye = (rand() % 5);
    printf("%s", bye[gbye]);
}

// F U N C T I O N S /////////////////////////////////////////////////////////



// This Function Displays The Opening Math Wars Logo
void  opening_screen(void)
{
    if (music_enabled == MUSIC_ON) {
        Music_Stop();
        Music_Play((music_ptr)&game_song, 0);
    }
    Delay(3);
    // Load The Math Wars Screen
    PCX_Init((pcx_picture_ptr)&image);
    PCX_Load("open.pcx", (pcx_picture_ptr)&image, 1);
    PCX_Copy_To_Buffer((pcx_picture_ptr)&image, double_buffer);
    Blit_String_DB(60, 160, 38, "Hit Any Key To Start!!!", 1);
    Screen_From_Black();
    PCX_Delete((pcx_picture_ptr)&image);
    while (keys_active == 0) {
        if (music_enabled == MUSIC_ON) {
            if (Music_Status() != SEQUENCE_PLAYING) {
                Music_Play((music_ptr)&game_song, 0);
            }
        }
    }
    if (music_enabled) {
        Music_Stop();
    }
    // Wipe The Screen
    Squeeze_Screen();
}

// this is the function to display the instructions screen
void inst_screen(void)
{
    // declaration of backround_pcx picture type
    int index;
    
    if (music_enabled == MUSIC_ON) {
        Music_Stop();
        Music_Play((music_ptr)&game_song, 8);
    }
    
    // load and display the graphic
    PCX_Init((pcx_picture_ptr)&image);
    PCX_Load("inst.pcx", (pcx_picture_ptr)&image, 1);
    Screen_From_Black_PCX();
    PCX_Delete((pcx_picture_ptr)&image);
    Tech_Print(10, 10, "MISSION BRIEFING:", video_buffer);
    Tech_Print(20, 20, "YOUR MISSION IS SIMPLE, YOU MUST", video_buffer);
    Tech_Print(20, 28, "DESTROY THE ENEMY SHIPS WHILE", video_buffer);
    Tech_Print(20, 36, "ANSWERING MATHEMATIC PROBLEMS", video_buffer);
    Tech_Print(20, 44, "THAT ARE NECESSARY FOR THE", video_buffer);
    Tech_Print(20, 52, "SUCCESS OF THIS MISSION.  COMPLETION", video_buffer);
    Tech_Print(20, 60, "OF THIS MISSION WILL RESULT IN IMMEDIATE", video_buffer);
    Tech_Print(20, 68, "HONOR AND STARDOM, FOR YOU WILL BE", video_buffer);
    Tech_Print(20, 76, "RANKED AMONG THE GREATEST OF THE", video_buffer);
    Tech_Print(20, 84, "MATH WARRIORS.  THIS IS NOT AN EASY", video_buffer);
    Tech_Print(20, 92, "TASK AND REQUIRES STRENGTH", video_buffer);
    Tech_Print(20, 100, "COURAGE AND MOST OF ALL,", video_buffer);
    Tech_Print(20, 108, "KNOWLEDGE.  THE ALLIANCE IS", video_buffer);
    Tech_Print(20, 116, "IS COUNTING ON YOU.  GOOD LUCK!!!", video_buffer);
    // do red special effects while the user is not hitting a key
    while (keys_active == 0) {
        if (music_enabled == MUSIC_ON) {
            if (Music_Status() != SEQUENCE_PLAYING) {
                Music_Play((music_ptr)&game_song, 8);
            }
        }
    }
    // disolve the screen
    Sweep_Rectangle();
    if (music_enabled == MUSIC_ON) {
        Music_Stop();
    }
    // remove the keyboard driver
}

// function to initialize the ships sprites
void Init_Ships(void)
{
    // declare the PCX picture
    pcx_picture planes_pcx;
    int index, index_p;
    // initialize the picture structure
    PCX_Init((pcx_picture_ptr)&planes_pcx);
    // load the picture into the buffer
    PCX_Load("planes.pcx", (pcx_picture_ptr)&planes_pcx, 1);
    // initialize the sprite structure with the sprite info
    Sprite_Init((sprite_ptr)&player_ship, 190, 165, 31, 31, 0, 0, 0, 0, 0, 0);
    // get the six frames of animation in the picture
    for (index = 0; index < 6; index++) {
        PCX_Get_Sprite((pcx_picture_ptr)&planes_pcx, (sprite_ptr)&player_ship, index, index, 0);
    }
    player_ship.state = SPRITE_ALIVE;
    cur_player.points = 0;
    // initialize the sprite structure with the sprite info
    Sprite_Init((sprite_ptr)&alien_1, PATTERN_XO, PATTERN_YO, 31, 31, 0, 0, 0, 0, 0, 0);
    // get the siz frames of animation  in the picture
    for (index = 0; index < 10; index++) {
        PCX_Get_Sprite((pcx_picture_ptr)&planes_pcx, (sprite_ptr)&alien_1, index, index, 1);
    }
    Sprite_Init((sprite_ptr)&exp_1, 0, 0, 31, 31, 0, 0, 0, 0, 0, 0);
    for (index = 0; index < 4; index++) {
        PCX_Get_Sprite((pcx_picture_ptr)&planes_pcx, (sprite_ptr)&exp_1, index, index, 3);
    }
    // initialize the sprite structure with the sprite info
    Sprite_Init((sprite_ptr)&alien_2, PATTERN_XO, PATTERN_YO, 31, 31, 0, 0, 0, 0, 0, 0);
    // get the siz frames of animation  in the picture
    for (index = 0; index < 10; index++) {
        PCX_Get_Sprite((pcx_picture_ptr)&planes_pcx, (sprite_ptr)&alien_2, index, index, 2);
    }
    // delete the memory associated with the sprite
    PCX_Delete((pcx_picture_ptr)&planes_pcx);
    Load_Game_Icons();
}

void Load_Game_Icons(void)
{
    pcx_picture data_pcx;
    int index;
    
    PCX_Init((pcx_picture_ptr)&data_pcx);
    // load the picture into the buffer
    PCX_Load("icons.pcx", (pcx_picture_ptr)&data_pcx, 1);
    Sprite_Init((sprite_ptr)&alien_missile, 0, 0, 7, 7, 0, 0, 0, 0, 0, 0);
    PCX_Get_Sprite((pcx_picture_ptr)&data_pcx, (sprite_ptr)&alien_missile, 0, 0, 15);
    Sprite_Init((sprite_ptr)&power_1, 0, 0, 17, 17, 0, 0, 0, 0, 0, 0);
    for (index = 0; index < 4; index++) {
        PCX_Get_Sprite((pcx_picture_ptr)&data_pcx, (sprite_ptr)&power_1, index, index, 2);
    }
    Sprite_Init((sprite_ptr)&power_2, 0, 0, 17, 17, 0, 0, 0, 0, 0, 0);
    for (index = 0; index < 4; index++) {
        PCX_Get_Sprite((pcx_picture_ptr)&data_pcx, (sprite_ptr)&power_2, index, index, 3);
    }
    Sprite_Init((sprite_ptr)&power_3, 0, 0, 17, 17, 0, 0, 0, 0, 0, 0);
    for (index = 0; index < 4; index++) {
        PCX_Get_Sprite((pcx_picture_ptr)&data_pcx, (sprite_ptr)&power_3, index, index, 4);
    }
    Sprite_Init((sprite_ptr)&power_4, 0, 0, 17, 17, 0, 0, 0, 0, 0, 0);
    for (index = 0; index < 4; index++) {
        PCX_Get_Sprite((pcx_picture_ptr)&data_pcx, (sprite_ptr)&power_4, index, index, 5);
    }
    PCX_Delete((pcx_picture_ptr)&data_pcx);
}

void Delete_Game_Icons(void)
{
    int index;
    
    Sprite_Delete((sprite_ptr)&alien_missile);
    Sprite_Delete((sprite_ptr)&power_1);
    Sprite_Delete((sprite_ptr)&power_2);
    Sprite_Delete((sprite_ptr)&power_3);
    Sprite_Delete((sprite_ptr)&power_4);
}

// this function deletes the memory associated with the ships
void Delete_Ships(void)
{
    int index;
    
    Sprite_Delete((sprite_ptr)&player_ship);
    Sprite_Delete((sprite_ptr)&alien_1);
    Sprite_Delete((sprite_ptr)&alien_2);
}

// this function draws the players ship
void Draw_Player_Ship(void)
{
    // this variable decides which frame (jets or no jets) is used
    int the_frame;
    static int is_dead = 0;
    
    if (player_ship.state == PLAYER_DYING) {
        if (!is_dead) {
            player_ship.curr_frame = 2;
            FX_Play(((rand() % 2) + 2), 1);
            Start_PDeath();
            is_dead = 1;
        }
        Sprite_Under((sprite_ptr)&player_ship, double_buffer);
        Sprite_Draw((sprite_ptr)&player_ship, double_buffer, 1);
        if (++player_ship.curr_frame > 5) {
            player_ship.curr_frame = 2;
        }
    } else if (player_ship.state == PLAYER_ALIVE) {
        is_dead = 0;
        the_frame = rand() % 2;
        // set the current frame depending on what the random number was
        if (the_frame == 1) {
            player_ship.curr_frame = 1;
        } else {
            player_ship.curr_frame = 0;
        }
        // finally, scan under the sprite and draw it
        Sprite_Under((sprite_ptr)&player_ship, double_buffer);
        Sprite_Draw((sprite_ptr)&player_ship, double_buffer, 1);
    }
}

// the function to erase the players ship
void Erase_Player_Ship(void)
{
    Sprite_Erase((sprite_ptr)&player_ship, double_buffer);
}

// Function For Displaying The DeltaSoft Logo
void dsoft_screen(void)
{
    int index;
    
    // Load The DeltaSoft Screen
    PCX_Init((pcx_picture_ptr)&image);
    PCX_Load("dsoft.pcx", (pcx_picture_ptr)&image, 1);
    Screen_From_Black_PCX();
    PCX_Delete((pcx_picture_ptr)&image);
    Delay(50);
    Screen_Transition(SCREEN_WHITENESS);
    if (music_enabled) {
        Music_Stop();
    }
}

// this function rotates the wireframe of the plane in the instruction menu
void Setup_Plane_Wire(void)
{
    // setup the vertices for the plane
    plane.vertices[0].x = 1;
    plane.vertices[0].y = -17;
    plane.vertices[1].x = 10;
    plane.vertices[1].y = 1;
    plane.vertices[2].x = 15;
    plane.vertices[2].y = 0;
    plane.vertices[3].x = 15;
    plane.vertices[3].y = 7;
    plane.vertices[4].x = 6;
    plane.vertices[4].y = 12;
    plane.vertices[5].x = 3;
    plane.vertices[5].y = 6;
    plane.vertices[6].x = -3;
    plane.vertices[6].y = 6;
    plane.vertices[7].x = -6;
    plane.vertices[7].y = 12;
    plane.vertices[8].x = -15;
    plane.vertices[8].y = 5;
    plane.vertices[9].x = -15;
    plane.vertices[9].y = 0;
    plane.vertices[10].x = -10;
    plane.vertices[10].y = 1;
    plane.vertices[11].x = -1;
    plane.vertices[11].y = -17;
    plane.vertices[12].x = -1;
    plane.vertices[12].y = -6;
    plane.vertices[13].x = 1;
    plane.vertices[13].y = -6;
    // set position of shaceship
    plane.lxo = 278;
    plane.lyo = 43;
    // fill in important fields
    plane.num_vertices = 14;
    plane.b_color = 1;
    plane.closed = 1;
}

// this is the funciton that rotates and draws the plane for the instructions screen
void Show_Plane_Wire(void)
{
    // erase the plane
    plane.b_color = 0;
    Draw_Polygon((polygon_ptr)&plane);
    // rotate the plane 5 degrees
    Rotate_Polygon((polygon_ptr)&plane, 5);
    // draw the plane
    plane.b_color = 130;
    Draw_Polygon((polygon_ptr)&plane);
    // delay 1 ms
    Delay(1);
}

// This Function Loads All The Imagery For The Controls Screen
void init_control(void)
{
    // Initialize and Load the Control Panel Screen
    pcx_picture image_pcx;
    int index;
    RGB_color color;
    
    if ((!control_music_playing) && (music_enabled == MUSIC_ON)) {
        Music_Stop();
        Music_Play((music_ptr)&game_song, 12);
    }
    // Get the sprites for the buttons
    PCX_Init((pcx_picture_ptr)&image_pcx);
    PCX_Load("buttons.pcx", (pcx_picture_ptr)&image_pcx, 1);
    Sprite_Init((sprite_ptr)&button, 118 - 9, 62, 10, 9, 0, 0, 0, 0, 0, 0);
    button.counter_1 = 0;
    for (index = 0; index < 2; index++) {
        PCX_Get_Sprite((pcx_picture_ptr)&image_pcx, (sprite_ptr)&button, index, index, 0);
    }
    // get the sprites for the right/wrong answers
    PCX_Delete((pcx_picture_ptr)&image_pcx);
    PCX_Init((pcx_picture_ptr)&image_pcx);
    PCX_Load("right.pcx", (pcx_picture_ptr)&image_pcx, 0);
    Sprite_Init((sprite_ptr)&picright, 15, 100, 59, 29, 0, 0, 0, 0, 0, 0);
    for (index = 0; index < 3; index++) {
        PCX_Get_Sprite((pcx_picture_ptr)&image_pcx, (sprite_ptr)&picright, index, index, 0);
    }
    // Finish up and display certain graphics
    PCX_Delete((pcx_picture_ptr)&image_pcx);
    PCX_Init((pcx_picture_ptr)&image);
    PCX_Load("menu.pcx", (pcx_picture_ptr)&image, 0);
    color.red = 100;
    color.green = 100;
    color.blue = 100;
    Set_Palette_Register(17, (RGB_color_ptr)&color);
    color.red = 100;
    color.green = 100;
    color.blue = 100;
    Set_Palette_Register(64, (RGB_color_ptr)&color);
    color.red = 100;
    color.green = 100;
    color.blue = 100;
    Set_Palette_Register(80, (RGB_color_ptr)&color);
    color.red = 100;
    color.green = 100;
    color.blue = 100;
    Set_Palette_Register(96, (RGB_color_ptr)&color);
    Screen_From_Black_PCX();
    PCX_Delete((pcx_picture_ptr)&image);
    Sprite_Under((sprite_ptr)&button, video_buffer);
    Sprite_Draw((sprite_ptr)&button, video_buffer, 1);
    if (sound_enabled == SOUND_ON) {
        Sound_Load("keysnd.voc", (sound_ptr)&control_snd[KEY_SND], 1);
        if (rand() % 2) {
            Sound_Load("select.voc", (sound_ptr)&control_snd[SELECT_SND], 1);
        } else {
            Sound_Load("select1.voc", (sound_ptr)&control_snd[SELECT_SND], 1);
        }
    }
    control_music_playing = 1;
}

// This Is the Function For The Main Controls Menu
void menu(void)
{
    // Declarations
    int pix, fx, index, ok_to_play = 1;
    pcx_picture level_pcx;
    RGB_color color;
    // Load The Graphics For Main Menu
    init_control();
    
    // This Is The Control Panel Event Loop
    while (!(game_state == GAME_OVER)) {
        while (game_state == GAME_MENU) {
            if (keys_active > 0) {
                if (keyboard_state[MAKE_UP]) {
                    // erase the button and move it up
                    Sprite_Erase((sprite_ptr)&button, video_buffer);
                    if (sound_enabled == SOUND_ON) {
                        Sound_Play((sound_ptr)&control_snd[KEY_SND]);
                    }
                    
                    button.y -= 12;
                    
                    // test if we need to wrap around bottom
                    
                    if (--button.counter_1 < 0) {
                        button.counter_1 = 3;
                        button.y         = 62 + 3 * 12;
                    } // end if wrap around
                    
                    // scan and draw button
                    Sprite_Under((sprite_ptr)&button, video_buffer);
                    Sprite_Draw((sprite_ptr)&button, video_buffer, 1);
                    Delay(1);
                } // end if up
                else if (keyboard_state[MAKE_DOWN]) {
                    // erase the button and move it down
                    Sprite_Erase((sprite_ptr)&button, video_buffer);
                    if (sound_enabled == SOUND_ON) {
                        Sound_Play((sound_ptr)&control_snd[KEY_SND]);
                    }
                    
                    button.y += 12;
                    
                    // test if we need to wrap around top
                    
                    if (++button.counter_1 > 3) {
                        button.counter_1 = 0;
                        button.y         = 62;
                    } // end if wrap around
                    
                    // scan and draw button
                    Sprite_Under((sprite_ptr)&button, video_buffer);
                    Sprite_Draw((sprite_ptr)&button, video_buffer, 1);
                    
                    Delay(1);
                } // end if down
                else if (keyboard_state[MAKE_ENTER] ) {
                
                    // illuminate button for a second
                    
                    // draw button down
                    
                    button.curr_frame = 1;
                    Sprite_Draw((sprite_ptr)&button, video_buffer, 1);
                    if (sound_enabled == SOUND_ON) {
                        Sound_Play((sound_ptr)&control_snd[KEY_SND]);
                    }
                    switch (button.counter_1) {
                        case 0: {
                            color.red = 255;
                            color.green = 247;
                            color.blue = 0;
                            Set_Palette_Register(17, (RGB_color_ptr)&color);
                            break;
                        }
                        case 1: {
                            color.red = 255;
                            color.green = 247;
                            color.blue = 0;
                            Set_Palette_Register(64, (RGB_color_ptr)&color);
                            break;
                        }
                        case 2: {
                            color.red = 255;
                            color.green = 247;
                            color.blue = 0;
                            Set_Palette_Register(80, (RGB_color_ptr)&color);
                            break;
                        }
                        case 3: {
                            color.red = 255;
                            color.green = 247;
                            color.blue = 0;
                            Set_Palette_Register(96, (RGB_color_ptr)&color);
                            break;
                        }
                    }
                    
                    Delay(5);
                    
                    // now draw button up
                    
                    button.curr_frame = 0;
                    Sprite_Draw((sprite_ptr)&button, video_buffer, 1);
                    switch (button.counter_1) {
                        case 0: {
                            color.red = 100;
                            color.green = 100;
                            color.blue = 100;
                            Set_Palette_Register(17, (RGB_color_ptr)&color);
                            break;
                        }
                        case 1: {
                            color.red = 100;
                            color.green = 100;
                            color.blue = 100;
                            Set_Palette_Register(64, (RGB_color_ptr)&color);
                            break;
                        }
                        case 2: {
                            color.red = 100;
                            color.green = 100;
                            color.blue = 100;
                            Set_Palette_Register(80, (RGB_color_ptr)&color);
                            break;
                        }
                        case 3: {
                            color.red = 100;
                            color.green = 100;
                            color.blue = 100;
                            Set_Palette_Register(96, (RGB_color_ptr)&color);
                            break;
                        }
                    }
                    
                    // test which item is being selected
                    
                    switch (button.counter_1) {
                            // The User Is Playing The Game
                        case MENU_PLAY: {
                            // Set the Variables For Playing the Game
                            Screen_Transition(SCREEN_DARKNESS);
                            clrscr();
                            right = 0;
                            dif_setting = dif_cur_setting;
                            level_setting = LEVEL1;
                            game_state = GAME_PLAYING;
                            control_music_playing = 0;
                            cur_player.power = 22000;
                            cur_player.points = 0;
                            worldn = 0;
                            player_ship.x = players_sx;
                            player_ship.y = players_sy;
                            players_x_speed = 4;
                            players_y_speed = 4;
                            bonus_x = 0;
                            bonus_y = 0;
                            num_missiles = 6;
                            Init_Power_Line();
                            Init_Patterns();
                            Init_Aliens();
                            Init_Explosions();
                            Init_Missiles();
                            if (sound_enabled == SOUND_ON) {
                                Sound_Unload((sound_ptr)&control_snd[KEY_SND]);
                                Sound_Unload((sound_ptr)&control_snd[SELECT_SND]);
                            }
                            Load_Game_Sounds();
                            if (music_enabled) {
                                Music_Stop();
                            }
                            // Delay For A Second
                            Delay(5);
                            break;
                        }
                        // the User Wants To Change The Difficulty Setting
                        case MENU_DIFFICULTY: {
                            if (music_enabled == MUSIC_ON) {
                                if ((Music_Status() != SEQUENCE_PLAYING) && (music_enabled == MUSIC_ON)) {
                                    Music_Play((music_ptr)&game_song, 12);
                                }
                            }
                            // Set the variables for selecting difficulty
                            game_state = GAME_SETUP_DIF;
                            Screen_Transition(SCREEN_DARKNESS);
                            clrscr();
                            PCX_Init((pcx_picture_ptr)&image);
                            PCX_Load("dific.pcx", (pcx_picture_ptr)&image, 1);
                            Screen_From_Black_PCX();
                            PCX_Delete((pcx_picture_ptr)&image);
                            // set the postition for the button sprites
                            button.x = 70;
                            button.y = 74;
                            button.counter_1 = 0;
                            // scan and draw the button sprites
                            Sprite_Under((sprite_ptr)&button, video_buffer);
                            Sprite_Draw((sprite_ptr)&button, video_buffer, 1);
                            // While the Game Is in Difficulty Setting Mode
                            while (game_state == GAME_SETUP_DIF) {
                                if (keys_active > 0) {
                                    if (keyboard_state[MAKE_UP]) {
                                        // erase the button and move it up
                                        Sprite_Erase((sprite_ptr)&button, video_buffer);
                                        if (sound_enabled == SOUND_ON) {
                                            Sound_Play((sound_ptr)&control_snd[KEY_SND]);
                                        }
                                        
                                        button.y -= 36;
                                        
                                        // test if we need to wrap around bottom
                                        
                                        if (--button.counter_1 < 0) {
                                            button.counter_1 = 2;
                                            button.y         = 74 + 2 * 36;
                                        } // end if wrap around
                                        
                                        // scan and draw button
                                        Sprite_Under((sprite_ptr)&button, video_buffer);
                                        Sprite_Draw((sprite_ptr)&button, video_buffer, 1);
                                        Delay(3);
                                    }   // end if up
                                    else if (keyboard_state[MAKE_DOWN]) {
                                        // erase the button and move it down
                                        Sprite_Erase((sprite_ptr)&button, video_buffer);
                                        if (sound_enabled == SOUND_ON) {
                                            Sound_Play((sound_ptr)&control_snd[KEY_SND]);
                                        }
                                        
                                        button.y += 36;
                                        
                                        // test if we need to wrap around top
                                        
                                        if (++button.counter_1 > 2) {
                                            button.counter_1 = 0;
                                            button.y         = 74;
                                        } // end if wrap around
                                        
                                        // scan and draw button
                                        Sprite_Under((sprite_ptr)&button, video_buffer);
                                        Sprite_Draw((sprite_ptr)&button, video_buffer, 1);
                                        
                                        Delay(3);
                                    } // end if down
                                    else if (keyboard_state[MAKE_ENTER] ) {
                                    
                                        // illuminate button for a second
                                        
                                        // draw button down
                                        
                                        button.curr_frame = 1;
                                        Sprite_Draw((sprite_ptr)&button, video_buffer, 1);
                                        if (sound_enabled == SOUND_ON) {
                                            Sound_Play((sound_ptr)&control_snd[SELECT_SND]);
                                        }
                                        Delay(5);
                                        
                                        // now draw button up
                                        
                                        button.curr_frame = 0;
                                        Sprite_Draw((sprite_ptr)&button, video_buffer, 1);
                                        
                                        // test which item is being selected
                                        
                                        switch (button.counter_1) {
                                                // Set the Difficulty To easy (1-5)
                                            case MENU_EASY: {
                                                dif_setting = DIF_EASY;
                                                dif_cur_setting = DIF_EASY;
                                                the_dif_setting = DIF_EASY;
                                                game_state = GAME_MENU;
                                                break;
                                            }
                                            // Set the difficulty to normal (medium) (1-15)
                                            case MENU_NORMAL: {
                                                dif_setting = DIF_NORMAL;
                                                dif_cur_setting = DIF_NORMAL;
                                                the_dif_setting = DIF_NORMAL;
                                                game_state = GAME_MENU;
                                                break;
                                            }
                                            // Set the difficulty to hard (1-25)
                                            case MENU_HARD: {
                                                dif_setting = DIF_HARD;
                                                dif_cur_setting = DIF_HARD;
                                                the_dif_setting = DIF_HARD;
                                                game_state = GAME_MENU;
                                                break;
                                            }
                                        }
                                    }
                                }
                                if (music_enabled == MUSIC_ON) {
                                    if ((Music_Status() != SEQUENCE_PLAYING) && (music_enabled == MUSIC_ON)) {
                                        Music_Play((music_ptr)&game_song, 12);
                                    }
                                }
                            }
                            // Delete the Screen And Load The Controls Screen
                            Screen_Transition(SCREEN_DARKNESS);
                            clrscr();
                            Sprite_Delete((sprite_ptr)&button);
                            init_control();
                            Delay(5);
                            break;
                        }
                        // for the user to see instructions on playing the game, and a cool display of
                        // the ship they will be flying
                        case MENU_INSTRUCTIONS: {
                            Screen_Transition(SCREEN_DARKNESS);
                            clrscr();
                            PCX_Init((pcx_picture_ptr)&image);
                            PCX_Load("instruct.pcx", (pcx_picture_ptr)&image, 1);
                            Screen_From_Black_PCX();
                            PCX_Delete((pcx_picture_ptr)&image);
                            while (keys_active == 0) {
                                Show_Plane_Wire();
                                if (music_enabled == MUSIC_ON) {
                                    if ((Music_Status() != SEQUENCE_PLAYING) && (music_enabled == MUSIC_ON)) {
                                        Music_Play((music_ptr)&game_song, 12);
                                    }
                                }
                            }
                            Fade_Lights();
                            clrscr();
                            Sprite_Delete((sprite_ptr)&button);
                            init_control();
                            break;
                        }
                        // exit the game and free memory
                        case MENU_EXIT: {
                            game_state = GAME_OVER;
                            ok_to_play = 0;
                            if (music_enabled == MUSIC_ON) {
                                Music_Stop();
                            }
                            Delay(5);
                            break;
                        }
                    }   // End Switch
                } // End If Enter Hit
            } // End if Keys Active
            Panel_FX();
            Do_Starburst();
            Delay(1);
            Sprite_Delete((sprite_ptr)&button);
            if (music_enabled == MUSIC_ON) {
                if ((Music_Status() != SEQUENCE_PLAYING) && (ok_to_play == 1) && (music_enabled == MUSIC_ON)) {
                    Music_Play((music_ptr)&game_song, 12);
                }
            }
        } // end While Game State = Setup
        // Remove keyboard driver
        if (sound_enabled == SOUND_ON) {
            Sound_Unload((sound_ptr)&control_snd[KEY_SND]);
            Sound_Unload((sound_ptr)&control_snd[SELECT_SND]);
        }
        
        
        // The looping while to play the game
        while (game_state == GAME_PLAYING) {
            for (index = 1; index <= 10; index++, level_setting++) {
                if (game_state == GAME_PLAYING) {
                    // load the level graphics
                    PCX_Init((pcx_picture_ptr)&level_pcx);
                    // display level 1 graphics
                    if (level_setting == LEVEL1) {
                        if (music_enabled == MUSIC_ON) {
                            Music_Play((music_ptr)&game_song, 2);
                        }
                        cur_music_track = 2;
                        PCX_Load("level1.pcx", (pcx_picture_ptr)&level_pcx, 1);
                        PCX_Show_Buffer((pcx_picture_ptr)&level_pcx);
                        PCX_Delete((pcx_picture_ptr)&level_pcx);
                        pattern_number = rand() % 10;
                        Start_Pattern();
                    } else
                        // display level 2 graphics
                        if (level_setting == LEVEL2) {
                            if (music_enabled == MUSIC_ON) {
                                Music_Play((music_ptr)&game_song, 3);
                            }
                            cur_music_track = 3;
                            PCX_Load("level2.pcx", (pcx_picture_ptr)&level_pcx, 1);
                            PCX_Show_Buffer((pcx_picture_ptr)&level_pcx);
                            PCX_Delete((pcx_picture_ptr)&level_pcx);
                            pattern_number = rand() % 10;
                            Start_Pattern();
                        } else
                            // display level 3 graphics
                            if (level_setting == LEVEL3) {
                                if (music_enabled == MUSIC_ON) {
                                    Music_Play((music_ptr)&game_song, 4);
                                }
                                cur_music_track = 4;
                                PCX_Load("level3.pcx", (pcx_picture_ptr)&level_pcx, 1);
                                PCX_Show_Buffer((pcx_picture_ptr)&level_pcx);
                                PCX_Delete((pcx_picture_ptr)&level_pcx);
                                pattern_number = rand() % 10;
                                Start_Pattern();
                            } else
                                // display level 4 graphics
                                if (level_setting == LEVEL4) {
                                    if (music_enabled == MUSIC_ON) {
                                        Music_Play((music_ptr)&game_song, 5);
                                    }
                                    cur_music_track = 5;
                                    PCX_Load("level4.pcx", (pcx_picture_ptr)&level_pcx, 1);
                                    PCX_Show_Buffer((pcx_picture_ptr)&level_pcx);
                                    PCX_Delete((pcx_picture_ptr)&level_pcx);
                                    pattern_number = rand() % 10;
                                    Start_Pattern();
                                } else
                                    // display level 5 graphics
                                    if (level_setting == LEVEL5) {
                                        if (music_enabled == MUSIC_ON) {
                                            Music_Play((music_ptr)&game_song, 6);
                                        }
                                        cur_music_track = 6;
                                        PCX_Load("level5.pcx", (pcx_picture_ptr)&level_pcx, 1);
                                        PCX_Show_Buffer((pcx_picture_ptr)&level_pcx);
                                        PCX_Delete((pcx_picture_ptr)&level_pcx);
                                        pattern_number = rand() % 10;
                                        Start_Pattern();
                                    } else
                                        // display level 6 graphics
                                        if (level_setting == LEVEL6) {
                                            if (music_enabled == MUSIC_ON) {
                                                Music_Play((music_ptr)&game_song, 7);
                                            }
                                            cur_music_track = 7;
                                            PCX_Load("level6.pcx", (pcx_picture_ptr)&level_pcx, 1);
                                            PCX_Show_Buffer((pcx_picture_ptr)&level_pcx);
                                            PCX_Delete((pcx_picture_ptr)&level_pcx);
                                            pattern_number = rand() % 10;
                                            Start_Pattern();
                                        } else
                                            // display level 7 graphics
                                            if (level_setting == LEVEL7) {
                                                if (music_enabled == MUSIC_ON) {
                                                    Music_Play((music_ptr)&game_song, 8);
                                                }
                                                cur_music_track = 8;
                                                PCX_Load("level7.pcx", (pcx_picture_ptr)&level_pcx, 1);
                                                PCX_Show_Buffer((pcx_picture_ptr)&level_pcx);
                                                PCX_Delete((pcx_picture_ptr)&level_pcx);
                                                pattern_number = rand() % 10;
                                                Start_Pattern();
                                            } else
                                                // display level 8 graphics
                                                if (level_setting == LEVEL8) {
                                                    if (music_enabled == MUSIC_ON) {
                                                        Music_Play((music_ptr)&game_song, 1);
                                                    }
                                                    cur_music_track = 1;
                                                    PCX_Load("level8.pcx", (pcx_picture_ptr)&level_pcx, 1);
                                                    PCX_Show_Buffer((pcx_picture_ptr)&level_pcx);
                                                    PCX_Delete((pcx_picture_ptr)&level_pcx);
                                                    pattern_number = rand() % 10;
                                                    Start_Pattern();
                                                } else
                                                    // display level 9 graphics
                                                    if (level_setting == LEVEL9) {
                                                        if (music_enabled == MUSIC_ON) {
                                                            Music_Play((music_ptr)&game_song, 9);
                                                        }
                                                        cur_music_track = 9;
                                                        PCX_Load("level9.pcx", (pcx_picture_ptr)&level_pcx, 1);
                                                        PCX_Show_Buffer((pcx_picture_ptr)&level_pcx);
                                                        PCX_Delete((pcx_picture_ptr)&level_pcx);
                                                        pattern_number = rand() % 10;
                                                        Start_Pattern();
                                                    } else
                                                        // display level 10 graphics
                                                        if (level_setting == LEVEL10) {
                                                            if (music_enabled == MUSIC_ON) {
                                                                Music_Play((music_ptr)&game_song, 10);
                                                            }
                                                            cur_music_track = 10;
                                                            PCX_Load("level10.pcx", (pcx_picture_ptr)&level_pcx, 1);
                                                            PCX_Show_Buffer((pcx_picture_ptr)&level_pcx);
                                                            PCX_Delete((pcx_picture_ptr)&level_pcx);
                                                            pattern_number = rand() % 10;
                                                            Start_Pattern();
                                                        }
                    Delay(15);
                    Melt();
                    play();
                }
            }
            Sprite_Delete((sprite_ptr)&picright);
            Delete_Power_Line();
            if (sound_enabled) {
                Delete_Game_Sounds();
            }
            Delete_Aliens();
            level_setting = LEVEL1;
            game_state = GAME_MENU;
        }
        
    } // End WHile game state is not over
    // Pick Which Transition To Do For Screen
    rand();
    // Get A Random Number and Test For 0
    fx = rand() % 6;
    while (fx == 0) {
        fx = rand() % 2;
    }
    // Fade The Lights
    if ((fx == 5) || (fx == 6)) {
        Read_Palette(0, 255, (RGB_palette_ptr)&game_palette);
        Fade_Lights();
        clrscr();
        Write_Palette(0, 255, (RGB_palette_ptr)&game_palette);
    }
    // Sheer The Screen
    if ((fx == 3) || (fx == 4)) {
        Sheer();
    }
    // Dissolve The Screen
    if (fx == 2) {
        Sweep_Rectangle();;
    }
    // Melt The Screen
    if (fx == 1) {
        Melt();
    }
}

// The Main Game Function
void play()
{
    // Initializations
    int answer, c, number, index;
    pcx_picture background_pcx;
    
    // Load the background imagery
    PCX_Init((pcx_picture_ptr)&background_pcx);
    PCX_Load("main.pcx", (pcx_picture_ptr)&background_pcx, 1);
    PCX_Copy_To_Buffer((pcx_picture_ptr)&background_pcx, double_buffer);
    Show_Points();
    PCX_Delete((pcx_picture_ptr)&background_pcx);
    // print out the level difficulty
    // print out medium for difficulty
    if (dif_cur_setting == DIF_NORMAL) {
        Blit_String_DB(15, 188, 245, "Medium", 1);
    } else
        // print out easy for difficulty
        if (dif_cur_setting == DIF_EASY) {
            Blit_String_DB(15, 188, 245, "Easy", 1);
        } else
            // print out hard for difficulty
            if (dif_cur_setting == DIF_HARD) {
                Blit_String_DB(15, 188, 245, "Hard", 1);
            }
    // print out level 1 for difficulty
    if (level_setting == LEVEL1) {
        Blit_String_DB(15, 180, 245, "Level 1", 1);
    } else
        // print out level 2 for difficulty
        if (level_setting == LEVEL2) {
            Blit_String_DB(15, 180, 245, "Level 2", 1);
        } else
            // print out level 3 for difficulty
            if (level_setting == LEVEL3) {
                Blit_String_DB(15, 180, 245, "Level 3", 1);
            } else
                // print out level 4 for difficulty
                if (level_setting == LEVEL4) {
                    Blit_String_DB(15, 180, 245, "Level 4", 1);
                } else
                    // print out level 5 for difficulty
                    if (level_setting == LEVEL5) {
                        Blit_String_DB(15, 180, 245, "Level 5", 1);
                    } else
                        // print out level 6 for difficulty
                        if (level_setting == LEVEL6) {
                            Blit_String_DB(15, 180, 245, "Level 6", 1);
                        } else
                            // print out level 7 for difficulty
                            if (level_setting == LEVEL7) {
                                Blit_String_DB(15, 180, 245, "Level 7", 1);
                            } else
                                // print out level 8 for difficulty
                                if (level_setting == LEVEL8) {
                                    Blit_String_DB(15, 180, 245, "Level 8", 1);
                                } else
                                    // print out level 9 for difficulty
                                    if (level_setting == LEVEL9) {
                                        Blit_String_DB(15, 180, 245, "Level 9", 1);
                                    } else
                                        // print out level 10 for difficulty
                                        if (level_setting == LEVEL10) {
                                            Blit_String_DB(15, 180, 245, "Level 10", 1);
                                        }
    player_ship.x = players_sx;
    player_ship.y = players_sy;
    init_stars();
    draw_stars();
    Delay(1);
    Draw_Power_Line();
    Behind_Aliens();
    Draw_Aliens();
    Draw_Player_Ship();
    Delay(1);
    // Do Five Addition Problems
    for (c = 1, number = 1; c <= 5; c++, number++) {
        if (game_state == GAME_PLAYING) {
            cur_problem = number;
            game_clock = 0;
            clrproblem();
            // Increase the Difficulty Setting by one
            printproblem(number);
            Show_Double_Buffer(double_buffer);
            if (c == 3) {
                dif_setting++;
            }
            answer = 0;
            answer = add();
            // Increase The Number of right answers
            if (answer == 1) {
                right++;
            }
            cur_player.power = 22000;
        }
    }
    
    // Do five subtraction problems
    for (c = 1; c <= 5; c++, number++) {
        if (game_state == GAME_PLAYING) {
            cur_problem = number;
            game_clock = 0;
            clrproblem();
            // increase the difficulty setting by one
            printproblem(number);
            Show_Double_Buffer(double_buffer);
            if (c == 3) {
                dif_setting++;
            }
            answer = 0;
            answer = sub();
            // Increase the number of right answers
            if (answer == 1) {
                right++;
            }
            cur_player.power = 22000;
        }
    }
    // Free the memory used by the imagery
    clrscr();
    Fill_Double_Buffer(0);
    // Use the reportit() function to display the report card
    if (music_enabled == MUSIC_ON) {
        Music_Stop();
    }
    reportit();
    right = 0;
    return;
}

void Show_Points(void)
{
    char point_buf[15];
    static char point_buf2[15];
    static int already_done = 0;
    
    if (!game_action_state) {
        if (already_done) {
            Blit_String_DB(10, 70, 0, point_buf2, 1);
        }
        if (!already_done) {
            already_done = 1;
        }
        sprintf(point_buf, "%u", cur_player.points);
        Blit_String_DB(10, 70, 59, point_buf, 1);
        if (already_done) {
            sprintf(point_buf2, "%u", cur_player.points);
        }
    } else {
        sprintf(point_buf, "%u", cur_player.points);
        Blit_String_DB(140, 187, 145, point_buf, 1);
        sprintf(point_buf, "%d", cur_player.missile_type);
        Blit_String_DB(280, 187, 145, point_buf, 1);
        Blit_String_DB(290, 187, 145, ":", 1);
        sprintf(point_buf, "%d", num_missiles);
        Blit_String_DB(300, 187, 145, point_buf, 1);
    }
}

// the function to display the report card
void reportit()
{
    // the initializations
    float percent;
    pcx_picture background_pcx;
    
    // load and display the background image
    PCX_Init((pcx_picture_ptr)&background_pcx);
    PCX_Load("report.pcx", (pcx_picture_ptr)&background_pcx, 1);
    PCX_Show_Buffer((pcx_picture_ptr)&background_pcx);
    PCX_Delete((pcx_picture_ptr)&background_pcx);
    // determine the percent of correct answers
    percent = (((float)right) / ((float)10)) * 100;
    // display the information
    sprintf(report, "%d !!!", right);
    Blit_String(176, 68, 32, report, 1);
    sprintf(report, "%d !!!", 10 - right);
    Blit_String(189, 107, 32, report, 1);
    sprintf(report, "%.2f !!!", percent);
    Blit_String(180, 147, 32, report, 1);
    // Give the user a grade based on percentage of correct answers
    if (percent == 100) {
        Blit_String(239, 186, 32, "A+", 1);
        cur_player.points += P_LEVEL_BEAT;
        cur_player.power = 22000;
    } else if (percent >= 90) {
        Blit_Char(239, 186, 'A', 32, 1);
        cur_player.points += P_LEVEL_BEAT;
        cur_player.power = 22000;
    } else if (percent >= 80) {
        Blit_Char(239, 186, 'B', 32, 1);
        cur_player.points += P_LEVEL_BEAT;
        cur_player.power = 22000;
    } else if (percent >= 70) {
        Blit_Char(239, 186, 'C', 32, 1);
        cur_player.points += P_LEVEL_BEAT;
        cur_player.power = 22000;
    }
    // If the user recieves a 'D' or 'F' he or she is returned to the main menu and must start over
    else if (percent >= 65) {
        Blit_Char(239, 186, 'D', 32, 1);
        if (((cur_player.points -= P_LEVEL_LOSE) < 0)) {
            cur_player.points = 0;
        }
        game_state = GAME_MENU;
    } else {
        Blit_Char(239, 186, 'F', 32, 1);
        if (((cur_player.points -= P_LEVEL_LOSE) < 0)) {
            cur_player.points = 0;
        }
        game_state = GAME_MENU;
    }
    while (keys_active == 0);
    // Do A Screen Wipe
    Screen_Transition(SCREEN_SWIPE_X);
    // If the user recieved a low grade reload the controls menu
    if (game_state == GAME_MENU) {
        clrscr();
        Test_For_Warrior();
        init_control();
    } else if (!(game_state == GAME_MENU)) {
        Load_Tiles();
        Init_World();
        Clear_Aliens();
        if (sound_enabled) {
            Sound_Unload((sound_ptr)&game_snd[CORRECT_SND]);
            Sound_Unload((sound_ptr)&game_snd[INCORRECT_SND]);
        }
        Init_Powerups();
        Init_Photons();
        game_action_state = 1;
        wait_state = 0;
        cur_player.power = 22000;
        power_line.x = 5;
        power_line.y = 189;
        Play_Action();
        Delete_Tiles();
        if (sound_enabled) {
            Sound_Load("correct.voc", (sound_ptr)&game_snd[CORRECT_SND], 1);
            Sound_Load("ncorrect.voc", (sound_ptr)&game_snd[INCORRECT_SND], 1);
        }
        Clear_Aliens();
        game_action_state = 0;
        power_line.x = 280;
        player_ship.state = PLAYER_ALIVE;
        power_line.y = 7;
        worldn++;
        if (level_setting == LEVEL10) {
            game_state = GAME_MENU;
            clrscr();
            End_Sequence();
            clrscr();
            Test_For_Warrior();
            init_control();
        }
    }
}

// The function to randomly generate and display the addition problem
int add()
{
    // initializations
    int first, second;
    int answer = 0;
    
    // get the first number using
    // the rand() standard library function
    first = rand();
    
    // utilize our difficulty setting to generate the correct number
    first = (rand() % dif_setting);
    
    // test for the first number being a 0, if it is
    // generate a new one until it isn't
    while (first == 0) {
        first = (rand() % dif_setting);
    }
    
    // get the second number using
    // the rand() standard library function
    second = rand();
    second = rand();
    
    // utilize our difficulty setting to generate the correct number
    second = (rand() % dif_setting);
    
    // test for the second number being a 0, if it is
    // generate a new one until it isn't
    while (second == 0) {
        second = (rand() % dif_setting);
    }
    
    // printf the first generated number into the character array "numbers" so it can be used by
    // our blit_string() function
    sprintf(numbers, "%d", first);
    // these next if's do a little organization of the
    // numbers displayed on the screen so they
    // line up in correct mathematical order
    first1 = first;
    if (first < 10) {
        Blit_String_DB(31, 13, 55, numbers, 0);
    }
    if (first >= 10) {
        Blit_String_DB(23, 13, 55, numbers, 0);
    }
    // print out the plus sign
    Blit_Char_DB(13, 21, '+', 55, 0);
    // printf the second generated number into the character array "numbers so it can be used
    // by our blit_string() function
    sprintf(numbers, "%d", second);
    // these next if's do a little organization of the
    // numbers displayed on the screen so they
    // line up in correct mathematical order
    second1 = second;
    if (second < 10) {
        Blit_String_DB(31, 21, 55, numbers, 0);
    }
    if (second >= 10) {
        Blit_String_DB(23, 21, 55, numbers, 0);
    }
    // draw the line to complete the form of the problem
    Bline_DB(13, 30, 42, 30, 55);
    Show_Double_Buffer(double_buffer);
    addit = get_answer(first, second, 1);
    if (game_state == GAME_PLAYING) {
        // return 1 if the user got it correct
        // return 2 if the user was incorrect
        if (addit == 1) {
            // these next 7 lines of code, display the right/wrong graphics
            picright.curr_frame = 0;
            cur_player.points += P_RIGHT_ANS;
            Sprite_Under((sprite_ptr)&picright, double_buffer);
            Sprite_Draw((sprite_ptr)&picright, double_buffer, 1);
            Blit_String_DB(5, 150, 116, "Hit Any Key", 1);
            Show_Double_Buffer(double_buffer);
            FX_Play(CORRECT_SND, 1);
            Delay(3);
            while (keys_active == 0);
            Blit_String_DB(5, 150, 0, "Hit Any Key", 1);
            Sprite_Erase((sprite_ptr)&picright, double_buffer);
            Show_Double_Buffer(double_buffer);
            return 1;
        } else {
            // these next 7 lines of code, display the right/wrong graphics
            picright.curr_frame = 1;
            if (((cur_player.points -= P_WRONG_ANS) < 0)) {
                cur_player.points = 0;
            }
            Sprite_Under((sprite_ptr)&picright, double_buffer);
            Sprite_Draw((sprite_ptr)&picright, double_buffer, 1);
            Blit_String_DB(5, 150, 116, "Hit Any Key", 1);
            Show_Double_Buffer(double_buffer);
            FX_Play(INCORRECT_SND, 1);
            Delay(3);
            while (keys_active == 0);
            Blit_String_DB(5, 150, 0, "Hit Any Key", 1);
            Sprite_Erase((sprite_ptr)&picright, double_buffer);
            Show_Double_Buffer(double_buffer);
            return 2;
        }
    }
}

// the function to randomly generate and display the subtraction problem
int sub()
{
    // initializations
    int first, second, dif;
    
    int answer = 0;
    
    // find the first number
    // use the rand() function found in the standard library
    first = rand();
    
    // utilize our difficulty setting to determine the right number to use
    first = (rand() % dif_setting);
    
    // test for the first number being 0, if it is
    // generate a new one until the number
    // is no longer 0
    while (first == 0) {
        first = (rand() % dif_setting);
    }
    
    // begin the generation of the second number
    second = rand();
    
    // utilize the difficulty setting to find the correct number
    second = (rand() % dif_setting);
    
    // test for the second number being 0, and then
    // find a new second number until it is greater than 0
    while (second == 0) {
        second = (rand() % dif_setting);
    }
    
    // test for the answer being negative, (the first number
    // less than the second) and generate a new first number
    // to be larger than the second
    while (first < second) {
        dif = (second - first);
        first += (dif + (rand() % ((int)(dif_setting / 2))));
    }
    
    // use printf to put the first number into the character array numbers so it can be used by blit_string()
    sprintf(numbers, "%d", first);
    // do a little text formatting so the numbers line up correctly
    first1 = first;
    if (first < 10) {
        Blit_String_DB(31, 13, 55, numbers, 0);
    }
    if (first >= 10) {
        Blit_String_DB(23, 13, 55, numbers, 0);
    }
    // print out the minus sign
    Blit_Char_DB(13, 21, '-', 55, 0);
    sprintf(numbers, "%d", second);
    // do a little text formatting so the numbers line up correctly
    second1 = second;
    if (second < 10) {
        Blit_String_DB(31, 21, 55, numbers, 0);
    }
    if (second >= 10) {
        Blit_String_DB(23, 21, 55, numbers, 0);
    }
    // complete the problem by drawing the line below it
    Bline_DB(13, 30, 42, 30, 55);
    Show_Double_Buffer(double_buffer);
    addit = get_answer(first, second, 0);
    if (game_state == GAME_PLAYING) {
        // return 1 if the user was correct
        // return 2 if the user was incorrect
        if (addit == 1) {
            picright.curr_frame = 0;
            cur_player.points += P_RIGHT_ANS;
            Sprite_Under((sprite_ptr)&picright, double_buffer);
            Sprite_Draw((sprite_ptr)&picright, double_buffer, 1);
            Blit_String_DB(5, 150, 116, "Hit Any Key", 1);
            Show_Double_Buffer(double_buffer);
            FX_Play(CORRECT_SND, 1);
            Delay(3);
            while (keys_active == 0);
            Blit_String_DB(5, 150, 0, "Hit Any Key", 1);
            Sprite_Erase((sprite_ptr)&picright, double_buffer);
            Show_Double_Buffer(double_buffer);
            return 1;
        } else {
            picright.curr_frame = 1;
            if (((cur_player.points -= P_WRONG_ANS) < 0)) {
                cur_player.points = 0;
            }
            Sprite_Under((sprite_ptr)&picright, double_buffer);
            Sprite_Draw((sprite_ptr)&picright, double_buffer, 1);
            Blit_String_DB(5, 150, 116, "Hit Any Key", 1);
            Show_Double_Buffer(double_buffer);
            FX_Play(INCORRECT_SND, 1);
            Delay(3);
            while (keys_active == 0);
            Blit_String_DB(5, 150, 0, "Hit Any Key", 1);
            Sprite_Erase((sprite_ptr)&picright, double_buffer);
            Show_Double_Buffer(double_buffer);
            return 2;
        }
    }
}

// My function to use our keyboard interrupt to recieve an answer to the problem
int get_answer(int first, int second, int mode)
{
    // Set the loop variable to 0
    int done = 0, done1 = 0, problem_ans, itsright, dir_count = 0, var = 1;
    static int index, index2;
    char answer_string[10];
    int counter = 0, ok_to_hit = 1;
    answer = 0;
    // this is the event loop for the game, everything happens here, it was originally meant to just
    // get the answer from the user but it grew and grew till it eventually ended up like this
    while (!done) {
        Erase_Power_Line();
        Erase_PDeath();
        Erase_Player_Ship();
        Erase_Aliens();
        answer_flag = 0;
        // test if the user is hitting a key
        if (keys_active > 0) {
            // test which key the user is hitting
            // if the user hits Q
            if (keyboard_state[MAKE_Q]) {
                // delay 3 ms
                Delay(3);
                // do a screen wipe
                Screen_Transition(SCREEN_SWIPE_Y);
                // clear the screen
                clrscr();
                // blink the exiting message
                for (index = 0; index < 5; index++) {
                    // print the string
                    blit_string3(120, 95, 131, 133, 135, "Exiting...", 0);
                    // wait so the user can see it
                    Delay(5);
                    // erase the string
                    blit_string3(120, 95, 0, 0, 0, "Exiting...", 0);
                    // wait so the user can see it
                    Delay(5);
                }
                // set the game_state variable to the GAME_MENU
                game_state = GAME_MENU;
                // set both done variables to yes (1)
                done = 1;
                done1 = 1;
                break;
            } else if ((keyboard_state[MAKE_D]) && (keyboard_state[MAKE_M]) && (keyboard_state[MAKE_W])) {
                // delay 3 ms
                level_setting = LEVEL9;
            } else if (keyboard_state[MAKE_P]) {
                // delay 3 ms
                Delay(3);
                // clear the screen
                clrscr();
                // do the screen saver stuff
                screen_saver();
                // readraw all the imagery for the game
                Show_Double_Buffer(double_buffer);
            } else if (keyboard_state[MAKE_0]) {
                // set the answer to 0
                answer = 0;
                // set the answer flag variable to 1 because the user has hit an answer
                answer_flag = 1;
                // print out the 0 number in the 1's column
                Blit_Char_DB(31, 32, '0', 55, 0);
                // while the user wants the answer to be zero
                while (!done1) {
                    Animate_Pdeath();
                    Move_Aliens();
                    Draw_Stars();
                    Behind_Aliens();
                    Draw_Aliens();
                    Draw_Player_Ship();
                    Behind_PDeath();
                    Draw_PDeath();
                    Draw_Power_Line();
                    Show_Points();
                    if (fast_machine) {
                        Delay(1);
                    }
                    Wait_For_Vsync();
                    Show_Double_Buffer(double_buffer);
                    game_clock++;
                    if ((cur_player.power -= 20) == 0) {
                        player_ship.state = PLAYER_DYING;
                        cur_player.power = 22000;
                        done = 1;
                        done1 = 1;
                        itsright = 0;
                    }
                    // if the user is hitting another key determine what it is
                    if (keys_active > 0) {
                        // if the user wants to quit
                        if (keyboard_state[MAKE_Q]) {
                            // delay 3 ms
                            Delay(3);
                            // do a vertical screen wipe
                            Screen_Transition(SCREEN_SWIPE_Y);
                            // clear the screen
                            clrscr();
                            // blink the 3 color exit string
                            for (index = 0; index < 5; index++) {
                                // write the string
                                blit_string3(120, 95, 131, 133, 135, "Exiting...", 0);
                                // delay for a second
                                Delay(5);
                                // erase the string by printing it in black
                                blit_string3(120, 95, 0, 0, 0, "Exiting...", 0);
                                // delay another second
                                Delay(5);
                            }
                            // set the game state variable to the GAME_MENU
                            game_state = GAME_MENU;
                            // set both done flags to 1
                            done = 1;
                            done1 = 1;
                            break;
                        } else
                            //if the user wants to pause the game
                            if (keyboard_state[MAKE_P]) {
                                // delay 3 ms
                                Delay(5);
                                // clear the screen
                                clrscr();
                                // do the screen saver
                                screen_saver();
                                // redraw all the graphics
                                Blit_Char_DB(31, 32, '0', 55, 0);
                                Show_Double_Buffer(double_buffer);
                                // delay 3 ms
                            } else
                                // the user wants to clear the current answer and start over
                                if (keyboard_state[MAKE_BKSP]) {
                                    // erase the 0
                                    Blit_Char_DB(31, 32, '0', 0, 0);
                                    // set the done1 flag to yes (1)
                                    done1 = 1;
                                } else
                                    // the user wants to accept the current answer
                                    if (keyboard_state[MAKE_ENTER]) {
                                        // determine if it is addition mode
                                        if (mode == 1) {
                                            // get the right answer
                                            problem_ans = (first + second);
                                        } else
                                            // or subtraction mode
                                            if (mode == 0) {
                                                // get the right answer
                                                problem_ans = (first - second);
                                            }
                                        // determine if the user got it right
                                        if (answer == problem_ans) {
                                            // set the itsright variable to yes (1)
                                            itsright = 1;
                                        } else {
                                            // set the itsright variable to no (0)
                                            itsright = 0;
                                        }
                                        // set the done flags to yes (1)
                                        done1 = 1;
                                        done = 1;
                                    }
                    }
                    Erase_Power_Line();
                    Erase_PDeath();
                    Erase_Player_Ship();
                    Erase_Aliens();
                    if (music_enabled == MUSIC_ON) {
                        if (Music_Status() != SEQUENCE_PLAYING) {
                            Music_Play((music_ptr)&game_song, cur_music_track);
                        }
                    }
                }
                // set the done flag to no (0)
                done1 = 0;
                // draw the next frame of animation for the star fields
            } // End if keyboard state ise key 0
            else
                // the user is hitting 1
                if (keyboard_state[MAKE_1]) {
                    // set the answer to 1
                    answer = 1;
                    // set the answer flag variable to yes because the user has an answer
                    answer_flag = 1;
                    ok_to_hit = 0;
                    // print out the 1 in the 1's column
                    Blit_Char_DB(31, 32, '1', 55, 0);
                    // while the user wants the answer to be 1
                    while (!done1) {
                        Animate_Pdeath();
                        Move_Aliens();
                        Draw_Stars();
                        Behind_Aliens();
                        Draw_Aliens();
                        Draw_Player_Ship();
                        Behind_PDeath();
                        Draw_PDeath();
                        Draw_Power_Line();
                        Show_Points();
                        if (fast_machine) {
                            Delay(1);
                        }
                        Wait_For_Vsync();
                        Show_Double_Buffer(double_buffer);
                        game_clock++;
                        if ((cur_player.power -= 20) == 0) {
                            player_ship.state = PLAYER_DYING;
                            cur_player.power = 22000;
                            done = 1;
                            done1 = 1;
                            itsright = 0;
                        }
                        // if the user is hitting another key determine what it is
                        if (ok_to_hit) {
                            if (keys_active > 0);
                            {
                                // the user wants to quit
                                if (keyboard_state[MAKE_Q]) {
                                    // delay 3 ms
                                    Delay(3);
                                    // to a vertical screen wipe
                                    Screen_Transition(SCREEN_SWIPE_Y);
                                    // clear the screen
                                    clrscr();
                                    // blink the exiting string
                                    for (index = 0; index < 5; index++) {
                                        // print out the string
                                        blit_string3(120, 95, 131, 133, 135, "Exiting...", 0);
                                        // delay a second
                                        Delay(5);
                                        // erase the string by printing it again in black
                                        blit_string3(120, 95, 0, 0, 0, "Exiting...", 0);
                                        // delay a second
                                        Delay(5);
                                    }
                                    // set the game state variable to GAME_MENU
                                    game_state = GAME_MENU;
                                    // set the done flags to yes (1)
                                    done = 1;
                                    done1 = 1;
                                    // remove the keyboard driver
                                    // return 3
                                    return 3;
                                } else
                                    // the user wants to pause the game
                                    if (keyboard_state[MAKE_P]) {
                                        // delay 3 ms
                                        Delay(3);
                                        // clear the screen
                                        clrscr();
                                        // do the screen saver
                                        screen_saver();
                                        // redraw the screen graphics
                                        sprintf(answer_string, "%d", answer);
                                        if (answer < 10) {
                                            Blit_String_DB(31, 32, 55, answer_string, 0);
                                        } else {
                                            Blit_String_DB(23, 32, 55, answer_string, 0);
                                        }
                                        Show_Double_Buffer(double_buffer);
                                        // delay another 3 ms
                                        Delay(3);
                                    } else
                                        // the user wants to delete the current answer
                                        if (keyboard_state[MAKE_BKSP]) {
                                            // erase the current answer from the screen by running
                                            // through each digit and printing out in black
                                            for (index = 0; index < 10; index++) {
                                                Blit_Char_DB(23, 32, index, 0, 0);
                                                Blit_Char_DB(31, 32, index, 0, 0);
                                            }
                                            // set the done1 variable to yes (1)
                                            done1 = 1;
                                        } else
                                            // the user wants to accept the current answer
                                            if (keyboard_state[MAKE_ENTER]) {
                                                // test which mode, addition or subtraction the game is in
                                                // if it is addition
                                                if (mode == 1) {
                                                    // get the correct answer
                                                    problem_ans = (first + second);
                                                } else
                                                    // if it is subtraction
                                                    if (mode == 0) {
                                                        // get the correct answer
                                                        problem_ans = (first - second);
                                                    }
                                                // test the users answer against the correct answer
                                                if (answer == problem_ans) {
                                                    // if it is right set the variable to 1
                                                    itsright = 1;
                                                } else {
                                                    // the user got it wrong so set the variable to 0
                                                    itsright = 0;
                                                }
                                                // set the done variables to 1
                                                done = 1;
                                                done1 = 1;
                                            } else
                                                // the user wants the answer to be 11
                                                if (keyboard_state[MAKE_1]) {
                                                    // set the answer variable to 11
                                                    answer = 11;
                                                    // erase the current 1 already on the screen
                                                    Blit_Char_DB(31, 32, '1', 0, 0);
                                                    // now print the 11 out in the correct columns
                                                    Blit_String_DB(23, 32, 55, "11", 0);
                                                } else
                                                    // the user wants the answer to be 12
                                                    if (keyboard_state[MAKE_2]) {
                                                        // set the answer variable to 12
                                                        answer = 12;
                                                        // erase the current 1 already on the screen
                                                        Blit_Char_DB(31, 32, '1', 0, 0);
                                                        // print out the 12 string in the correct columns
                                                        Blit_String_DB(23, 32, 55, "12", 0);
                                                    } else
                                                        // the user wants the answer to be 13
                                                        if (keyboard_state[MAKE_3]) {
                                                            // set the answer variable to 13
                                                            answer = 13;
                                                            // erase the current 1 already on the screen
                                                            Blit_Char_DB(31, 32, '1', 0, 0);
                                                            // print out the 13 string in the correct columns
                                                            Blit_String_DB(23, 32, 55, "13", 0);
                                                        } else
                                                            // the user wants the answer to be 14
                                                            if (keyboard_state[MAKE_4]) {
                                                                // set the answer variable to 14
                                                                answer = 14;
                                                                // erase the 1 already on the screen
                                                                Blit_Char_DB(31, 32, '1', 0, 0);
                                                                // print out the 14 string in the correct columns
                                                                Blit_String_DB(23, 32, 55, "14", 0);
                                                            } else
                                                                // the user wnats the answer to be 15
                                                                if (keyboard_state[MAKE_5]) {
                                                                    // set the answer variable to 15
                                                                    answer = 15;
                                                                    // erase the 1 already on the screen
                                                                    Blit_Char_DB(31, 32, '1', 0, 0);
                                                                    // print out the 15 string in the correct columns
                                                                    Blit_String_DB(23, 32, 55, "15", 0);
                                                                } else if (keyboard_state[MAKE_6]) {
                                                                    answer = 16;
                                                                    Blit_Char_DB(31, 32, '1', 0, 0);
                                                                    Blit_String_DB(23, 32, 55, "16", 0);
                                                                } else if (keyboard_state[MAKE_7]) {
                                                                    answer = 17;
                                                                    Blit_Char_DB(31, 32, '1', 0, 0);
                                                                    Blit_String_DB(23, 32, 55, "17", 0);
                                                                } else if (keyboard_state[MAKE_8]) {
                                                                    answer = 18;
                                                                    Blit_Char_DB(31, 32, '1', 0, 0);
                                                                    Blit_String_DB(23, 32, 55, "18", 0);
                                                                } else if (keyboard_state[MAKE_9]) {
                                                                    answer = 19;
                                                                    Blit_Char_DB(31, 32, '1', 0, 0);
                                                                    Blit_String_DB(23, 32, 55, "19", 0);
                                                                } else if (keyboard_state[MAKE_0]) {
                                                                    answer = 10;
                                                                    Blit_Char_DB(31, 32, '1', 0, 0);
                                                                    Blit_String_DB(23, 32, 55, "10", 0);
                                                                }
                            }
                        }
                        if (!ok_to_hit) {
                            if (++counter == 7) {
                                ok_to_hit = 1;
                                counter = 0;
                            }
                        }
                        Erase_Power_Line();
                        Erase_PDeath();
                        Erase_Player_Ship();
                        Erase_Aliens();
                        if (music_enabled == MUSIC_ON) {
                            if (Music_Status() != SEQUENCE_PLAYING) {
                                Music_Play((music_ptr)&game_song, cur_music_track);
                            }
                        }
                    }
                    done1 = 0;
                } // end if keyboard state is 1
                else if (keyboard_state[MAKE_2]) {
                    answer = 2;
                    answer_flag = 1;
                    ok_to_hit = 0;
                    Blit_Char_DB(31, 32, '2', 55, 0);
                    while (!done1) {
                        Animate_Pdeath();
                        Move_Aliens();
                        Draw_Stars();
                        Behind_Aliens();
                        Draw_Aliens();
                        Draw_Player_Ship();
                        Behind_PDeath();
                        Draw_PDeath();
                        Draw_Power_Line();
                        Show_Points();
                        if (fast_machine) {
                            Delay(1);
                        }
                        Wait_For_Vsync();
                        Show_Double_Buffer(double_buffer);
                        game_clock++;
                        if ((cur_player.power -= 20) == 0) {
                            player_ship.state = PLAYER_DYING;
                            cur_player.power = 22000;
                            done = 1;
                            done1 = 1;
                            itsright = 0;
                        }
                        if (ok_to_hit) {
                            if (keys_active > 0);
                            {
                                if (keyboard_state[MAKE_Q]) {
                                    Delay(3);
                                    Screen_Transition(SCREEN_SWIPE_Y);
                                    clrscr();
                                    for (index = 0; index < 5; index++) {
                                        blit_string3(120, 95, 131, 133, 135, "Exiting...", 0);
                                        Delay(5);
                                        blit_string3(120, 95, 0, 0, 0, "Exiting...", 0);
                                        Delay(5);
                                    }
                                    game_state = GAME_MENU;
                                    done = 1;
                                    done1 = 1;
                                    return 3;
                                } else if (keyboard_state[MAKE_P]) {
                                    Delay(3);
                                    clrscr();
                                    screen_saver();
                                    sprintf(answer_string, "%d", answer);
                                    if (answer < 10) {
                                        Blit_String_DB(31, 32, 55, answer_string, 0);
                                    } else {
                                        Blit_String_DB(23, 32, 55, answer_string, 0);
                                    }
                                    Show_Double_Buffer(double_buffer);
                                    Delay(3);
                                } else if (keyboard_state[MAKE_BKSP]) {
                                    for (index = 0; index < 10; index++) {
                                        Blit_Char_DB(23, 32, index, 0, 0);
                                        Blit_Char_DB(31, 32, index, 0, 0);
                                    }
                                    done1 = 1;
                                } else if (keyboard_state[MAKE_ENTER]) {
                                    if (mode == 1) {
                                        problem_ans = (first + second);
                                    } else if (mode == 0) {
                                        problem_ans = (first - second);
                                    }
                                    if (answer == problem_ans) {
                                        itsright = 1;
                                    } else {
                                        itsright = 0;
                                    }
                                    done = 1;
                                    done1 = 1;
                                } else if (keyboard_state[MAKE_1]) {
                                    answer = 21;
                                    Blit_Char_DB(31, 32, '2', 0, 0);
                                    Blit_String_DB(23, 32, 55, "21", 0);
                                } else if (keyboard_state[MAKE_2]) {
                                    answer = 22;
                                    Blit_Char_DB(31, 32, '2', 0, 0);
                                    Blit_String_DB(23, 32, 55, "22", 0);
                                } else if (keyboard_state[MAKE_3]) {
                                    answer = 23;
                                    Blit_Char_DB(31, 32, '2', 0, 0);
                                    Blit_String_DB(23, 32, 55, "23", 0);
                                } else if (keyboard_state[MAKE_4]) {
                                    answer = 24;
                                    Blit_Char_DB(31, 32, '2', 0, 0);
                                    Blit_String_DB(23, 32, 55, "24", 0);
                                } else if (keyboard_state[MAKE_5]) {
                                    answer = 25;
                                    Blit_Char_DB(31, 32, '2', 0, 0);
                                    Blit_String_DB(23, 32, 55, "25", 0);
                                } else if (keyboard_state[MAKE_6]) {
                                    answer = 26;
                                    Blit_Char_DB(31, 32, '2', 0, 0);
                                    Blit_String_DB(23, 32, 55, "26", 0);
                                } else if (keyboard_state[MAKE_7]) {
                                    answer = 27;
                                    Blit_Char_DB(31, 32, '2', 0, 0);
                                    Blit_String_DB(23, 32, 55, "27", 0);
                                } else if (keyboard_state[MAKE_8]) {
                                    answer = 28;
                                    Blit_Char_DB(31, 32, '2', 0, 0);
                                    Blit_String_DB(23, 32, 55, "28", 0);
                                } else if (keyboard_state[MAKE_9]) {
                                    answer = 29;
                                    Blit_Char_DB(31, 32, '2', 0, 0);
                                    Blit_String_DB(23, 32, 55, "29", 0);
                                } else if (keyboard_state[MAKE_0]) {
                                    answer = 20;
                                    Blit_Char_DB(31, 32, '2', 0, 0);
                                    Blit_String_DB(23, 32, 55, "20", 0);
                                }
                            }
                        }
                        if (!ok_to_hit) {
                            if (++counter == 7) {
                                ok_to_hit = 1;
                                counter = 0;
                            }
                        }
                        Erase_Power_Line();
                        Erase_PDeath();
                        Erase_Player_Ship();
                        Erase_Aliens();
                        if (music_enabled == MUSIC_ON) {
                            if (Music_Status() != SEQUENCE_PLAYING) {
                                Music_Play((music_ptr)&game_song, cur_music_track);
                            }
                        }
                    }
                    done1 = 0;
                } // end if keyboard state is 2
                else if (keyboard_state[MAKE_3]) {
                    answer = 3;
                    answer_flag = 1;
                    ok_to_hit = 0;
                    Blit_Char_DB(31, 32, '3', 55, 0);
                    while (!done1) {
                        Animate_Pdeath();
                        Move_Aliens();
                        Draw_Stars();
                        Behind_Aliens();
                        Draw_Aliens();
                        Draw_Player_Ship();
                        Behind_PDeath();
                        Draw_PDeath();
                        Draw_Power_Line();
                        Show_Points();
                        if (fast_machine) {
                            Delay(1);
                        }
                        Wait_For_Vsync();
                        Show_Double_Buffer(double_buffer);
                        game_clock++;
                        if ((cur_player.power -= 20) == 0) {
                            player_ship.state = PLAYER_DYING;
                            cur_player.power = 22000;
                            done = 1;
                            done1 = 1;
                            itsright = 0;
                        }
                        if (ok_to_hit) {
                            if (keys_active > 0);
                            {
                                if (keyboard_state[MAKE_Q]) {
                                    Delay(3);
                                    Screen_Transition(SCREEN_SWIPE_Y);
                                    clrscr();
                                    for (index = 0; index < 5; index++) {
                                        blit_string3(120, 95, 131, 133, 135, "Exiting...", 0);
                                        Delay(5);
                                        blit_string3(120, 95, 0, 0, 0, "Exiting...", 0);
                                        Delay(5);
                                    }
                                    game_state = GAME_MENU;
                                    done = 1;
                                    done1 = 1;
                                    return 3;
                                } else if (keyboard_state[MAKE_P]) {
                                    Delay(3);
                                    clrscr();
                                    screen_saver();
                                    sprintf(answer_string, "%d", answer);
                                    if (answer < 10) {
                                        Blit_String_DB(31, 32, 55, answer_string, 0);
                                    } else {
                                        Blit_String_DB(23, 32, 55, answer_string, 0);
                                    }
                                    Show_Double_Buffer(double_buffer);
                                    Delay(3);
                                } else if (keyboard_state[MAKE_BKSP]) {
                                    for (index = 0; index < 10; index++) {
                                        Blit_Char_DB(23, 32, index, 0, 0);
                                        Blit_Char_DB(31, 32, index, 0, 0);
                                    }
                                    done1 = 1;
                                } else if (keyboard_state[MAKE_ENTER]) {
                                    if (mode == 1) {
                                        problem_ans = (first + second);
                                    } else if (mode == 0) {
                                        problem_ans = (first - second);
                                    }
                                    if (answer == problem_ans) {
                                        itsright = 1;
                                    } else {
                                        itsright = 0;
                                    }
                                    done = 1;
                                    done1 = 1;
                                } else if (keyboard_state[MAKE_1]) {
                                    answer = 31;
                                    Blit_Char_DB(31, 32, '3', 0, 0);
                                    Blit_String_DB(23, 32, 55, "31", 0);
                                } else if (keyboard_state[MAKE_2]) {
                                    answer = 32;
                                    Blit_Char_DB(31, 32, '3', 0, 0);
                                    Blit_String_DB(23, 32, 55, "32", 0);
                                } else if (keyboard_state[MAKE_3]) {
                                    answer = 33;
                                    Blit_Char_DB(31, 32, '3', 0, 0);
                                    Blit_String_DB(23, 32, 55, "33", 0);
                                } else if (keyboard_state[MAKE_4]) {
                                    answer = 34;
                                    Blit_Char_DB(31, 32, '3', 0, 0);
                                    Blit_String_DB(23, 32, 55, "34", 0);
                                } else if (keyboard_state[MAKE_5]) {
                                    answer = 35;
                                    Blit_Char_DB(31, 32, '3', 0, 0);
                                    Blit_String_DB(23, 32, 55, "35", 0);
                                } else if (keyboard_state[MAKE_6]) {
                                    answer = 36;
                                    Blit_Char_DB(31, 32, '3', 0, 0);
                                    Blit_String_DB(23, 32, 55, "36", 0);
                                } else if (keyboard_state[MAKE_7]) {
                                    answer = 37;
                                    Blit_Char_DB(31, 32, '3', 0, 0);
                                    Blit_String_DB(23, 32, 55, "37", 0);
                                } else if (keyboard_state[MAKE_8]) {
                                    answer = 38;
                                    Blit_Char_DB(31, 32, '3', 0, 0);
                                    Blit_String_DB(23, 32, 55, "38", 0);
                                } else if (keyboard_state[MAKE_9]) {
                                    answer = 39;
                                    Blit_Char_DB(31, 32, '3', 0, 0);
                                    Blit_String_DB(23, 32, 55, "39", 0);
                                } else if (keyboard_state[MAKE_0]) {
                                    answer = 30;
                                    Blit_Char_DB(31, 32, '3', 0, 0);
                                    Blit_String_DB(23, 32, 55, "30", 0);
                                }
                            }
                        }
                        if (!ok_to_hit) {
                            if (++counter == 7) {
                                ok_to_hit = 1;
                                counter = 0;
                            }
                        }
                        Erase_Power_Line();
                        Erase_PDeath();
                        Erase_Player_Ship();
                        Erase_Aliens();
                        if (music_enabled == MUSIC_ON) {
                            if (Music_Status() != SEQUENCE_PLAYING) {
                                Music_Play((music_ptr)&game_song, cur_music_track);
                            }
                        }
                    }
                    done1 = 0;
                } // end if keyboard state is 3
                else if (keyboard_state[MAKE_4]) {
                    answer = 4;
                    answer_flag = 1;
                    ok_to_hit = 0;
                    Blit_Char_DB(31, 32, '4', 55, 0);
                    while (!done1) {
                        Animate_Pdeath();
                        Move_Aliens();
                        Draw_Stars();
                        Behind_Aliens();
                        Draw_Aliens();
                        Draw_Player_Ship();
                        Behind_PDeath();
                        Draw_PDeath();
                        Draw_Power_Line();
                        Show_Points();
                        if (fast_machine) {
                            Delay(1);
                        }
                        Wait_For_Vsync();
                        Show_Double_Buffer(double_buffer);
                        game_clock++;
                        if ((cur_player.power -= 20) == 0) {
                            player_ship.state = PLAYER_DYING;
                            cur_player.power = 22000;
                            done = 1;
                            done1 = 1;
                            itsright = 0;
                        }
                        if (ok_to_hit) {
                            if (keys_active > 0);
                            {
                                if (keyboard_state[MAKE_Q]) {
                                    Delay(3);
                                    Screen_Transition(SCREEN_SWIPE_Y);
                                    clrscr();
                                    for (index = 0; index < 5; index++) {
                                        blit_string3(120, 95, 131, 133, 135, "Exiting...", 0);
                                        Delay(5);
                                        blit_string3(120, 95, 0, 0, 0, "Exiting...", 0);
                                        Delay(5);
                                    }
                                    game_state = GAME_MENU;
                                    done = 1;
                                    done1 = 1;
                                    return 3;
                                } else if (keyboard_state[MAKE_P]) {
                                    Delay(3);
                                    clrscr();
                                    screen_saver();
                                    sprintf(answer_string, "%d", answer);
                                    if (answer < 10) {
                                        Blit_String_DB(31, 32, 55, answer_string, 0);
                                    } else {
                                        Blit_String_DB(23, 32, 55, answer_string, 0);
                                    }
                                    Show_Double_Buffer(double_buffer);
                                    Delay(3);
                                } else if (keyboard_state[MAKE_BKSP]) {
                                    for (index = 0; index < 10; index++) {
                                        Blit_Char_DB(23, 32, index, 0, 0);
                                        Blit_Char_DB(31, 32, index, 0, 0);
                                    }
                                    done1 = 1;
                                } else if (keyboard_state[MAKE_ENTER]) {
                                    if (mode == 1) {
                                        problem_ans = (first + second);
                                    } else if (mode == 0) {
                                        problem_ans = (first - second);
                                    }
                                    if (answer == problem_ans) {
                                        itsright = 1;
                                    } else {
                                        itsright = 0;
                                    }
                                    done = 1;
                                    done1 = 1;
                                } else if (keyboard_state[MAKE_1]) {
                                    answer = 41;
                                    Blit_Char_DB(31, 32, '4', 0, 0);
                                    Blit_String_DB(23, 32, 55, "41", 0);
                                } else if (keyboard_state[MAKE_2]) {
                                    answer = 42;
                                    Blit_Char_DB(31, 32, '4', 0, 0);
                                    Blit_String_DB(23, 32, 55, "42", 0);
                                } else if (keyboard_state[MAKE_3]) {
                                    answer = 43;
                                    Blit_Char_DB(31, 32, '4', 0, 0);
                                    Blit_String_DB(23, 32, 55, "43", 0);
                                } else if (keyboard_state[MAKE_4]) {
                                    answer = 44;
                                    Blit_Char_DB(31, 32, '4', 0, 0);
                                    Blit_String_DB(23, 32, 55, "44", 0);
                                } else if (keyboard_state[MAKE_5]) {
                                    answer = 45;
                                    Blit_Char_DB(31, 32, '4', 0, 0);
                                    Blit_String_DB(23, 32, 55, "45", 0);
                                } else if (keyboard_state[MAKE_6]) {
                                    answer = 46;
                                    Blit_Char_DB(31, 32, '4', 0, 0);
                                    Blit_String_DB(23, 32, 55, "46", 0);
                                } else if (keyboard_state[MAKE_7]) {
                                    answer = 47;
                                    Blit_Char_DB(31, 32, '4', 0, 0);
                                    Blit_String_DB(23, 32, 55, "47", 0);
                                } else if (keyboard_state[MAKE_8]) {
                                    answer = 48;
                                    Blit_Char_DB(31, 32, '4', 0, 0);
                                    Blit_String_DB(23, 32, 55, "48", 0);
                                } else if (keyboard_state[MAKE_9]) {
                                    answer = 49;
                                    Blit_Char_DB(31, 32, '4', 0, 0);
                                    Blit_String_DB(23, 32, 55, "49", 0);
                                } else if (keyboard_state[MAKE_0]) {
                                    answer = 40;
                                    Blit_Char_DB(31, 32, '4', 0, 0);
                                    Blit_String_DB(23, 32, 55, "40", 0);
                                }
                            }
                        }
                        if (!ok_to_hit) {
                            if (++counter == 7) {
                                ok_to_hit = 1;
                                counter = 0;
                            }
                        }
                        Erase_Power_Line();
                        Erase_PDeath();
                        Erase_Player_Ship();
                        Erase_Aliens();
                        if (music_enabled == MUSIC_ON) {
                            if (Music_Status() != SEQUENCE_PLAYING) {
                                Music_Play((music_ptr)&game_song, cur_music_track);
                            }
                        }
                    }
                    done1 = 0;
                } // end if keyboard state is 4
                else if (keyboard_state[MAKE_5]) {
                    answer = 5;
                    answer_flag = 1;
                    ok_to_hit = 0;
                    Blit_Char_DB(31, 32, '5', 55, 0);
                    while (!done1) {
                        Animate_Pdeath();
                        Move_Aliens();
                        Draw_Stars();
                        Behind_Aliens();
                        Draw_Aliens();
                        Draw_Player_Ship();
                        Behind_PDeath();
                        Draw_PDeath();
                        Draw_Power_Line();
                        Show_Points();
                        if (fast_machine) {
                            Delay(1);
                        }
                        Wait_For_Vsync();
                        Show_Double_Buffer(double_buffer);
                        game_clock++;
                        if ((cur_player.power -= 20) == 0) {
                            player_ship.state = PLAYER_DYING;
                            cur_player.power = 22000;
                            done = 1;
                            done1 = 1;
                            itsright = 0;
                        }
                        if (ok_to_hit) {
                            if (keys_active > 0);
                            {
                                if (keyboard_state[MAKE_Q]) {
                                    Delay(3);
                                    Screen_Transition(SCREEN_SWIPE_Y);
                                    clrscr();
                                    for (index = 0; index < 5; index++) {
                                        blit_string3(120, 95, 131, 133, 135, "Exiting...", 0);
                                        Delay(5);
                                        blit_string3(120, 95, 0, 0, 0, "Exiting...", 0);
                                        Delay(5);
                                    }
                                    game_state = GAME_MENU;
                                    done = 1;
                                    done1 = 1;
                                    return 3;
                                } else if (keyboard_state[MAKE_P]) {
                                    Delay(3);
                                    clrscr();
                                    screen_saver();
                                    sprintf(answer_string, "%d", answer);
                                    if (answer < 10) {
                                        Blit_String_DB(31, 32, 55, answer_string, 0);
                                    } else {
                                        Blit_String_DB(23, 32, 55, answer_string, 0);
                                    }
                                    Show_Double_Buffer(double_buffer);
                                    Delay(3);
                                } else if (keyboard_state[MAKE_BKSP]) {
                                    for (index = 0; index < 10; index++) {
                                        Blit_Char_DB(23, 32, index, 0, 0);
                                        Blit_Char_DB(31, 32, index, 0, 0);
                                    }
                                    done1 = 1;
                                } else if (keyboard_state[MAKE_ENTER]) {
                                    if (mode == 1) {
                                        problem_ans = (first + second);
                                    } else if (mode == 0) {
                                        problem_ans = (first - second);
                                    }
                                    if (answer == problem_ans) {
                                        itsright = 1;
                                    } else {
                                        itsright = 0;
                                    }
                                    done = 1;
                                    done1 = 1;
                                } else if (keyboard_state[MAKE_1]) {
                                    answer = 51;
                                    Blit_Char_DB(31, 32, '5', 0, 0);
                                    Blit_String_DB(23, 32, 55, "51", 0);
                                } else if (keyboard_state[MAKE_2]) {
                                    answer = 52;
                                    Blit_Char_DB(31, 32, '5', 0, 0);
                                    Blit_String_DB(23, 32, 55, "52", 0);
                                } else if (keyboard_state[MAKE_3]) {
                                    answer = 53;
                                    Blit_Char_DB(31, 32, '5', 0, 0);
                                    Blit_String_DB(23, 32, 55, "53", 0);
                                } else if (keyboard_state[MAKE_4]) {
                                    answer = 54;
                                    Blit_Char_DB(31, 32, '5', 0, 0);
                                    Blit_String_DB(23, 32, 55, "54", 0);
                                } else if (keyboard_state[MAKE_5]) {
                                    answer = 55;
                                    Blit_Char_DB(31, 32, '5', 0, 0);
                                    Blit_String_DB(23, 32, 55, "55", 0);
                                } else if (keyboard_state[MAKE_6]) {
                                    answer = 56;
                                    Blit_Char_DB(31, 32, '5', 0, 0);
                                    Blit_String_DB(23, 32, 55, "56", 0);
                                } else if (keyboard_state[MAKE_7]) {
                                    answer = 57;
                                    Blit_Char_DB(31, 32, '5', 0, 0);
                                    Blit_String_DB(23, 32, 55, "57", 0);
                                } else if (keyboard_state[MAKE_8]) {
                                    answer = 58;
                                    Blit_Char_DB(31, 32, '5', 0, 0);
                                    Blit_String_DB(23, 32, 55, "58", 0);
                                } else if (keyboard_state[MAKE_9]) {
                                    answer = 59;
                                    Blit_Char_DB(31, 32, '5', 0, 0);
                                    Blit_String_DB(23, 32, 55, "59", 0);
                                } else if (keyboard_state[MAKE_0]) {
                                    answer = 50;
                                    Blit_Char_DB(31, 32, '5', 0, 0);
                                    Blit_String_DB(23, 32, 55, "50", 0);
                                }
                            }
                        }
                        if (!ok_to_hit) {
                            if (++counter == 7) {
                                ok_to_hit = 1;
                                counter = 0;
                            }
                        }
                        Erase_Power_Line();
                        Erase_PDeath();
                        Erase_Player_Ship();
                        Erase_Aliens();
                        if (music_enabled == MUSIC_ON) {
                            if (Music_Status() != SEQUENCE_PLAYING) {
                                Music_Play((music_ptr)&game_song, cur_music_track);
                            }
                        }
                    }
                    done1 = 0;
                } // end if keyboard state is 5
                else if (keyboard_state[MAKE_6]) {
                    answer = 6;
                    answer_flag = 1;
                    ok_to_hit = 0;
                    Blit_Char_DB(31, 32, '6', 55, 0);
                    while (!done1) {
                        Animate_Pdeath();
                        Move_Aliens();
                        Draw_Stars();
                        Behind_Aliens();
                        Draw_Aliens();
                        Draw_Player_Ship();
                        Behind_PDeath();
                        Draw_PDeath();
                        Draw_Power_Line();
                        Show_Points();
                        if (fast_machine) {
                            Delay(1);
                        }
                        Wait_For_Vsync();
                        Show_Double_Buffer(double_buffer);
                        game_clock++;
                        if ((cur_player.power -= 20) == 0) {
                            player_ship.state = PLAYER_DYING;
                            cur_player.power = 22000;
                            done = 1;
                            done1 = 1;
                            itsright = 0;
                        }
                        if (ok_to_hit) {
                            if (keys_active > 0);
                            {
                                if (keyboard_state[MAKE_Q]) {
                                    Delay(3);
                                    Screen_Transition(SCREEN_SWIPE_Y);
                                    clrscr();
                                    for (index = 0; index < 5; index++) {
                                        blit_string3(120, 95, 131, 133, 135, "Exiting...", 0);
                                        Delay(5);
                                        blit_string3(120, 95, 0, 0, 0, "Exiting...", 0);
                                        Delay(5);
                                    }
                                    game_state = GAME_MENU;
                                    done = 1;
                                    done1 = 1;
                                    return 3;
                                } else if (keyboard_state[MAKE_P]) {
                                    Delay(3);
                                    clrscr();
                                    screen_saver();
                                    sprintf(answer_string, "%d", answer);
                                    if (answer < 10) {
                                        Blit_String_DB(31, 32, 55, answer_string, 0);
                                    } else {
                                        Blit_String_DB(23, 32, 55, answer_string, 0);
                                    }
                                    Show_Double_Buffer(double_buffer);
                                    Delay(3);
                                } else if (keyboard_state[MAKE_BKSP]) {
                                    for (index = 0; index < 10; index++) {
                                        Blit_Char_DB(23, 32, index, 0, 0);
                                        Blit_Char_DB(31, 32, index, 0, 0);
                                    }
                                    done1 = 1;
                                } else if (keyboard_state[MAKE_ENTER]) {
                                    if (mode == 1) {
                                        problem_ans = (first + second);
                                    } else if (mode == 0) {
                                        problem_ans = (first - second);
                                    }
                                    if (answer == problem_ans) {
                                        itsright = 1;
                                    } else {
                                        itsright = 0;
                                    }
                                    done = 1;
                                    done1 = 1;
                                } else if (keyboard_state[MAKE_1]) {
                                    answer = 61;
                                    Blit_Char_DB(31, 32, '6', 0, 0);
                                    Blit_String_DB(23, 32, 55, "61", 0);
                                } else if (keyboard_state[MAKE_2]) {
                                    answer = 62;
                                    Blit_Char_DB(31, 32, '6', 0, 0);
                                    Blit_String_DB(23, 32, 55, "62", 0);
                                } else if (keyboard_state[MAKE_3]) {
                                    answer = 63;
                                    Blit_Char_DB(31, 32, '6', 0, 0);
                                    Blit_String_DB(23, 32, 55, "63", 0);
                                } else if (keyboard_state[MAKE_4]) {
                                    answer = 64;
                                    Blit_Char_DB(31, 32, '6', 0, 0);
                                    Blit_String_DB(23, 32, 55, "64", 0);
                                } else if (keyboard_state[MAKE_5]) {
                                    answer = 65;
                                    Blit_Char_DB(31, 32, '6', 0, 0);
                                    Blit_String_DB(23, 32, 55, "65", 0);
                                } else if (keyboard_state[MAKE_6]) {
                                    answer = 66;
                                    Blit_Char_DB(31, 32, '6', 0, 0);
                                    Blit_String_DB(23, 32, 55, "66", 0);
                                } else if (keyboard_state[MAKE_7]) {
                                    answer = 67;
                                    Blit_Char_DB(31, 32, '6', 0, 0);
                                    Blit_String_DB(23, 32, 55, "67", 0);
                                } else if (keyboard_state[MAKE_8]) {
                                    answer = 68;
                                    Blit_Char_DB(31, 32, '6', 0, 0);
                                    Blit_String_DB(23, 32, 55, "68", 0);
                                } else if (keyboard_state[MAKE_9]) {
                                    answer = 69;
                                    Blit_Char_DB(31, 32, '6', 0, 0);
                                    Blit_String_DB(23, 32, 55, "69", 0);
                                } else if (keyboard_state[MAKE_0]) {
                                    answer = 60;
                                    Blit_Char_DB(31, 32, '6', 0, 0);
                                    Blit_String_DB(23, 32, 55, "60", 0);
                                }
                            }
                        }
                        if (!ok_to_hit) {
                            if (++counter == 7) {
                                ok_to_hit = 1;
                                counter = 0;
                            }
                        }
                        Erase_Power_Line();
                        Erase_PDeath();
                        Erase_Player_Ship();
                        Erase_Aliens();
                        if (music_enabled == MUSIC_ON) {
                            if (Music_Status() != SEQUENCE_PLAYING) {
                                Music_Play((music_ptr)&game_song, cur_music_track);
                            }
                        }
                    }
                    done1 = 0;
                } // end if keyboard state is 6
                else if (keyboard_state[MAKE_7]) {
                    answer = 7;
                    answer_flag = 1;
                    ok_to_hit = 0;
                    Blit_Char_DB(31, 32, '7', 55, 0);
                    while (!done1) {
                        Animate_Pdeath();
                        Move_Aliens();
                        Draw_Stars();
                        Behind_Aliens();
                        Draw_Aliens();
                        Draw_Player_Ship();
                        Behind_PDeath();
                        Draw_PDeath();
                        Draw_Power_Line();
                        Show_Points();
                        if (fast_machine) {
                            Delay(1);
                        }
                        Wait_For_Vsync();
                        Show_Double_Buffer(double_buffer);
                        game_clock++;
                        if ((cur_player.power -= 20) == 0) {
                            player_ship.state = PLAYER_DYING;
                            cur_player.power = 22000;
                            done = 1;
                            done1 = 1;
                            itsright = 0;
                        }
                        if (ok_to_hit) {
                            if (keys_active > 0);
                            {
                                if (keyboard_state[MAKE_Q]) {
                                    Delay(3);
                                    Screen_Transition(SCREEN_SWIPE_Y);
                                    clrscr();
                                    for (index = 0; index < 5; index++) {
                                        blit_string3(120, 95, 131, 133, 135, "Exiting...", 0);
                                        Delay(5);
                                        blit_string3(120, 95, 0, 0, 0, "Exiting...", 0);
                                        Delay(5);
                                    }
                                    game_state = GAME_MENU;
                                    done = 1;
                                    done1 = 1;
                                    return 3;
                                } else if (keyboard_state[MAKE_P]) {
                                    Delay(3);
                                    clrscr();
                                    screen_saver();
                                    sprintf(answer_string, "%d", answer);
                                    if (answer < 10) {
                                        Blit_String_DB(31, 32, 55, answer_string, 0);
                                    } else {
                                        Blit_String_DB(23, 32, 55, answer_string, 0);
                                    }
                                    Show_Double_Buffer(double_buffer);
                                    Delay(3);
                                } else if (keyboard_state[MAKE_BKSP]) {
                                    for (index = 0; index < 10; index++) {
                                        Blit_Char_DB(23, 32, index, 0, 0);
                                        Blit_Char_DB(31, 32, index, 0, 0);
                                    }
                                    done1 = 1;
                                } else if (keyboard_state[MAKE_ENTER]) {
                                    if (mode == 1) {
                                        problem_ans = (first + second);
                                    } else if (mode == 0) {
                                        problem_ans = (first - second);
                                    }
                                    if (answer == problem_ans) {
                                        itsright = 1;
                                    } else {
                                        itsright = 0;
                                    }
                                    done = 1;
                                    done1 = 1;
                                } else if (keyboard_state[MAKE_1]) {
                                    answer = 71;
                                    Blit_Char_DB(31, 32, '7', 0, 0);
                                    Blit_String_DB(23, 32, 55, "71", 0);
                                } else if (keyboard_state[MAKE_2]) {
                                    answer = 72;
                                    Blit_Char_DB(31, 32, '7', 0, 0);
                                    Blit_String_DB(23, 32, 55, "72", 0);
                                } else if (keyboard_state[MAKE_3]) {
                                    answer = 73;
                                    Blit_Char_DB(31, 32, '7', 0, 0);
                                    Blit_String_DB(23, 32, 55, "73", 0);
                                } else if (keyboard_state[MAKE_4]) {
                                    answer = 74;
                                    Blit_Char_DB(31, 32, '7', 0, 0);
                                    Blit_String_DB(23, 32, 55, "74", 0);
                                } else if (keyboard_state[MAKE_5]) {
                                    answer = 75;
                                    Blit_Char_DB(31, 32, '7', 0, 0);
                                    Blit_String_DB(23, 32, 55, "75", 0);
                                } else if (keyboard_state[MAKE_6]) {
                                    answer = 76;
                                    Blit_Char_DB(31, 32, '7', 0, 0);
                                    Blit_String_DB(23, 32, 55, "76", 0);
                                } else if (keyboard_state[MAKE_7]) {
                                    answer = 77;
                                    Blit_Char_DB(31, 32, '7', 0, 0);
                                    Blit_String_DB(23, 32, 55, "77", 0);
                                } else if (keyboard_state[MAKE_8]) {
                                    answer = 78;
                                    Blit_Char_DB(31, 32, '7', 0, 0);
                                    Blit_String_DB(23, 32, 55, "78", 0);
                                } else if (keyboard_state[MAKE_9]) {
                                    answer = 79;
                                    Blit_Char_DB(31, 32, '7', 0, 0);
                                    Blit_String_DB(23, 32, 55, "79", 0);
                                } else if (keyboard_state[MAKE_0]) {
                                    answer = 70;
                                    Blit_Char_DB(31, 32, '7', 0, 0);
                                    Blit_String_DB(23, 32, 55, "70", 0);
                                }
                            }
                        }
                        if (!ok_to_hit) {
                            if (++counter == 7) {
                                ok_to_hit = 1;
                                counter = 0;
                            }
                        }
                        Erase_Power_Line();
                        Erase_PDeath();
                        Erase_Player_Ship();
                        Erase_Aliens();
                        if (music_enabled == MUSIC_ON) {
                            if (Music_Status() != SEQUENCE_PLAYING) {
                                Music_Play((music_ptr)&game_song, cur_music_track);
                            }
                        }
                    }
                    done1 = 0;
                } // end if keyboard state is 7
                else if (keyboard_state[MAKE_8]) {
                    answer = 8;
                    answer_flag = 1;
                    ok_to_hit = 0;
                    Blit_Char_DB(31, 32, '8', 55, 0);
                    while (!done1) {
                        Animate_Pdeath();
                        Move_Aliens();
                        Draw_Stars();
                        Behind_Aliens();
                        Draw_Aliens();
                        Draw_Player_Ship();
                        Behind_PDeath();
                        Draw_PDeath();
                        Draw_Power_Line();
                        Show_Points();
                        if (fast_machine) {
                            Delay(1);
                        }
                        Wait_For_Vsync();
                        Show_Double_Buffer(double_buffer);
                        game_clock++;
                        if ((cur_player.power -= 20) == 0) {
                            player_ship.state = PLAYER_DYING;
                            cur_player.power = 22000;
                            done = 1;
                            done1 = 1;
                            itsright = 0;
                        }
                        if (ok_to_hit) {
                            if (keys_active > 0);
                            {
                                if (keyboard_state[MAKE_Q]) {
                                    Delay(3);
                                    Screen_Transition(SCREEN_SWIPE_Y);
                                    clrscr();
                                    for (index = 0; index < 5; index++) {
                                        blit_string3(120, 95, 131, 133, 135, "Exiting...", 0);
                                        Delay(5);
                                        blit_string3(120, 95, 0, 0, 0, "Exiting...", 0);
                                        Delay(5);
                                    }
                                    game_state = GAME_MENU;
                                    done = 1;
                                    done1 = 1;
                                    return 3;
                                } else if (keyboard_state[MAKE_P]) {
                                    Delay(3);
                                    clrscr();
                                    screen_saver();
                                    sprintf(answer_string, "%d", answer);
                                    if (answer < 10) {
                                        Blit_String_DB(31, 32, 55, answer_string, 0);
                                    } else {
                                        Blit_String_DB(23, 32, 55, answer_string, 0);
                                    }
                                    Show_Double_Buffer(double_buffer);
                                    Delay(3);
                                } else if (keyboard_state[MAKE_BKSP]) {
                                    for (index = 0; index < 10; index++) {
                                        Blit_Char_DB(23, 32, index, 0, 0);
                                        Blit_Char_DB(31, 32, index, 0, 0);
                                    }
                                    done1 = 1;
                                } else if (keyboard_state[MAKE_ENTER]) {
                                    if (mode == 1) {
                                        problem_ans = (first + second);
                                    } else if (mode == 0) {
                                        problem_ans = (first - second);
                                    }
                                    if (answer == problem_ans) {
                                        itsright = 1;
                                    } else {
                                        itsright = 0;
                                    }
                                    done = 1;
                                    done1 = 1;
                                } else if (keyboard_state[MAKE_1]) {
                                    answer = 81;
                                    Blit_Char_DB(31, 32, '8', 0, 0);
                                    Blit_String_DB(23, 32, 55, "81", 0);
                                } else if (keyboard_state[MAKE_2]) {
                                    answer = 82;
                                    Blit_Char_DB(31, 32, '8', 0, 0);
                                    Blit_String_DB(23, 32, 55, "82", 0);
                                } else if (keyboard_state[MAKE_3]) {
                                    answer = 83;
                                    Blit_Char_DB(31, 32, '8', 0, 0);
                                    Blit_String_DB(23, 32, 55, "83", 0);
                                } else if (keyboard_state[MAKE_4]) {
                                    answer = 84;
                                    Blit_Char_DB(31, 32, '8', 0, 0);
                                    Blit_String_DB(23, 32, 55, "84", 0);
                                } else if (keyboard_state[MAKE_5]) {
                                    answer = 85;
                                    Blit_Char_DB(31, 32, '8', 0, 0);
                                    Blit_String_DB(23, 32, 55, "85", 0);
                                } else if (keyboard_state[MAKE_6]) {
                                    answer = 86;
                                    Blit_Char_DB(31, 32, '8', 0, 0);
                                    Blit_String_DB(23, 32, 55, "86", 0);
                                } else if (keyboard_state[MAKE_7]) {
                                    answer = 87;
                                    Blit_Char_DB(31, 32, '8', 0, 0);
                                    Blit_String_DB(23, 32, 55, "87", 0);
                                } else if (keyboard_state[MAKE_8]) {
                                    answer = 88;
                                    Blit_Char_DB(31, 32, '8', 0, 0);
                                    Blit_String_DB(23, 32, 55, "88", 0);
                                } else if (keyboard_state[MAKE_9]) {
                                    answer = 89;
                                    Blit_Char_DB(31, 32, '8', 0, 0);
                                    Blit_String_DB(23, 32, 55, "89", 0);
                                } else if (keyboard_state[MAKE_0]) {
                                    answer = 80;
                                    Blit_Char_DB(31, 32, '8', 0, 0);
                                    Blit_String_DB(23, 32, 55, "80", 0);
                                }
                            }
                        }
                        if (!ok_to_hit) {
                            if (++counter == 7) {
                                ok_to_hit = 1;
                                counter = 0;
                            }
                        }
                        Erase_Power_Line();
                        Erase_PDeath();
                        Erase_Player_Ship();
                        Erase_Aliens();
                        if (music_enabled == MUSIC_ON) {
                            if (Music_Status() != SEQUENCE_PLAYING) {
                                Music_Play((music_ptr)&game_song, cur_music_track);
                            }
                        }
                    }
                    done1 = 0;
                } // end if keyboard state is 8
                else if (keyboard_state[MAKE_9]) {
                    answer = 9;
                    answer_flag = 1;
                    ok_to_hit = 0;
                    Blit_Char_DB(31, 32, '9', 55, 0);
                    while (!done1) {
                        Animate_Pdeath();
                        Move_Aliens();
                        Draw_Stars();
                        Behind_Aliens();
                        Draw_Aliens();
                        Draw_Player_Ship();
                        Behind_PDeath();
                        Draw_PDeath();
                        Draw_Power_Line();
                        Show_Points();
                        if (fast_machine) {
                            Delay(1);
                        }
                        Wait_For_Vsync();
                        Show_Double_Buffer(double_buffer);
                        game_clock++;
                        if ((cur_player.power -= 20) == 0) {
                            player_ship.state = PLAYER_DYING;
                            cur_player.power = 22000;
                            done = 1;
                            done1 = 1;
                            itsright = 0;
                        }
                        if (ok_to_hit) {
                            if (keys_active > 0);
                            {
                                if (keyboard_state[MAKE_Q]) {
                                    Delay(3);
                                    Screen_Transition(SCREEN_SWIPE_Y);
                                    clrscr();
                                    for (index = 0; index < 5; index++) {
                                        blit_string3(120, 95, 131, 133, 135, "Exiting...", 0);
                                        Delay(5);
                                        blit_string3(120, 95, 0, 0, 0, "Exiting...", 0);
                                        Delay(5);
                                    }
                                    game_state = GAME_MENU;
                                    done = 1;
                                    done1 = 1;
                                    return 3;
                                } else if (keyboard_state[MAKE_P]) {
                                    Delay(3);
                                    clrscr();
                                    screen_saver();
                                    sprintf(answer_string, "%d", answer);
                                    if (answer < 10) {
                                        Blit_String_DB(31, 32, 55, answer_string, 0);
                                    } else {
                                        Blit_String_DB(23, 32, 55, answer_string, 0);
                                    }
                                    Show_Double_Buffer(double_buffer);
                                    Delay(3);
                                } else if (keyboard_state[MAKE_BKSP]) {
                                    for (index = 0; index < 10; index++) {
                                        Blit_Char_DB(23, 32, index, 0, 0);
                                        Blit_Char_DB(31, 32, index, 0, 0);
                                    }
                                    done1 = 1;
                                } else if (keyboard_state[MAKE_ENTER]) {
                                    if (mode == 1) {
                                        problem_ans = (first + second);
                                    } else if (mode == 0) {
                                        problem_ans = (first - second);
                                    }
                                    if (answer == problem_ans) {
                                        itsright = 1;
                                    } else {
                                        itsright = 0;
                                    }
                                    done = 1;
                                    done1 = 1;
                                } else if (keyboard_state[MAKE_1]) {
                                    answer = 91;
                                    Blit_Char_DB(31, 32, '9', 0, 0);
                                    Blit_String_DB(23, 32, 55, "91", 0);
                                } else if (keyboard_state[MAKE_2]) {
                                    answer = 92;
                                    Blit_Char_DB(31, 32, '9', 0, 0);
                                    Blit_String_DB(23, 32, 55, "92", 0);
                                } else if (keyboard_state[MAKE_3]) {
                                    answer = 93;
                                    Blit_Char_DB(31, 32, '9', 0, 0);
                                    Blit_String_DB(23, 32, 55, "93", 0);
                                } else if (keyboard_state[MAKE_4]) {
                                    answer = 94;
                                    Blit_Char_DB(31, 32, '9', 0, 0);
                                    Blit_String_DB(23, 32, 55, "94", 0);
                                } else if (keyboard_state[MAKE_5]) {
                                    answer = 95;
                                    Blit_Char_DB(31, 32, '9', 0, 0);
                                    Blit_String_DB(23, 32, 55, "95", 0);
                                } else if (keyboard_state[MAKE_6]) {
                                    answer = 96;
                                    Blit_Char_DB(31, 32, '9', 0, 0);
                                    Blit_String_DB(23, 32, 55, "96", 0);
                                } else if (keyboard_state[MAKE_7]) {
                                    answer = 97;
                                    Blit_Char_DB(31, 32, '9', 0, 0);
                                    Blit_String_DB(23, 32, 55, "97", 0);
                                } else if (keyboard_state[MAKE_8]) {
                                    answer = 98;
                                    Blit_Char_DB(31, 32, '9', 0, 0);
                                    Blit_String_DB(23, 32, 55, "98", 0);
                                } else if (keyboard_state[MAKE_9]) {
                                    answer = 99;
                                    Blit_Char_DB(31, 32, '9', 0, 0);
                                    Blit_String_DB(23, 32, 55, "99", 0);
                                } else if (keyboard_state[MAKE_0]) {
                                    answer = 90;
                                    Blit_Char_DB(31, 32, '9', 0, 0);
                                    Blit_String_DB(23, 32, 55, "90", 0);
                                }
                            }
                        }
                        if (!ok_to_hit) {
                            if (++counter == 7) {
                                ok_to_hit = 1;
                                counter = 0;
                            }
                        }
                        Erase_Power_Line();
                        Erase_PDeath();
                        Erase_Player_Ship();
                        Erase_Aliens();
                        if (music_enabled == MUSIC_ON) {
                            if (Music_Status() != SEQUENCE_PLAYING) {
                                Music_Play((music_ptr)&game_song, cur_music_track);
                            }
                        }
                    }
                    done1 = 0;
                } // end if keyboard state is 9
        } // End if keys active is greater than 0
        Animate_Pdeath();
        Move_Aliens();
        Draw_Stars();
        Behind_Aliens();
        Draw_Aliens();
        Draw_Player_Ship();
        Behind_PDeath();
        Draw_PDeath();
        Draw_Power_Line();
        Show_Points();
        if (fast_machine) {
            Delay(1);
        }
        Wait_For_Vsync();
        Show_Double_Buffer(double_buffer);
        game_clock++;
        if ((cur_player.power -= 20) == 0) {
            player_ship.state = PLAYER_DYING;
            cur_player.power = 22000;
            done = 1;
            itsright = 0;
        }
        if (music_enabled == MUSIC_ON) {
            if (Music_Status() != SEQUENCE_PLAYING) {
                Music_Play((music_ptr)&game_song, cur_music_track);
            }
        }
    }  // End Of while not done
    // Remove our keyboard driver
    if (itsright == 1) {
        return 1;
    } else if (itsright == 0) {
        return 0;
    }
}

// This is a simple function to load the video_buffer with the color black in o

// This function erases the current problem without destroying the rest of the screen.
// It uses the H_Line (horizontal line function) to draw 40 black lines across the current
// problems area
void clrproblem()
{
    int index;
    // for every time index is less than 53 draw a black horizontal line
    // at index's y position
    for (index = 13; index < 53; index++) {
        Bline_DB(13, index, 42, index, 0);
    }
}

// this function prints the problem number above the current problem
void printproblem(int number)
{
    int index;
    // first it erases the current problem number that is already displayed by drawing black
    // horizontal lines, same concept as the clrproblem() function
    for (index = 1; index < 9; index++) {
        BLine_DB(1, index, 82, index, 0);
    }
    // print the "Problem #" first and then print the number of problem
    Blit_String_DB(1, 1, 144, "Problem #", 1);
    // this switch is used as a fast way of printing out the right current number
    switch (number) {
            // print #1
        case 1:
            Blit_Char_DB(74, 1, '1', 144, 1);
            break;
            // print #2
        case 2:
            Blit_Char_DB(74, 1, '2', 144, 1);
            break;
            // print #3
        case 3:
            Blit_Char_DB(74, 1, '3', 144, 1);
            break;
            // print #4
        case 4:
            Blit_Char_DB(74, 1, '4', 144, 1);
            break;
            // print #5
        case 5:
            Blit_Char_DB(74, 1, '5', 144, 1);
            break;
            // print #6
        case 6:
            Blit_Char_DB(74, 1, '6', 144, 1);
            break;
            // print #7
        case 7:
            Blit_Char_DB(74, 1, '7', 144, 1);
            break;
            // print #8
        case 8:
            Blit_Char_DB(74, 1, '8', 144, 1);
            break;
            // print #9
        case 9:
            Blit_Char_DB(74, 1, '9', 144, 1);
            break;
            // print #10
        case 10:
            Blit_String_DB(74, 1, 144, "10", 1);
            break;
    }
}


// function to do the special effects on the main control panel
void Panel_FX(void)
{
// this function performs all of the special effects for the control panel

    int index; // lopping variable
    
    static int panel_counter = 0;  // used to time the color rotation of the panel
    
// is it time to update colors?

    if (++panel_counter > 1) {
        // reset counter
        
        panel_counter = 0;
        
        // do animation to colors
        
        Get_Palette_Register(41, (RGB_color_ptr)&color_1);
        
        for (index = 41; index > 32; index--) {
            // read the (i-1)th register
            
            Get_Palette_Register(index - 1, (RGB_color_ptr)&color_2);
            
            // assign it to the ith
            
            Set_Palette_Register(index, (RGB_color_ptr)&color_2);
            
        } // end rotate loop
        
        // place the value of the first color register into the last to
        // complete the rotation
        
        Set_Palette_Register(32, (RGB_color_ptr)&color_1);
        
    } // end if time
    
} // end Panel_FX

void Init_Stars(void)
{
// this function initializes all the stars in the star field

    int index; // looping variable
    
    for (index = 0; index < NUM_STARS; index++) {
    
        // select plane that star will be in
        
        switch (rand() % 3) {
            case 0: {
                stars[index].color = 8;
                stars[index].plane = PLANE_1;
            }
            break;
            
            case PLANE_1: {
                stars[index].color = 7;
                stars[index].plane = PLANE_2;
            }
            break;
            
            case PLANE_2: {
                stars[index].color = 15;
                stars[index].plane = PLANE_3;
            }
            break;
            
            default:
                break;
                
        } // end switch
        
        // set fields that aren't plane specific
        
        stars[index].x = rand() % 320; // change this latter to reflect clipping
        stars[index].y = rand() % 200; // region
        
        while ((stars[index].x < 106) || (stars[index].x > 315)) {
            stars[index].x = rand() % 320;
        }
        // make sure that the star does not surpass any of the boundaries for the y axis
        while ((stars[index].y < 13) || (stars[index].y > 197)) {
            stars[index].y = rand() % 200;
        }
    } // end for index
    
} // end Init_Stars

// this function is used to move the starfield
void draw_stars(void)
{
    int index, x, y;
    
    for (index = 0; index < NUM_STARS; index++) {
        // erase the star
        Plot_Pixel_Fast_DB(stars[index].x, stars[index].y, 0);
        
        // move the star and test for off-screen condition
        
        // each star is on a different plane so test for
        // which plane the star is on so that the proper velocity can
        // be used
        
        switch (stars[index].plane) {
            case PLANE_1: { // the slowest and farthest plane
                stars[index].y += velocity_1;
                if ((stars[index].x >= 243) && (stars[index].x <= 197)) {
                    if (stars[index].y > 187) {
                        stars[index].y = 14;
                    }
                }
                break;
            }
            case PLANE_2: { // the mid-speed plane
                stars[index].y += velocity_2;
                if ((stars[index].x >= 243) && (stars[index].x <= 197)) {
                    if (stars[index].y > 187) {
                        stars[index].y = 14;
                    }
                }
                break;
            }
            case PLANE_3: { // the fastest, nearest plane
                stars[index].y += velocity_3;
                if ((stars[index].x >= 243) && (stars[index].x <= 197)) {
                    if (stars[index].y > 187) {
                        stars[index].y = 14;
                    }
                }
                break;
            }
        } // end switch
        // test whether the star went past our boundaries
        if (stars[index].y > 197) { // past the bottom of the boundary
            stars[index].y = (stars[index].y - 184);
        }
        x = stars[index].x;
        y = stars[index].y;
        // draw the star in its final position
        Plot_Pixel_Fast_DB(x , y, stars[index].color);
    }
}

// my homemade screen saver
int screen_saver(void)
{
    int done = 0;
    int done1 = 0;
    int cur_saver;
    
    cur_saver = rand() % 5;
    while (cur_saver == 0) {
        cur_saver = rand() % 5;
    }
    while (!done) {
        // this function makes use of the bline function to bounce a line around
        if (cur_saver == SAVER_FOLLOW) {
            clrscr();
            while (!done1) {
                int xo, yo, x1, y1, x2, y2, x3, y3;
                int dxo, dyo, dx1, dy1, dx2, dy2, dx3, dy3;
                long counter = 0;
                int color = rand() % 16;
                
                // starting positions of lines
                
                xo = x2 = rand() % 320;
                yo = y2 = rand() % 200;
                x1 = x3 = rand() % 320;
                y1 = y3 = rand() % 200;
                
                // velocities of lines
                
                dxo = dx2 = 2 + rand() % 5;
                dyo = dy2 = 3 + rand() % 5;
                dx1 = dx3 = 2 + rand() % 5;
                dy1 = dy3 = 2 + rand() % 5;
                
                // animation loop
                
                while (keys_active == 0) {
                    // draw leader
                    
                    Bline(xo, yo, x1, y1, color);
                    
                    // move line
                    
                    if ((xo += dxo) >= 315 || xo < 5) {
                        dxo = -dxo;
                    }
                    
                    if ((yo += dyo) >= 195 || yo < 5) {
                        dyo = -dyo;
                    }
                    
                    if ((x1 += dx1) >= 315 || x1 < 5) {
                        dx1 = -dx1;
                    }
                    
                    if ((y1 += dy1) >= 195 || y1 < 5) {
                        dy1 = -dy1;
                    }
                    
                    // test if it's time to follow the leader
                    
                    if (++counter > 30) {
                    
                        Bline(x2, y2, x3, y3, 0);
                        
                        // move line
                        
                        if ((x2 += dx2) >= 315 || x2 < 5) {
                            dx2 = -dx2;
                        }
                        
                        if ((y2 += dy2) >= 195 || y2 < 5) {
                            dy2 = -dy2;
                        }
                        
                        if ((x3 += dx3) >= 315 || x3 < 5) {
                            dx3 = -dx3;
                        }
                        
                        if ((y3 += dy3) >= 195 || y3 < 5) {
                            dy3 = -dy3;
                        }
                        
                    } // end if time to follow
                    
                    // wait a while so humans can see it
                    Blit_String(70, 190, 245, "Hit Any Key To Continue...", 1);
                    // draw the puased sprite bitmap
                    blit_string3(5, 170, 51, 52, 54, "PAUSED", 1);
                    
                    Delay(1);
                    // update color
                    
                    if (counter > 100) {
                        if (++color >= 16) {
                            color = 1;
                        }
                        counter = 51;
                        
                    } // end if time to change color
                } // end while
                done1 = 1;
            }
            done1 = 0;
            cur_saver = NO_SAVER;
        }// end while screen saver is follow the leader
        else if (cur_saver == SAVER_LINES) {
            while (!done1) {
                clrscr();
                while (keys_active == 0) {
                    // plot a line between a random start and end point
                    
                    Bline(rand() % 320, rand() % 200, rand() % 320, rand() % 200, rand() % 256);
                    Blit_String(70, 190, 245, "Hit Any Key To Continue...", 0);
                    // draw the puased sprite bitmap
                    blit_string3(5, 170, 51, 52, 54, "PAUSED", 0);
                    Delay(1);
                    
                } // end while
                done1 = 1;
            }
            cur_saver = NO_SAVER;
            done1 = 0;
        } else if (cur_saver == SAVER_DOTS) {
            while (!done1) {
                clrscr();
                while (keys_active == 0) {
                    Plot_Pixel_Fast(rand() % 320, rand() % 200, rand() % 255);
                    Blit_String(70, 190, 245, "Hit Any Key To Continue...", 0);
                    // draw the puased sprite bitmap
                    blit_string3(5, 170, 51, 52, 54, "PAUSED", 0);
                }
                done1 = 1;
            }
            cur_saver = NO_SAVER;
            done1 = 0;
        } else if (cur_saver == SAVER_POLY) {
            while (!done1) {
                polygon square;
                vertex cur_vertex[4];
                int index, direction, go_index, color_index;
                int color[12];
                for (index = 0; index < 4; index++) {
                    cur_vertex[index].x = 16;
                    cur_vertex[index].y = 10;
                }
                color_index = 31;
                for (index = 0; index < 10; index++, color_index--) {
                    color[index] = color_index;
                }
                
                clrscr();
                while (keys_active == 0) {
                    color[10] = color[0];
                    color[0]  = color[9];
                    color[11] = color[1];
                    color[1] = color[10];
                    color[10] = color[2];
                    color[2] = color[11];
                    color[11] = color[3];
                    color[3] = color[10];
                    color[10] = color[4];
                    color[4] = color[11];
                    color[11] = color[5];
                    color[5] = color[10];
                    color[10] = color[6];
                    color[6] = color[11];
                    color[11] = color[7];
                    color[7] = color[10];
                    color[10] = color[8];
                    color[8] = color[11];
                    color[11] = color[9];
                    color[9] = color[10];
                    for (go_index = 0; go_index < 10 && keys_active == 0; go_index++) {
                        square.b_color = color[go_index];
                        square.i_color = square.b_color;
                        square.closed = 1;
                        square.filled = 0;
                        square.lxo = 160;
                        square.lyo = 100;
                        square.vertices[0].x = -cur_vertex[0].x;
                        square.vertices[0].y = -cur_vertex[0].y;
                        square.vertices[1].x = cur_vertex[1].x;
                        square.vertices[1].y = -cur_vertex[1].y;
                        square.vertices[2].x = cur_vertex[2].x;
                        square.vertices[2].y = cur_vertex[2].y;
                        square.vertices[3].x = -cur_vertex[3].x;
                        square.vertices[3].y = cur_vertex[3].y;
                        square.num_vertices = 4;
                        
                        Draw_Polygon((polygon_ptr)&square);
                        Delay(1);
                        
                        for (index = 0; index < 4 && keys_active == 0; index++) {
                            cur_vertex[index].x += 16;
                            cur_vertex[index].y += 10;
                            if (cur_vertex[index].x >= 144) {
                                cur_vertex[index].x = 16;
                                cur_vertex[index].y = 10;
                            }
                        }
                    }
                    Blit_String(70, 190, 245, "Hit Any Key To Continue...", 0);
                    // draw the puased sprite bitmap
                    blit_string3(5, 170, 51, 52, 54, "PAUSED", 0);
                }
                done1 = 1;
            }
            done1 = 0;
            cur_saver = NO_SAVER;
        } else if (cur_saver == SAVER_WORM) {
            while (!done1) {
                int index, moving_index;
                int direction = DOWN;
                int direction_decision, set_direction = 1, old_dir = DOWN_RIGHT;
                pixel worm[20];
                
                clrscr();
                for (index = 0; index < 20; index++) {
                    worm[index].color = rand() % 256;
                }
                worm[0].x = rand() % 290;
                worm[0].y = rand() % 170;
                while ((worm[0].x < 30) || (worm[0].y < 30)) {
                    worm[0].x = rand() % 290;
                    worm[0].y = rand() % 170;
                }
                for (index = 1; index < 20; index++) {
                    worm[index].x = worm[index - 1].x - 1;
                    worm[index].y = worm[index - 1].y - 1;
                }
                while (keys_active == 0) {
                    Fill_Double_Buffer(0);
                    if (set_direction == 1 && keys_active == 0) {
                        set_direction = 0;
                        switch (direction) {
                            case DOWN_RIGHT: {
                                switch (old_dir) {
                                    case DOWN: {
                                        for (moving_index = 1; moving_index <= 20 && keys_active == 0; moving_index++) {
                                            if (moving_index < 21) {
                                                worm[0].x++;
                                                Plot_Pixel_Fast(worm[0].x - 1, worm[0].y, 0);
                                                Plot_Pixel_Fast(worm[0].x, worm[0].y, worm[0].color);
                                            }
                                            if (moving_index < 20) {
                                                worm[1].x++;
                                                Plot_Pixel_Fast(worm[1].x - 1, worm[1].y, 0);
                                                Plot_Pixel_Fast(worm[1].x, worm[1].y, worm[1].color);
                                            }
                                            if (moving_index < 19) {
                                                worm[2].x++;
                                                Plot_Pixel_Fast(worm[2].x - 1, worm[2].y, 0);
                                                Plot_Pixel_Fast(worm[2].x, worm[2].y, worm[2].color);
                                            }
                                            if (moving_index < 18) {
                                                worm[3].x++;
                                                Plot_Pixel_Fast(worm[3].x - 1, worm[3].y, 0);
                                                Plot_Pixel_Fast(worm[3].x, worm[3].y, worm[3].color);
                                            }
                                            if (moving_index < 17) {
                                                worm[4].x++;
                                                Plot_Pixel_Fast(worm[4].x - 1, worm[4].y, 0);
                                                Plot_Pixel_Fast(worm[4].x, worm[4].y, worm[4].color);
                                            }
                                            if (moving_index < 16) {
                                                worm[5].x++;
                                                Plot_Pixel_Fast(worm[5].x - 1, worm[5].y, 0);
                                                Plot_Pixel_Fast(worm[5].x, worm[5].y, worm[5].color);
                                            }
                                            if (moving_index < 15) {
                                                worm[6].x++;
                                                Plot_Pixel_Fast(worm[6].x - 1, worm[6].y, 0);
                                                Plot_Pixel_Fast(worm[6].x, worm[6].y, worm[6].color);
                                            }
                                            if (moving_index < 14) {
                                                worm[7].x++;
                                                Plot_Pixel_Fast(worm[7].x - 1, worm[7].y, 0);
                                                Plot_Pixel_Fast(worm[7].x, worm[7].y, worm[7].color);
                                            }
                                            if (moving_index < 13) {
                                                worm[8].x++;
                                                Plot_Pixel_Fast(worm[8].x - 1, worm[8].y, 0);
                                                Plot_Pixel_Fast(worm[8].x, worm[8].y, worm[8].color);
                                            }
                                            if (moving_index < 12) {
                                                worm[9].x++;
                                                Plot_Pixel_Fast(worm[9].x - 1, worm[9].y, 0);
                                                Plot_Pixel_Fast(worm[9].x, worm[9].y, worm[9].color);
                                            }
                                            if (moving_index < 11) {
                                                worm[10].x++;
                                                Plot_Pixel_Fast(worm[10].x - 1, worm[10].y, 0);
                                                Plot_Pixel_Fast(worm[10].x, worm[10].y, worm[10].color);
                                            }
                                            if (moving_index < 10) {
                                                worm[11].x++;
                                                Plot_Pixel_Fast(worm[11].x - 1, worm[11].y, 0);
                                                Plot_Pixel_Fast(worm[11].x, worm[11].y, worm[11].color);
                                            }
                                            if (moving_index < 9) {
                                                worm[12].x++;
                                                Plot_Pixel_Fast(worm[12].x - 1, worm[12].y, 0);
                                                Plot_Pixel_Fast(worm[12].x, worm[12].y, worm[12].color);
                                            }
                                            if (moving_index < 8) {
                                                worm[13].x++;
                                                Plot_Pixel_Fast(worm[13].x - 1, worm[13].y, 0);
                                                Plot_Pixel_Fast(worm[13].x, worm[13].y, worm[13].color);
                                            }
                                            if (moving_index < 7) {
                                                worm[14].x++;
                                                Plot_Pixel_Fast(worm[14].x - 1, worm[14].y, 0);
                                                Plot_Pixel_Fast(worm[14].x, worm[14].y, worm[14].color);
                                            }
                                            if (moving_index < 6) {
                                                worm[15].x++;
                                                Plot_Pixel_Fast(worm[15].x - 1, worm[15].y, 0);
                                                Plot_Pixel_Fast(worm[15].x, worm[15].y, worm[15].color);
                                            }
                                            if (moving_index < 5) {
                                                worm[16].x++;
                                                Plot_Pixel_Fast(worm[16].x - 1, worm[16].y, 0);
                                                Plot_Pixel_Fast(worm[16].x, worm[16].y, worm[16].color);
                                            }
                                            if (moving_index < 4) {
                                                worm[17].x++;
                                                Plot_Pixel_Fast(worm[17].x - 1, worm[17].y, 0);
                                                Plot_Pixel_Fast(worm[17].x, worm[17].y, worm[17].color);
                                            }
                                            if (moving_index < 3) {
                                                worm[18].x++;
                                                Plot_Pixel_Fast(worm[18].x - 1, worm[18].y, 0);
                                                Plot_Pixel_Fast(worm[18].x, worm[18].y, worm[19].color);
                                            }
                                            if (moving_index < 2) {
                                                worm[19].x++;
                                                Plot_Pixel_Fast(worm[19].x - 1, worm[19].y, 0);
                                                Plot_Pixel_Fast(worm[19].x, worm[19].y, worm[19].color);
                                            }
                                            Blit_String(70, 190, 245, "Hit Any Key To Continue...", 1);
                                            // draw the puased sprite bitmap
                                            blit_string3(5, 170, 51, 52, 54, "PAUSED", 1);
                                            Delay(1);
                                        }
                                        break;
                                    }
                                    case RIGHT: {
                                        for (moving_index = 1; moving_index <= 20 && keys_active == 0; moving_index++) {
                                            if (moving_index < 21) {
                                                worm[0].y++;
                                                Plot_Pixel_Fast(worm[0].x, worm[0].y - 1, 0);
                                                Plot_Pixel_Fast(worm[0].x, worm[0].y, worm[0].color);
                                            }
                                            if (moving_index < 20) {
                                                worm[1].y++;
                                                Plot_Pixel_Fast(worm[1].x, worm[1].y - 1, 0);
                                                Plot_Pixel_Fast(worm[1].x, worm[1].y, worm[1].color);
                                            }
                                            if (moving_index < 19) {
                                                worm[2].y++;
                                                Plot_Pixel_Fast(worm[2].x, worm[2].y - 1, 0);
                                                Plot_Pixel_Fast(worm[2].x, worm[2].y, worm[2].color);
                                            }
                                            if (moving_index < 18) {
                                                worm[3].y++;
                                                Plot_Pixel_Fast(worm[3].x, worm[3].y - 1, 0);
                                                Plot_Pixel_Fast(worm[3].x, worm[3].y, worm[3].color);
                                            }
                                            if (moving_index < 17) {
                                                worm[4].y++;
                                                Plot_Pixel_Fast(worm[4].x, worm[4].y - 1, 0);
                                                Plot_Pixel_Fast(worm[4].x, worm[4].y, worm[4].color);
                                            }
                                            if (moving_index < 16) {
                                                worm[5].y++;
                                                Plot_Pixel_Fast(worm[5].x, worm[5].y - 1, 0);
                                                Plot_Pixel_Fast(worm[5].x, worm[5].y, worm[5].color);
                                            }
                                            if (moving_index < 15) {
                                                worm[6].y++;
                                                Plot_Pixel_Fast(worm[6].x, worm[6].y - 1, 0);
                                                Plot_Pixel_Fast(worm[6].x, worm[6].y, worm[6].color);
                                            }
                                            if (moving_index < 14) {
                                                worm[7].y++;
                                                Plot_Pixel_Fast(worm[7].x, worm[7].y - 1, 0);
                                                Plot_Pixel_Fast(worm[7].x, worm[7].y, worm[7].color);
                                            }
                                            if (moving_index < 13) {
                                                worm[8].y++;
                                                Plot_Pixel_Fast(worm[8].x, worm[8].y - 1, 0);
                                                Plot_Pixel_Fast(worm[8].x, worm[8].y, worm[8].color);
                                            }
                                            if (moving_index < 12) {
                                                worm[9].y++;
                                                Plot_Pixel_Fast(worm[9].x, worm[9].y - 1, 0);
                                                Plot_Pixel_Fast(worm[9].x, worm[9].y, worm[9].color);
                                            }
                                            if (moving_index < 11) {
                                                worm[10].y++;
                                                Plot_Pixel_Fast(worm[10].x, worm[10].y - 1, 0);
                                                Plot_Pixel_Fast(worm[10].x, worm[10].y, worm[10].color);
                                            }
                                            if (moving_index < 10) {
                                                worm[11].y++;
                                                Plot_Pixel_Fast(worm[11].x, worm[11].y - 1, 0);
                                                Plot_Pixel_Fast(worm[11].x, worm[11].y, worm[11].color);
                                            }
                                            if (moving_index < 9) {
                                                worm[12].y++;
                                                Plot_Pixel_Fast(worm[12].x, worm[12].y - 1, 0);
                                                Plot_Pixel_Fast(worm[12].x, worm[12].y, worm[12].color);
                                            }
                                            if (moving_index < 8) {
                                                worm[13].y++;
                                                Plot_Pixel_Fast(worm[13].x, worm[13].y - 1, 0);
                                                Plot_Pixel_Fast(worm[13].x, worm[13].y, worm[13].color);
                                            }
                                            if (moving_index < 7) {
                                                worm[14].y++;
                                                Plot_Pixel_Fast(worm[14].x, worm[14].y - 1, 0);
                                                Plot_Pixel_Fast(worm[14].x, worm[14].y, worm[14].color);
                                            }
                                            if (moving_index < 6) {
                                                worm[15].y++;
                                                Plot_Pixel_Fast(worm[15].x, worm[15].y - 1, 0);
                                                Plot_Pixel_Fast(worm[15].x, worm[15].y, worm[15].color);
                                            }
                                            if (moving_index < 5) {
                                                worm[16].y++;
                                                Plot_Pixel_Fast(worm[16].x, worm[16].y - 1, 0);
                                                Plot_Pixel_Fast(worm[16].x, worm[16].y, worm[16].color);
                                            }
                                            if (moving_index < 4) {
                                                worm[17].y++;
                                                Plot_Pixel_Fast(worm[17].x, worm[17].y - 1, 0);
                                                Plot_Pixel_Fast(worm[17].x, worm[17].y, worm[17].color);
                                            }
                                            if (moving_index < 3) {
                                                worm[18].y++;
                                                Plot_Pixel_Fast(worm[18].x, worm[18].y - 1, 0);
                                                Plot_Pixel_Fast(worm[18].x, worm[18].y, worm[19].color);
                                            }
                                            if (moving_index < 2) {
                                                worm[19].y++;
                                                Plot_Pixel_Fast(worm[19].x, worm[19].y - 1, 0);
                                                Plot_Pixel_Fast(worm[19].x, worm[19].y, worm[19].color);
                                            }
                                            Blit_String(70, 190, 245, "Hit Any Key To Continue...", 1);
                                            // draw the puased sprite bitmap
                                            blit_string3(5, 170, 51, 52, 54, "PAUSED", 1);
                                            Delay(1);
                                        }
                                        break;
                                    }
                                }
                                break;
                            }
                            case DOWN: {
                                switch (old_dir) {
                                    case DOWN_LEFT: {
                                        for (moving_index = 1; moving_index <= 20 && keys_active == 0; moving_index++) {
                                            if (moving_index < 21) {
                                                worm[0].x++;
                                                Plot_Pixel_Fast(worm[0].x - 1, worm[0].y, 0);
                                                Plot_Pixel_Fast(worm[0].x, worm[0].y, worm[0].color);
                                            }
                                            if (moving_index < 20) {
                                                worm[1].x++;
                                                Plot_Pixel_Fast(worm[1].x - 1, worm[1].y, 0);
                                                Plot_Pixel_Fast(worm[1].x, worm[1].y, worm[1].color);
                                            }
                                            if (moving_index < 19) {
                                                worm[2].x++;
                                                Plot_Pixel_Fast(worm[2].x - 1, worm[2].y, 0);
                                                Plot_Pixel_Fast(worm[2].x, worm[2].y, worm[2].color);
                                            }
                                            if (moving_index < 18) {
                                                worm[3].x++;
                                                Plot_Pixel_Fast(worm[3].x - 1, worm[3].y, 0);
                                                Plot_Pixel_Fast(worm[3].x, worm[3].y, worm[3].color);
                                            }
                                            if (moving_index < 17) {
                                                worm[4].x++;
                                                Plot_Pixel_Fast(worm[4].x - 1, worm[4].y, 0);
                                                Plot_Pixel_Fast(worm[4].x, worm[4].y, worm[4].color);
                                            }
                                            if (moving_index < 16) {
                                                worm[5].x++;
                                                Plot_Pixel_Fast(worm[5].x - 1, worm[5].y, 0);
                                                Plot_Pixel_Fast(worm[5].x, worm[5].y, worm[5].color);
                                            }
                                            if (moving_index < 15) {
                                                worm[6].x++;
                                                Plot_Pixel_Fast(worm[6].x - 1, worm[6].y, 0);
                                                Plot_Pixel_Fast(worm[6].x, worm[6].y, worm[6].color);
                                            }
                                            if (moving_index < 14) {
                                                worm[7].x++;
                                                Plot_Pixel_Fast(worm[7].x - 1, worm[7].y, 0);
                                                Plot_Pixel_Fast(worm[7].x, worm[7].y, worm[7].color);
                                            }
                                            if (moving_index < 13) {
                                                worm[8].x++;
                                                Plot_Pixel_Fast(worm[8].x - 1, worm[8].y, 0);
                                                Plot_Pixel_Fast(worm[8].x, worm[8].y, worm[8].color);
                                            }
                                            if (moving_index < 12) {
                                                worm[9].x++;
                                                Plot_Pixel_Fast(worm[9].x - 1, worm[9].y, 0);
                                                Plot_Pixel_Fast(worm[9].x, worm[9].y, worm[9].color);
                                            }
                                            if (moving_index < 11) {
                                                worm[10].x++;
                                                Plot_Pixel_Fast(worm[10].x - 1, worm[10].y, 0);
                                                Plot_Pixel_Fast(worm[10].x, worm[10].y, worm[10].color);
                                            }
                                            if (moving_index < 10) {
                                                worm[11].x++;
                                                Plot_Pixel_Fast(worm[11].x - 1, worm[11].y, 0);
                                                Plot_Pixel_Fast(worm[11].x, worm[11].y, worm[11].color);
                                            }
                                            if (moving_index < 9) {
                                                worm[12].x++;
                                                Plot_Pixel_Fast(worm[12].x - 1, worm[12].y, 0);
                                                Plot_Pixel_Fast(worm[12].x, worm[12].y, worm[12].color);
                                            }
                                            if (moving_index < 8) {
                                                worm[13].x++;
                                                Plot_Pixel_Fast(worm[13].x - 1, worm[13].y, 0);
                                                Plot_Pixel_Fast(worm[13].x, worm[13].y, worm[13].color);
                                            }
                                            if (moving_index < 7) {
                                                worm[14].x++;
                                                Plot_Pixel_Fast(worm[14].x - 1, worm[14].y, 0);
                                                Plot_Pixel_Fast(worm[14].x, worm[14].y, worm[14].color);
                                            }
                                            if (moving_index < 6) {
                                                worm[15].x++;
                                                Plot_Pixel_Fast(worm[15].x - 1, worm[15].y, 0);
                                                Plot_Pixel_Fast(worm[15].x, worm[15].y, worm[15].color);
                                            }
                                            if (moving_index < 5) {
                                                worm[16].x++;
                                                Plot_Pixel_Fast(worm[16].x - 1, worm[16].y, 0);
                                                Plot_Pixel_Fast(worm[16].x, worm[16].y, worm[16].color);
                                            }
                                            if (moving_index < 4) {
                                                worm[17].x++;
                                                Plot_Pixel_Fast(worm[17].x - 1, worm[17].y, 0);
                                                Plot_Pixel_Fast(worm[17].x, worm[17].y, worm[17].color);
                                            }
                                            if (moving_index < 3) {
                                                worm[18].x++;
                                                Plot_Pixel_Fast(worm[18].x - 1, worm[18].y, 0);
                                                Plot_Pixel_Fast(worm[18].x, worm[18].y, worm[19].color);
                                            }
                                            if (moving_index < 2) {
                                                worm[19].x++;
                                                Plot_Pixel_Fast(worm[19].x - 1, worm[19].y, 0);
                                                Plot_Pixel_Fast(worm[19].x, worm[19].y, worm[19].color);
                                            }
                                            Blit_String(70, 190, 245, "Hit Any Key To Continue...", 1);
                                            // draw the puased sprite bitmap
                                            blit_string3(5, 170, 51, 52, 54, "PAUSED", 1);
                                            Delay(1);
                                        }
                                        break;
                                    }
                                    case DOWN_RIGHT: {
                                        for (moving_index = 1; moving_index <= 20 && keys_active == 0; moving_index++) {
                                            if (moving_index < 21) {
                                                worm[0].x--;
                                                Plot_Pixel_Fast(worm[0].x + 1, worm[0].y, 0);
                                                Plot_Pixel_Fast(worm[0].x, worm[0].y, worm[0].color);
                                            }
                                            if (moving_index < 20) {
                                                worm[1].x--;
                                                Plot_Pixel_Fast(worm[1].x + 1, worm[1].y, 0);
                                                Plot_Pixel_Fast(worm[1].x, worm[1].y, worm[1].color);
                                            }
                                            if (moving_index < 19) {
                                                worm[2].x--;
                                                Plot_Pixel_Fast(worm[2].x + 1, worm[2].y, 0);
                                                Plot_Pixel_Fast(worm[2].x, worm[2].y, worm[2].color);
                                            }
                                            if (moving_index < 18) {
                                                worm[3].x--;
                                                Plot_Pixel_Fast(worm[3].x + 1, worm[3].y, 0);
                                                Plot_Pixel_Fast(worm[3].x, worm[3].y, worm[3].color);
                                            }
                                            if (moving_index < 17) {
                                                worm[4].x--;
                                                Plot_Pixel_Fast(worm[4].x + 1, worm[4].y, 0);
                                                Plot_Pixel_Fast(worm[4].x, worm[4].y, worm[4].color);
                                            }
                                            if (moving_index < 16) {
                                                worm[5].x--;
                                                Plot_Pixel_Fast(worm[5].x + 1, worm[5].y, 0);
                                                Plot_Pixel_Fast(worm[5].x, worm[5].y, worm[5].color);
                                            }
                                            if (moving_index < 15) {
                                                worm[6].x--;
                                                Plot_Pixel_Fast(worm[6].x + 1, worm[6].y, 0);
                                                Plot_Pixel_Fast(worm[6].x, worm[6].y, worm[6].color);
                                            }
                                            if (moving_index < 14) {
                                                worm[7].x--;
                                                Plot_Pixel_Fast(worm[7].x + 1, worm[7].y, 0);
                                                Plot_Pixel_Fast(worm[7].x, worm[7].y, worm[7].color);
                                            }
                                            if (moving_index < 13) {
                                                worm[8].x--;
                                                Plot_Pixel_Fast(worm[8].x + 1, worm[8].y, 0);
                                                Plot_Pixel_Fast(worm[8].x, worm[8].y, worm[8].color);
                                            }
                                            if (moving_index < 12) {
                                                worm[9].x--;
                                                Plot_Pixel_Fast(worm[9].x + 1, worm[9].y, 0);
                                                Plot_Pixel_Fast(worm[9].x, worm[9].y, worm[9].color);
                                            }
                                            if (moving_index < 11) {
                                                worm[10].x--;
                                                Plot_Pixel_Fast(worm[10].x + 1, worm[10].y, 0);
                                                Plot_Pixel_Fast(worm[10].x, worm[10].y, worm[10].color);
                                            }
                                            if (moving_index < 10) {
                                                worm[11].x--;
                                                Plot_Pixel_Fast(worm[11].x + 1, worm[11].y, 0);
                                                Plot_Pixel_Fast(worm[11].x, worm[11].y, worm[11].color);
                                            }
                                            if (moving_index < 9) {
                                                worm[12].x--;
                                                Plot_Pixel_Fast(worm[12].x + 1, worm[12].y, 0);
                                                Plot_Pixel_Fast(worm[12].x, worm[12].y, worm[12].color);
                                            }
                                            if (moving_index < 8) {
                                                worm[13].x--;
                                                Plot_Pixel_Fast(worm[13].x + 1, worm[13].y, 0);
                                                Plot_Pixel_Fast(worm[13].x, worm[13].y, worm[13].color);
                                            }
                                            if (moving_index < 7) {
                                                worm[14].x--;
                                                Plot_Pixel_Fast(worm[14].x + 1, worm[14].y, 0);
                                                Plot_Pixel_Fast(worm[14].x, worm[14].y, worm[14].color);
                                            }
                                            if (moving_index < 6) {
                                                worm[15].x--;
                                                Plot_Pixel_Fast(worm[15].x + 1, worm[15].y, 0);
                                                Plot_Pixel_Fast(worm[15].x, worm[15].y, worm[15].color);
                                            }
                                            if (moving_index < 5) {
                                                worm[16].x--;
                                                Plot_Pixel_Fast(worm[16].x + 1, worm[16].y, 0);
                                                Plot_Pixel_Fast(worm[16].x, worm[16].y, worm[16].color);
                                            }
                                            if (moving_index < 4) {
                                                worm[17].x--;
                                                Plot_Pixel_Fast(worm[17].x + 1, worm[17].y, 0);
                                                Plot_Pixel_Fast(worm[17].x, worm[17].y, worm[17].color);
                                            }
                                            if (moving_index < 3) {
                                                worm[18].x--;
                                                Plot_Pixel_Fast(worm[18].x + 1, worm[18].y, 0);
                                                Plot_Pixel_Fast(worm[18].x, worm[18].y, worm[19].color);
                                            }
                                            if (moving_index < 2) {
                                                worm[19].x--;
                                                Plot_Pixel_Fast(worm[19].x + 1, worm[19].y, 0);
                                                Plot_Pixel_Fast(worm[19].x, worm[19].y, worm[19].color);
                                            }
                                            Blit_String(70, 190, 245, "Hit Any Key To Continue...", 1);
                                            // draw the puased sprite bitmap
                                            blit_string3(5, 170, 51, 52, 54, "PAUSED", 1);
                                            Delay(1);
                                        }
                                        break;
                                    }
                                }
                                break;
                            }
                            case DOWN_LEFT: {
                                switch (old_dir) {
                                    case LEFT: {
                                        for (moving_index = 1; moving_index <= 20 && keys_active == 0; moving_index++) {
                                            if (moving_index < 21) {
                                                worm[0].y++;
                                                Plot_Pixel_Fast(worm[0].x, worm[0].y - 1, 0);
                                                Plot_Pixel_Fast(worm[0].x, worm[0].y, worm[0].color);
                                            }
                                            if (moving_index < 20) {
                                                worm[1].y++;
                                                Plot_Pixel_Fast(worm[1].x, worm[1].y - 1, 0);
                                                Plot_Pixel_Fast(worm[1].x, worm[1].y, worm[1].color);
                                            }
                                            if (moving_index < 19) {
                                                worm[2].y++;
                                                Plot_Pixel_Fast(worm[2].x, worm[2].y - 1, 0);
                                                Plot_Pixel_Fast(worm[2].x, worm[2].y, worm[2].color);
                                            }
                                            if (moving_index < 18) {
                                                worm[3].y++;
                                                Plot_Pixel_Fast(worm[3].x, worm[3].y - 1, 0);
                                                Plot_Pixel_Fast(worm[3].x, worm[3].y, worm[3].color);
                                            }
                                            if (moving_index < 17) {
                                                worm[4].y++;
                                                Plot_Pixel_Fast(worm[4].x, worm[4].y - 1, 0);
                                                Plot_Pixel_Fast(worm[4].x, worm[4].y, worm[4].color);
                                            }
                                            if (moving_index < 16) {
                                                worm[5].y++;
                                                Plot_Pixel_Fast(worm[5].x, worm[5].y - 1, 0);
                                                Plot_Pixel_Fast(worm[5].x, worm[5].y, worm[5].color);
                                            }
                                            if (moving_index < 15) {
                                                worm[6].y++;
                                                Plot_Pixel_Fast(worm[6].x, worm[6].y - 1, 0);
                                                Plot_Pixel_Fast(worm[6].x, worm[6].y, worm[6].color);
                                            }
                                            if (moving_index < 14) {
                                                worm[7].y++;
                                                Plot_Pixel_Fast(worm[7].x, worm[7].y - 1, 0);
                                                Plot_Pixel_Fast(worm[7].x, worm[7].y, worm[7].color);
                                            }
                                            if (moving_index < 13) {
                                                worm[8].y++;
                                                Plot_Pixel_Fast(worm[8].x, worm[8].y - 1, 0);
                                                Plot_Pixel_Fast(worm[8].x, worm[8].y, worm[8].color);
                                            }
                                            if (moving_index < 12) {
                                                worm[9].y++;
                                                Plot_Pixel_Fast(worm[9].x, worm[9].y - 1, 0);
                                                Plot_Pixel_Fast(worm[9].x, worm[9].y, worm[9].color);
                                            }
                                            if (moving_index < 11) {
                                                worm[10].y++;
                                                Plot_Pixel_Fast(worm[10].x, worm[10].y - 1, 0);
                                                Plot_Pixel_Fast(worm[10].x, worm[10].y, worm[10].color);
                                            }
                                            if (moving_index < 10) {
                                                worm[11].y++;
                                                Plot_Pixel_Fast(worm[11].x, worm[11].y - 1, 0);
                                                Plot_Pixel_Fast(worm[11].x, worm[11].y, worm[11].color);
                                            }
                                            if (moving_index < 9) {
                                                worm[12].y++;
                                                Plot_Pixel_Fast(worm[12].x, worm[12].y - 1, 0);
                                                Plot_Pixel_Fast(worm[12].x, worm[12].y, worm[12].color);
                                            }
                                            if (moving_index < 8) {
                                                worm[13].y++;
                                                Plot_Pixel_Fast(worm[13].x, worm[13].y - 1, 0);
                                                Plot_Pixel_Fast(worm[13].x, worm[13].y, worm[13].color);
                                            }
                                            if (moving_index < 7) {
                                                worm[14].y++;
                                                Plot_Pixel_Fast(worm[14].x, worm[14].y - 1, 0);
                                                Plot_Pixel_Fast(worm[14].x, worm[14].y, worm[14].color);
                                            }
                                            if (moving_index < 6) {
                                                worm[15].y++;
                                                Plot_Pixel_Fast(worm[15].x, worm[15].y - 1, 0);
                                                Plot_Pixel_Fast(worm[15].x, worm[15].y, worm[15].color);
                                            }
                                            if (moving_index < 5) {
                                                worm[16].y++;
                                                Plot_Pixel_Fast(worm[16].x, worm[16].y - 1, 0);
                                                Plot_Pixel_Fast(worm[16].x, worm[16].y, worm[16].color);
                                            }
                                            if (moving_index < 4) {
                                                worm[17].y++;
                                                Plot_Pixel_Fast(worm[17].x, worm[17].y - 1, 0);
                                                Plot_Pixel_Fast(worm[17].x, worm[17].y, worm[17].color);
                                            }
                                            if (moving_index < 3) {
                                                worm[18].y++;
                                                Plot_Pixel_Fast(worm[18].x, worm[18].y - 1, 0);
                                                Plot_Pixel_Fast(worm[18].x, worm[18].y, worm[19].color);
                                            }
                                            if (moving_index < 2) {
                                                worm[19].y++;
                                                Plot_Pixel_Fast(worm[19].x, worm[19].y - 1, 0);
                                                Plot_Pixel_Fast(worm[19].x, worm[19].y, worm[19].color);
                                            }
                                            Blit_String(70, 190, 245, "Hit Any Key To Continue...", 1);
                                            // draw the puased sprite bitmap
                                            blit_string3(5, 170, 51, 52, 54, "PAUSED", 1);
                                            Delay(1);
                                        }
                                        break;
                                    }
                                    case DOWN: {
                                        for (moving_index = 1; moving_index <= 20 && keys_active == 0; moving_index++) {
                                            if (moving_index < 21) {
                                                worm[0].x--;
                                                Plot_Pixel_Fast(worm[0].x + 1, worm[0].y, 0);
                                                Plot_Pixel_Fast(worm[0].x, worm[0].y, worm[0].color);
                                            }
                                            if (moving_index < 20) {
                                                worm[1].x--;
                                                Plot_Pixel_Fast(worm[1].x + 1, worm[1].y, 0);
                                                Plot_Pixel_Fast(worm[1].x, worm[1].y, worm[1].color);
                                            }
                                            if (moving_index < 19) {
                                                worm[2].x--;
                                                Plot_Pixel_Fast(worm[2].x + 1, worm[2].y, 0);
                                                Plot_Pixel_Fast(worm[2].x, worm[2].y, worm[2].color);
                                            }
                                            if (moving_index < 18) {
                                                worm[3].x--;
                                                Plot_Pixel_Fast(worm[3].x + 1, worm[3].y, 0);
                                                Plot_Pixel_Fast(worm[3].x, worm[3].y, worm[3].color);
                                            }
                                            if (moving_index < 17) {
                                                worm[4].x--;
                                                Plot_Pixel_Fast(worm[4].x + 1, worm[4].y, 0);
                                                Plot_Pixel_Fast(worm[4].x, worm[4].y, worm[4].color);
                                            }
                                            if (moving_index < 16) {
                                                worm[5].x--;
                                                Plot_Pixel_Fast(worm[5].x + 1, worm[5].y, 0);
                                                Plot_Pixel_Fast(worm[5].x, worm[5].y, worm[5].color);
                                            }
                                            if (moving_index < 15) {
                                                worm[6].x--;
                                                Plot_Pixel_Fast(worm[6].x + 1, worm[6].y, 0);
                                                Plot_Pixel_Fast(worm[6].x, worm[6].y, worm[6].color);
                                            }
                                            if (moving_index < 14) {
                                                worm[7].x--;
                                                Plot_Pixel_Fast(worm[7].x + 1, worm[7].y, 0);
                                                Plot_Pixel_Fast(worm[7].x, worm[7].y, worm[7].color);
                                            }
                                            if (moving_index < 13) {
                                                worm[8].x--;
                                                Plot_Pixel_Fast(worm[8].x + 1, worm[8].y, 0);
                                                Plot_Pixel_Fast(worm[8].x, worm[8].y, worm[8].color);
                                            }
                                            if (moving_index < 12) {
                                                worm[9].x--;
                                                Plot_Pixel_Fast(worm[9].x + 1, worm[9].y, 0);
                                                Plot_Pixel_Fast(worm[9].x, worm[9].y, worm[9].color);
                                            }
                                            if (moving_index < 11) {
                                                worm[10].x--;
                                                Plot_Pixel_Fast(worm[10].x + 1, worm[10].y, 0);
                                                Plot_Pixel_Fast(worm[10].x, worm[10].y, worm[10].color);
                                            }
                                            if (moving_index < 10) {
                                                worm[11].x--;
                                                Plot_Pixel_Fast(worm[11].x + 1, worm[11].y, 0);
                                                Plot_Pixel_Fast(worm[11].x, worm[11].y, worm[11].color);
                                            }
                                            if (moving_index < 9) {
                                                worm[12].x--;
                                                Plot_Pixel_Fast(worm[12].x + 1, worm[12].y, 0);
                                                Plot_Pixel_Fast(worm[12].x, worm[12].y, worm[12].color);
                                            }
                                            if (moving_index < 8) {
                                                worm[13].x--;
                                                Plot_Pixel_Fast(worm[13].x + 1, worm[13].y, 0);
                                                Plot_Pixel_Fast(worm[13].x, worm[13].y, worm[13].color);
                                            }
                                            if (moving_index < 7) {
                                                worm[14].x--;
                                                Plot_Pixel_Fast(worm[14].x + 1, worm[14].y, 0);
                                                Plot_Pixel_Fast(worm[14].x, worm[14].y, worm[14].color);
                                            }
                                            if (moving_index < 6) {
                                                worm[15].x--;
                                                Plot_Pixel_Fast(worm[15].x + 1, worm[15].y, 0);
                                                Plot_Pixel_Fast(worm[15].x, worm[15].y, worm[15].color);
                                            }
                                            if (moving_index < 5) {
                                                worm[16].x--;
                                                Plot_Pixel_Fast(worm[16].x + 1, worm[16].y, 0);
                                                Plot_Pixel_Fast(worm[16].x, worm[16].y, worm[16].color);
                                            }
                                            if (moving_index < 4) {
                                                worm[17].x--;
                                                Plot_Pixel_Fast(worm[17].x + 1, worm[17].y, 0);
                                                Plot_Pixel_Fast(worm[17].x, worm[17].y, worm[17].color);
                                            }
                                            if (moving_index < 3) {
                                                worm[18].x--;
                                                Plot_Pixel_Fast(worm[18].x + 1, worm[18].y, 0);
                                                Plot_Pixel_Fast(worm[18].x, worm[18].y, worm[19].color);
                                            }
                                            if (moving_index < 2) {
                                                worm[19].x--;
                                                Plot_Pixel_Fast(worm[19].x + 1, worm[19].y, 0);
                                                Plot_Pixel_Fast(worm[19].x, worm[19].y, worm[19].color);
                                            }
                                            Blit_String(70, 190, 245, "Hit Any Key To Continue...", 1);
                                            // draw the puased sprite bitmap
                                            blit_string3(5, 170, 51, 52, 54, "PAUSED", 1);
                                            Delay(1);
                                        }
                                        break;
                                    }
                                }
                                break;
                            }
                            case LEFT: {
                                switch (old_dir) {
                                    case LEFT_UP: {
                                        for (moving_index = 1; moving_index <= 20 && keys_active == 0; moving_index++) {
                                            if (moving_index < 21) {
                                                worm[0].y++;
                                                Plot_Pixel_Fast(worm[0].x, worm[0].y - 1, 0);
                                                Plot_Pixel_Fast(worm[0].x, worm[0].y, worm[0].color);
                                            }
                                            if (moving_index < 20) {
                                                worm[1].y++;
                                                Plot_Pixel_Fast(worm[1].x, worm[1].y - 1, 0);
                                                Plot_Pixel_Fast(worm[1].x, worm[1].y, worm[1].color);
                                            }
                                            if (moving_index < 19) {
                                                worm[2].y++;
                                                Plot_Pixel_Fast(worm[2].x, worm[2].y - 1, 0);
                                                Plot_Pixel_Fast(worm[2].x, worm[2].y, worm[2].color);
                                            }
                                            if (moving_index < 18) {
                                                worm[3].y++;
                                                Plot_Pixel_Fast(worm[3].x, worm[3].y - 1, 0);
                                                Plot_Pixel_Fast(worm[3].x, worm[3].y, worm[3].color);
                                            }
                                            if (moving_index < 17) {
                                                worm[4].y++;
                                                Plot_Pixel_Fast(worm[4].x, worm[4].y - 1, 0);
                                                Plot_Pixel_Fast(worm[4].x, worm[4].y, worm[4].color);
                                            }
                                            if (moving_index < 16) {
                                                worm[5].y++;
                                                Plot_Pixel_Fast(worm[5].x, worm[5].y - 1, 0);
                                                Plot_Pixel_Fast(worm[5].x, worm[5].y, worm[5].color);
                                            }
                                            if (moving_index < 15) {
                                                worm[6].y++;
                                                Plot_Pixel_Fast(worm[6].x, worm[6].y - 1, 0);
                                                Plot_Pixel_Fast(worm[6].x, worm[6].y, worm[6].color);
                                            }
                                            if (moving_index < 14) {
                                                worm[7].y++;
                                                Plot_Pixel_Fast(worm[7].x, worm[7].y - 1, 0);
                                                Plot_Pixel_Fast(worm[7].x, worm[7].y, worm[7].color);
                                            }
                                            if (moving_index < 13) {
                                                worm[8].y++;
                                                Plot_Pixel_Fast(worm[8].x, worm[8].y - 1, 0);
                                                Plot_Pixel_Fast(worm[8].x, worm[8].y, worm[8].color);
                                            }
                                            if (moving_index < 12) {
                                                worm[9].y++;
                                                Plot_Pixel_Fast(worm[9].x, worm[9].y - 1, 0);
                                                Plot_Pixel_Fast(worm[9].x, worm[9].y, worm[9].color);
                                            }
                                            if (moving_index < 11) {
                                                worm[10].y++;
                                                Plot_Pixel_Fast(worm[10].x, worm[10].y - 1, 0);
                                                Plot_Pixel_Fast(worm[10].x, worm[10].y, worm[10].color);
                                            }
                                            if (moving_index < 10) {
                                                worm[11].y++;
                                                Plot_Pixel_Fast(worm[11].x, worm[11].y - 1, 0);
                                                Plot_Pixel_Fast(worm[11].x, worm[11].y, worm[11].color);
                                            }
                                            if (moving_index < 9) {
                                                worm[12].y++;
                                                Plot_Pixel_Fast(worm[12].x, worm[12].y - 1, 0);
                                                Plot_Pixel_Fast(worm[12].x, worm[12].y, worm[12].color);
                                            }
                                            if (moving_index < 8) {
                                                worm[13].y++;
                                                Plot_Pixel_Fast(worm[13].x, worm[13].y - 1, 0);
                                                Plot_Pixel_Fast(worm[13].x, worm[13].y, worm[13].color);
                                            }
                                            if (moving_index < 7) {
                                                worm[14].y++;
                                                Plot_Pixel_Fast(worm[14].x, worm[14].y - 1, 0);
                                                Plot_Pixel_Fast(worm[14].x, worm[14].y, worm[14].color);
                                            }
                                            if (moving_index < 6) {
                                                worm[15].y++;
                                                Plot_Pixel_Fast(worm[15].x, worm[15].y - 1, 0);
                                                Plot_Pixel_Fast(worm[15].x, worm[15].y, worm[15].color);
                                            }
                                            if (moving_index < 5) {
                                                worm[16].y++;
                                                Plot_Pixel_Fast(worm[16].x, worm[16].y - 1, 0);
                                                Plot_Pixel_Fast(worm[16].x, worm[16].y, worm[16].color);
                                            }
                                            if (moving_index < 4) {
                                                worm[17].y++;
                                                Plot_Pixel_Fast(worm[17].x, worm[17].y - 1, 0);
                                                Plot_Pixel_Fast(worm[17].x, worm[17].y, worm[17].color);
                                            }
                                            if (moving_index < 3) {
                                                worm[18].y++;
                                                Plot_Pixel_Fast(worm[18].x, worm[18].y - 1, 0);
                                                Plot_Pixel_Fast(worm[18].x, worm[18].y, worm[19].color);
                                            }
                                            if (moving_index < 2) {
                                                worm[19].y++;
                                                Plot_Pixel_Fast(worm[19].x, worm[19].y - 1, 0);
                                                Plot_Pixel_Fast(worm[19].x, worm[19].y, worm[19].color);
                                            }
                                            Blit_String(70, 190, 245, "Hit Any Key To Continue...", 1);
                                            // draw the puased sprite bitmap
                                            blit_string3(5, 170, 51, 52, 54, "PAUSED", 1);
                                            Delay(1);
                                        }
                                        break;
                                    }
                                    case DOWN_LEFT: {
                                        for (moving_index = 1; moving_index <= 20 && keys_active == 0; moving_index++) {
                                            if (moving_index < 21) {
                                                worm[0].y--;
                                                Plot_Pixel_Fast(worm[0].x, worm[0].y + 1, 0);
                                                Plot_Pixel_Fast(worm[0].x, worm[0].y, worm[0].color);
                                            }
                                            if (moving_index < 20) {
                                                worm[1].y--;
                                                Plot_Pixel_Fast(worm[1].x, worm[1].y + 1, 0);
                                                Plot_Pixel_Fast(worm[1].x, worm[1].y, worm[1].color);
                                            }
                                            if (moving_index < 19) {
                                                worm[2].y--;
                                                Plot_Pixel_Fast(worm[2].x, worm[2].y + 1, 0);
                                                Plot_Pixel_Fast(worm[2].x, worm[2].y, worm[2].color);
                                            }
                                            if (moving_index < 18) {
                                                worm[3].y--;
                                                Plot_Pixel_Fast(worm[3].x, worm[3].y + 1, 0);
                                                Plot_Pixel_Fast(worm[3].x, worm[3].y, worm[3].color);
                                            }
                                            if (moving_index < 17) {
                                                worm[4].y--;
                                                Plot_Pixel_Fast(worm[4].x, worm[4].y + 1, 0);
                                                Plot_Pixel_Fast(worm[4].x, worm[4].y, worm[4].color);
                                            }
                                            if (moving_index < 16) {
                                                worm[5].y--;
                                                Plot_Pixel_Fast(worm[5].x, worm[5].y + 1, 0);
                                                Plot_Pixel_Fast(worm[5].x, worm[5].y, worm[5].color);
                                            }
                                            if (moving_index < 15) {
                                                worm[6].y--;
                                                Plot_Pixel_Fast(worm[6].x, worm[6].y + 1, 0);
                                                Plot_Pixel_Fast(worm[6].x, worm[6].y, worm[6].color);
                                            }
                                            if (moving_index < 14) {
                                                worm[7].y--;
                                                Plot_Pixel_Fast(worm[7].x, worm[7].y + 1, 0);
                                                Plot_Pixel_Fast(worm[7].x, worm[7].y, worm[7].color);
                                            }
                                            if (moving_index < 13) {
                                                worm[8].y--;
                                                Plot_Pixel_Fast(worm[8].x, worm[8].y + 1, 0);
                                                Plot_Pixel_Fast(worm[8].x, worm[8].y, worm[8].color);
                                            }
                                            if (moving_index < 12) {
                                                worm[9].y--;
                                                Plot_Pixel_Fast(worm[9].x, worm[9].y + 1, 0);
                                                Plot_Pixel_Fast(worm[9].x, worm[9].y, worm[9].color);
                                            }
                                            if (moving_index < 11) {
                                                worm[10].y--;
                                                Plot_Pixel_Fast(worm[10].x, worm[10].y + 1, 0);
                                                Plot_Pixel_Fast(worm[10].x, worm[10].y, worm[10].color);
                                            }
                                            if (moving_index < 10) {
                                                worm[11].y--;
                                                Plot_Pixel_Fast(worm[11].x, worm[11].y + 1, 0);
                                                Plot_Pixel_Fast(worm[11].x, worm[11].y, worm[11].color);
                                            }
                                            if (moving_index < 9) {
                                                worm[12].y--;
                                                Plot_Pixel_Fast(worm[12].x, worm[12].y + 1, 0);
                                                Plot_Pixel_Fast(worm[12].x, worm[12].y, worm[12].color);
                                            }
                                            if (moving_index < 8) {
                                                worm[13].y--;
                                                Plot_Pixel_Fast(worm[13].x, worm[13].y + 1, 0);
                                                Plot_Pixel_Fast(worm[13].x, worm[13].y, worm[13].color);
                                            }
                                            if (moving_index < 7) {
                                                worm[14].y--;
                                                Plot_Pixel_Fast(worm[14].x, worm[14].y + 1, 0);
                                                Plot_Pixel_Fast(worm[14].x, worm[14].y, worm[14].color);
                                            }
                                            if (moving_index < 6) {
                                                worm[15].y--;
                                                Plot_Pixel_Fast(worm[15].x, worm[15].y + 1, 0);
                                                Plot_Pixel_Fast(worm[15].x, worm[15].y, worm[15].color);
                                            }
                                            if (moving_index < 5) {
                                                worm[16].y--;
                                                Plot_Pixel_Fast(worm[16].x, worm[16].y + 1, 0);
                                                Plot_Pixel_Fast(worm[16].x, worm[16].y, worm[16].color);
                                            }
                                            if (moving_index < 4) {
                                                worm[17].y--;
                                                Plot_Pixel_Fast(worm[17].x, worm[17].y + 1, 0);
                                                Plot_Pixel_Fast(worm[17].x, worm[17].y, worm[17].color);
                                            }
                                            if (moving_index < 3) {
                                                worm[18].y--;
                                                Plot_Pixel_Fast(worm[18].x, worm[18].y + 1, 0);
                                                Plot_Pixel_Fast(worm[18].x, worm[18].y, worm[19].color);
                                            }
                                            if (moving_index < 2) {
                                                worm[19].y--;
                                                Plot_Pixel_Fast(worm[19].x, worm[19].y + 1, 0);
                                                Plot_Pixel_Fast(worm[19].x, worm[19].y, worm[19].color);
                                            }
                                            Blit_String(70, 190, 245, "Hit Any Key To Continue...", 1);
                                            // draw the puased sprite bitmap
                                            blit_string3(5, 170, 51, 52, 54, "PAUSED", 1);
                                            Delay(1);
                                        }
                                        break;
                                    }
                                }
                                break;
                            }
                            case LEFT_UP: {
                                switch (old_dir) {
                                    case UP: {
                                        for (moving_index = 1; moving_index <= 20 && keys_active == 0; moving_index++) {
                                            if (moving_index < 21) {
                                                worm[0].x--;
                                                Plot_Pixel_Fast(worm[0].x + 1, worm[0].y, 0);
                                                Plot_Pixel_Fast(worm[0].x, worm[0].y, worm[0].color);
                                            }
                                            if (moving_index < 20) {
                                                worm[1].x--;
                                                Plot_Pixel_Fast(worm[1].x + 1, worm[1].y, 0);
                                                Plot_Pixel_Fast(worm[1].x, worm[1].y, worm[1].color);
                                            }
                                            if (moving_index < 19) {
                                                worm[2].x--;
                                                Plot_Pixel_Fast(worm[2].x + 1, worm[2].y, 0);
                                                Plot_Pixel_Fast(worm[2].x, worm[2].y, worm[2].color);
                                            }
                                            if (moving_index < 18) {
                                                worm[3].x--;
                                                Plot_Pixel_Fast(worm[3].x + 1, worm[3].y, 0);
                                                Plot_Pixel_Fast(worm[3].x, worm[3].y, worm[3].color);
                                            }
                                            if (moving_index < 17) {
                                                worm[4].x--;
                                                Plot_Pixel_Fast(worm[4].x + 1, worm[4].y, 0);
                                                Plot_Pixel_Fast(worm[4].x, worm[4].y, worm[4].color);
                                            }
                                            if (moving_index < 16) {
                                                worm[5].x--;
                                                Plot_Pixel_Fast(worm[5].x + 1, worm[5].y, 0);
                                                Plot_Pixel_Fast(worm[5].x, worm[5].y, worm[5].color);
                                            }
                                            if (moving_index < 15) {
                                                worm[6].x--;
                                                Plot_Pixel_Fast(worm[6].x + 1, worm[6].y, 0);
                                                Plot_Pixel_Fast(worm[6].x, worm[6].y, worm[6].color);
                                            }
                                            if (moving_index < 14) {
                                                worm[7].x--;
                                                Plot_Pixel_Fast(worm[7].x + 1, worm[7].y, 0);
                                                Plot_Pixel_Fast(worm[7].x, worm[7].y, worm[7].color);
                                            }
                                            if (moving_index < 13) {
                                                worm[8].x--;
                                                Plot_Pixel_Fast(worm[8].x + 1, worm[8].y, 0);
                                                Plot_Pixel_Fast(worm[8].x, worm[8].y, worm[8].color);
                                            }
                                            if (moving_index < 12) {
                                                worm[9].x--;
                                                Plot_Pixel_Fast(worm[9].x + 1, worm[9].y, 0);
                                                Plot_Pixel_Fast(worm[9].x, worm[9].y, worm[9].color);
                                            }
                                            if (moving_index < 11) {
                                                worm[10].x--;
                                                Plot_Pixel_Fast(worm[10].x + 1, worm[10].y, 0);
                                                Plot_Pixel_Fast(worm[10].x, worm[10].y, worm[10].color);
                                            }
                                            if (moving_index < 10) {
                                                worm[11].x--;
                                                Plot_Pixel_Fast(worm[11].x + 1, worm[11].y, 0);
                                                Plot_Pixel_Fast(worm[11].x, worm[11].y, worm[11].color);
                                            }
                                            if (moving_index < 9) {
                                                worm[12].x--;
                                                Plot_Pixel_Fast(worm[12].x + 1, worm[12].y, 0);
                                                Plot_Pixel_Fast(worm[12].x, worm[12].y, worm[12].color);
                                            }
                                            if (moving_index < 8) {
                                                worm[13].x--;
                                                Plot_Pixel_Fast(worm[13].x + 1, worm[13].y, 0);
                                                Plot_Pixel_Fast(worm[13].x, worm[13].y, worm[13].color);
                                            }
                                            if (moving_index < 7) {
                                                worm[14].x--;
                                                Plot_Pixel_Fast(worm[14].x + 1, worm[14].y, 0);
                                                Plot_Pixel_Fast(worm[14].x, worm[14].y, worm[14].color);
                                            }
                                            if (moving_index < 6) {
                                                worm[15].x--;
                                                Plot_Pixel_Fast(worm[15].x + 1, worm[15].y, 0);
                                                Plot_Pixel_Fast(worm[15].x, worm[15].y, worm[15].color);
                                            }
                                            if (moving_index < 5) {
                                                worm[16].x--;
                                                Plot_Pixel_Fast(worm[16].x + 1, worm[16].y, 0);
                                                Plot_Pixel_Fast(worm[16].x, worm[16].y, worm[16].color);
                                            }
                                            if (moving_index < 4) {
                                                worm[17].x--;
                                                Plot_Pixel_Fast(worm[17].x + 1, worm[17].y, 0);
                                                Plot_Pixel_Fast(worm[17].x, worm[17].y, worm[17].color);
                                            }
                                            if (moving_index < 3) {
                                                worm[18].x--;
                                                Plot_Pixel_Fast(worm[18].x + 1, worm[18].y, 0);
                                                Plot_Pixel_Fast(worm[18].x, worm[18].y, worm[19].color);
                                            }
                                            if (moving_index < 2) {
                                                worm[19].x--;
                                                Plot_Pixel_Fast(worm[19].x + 1, worm[19].y, 0);
                                                Plot_Pixel_Fast(worm[19].x, worm[19].y, worm[19].color);
                                            }
                                            Blit_String(70, 190, 245, "Hit Any Key To Continue...", 1);
                                            // draw the puased sprite bitmap
                                            blit_string3(5, 170, 51, 52, 54, "PAUSED", 1);
                                            Delay(1);
                                        }
                                        break;
                                    }
                                    case LEFT: {
                                        for (moving_index = 1; moving_index <= 20 && keys_active == 0; moving_index++) {
                                            if (moving_index < 21) {
                                                worm[0].y--;
                                                Plot_Pixel_Fast(worm[0].x, worm[0].y + 1, 0);
                                                Plot_Pixel_Fast(worm[0].x, worm[0].y, worm[0].color);
                                            }
                                            if (moving_index < 20) {
                                                worm[1].y--;
                                                Plot_Pixel_Fast(worm[1].x, worm[1].y + 1, 0);
                                                Plot_Pixel_Fast(worm[1].x, worm[1].y, worm[1].color);
                                            }
                                            if (moving_index < 19) {
                                                worm[2].y--;
                                                Plot_Pixel_Fast(worm[2].x, worm[2].y + 1, 0);
                                                Plot_Pixel_Fast(worm[2].x, worm[2].y, worm[2].color);
                                            }
                                            if (moving_index < 18) {
                                                worm[3].y--;
                                                Plot_Pixel_Fast(worm[3].x, worm[3].y + 1, 0);
                                                Plot_Pixel_Fast(worm[3].x, worm[3].y, worm[3].color);
                                            }
                                            if (moving_index < 17) {
                                                worm[4].y--;
                                                Plot_Pixel_Fast(worm[4].x, worm[4].y + 1, 0);
                                                Plot_Pixel_Fast(worm[4].x, worm[4].y, worm[4].color);
                                            }
                                            if (moving_index < 16) {
                                                worm[5].y--;
                                                Plot_Pixel_Fast(worm[5].x, worm[5].y + 1, 0);
                                                Plot_Pixel_Fast(worm[5].x, worm[5].y, worm[5].color);
                                            }
                                            if (moving_index < 15) {
                                                worm[6].y--;
                                                Plot_Pixel_Fast(worm[6].x, worm[6].y + 1, 0);
                                                Plot_Pixel_Fast(worm[6].x, worm[6].y, worm[6].color);
                                            }
                                            if (moving_index < 14) {
                                                worm[7].y--;
                                                Plot_Pixel_Fast(worm[7].x, worm[7].y + 1, 0);
                                                Plot_Pixel_Fast(worm[7].x, worm[7].y, worm[7].color);
                                            }
                                            if (moving_index < 13) {
                                                worm[8].y--;
                                                Plot_Pixel_Fast(worm[8].x, worm[8].y + 1, 0);
                                                Plot_Pixel_Fast(worm[8].x, worm[8].y, worm[8].color);
                                            }
                                            if (moving_index < 12) {
                                                worm[9].y--;
                                                Plot_Pixel_Fast(worm[9].x, worm[9].y + 1, 0);
                                                Plot_Pixel_Fast(worm[9].x, worm[9].y, worm[9].color);
                                            }
                                            if (moving_index < 11) {
                                                worm[10].y--;
                                                Plot_Pixel_Fast(worm[10].x, worm[10].y + 1, 0);
                                                Plot_Pixel_Fast(worm[10].x, worm[10].y, worm[10].color);
                                            }
                                            if (moving_index < 10) {
                                                worm[11].y--;
                                                Plot_Pixel_Fast(worm[11].x, worm[11].y + 1, 0);
                                                Plot_Pixel_Fast(worm[11].x, worm[11].y, worm[11].color);
                                            }
                                            if (moving_index < 9) {
                                                worm[12].y--;
                                                Plot_Pixel_Fast(worm[12].x, worm[12].y + 1, 0);
                                                Plot_Pixel_Fast(worm[12].x, worm[12].y, worm[12].color);
                                            }
                                            if (moving_index < 8) {
                                                worm[13].y--;
                                                Plot_Pixel_Fast(worm[13].x, worm[13].y + 1, 0);
                                                Plot_Pixel_Fast(worm[13].x, worm[13].y, worm[13].color);
                                            }
                                            if (moving_index < 7) {
                                                worm[14].y--;
                                                Plot_Pixel_Fast(worm[14].x, worm[14].y + 1, 0);
                                                Plot_Pixel_Fast(worm[14].x, worm[14].y, worm[14].color);
                                            }
                                            if (moving_index < 6) {
                                                worm[15].y--;
                                                Plot_Pixel_Fast(worm[15].x, worm[15].y + 1, 0);
                                                Plot_Pixel_Fast(worm[15].x, worm[15].y, worm[15].color);
                                            }
                                            if (moving_index < 5) {
                                                worm[16].y--;
                                                Plot_Pixel_Fast(worm[16].x, worm[16].y + 1, 0);
                                                Plot_Pixel_Fast(worm[16].x, worm[16].y, worm[16].color);
                                            }
                                            if (moving_index < 4) {
                                                worm[17].y--;
                                                Plot_Pixel_Fast(worm[17].x, worm[17].y + 1, 0);
                                                Plot_Pixel_Fast(worm[17].x, worm[17].y, worm[17].color);
                                            }
                                            if (moving_index < 3) {
                                                worm[18].y--;
                                                Plot_Pixel_Fast(worm[18].x, worm[18].y + 1, 0);
                                                Plot_Pixel_Fast(worm[18].x, worm[18].y, worm[19].color);
                                            }
                                            if (moving_index < 2) {
                                                worm[19].y--;
                                                Plot_Pixel_Fast(worm[19].x, worm[19].y + 1, 0);
                                                Plot_Pixel_Fast(worm[19].x, worm[19].y, worm[19].color);
                                            }
                                            Blit_String(70, 190, 245, "Hit Any Key To Continue...", 1);
                                            // draw the puased sprite bitmap
                                            blit_string3(5, 170, 51, 52, 54, "PAUSED", 1);
                                            Delay(1);
                                        }
                                        break;
                                    }
                                }
                                break;
                            }
                            case UP: {
                                switch (old_dir) {
                                    case UP_RIGHT: {
                                        for (moving_index = 1; moving_index <= 20 && keys_active == 0; moving_index++) {
                                            if (moving_index < 21) {
                                                worm[0].x--;
                                                Plot_Pixel_Fast(worm[0].x + 1, worm[0].y, 0);
                                                Plot_Pixel_Fast(worm[0].x, worm[0].y, worm[0].color);
                                            }
                                            if (moving_index < 20) {
                                                worm[1].x--;
                                                Plot_Pixel_Fast(worm[1].x + 1, worm[1].y, 0);
                                                Plot_Pixel_Fast(worm[1].x, worm[1].y, worm[1].color);
                                            }
                                            if (moving_index < 19) {
                                                worm[2].x--;
                                                Plot_Pixel_Fast(worm[2].x + 1, worm[2].y, 0);
                                                Plot_Pixel_Fast(worm[2].x, worm[2].y, worm[2].color);
                                            }
                                            if (moving_index < 18) {
                                                worm[3].x--;
                                                Plot_Pixel_Fast(worm[3].x + 1, worm[3].y, 0);
                                                Plot_Pixel_Fast(worm[3].x, worm[3].y, worm[3].color);
                                            }
                                            if (moving_index < 17) {
                                                worm[4].x--;
                                                Plot_Pixel_Fast(worm[4].x + 1, worm[4].y, 0);
                                                Plot_Pixel_Fast(worm[4].x, worm[4].y, worm[4].color);
                                            }
                                            if (moving_index < 16) {
                                                worm[5].x--;
                                                Plot_Pixel_Fast(worm[5].x + 1, worm[5].y, 0);
                                                Plot_Pixel_Fast(worm[5].x, worm[5].y, worm[5].color);
                                            }
                                            if (moving_index < 15) {
                                                worm[6].x--;
                                                Plot_Pixel_Fast(worm[6].x + 1, worm[6].y, 0);
                                                Plot_Pixel_Fast(worm[6].x, worm[6].y, worm[6].color);
                                            }
                                            if (moving_index < 14) {
                                                worm[7].x--;
                                                Plot_Pixel_Fast(worm[7].x + 1, worm[7].y, 0);
                                                Plot_Pixel_Fast(worm[7].x, worm[7].y, worm[7].color);
                                            }
                                            if (moving_index < 13) {
                                                worm[8].x--;
                                                Plot_Pixel_Fast(worm[8].x + 1, worm[8].y, 0);
                                                Plot_Pixel_Fast(worm[8].x, worm[8].y, worm[8].color);
                                            }
                                            if (moving_index < 12) {
                                                worm[9].x--;
                                                Plot_Pixel_Fast(worm[9].x + 1, worm[9].y, 0);
                                                Plot_Pixel_Fast(worm[9].x, worm[9].y, worm[9].color);
                                            }
                                            if (moving_index < 11) {
                                                worm[10].x--;
                                                Plot_Pixel_Fast(worm[10].x + 1, worm[10].y, 0);
                                                Plot_Pixel_Fast(worm[10].x, worm[10].y, worm[10].color);
                                            }
                                            if (moving_index < 10) {
                                                worm[11].x--;
                                                Plot_Pixel_Fast(worm[11].x + 1, worm[11].y, 0);
                                                Plot_Pixel_Fast(worm[11].x, worm[11].y, worm[11].color);
                                            }
                                            if (moving_index < 9) {
                                                worm[12].x--;
                                                Plot_Pixel_Fast(worm[12].x + 1, worm[12].y, 0);
                                                Plot_Pixel_Fast(worm[12].x, worm[12].y, worm[12].color);
                                            }
                                            if (moving_index < 8) {
                                                worm[13].x--;
                                                Plot_Pixel_Fast(worm[13].x + 1, worm[13].y, 0);
                                                Plot_Pixel_Fast(worm[13].x, worm[13].y, worm[13].color);
                                            }
                                            if (moving_index < 7) {
                                                worm[14].x--;
                                                Plot_Pixel_Fast(worm[14].x + 1, worm[14].y, 0);
                                                Plot_Pixel_Fast(worm[14].x, worm[14].y, worm[14].color);
                                            }
                                            if (moving_index < 6) {
                                                worm[15].x--;
                                                Plot_Pixel_Fast(worm[15].x + 1, worm[15].y, 0);
                                                Plot_Pixel_Fast(worm[15].x, worm[15].y, worm[15].color);
                                            }
                                            if (moving_index < 5) {
                                                worm[16].x--;
                                                Plot_Pixel_Fast(worm[16].x + 1, worm[16].y, 0);
                                                Plot_Pixel_Fast(worm[16].x, worm[16].y, worm[16].color);
                                            }
                                            if (moving_index < 4) {
                                                worm[17].x--;
                                                Plot_Pixel_Fast(worm[17].x + 1, worm[17].y, 0);
                                                Plot_Pixel_Fast(worm[17].x, worm[17].y, worm[17].color);
                                            }
                                            if (moving_index < 3) {
                                                worm[18].x--;
                                                Plot_Pixel_Fast(worm[18].x + 1, worm[18].y, 0);
                                                Plot_Pixel_Fast(worm[18].x, worm[18].y, worm[19].color);
                                            }
                                            if (moving_index < 2) {
                                                worm[19].x--;
                                                Plot_Pixel_Fast(worm[19].x + 1, worm[19].y, 0);
                                                Plot_Pixel_Fast(worm[19].x, worm[19].y, worm[19].color);
                                            }
                                            Blit_String(70, 190, 245, "Hit Any Key To Continue...", 1);
                                            // draw the puased sprite bitmap
                                            blit_string3(5, 170, 51, 52, 54, "PAUSED", 1);
                                            Delay(1);
                                        }
                                        break;
                                    }
                                    case LEFT_UP: {
                                        for (moving_index = 1; moving_index <= 20 && keys_active == 0; moving_index++) {
                                            if (moving_index < 21) {
                                                worm[0].x++;
                                                Plot_Pixel_Fast(worm[0].x - 1, worm[0].y, 0);
                                                Plot_Pixel_Fast(worm[0].x, worm[0].y, worm[0].color);
                                            }
                                            if (moving_index < 20) {
                                                worm[1].x++;
                                                Plot_Pixel_Fast(worm[1].x - 1, worm[1].y, 0);
                                                Plot_Pixel_Fast(worm[1].x, worm[1].y, worm[1].color);
                                            }
                                            if (moving_index < 19) {
                                                worm[2].x++;
                                                Plot_Pixel_Fast(worm[2].x - 1, worm[2].y, 0);
                                                Plot_Pixel_Fast(worm[2].x, worm[2].y, worm[2].color);
                                            }
                                            if (moving_index < 18) {
                                                worm[3].x++;
                                                Plot_Pixel_Fast(worm[3].x - 1, worm[3].y, 0);
                                                Plot_Pixel_Fast(worm[3].x, worm[3].y, worm[3].color);
                                            }
                                            if (moving_index < 17) {
                                                worm[4].x++;
                                                Plot_Pixel_Fast(worm[4].x - 1, worm[4].y, 0);
                                                Plot_Pixel_Fast(worm[4].x, worm[4].y, worm[4].color);
                                            }
                                            if (moving_index < 16) {
                                                worm[5].x++;
                                                Plot_Pixel_Fast(worm[5].x - 1, worm[5].y, 0);
                                                Plot_Pixel_Fast(worm[5].x, worm[5].y, worm[5].color);
                                            }
                                            if (moving_index < 15) {
                                                worm[6].x++;
                                                Plot_Pixel_Fast(worm[6].x - 1, worm[6].y, 0);
                                                Plot_Pixel_Fast(worm[6].x, worm[6].y, worm[6].color);
                                            }
                                            if (moving_index < 14) {
                                                worm[7].x++;
                                                Plot_Pixel_Fast(worm[7].x - 1, worm[7].y, 0);
                                                Plot_Pixel_Fast(worm[7].x, worm[7].y, worm[7].color);
                                            }
                                            if (moving_index < 13) {
                                                worm[8].x++;
                                                Plot_Pixel_Fast(worm[8].x - 1, worm[8].y, 0);
                                                Plot_Pixel_Fast(worm[8].x, worm[8].y, worm[8].color);
                                            }
                                            if (moving_index < 12) {
                                                worm[9].x++;
                                                Plot_Pixel_Fast(worm[9].x - 1, worm[9].y, 0);
                                                Plot_Pixel_Fast(worm[9].x, worm[9].y, worm[9].color);
                                            }
                                            if (moving_index < 11) {
                                                worm[10].x++;
                                                Plot_Pixel_Fast(worm[10].x - 1, worm[10].y, 0);
                                                Plot_Pixel_Fast(worm[10].x, worm[10].y, worm[10].color);
                                            }
                                            if (moving_index < 10) {
                                                worm[11].x++;
                                                Plot_Pixel_Fast(worm[11].x - 1, worm[11].y, 0);
                                                Plot_Pixel_Fast(worm[11].x, worm[11].y, worm[11].color);
                                            }
                                            if (moving_index < 9) {
                                                worm[12].x++;
                                                Plot_Pixel_Fast(worm[12].x - 1, worm[12].y, 0);
                                                Plot_Pixel_Fast(worm[12].x, worm[12].y, worm[12].color);
                                            }
                                            if (moving_index < 8) {
                                                worm[13].x++;
                                                Plot_Pixel_Fast(worm[13].x - 1, worm[13].y, 0);
                                                Plot_Pixel_Fast(worm[13].x, worm[13].y, worm[13].color);
                                            }
                                            if (moving_index < 7) {
                                                worm[14].x++;
                                                Plot_Pixel_Fast(worm[14].x - 1, worm[14].y, 0);
                                                Plot_Pixel_Fast(worm[14].x, worm[14].y, worm[14].color);
                                            }
                                            if (moving_index < 6) {
                                                worm[15].x++;
                                                Plot_Pixel_Fast(worm[15].x - 1, worm[15].y, 0);
                                                Plot_Pixel_Fast(worm[15].x, worm[15].y, worm[15].color);
                                            }
                                            if (moving_index < 5) {
                                                worm[16].x++;
                                                Plot_Pixel_Fast(worm[16].x - 1, worm[16].y, 0);
                                                Plot_Pixel_Fast(worm[16].x, worm[16].y, worm[16].color);
                                            }
                                            if (moving_index < 4) {
                                                worm[17].x++;
                                                Plot_Pixel_Fast(worm[17].x - 1, worm[17].y, 0);
                                                Plot_Pixel_Fast(worm[17].x, worm[17].y, worm[17].color);
                                            }
                                            if (moving_index < 3) {
                                                worm[18].x++;
                                                Plot_Pixel_Fast(worm[18].x - 1, worm[18].y, 0);
                                                Plot_Pixel_Fast(worm[18].x, worm[18].y, worm[19].color);
                                            }
                                            if (moving_index < 2) {
                                                worm[19].x++;
                                                Plot_Pixel_Fast(worm[19].x - 1, worm[19].y, 0);
                                                Plot_Pixel_Fast(worm[19].x, worm[19].y, worm[19].color);
                                            }
                                            Blit_String(70, 190, 245, "Hit Any Key To Continue...", 1);
                                            // draw the puased sprite bitmap
                                            blit_string3(5, 170, 51, 52, 54, "PAUSED", 1);
                                            Delay(1);
                                        }
                                        break;
                                    }
                                }
                                break;
                            }
                            case UP_RIGHT: {
                                switch (old_dir) {
                                    case RIGHT: {
                                        for (moving_index = 1; moving_index <= 20 && keys_active == 0; moving_index++) {
                                            if (moving_index < 21) {
                                                worm[0].y--;
                                                Plot_Pixel_Fast(worm[0].x, worm[0].y + 1, 0);
                                                Plot_Pixel_Fast(worm[0].x, worm[0].y, worm[0].color);
                                            }
                                            if (moving_index < 20) {
                                                worm[1].y--;
                                                Plot_Pixel_Fast(worm[1].x, worm[1].y + 1, 0);
                                                Plot_Pixel_Fast(worm[1].x, worm[1].y, worm[1].color);
                                            }
                                            if (moving_index < 19) {
                                                worm[2].y--;
                                                Plot_Pixel_Fast(worm[2].x, worm[2].y + 1, 0);
                                                Plot_Pixel_Fast(worm[2].x, worm[2].y, worm[2].color);
                                            }
                                            if (moving_index < 18) {
                                                worm[3].y--;
                                                Plot_Pixel_Fast(worm[3].x, worm[3].y + 1, 0);
                                                Plot_Pixel_Fast(worm[3].x, worm[3].y, worm[3].color);
                                            }
                                            if (moving_index < 17) {
                                                worm[4].y--;
                                                Plot_Pixel_Fast(worm[4].x, worm[4].y + 1, 0);
                                                Plot_Pixel_Fast(worm[4].x, worm[4].y, worm[4].color);
                                            }
                                            if (moving_index < 16) {
                                                worm[5].y--;
                                                Plot_Pixel_Fast(worm[5].x, worm[5].y + 1, 0);
                                                Plot_Pixel_Fast(worm[5].x, worm[5].y, worm[5].color);
                                            }
                                            if (moving_index < 15) {
                                                worm[6].y--;
                                                Plot_Pixel_Fast(worm[6].x, worm[6].y + 1, 0);
                                                Plot_Pixel_Fast(worm[6].x, worm[6].y, worm[6].color);
                                            }
                                            if (moving_index < 14) {
                                                worm[7].y--;
                                                Plot_Pixel_Fast(worm[7].x, worm[7].y + 1, 0);
                                                Plot_Pixel_Fast(worm[7].x, worm[7].y, worm[7].color);
                                            }
                                            if (moving_index < 13) {
                                                worm[8].y--;
                                                Plot_Pixel_Fast(worm[8].x, worm[8].y + 1, 0);
                                                Plot_Pixel_Fast(worm[8].x, worm[8].y, worm[8].color);
                                            }
                                            if (moving_index < 12) {
                                                worm[9].y--;
                                                Plot_Pixel_Fast(worm[9].x, worm[9].y + 1, 0);
                                                Plot_Pixel_Fast(worm[9].x, worm[9].y, worm[9].color);
                                            }
                                            if (moving_index < 11) {
                                                worm[10].y--;
                                                Plot_Pixel_Fast(worm[10].x, worm[10].y + 1, 0);
                                                Plot_Pixel_Fast(worm[10].x, worm[10].y, worm[10].color);
                                            }
                                            if (moving_index < 10) {
                                                worm[11].y--;
                                                Plot_Pixel_Fast(worm[11].x, worm[11].y + 1, 0);
                                                Plot_Pixel_Fast(worm[11].x, worm[11].y, worm[11].color);
                                            }
                                            if (moving_index < 9) {
                                                worm[12].y--;
                                                Plot_Pixel_Fast(worm[12].x, worm[12].y + 1, 0);
                                                Plot_Pixel_Fast(worm[12].x, worm[12].y, worm[12].color);
                                            }
                                            if (moving_index < 8) {
                                                worm[13].y--;
                                                Plot_Pixel_Fast(worm[13].x, worm[13].y + 1, 0);
                                                Plot_Pixel_Fast(worm[13].x, worm[13].y, worm[13].color);
                                            }
                                            if (moving_index < 7) {
                                                worm[14].y--;
                                                Plot_Pixel_Fast(worm[14].x, worm[14].y + 1, 0);
                                                Plot_Pixel_Fast(worm[14].x, worm[14].y, worm[14].color);
                                            }
                                            if (moving_index < 6) {
                                                worm[15].y--;
                                                Plot_Pixel_Fast(worm[15].x, worm[15].y + 1, 0);
                                                Plot_Pixel_Fast(worm[15].x, worm[15].y, worm[15].color);
                                            }
                                            if (moving_index < 5) {
                                                worm[16].y--;
                                                Plot_Pixel_Fast(worm[16].x, worm[16].y + 1, 0);
                                                Plot_Pixel_Fast(worm[16].x, worm[16].y, worm[16].color);
                                            }
                                            if (moving_index < 4) {
                                                worm[17].y--;
                                                Plot_Pixel_Fast(worm[17].x, worm[17].y + 1, 0);
                                                Plot_Pixel_Fast(worm[17].x, worm[17].y, worm[17].color);
                                            }
                                            if (moving_index < 3) {
                                                worm[18].y--;
                                                Plot_Pixel_Fast(worm[18].x, worm[18].y + 1, 0);
                                                Plot_Pixel_Fast(worm[18].x, worm[18].y, worm[19].color);
                                            }
                                            if (moving_index < 2) {
                                                worm[19].y--;
                                                Plot_Pixel_Fast(worm[19].x, worm[19].y + 1, 0);
                                                Plot_Pixel_Fast(worm[19].x, worm[19].y, worm[19].color);
                                            }
                                            Blit_String(70, 190, 245, "Hit Any Key To Continue...", 1);
                                            // draw the puased sprite bitmap
                                            blit_string3(5, 170, 51, 52, 54, "PAUSED", 1);
                                            Delay(1);
                                        }
                                        break;
                                    }
                                    case UP: {
                                        for (moving_index = 1; moving_index <= 20 && keys_active == 0; moving_index++) {
                                            if (moving_index < 21) {
                                                worm[0].x++;
                                                Plot_Pixel_Fast(worm[0].x - 1, worm[0].y, 0);
                                                Plot_Pixel_Fast(worm[0].x, worm[0].y, worm[0].color);
                                            }
                                            if (moving_index < 20) {
                                                worm[1].x++;
                                                Plot_Pixel_Fast(worm[1].x - 1, worm[1].y, 0);
                                                Plot_Pixel_Fast(worm[1].x, worm[1].y, worm[1].color);
                                            }
                                            if (moving_index < 19) {
                                                worm[2].x++;
                                                Plot_Pixel_Fast(worm[2].x - 1, worm[2].y, 0);
                                                Plot_Pixel_Fast(worm[2].x, worm[2].y, worm[2].color);
                                            }
                                            if (moving_index < 18) {
                                                worm[3].x++;
                                                Plot_Pixel_Fast(worm[3].x - 1, worm[3].y, 0);
                                                Plot_Pixel_Fast(worm[3].x, worm[3].y, worm[3].color);
                                            }
                                            if (moving_index < 17) {
                                                worm[4].x++;
                                                Plot_Pixel_Fast(worm[4].x - 1, worm[4].y, 0);
                                                Plot_Pixel_Fast(worm[4].x, worm[4].y, worm[4].color);
                                            }
                                            if (moving_index < 16) {
                                                worm[5].x++;
                                                Plot_Pixel_Fast(worm[5].x - 1, worm[5].y, 0);
                                                Plot_Pixel_Fast(worm[5].x, worm[5].y, worm[5].color);
                                            }
                                            if (moving_index < 15) {
                                                worm[6].x++;
                                                Plot_Pixel_Fast(worm[6].x - 1, worm[6].y, 0);
                                                Plot_Pixel_Fast(worm[6].x, worm[6].y, worm[6].color);
                                            }
                                            if (moving_index < 14) {
                                                worm[7].x++;
                                                Plot_Pixel_Fast(worm[7].x - 1, worm[7].y, 0);
                                                Plot_Pixel_Fast(worm[7].x, worm[7].y, worm[7].color);
                                            }
                                            if (moving_index < 13) {
                                                worm[8].x++;
                                                Plot_Pixel_Fast(worm[8].x - 1, worm[8].y, 0);
                                                Plot_Pixel_Fast(worm[8].x, worm[8].y, worm[8].color);
                                            }
                                            if (moving_index < 12) {
                                                worm[9].x++;
                                                Plot_Pixel_Fast(worm[9].x - 1, worm[9].y, 0);
                                                Plot_Pixel_Fast(worm[9].x, worm[9].y, worm[9].color);
                                            }
                                            if (moving_index < 11) {
                                                worm[10].x++;
                                                Plot_Pixel_Fast(worm[10].x - 1, worm[10].y, 0);
                                                Plot_Pixel_Fast(worm[10].x, worm[10].y, worm[10].color);
                                            }
                                            if (moving_index < 10) {
                                                worm[11].x++;
                                                Plot_Pixel_Fast(worm[11].x - 1, worm[11].y, 0);
                                                Plot_Pixel_Fast(worm[11].x, worm[11].y, worm[11].color);
                                            }
                                            if (moving_index < 9) {
                                                worm[12].x++;
                                                Plot_Pixel_Fast(worm[12].x - 1, worm[12].y, 0);
                                                Plot_Pixel_Fast(worm[12].x, worm[12].y, worm[12].color);
                                            }
                                            if (moving_index < 8) {
                                                worm[13].x++;
                                                Plot_Pixel_Fast(worm[13].x - 1, worm[13].y, 0);
                                                Plot_Pixel_Fast(worm[13].x, worm[13].y, worm[13].color);
                                            }
                                            if (moving_index < 7) {
                                                worm[14].x++;
                                                Plot_Pixel_Fast(worm[14].x - 1, worm[14].y, 0);
                                                Plot_Pixel_Fast(worm[14].x, worm[14].y, worm[14].color);
                                            }
                                            if (moving_index < 6) {
                                                worm[15].x++;
                                                Plot_Pixel_Fast(worm[15].x - 1, worm[15].y, 0);
                                                Plot_Pixel_Fast(worm[15].x, worm[15].y, worm[15].color);
                                            }
                                            if (moving_index < 5) {
                                                worm[16].x++;
                                                Plot_Pixel_Fast(worm[16].x - 1, worm[16].y, 0);
                                                Plot_Pixel_Fast(worm[16].x, worm[16].y, worm[16].color);
                                            }
                                            if (moving_index < 4) {
                                                worm[17].x++;
                                                Plot_Pixel_Fast(worm[17].x - 1, worm[17].y, 0);
                                                Plot_Pixel_Fast(worm[17].x, worm[17].y, worm[17].color);
                                            }
                                            if (moving_index < 3) {
                                                worm[18].x++;
                                                Plot_Pixel_Fast(worm[18].x - 1, worm[18].y, 0);
                                                Plot_Pixel_Fast(worm[18].x, worm[18].y, worm[19].color);
                                            }
                                            if (moving_index < 2) {
                                                worm[19].x++;
                                                Plot_Pixel_Fast(worm[19].x - 1, worm[19].y, 0);
                                                Plot_Pixel_Fast(worm[19].x, worm[19].y, worm[19].color);
                                            }
                                            Blit_String(70, 190, 245, "Hit Any Key To Continue...", 1);
                                            // draw the puased sprite bitmap
                                            blit_string3(5, 170, 51, 52, 54, "PAUSED", 1);
                                            Delay(1);
                                        }
                                        break;
                                    }
                                }
                                break;
                            }
                            case RIGHT: {
                                switch (old_dir) {
                                    case DOWN_RIGHT: {
                                        for (moving_index = 1; moving_index <= 20 && keys_active == 0; moving_index++) {
                                            if (moving_index < 21) {
                                                worm[0].y--;
                                                Plot_Pixel_Fast(worm[0].x, worm[0].y + 1, 0);
                                                Plot_Pixel_Fast(worm[0].x, worm[0].y, worm[0].color);
                                            }
                                            if (moving_index < 20) {
                                                worm[1].y--;
                                                Plot_Pixel_Fast(worm[1].x, worm[1].y + 1, 0);
                                                Plot_Pixel_Fast(worm[1].x, worm[1].y, worm[1].color);
                                            }
                                            if (moving_index < 19) {
                                                worm[2].y--;
                                                Plot_Pixel_Fast(worm[2].x, worm[2].y + 1, 0);
                                                Plot_Pixel_Fast(worm[2].x, worm[2].y, worm[2].color);
                                            }
                                            if (moving_index < 18) {
                                                worm[3].y--;
                                                Plot_Pixel_Fast(worm[3].x, worm[3].y + 1, 0);
                                                Plot_Pixel_Fast(worm[3].x, worm[3].y, worm[3].color);
                                            }
                                            if (moving_index < 17) {
                                                worm[4].y--;
                                                Plot_Pixel_Fast(worm[4].x, worm[4].y + 1, 0);
                                                Plot_Pixel_Fast(worm[4].x, worm[4].y, worm[4].color);
                                            }
                                            if (moving_index < 16) {
                                                worm[5].y--;
                                                Plot_Pixel_Fast(worm[5].x, worm[5].y + 1, 0);
                                                Plot_Pixel_Fast(worm[5].x, worm[5].y, worm[5].color);
                                            }
                                            if (moving_index < 15) {
                                                worm[6].y--;
                                                Plot_Pixel_Fast(worm[6].x, worm[6].y + 1, 0);
                                                Plot_Pixel_Fast(worm[6].x, worm[6].y, worm[6].color);
                                            }
                                            if (moving_index < 14) {
                                                worm[7].y--;
                                                Plot_Pixel_Fast(worm[7].x, worm[7].y + 1, 0);
                                                Plot_Pixel_Fast(worm[7].x, worm[7].y, worm[7].color);
                                            }
                                            if (moving_index < 13) {
                                                worm[8].y--;
                                                Plot_Pixel_Fast(worm[8].x, worm[8].y + 1, 0);
                                                Plot_Pixel_Fast(worm[8].x, worm[8].y, worm[8].color);
                                            }
                                            if (moving_index < 12) {
                                                worm[9].y--;
                                                Plot_Pixel_Fast(worm[9].x, worm[9].y + 1, 0);
                                                Plot_Pixel_Fast(worm[9].x, worm[9].y, worm[9].color);
                                            }
                                            if (moving_index < 11) {
                                                worm[10].y--;
                                                Plot_Pixel_Fast(worm[10].x, worm[10].y + 1, 0);
                                                Plot_Pixel_Fast(worm[10].x, worm[10].y, worm[10].color);
                                            }
                                            if (moving_index < 10) {
                                                worm[11].y--;
                                                Plot_Pixel_Fast(worm[11].x, worm[11].y + 1, 0);
                                                Plot_Pixel_Fast(worm[11].x, worm[11].y, worm[11].color);
                                            }
                                            if (moving_index < 9) {
                                                worm[12].y--;
                                                Plot_Pixel_Fast(worm[12].x, worm[12].y + 1, 0);
                                                Plot_Pixel_Fast(worm[12].x, worm[12].y, worm[12].color);
                                            }
                                            if (moving_index < 8) {
                                                worm[13].y--;
                                                Plot_Pixel_Fast(worm[13].x, worm[13].y + 1, 0);
                                                Plot_Pixel_Fast(worm[13].x, worm[13].y, worm[13].color);
                                            }
                                            if (moving_index < 7) {
                                                worm[14].y--;
                                                Plot_Pixel_Fast(worm[14].x, worm[14].y + 1, 0);
                                                Plot_Pixel_Fast(worm[14].x, worm[14].y, worm[14].color);
                                            }
                                            if (moving_index < 6) {
                                                worm[15].y--;
                                                Plot_Pixel_Fast(worm[15].x, worm[15].y + 1, 0);
                                                Plot_Pixel_Fast(worm[15].x, worm[15].y, worm[15].color);
                                            }
                                            if (moving_index < 5) {
                                                worm[16].y--;
                                                Plot_Pixel_Fast(worm[16].x, worm[16].y + 1, 0);
                                                Plot_Pixel_Fast(worm[16].x, worm[16].y, worm[16].color);
                                            }
                                            if (moving_index < 4) {
                                                worm[17].y--;
                                                Plot_Pixel_Fast(worm[17].x, worm[17].y + 1, 0);
                                                Plot_Pixel_Fast(worm[17].x, worm[17].y, worm[17].color);
                                            }
                                            if (moving_index < 3) {
                                                worm[18].y--;
                                                Plot_Pixel_Fast(worm[18].x, worm[18].y + 1, 0);
                                                Plot_Pixel_Fast(worm[18].x, worm[18].y, worm[19].color);
                                            }
                                            if (moving_index < 2) {
                                                worm[19].y--;
                                                Plot_Pixel_Fast(worm[19].x, worm[19].y + 1, 0);
                                                Plot_Pixel_Fast(worm[19].x, worm[19].y, worm[19].color);
                                            }
                                            Blit_String(70, 190, 245, "Hit Any Key To Continue...", 1);
                                            // draw the puased sprite bitmap
                                            blit_string3(5, 170, 51, 52, 54, "PAUSED", 1);
                                            Delay(1);
                                        }
                                        break;
                                    }
                                    case UP_RIGHT: {
                                        for (moving_index = 1; moving_index <= 20 && keys_active == 0; moving_index++) {
                                            if (moving_index < 21) {
                                                worm[0].y++;
                                                Plot_Pixel_Fast(worm[0].x, worm[0].y - 1, 0);
                                                Plot_Pixel_Fast(worm[0].x, worm[0].y, worm[0].color);
                                            }
                                            if (moving_index < 20) {
                                                worm[1].y++;
                                                Plot_Pixel_Fast(worm[1].x, worm[1].y - 1, 0);
                                                Plot_Pixel_Fast(worm[1].x, worm[1].y, worm[1].color);
                                            }
                                            if (moving_index < 19) {
                                                worm[2].y++;
                                                Plot_Pixel_Fast(worm[2].x, worm[2].y - 1, 0);
                                                Plot_Pixel_Fast(worm[2].x, worm[2].y, worm[2].color);
                                            }
                                            if (moving_index < 18) {
                                                worm[3].y++;
                                                Plot_Pixel_Fast(worm[3].x, worm[3].y - 1, 0);
                                                Plot_Pixel_Fast(worm[3].x, worm[3].y, worm[3].color);
                                            }
                                            if (moving_index < 17) {
                                                worm[4].y++;
                                                Plot_Pixel_Fast(worm[4].x, worm[4].y - 1, 0);
                                                Plot_Pixel_Fast(worm[4].x, worm[4].y, worm[4].color);
                                            }
                                            if (moving_index < 16) {
                                                worm[5].y++;
                                                Plot_Pixel_Fast(worm[5].x, worm[5].y - 1, 0);
                                                Plot_Pixel_Fast(worm[5].x, worm[5].y, worm[5].color);
                                            }
                                            if (moving_index < 15) {
                                                worm[6].y++;
                                                Plot_Pixel_Fast(worm[6].x, worm[6].y - 1, 0);
                                                Plot_Pixel_Fast(worm[6].x, worm[6].y, worm[6].color);
                                            }
                                            if (moving_index < 14) {
                                                worm[7].y++;
                                                Plot_Pixel_Fast(worm[7].x, worm[7].y - 1, 0);
                                                Plot_Pixel_Fast(worm[7].x, worm[7].y, worm[7].color);
                                            }
                                            if (moving_index < 13) {
                                                worm[8].y++;
                                                Plot_Pixel_Fast(worm[8].x, worm[8].y - 1, 0);
                                                Plot_Pixel_Fast(worm[8].x, worm[8].y, worm[8].color);
                                            }
                                            if (moving_index < 12) {
                                                worm[9].y++;
                                                Plot_Pixel_Fast(worm[9].x, worm[9].y - 1, 0);
                                                Plot_Pixel_Fast(worm[9].x, worm[9].y, worm[9].color);
                                            }
                                            if (moving_index < 11) {
                                                worm[10].y++;
                                                Plot_Pixel_Fast(worm[10].x, worm[10].y - 1, 0);
                                                Plot_Pixel_Fast(worm[10].x, worm[10].y, worm[10].color);
                                            }
                                            if (moving_index < 10) {
                                                worm[11].y++;
                                                Plot_Pixel_Fast(worm[11].x, worm[11].y - 1, 0);
                                                Plot_Pixel_Fast(worm[11].x, worm[11].y, worm[11].color);
                                            }
                                            if (moving_index < 9) {
                                                worm[12].y++;
                                                Plot_Pixel_Fast(worm[12].x, worm[12].y - 1, 0);
                                                Plot_Pixel_Fast(worm[12].x, worm[12].y, worm[12].color);
                                            }
                                            if (moving_index < 8) {
                                                worm[13].y++;
                                                Plot_Pixel_Fast(worm[13].x, worm[13].y - 1, 0);
                                                Plot_Pixel_Fast(worm[13].x, worm[13].y, worm[13].color);
                                            }
                                            if (moving_index < 7) {
                                                worm[14].y++;
                                                Plot_Pixel_Fast(worm[14].x, worm[14].y - 1, 0);
                                                Plot_Pixel_Fast(worm[14].x, worm[14].y, worm[14].color);
                                            }
                                            if (moving_index < 6) {
                                                worm[15].y++;
                                                Plot_Pixel_Fast(worm[15].x, worm[15].y - 1, 0);
                                                Plot_Pixel_Fast(worm[15].x, worm[15].y, worm[15].color);
                                            }
                                            if (moving_index < 5) {
                                                worm[16].y++;
                                                Plot_Pixel_Fast(worm[16].x, worm[16].y - 1, 0);
                                                Plot_Pixel_Fast(worm[16].x, worm[16].y, worm[16].color);
                                            }
                                            if (moving_index < 4) {
                                                worm[17].y++;
                                                Plot_Pixel_Fast(worm[17].x, worm[17].y - 1, 0);
                                                Plot_Pixel_Fast(worm[17].x, worm[17].y, worm[17].color);
                                            }
                                            if (moving_index < 3) {
                                                worm[18].y++;
                                                Plot_Pixel_Fast(worm[18].x, worm[18].y - 1, 0);
                                                Plot_Pixel_Fast(worm[18].x, worm[18].y, worm[19].color);
                                            }
                                            if (moving_index < 2) {
                                                worm[19].y++;
                                                Plot_Pixel_Fast(worm[19].x, worm[19].y - 1, 0);
                                                Plot_Pixel_Fast(worm[19].x, worm[19].y, worm[19].color);
                                            }
                                            Blit_String(70, 190, 245, "Hit Any Key To Continue...", 1);
                                            // draw the puased sprite bitmap
                                            blit_string3(5, 170, 51, 52, 54, "PAUSED", 1);
                                            Delay(1);
                                        }
                                        break;
                                    }
                                }
                                break;
                            }
                        }
                    }
                    if (keys_active == 0) {
                        if (direction == DOWN_RIGHT) {
                            for (index = 0; index < 20; index++) {
                                worm[index].x += 1;
                                worm[index].y += 1;
                                if (worm[index].x > 320) {
                                    worm[index].x -= 320;
                                }
                                if (worm[index].y > 200) {
                                    worm[index].y -= 200;
                                }
                                Plot_Pixel_Fast_DB(worm[index].x, worm[index].y, worm[index].color);
                            }
                        } else if (direction == DOWN) {
                            for (index = 0; index < 20; index++) {
                                worm[index].y += 1;
                                if (worm[index].x > 320) {
                                    worm[index].x -= 320;
                                }
                                if (worm[index].y > 200) {
                                    worm[index].y -= 200;
                                }
                                Plot_Pixel_Fast_DB(worm[index].x, worm[index].y, worm[index].color);
                            }
                        } else if (direction == DOWN_LEFT) {
                            for (index = 0; index < 20; index++) {
                                worm[index].x -= 1;
                                worm[index].y += 1;
                                if (worm[index].x > 320) {
                                    worm[index].x -= 320;
                                }
                                if (worm[index].y > 200) {
                                    worm[index].y -= 200;
                                }
                                Plot_Pixel_Fast_DB(worm[index].x, worm[index].y, worm[index].color);
                            }
                        } else if (direction == LEFT) {
                            for (index = 0; index < 20; index++) {
                                worm[index].x -= 1;
                                if (worm[index].x > 320) {
                                    worm[index].x -= 320;
                                }
                                if (worm[index].y > 200) {
                                    worm[index].y -= 200;
                                }
                                Plot_Pixel_Fast_DB(worm[index].x, worm[index].y, worm[index].color);
                            }
                        } else if (direction == LEFT_UP) {
                            for (index = 0; index < 20; index++) {
                                worm[index].x -= 1;
                                worm[index].y -= 1;
                                if (worm[index].x > 320) {
                                    worm[index].x -= 320;
                                }
                                if (worm[index].y > 200) {
                                    worm[index].y -= 200;
                                }
                                Plot_Pixel_Fast_DB(worm[index].x, worm[index].y, worm[index].color);
                            }
                        } else if (direction == UP) {
                            for (index = 0; index < 20; index++) {
                                worm[index].y -= 1;
                                if (worm[index].x > 320) {
                                    worm[index].x -= 320;
                                }
                                if (worm[index].y > 200) {
                                    worm[index].y -= 200;
                                }
                                Plot_Pixel_Fast_DB(worm[index].x, worm[index].y, worm[index].color);
                            }
                        } else if (direction == UP_RIGHT) {
                            for (index = 0; index < 20; index++) {
                                worm[index].x += 1;
                                worm[index].y -= 1;
                                if (worm[index].x > 320) {
                                    worm[index].x -= 320;
                                }
                                if (worm[index].y > 200) {
                                    worm[index].y -= 200;
                                }
                                Plot_Pixel_Fast_DB(worm[index].x, worm[index].y, worm[index].color);
                            }
                        } else if (direction == RIGHT) {
                            for (index = 0; index < 20; index++) {
                                worm[index].x += 1;
                                if (worm[index].x > 320) {
                                    worm[index].x -= 320;
                                }
                                if (worm[index].y > 200) {
                                    worm[index].y -= 200;
                                }
                                Plot_Pixel_Fast_DB(worm[index].x, worm[index].y, worm[index].color);
                            }
                        }
                    }
                    if (rand() % 30 == 1 && keys_active == 0) {
                        direction_decision = rand() % 2;
                        old_dir = direction;
                        set_direction = 1;
                        if (direction_decision == 0) {
                            if (++direction > 7) {
                                direction = DOWN_RIGHT;
                            }
                        } else if (direction_decision == 1) {
                            if (--direction < 0) {
                                direction = RIGHT;
                            }
                        } else {
                            if (--direction < 0) {
                                direction = RIGHT;
                            }
                        }
                    }
                    Wait_For_Vsync();
                    Show_Double_Buffer(double_buffer);
                    Blit_String(70, 190, 245, "Hit Any Key To Continue...", 1);
                    // draw the puased sprite bitmap
                    blit_string3(5, 170, 51, 52, 54, "PAUSED", 1);
                }
                done1 = 1;
            }
            cur_saver = NO_SAVER;
            done1 = 0;
        } else if (cur_saver == 6) {
            while (!done1) {
                int intersect_x = 160, intersect_y = 100;
                int index = 0, index1 = 0, x, y, x1, y1, x2, y2, x3, y3, done2 = 0, counter = 0, counter1 = 0, color;
                
                color = rand() % 256;
                Fill_Screen();
                
                while (!done2) {
                    x = rand() % (intersect_x + 1);
                    y = rand() % (intersect_y + 1);
                    Plot_Pixel_Fast(x, y, color);
                    
                    x1 = ((intersect_x - x) + intersect_x);
                    y1 = y;
                    Plot_Pixel_Fast(x1, y1, color);
                    
                    x2 = x1;
                    y2 = ((intersect_y - y) + intersect_y);
                    Plot_Pixel_Fast(x2, y2, color);
                    
                    x3 = x;
                    y3 = y2;
                    Plot_Pixel_Fast(x3, y3, color);
                    
                    if (++counter == 30) {
                        if (++color > 254) {
                            color = 1;
                        }
                        counter = 0;
                    }
                    if (++counter1 > 150) {
                        Fill_Screen();
                        counter1 = 0;
                    }
                    if (keys_active > 0) {
                        done2 = 1;
                        done1 = 1;
                    }
                    Delay(1);
                }
            }
            cur_saver = NO_SAVER;
            done1 = 0;
        }
        if (keys_active > 0) {
            if (keyboard_state[MAKE_1]) {
                Delay(3);
                cur_saver = SAVER_FOLLOW;
            } else if (keyboard_state[MAKE_2]) {
                Delay(3);
                cur_saver = SAVER_LINES;
            } else if (keyboard_state[MAKE_3]) {
                Delay(3);
                cur_saver = SAVER_POLY;
            } else if (keyboard_state[MAKE_4]) {
                Delay(3);
                cur_saver = SAVER_DOTS;
            } else if (keyboard_state[MAKE_5]) {
                Delay(3);
                cur_saver = SAVER_WORM;
            } else if (keyboard_state[MAKE_6]) {
                Delay(3);
                cur_saver = 6;
            } else {
                done = 1;
                redraw_screen_db();
            }
        }
    } // end while not done
    return 0;
} // end screen_saver();

// this is the function to redraw the screen to the double buffer after the user did the
// worm screen saver (it changes the double bufer)
void redraw_screen_db()
{
    pcx_picture background_pcx;
    
    PCX_Init((pcx_picture_ptr)&background_pcx);
    PCX_Load("main.pcx", (pcx_picture_ptr)&background_pcx, 1);
    PCX_Copy_To_Buffer((pcx_picture_ptr)&background_pcx, double_buffer);
    PCX_Delete((pcx_picture_ptr)&background_pcx);
    // print out the level difficulty
    // print out medium for difficulty
    if (dif_cur_setting == DIF_NORMAL) {
        Blit_String_DB(15, 188, 245, "Medium", 1);
    } else
        // print out easy for difficulty
        if (dif_cur_setting == DIF_EASY) {
            Blit_String_DB(15, 188, 245, "Easy", 1);
        } else
            // print out hard for difficulty
            if (dif_cur_setting == DIF_HARD) {
                Blit_String_DB(15, 188, 245, "Hard", 1);
            }
    // print out level 1 for difficulty
    if (level_setting == LEVEL1) {
        Blit_String_DB(15, 180, 245, "Level 1", 1);
    } else
        // print out level 2 for difficulty
        if (level_setting == LEVEL2) {
            Blit_String_DB(15, 180, 245, "Level 2", 1);
        } else
            // print out level 3 for difficulty
            if (level_setting == LEVEL3) {
                Blit_String_DB(15, 180, 245, "Level 3", 1);
            } else
                // print out level 4 for difficulty
                if (level_setting == LEVEL4) {
                    Blit_String_DB(15, 180, 245, "Level 4", 1);
                } else
                    // print out level 5 for difficulty
                    if (level_setting == LEVEL5) {
                        Blit_String_DB(15, 180, 245, "Level 5", 1);
                    } else
                        // print out level 6 for difficulty
                        if (level_setting == LEVEL6) {
                            Blit_String_DB(15, 180, 245, "Level 6", 1);
                        } else
                            // print out level 7 for difficulty
                            if (level_setting == LEVEL7) {
                                Blit_String_DB(15, 180, 245, "Level 7", 1);
                            } else
                                // print out level 8 for difficulty
                                if (level_setting == LEVEL8) {
                                    Blit_String_DB(15, 180, 245, "Level 8", 1);
                                } else
                                    // print out level 9 for difficulty
                                    if (level_setting == LEVEL9) {
                                        Blit_String_DB(15, 180, 245, "Level 9", 1);
                                    } else
                                        // print out level 10 for difficulty
                                        if (level_setting == LEVEL10) {
                                            Blit_String_DB(15, 180, 245, "Level 10", 1);
                                        }
    printproblem(cur_problem);
    sprintf(numbers, "%d", first1);
    if (first1 < 10) {
        Blit_String_DB(31, 13, 55, numbers, 0);
    }
    if (first1 >= 10) {
        Blit_String_DB(23, 13, 55, numbers, 0);
    }
    // print out the plus sign
    Blit_Char_DB(13, 21, '+', 55, 0);
    // printf the second generated number into the character array "numbers so it can be used
    // by our blit_string() function
    sprintf(numbers, "%d", second1);
    // these next if's do a little organization of the
    // numbers displayed on the screen so they
    // line up in correct mathematical order
    if (second1 < 10) {
        Blit_String_DB(31, 21, 55, numbers, 0);
    }
    if (second1 >= 10) {
        Blit_String_DB(23, 21, 55, numbers, 0);
    }
    // draw the line to complete the form of the problem
    Bline_DB(13, 30, 42, 30, 55);
}

void Tech_Print(int x, int y, char *string, unsigned char far *destination)
{
// this function is used to print text out like a teletypwriter,it looks
// cool, trust me!

    int length,  // length of input string
        index;   // looping variable
        
    char buffer[3];  // a little string used to call font engine with
    
// compute length of input string

    length = strlen(string);
    
// print the string out a character at a time

    for (index = 0; index < length; index++) {
        // the first character is the actual printable character
        
        buffer[0] = string[index];
        
        // this is a little cursor kind of thing
        
        buffer[1] = '<';
        
        // null terminate
        
        buffer[2] = 0;
        
        // print the string
        
        Font_Engine_1(x, y, 0, 0, buffer, destination);
        
        // move to next position
        
        x += (TECH_FONT_WIDTH + 1);
        
        // wait a bit  1/70th of a second
        
        Wait_For_Vsync();
        
        // clear the cursor
        
    } // end for
    
    // clear the cursor
    
    buffer[0] = ' ';
    buffer[1] = ' ';
    buffer[2] = 0;
    
    Font_Engine_1(x, y, 0, 0, buffer, destination);
    
// done!

} // end Tech_Print

//////////////////////////////////////////////////////////////////////////////

void Font_Engine_1(int x, int y,
                   int font, int color,
                   char *string, unsigned char far *destination)
{
// this function prints a string out using one of the graphics fonts that
// we have drawn, note this first version doesn't use the font field, but
// we'll throw it in to keep the interface open for a future version
    pcx_picture image_pcx;
    static int font_loaded = 0; // this is used to track the first time the
    // function is loaded
    
    int index,    // loop index
        c_index,  // character index
        length;   // used to compute lengths of strings
        
// test if this is the first time this function is called, if so load the
// font

    if (!font_loaded) {
    
        // load the 4x7 tech font
        
        PCX_Init((pcx_picture_ptr)&image_pcx);
        PCX_Load("font.pcx", (pcx_picture_ptr)&image_pcx, 1);
        
        // allocate memory for each bitmap and load character
        
        for (index = 0; index < NUM_TECH_FONT; index++) {
            // allocate memory for charcter
            
            Bitmap_Allocate((bitmap_ptr)&tech_font[index],
                            TECH_FONT_WIDTH, TECH_FONT_HEIGHT);
                            
            // set size of character
            
            tech_font[index].width = TECH_FONT_WIDTH;
            tech_font[index].height = TECH_FONT_HEIGHT;
            
            // extract bitmap from PCX buffer
            
            tech_font[index].x = 1 + (index % 16) * (TECH_FONT_WIDTH + 1);
            tech_font[index].y = 1 + (index / 16) * (TECH_FONT_HEIGHT + 1);
            
            Bitmap_Get((bitmap_ptr)&tech_font[index],
                       (unsigned char far *)image_pcx.buffer);
                       
        } // end for index
        // font is loaded, delete pcx file and set flag
        
        PCX_Delete((pcx_picture_ptr)&image_pcx);
        
        font_loaded = 1;
        
    } // end if first time
    else {
        // print the sent string
        
        // pre-compute length of string
        
        length = strlen(string);
        
        // print the string character by character
        
        for (index = 0; index < length; index++) {
            // extract the character index from the space character
            
            c_index = string[index] - ' ';
            
            // set bitmap position
            
            tech_font[c_index].y = y;
            tech_font[c_index].x = x;
            
            // display bitmap
            
            Bitmap_Put((bitmap_ptr)&tech_font[c_index],
                       (unsigned char far *)destination, 0);
                       
            // move to next character position
            
            x += (TECH_FONT_WIDTH + 1);
            
        } // end for index
        
    } // end else print string
    
}

void Init_Starburst(void)
{
    pcx_picture image_pcx;
    int index;
    
    PCX_Init((pcx_picture_ptr)&image_pcx);
    PCX_Load("miscpcx.pcx", (pcx_picture_ptr)&image_pcx, 1);
    Sprite_Init((sprite_ptr)&starburst, 0, 0, 22, 18, 0, 0, 0, 0, 0, 0);
    for (index = 0; index < 6; index++) {
        PCX_Get_Sprite((pcx_picture_ptr)&image_pcx, (sprite_ptr)&starburst, index, index, 0);
    }
    PCX_Delete((pcx_picture_ptr)&image_pcx);
}

void Do_Starburst(void)
{
    static int new_starburst = 1;
    static int ok_to_go = 0;
    
// select a random number of startb bursts
    if (game_state == GAME_MENU) {
        if ((new_starburst == 1) && (rand() % 10 == 0)) {
            starburst.x = 50 + (rand() % 210);
            starburst.y = 20 + (rand() % 7);
            starburst.curr_frame = 0;
            new_starburst = 0;
            ok_to_go = 1;
            Sprite_Under((sprite_ptr)&starburst, video_buffer);
        }
        
        if (ok_to_go) {
            Sprite_Erase((sprite_ptr)&starburst, video_buffer);
            Sprite_Under((sprite_ptr)&starburst, video_buffer);
            Wait_For_Vsync();
            Sprite_Draw((sprite_ptr)&starburst, video_buffer, 1);
            
            
            starburst.curr_frame++;
            if (starburst.curr_frame > 5) {
                new_starburst = 1;
                Sprite_Erase((sprite_ptr)&starburst, video_buffer);
            }
        }
    }
} // end Do_Starburst

int Parse_Commands(int argc, char **argv)
{
// this function is used to parse the commands line parameters that are to be
// used as switched to enable different modes of operation

    int index;  // looping variable
    
    for (index = 1; index < argc; index++) {
        // get the first character from the string
        
        switch (argv[index][0]) {
        
            case 's': // enable sound effects
            case 'S': {
                sound_enabled = 1;
            }
            break;
            
            case 'm': // enable nusic
            case 'M': {
                music_enabled = 1;
            }
            break;
            
            case 'f':
            case 'F': {
                fast_machine = 1;
            }
            break;
            
            case 'w':
            case 'W': {
                in_windows = 1;
            }
            break;
            // more commands would go here...
            
            default:
                break;
                
        } // end switch
        
    } // end for index
    return 0;
    
} // end Parse_Commands

void Screen_From_Black_PCX(void)
{
    int index_1, index;
    RGB_palette screen_palette;
    RGB_color cur_color;
    
    Read_Palette(0, 255, (RGB_palette_ptr)&screen_palette);
    
    for (index = 1; index < 255; index++) {
        cur_color.red = 0;
        cur_color.blue = 0;
        cur_color.green = 0;
        Set_Palette_Register(index, (RGB_color_ptr)&cur_color);
    }
    PCX_Show_Buffer((pcx_picture_ptr)&image);
    
    for (index_1 = 0; index_1 < 14; index_1++) {
        for (index = 1; index < 255; index++) {
            Get_Palette_Register(index, (RGB_color_ptr)&cur_color);
            if ((cur_color.red) <= (screen_palette.colors[index].red)) {
                if ((cur_color.red += pal_look[screen_palette.colors[index].red]) > screen_palette.colors[index].red) {
                    cur_color.red -= pal_look[screen_palette.colors[index].red];
                }
            }
            if ((cur_color.green) <= (screen_palette.colors[index].green)) {
                if ((cur_color.green += pal_look[screen_palette.colors[index].green]) > screen_palette.colors[index].green) {
                    cur_color.green -= pal_look[screen_palette.colors[index].green];
                }
            }
            if ((cur_color.blue) <= (screen_palette.colors[index].blue)) {
                if ((cur_color.blue += pal_look[screen_palette.colors[index].blue]) > screen_palette.colors[index].blue) {
                    cur_color.blue -= pal_look[screen_palette.colors[index].blue];
                }
            }
            Set_Palette_Register(index, (RGB_color_ptr)&cur_color);
        }
        Delay(1);
    }
    
    Write_Palette(0, 255, (RGB_palette_ptr)&screen_palette);
}

void Moving_Text_Y(char *string, int x, int y)
{
    int index;
    char eraser[51] = "                                        ";
    
    for (index = 192; y < index; index -= 2) {
        Font_Engine_1(x, index, 0, 0, string, double_buffer);
        Delay(1);
        Wait_For_Vsync();
        Show_Double_Buffer(double_buffer);
        Font_Engine_1(x, index, 0, 0, eraser, double_buffer);
    }
    Font_Engine_1(x, index + 2, 0, 0, string, double_buffer);
}

void Init_Power_Line(void)
{
    pcx_picture data_pcx;
    int index;
    
    PCX_Init((pcx_picture_ptr)&data_pcx);
    PCX_Load("icons.pcx", (pcx_picture_ptr)&data_pcx, 1);
    
    // intialize the button sprite
    
    Sprite_Init((sprite_ptr)&power_line, 280, 7, 34, 6, 0, 0, 0, 0, 0, 0);
    
    // extract the bitmaps for heads up gauges
    
    for (index = 0; index < 23; index++) {
        PCX_Get_Sprite((pcx_picture_ptr)&data_pcx, (sprite_ptr)&power_line, index, index % 9, index / 9);
    }
    
    // delete pcx file
    
    PCX_Delete((pcx_picture_ptr)&data_pcx);
}

void Delete_Power_Line(void)
{
    Sprite_Delete((sprite_ptr)&power_line);
}

void Draw_Power_Line(void)
{
    power_line.curr_frame = 22 - (cur_player.power / 1000);
    
    if (!game_action_state) {
        Sprite_Under((sprite_ptr)&power_line, double_buffer);
    }
    Sprite_Draw((sprite_ptr)&power_line, double_buffer, 1);
}

void Erase_Power_Line(void)
{
    Sprite_Erase((sprite_ptr)&power_line, double_buffer);
}

void Start_PDeath(void)
{

// this function begins the death of a player

    int index;
    
// make sure the player is dying otherwise return

    if (player_ship.state != PLAYER_DYING) {
        return;
    }
    
// loop thru all particles and initialize them to different upward velocities

    for (index = 0; index < NUM_DEATH_PARTICLES; index++) {
    
        pdeath[index].x           =  9 + player_ship.x - 4 + rand() % 9;
        pdeath[index].y           =  9 + player_ship.y - 4 + rand() % 9;
        pdeath[index].xv          =  -5 + rand() % 11;
        pdeath[index].yv          =  -5 - (rand() % 7);
        pdeath[index].color       =  96;
        pdeath[index].back        =  0;
        pdeath[index].state       =  0;
        
        pdeath[index].counter     =  0;
        pdeath[index].threshold   =  5;
        pdeath[index].lifetime = 40;
        
        pdeath[index].counter_2   =  0;
        pdeath[index].threshold_2 =  5;
        
        
    } // end for index
    
} // end Start_PDeath

void Erase_PDeath(void)
{
// this function is used to erase all the particles in the players death

    int index;
    
// make sure the player is dying otherwise return

    if (player_ship.state != PLAYER_DYING) {
        return;
    }
    
// loop thru all particles and erase them i.e. replace the background

    for (index = 0; index < NUM_DEATH_PARTICLES; index++) {
    
        Plot_Pixel_Fast_DB(pdeath[index].x, pdeath[index].y, pdeath[index].back);
        
    } // end for index
    
} // end Erase_PDeath

///////////////////////////////////////////////////////////////////////////////

void Animate_PDeath(void)
{

// this is the workhorse of the death animation sequence, it moves the particles
// applies gravity to them and changes their colors, also it bounds them to the
// screen

    int index;
    
// should we be doing this?

    if (player_ship.state != PLAYER_DYING) {
        return;
    }
    
// process each particle

    for (index = 0; index < NUM_DEATH_PARTICLES; index++) {
    
        // translation
        
        pdeath[index].x += pdeath[index].xv;
        pdeath[index].y += pdeath[index].yv;
        
        // boundary tests
        
        // xtests
        if (!game_action_state) {
            if (pdeath[index].x > 315) {
                pdeath[index].x = 315;
            } else if (pdeath[index].x < 104) {
                pdeath[index].x = 104;
            }
            
            // ytests
            
            if (pdeath[index].y > 197) {
                pdeath[index].y     = 197;
                pdeath[index].xv    = 0;
                pdeath[index].color = 0;
            } else if (pdeath[index].y < 13) {
                pdeath[index].y = 13;
            }
            
            // gravity
            
            if (++pdeath[index].counter == pdeath[index].threshold) {
                // apply gravity field
                
                pdeath[index].yv++;
                
                // reset counter
                
                pdeath[index].counter = 0;
                
            } // end if time to apply gravity
            
            // color
            
            if (pdeath[index].y < 197)
                if (++pdeath[index].counter_2 == pdeath[index].threshold_2) {
                    // change color
                    
                    pdeath[index].color++;
                    
                    // reset counter
                    
                    pdeath[index].counter_2 = 0;
                    
                } // end if time to change color
        } else {
            if (pdeath[index].x > 320) {
                pdeath[index].x = 320;
            } else if (pdeath[index].x < 0) {
                pdeath[index].x = 0;
            }
            
            // ytests
            
            if (pdeath[index].y > 200) {
                pdeath[index].y     = 200;
                pdeath[index].xv    = 0;
                pdeath[index].color = 0;
            } else if (pdeath[index].y < 0) {
                pdeath[index].y = 0;
            }
            
            // gravity
            
            if (++pdeath[index].counter == pdeath[index].threshold) {
                // apply gravity field
                
                pdeath[index].yv++;
                
                // reset counter
                
                pdeath[index].counter = 0;
                
            } // end if time to apply gravity
            
            // color
            
            if (pdeath[index].y < 200)
                if (++pdeath[index].counter_2 == pdeath[index].threshold_2) {
                    // change color
                    
                    pdeath[index].color++;
                    
                    // reset counter
                    
                    pdeath[index].counter_2 = 0;
                    
                } // end if time to change color
        }
        // end of sequence
        if (--pdeath[index].lifetime == 0) {
            player_ship.state = PLAYER_ALIVE;
        }
    } // end for
} // end Animate_PDeath

///////////////////////////////////////////////////////////////////////////////

void Behind_PDeath(void)
{

    int index;
    
// check if we should do this

    if (player_ship.state != PLAYER_DYING) {
        return;
    }
    
// loop thru all particles and scan their backgrounds

    for (index = 0; index < NUM_DEATH_PARTICLES; index++) {
    
        pdeath[index].back = Get_Pixel_DB(pdeath[index].x, pdeath[index].y);
        
    } // end for index
    
} // end Behind_PDeath

///////////////////////////////////////////////////////////////////////////////

void Draw_PDeath(void)
{

    int index;
    
// check if we should do this

    if (player_ship.state != PLAYER_DYING) {
        return;
    }
    
// loop thru all particles and draw them

    for (index = 0; index < NUM_DEATH_PARTICLES; index++) {
    
        Plot_Pixel_Fast_DB(pdeath[index].x, pdeath[index].y, pdeath[index].color);
        
    } // end for index
    
}

unsigned char Get_Pixel_DB(int x, int y)
{

// gets the color value of pixel at (x,y) from the double buffer

    return double_buffer[((y << 8) + (y << 6)) + x];
    
}

int FX_Play(int the_effect, int priority)
{
// this function is used to play a digital effect using a pre-emptive priority
// scheme. The algorithm works like this: if a sound is playing then its
// priority is compared to the sound that is being requested to be played
// if the new sound has higher priority (a smaller number) then the currenlty
// playing sound is pre-empted for the new sound and the global FX priority
// is set to the new sound. If there is no sound playing then the new sound
// is simple played and the global priority is set

// is the digital fx system on-line?
    if (!sound_enabled) {
        return 0;
    }
    
// is there a sound playing?

    if (!Sound_Status() || (priority <= FX_priority)) {
        // start new sound
        
        Sound_Stop();
        
        Sound_Play((sound_ptr)&game_snd[the_effect]);
        
        // set the priority
        
        FX_priority = priority;
        
        return(1);
        
    } // end if
    else { // the current sound is of higher priority
        return(0);
    }
    
}

void Load_Game_Sounds(void)
{
    if (sound_enabled == SOUND_ON) {
        Sound_Load("correct.voc", (sound_ptr)&game_snd[CORRECT_SND], 1);
        Sound_Load("ncorrect.voc", (sound_ptr)&game_snd[INCORRECT_SND], 1);
        Sound_Load("crash1.voc", (sound_ptr)&game_snd[CRASH1_SND], 1);
        Sound_Load("crash2.voc", (sound_ptr)&game_snd[CRASH2_SND], 1);
        Sound_Load("laser4.voc", (sound_ptr)&game_snd[LASER_SND], 1);
        Sound_Load("power.voc", (sound_ptr)&game_snd[POWER_SND], 1);
        Sound_Load("attack.voc", (sound_ptr)&game_snd[ATTACK_SND], 1);
    }
}

void Delete_Game_Sounds(void)
{
    int index;
    
    if (sound_enabled) {
        for (index = 0; index < NUM_OF_GAME_SOUNDS; index++) {
            Sound_Unload((sound_ptr)&game_snd[index]);
        }
    }
}

void Start_Pattern(void)
{

// this function is used to start a wave off

    int x, y, element;
    num_aliens = 0;
    
    current_pattern = patterns[pattern_number];
    
// loop and create mechs

    for (x = 0; x < PATTERN_X_SIZE; x++) {
    
        for (y = 0; y < PATTERN_Y_SIZE; y++) {
        
            // extract element out of database
            
            element = current_pattern[PATTERN_X_SIZE * y + x];
            
            // test if this is a live mech
            
            if (element != 0) {
            
                // set fixed fields
                alien_array[num_aliens].x            = x * 32 + PATTERN_XO;
                alien_array[num_aliens].y            = y * 32 + PATTERN_YO;
                if (element == 1) {
                    alien_array[num_aliens].xv           = 3;
                    alien_array[num_aliens].yv           = 3;
                }
                if (element == 2) {
                    alien_array[num_aliens].xv          = 5;
                    alien_array[num_aliens].yv          = 5;
                }
                alien_array[num_aliens].state_1      = SPRITE_ALIVE;
                alien_array[num_aliens].state_2      = ALIEN_FLY;
                alien_array[num_aliens].new_state = 0;
                alien_array[num_aliens].counter_1    = 0;
                alien_array[num_aliens].counter_2    = 0;
                alien_array[num_aliens].aux_1        = 0;
                alien_array[num_aliens].aux_2        = 0;
                alien_array[num_aliens].threshold_1  = 0;
                alien_array[num_aliens].threshold_2  = 64;
                alien_array[num_aliens].direction    = 0;
                alien_array[num_aliens].curr_frame   = 0;
                
                // set type field
                
                alien_array[num_aliens].type         = element;
                
                // there is one more mech now
                
                num_aliens++;
            }
        } // end for y
        
    } // end for x
    game_clock = 0;
} // end Start_Wave

void Init_Patterns(void)
{
    static int already_done = 0;
    
    if (!already_done) {
        patterns[0] = pattern_0;
        patterns[1] = pattern_1;
        patterns[2] = pattern_2;
        patterns[3] = pattern_3;
        patterns[4] = pattern_4;
        patterns[5] = pattern_5;
        patterns[6] = pattern_6;
        patterns[7] = pattern_7;
        patterns[8] = pattern_8;
        patterns[9] = pattern_9;
        already_done = 1;
    }
}

void Move_Aliens(void)
{
    int move_x, move_y, index, the_frame, curr_direction;
    alien_ptr worker;
    
    for (index = 0; index < MAX_NUMBER_OF_ALIEN_SHIPS; index++) {
        worker = (alien_ptr)&alien_array[index];
        // test if alien is alive
        if (worker->state_1 == SPRITE_ALIVE) {
            switch (worker->state_2) {
                case ALIEN_FLY: {
                    move_y = worker->yv;
                    
                    if ((worker->y += move_y) > 196) {
                        worker->y = -32;
                    }
                    the_frame = rand() % 2;
                    worker->curr_frame = the_frame;
                    if ((game_clock > attack_time) && (rand() % 100 == 1)) {
                        worker->state_2    = ALIEN_ATTACK;
                        worker->new_state  = 1;
                    }
                }
                break;
                case ALIEN_ATTACK: {
                    if (worker->new_state) {
                        // reset new state
                        
                        worker->new_state = 0;
                        
                        // select a pattern and reset all vars
                        
                        worker->aux_1 = rand() % NUM_ATTACK_PATTERNS;
                        
                        // use counter 1 as index into pattern table
                        
                        worker->counter_1 = 0;
                        worker->counter_2   = 0;
                        worker->threshold_2 = 2 + rand() % 3;
                    } // end if need to initialize state
                    
                    // else must be continuing state
                    
                    // access current direction
                    
                    curr_direction = attack_patterns[worker->aux_1][worker->counter_1];
                    
                    // test if we are at end of sequence
                    
                    if (curr_direction == -1) {
                        worker->state_2    = ALIEN_FLY;
                        worker->new_state  = 1;
                        break;
                    } // end if at end
                    
                    the_frame = rand() % 2;
                    // extract current frame of animation
                    switch (curr_direction) {
                        case D0: {
                            if (the_frame) {
                                worker->curr_frame = 0;
                            } else {
                                worker->curr_frame = 1;
                            }
                        }
                        break;
                        case L15: {
                            if (the_frame) {
                                worker->curr_frame = 2;
                            } else {
                                worker->curr_frame = 3;
                            }
                        }
                        break;
                        case L30: {
                            if (the_frame) {
                                worker->curr_frame = 4;
                            } else {
                                worker->curr_frame = 5;
                            }
                        }
                        break;
                        case R30: {
                            if (the_frame) {
                                worker->curr_frame = 8;
                            } else {
                                worker->curr_frame = 9;
                            }
                        }
                        break;
                        case R15: {
                            if (the_frame) {
                                worker->curr_frame = 6;
                            } else {
                                worker->curr_frame = 7;
                            }
                        }
                        break;
                        default:
                            break;
                    }
                    
                    // using current direction, compute velocity vector
                    move_x = dirs_x[curr_direction];
                    if (curr_direction == -1) {
                        move_x = 0;
                    }
                    move_y = worker->yv;
                    
                    worker->x += move_x;
                    worker->y += move_y;
                    // test if we went too far
                    
                    if (worker->x > 315 ) {
                        worker->x = (105 - alien_1.width);
                    } else if (worker->x < (105 - alien_1.width)) {
                        worker->x = (315 + alien_1.width);
                    }
                    
                    if (worker->y > 196) {
                        worker->y = -32;
                    }
                    
                    if (++worker->counter_2 == worker->threshold_2) {
                        worker->counter_2 = 0;
                        worker->counter_1++;
                    } // end if time to switch pattern element
                }
                break;
                default:
                    break;
            }
        }
    }
}

void Clear_Aliens(void)
{
    int index;
    
    for (index = 0; index < MAX_NUMBER_OF_ALIEN_SHIPS; index++) {
        // zero out all the fields and allocate the memory
        
        alien_array[index].type        = 0;
        alien_array[index].x           = 0;
        alien_array[index].y           = 0;
        alien_array[index].xv          = 0;
        alien_array[index].yv          = 0;
        alien_array[index].state_1     = 0;
        alien_array[index].state_2     = 0;
        alien_array[index].aux_1       = 0;
        alien_array[index].aux_2       = 0;
        alien_array[index].new_state   = 0;
        alien_array[index].counter_1   = 0;
        alien_array[index].counter_2   = 0;
        alien_array[index].threshold_1 = 0;
        alien_array[index].threshold_2 = 0;
        alien_array[index].direction   = 0;
        alien_array[index].curr_frame  = 0;
        alien_array[index].x_clip = 0;
        alien_array[index].y_clip = 0;
        alien_array[index].width_clip = 0;
        alien_array[index].height_clip = 0;
        alien_array[index].visible = 1;
    }
}


void Init_Aliens(void)
{

// this function is used to clear all the mechs and get them ready

    int index;
    
    for (index = 0; index < MAX_NUMBER_OF_ALIEN_SHIPS; index++) {
        // zero out all the fields and allocate the memory
        
        alien_array[index].type        = 0;
        alien_array[index].x           = 0;
        alien_array[index].y           = 0;
        alien_array[index].xv          = 0;
        alien_array[index].yv          = 0;
        alien_array[index].state_1     = 0;
        alien_array[index].state_2     = 0;
        alien_array[index].aux_1       = 0;
        alien_array[index].aux_2       = 0;
        alien_array[index].new_state   = 0;
        alien_array[index].counter_1   = 0;
        alien_array[index].counter_2   = 0;
        alien_array[index].threshold_1 = 0;
        alien_array[index].threshold_2 = 0;
        alien_array[index].direction   = 0;
        alien_array[index].curr_frame  = 0;
        alien_array[index].background  = (unsigned char far *)_fmalloc((alien_1.width * alien_1.height) + 1);
        alien_array[index].x_clip = 0;
        alien_array[index].y_clip = 0;
        alien_array[index].width_clip = 0;
        alien_array[index].height_clip = 0;
        alien_array[index].visible = 1;
        alien_array[index].width = alien_1.width;
        alien_array[index].height = alien_1.height;
        
    } // end index
    
} // end Init_Mechs

///////////////////////////////////////////////////////////////////////////////

void Delete_Aliens(void)
{

    int index;
    
    for (index = 0; index < MAX_NUMBER_OF_ALIEN_SHIPS; index++) {
        _ffree(alien_array[index].background);
    } // end for index
    
} // end Delete_Mechs

void Erase_Aliens(void)
{

// this function erases all the mechs

    int index;
    
// loop thru all mechs

    for (index = num_aliens; index >= 0; index--) {
    
        // based on type of mech use proper animation frames
        
        // test if mech is alive
        
        if (alien_array[index].state_1 != SPRITE_DEAD) {
        
            // need to know which mech type so correct bitmaps can be used
            
            switch (alien_array[index].type) {
            
                case ALIEN_1: { // type one mech
                    alien_1.visible = alien_array[index].visible;
                    alien_1.width = alien_array[index].width;
                    alien_1.x_clip = alien_array[index].x_clip;
                    alien_1.y_clip = alien_array[index].y_clip;
                    alien_1.width_clip = alien_array[index].width_clip;
                    alien_1.height_clip = alien_array[index].height_clip;
                    alien_1.background = alien_array[index].background;
                    
                    Sprite_Erase_Clip((sprite_ptr)&alien_1, double_buffer);
                    
                }
                break;
                
                case ALIEN_2: { // type two mech
                    alien_2.visible = alien_array[index].visible;
                    alien_2.width = alien_array[index].width;
                    alien_2.x_clip = alien_array[index].x_clip;
                    alien_2.y_clip = alien_array[index].y_clip;
                    alien_2.width_clip = alien_array[index].width_clip;
                    alien_2.height_clip = alien_array[index].height_clip;
                    alien_2.background = alien_array[index].background;
                    
                    Sprite_Erase_Clip((sprite_ptr)&alien_2, double_buffer);
                    
                }
                break;
                
                default:
                    break;
                    
            } // end switch
            
        } // end if mech dead
        
    } // end for index
    
} // end Erase_Aliens

///////////////////////////////////////////////////////////////////////////////

void Draw_Aliens(void)
{

// this function draws the mechs

    int index;
    
// process each mech

    for (index = 0; index < num_aliens; index++) {
    
        // test if mech is alive
        
        if (alien_array[index].state_1 != SPRITE_DEAD) {
        
            switch (alien_array[index].type) {
            
                case ALIEN_1: { // type one mech
                    alien_1.width = alien_array[index].width;
                    alien_1.x_clip = alien_array[index].x_clip;
                    alien_1.y_clip = alien_array[index].y_clip;
                    alien_1.visible = alien_array[index].visible;
                    alien_1.width_clip = alien_array[index].width_clip;
                    alien_1.height_clip = alien_array[index].height_clip;
                    alien_1.bitmap_x = alien_array[index].bitmap_x;
                    alien_1.bitmap_y = alien_array[index].bitmap_y;
                    alien_1.curr_frame = alien_array[index].curr_frame;
                    
                    // test if we should use dying blitter
                    Sprite_Draw_Clip1((sprite_ptr)&alien_1, double_buffer);
                    
                }
                break;
                
                case ALIEN_2: { // type two mech
                    alien_2.width = alien_array[index].width;
                    alien_2.x_clip = alien_array[index].x_clip;
                    alien_2.y_clip = alien_array[index].y_clip;
                    alien_2.width_clip = alien_array[index].width_clip;
                    alien_2.height_clip = alien_array[index].height_clip;
                    alien_2.visible = alien_array[index].visible;
                    alien_2.bitmap_x = alien_array[index].bitmap_x;
                    alien_2.bitmap_y = alien_array[index].bitmap_y;
                    alien_2.curr_frame = alien_array[index].curr_frame;
                    
                    // test if we should use dying blitter
                    Sprite_Draw_Clip1((sprite_ptr)&alien_2, double_buffer);
                    
                }
                break;
                
                default:
                    break;
                    
            } // end switch
            
        } // end if mech dead
        
    } // end for index
    
} // end Draw_Mechs

//////////////////////////////////////////////////////////////////////////////

void Behind_Aliens(void)
{
// this function scans the background under the mechs

    int index;
    
// loop and process all mechs

    for (index = 0; index < num_aliens; index++) {
    
        // test if mech is alive
        
        if (alien_array[index].state_1 != SPRITE_DEAD) {
        
            switch (alien_array[index].type) {
            
                case ALIEN_1: { // type one mech
                    alien_1.width      = alien_array[index].width;
                    alien_1.height     = alien_array[index].height;
                    alien_1.x          = alien_array[index].x;
                    alien_1.y          = alien_array[index].y;
                    alien_1.background = alien_array[index].background;
                    
                    Sprite_Under_Clip1((sprite_ptr)&alien_1, double_buffer, 315, 196, 105, 15);
                    
                    alien_array[index].x_clip = alien_1.x_clip;
                    alien_array[index].y_clip = alien_1.y_clip;
                    alien_array[index].width_clip = alien_1.width_clip;
                    alien_array[index].height_clip = alien_1.height_clip;
                    alien_array[index].visible = alien_1.visible;
                    alien_array[index].background = alien_1.background;
                    alien_array[index].bitmap_x = alien_1.bitmap_x;
                    alien_array[index].bitmap_y = alien_1.bitmap_y;
                    
                }
                break;
                
                case ALIEN_2: { // type two mech
                    alien_2.width      = alien_array[index].width;
                    alien_2.height     = alien_array[index].height;
                    alien_2.x          = alien_array[index].x;
                    alien_2.y          = alien_array[index].y;
                    alien_2.background = alien_array[index].background;
                    
                    Sprite_Under_Clip1((sprite_ptr)&alien_2, double_buffer, 315, 196, 105, 15);
                    
                    alien_array[index].x_clip = alien_2.x_clip;
                    alien_array[index].y_clip = alien_2.y_clip;
                    alien_array[index].width_clip = alien_2.width_clip;
                    alien_array[index].height_clip = alien_2.height_clip;
                    alien_array[index].visible = alien_2.visible;
                    alien_array[index].background = alien_2.background;
                    alien_array[index].bitmap_x = alien_2.bitmap_x;
                    alien_array[index].bitmap_y = alien_2.bitmap_y;
                    
                }
                break;
                
                default:
                    break;
                    
            } // end switch
            
        } // end if mech dead
        
    } // end for index
    
} // end Behind_Mechs

////////////////////////////////////////////////////////////////////////////////

void Start_Explosion(int x, int y, int speed)
{
// this function stars a generic explosion

    int index, variable;  // looping variable
    
// scan for a useable explosion

    for (index = 0; index < MAX_NUMBER_OF_EXP; index++) {
    
        if (explosions[index].state_1 == EXPLOSION_INACTIVE) {
        
            // set up fields
            
            explosions[index].state_1       = EXPLOSION_ACTIVE;
            explosions[index].x           = x; // screen coordinates
            explosions[index].y           = y;
            
            explosions[index].curr_frame  = 0;
            explosions[index].threshold_1 = speed;
            explosions[index].counter_1   = 0;
            
            
            // make some sound
            FX_Play((rand() % 2 + 2), 1);
            
            break; // exit loop
            
        } // end if found a good one
        
    } // end for index
    
} // end Start_Explosion

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Draw_Explosions(void)
{
// this function draws the explosion

    int index; // looping variable
    
// scan for a useable explosion

    for (index = 0; index < MAX_NUMBER_OF_EXP; index++) {
    
        // make sure this explosion is alive
        
        if (explosions[index].state_1 == EXPLOSION_ACTIVE) {
            exp_1.x = explosions[index].x;
            exp_1.y = explosions[index].y;
            exp_1.curr_frame  = explosions[index].curr_frame;
            
            Sprite_Draw_Clip2((sprite_ptr)&exp_1);
            
        } // end if found a good one
        
    } // end for index
    
} // end Draw_Explosions

/////////////////////////////////////////////////////////////////////////////

void Animate_Explosions(void)
{
// this function aniamtes the explosions

    int index;  // looping index
    
// scan for a useable explosion

    for (index = 0; index < MAX_NUMBER_OF_EXP; index++) {
        // test if explosion is active
        
        if (explosions[index].state_1 == EXPLOSION_ACTIVE) {
            // test if it's time to change frames
            
            if (++explosions[index].counter_1 == explosions[index].threshold_1) {
                // is the explosion over?
                
                if (++explosions[index].curr_frame == NUM_SHIP_EXP_FRAMES) {
                    explosions[index].state_1 = EXPLOSION_INACTIVE;
                }
                
                // reset animation clock for future
                
                explosions[index].counter_1 = 0;
                
            } // end if time to change frames
            
        } // end if found a good one
        
    } // end for index
    
} // end Animate_Explosions

//////////////////////////////////////////////////////////////////////////////

void Init_Explosions(void)
{
// clear out the states of all explosions

    int index; // looping variable
    
    for (index = 0; index < MAX_NUMBER_OF_EXP; index++) {
        explosions[index].state_1 = EXPLOSION_INACTIVE;
        explosions[index].x_clip = 0;
        explosions[index].y_clip = 0;
        explosions[index].width_clip = 0;
        explosions[index].height_clip = 0;
        explosions[index].visible = 1;
        explosions[index].width = exp_1.width;
        explosions[index].height = exp_1.height;
    }
    
} // Init_Explosions

void Sprite_Under_Clip1(sprite_ptr sprite, unsigned char far *buffer, int rx, int by, int lx, int ty)
{

// this function scans the background under a sprite, but only those
// portions that are visible

    unsigned char far *back_buffer;   // pointer to sprite background buffer
    unsigned char far *source_buffer; // pointer to source buffer
    
    int x, y,                       // looping variables
        sx, sy,                     // position of sprite
        width,
        bitmap_x      = 0,          // starting upper left corner of sub-bitmap
        bitmap_y      = 0,          // to be drawn after clipping                // width of sprite
        bitmap_width  = 0,          // width and height of sub-bitmap
        bitmap_height = 0;
        
    unsigned char pixel;            // the current pixel being processed
    
// alias a variable to sprite size

    width         = sprite->width;
    bitmap_width  = width;
    bitmap_height = sprite->height;
    sx            = sprite->x;
    sy            = sprite->y;
    
// perform trivial rejection tests

    if (sx > rx || sy > by || (sx + width) < lx  || (sy + bitmap_height) < ty) {
        // sprite is totally invisible therefore don't scan
        
        // set invisible flag in strucuture so that the draw sub-function
        // doesn't do anything
        
        sprite->visible = 0;
        
        return;
        
    } // end if invisible
    
// the sprite background region must be clipped before scanning
// therefore compute visible portion

// first compute upper left hand corner of clipped sprite background

    if (sx < lx) {
        bitmap_x += (lx - sx);
        bitmap_width -= (lx - sx);
        sx            = lx;
        
        
    } // end off left edge
    else if ((sx + width) > rx) {
        bitmap_width  = (rx - sx);
    } // end off right edge
    
// now process y

    if (sy < ty) {
        bitmap_y += (ty - sy);
        bitmap_height -= (ty - sy);
        sy             = ty;
        
    } // end off top edge
    else if ((sy + bitmap_height) > by) {
        bitmap_height  = (by - sy);
        
    } // end off lower edge
    
// this point we know were to start scanning the bitmap i.e.
// sx,sy
// and we know the size of the bitmap to be scanned i.e.
// width,height, so plug it all into the rest of function

// compute number of bytes between adjacent video lines after a row of pixels
// has been drawn

// compute offset of sprite background in source buffer

    source_buffer = buffer + (sy << 8) + (sy << 6) + sx;
    
// alias a pointer to sprite background

    back_buffer = sprite->background;
    
    for (y = 0; y < bitmap_height; y++) {
        // copy the next row into the destination buffer
        
        _fmemcpy((void far *)back_buffer, (void far *)source_buffer, bitmap_width);
        
        // move to next line in desintation buffer and sprite image buffer
        
        source_buffer += SCREEN_WIDTH;
        back_buffer   += width;  // note this width is the actual width of the
        // entire bitmap NOT the visible portion
    } // end for y
    
// set variables in strucuture so that the erase sub-function can operate
// faster

    sprite->visible     = 1;
    sprite->x_clip      = sx;
    sprite->y_clip      = sy;
    sprite->width_clip  = bitmap_width;
    sprite->height_clip = bitmap_height;
    sprite->bitmap_x = bitmap_x;
    sprite->bitmap_y = bitmap_y;
    
    
} // end Sprite_Under_Clip

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Sprite_Draw_Clip1(sprite_ptr sprite, unsigned char far *buffer)
{

// this function draws a sprite on the screen row by row very quickly
// note the use of shifting to implement multplication
// if the transparent flag is true then pixels wil be draw one by one
// else a memcpy will be used to draw each line
// this function also performs clipping. It will test if the sprite
// is totally visible/invisible and will only draw the portions that are visible

    unsigned char far *sprite_data; // pointer to sprite data
    unsigned char far *dest_buffer; // pointer to destination buffer
    
    int x, y,                       // looping variables
        sx, sy,                     // position of sprite
        width,                      // width of sprite
        bitmap_x      = 0,          // starting upper left corner of sub-bitmap
        bitmap_y      = 0,          // to be drawn after clipping
        bitmap_width  = 0,          // width and height of sub-bitmap
        bitmap_height = 0;
        
    unsigned char pixel;            // the current pixel being processed
    
// alias a variable to sprite size
    if (!sprite->visible) {
        return;
    }
    
    width         = sprite->width;
    sx = sprite->x_clip;
    sy = sprite->y_clip;
    bitmap_width = sprite->width_clip;
    bitmap_height = sprite->height_clip;
    bitmap_y = sprite->bitmap_y;
    bitmap_x = sprite->bitmap_x;
    
    
// this point we know were to start drawing the bitmap i.e.
// sx,sy
// and we know were in the data to extract the bitmap i.e.
// bitmap_x, bitmap_y,
// and finaly we know the size of the bitmap to be drawn i.e.
// width,height, so plug it all into the rest of function

// compute number of bytes between adjacent video lines after a row of pixels
// has been drawn

// compute offset of sprite in destination buffer

    dest_buffer = buffer + (sy << 8) + (sy << 6) + sx;
    
// alias a pointer to sprite for ease of access and locate starting sub
// bitmap that will be drawn

    sprite_data = sprite->frames[sprite->curr_frame] + (bitmap_y * width) + bitmap_x;
    
// copy each line of the sprite data into destination buffer


    for (y = 0; y < bitmap_height; y++) {
        // copy the next row into the destination buffer
        
        for (x = 0; x < bitmap_width; x++) {
        
            // test for transparent pixel i.e. 0, if not transparent then draw
            
            if ((pixel = sprite_data[x])) {
                dest_buffer[x] = pixel;
            }
            
        } // end for x
        
        // move to next line in desintation buffer and sprite image buffer
        
        dest_buffer += SCREEN_WIDTH;
        sprite_data += width;   // note this width is the actual width of the
        // entire bitmap NOT the visible portion
    } // end for y
    
} // end Sprite_Draw_Clip

void Test_For_Warrior(void)
{
    FILE *fp, *fp1;
    char name[10][15];
    char score[10][15];
    char buffer[15];
    long cur_score;
    long the_score;
    int warrior = 0;
    char curname[10], curscore[15];
    int done = 0;
    int text_x = 111;
    int let_index = 0;
    int index, tmp_index;
    pcx_picture image_pcx;
    
    the_score = cur_player.points;
    
    fp = fopen("scorelog.dat", "r+");
    fscanf(fp, "%s %s", &name[0], &score[0]);
    fscanf(fp, "%s %s", &name[1], &score[1]);
    fscanf(fp, "%s %s", &name[2], &score[2]);
    fscanf(fp, "%s %s", &name[3], &score[3]);
    fscanf(fp, "%s %s", &name[4], &score[4]);
    fscanf(fp, "%s %s", &name[5], &score[5]);
    fscanf(fp, "%s %s", &name[6], &score[6]);
    fscanf(fp, "%s %s", &name[7], &score[7]);
    fscanf(fp, "%s %s", &name[8], &score[8]);
    fscanf(fp, "%s %s", &name[9], &score[9]);
    fclose(fp);
    for (index = 0; index < 10 && warrior == 0; index++) {
        cur_score = atol(score[index]);
        if (the_score > cur_score) {
            warrior = 1;
            tmp_index = index;
        }
    }
    index = tmp_index;
    if (warrior) {
        PCX_Init((pcx_picture_ptr)&image_pcx);
        PCX_Load("warrior.pcx", (pcx_picture_ptr)&image_pcx, 1);
        PCX_Show_Buffer((pcx_picture_ptr)&image_pcx);
        PCX_Delete((pcx_picture_ptr)&image_pcx);
        while (!done) {
            if (keys_active > 0) {
                if (keyboard_state[MAKE_A] && let_index < 10) {
                    Tech_Print(text_x, 111, "A", video_buffer);
                    text_x += 9;
                    curname[let_index] = 'A';
                    let_index++;
                } else if (keyboard_state[MAKE_B] && let_index < 10) {
                    Tech_Print(text_x, 111, "B", video_buffer);
                    text_x += 9;
                    curname[let_index] = 'B';
                    let_index++;
                } else if (keyboard_state[MAKE_C] && let_index < 10) {
                    Tech_Print(text_x, 111, "C", video_buffer);
                    text_x += 9;
                    curname[let_index] = 'C';
                    let_index++;
                } else if (keyboard_state[MAKE_D] && let_index < 10) {
                    Tech_Print(text_x, 111, "D", video_buffer);
                    text_x += 9;
                    curname[let_index] = 'D';
                    let_index++;
                } else if (keyboard_state[MAKE_E] && let_index < 10) {
                    Tech_Print(text_x, 111, "E", video_buffer);
                    text_x += 9;
                    curname[let_index] = 'E';
                    let_index++;
                } else if (keyboard_state[MAKE_F] && let_index < 10) {
                    Tech_Print(text_x, 111, "F", video_buffer);
                    text_x += 9;
                    curname[let_index] = 'F';
                    let_index++;
                } else if (keyboard_state[MAKE_G] && let_index < 10) {
                    Tech_Print(text_x, 111, "G", video_buffer);
                    text_x += 9;
                    curname[let_index] = 'G';
                    let_index++;
                } else if (keyboard_state[MAKE_H] && let_index < 10) {
                    Tech_Print(text_x, 111, "H", video_buffer);
                    text_x += 9;
                    curname[let_index] = 'H';
                    let_index++;
                } else if (keyboard_state[MAKE_I] && let_index < 10) {
                    Tech_Print(text_x, 111, "I", video_buffer);
                    text_x += 9;
                    curname[let_index] = 'I';
                    let_index++;
                } else if (keyboard_state[MAKE_J] && let_index < 10) {
                    Tech_Print(text_x, 111, "J", video_buffer);
                    text_x += 9;
                    curname[let_index] = 'J';
                    let_index++;
                } else if (keyboard_state[MAKE_K] && let_index < 10) {
                    Tech_Print(text_x, 111, "K", video_buffer);
                    text_x += 9;
                    curname[let_index] = 'K';
                    let_index++;
                } else if (keyboard_state[MAKE_L] && let_index < 10) {
                    Tech_Print(text_x, 111, "L", video_buffer);
                    text_x += 9;
                    curname[let_index] = 'L';
                    let_index++;
                } else if (keyboard_state[MAKE_M] && let_index < 10) {
                    Tech_Print(text_x, 111, "M", video_buffer);
                    text_x += 9;
                    curname[let_index] = 'M';
                    let_index++;
                } else if (keyboard_state[MAKE_N] && let_index < 10) {
                    Tech_Print(text_x, 111, "N", video_buffer);
                    text_x += 9;
                    curname[let_index] = 'N';
                    let_index++;
                } else if (keyboard_state[MAKE_O] && let_index < 10) {
                    Tech_Print(text_x, 111, "O", video_buffer);
                    text_x += 9;
                    curname[let_index] = 'O';
                    let_index++;
                } else if (keyboard_state[MAKE_P] && let_index < 10) {
                    Tech_Print(text_x, 111, "P", video_buffer);
                    text_x += 9;
                    curname[let_index] = 'P';
                    let_index++;
                } else if (keyboard_state[MAKE_Q] && let_index < 10) {
                    Tech_Print(text_x, 111, "Q", video_buffer);
                    text_x += 9;
                    curname[let_index] = 'Q';
                    let_index++;
                } else if (keyboard_state[MAKE_R] && let_index < 10) {
                    Tech_Print(text_x, 111, "R", video_buffer);
                    text_x += 9;
                    curname[let_index] = 'R';
                    let_index++;
                } else if (keyboard_state[MAKE_S] && let_index < 10) {
                    Tech_Print(text_x, 111, "S", video_buffer);
                    text_x += 9;
                    curname[let_index] = 'S';
                    let_index++;
                } else if (keyboard_state[MAKE_T] && let_index < 10) {
                    Tech_Print(text_x, 111, "T", video_buffer);
                    text_x += 9;
                    curname[let_index] = 'T';
                    let_index++;
                } else if (keyboard_state[MAKE_U] && let_index < 10) {
                    Tech_Print(text_x, 111, "U", video_buffer);
                    text_x += 9;
                    curname[let_index] = 'U';
                    let_index++;
                } else if (keyboard_state[MAKE_V] && let_index < 10) {
                    Tech_Print(text_x, 111, "V", video_buffer);
                    text_x += 9;
                    curname[let_index] = 'V';
                    let_index++;
                } else if (keyboard_state[MAKE_W] && let_index < 10) {
                    Tech_Print(text_x, 111, "W", video_buffer);
                    text_x += 9;
                    curname[let_index] = 'W';
                    let_index++;
                } else if (keyboard_state[MAKE_X] && let_index < 10) {
                    Tech_Print(text_x, 111, "X", video_buffer);
                    text_x += 9;
                    curname[let_index] = 'X';
                    let_index++;
                }
                if (keyboard_state[MAKE_Y] && let_index < 10) {
                    Tech_Print(text_x, 111, "Y", video_buffer);
                    text_x += 9;
                    curname[let_index] = 'Y';
                    let_index++;
                } else if (keyboard_state[MAKE_Z] && let_index < 10) {
                    Tech_Print(text_x, 111, "Z", video_buffer);
                    text_x += 9;
                    curname[let_index] = 'Z';
                    let_index++;
                } else if (keyboard_state[MAKE_1] && let_index < 10) {
                    Tech_Print(text_x, 111, "1", video_buffer);
                    text_x += 9;
                    curname[let_index] = '1';
                    let_index++;
                } else if (keyboard_state[MAKE_2] && let_index < 10) {
                    Tech_Print(text_x, 111, "2", video_buffer);
                    text_x += 9;
                    curname[let_index] = '2';
                    let_index++;
                } else if (keyboard_state[MAKE_3] && let_index < 10) {
                    Tech_Print(text_x, 111, "3", video_buffer);
                    text_x += 9;
                    curname[let_index] = '3';
                    let_index++;
                } else if (keyboard_state[MAKE_4] && let_index < 10) {
                    Tech_Print(text_x, 111, "4", video_buffer);
                    text_x += 9;
                    curname[let_index] = '4';
                    let_index++;
                } else if (keyboard_state[MAKE_5] && let_index < 10) {
                    Tech_Print(text_x, 111, "5", video_buffer);
                    text_x += 9;
                    curname[let_index] = '5';
                    let_index++;
                } else if (keyboard_state[MAKE_6] && let_index < 10) {
                    Tech_Print(text_x, 111, "6", video_buffer);
                    text_x += 9;
                    curname[let_index] = '6';
                    let_index++;
                } else if (keyboard_state[MAKE_7] && let_index < 10) {
                    Tech_Print(text_x, 111, "7", video_buffer);
                    text_x += 9;
                    curname[let_index] = '7';
                    let_index++;
                } else if (keyboard_state[MAKE_8] && let_index < 10) {
                    Tech_Print(text_x, 111, "8", video_buffer);
                    text_x += 9;
                    curname[let_index] = '8';
                    let_index++;
                } else if (keyboard_state[MAKE_9] && let_index < 10) {
                    Tech_Print(text_x, 111, "9", video_buffer);
                    text_x += 9;
                    curname[let_index] = '9';
                    let_index++;
                } else if (keyboard_state[MAKE_0] && let_index < 10) {
                    Tech_Print(text_x, 111, "0", video_buffer);
                    text_x += 9;
                    curname[let_index] = '0';
                    let_index++;
                } else if (keyboard_state[MAKE_1] && let_index < 10) {
                    Tech_Print(text_x, 111, "1", video_buffer);
                    text_x += 9;
                    curname[let_index] = '1';
                    let_index++;
                } else if (keyboard_state[MAKE_ENTER]) {
                    done = 1;
                    ltoa(cur_player.points, buffer, 10);
                    strcpy(curscore, buffer);
                    for (; let_index < 10; let_index++) {
                        curname[let_index] = NULL;
                    }
                    curname[10] = NULL;
                    break;
                } else if (keyboard_state[MAKE_BKSP]) {
                    if (let_index > 0) {
                        text_x -= 9;
                        let_index--;
                        curname[let_index] = NULL;
                        Tech_Print(text_x, 111, " ", video_buffer);
                    }
                }
            }
            Delay(2);
        }
        remove("scorelog.dat");
        fp1 = fopen("scorelog.dat", "w");
        if (index < 10) {
            switch (index) {
                case 0: {
                    fprintf(fp1, "%s %s\n", curname, curscore);
                    fprintf(fp1, "%s %s\n", name[0], score[0]);
                    fprintf(fp1, "%s %s\n", name[1], score[1]);
                    fprintf(fp1, "%s %s\n", name[2], score[2]);
                    fprintf(fp1, "%s %s\n", name[3], score[3]);
                    fprintf(fp1, "%s %s\n", name[4], score[4]);
                    fprintf(fp1, "%s %s\n", name[5], score[5]);
                    fprintf(fp1, "%s %s\n", name[6], score[6]);
                    fprintf(fp1, "%s %s\n", name[7], score[7]);
                    fprintf(fp1, "%s %s\n", name[8], score[8]);
                }
                break;
                case 1: {
                    fprintf(fp1, "%s %s\n", name[0], score[0]);
                    fprintf(fp1, "%s %s\n", curname, curscore);
                    fprintf(fp1, "%s %s\n", name[1], score[1]);
                    fprintf(fp1, "%s %s\n", name[2], score[2]);
                    fprintf(fp1, "%s %s\n", name[3], score[3]);
                    fprintf(fp1, "%s %s\n", name[4], score[4]);
                    fprintf(fp1, "%s %s\n", name[5], score[5]);
                    fprintf(fp1, "%s %s\n", name[6], score[6]);
                    fprintf(fp1, "%s %s\n", name[7], score[7]);
                    fprintf(fp1, "%s %s\n", name[8], score[8]);
                }
                break;
                case 2: {
                    fprintf(fp1, "%s %s\n", name[0], score[0]);
                    fprintf(fp1, "%s %s\n", name[1], score[1]);
                    fprintf(fp1, "%s %s\n", curname, curscore);
                    fprintf(fp1, "%s %s\n", name[2], score[2]);
                    fprintf(fp1, "%s %s\n", name[3], score[3]);
                    fprintf(fp1, "%s %s\n", name[4], score[4]);
                    fprintf(fp1, "%s %s\n", name[5], score[5]);
                    fprintf(fp1, "%s %s\n", name[6], score[6]);
                    fprintf(fp1, "%s %s\n", name[7], score[7]);
                    fprintf(fp1, "%s %s\n", name[8], score[8]);
                }
                break;
                case 3: {
                    fprintf(fp1, "%s %s\n", name[0], score[0]);
                    fprintf(fp1, "%s %s\n", name[1], score[1]);
                    fprintf(fp1, "%s %s\n", name[2], score[2]);
                    fprintf(fp1, "%s %s\n", curname, curscore);
                    fprintf(fp1, "%s %s\n", name[3], score[3]);
                    fprintf(fp1, "%s %s\n", name[4], score[4]);
                    fprintf(fp1, "%s %s\n", name[5], score[5]);
                    fprintf(fp1, "%s %s\n", name[6], score[6]);
                    fprintf(fp1, "%s %s\n", name[7], score[7]);
                    fprintf(fp1, "%s %s\n", name[8], score[8]);
                }
                break;
                case 4: {
                    fprintf(fp1, "%s %s\n", name[0], score[0]);
                    fprintf(fp1, "%s %s\n", name[1], score[1]);
                    fprintf(fp1, "%s %s\n", name[2], score[2]);
                    fprintf(fp1, "%s %s\n", name[3], score[3]);
                    fprintf(fp1, "%s %s\n", curname, curscore);
                    fprintf(fp1, "%s %s\n", name[4], score[4]);
                    fprintf(fp1, "%s %s\n", name[5], score[5]);
                    fprintf(fp1, "%s %s\n", name[6], score[6]);
                    fprintf(fp1, "%s %s\n", name[7], score[7]);
                    fprintf(fp1, "%s %s\n", name[8], score[8]);
                }
                break;
                case 5: {
                    fprintf(fp1, "%s %s\n", name[0], score[0]);
                    fprintf(fp1, "%s %s\n", name[1], score[1]);
                    fprintf(fp1, "%s %s\n", name[2], score[2]);
                    fprintf(fp1, "%s %s\n", name[3], score[3]);
                    fprintf(fp1, "%s %s\n", name[4], score[4]);
                    fprintf(fp1, "%s %s\n", curname, curscore);
                    fprintf(fp1, "%s %s\n", name[5], score[5]);
                    fprintf(fp1, "%s %s\n", name[6], score[6]);
                    fprintf(fp1, "%s %s\n", name[7], score[7]);
                    fprintf(fp1, "%s %s\n", name[8], score[8]);
                }
                break;
                case 6: {
                    fprintf(fp1, "%s %s\n", name[0], score[0]);
                    fprintf(fp1, "%s %s\n", name[1], score[1]);
                    fprintf(fp1, "%s %s\n", name[2], score[2]);
                    fprintf(fp1, "%s %s\n", name[3], score[3]);
                    fprintf(fp1, "%s %s\n", name[4], score[4]);
                    fprintf(fp1, "%s %s\n", name[5], score[5]);
                    fprintf(fp1, "%s %s\n", curname, curscore);
                    fprintf(fp1, "%s %s\n", name[6], score[6]);
                    fprintf(fp1, "%s %s\n", name[7], score[7]);
                    fprintf(fp1, "%s %s\n", name[8], score[8]);
                }
                break;
                case 7: {
                    fprintf(fp1, "%s %s\n", name[0], score[0]);
                    fprintf(fp1, "%s %s\n", name[1], score[1]);
                    fprintf(fp1, "%s %s\n", name[2], score[2]);
                    fprintf(fp1, "%s %s\n", name[3], score[3]);
                    fprintf(fp1, "%s %s\n", name[4], score[4]);
                    fprintf(fp1, "%s %s\n", name[5], score[5]);
                    fprintf(fp1, "%s %s\n", name[6], score[6]);
                    fprintf(fp1, "%s %s\n", curname, curscore);
                    fprintf(fp1, "%s %s\n", name[7], score[7]);
                    fprintf(fp1, "%s %s\n", name[8], score[8]);
                }
                break;
                case 8: {
                    fprintf(fp1, "%s %s\n", name[0], score[0]);
                    fprintf(fp1, "%s %s\n", name[1], score[1]);
                    fprintf(fp1, "%s %s\n", name[2], score[2]);
                    fprintf(fp1, "%s %s\n", name[3], score[3]);
                    fprintf(fp1, "%s %s\n", name[4], score[4]);
                    fprintf(fp1, "%s %s\n", name[5], score[5]);
                    fprintf(fp1, "%s %s\n", name[6], score[6]);
                    fprintf(fp1, "%s %s\n", name[7], score[7]);
                    fprintf(fp1, "%s %s\n", curname, curscore);
                    fprintf(fp1, "%s %s\n", name[8], score[8]);
                }
                break;
                case 9: {
                    fprintf(fp1, "%s %s\n", name[0], score[0]);
                    fprintf(fp1, "%s %s\n", name[1], score[1]);
                    fprintf(fp1, "%s %s\n", name[2], score[2]);
                    fprintf(fp1, "%s %s\n", name[3], score[3]);
                    fprintf(fp1, "%s %s\n", name[4], score[4]);
                    fprintf(fp1, "%s %s\n", name[5], score[5]);
                    fprintf(fp1, "%s %s\n", name[6], score[6]);
                    fprintf(fp1, "%s %s\n", name[7], score[7]);
                    fprintf(fp1, "%s %s\n", name[8], score[8]);
                    fprintf(fp1, "%s %s\n", curname, curscore);
                }
            }
        }
        fclose(fp1);
    }
    Screen_Transition(SCREEN_DARKNESS);
    clrscr();
}

void Init_Missiles(void)
{
    int index;
    
    cur_player.missile_type = 0;
    for (index = 0; index < NUM_MISSILES; index++) {
        missiles[index].state = MISS_DEAD;
    }
}

void Start_Missile(int x,
                   int y,
                   int x1,
                   int type)
{

// this function scans through the missile array and tries to find one that
// isn't being used.  this function could be more efficient.

    int index;
    
// scan for a useable missle

    for (index = 0; index < num_missiles; index++) {
        // is this missile free?
        
        if (missiles[index].state == MISS_DEAD) {
        
            // set up fields
            
            missiles[index].state = MISS_ALIVE;
            missiles[index].x     = x;
            missiles[index].y     = y;
            missiles[index].type  = type;
            switch (type) {
                case 0: {
                    missiles[index].color = 55;
                    missiles[index].length = 6;
                    missiles[index].yv = 3;
                }
                break;
                case 1: {
                    missiles[index].color = 59;
                    missiles[index].length = 8;
                    missiles[index].yv = 5;
                }
                break;
                case 2: {
                    missiles[index].color = 71;
                    missiles[index].length = 10;
                    missiles[index].yv = 7;
                }
                break;
                case 3: {
                    missiles[index].color = 96;
                    missiles[index].length = 12;
                    missiles[index].yv = 9;
                }
                break;
                case 4: {
                    missiles[index].color = 119;
                    missiles[index].length = 14;
                    missiles[index].yv = 11;
                }
                break;
                case 5: {
                    missiles[index].color = 143;
                    missiles[index].length = 16;
                    missiles[index].yv = 13;
                }
                break;
                case 6: {
                    missiles[index].color = 183;
                    missiles[index].length = 18;
                    missiles[index].yv = 15;
                }
                break;
                case 7: {
                    missiles[index].color = 12;
                    missiles[index].length = 20;
                    missiles[index].yv = 17;
                }
                break;
            }
            cur_missile_length = missiles[index].length;
            break; // exit loop
            
        } // end if found a good one
        
    } // end for index
    
    for (index = 0; index < num_missiles; index++) {
        if (missiles[index].state == MISS_DEAD) {
            missiles[index].state = MISS_ALIVE;
            missiles[index].x     = x1;
            missiles[index].y     = y;
            missiles[index].type  = type;
            switch (type) {
                case 0: {
                    missiles[index].color = 55;
                    missiles[index].length = 6;
                    missiles[index].yv = 3;
                }
                break;
                case 1: {
                    missiles[index].color = 59;
                    missiles[index].length = 8;
                    missiles[index].yv = 5;
                }
                break;
                case 2: {
                    missiles[index].color = 71;
                    missiles[index].length = 10;
                    missiles[index].yv = 7;
                }
                break;
                case 3: {
                    missiles[index].color = 96;
                    missiles[index].length = 12;
                    missiles[index].yv = 9;
                }
                break;
                case 4: {
                    missiles[index].color = 119;
                    missiles[index].length = 14;
                    missiles[index].yv = 11;
                }
                break;
                case 5: {
                    missiles[index].color = 143;
                    missiles[index].length = 16;
                    missiles[index].yv = 13;
                }
                break;
                case 6: {
                    missiles[index].color = 183;
                    missiles[index].length = 18;
                    missiles[index].yv = 15;
                }
                break;
                case 7: {
                    missiles[index].color = 12;
                    missiles[index].length = 20;
                    missiles[index].yv = 17;
                }
                break;
            }
            cur_missile_length = missiles[index].length;
            FX_Play(LASER_SND, 2);
            break;
        }
    }
} // end Start_Missile

void Move_Missiles(void)
{

// this function moves the missiles and does all the collision detection

    int index,     // used for loops
        index2,
        index_3,
        pixel_y,   // used during barrier collsion scan
        delta_x,   // used to help test for bouding box collisions
        delta_y,
        creature_hit = 0, // position of missile
        miss_y,
        b_miss_x,
        b_miss_y;
        
    alien_ptr worker;     // the current mech being processed
    
    
// loop thru all missiles and perform a lot of tests

    for (index = 0; index < NUM_MISSILES; index++) {
    
        // is missile active
        
        if (missiles[index].state == MISS_ALIVE) {
        
            // move the missile
            miss_y = (missiles[index].y -= missiles[index].yv);
            b_miss_y = miss_y + cur_missile_length;
            
            // test if it's hit the edge of the screen or a wall
            
            if ( ( b_miss_y < 1) ) {
                missiles[index].state = MISS_DEAD;
                continue;
                
            } // end if off edge of screen
            
            // test for player->creature collisions
            
            creature_hit = 0;
            // test if missiles hit a creature
            
            for (index_3 = 0; index_3 < MAX_NUMBER_OF_ALIEN_SHIPS && !creature_hit; index_3++) {
            
                // extract the working mech
                
                worker = (alien_ptr)&alien_array[index_3];
                
                
                // test if the mech is a live and it isn't energizing
                
                if (worker->state_1 == SPRITE_ALIVE) {
                
                    // compute deltas
                    
                    delta_x = missiles[index].x - worker->x;
                    delta_y = miss_y - worker->y;
                    
                    
                    // test for collision
                    
                    if ( (delta_x >= 0 && delta_x <= 31) &&
                            (delta_y >= -missiles[index].length && delta_y <= 31)) {
                            
                        // kill missile
                        
                        missiles[index].state = MISS_DEAD;
                        
                        // kill mech
                        
                        worker->state_1     = SPRITE_DEAD;
                        worker->counter_1   = 0;
                        worker->threshold_1 = 8;
                        creature_hit = 1;
                        num_aliens--;
                        
                        Start_Explosion(worker->x, worker->y, 2);
                        if (worker->carry_goodie) {
                            Start_Powerup(worker->x, worker->y, worker->type_of_goodie, (rand() % 25) + 50);
                        }
                        
                        // start explosion
                        
                        cur_player.points += 20 + rand() % 50;
                        
                    } // end if a hit
                    
                } // end if worth testing
                
            } // end for index_3
            // test for barrier collisions by scanning the pixels in the near vicinity
            // to the torpedo
        } // end if missile alive
        
    } // end for index
    
} // end Move_Missiles

void Draw_Missiles(void)
{

// this function indexes through all the missiles and if they are active
// draws the missile as a bright white pixel on the screen

    int index;
    
    for (index = 0; index < NUM_MISSILES; index++) {
    
        // is this missile active
        
        if (missiles[index].state == MISS_ALIVE) {
            if (missiles[index].y < 1) {
                V_Line_DB(missiles[index].x, 1, (missiles[index].y + cur_missile_length), missiles[index].color);
            } else {
                V_Line_DB(missiles[index].x, missiles[index].y, (missiles[index].y + cur_missile_length), missiles[index].color);
            }
            
        } // end if alive
        
    } // end for index
    
} // end Draw_Missiles

void V_Line_DB(int x, int y1, int y2, unsigned int color)
{
// draw a vertical line, note y2 > y1

    unsigned int line_offset,
             index;
             
// compute starting position

    line_offset = ((y1 << 8) + (y1 << 6)) + x;
    
    for (index = 0; index <= y2 - y1; index++) {
        double_buffer[line_offset] = color;
        
        line_offset += 320; // move to next line
        
    } // end for index
    
} // end V_Line

void Load_Tiles(void)
{
    int index;
    
    PCX_Init((pcx_picture_ptr)&image);
    PCX_Load("tile1.pcx", (pcx_picture_ptr)&image, 1);
    Sprite_Init((sprite_ptr)&tiles, 0, 0, 16, 16, 0, 0, 0, 0, 0, 0);
    if (worldn < 5) {
        for (index = 0; index < 7; index++) {
            PCX_Get_Sprite((pcx_picture_ptr)&image, (sprite_ptr)&tiles, index, index, (worldn * 2));
        }
        for (index = 7; index < 10; index++) {
            PCX_Get_Sprite((pcx_picture_ptr)&image, (sprite_ptr)&tiles, index, index - 7, ((worldn * 2) + 1));
        }
    } else if (worldn > 4) {
        for (index = 0; index < 7; index++) {
            PCX_Get_Sprite((pcx_picture_ptr)&image, (sprite_ptr)&tiles, index, index, (worldn * 2) - 10);
        }
        for (index = 7; index < 10; index++) {
            PCX_Get_Sprite((pcx_picture_ptr)&image, (sprite_ptr)&tiles, index, index - 7, ((worldn * 2) + 1) - 10);
        }
    }
    PCX_Delete((pcx_picture_ptr)&image);
}

void Delete_Tiles(void)
{
    Sprite_Delete((sprite_ptr)&tiles);
}

int Draw_World(void)
{
    int index;
    
    the_world = (world_ptr)&worlds;
    
    for (index = 0; index < NUM_ROWS; index++) {
        if ((the_world->rows[index].y > -16) && (the_world->rows[index].y < 200)) {
            Draw_Row(index);
        }
    }
}

void Draw_Row(int row_num)
{
    int index;
    row_ptr cur_row;
    
    cur_row = (row_ptr)&worlds.rows[row_num];
    
    for (index = 0; index < NUM_TILES_ROW; index++) {
        if (cur_row->tiles[index].there) {
            tile_p->curr_frame = t_array[cur_row->tiles[index].type];
            Draw_Tile(row_num, index);
        }
    }
}

void Draw_Tile(int row_num, int tile_num)
{
    // this function draws a sprite on the screen row by row very quickly
// note the use of shifting to implement multplication
// if the transparent flag is true then pixels wil be draw one by one
// else a memcpy will be used to draw each line
// this function also performs clipping. It will test if the sprite
// is totally visible/invisible and will only draw the portions that are visible

    unsigned char far *sprite_data; // pointer to sprite data
    unsigned char far *dest_buffer; // pointer to destination buffer
    unsigned char far *buffer = double_buffer;
    
    int x, y,                       // looping variables
        sx, sy,                     // position of sprite
        bitmap_x      = 0,          // starting upper left corner of sub-bitmap
        bitmap_y      = 0;          // to be drawn after clipping
        
    unsigned char pixel;            // the current pixel being processed
    sx = worlds.rows[row_num].tiles[tile_num].x;
    sy = worlds.rows[row_num].y;
    t_bitmap_height = 16;
    t_bitmap_width = 16;
    if (sy < 0) {
        bitmap_y += (0 - sy);
        t_bitmap_height -= (0 - sy);
        sy             = 0;
    } // end off top edge
    else if ((sy + t_bitmap_height) > 200) {
        t_bitmap_height  = (200 - sy);
    } // end off lower edge
    
    
// this point we know were to start drawing the bitmap i.e.
// sx,sy
// and we know were in the data to extract the bitmap i.e.
// bitmap_x, bitmap_y,
// and finaly we know the size of the bitmap to be drawn i.e.
// width,height, so plug it all into the rest of function

// compute number of bytes between adjacent video lines after a row of pixels
// has been drawn

// compute offset of sprite in destination buffer

    dest_buffer = buffer + (sy << 8) + (sy << 6) + sx;
    
// alias a pointer to sprite for ease of access and locate starting sub
// bitmap that will be drawn

    sprite_data = tile_p->frames[tile_p->curr_frame] + (bitmap_y * t_width) + bitmap_x;
    
// copy each line of the sprite data into destination buffer


    for (y = 0; y < t_bitmap_height; y++) {
        // copy the next row into the destination buffer
        
        for (x = 0; x < t_bitmap_width; x++) {
        
            // test for transparent pixel i.e. 0, if not transparent then draw
            
            dest_buffer[x] = sprite_data[x];
            
        } // end for x
        
        // move to next line in desintation buffer and sprite image buffer
        
        dest_buffer += SCREEN_WIDTH;
        sprite_data += t_width;   // note this width is the actual width of the
        // entire bitmap NOT the visible portion
    } // end for y
    
} // end Draw_Tile

void Erase_World(void)
{
    fquadset((void far *)double_buffer, 0, 16000);
}

void Move_World(void)
{
    int index;
    row_ptr r;
    
    r = (row_ptr)&worlds.rows;
    for (index = 0; index < NUM_ROWS; index++) {
        r[index].y++;
    }
}

void Init_World(void)
{
    // this function is used to start a wave off
    
    int x, y, element, index;
    FILE *w;
    tile_ptr tileset;
    
    t_array[0] = 0;
    for (index = 1; index < 10; index++) {
        t_array[index] = index - 1;
    }
    switch (worldn) {
        case 0: {
            w = fopen("world1.dat", "r");
        }
        break;
        case 1: {
            w = fopen("world2.dat", "r");
        }
        break;
        case 2: {
            w = fopen("world3.dat", "r");
        }
        break;
        case 3: {
            w = fopen("world4.dat", "r");
        }
        break;
        case 4: {
            w = fopen("world5.dat", "r");
        }
        break;
        case 5: {
            w = fopen("world6.dat", "r");
        }
        break;
        case 6: {
            w = fopen("world7.dat", "r");
        }
        break;
        case 7: {
            w = fopen("world8.dat", "r");
        }
        break;
        case 8: {
            w = fopen("world9.dat", "r");
        }
        break;
        case 9: {
            w = fopen("world10.dat", "r");
        }
        break;
    }
    
    // loop and create world
    the_world = (world_ptr)&worlds;
    for (y = (NUM_ROWS - 1); y >= 0; y--) {
        the_world->rows[y].y = y * -16;
        tileset = (tile_ptr)&worlds.rows[y].tiles;
        for (x = 0; x < 20; x++) {
        
            // extract element out of file data
            fscanf(w, "%d ", &tileset[x].type);
            element = tileset[x].type;
            tileset[x].there = 0;
            
            // test if this is a live tile
            
            if (element != 0) {
                tileset[x].there = 1;
                tileset[x].x = (x * 16);
            }
        } // end for x
    }// end for y
    fclose(w);
}

void Fill_Double_Buffer_I(void)
{
    fquadset((void far *)double_buffer, 0, 16000);
}

void Play_Action(void)
{
    int done = 0, counter = 0;
    
    num_aliens = 0;
    if (music_enabled)   {
        Music_Play((music_ptr)&game_song, cur_music_track);
    }
    while (!done) {
        Erase_World();
        if ((keys_active > 0) && (player_ship.state == PLAYER_ALIVE)) {
            if (keyboard_state[MAKE_LEFT]) {
                player_ship.x -= players_x_speed;
                if (player_ship.x < 0) {
                    player_ship.x = 0;
                }
            } else if (keyboard_state[MAKE_RIGHT]) {
                player_ship.x += players_x_speed;
                if (player_ship.x > 289) {
                    player_ship.x = 289;
                }
            }
            
            if (keyboard_state[MAKE_UP]) {
                player_ship.y -= players_y_speed;
                if (player_ship.y < 0) {
                    player_ship.y = 0;
                }
            } else if (keyboard_state[MAKE_DOWN]) {
                player_ship.y += players_y_speed;
                if (player_ship.y > 169) {
                    player_ship.y = 169;
                }
            }
            if (keyboard_state[MAKE_SPACE]) {
                Start_Missile(player_ship.x + 7, player_ship.y, player_ship.x + 21, cur_player.missile_type);
            }
            if (keyboard_state[MAKE_M] && keyboard_state[MAKE_J] && keyboard_state[MAKE_R]) {
                cur_player.missile_type = 7;
                num_missiles = 20;
                cur_player.power = 22000;
                players_x_speed = 12;
                players_y_speed = 12;
                bonus_y = 6;
            }
        }
        Move_World();
        Start_Alien();
        Move_Aliens2();
        Move_Missiles();
        Move_Photons();
        Animate_Powerups();
        Animate_Explosions();
        Animate_Pdeath();
        Test_For_Collisions();
        Draw_World();
        Draw_Player_Ship1();
        Draw_Aliens2();
        Draw_Explosions();
        Draw_Powerups();
        Draw_Missiles();
        Draw_Photons();
        Draw_Pdeath();
        Draw_Power_Line();
        Show_Points();
        if (fast_machine) {
            Delay(1);
        }
        Wait_For_Vsync();
        Show_Double_Buffer(double_buffer);
        if (worlds.rows[199].y > 200) {
            done = 1;
        }
        if (wait_state && player_ship.state == PLAYER_ALIVE) {
            if (++counter == 15) {
                counter = 0;
                wait_state = 0;
            }
        }
        if (music_enabled)   {
            if (Music_Status() != SEQUENCE_PLAYING) {
                Music_Play((music_ptr)&game_song, cur_music_track);
            }
        }
        
    }
    Erase_World();
}

void Draw_Aliens2(void)
{

// this function draws the mechs

    int index;
    
// process each mech

    for (index = 0; index < MAX_NUMBER_OF_ALIEN_SHIPS; index++) {
    
        // test if mech is alive
        
        if (alien_array[index].state_1 != SPRITE_DEAD) {
        
            switch (alien_array[index].type) {
            
                case ALIEN_1: { // type one mech
                    alien_1.x = alien_array[index].x;
                    alien_1.y = alien_array[index].y;
                    alien_1.curr_frame = alien_array[index].curr_frame;
                    
                    // test if we should use dying blitter
                    Sprite_Draw_Clip2((sprite_ptr)&alien_1);
                    
                    alien_array[index].state_1 = alien_1.state;
                    alien_1.state = SPRITE_ALIVE;
                    
                }
                break;
                
                case ALIEN_2: { // type two mech
                    alien_2.x = alien_array[index].x;
                    alien_2.y = alien_array[index].y;
                    alien_2.curr_frame = alien_array[index].curr_frame;
                    
                    // test if we should use dying blitter
                    Sprite_Draw_Clip2((sprite_ptr)&alien_2);
                    
                    alien_array[index].state_1 = alien_2.state;
                    alien_2.state = SPRITE_ALIVE;
                    
                }
                break;
                
                default:
                    break;
                    
            } // end switch
            
        } // end if mech dead
        
    } // end for index
    
} // end Draw_Mechs

void Sprite_Draw_Clip2(sprite_ptr sprite)
{

// this function draws a sprite on the screen row by row very quickly
// note the use of shifting to implement multplication
// if the transparent flag is true then pixels wil be draw one by one
// else a memcpy will be used to draw each line
// this function also performs clipping. It will test if the sprite
// is totally visible/invisible and will only draw the portions that are visible

    unsigned char far *sprite_data; // pointer to sprite data
    unsigned char far *dest_buffer; // pointer to destination buffer
    unsigned char far *buffer = double_buffer;
    
    int x, y,                       // looping variables
        sx, sy,                     // position of sprite                      // width of sprite
        bitmap_x      = 0,          // starting upper left corner of sub-bitmap
        bitmap_y      = 0,          // to be drawn after clipping
        bitmap_width  = 0,          // width and height of sub-bitmap
        bitmap_height = 0;
        
    unsigned char pixel;            // the current pixel being processed
    
// alias a variable to sprite size

    bitmap_width  = 31;
    bitmap_height = 31;
    sx            = sprite->x;
    sy            = sprite->y;
    
// perform trivial rejection tests

    if (sx > 320 || sy > 200 ||
            (sx + 31) < 0) {
        // sprite is totally invisible therefore don't draw
        
        // set invisible flag in strucuture so that the erase sub-function
        // doesn't do anything
        sprite->state = SPRITE_DEAD;
        num_aliens--;
        
        return;
        
    } // end if invisible
    
// the sprite needs some clipping or no clipping at all, so compute
// visible portion of sprite rectangle

// first compute upper left hand corner of clipped sprite

    if (sx < 0) {
    
        bitmap_x      = -sx;
        sx            = 0;
        bitmap_width -= bitmap_x;
        
    } // end off left edge
    else if ((sx + 31) > 320) {
    
        bitmap_x      = 0;
        bitmap_width  = 320 - sx;
        
    } // end off right edge
    
// now process y

    if (sy < 0) {
    
        bitmap_y       = -sy;
        sy             = 0;
        bitmap_height -= bitmap_y;
        
    } // end off top edge
    else if ((sy + 31) > 200) {
    
        bitmap_y       = 0;
        bitmap_height  = 200 - sy;
    } // end off lower edge
    
// this point we know were to start drawing the bitmap i.e.
// sx,sy
// and we know were in the data to extract the bitmap i.e.
// bitmap_x, bitmap_y,
// and finaly we know the size of the bitmap to be drawn i.e.
// width,height, so plug it all into the rest of function

// compute number of bytes between adjacent video lines after a row of pixels
// has been drawn

// compute offset of sprite in destination buffer

    dest_buffer = buffer + (sy << 8) + (sy << 6) + sx;
    
// alias a pointer to sprite for ease of access and locate starting sub
// bitmap that will be drawn

    sprite_data = sprite->frames[sprite->curr_frame] + (bitmap_y * 31) + bitmap_x;
    
// copy each line of the sprite data into destination buffer
    for (y = 0; y < bitmap_height; y++) {
        // copy the next row into the destination buffer
        
        for (x = 0; x < bitmap_width; x++) {
        
            // test for transparent pixel i.e. 0, if not transparent then draw
            
            if ((pixel = sprite_data[x])) {
                dest_buffer[x] = pixel;
            }
            
        } // end for x
        
        // move to next line in desintation buffer and sprite image buffer
        
        dest_buffer += 320;
        sprite_data += 31;   // note this width is the actual width of the
        // entire bitmap NOT the visible portion
    } // end for y
// set variables in strucuture so that the erase sub-function can operate
// faster

} // end Sprite_Draw_Clip

void Start_Alien(void)
{
    int index;
    int done = 0;
    int element;
    int goodie = 0;
    
    
    element = ((rand() % 2) + 1);
    if (rand() % 4 == 0) {
        goodie = 1;
    }
    switch (the_dif_setting) {
        case DIF_EASY: {
            if ((rand() % 40 == 1) && (num_aliens < MAX_NUMBER_OF_ALIEN_SHIPS - 1)) {
                for (index = 0; index < MAX_NUMBER_OF_ALIEN_SHIPS; index++) {
                    if (alien_array[index].state_1 == SPRITE_DEAD) {
                        if (element == 1) {
                            alien_array[index].xv           = 3;
                            alien_array[index].yv           = 3;
                        }
                        if (element == 2) {
                            alien_array[index].xv          = 5;
                            alien_array[index].yv          = 5;
                        }
                        alien_array[index].x = rand() % 289;
                        alien_array[index].y = -30;
                        alien_array[index].state_1      = SPRITE_ALIVE;
                        alien_array[index].state_2      = ALIEN_ATTACK;
                        alien_array[index].new_state = 1;
                        alien_array[index].counter_1    = 0;
                        alien_array[index].counter_2    = 0;
                        alien_array[index].aux_1        = 0;
                        alien_array[index].aux_2        = 0;
                        alien_array[index].threshold_1  = 0;
                        alien_array[index].threshold_2  = 64;
                        alien_array[index].direction    = 0;
                        alien_array[index].curr_frame   = 0;
                        if (goodie == 1) {
                            alien_array[index].carry_goodie = 1;
                            alien_array[index].type_of_goodie = Get_Goodie();
                        } else {
                            alien_array[index].carry_goodie = 0;
                            alien_array[index].type_of_goodie = 5;
                        }
                        
                        // set type field
                        
                        alien_array[index].type         = element;
                        
                        // there is one more mech now
                        num_aliens++;
                        done = 1;
                    }
                    if (done) {
                        break;
                    }
                }
            }
        }
        break;
        case DIF_NORMAL: {
            if ((rand() % 20 == 1) && (num_aliens < MAX_NUMBER_OF_ALIEN_SHIPS - 1)) {
                for (index = 0; index < MAX_NUMBER_OF_ALIEN_SHIPS; index++) {
                    if (alien_array[index].state_1 == SPRITE_DEAD) {
                        if (element == 1) {
                            alien_array[index].xv           = 3;
                            alien_array[index].yv           = 3;
                        }
                        if (element == 2) {
                            alien_array[index].xv          = 5;
                            alien_array[index].yv          = 5;
                        }
                        alien_array[index].x = rand() % 289;
                        alien_array[index].y = -30;
                        alien_array[index].state_1      = SPRITE_ALIVE;
                        alien_array[index].state_2      = ALIEN_ATTACK;
                        alien_array[index].new_state = 1;
                        alien_array[index].counter_1    = 0;
                        alien_array[index].counter_2    = 0;
                        alien_array[index].aux_1        = 0;
                        alien_array[index].aux_2        = 0;
                        alien_array[index].threshold_1  = 0;
                        alien_array[index].threshold_2  = 64;
                        alien_array[index].direction    = 0;
                        alien_array[index].curr_frame   = 0;
                        if (goodie == 1) {
                            alien_array[index].carry_goodie = 1;
                            alien_array[index].type_of_goodie = Get_Goodie();
                        } else {
                            alien_array[index].carry_goodie = 0;
                            alien_array[index].type_of_goodie = 5;
                        }
                        
                        // set type field
                        
                        alien_array[index].type         = element;
                        
                        // there is one more mech now
                        num_aliens++;
                        done = 1;
                    }
                    if (done) {
                        break;
                    }
                }
            }
        }
        break;
        case DIF_HARD: {
            if ((rand() % 10 == 1) && (num_aliens < MAX_NUMBER_OF_ALIEN_SHIPS - 1)) {
                for (index = 0; index < MAX_NUMBER_OF_ALIEN_SHIPS; index++) {
                    if (alien_array[index].state_1 == SPRITE_DEAD) {
                        if (element == 1) {
                            alien_array[index].xv           = 3;
                            alien_array[index].yv           = 3;
                        }
                        if (element == 2) {
                            alien_array[index].xv          = 5;
                            alien_array[index].yv          = 5;
                        }
                        alien_array[index].x = rand() % 289;
                        alien_array[index].y = -30;
                        alien_array[index].state_1      = SPRITE_ALIVE;
                        alien_array[index].state_2      = ALIEN_ATTACK;
                        alien_array[index].new_state = 1;
                        alien_array[index].counter_1    = 0;
                        alien_array[index].counter_2    = 0;
                        alien_array[index].aux_1        = 0;
                        alien_array[index].aux_2        = 0;
                        alien_array[index].threshold_1  = 0;
                        alien_array[index].threshold_2  = 64;
                        alien_array[index].direction    = 0;
                        alien_array[index].curr_frame   = 0;
                        if (goodie == 1) {
                            alien_array[index].carry_goodie = 1;
                            alien_array[index].type_of_goodie = Get_Goodie();;
                        } else {
                            alien_array[index].carry_goodie = 0;
                            alien_array[index].type_of_goodie = 5;
                        }
                        
                        // set type field
                        
                        alien_array[index].type         = element;
                        
                        // there is one more mech now
                        
                        num_aliens++;
                        done = 1;
                    }
                    if (done) {
                        break;
                    }
                }
            }
        }
        break;
    }
}

void Move_Aliens2(void)
{
    int move_x, move_y, index, the_frame, curr_direction, mdx, by;
    alien_ptr worker;
    
    for (index = 0; index < MAX_NUMBER_OF_ALIEN_SHIPS; index++) {
        worker = (alien_ptr)&alien_array[index];
        // test if alien is alive
        if (worker->state_1 == SPRITE_ALIVE) {
            switch (worker->state_2) {
                case ALIEN_FLY: {
                    move_y = worker->yv;
                    worker->y += move_y;
                    the_frame = rand() % 2;
                    worker->curr_frame = the_frame;
                    if (rand() % 100 == 1) {
                        worker->state_2    = ALIEN_ATTACK;
                        worker->new_state  = 1;
                    }
                }
                break;
                case ALIEN_ATTACK: {
                    if (worker->new_state) {
                        // reset new state
                        
                        worker->new_state = 0;
                        
                        // select a pattern and reset all vars
                        
                        worker->aux_1 = rand() % NUM_ATTACK_PATTERNS;
                        
                        // use counter 1 as index into pattern table
                        
                        worker->counter_1 = 0;
                        worker->counter_2   = 0;
                        worker->threshold_2 = 2 + rand() % 3;
                    } // end if need to initialize state
                    
                    // else must be continuing state
                    
                    // access current direction
                    
                    curr_direction = attack_patterns[worker->aux_1][worker->counter_1];
                    
                    // test if we are at end of sequence
                    
                    if (curr_direction == -1) {
                        worker->state_2    = ALIEN_FLY;
                        worker->new_state  = 1;
                        break;
                    } // end if at end
                    
                    the_frame = rand() % 2;
                    // extract current frame of animation
                    switch (curr_direction) {
                        case D0: {
                            if (the_frame) {
                                worker->curr_frame = 0;
                            } else {
                                worker->curr_frame = 1;
                            }
                        }
                        break;
                        case L15: {
                            if (the_frame) {
                                worker->curr_frame = 2;
                            } else {
                                worker->curr_frame = 3;
                            }
                        }
                        break;
                        case L30: {
                            if (the_frame) {
                                worker->curr_frame = 4;
                            } else {
                                worker->curr_frame = 5;
                            }
                        }
                        break;
                        case R30: {
                            if (the_frame) {
                                worker->curr_frame = 8;
                            } else {
                                worker->curr_frame = 9;
                            }
                        }
                        break;
                        case R15: {
                            if (the_frame) {
                                worker->curr_frame = 6;
                            } else {
                                worker->curr_frame = 7;
                            }
                        }
                        break;
                        default:
                            break;
                    }
                    
                    // using current direction, compute velocity vector
                    move_x = dirs_x[curr_direction];
                    if (curr_direction == -1) {
                        move_x = 0;
                    }
                    move_y = worker->yv;
                    
                    worker->x += (move_x + bonus_x);
                    worker->y += (move_y + bonus_y);
                    // test if we went too far
                    
                    
                    if (++worker->counter_2 == worker->threshold_2) {
                        worker->counter_2 = 0;
                        worker->counter_1++;
                    } // end if time to switch pattern element
                }
                break;
                default:
                    break;
            }
        }
    }
    for (index = 0; index < MAX_NUMBER_OF_ALIEN_SHIPS; index++) {
        if (alien_array[index].state_1 == SPRITE_ALIVE) {
            worker = (alien_ptr)&alien_array[index];
            
            by = (worker->y + 31);
            if ( (worker->x > (player_ship.x - 70)) &&
                    (worker->x < (player_ship.x + 101)) &&
                    (rand() % 10 == 1) && (by < (player_ship.y - 20))) {
                // compute trajectory
                if (worker->x < player_ship.x - 30) {
                    mdx = +2;
                } else if (worker->x > player_ship.x + 61) {
                    mdx = -2;
                } else {
                    mdx = 0;
                }
            }
            if (cur_player.missile_type > 0) {
                Start_Photon((worker->x + 12), by, mdx, (cur_player.missile_type + 2));
            } else {
                Start_Photon((worker->x + 12), by, mdx, 3);
            }
        }
    }
}

void Init_Powerups(void)
{
    int index;
    
    for (index = 0; index < NUM_OF_POWER_UPS; index++) {
        power_array[index].state = SPRITE_DEAD;
        power_array[index].x = 0;
        power_array[index].y = 0;
        power_array[index].lifetime = 0;
        power_array[index].curr_frame = 0;
        power_array[index].type = 0;
        power_array[index].counter_1 = 0;
        power_array[index].threshold_1 = 0;
    }
}

void Start_Powerup(int x, int y, int type, int lifetime)
{
    int index, done = 0;
    
    for (index = 0; index < NUM_OF_POWER_UPS; index++) {
        if (power_array[index].state == SPRITE_DEAD) {
            power_array[index].state = SPRITE_ALIVE;
            power_array[index].x = x;
            power_array[index].y = y;
            power_array[index].lifetime = lifetime;
            power_array[index].type =  type;
            power_array[index].curr_frame = 0;
            power_array[index].counter_1 = 0;
            done = 1;
        }
        if (done) {
            break;
        }
    }
}

void Draw_Powerups(void)
{
    int index;
    
    for (index = 0; index < NUM_OF_POWER_UPS; index++) {
        if (power_array[index].state == SPRITE_ALIVE) {
            switch (power_array[index].type) {
                case POWERUP_SHIELD: {
                    power_1.x = power_array[index].x;
                    power_1.y = power_array[index].y;
                    power_1.curr_frame = power_array[index].curr_frame;
                    
                    Sprite_Draw_Clip3((sprite_ptr)&power_1);
                }
                break;
                case POWERUP_POINTS: {
                    power_2.x = power_array[index].x;
                    power_2.y = power_array[index].y;
                    power_2.curr_frame =  power_array[index].curr_frame;
                    
                    Sprite_Draw_Clip3((sprite_ptr)&power_2);
                }
                break;
                case POWERUP_BOMB: {
                    power_3.x = power_array[index].x;
                    power_3.y = power_array[index].y;
                    power_3.curr_frame = power_array[index].curr_frame;
                    
                    Sprite_Draw_Clip3((sprite_ptr)&power_3);
                }
                break;
                case POWERUP_WEAPONS: {
                    power_4.x = power_array[index].x;
                    power_4.y = power_array[index].y;
                    power_4.curr_frame =  power_array[index].curr_frame;
                    
                    Sprite_Draw_Clip3((sprite_ptr)&power_4);
                }
                break;
            }
        }
    }
}

void Animate_Powerups(void)
{
    int index;
    
    for (index = 0; index < NUM_OF_POWER_UPS; index++) {
        // test if explosion is active
        
        if (power_array[index].state == SPRITE_ALIVE) {
            // test if it's time to change frames
            
            if (++power_array[index].counter_1 == power_array[index].lifetime) {
                // is the explosion over?
                power_array[index].state = SPRITE_DEAD;
                
            } // end if time to change frames
            else {
                if (++power_array[index].curr_frame > 3) {
                    power_array[index].curr_frame = 0;
                }
            }
            
        } // end if found a good one
        
    } // end for index
    
} // end Animate_Explosions

void Sprite_Draw_Clip3(sprite_ptr sprite)
{

// this function draws a sprite on the screen row by row very quickly
// note the use of shifting to implement multplication
// if the transparent flag is true then pixels wil be draw one by one
// else a memcpy will be used to draw each line
// this function also performs clipping. It will test if the sprite
// is totally visible/invisible and will only draw the portions that are visible

    unsigned char far *sprite_data; // pointer to sprite data
    unsigned char far *dest_buffer; // pointer to destination buffer
    unsigned char far *buffer = double_buffer;
    
    int x, y,                       // looping variables
        sx, sy,                     // position of sprite                      // width of sprite
        bitmap_x      = 0,          // starting upper left corner of sub-bitmap
        bitmap_y      = 0,          // to be drawn after clipping
        bitmap_width  = 0,          // width and height of sub-bitmap
        bitmap_height = 0;
        
    unsigned char pixel;            // the current pixel being processed
    
// alias a variable to sprite size

    bitmap_width  = 17;
    bitmap_height = 17;
    sx            = sprite->x;
    sy            = sprite->y;
    
// perform trivial rejection tests

    if (((sx + 17) < 0) || ((sy + 17) < 0)) {
        // sprite is totally invisible therefore don't draw
        
        // set invisible flag in strucuture so that the erase sub-function
        // doesn't do anything
        sprite->state = SPRITE_DEAD;
        
        return;
        
    } // end if invisible
    
// the sprite needs some clipping or no clipping at all, so compute
// visible portion of sprite rectangle

// first compute upper left hand corner of clipped sprite

    if (sx < 0) {
    
        bitmap_x      = -sx;
        sx            = 0;
        bitmap_width -= bitmap_x;
        
    } // end off left edge
    else if ((sx + 17) > 320) {
    
        bitmap_x      = 0;
        bitmap_width  = 320 - sx;
        
    } // end off right edge
    
// now process y

    if (sy < 0) {
    
        bitmap_y       = -sy;
        sy             = 0;
        bitmap_height -= bitmap_y;
        
    } // end off top edge
    else if ((sy + 17) > 200) {
    
        bitmap_y       = 0;
        bitmap_height  = 200 - sy;
    } // end off lower edge
    
// this point we know were to start drawing the bitmap i.e.
// sx,sy
// and we know were in the data to extract the bitmap i.e.
// bitmap_x, bitmap_y,
// and finaly we know the size of the bitmap to be drawn i.e.
// width,height, so plug it all into the rest of function

// compute number of bytes between adjacent video lines after a row of pixels
// has been drawn

// compute offset of sprite in destination buffer

    dest_buffer = buffer + (sy << 8) + (sy << 6) + sx;
    
// alias a pointer to sprite for ease of access and locate starting sub
// bitmap that will be drawn

    sprite_data = sprite->frames[sprite->curr_frame] + (bitmap_y * 17) + bitmap_x;
    
// copy each line of the sprite data into destination buffer
    for (y = 0; y < bitmap_height; y++) {
        // copy the next row into the destination buffer
        
        for (x = 0; x < bitmap_width; x++) {
        
            // test for transparent pixel i.e. 0, if not transparent then draw
            
            if ((pixel = sprite_data[x])) {
                dest_buffer[x] = pixel;
            }
            
        } // end for x
        
        // move to next line in desintation buffer and sprite image buffer
        
        dest_buffer += 320;
        sprite_data += 17;   // note this width is the actual width of the
        // entire bitmap NOT the visible portion
    } // end for y
// set variables in strucuture so that the erase sub-function can operate
// faster

} // end Sprite_Draw_Clip

int Get_Goodie(void)
{
    int goodie;
    
    goodie = rand() % 20;
    
    if (goodie == 0 || goodie == 1 || goodie == 2 || goodie == 15) {
        goodie = POWERUP_WEAPONS;
        return goodie;
    } else if (goodie == 3 || goodie == 4 || goodie == 5 || goodie == 6 || goodie == 7 || goodie == 8 || goodie == 9) {
        goodie =  POWERUP_POINTS;
        return goodie;
    } else if (goodie == 10 || goodie == 11 || goodie == 12 || goodie == 13 || goodie == 14 || goodie == 16) {
        goodie = POWERUP_SHIELD;
        return goodie;
    } else if (goodie == 17 || goodie == 18 || goodie == 19) {
        goodie = POWERUP_BOMB;
        return goodie;
    }
}

int Sprites_Collide1(sprite_ptr sprite_1, sprite_ptr sprite_2)
{
    int dx, dy, x1, x2, y1, y2, width_1, width_2, height_1, height_2, m1, m2;
    
    if (sprite_1->width == 17) {
        m1 = 3;
    } else if (sprite_1->width == 7) {
        m1 = 1;
    } else {
        m1 = 5;
    }
    m2 = 5;
    
    x1 = (sprite_1->x + m1);
    x2 = (sprite_2->x + m2);
    y1 = (sprite_1->y + m1);
    y2 = (sprite_2->y + m2);
    width_1 = (sprite_1->width - m1);
    width_2 = (sprite_2->width - m2);
    height_1 = (sprite_1->height - m1);
    height_2 = (sprite_2->height - m2);
    
    // this function tests if two sprites have intersected by testing their
    // bounding boxes for collision
    if ((((x1 >= x2) && (x1 < x2 + width_2)) || ((x1 + width_1 >= x2) && (x1 + width_1 < x2 + width_2)))
            && (((y1 >= y2) && (y1 < y2 + height_2)) || ((y1 + height_1 >= y2) && (y1 + height_1 < y2 + height_2)))) {
        return(1);
    } else {
        return(0);
    }
    
} // end Sprites_Collide

void Test_For_Collisions(void)
{
    int index, index1;
    
    for (index = 0; index < NUM_OF_POWER_UPS; index++) {
        if (power_array[index].state == SPRITE_ALIVE) {
            switch (power_array[index].type) {
                case POWERUP_SHIELD: {
                    power_1.x = power_array[index].x;
                    power_1.y = power_array[index].y;
                    
                    if (Sprites_Collide1((sprite_ptr)&power_1, (sprite_ptr)&player_ship) && (player_ship.state == PLAYER_ALIVE)) {
                        FX_Play(POWER_SND, 1);
                        power_array[index].state = SPRITE_DEAD;
                        if ((players_x_speed += 2) > 12) {
                            players_x_speed = 12;
                        }
                        if ((players_y_speed += 2) > 12) {
                            players_y_speed = 12;
                        }
                        cur_player.power = 22000;
                        if (++bonus_y > 6) {
                            bonus_y = 6;
                        }
                    }
                }
                break;
                case POWERUP_POINTS: {
                    power_2.x = power_array[index].x;
                    power_2.y = power_array[index].y;
                    
                    if (Sprites_Collide1((sprite_ptr)&power_2, (sprite_ptr)&player_ship) && (player_ship.state == PLAYER_ALIVE)) {
                        FX_Play(POWER_SND, 1);
                        power_array[index].state = SPRITE_DEAD;
                        cur_player.points += 1000;
                    }
                }
                break;
                case POWERUP_BOMB: {
                    power_3.x = power_array[index].x;
                    power_3.y = power_array[index].y;
                    
                    
                    if (Sprites_Collide1((sprite_ptr)&power_3, (sprite_ptr)&player_ship) && (player_ship.state == PLAYER_ALIVE)) {
                        FX_Play(POWER_SND, 1);
                        power_array[index].state = SPRITE_DEAD;
                        for (index1 = 0; index1 < MAX_NUMBER_OF_ALIEN_SHIPS; index1++) {
                            if (alien_array[index1].state_1 == SPRITE_ALIVE) {
                                alien_array[index1].state_1 = SPRITE_DEAD;
                            }
                        }
                        num_aliens = 0;
                        cur_player.points += 100;
                    }
                }
                break;
                case POWERUP_WEAPONS: {
                    power_4.x = power_array[index].x;
                    power_4.y = power_array[index].y;
                    
                    
                    if (Sprites_Collide1((sprite_ptr)&power_4, (sprite_ptr)&player_ship) && (player_ship.state == PLAYER_ALIVE)) {
                        FX_Play(POWER_SND, 1);
                        power_array[index].state = SPRITE_DEAD;
                        if (++cur_player.missile_type > 7) {
                            cur_player.missile_type = 7;
                        }
                        if ((num_missiles += 2) > NUM_MISSILES) {
                            num_missiles = NUM_MISSILES;
                        }
                    }
                }
                break;
            }
        }
    }
    for (index = 0; index < NUM_PHOTONS; index++) {
        if (photon_array[index].state == SPRITE_ALIVE) {
            alien_missile.x = photon_array[index].x;
            alien_missile.y = photon_array[index].y;
            
            if (Sprites_Collide1((sprite_ptr)&alien_missile, (sprite_ptr)&player_ship) && (player_ship.state == PLAYER_ALIVE) && (!wait_state)) {
                FX_Play(ATTACK_SND, 1);
                photon_array[index].state = SPRITE_DEAD;
                if ((cur_player.points -= 100) <= 0) {
                    cur_player.points = 0;
                }
                if ((cur_player.power -= 2000) == 0) {
                    player_ship.state = PLAYER_DYING;
                    wait_state = 1;
                    cur_player.missile_type = 0;
                    players_x_speed = 4;
                    players_y_speed = 4;
                    cur_player.power = 22000;
                    num_missiles = 6;
                    bonus_x = 0;
                    bonus_y = 0;
                }
            }
        }
    }
    for (index = 0; index < MAX_NUMBER_OF_ALIEN_SHIPS; index++) {
        if (alien_array[index].state_1 == SPRITE_ALIVE) {
            switch (alien_array[index].type) {
                case 1: {
                    alien_1.x = alien_array[index].x;
                    alien_1.y = alien_array[index].y;
                    
                    if (Sprites_Collide1((sprite_ptr)&alien_1, (sprite_ptr)&player_ship) && (player_ship.state == PLAYER_ALIVE) && (!wait_state)) {
                        player_ship.state = PLAYER_DYING;
                        wait_state = 1;
                        alien_array[index].state_1 = SPRITE_DEAD;
                        num_aliens--;
                        Start_Explosion(alien_1.x, alien_1.y, 2);
                        cur_player.power = 22000;
                        if ((cur_player.points -= 2500) <= 0) {
                            cur_player.points = 0;
                        }
                        cur_player.missile_type = 0;
                        players_x_speed = 4;
                        players_y_speed = 4;
                        num_missiles = 6;
                        bonus_x = 0;
                        bonus_y = 0;
                    }
                }
                break;
                case 2: {
                    alien_2.x = alien_array[index].x;
                    alien_2.y = alien_array[index].y;
                    
                    if (Sprites_Collide1((sprite_ptr)&alien_2, (sprite_ptr)&player_ship) && (player_ship.state == PLAYER_ALIVE) && (!wait_state)) {
                        player_ship.state = PLAYER_DYING;
                        alien_array[index].state_1 = SPRITE_DEAD;
                        wait_state = 1;
                        Start_Explosion(alien_2.x, alien_2.y, 2);
                        cur_player.power = 22000;
                        num_aliens--;
                        cur_player.missile_type = 0;
                        if ((cur_player.points -= 2500) <= 0) {
                            cur_player.points = 0;
                        }
                        players_x_speed = 4;
                        players_y_speed = 4;
                        num_missiles = 6;
                        bonus_x = 0;
                        bonus_y = 0;
                    }
                }
                break;
            }
        }
    }
}

void Init_Photons(void)
{
    int index;
    
    for (index = 0; index < NUM_PHOTONS; index++) {
        photon_array[index].state = SPRITE_DEAD;
        photon_array[index].x = 0;
        photon_array[index].y = 0;
        photon_array[index].move_x = 0;
        photon_array[index].move_y = 0;
    }
}

void Start_Photon(int x, int y, int move_x, int move_y)
{
    int index, done = 0;
    
    for (index = 0; index < NUM_PHOTONS; index++) {
        if (photon_array[index].state == SPRITE_DEAD) {
            photon_array[index].state = SPRITE_ALIVE;
            photon_array[index].x = x;
            photon_array[index].y = y;
            photon_array[index].move_x = move_x;
            photon_array[index].move_y = move_y;
            done = 1;
        }
        if (done) {
            break;
        }
    }
}

void Draw_Photons(void)
{
    int index;
    
    for (index = 0; index < NUM_PHOTONS; index++) {
        if (photon_array[index].state == SPRITE_ALIVE) {
            alien_missile.x = photon_array[index].x;
            alien_missile.y = photon_array[index].y;
            alien_missile.curr_frame = 0;
            
            Sprite_Draw_Clip4((sprite_ptr)&alien_missile);
        }
    }
}

void Sprite_Draw_Clip4(sprite_ptr sprite)
{

// this function draws a sprite on the screen row by row very quickly
// note the use of shifting to implement multplication
// if the transparent flag is true then pixels wil be draw one by one
// else a memcpy will be used to draw each line
// this function also performs clipping. It will test if the sprite
// is totally visible/invisible and will only draw the portions that are visible

    unsigned char far *sprite_data; // pointer to sprite data
    unsigned char far *dest_buffer; // pointer to destination buffer
    unsigned char far *buffer = double_buffer;
    
    int x, y,                       // looping variables
        sx, sy,                     // position of sprite                      // width of sprite
        bitmap_x      = 0,          // starting upper left corner of sub-bitmap
        bitmap_y      = 0,          // to be drawn after clipping
        bitmap_width  = 0,          // width and height of sub-bitmap
        bitmap_height = 0;
        
    unsigned char pixel;            // the current pixel being processed
    
// alias a variable to sprite size

    bitmap_width  = 7;
    bitmap_height = 7;
    sx            = sprite->x;
    sy            = sprite->y;
    
// perform trivial rejection tests

    if (((sx + 7) < 0) || (sx > 320) || (sy > 200)) {
        // sprite is totally invisible therefore don't draw
        
        // set invisible flag in strucuture so that the erase sub-function
        // doesn't do anything
        sprite->state = SPRITE_DEAD;
        
        return;
        
    } // end if invisible
    
// the sprite needs some clipping or no clipping at all, so compute
// visible portion of sprite rectangle

// first compute upper left hand corner of clipped sprite

    if (sx < 0) {
    
        bitmap_x      = -sx;
        sx            = 0;
        bitmap_width -= bitmap_x;
        
    } // end off left edge
    else if ((sx + 7) > 320) {
    
        bitmap_x      = 0;
        bitmap_width  = 320 - sx;
        
    } // end off right edge
    
// now process y

    if (sy < 0) {
    
        bitmap_y       = -sy;
        sy             = 0;
        bitmap_height -= bitmap_y;
        
    } // end off top edge
    else if ((sy + 7) > 200) {
    
        bitmap_y       = 0;
        bitmap_height  = 200 - sy;
    } // end off lower edge
    
// this point we know were to start drawing the bitmap i.e.
// sx,sy
// and we know were in the data to extract the bitmap i.e.
// bitmap_x, bitmap_y,
// and finaly we know the size of the bitmap to be drawn i.e.
// width,height, so plug it all into the rest of function

// compute number of bytes between adjacent video lines after a row of pixels
// has been drawn

// compute offset of sprite in destination buffer

    dest_buffer = buffer + (sy << 8) + (sy << 6) + sx;
    
// alias a pointer to sprite for ease of access and locate starting sub
// bitmap that will be drawn

    sprite_data = sprite->frames[sprite->curr_frame] + (bitmap_y * 7) + bitmap_x;
    
// copy each line of the sprite data into destination buffer
    for (y = 0; y < bitmap_height; y++) {
        // copy the next row into the destination buffer
        
        for (x = 0; x < bitmap_width; x++) {
        
            // test for transparent pixel i.e. 0, if not transparent then draw
            
            if ((pixel = sprite_data[x])) {
                dest_buffer[x] = pixel;
            }
            
        } // end for x
        
        // move to next line in desintation buffer and sprite image buffer
        
        dest_buffer += 320;
        sprite_data += 7;   // note this width is the actual width of the
        // entire bitmap NOT the visible portion
    } // end for y
// set variables in strucuture so that the erase sub-function can operate
// faster

} // end Sprite_Draw_Clip

void Move_Photons(void)
{
    int index;
    
    for (index = 0; index < NUM_PHOTONS; index++) {
        if (photon_array[index].state == SPRITE_ALIVE) {
            if (((photon_array[index].x += photon_array[index].move_x) > 320) ||  ((photon_array[index].x += photon_array[index].move_x) < -6)) {
                photon_array[index].state = SPRITE_DEAD;
            }
            if (((photon_array[index].y += photon_array[index].move_y) > 200)) {
                photon_array[index].state = SPRITE_DEAD;
            }
        }
    }
}

void Draw_Player_Ship1(void)
{
    // this variable decides which frame (jets or no jets) is used
    int the_frame;
    static int is_dead1 = 0;
    
    if (player_ship.state == PLAYER_DYING) {
        if (!is_dead1) {
            player_ship.curr_frame = 2;
            FX_Play(((rand() % 2) + 2), 1);
            Start_PDeath();
            is_dead1 = 1;
        }
        Sprite_Draw((sprite_ptr)&player_ship, double_buffer, 1);
        if (++player_ship.curr_frame > 5) {
            player_ship.curr_frame = 2;
        }
    } else if (player_ship.state == PLAYER_ALIVE) {
        is_dead1 = 0;
        the_frame = rand() % 2;
        // set the current frame depending on what the random number was
        if (the_frame == 1) {
            player_ship.curr_frame = 1;
        } else {
            player_ship.curr_frame = 0;
        }
        // finally, scan under the sprite and draw it
        Sprite_Draw((sprite_ptr)&player_ship, double_buffer, 1);
    }
}

void End_Sequence(void)
{
    int index = 0;
    
    if (music_enabled) {
        Music_Play((music_ptr)&game_song, 13);
    }
    PCX_Init((pcx_picture_ptr)&image);
    PCX_Load("end.pcx", (pcx_picture_ptr)&image, 1);
    Screen_From_Black_PCX();
    PCX_Delete((pcx_picture_ptr)&image);
    
    while (index < 20) {
        Blit_String(50, 140, 183, "Design by Matt Reiferson", 1);
        index++;
        Delay(5);
        if (music_enabled) {
            if (Music_Status() != SEQUENCE_PLAYING) {
                Music_Play((music_ptr)&game_song, 13);
            }
        }
    }
    Blit_String(50, 140, 0, "Design by Matt Reiferson", 1);
    Delay(5);
    while (index < 40) {
        Blit_String(50, 140, 183, "Programming by Matt Reiferson", 1);
        index++;
        Delay(5);
        if (music_enabled) {
            if (Music_Status() != SEQUENCE_PLAYING) {
                Music_Play((music_ptr)&game_song, 13);
            }
        }
    }
    Blit_String(50, 140, 0, "Programming by Matt Reiferson", 1);
    Delay(5);
    while (index < 60) {
        Blit_String(50, 140, 183, "Graphics by Matt Reiferson", 1);
        index++;
        Delay(5);
        if (music_enabled) {
            if (Music_Status() != SEQUENCE_PLAYING) {
                Music_Play((music_ptr)&game_song, 13);
            }
        }
    }
    Blit_String(50, 140, 0, "Graphics by Matt Reiferson", 1);
    Delay(5);
    while (index < 80) {
        Blit_String(50, 140, 183, "Sound FX by Matt Reiferson", 1);
        index++;
        Delay(5);
        if (music_enabled) {
            if (Music_Status() != SEQUENCE_PLAYING) {
                Music_Play((music_ptr)&game_song, 13);
            }
        }
    }
    Blit_String(50, 140, 0, "Sound FX by Matt Reiferson", 1);
    Delay(5);
    while (index < 100) {
        Blit_String(50, 140, 183, "Music by Matt, Jesse, & Eric", 1);
        index++;
        Delay(5);
        if (music_enabled) {
            if (Music_Status() != SEQUENCE_PLAYING) {
                Music_Play((music_ptr)&game_song, 13);
            }
        }
    }
    Blit_String(50, 140, 0, "Music by Matt, Jesse, & Eric", 1);
    Delay(5);
    while (index < 120) {
        Blit_String(50, 140, 183, "Special Thanks to Andre LaMothe", 1);
        index++;
        Delay(5);
        if (music_enabled) {
            if (Music_Status() != SEQUENCE_PLAYING) {
                Music_Play((music_ptr)&game_song, 13);
            }
        }
    }
    Blit_String(50, 140, 0, "Special Thanks to Andre LaMothe", 1);
    Delay(5);
    while (index < 140) {
        Blit_String(50, 140, 183, "Dedicated To Nicolas Pesola", 1);
        index++;
        Delay(5);
        if (music_enabled) {
            if (Music_Status() != SEQUENCE_PLAYING) {
                Music_Play((music_ptr)&game_song, 13);
            }
        }
    }
    Blit_String(50, 140, 0, "Dedicated To Nicolas Pesola", 1);
    Delay(10);
    Screen_Transition(SCREEN_DARKNESS);
}

void Fill_Screen(void)
{
    fquadset((void far *)video_buffer, 0, 16000);
}

void blit_string3(int n1, int n2, int color1, int color2, int color3, char *string, int n3)
{
    Blit_String(n1, n2, color1, string, n3);
}
















