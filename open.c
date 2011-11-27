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

void Opening_Movie(void);

layer mountains,
      grass1,
      water,
      grass2,
      grass3,
      grass4;

sound speech[10];
extern sound_enabled;
extern music_enabled;
extern game_song;
extern in_windows;

pcx_picture scroll_image;
int mount_x = 0,
    grass1_x = 0,
    water_x = 0,
    grass2_x = 0,
    grass3_x = 0,
    grass4_x = 0,
    index, index1, si = 1, already_playing = 0;

void Opening_Movie(void)
{
    PCX_Init((pcx_picture_ptr)&scroll_image);
    PCX_Load("icymount.pcx", (pcx_picture_ptr)&scroll_image, 1);
    PCX_Copy_To_Buffer((pcx_picture_ptr)&scroll_image, double_buffer);
    PCX_Delete((pcx_picture_ptr)&scroll_image);
    Layer_Create((layer_ptr)&mountains, 320, 69);
    Layer_Create((layer_ptr)&grass1, 320, 5);
    Layer_Create((layer_ptr)&water, 320, 8);
    Layer_Create((layer_ptr)&grass2, 320, 9);
    Layer_Create((layer_ptr)&grass3, 320, 16);
    Layer_Create((layer_ptr)&grass4, 320, 12);
    Layer_Build((layer_ptr)&mountains, 0, 0, double_buffer, 0, 65, 320, 69);
    Layer_Build((layer_ptr)&grass1, 0, 0, double_buffer, 0, 134, 320, 5);
    Layer_Build((layer_ptr)&water, 0, 0, double_buffer, 0, 139, 320, 8);
    Layer_Build((layer_ptr)&grass2, 0, 0, double_buffer, 0, 147, 320, 9);
    Layer_Build((layer_ptr)&grass3, 0, 0, double_buffer, 0, 156, 320, 16);
    Layer_Build((layer_ptr)&grass4, 0, 0, double_buffer, 0, 172, 320, 12);
    Fill_Double_Buffer(156);
    for (index = 0; keys_active == 0; index++) {
        if (index == 0) {
            PCX_Init((pcx_picture_ptr)&scroll_image);
            PCX_Load("icymount.pcx", (pcx_picture_ptr)&scroll_image, 1);
            PCX_Delete((pcx_picture_ptr)&scroll_image);
            if (sound_enabled) {
                Sound_Load("speech1a.voc", (sound_ptr)&speech[0], 1);
                Sound_Load("speech1b.voc", (sound_ptr)&speech[1], 1);
                Sound_Load("speech2a.voc", (sound_ptr)&speech[2], 1);
                Sound_Load("speech2b.voc", (sound_ptr)&speech[3], 1);
                Sound_Load("speech3a.voc", (sound_ptr)&speech[4], 1);
                Sound_Load("speech3b.voc", (sound_ptr)&speech[5], 1);
                Sound_Load("speech3c.voc", (sound_ptr)&speech[6], 1);
                Sound_Load("speech4a.voc", (sound_ptr)&speech[7], 1);
                Sound_Load("speech4b.voc", (sound_ptr)&speech[8], 1);
            }
            if (music_enabled) {
                Music_Play((music_ptr)&game_song, 3);
            }
        }
        if (sound_enabled) {
            if (sound_enabled) {
                Sound_Play((sound_ptr)&speech[si - 1]);
            }
            while (Sound_Status() && keys_active == 0) {
                if ((mount_x += 1) > 319) {
                    mount_x -= 320;
                }
                if ((grass1_x += 2) > 319) {
                    grass1_x -= 320;
                }
                if ((water_x += 4) > 319) {
                    water_x -= 320;
                }
                if ((grass2_x += 8) > 319) {
                    grass2_x -= 320;
                }
                if ((grass3_x += 14) > 319) {
                    grass3_x -= 320;
                }
                if ((grass4_x += 24) > 319) {
                    grass4_x -= 320;
                }
                Layer_Draw((layer_ptr)&mountains, mount_x, 0, double_buffer, 65 + 16, 70, 0);
                Layer_Draw((layer_ptr)&grass1, grass1_x, 0, double_buffer, 134 + 16, 5, 0);
                Layer_Draw((layer_ptr)&water, water_x, 0, double_buffer, 139 + 16, 8, 0);
                Layer_Draw((layer_ptr)&grass2, grass2_x, 0, double_buffer, 147 + 16, 9, 0);
                Layer_Draw((layer_ptr)&grass3, grass3_x, 0, double_buffer, 156 + 16, 16, 0);
                Layer_Draw((layer_ptr)&grass4, grass4_x, 0, double_buffer, 172 + 16, 12, 0);
                Blit_String_DB(5, 5, 5, "Alliance Control To Math Warrior.", 1);
                Show_Double_Buffer(double_buffer);
                Delay(1);
                if (keys_active > 0) {
                    return(0);
                }
            }
            si = 2;
            for (index1 = 4; index1 < 14; index1++) {
                Bline_DB(0, index1, 320, index1, 156);
            }
            
            if (sound_enabled) {
                Sound_Play((sound_ptr)&speech[si - 1]);
            }
            while (Sound_Status() && keys_active == 0) {
                if ((mount_x += 1) > 319) {
                    mount_x -= 320;
                }
                if ((grass1_x += 2) > 319) {
                    grass1_x -= 320;
                }
                if ((water_x += 4) > 319) {
                    water_x -= 320;
                }
                if ((grass2_x += 8) > 319) {
                    grass2_x -= 320;
                }
                if ((grass3_x += 14) > 319) {
                    grass3_x -= 320;
                }
                if ((grass4_x += 24) > 319) {
                    grass4_x -= 320;
                }
                Layer_Draw((layer_ptr)&mountains, mount_x, 0, double_buffer, 65 + 16, 70, 0);
                Layer_Draw((layer_ptr)&grass1, grass1_x, 0, double_buffer, 134 + 16, 5, 0);
                Layer_Draw((layer_ptr)&water, water_x, 0, double_buffer, 139 + 16, 8, 0);
                Layer_Draw((layer_ptr)&grass2, grass2_x, 0, double_buffer, 147 + 16, 9, 0);
                Layer_Draw((layer_ptr)&grass3, grass3_x, 0, double_buffer, 156 + 16, 16, 0);
                Layer_Draw((layer_ptr)&grass4, grass4_x, 0, double_buffer, 172 + 16, 12, 0);
                Blit_String_DB(5, 5, 5, "Please Respond.", 1);
                Show_Double_Buffer(double_buffer);
                Delay(1);
                if (keys_active > 0) {
                    return(0);
                }
            }
            si = 3;
            for (index1 = 4; index1 < 14; index1++) {
                Bline_DB(0, index1, 320, index1, 156);
            }
            if (sound_enabled) {
                Sound_Play((sound_ptr)&speech[si - 1]);
            }
            while (Sound_Status() && keys_active == 0) {
                if ((mount_x += 1) > 319) {
                    mount_x -= 320;
                }
                if ((grass1_x += 2) > 319) {
                    grass1_x -= 320;
                }
                if ((water_x += 4) > 319) {
                    water_x -= 320;
                }
                if ((grass2_x += 8) > 319) {
                    grass2_x -= 320;
                }
                if ((grass3_x += 14) > 319) {
                    grass3_x -= 320;
                }
                if ((grass4_x += 24) > 319) {
                    grass4_x -= 320;
                }
                Layer_Draw((layer_ptr)&mountains, mount_x, 0, double_buffer, 65 + 16, 70, 0);
                Layer_Draw((layer_ptr)&grass1, grass1_x, 0, double_buffer, 134 + 16, 5, 0);
                Layer_Draw((layer_ptr)&water, water_x, 0, double_buffer, 139 + 16, 8, 0);
                Layer_Draw((layer_ptr)&grass2, grass2_x, 0, double_buffer, 147 + 16, 9, 0);
                Layer_Draw((layer_ptr)&grass3, grass3_x, 0, double_buffer, 156 + 16, 16, 0);
                Layer_Draw((layer_ptr)&grass4, grass4_x, 0, double_buffer, 172 + 16, 12, 0);
                Blit_String_DB(5, 5, 5, "I Here Ya.", 1);
                Show_Double_Buffer(double_buffer);
                Delay(1);
                if (keys_active > 0) {
                    return(0);
                }
            }
            si = 4;
            for (index1 = 4; index1 < 14; index1++) {
                Bline_DB(0, index1, 320, index1, 156);
            }
            
            if (sound_enabled) {
                Sound_Play((sound_ptr)&speech[si - 1]);
            }
            while (Sound_Status() && keys_active == 0) {
                if ((mount_x += 1) > 319) {
                    mount_x -= 320;
                }
                if ((grass1_x += 2) > 319) {
                    grass1_x -= 320;
                }
                if ((water_x += 4) > 319) {
                    water_x -= 320;
                }
                if ((grass2_x += 8) > 319) {
                    grass2_x -= 320;
                }
                if ((grass3_x += 14) > 319) {
                    grass3_x -= 320;
                }
                if ((grass4_x += 24) > 319) {
                    grass4_x -= 320;
                }
                Layer_Draw((layer_ptr)&mountains, mount_x, 0, double_buffer, 65 + 16, 70, 0);
                Layer_Draw((layer_ptr)&grass1, grass1_x, 0, double_buffer, 134 + 16, 5, 0);
                Layer_Draw((layer_ptr)&water, water_x, 0, double_buffer, 139 + 16, 8, 0);
                Layer_Draw((layer_ptr)&grass2, grass2_x, 0, double_buffer, 147 + 16, 9, 0);
                Layer_Draw((layer_ptr)&grass3, grass3_x, 0, double_buffer, 156 + 16, 16, 0);
                Layer_Draw((layer_ptr)&grass4, grass4_x, 0, double_buffer, 172 + 16, 12, 0);
                Blit_String_DB(5, 5, 5, "Whats Going On.", 1);
                Show_Double_Buffer(double_buffer);
                Delay(1);
                if (keys_active > 0) {
                    return(0);
                }
            }
            si = 5;
            for (index1 = 4; index1 < 14; index1++) {
                Bline_DB(0, index1, 320, index1, 156);
            }
            
            if (sound_enabled) {
                Sound_Play((sound_ptr)&speech[si - 1]);
            }
            while (Sound_Status() && keys_active == 0) {
                if ((mount_x += 1) > 319) {
                    mount_x -= 320;
                }
                if ((grass1_x += 2) > 319) {
                    grass1_x -= 320;
                }
                if ((water_x += 4) > 319) {
                    water_x -= 320;
                }
                if ((grass2_x += 8) > 319) {
                    grass2_x -= 320;
                }
                if ((grass3_x += 14) > 319) {
                    grass3_x -= 320;
                }
                if ((grass4_x += 24) > 319) {
                    grass4_x -= 320;
                }
                Layer_Draw((layer_ptr)&mountains, mount_x, 0, double_buffer, 65 + 16, 70, 0);
                Layer_Draw((layer_ptr)&grass1, grass1_x, 0, double_buffer, 134 + 16, 5, 0);
                Layer_Draw((layer_ptr)&water, water_x, 0, double_buffer, 139 + 16, 8, 0);
                Layer_Draw((layer_ptr)&grass2, grass2_x, 0, double_buffer, 147 + 16, 9, 0);
                Layer_Draw((layer_ptr)&grass3, grass3_x, 0, double_buffer, 156 + 16, 16, 0);
                Layer_Draw((layer_ptr)&grass4, grass4_x, 0, double_buffer, 172 + 16, 12, 0);
                Blit_String_DB(5, 5, 5, "You Must Save The Alliance.", 1);
                Show_Double_Buffer(double_buffer);
                Delay(1);
                if (keys_active > 0) {
                    return(0);
                }
            }
            si = 6;
            for (index1 = 4; index1 < 14; index1++) {
                Bline_DB(0, index1, 320, index1, 156);
            }
            
            if (sound_enabled) {
                Sound_Play((sound_ptr)&speech[si - 1]);
            }
            while (Sound_Status() && keys_active == 0) {
                if ((mount_x += 1) > 319) {
                    mount_x -= 320;
                }
                if ((grass1_x += 2) > 319) {
                    grass1_x -= 320;
                }
                if ((water_x += 4) > 319) {
                    water_x -= 320;
                }
                if ((grass2_x += 8) > 319) {
                    grass2_x -= 320;
                }
                if ((grass3_x += 14) > 319) {
                    grass3_x -= 320;
                }
                if ((grass4_x += 24) > 319) {
                    grass4_x -= 320;
                }
                Layer_Draw((layer_ptr)&mountains, mount_x, 0, double_buffer, 65 + 16, 70, 0);
                Layer_Draw((layer_ptr)&grass1, grass1_x, 0, double_buffer, 134 + 16, 5, 0);
                Layer_Draw((layer_ptr)&water, water_x, 0, double_buffer, 139 + 16, 8, 0);
                Layer_Draw((layer_ptr)&grass2, grass2_x, 0, double_buffer, 147 + 16, 9, 0);
                Layer_Draw((layer_ptr)&grass3, grass3_x, 0, double_buffer, 156 + 16, 16, 0);
                Layer_Draw((layer_ptr)&grass4, grass4_x, 0, double_buffer, 172 + 16, 12, 0);
                Blit_String_DB(5, 5, 5, "Before They Destroy Us All.", 1);
                Show_Double_Buffer(double_buffer);
                Delay(1);
                if (keys_active > 0) {
                    return(0);
                }
            }
            si = 7;
            for (index1 = 4; index1 < 14; index1++) {
                Bline_DB(0, index1, 320, index1, 156);
            }
            
            if (sound_enabled) {
                Sound_Play((sound_ptr)&speech[si - 1]);
            }
            while (Sound_Status() && keys_active == 0) {
                if ((mount_x += 1) > 319) {
                    mount_x -= 320;
                }
                if ((grass1_x += 2) > 319) {
                    grass1_x -= 320;
                }
                if ((water_x += 4) > 319) {
                    water_x -= 320;
                }
                if ((grass2_x += 8) > 319) {
                    grass2_x -= 320;
                }
                if ((grass3_x += 14) > 319) {
                    grass3_x -= 320;
                }
                if ((grass4_x += 24) > 319) {
                    grass4_x -= 320;
                }
                Layer_Draw((layer_ptr)&mountains, mount_x, 0, double_buffer, 65 + 16, 70, 0);
                Layer_Draw((layer_ptr)&grass1, grass1_x, 0, double_buffer, 134 + 16, 5, 0);
                Layer_Draw((layer_ptr)&water, water_x, 0, double_buffer, 139 + 16, 8, 0);
                Layer_Draw((layer_ptr)&grass2, grass2_x, 0, double_buffer, 147 + 16, 9, 0);
                Layer_Draw((layer_ptr)&grass3, grass3_x, 0, double_buffer, 156 + 16, 16, 0);
                Layer_Draw((layer_ptr)&grass4, grass4_x, 0, double_buffer, 172 + 16, 12, 0);
                Blit_String_DB(5, 5, 5, "(GUNSHOT) AAAAAHHH!!", 1);
                Show_Double_Buffer(double_buffer);
                Delay(1);
                if (keys_active > 0) {
                    return(0);
                }
            }
            si = 8;
            for (index1 = 4; index1 < 14; index1++) {
                Bline_DB(0, index1, 320, index1, 156);
            }
            
            if (sound_enabled) {
                Sound_Play((sound_ptr)&speech[si - 1]);
            }
            while (Sound_Status() && keys_active == 0) {
                if ((mount_x += 1) > 319) {
                    mount_x -= 320;
                }
                if ((grass1_x += 2) > 319) {
                    grass1_x -= 320;
                }
                if ((water_x += 4) > 319) {
                    water_x -= 320;
                }
                if ((grass2_x += 8) > 319) {
                    grass2_x -= 320;
                }
                if ((grass3_x += 14) > 319) {
                    grass3_x -= 320;
                }
                if ((grass4_x += 24) > 319) {
                    grass4_x -= 320;
                }
                Layer_Draw((layer_ptr)&mountains, mount_x, 0, double_buffer, 65 + 16, 70, 0);
                Layer_Draw((layer_ptr)&grass1, grass1_x, 0, double_buffer, 134 + 16, 5, 0);
                Layer_Draw((layer_ptr)&water, water_x, 0, double_buffer, 139 + 16, 8, 0);
                Layer_Draw((layer_ptr)&grass2, grass2_x, 0, double_buffer, 147 + 16, 9, 0);
                Layer_Draw((layer_ptr)&grass3, grass3_x, 0, double_buffer, 156 + 16, 16, 0);
                Layer_Draw((layer_ptr)&grass4, grass4_x, 0, double_buffer, 172 + 16, 12, 0);
                Blit_String_DB(5, 5, 5, "COMMANDER?", 1);
                Show_Double_Buffer(double_buffer);
                Delay(1);
                if (keys_active > 0) {
                    return(0);
                }
            }
            si = 9;
            for (index1 = 4; index1 < 14; index1++) {
                Bline_DB(0, index1, 320, index1, 156);
            }
            
            if (sound_enabled) {
                Sound_Play((sound_ptr)&speech[si - 1]);
            }
            while (Sound_Status() && keys_active == 0) {
                if ((mount_x += 1) > 319) {
                    mount_x -= 320;
                }
                if ((grass1_x += 2) > 319) {
                    grass1_x -= 320;
                }
                if ((water_x += 4) > 319) {
                    water_x -= 320;
                }
                if ((grass2_x += 8) > 319) {
                    grass2_x -= 320;
                }
                if ((grass3_x += 14) > 319) {
                    grass3_x -= 320;
                }
                if ((grass4_x += 24) > 319) {
                    grass4_x -= 320;
                }
                Layer_Draw((layer_ptr)&mountains, mount_x, 0, double_buffer, 65 + 16, 70, 0);
                Layer_Draw((layer_ptr)&grass1, grass1_x, 0, double_buffer, 134 + 16, 5, 0);
                Layer_Draw((layer_ptr)&water, water_x, 0, double_buffer, 139 + 16, 8, 0);
                Layer_Draw((layer_ptr)&grass2, grass2_x, 0, double_buffer, 147 + 16, 9, 0);
                Layer_Draw((layer_ptr)&grass3, grass3_x, 0, double_buffer, 156 + 16, 16, 0);
                Layer_Draw((layer_ptr)&grass4, grass4_x, 0, double_buffer, 172 + 16, 12, 0);
                Blit_String_DB(5, 5, 5, "I Will Try My Best.", 1);
                Show_Double_Buffer(double_buffer);
                Delay(1);
                if (keys_active > 0) {
                    return(0);
                }
            }
        } else {
            for (index1 = 0; index1 < 20; index1++) {
                if ((mount_x += 1) > 319) {
                    mount_x -= 320;
                }
                if ((grass1_x += 2) > 319) {
                    grass1_x -= 320;
                }
                if ((water_x += 4) > 319) {
                    water_x -= 320;
                }
                if ((grass2_x += 8) > 319) {
                    grass2_x -= 320;
                }
                if ((grass3_x += 14) > 319) {
                    grass3_x -= 320;
                }
                if ((grass4_x += 24) > 319) {
                    grass4_x -= 320;
                }
                Layer_Draw((layer_ptr)&mountains, mount_x, 0, double_buffer, 65 + 16, 70, 0);
                Layer_Draw((layer_ptr)&grass1, grass1_x, 0, double_buffer, 134 + 16, 5, 0);
                Layer_Draw((layer_ptr)&water, water_x, 0, double_buffer, 139 + 16, 8, 0);
                Layer_Draw((layer_ptr)&grass2, grass2_x, 0, double_buffer, 147 + 16, 9, 0);
                Layer_Draw((layer_ptr)&grass3, grass3_x, 0, double_buffer, 156 + 16, 16, 0);
                Layer_Draw((layer_ptr)&grass4, grass4_x, 0, double_buffer, 172 + 16, 12, 0);
                Blit_String_DB(5, 5, 5, "Alliance Control To Math Warrior.", 1);
                Show_Double_Buffer(double_buffer);
                Delay(1);
                if (keys_active > 0) {
                    return(0);
                }
            }
            for (index1 = 4; index1 < 14; index1++) {
                Bline_DB(0, index1, 320, index1, 156);
            }
            
            for (index1 = 0; index1 < 20; index1++) {
                if ((mount_x += 1) > 319) {
                    mount_x -= 320;
                }
                if ((grass1_x += 2) > 319) {
                    grass1_x -= 320;
                }
                if ((water_x += 4) > 319) {
                    water_x -= 320;
                }
                if ((grass2_x += 8) > 319) {
                    grass2_x -= 320;
                }
                if ((grass3_x += 14) > 319) {
                    grass3_x -= 320;
                }
                if ((grass4_x += 24) > 319) {
                    grass4_x -= 320;
                }
                Layer_Draw((layer_ptr)&mountains, mount_x, 0, double_buffer, 65 + 16, 70, 0);
                Layer_Draw((layer_ptr)&grass1, grass1_x, 0, double_buffer, 134 + 16, 5, 0);
                Layer_Draw((layer_ptr)&water, water_x, 0, double_buffer, 139 + 16, 8, 0);
                Layer_Draw((layer_ptr)&grass2, grass2_x, 0, double_buffer, 147 + 16, 9, 0);
                Layer_Draw((layer_ptr)&grass3, grass3_x, 0, double_buffer, 156 + 16, 16, 0);
                Layer_Draw((layer_ptr)&grass4, grass4_x, 0, double_buffer, 172 + 16, 12, 0);
                Blit_String_DB(5, 5, 5, "Please Respond.", 1);
                Show_Double_Buffer(double_buffer);
                Delay(1);
                if (keys_active > 0) {
                    return(0);
                }
            }
            for (index1 = 4; index1 < 14; index1++) {
                Bline_DB(0, index1, 320, index1, 156);
            }
            for (index = 0; index1 < 20; index1++) {
                if ((mount_x += 1) > 319) {
                    mount_x -= 320;
                }
                if ((grass1_x += 2) > 319) {
                    grass1_x -= 320;
                }
                if ((water_x += 4) > 319) {
                    water_x -= 320;
                }
                if ((grass2_x += 8) > 319) {
                    grass2_x -= 320;
                }
                if ((grass3_x += 14) > 319) {
                    grass3_x -= 320;
                }
                if ((grass4_x += 24) > 319) {
                    grass4_x -= 320;
                }
                Layer_Draw((layer_ptr)&mountains, mount_x, 0, double_buffer, 65 + 16, 70, 0);
                Layer_Draw((layer_ptr)&grass1, grass1_x, 0, double_buffer, 134 + 16, 5, 0);
                Layer_Draw((layer_ptr)&water, water_x, 0, double_buffer, 139 + 16, 8, 0);
                Layer_Draw((layer_ptr)&grass2, grass2_x, 0, double_buffer, 147 + 16, 9, 0);
                Layer_Draw((layer_ptr)&grass3, grass3_x, 0, double_buffer, 156 + 16, 16, 0);
                Layer_Draw((layer_ptr)&grass4, grass4_x, 0, double_buffer, 172 + 16, 12, 0);
                Blit_String_DB(5, 5, 5, "I Here Ya.", 1);
                Show_Double_Buffer(double_buffer);
                Delay(1);
                if (keys_active > 0) {
                    return(0);
                }
            }
            for (index1 = 4; index1 < 14; index1++) {
                Bline_DB(0, index1, 320, index1, 156);
            }
            
            for (index1 = 0; index1 < 20; index1++) {
                if ((mount_x += 1) > 319) {
                    mount_x -= 320;
                }
                if ((grass1_x += 2) > 319) {
                    grass1_x -= 320;
                }
                if ((water_x += 4) > 319) {
                    water_x -= 320;
                }
                if ((grass2_x += 8) > 319) {
                    grass2_x -= 320;
                }
                if ((grass3_x += 14) > 319) {
                    grass3_x -= 320;
                }
                if ((grass4_x += 24) > 319) {
                    grass4_x -= 320;
                }
                Layer_Draw((layer_ptr)&mountains, mount_x, 0, double_buffer, 65 + 16, 70, 0);
                Layer_Draw((layer_ptr)&grass1, grass1_x, 0, double_buffer, 134 + 16, 5, 0);
                Layer_Draw((layer_ptr)&water, water_x, 0, double_buffer, 139 + 16, 8, 0);
                Layer_Draw((layer_ptr)&grass2, grass2_x, 0, double_buffer, 147 + 16, 9, 0);
                Layer_Draw((layer_ptr)&grass3, grass3_x, 0, double_buffer, 156 + 16, 16, 0);
                Layer_Draw((layer_ptr)&grass4, grass4_x, 0, double_buffer, 172 + 16, 12, 0);
                Blit_String_DB(5, 5, 5, "Whats Going On.", 1);
                Show_Double_Buffer(double_buffer);
                Delay(1);
                if (keys_active > 0) {
                    return(0);
                }
            }
            for (index1 = 4; index1 < 14; index1++) {
                Bline_DB(0, index1, 320, index1, 156);
            }
            
            for (index1 = 0; index1 < 20; index1++) {
                if ((mount_x += 1) > 319) {
                    mount_x -= 320;
                }
                if ((grass1_x += 2) > 319) {
                    grass1_x -= 320;
                }
                if ((water_x += 4) > 319) {
                    water_x -= 320;
                }
                if ((grass2_x += 8) > 319) {
                    grass2_x -= 320;
                }
                if ((grass3_x += 14) > 319) {
                    grass3_x -= 320;
                }
                if ((grass4_x += 24) > 319) {
                    grass4_x -= 320;
                }
                Layer_Draw((layer_ptr)&mountains, mount_x, 0, double_buffer, 65 + 16, 70, 0);
                Layer_Draw((layer_ptr)&grass1, grass1_x, 0, double_buffer, 134 + 16, 5, 0);
                Layer_Draw((layer_ptr)&water, water_x, 0, double_buffer, 139 + 16, 8, 0);
                Layer_Draw((layer_ptr)&grass2, grass2_x, 0, double_buffer, 147 + 16, 9, 0);
                Layer_Draw((layer_ptr)&grass3, grass3_x, 0, double_buffer, 156 + 16, 16, 0);
                Layer_Draw((layer_ptr)&grass4, grass4_x, 0, double_buffer, 172 + 16, 12, 0);
                Blit_String_DB(5, 5, 5, "You Must Save The Alliance.", 1);
                Show_Double_Buffer(double_buffer);
                Delay(1);
                if (keys_active > 0) {
                    return(0);
                }
            }
            for (index1 = 4; index1 < 14; index1++) {
                Bline_DB(0, index1, 320, index1, 156);
            }
            
            for (index1 = 0; index1 < 20; index1++) {
                if ((mount_x += 1) > 319) {
                    mount_x -= 320;
                }
                if ((grass1_x += 2) > 319) {
                    grass1_x -= 320;
                }
                if ((water_x += 4) > 319) {
                    water_x -= 320;
                }
                if ((grass2_x += 8) > 319) {
                    grass2_x -= 320;
                }
                if ((grass3_x += 14) > 319) {
                    grass3_x -= 320;
                }
                if ((grass4_x += 24) > 319) {
                    grass4_x -= 320;
                }
                Layer_Draw((layer_ptr)&mountains, mount_x, 0, double_buffer, 65 + 16, 70, 0);
                Layer_Draw((layer_ptr)&grass1, grass1_x, 0, double_buffer, 134 + 16, 5, 0);
                Layer_Draw((layer_ptr)&water, water_x, 0, double_buffer, 139 + 16, 8, 0);
                Layer_Draw((layer_ptr)&grass2, grass2_x, 0, double_buffer, 147 + 16, 9, 0);
                Layer_Draw((layer_ptr)&grass3, grass3_x, 0, double_buffer, 156 + 16, 16, 0);
                Layer_Draw((layer_ptr)&grass4, grass4_x, 0, double_buffer, 172 + 16, 12, 0);
                Blit_String_DB(5, 5, 5, "Before They Destroy Us All.", 1);
                Show_Double_Buffer(double_buffer);
                Delay(1);
                if (keys_active > 0) {
                    return(0);
                }
            }
            si = 7;
            for (index1 = 4; index1 < 14; index1++) {
                Bline_DB(0, index1, 320, index1, 156);
            }
            
            for (index1 = 0; index1 < 20; index1++) {
                if ((mount_x += 1) > 319) {
                    mount_x -= 320;
                }
                if ((grass1_x += 2) > 319) {
                    grass1_x -= 320;
                }
                if ((water_x += 4) > 319) {
                    water_x -= 320;
                }
                if ((grass2_x += 8) > 319) {
                    grass2_x -= 320;
                }
                if ((grass3_x += 14) > 319) {
                    grass3_x -= 320;
                }
                if ((grass4_x += 24) > 319) {
                    grass4_x -= 320;
                }
                Layer_Draw((layer_ptr)&mountains, mount_x, 0, double_buffer, 65 + 16, 70, 0);
                Layer_Draw((layer_ptr)&grass1, grass1_x, 0, double_buffer, 134 + 16, 5, 0);
                Layer_Draw((layer_ptr)&water, water_x, 0, double_buffer, 139 + 16, 8, 0);
                Layer_Draw((layer_ptr)&grass2, grass2_x, 0, double_buffer, 147 + 16, 9, 0);
                Layer_Draw((layer_ptr)&grass3, grass3_x, 0, double_buffer, 156 + 16, 16, 0);
                Layer_Draw((layer_ptr)&grass4, grass4_x, 0, double_buffer, 172 + 16, 12, 0);
                Blit_String_DB(5, 5, 5, "(GUNSHOT) AAAAAHHH!!", 1);
                Show_Double_Buffer(double_buffer);
                Delay(1);
                if (keys_active > 0) {
                    return(0);
                }
            }
            for (index1 = 4; index1 < 14; index1++) {
                Bline_DB(0, index1, 320, index1, 156);
            }
            
            for (index1 = 0; index1 < 20; index1++) {
                if ((mount_x += 1) > 319) {
                    mount_x -= 320;
                }
                if ((grass1_x += 2) > 319) {
                    grass1_x -= 320;
                }
                if ((water_x += 4) > 319) {
                    water_x -= 320;
                }
                if ((grass2_x += 8) > 319) {
                    grass2_x -= 320;
                }
                if ((grass3_x += 14) > 319) {
                    grass3_x -= 320;
                }
                if ((grass4_x += 24) > 319) {
                    grass4_x -= 320;
                }
                Layer_Draw((layer_ptr)&mountains, mount_x, 0, double_buffer, 65 + 16, 70, 0);
                Layer_Draw((layer_ptr)&grass1, grass1_x, 0, double_buffer, 134 + 16, 5, 0);
                Layer_Draw((layer_ptr)&water, water_x, 0, double_buffer, 139 + 16, 8, 0);
                Layer_Draw((layer_ptr)&grass2, grass2_x, 0, double_buffer, 147 + 16, 9, 0);
                Layer_Draw((layer_ptr)&grass3, grass3_x, 0, double_buffer, 156 + 16, 16, 0);
                Layer_Draw((layer_ptr)&grass4, grass4_x, 0, double_buffer, 172 + 16, 12, 0);
                Blit_String_DB(5, 5, 5, "COMMANDER?", 1);
                Show_Double_Buffer(double_buffer);
                Delay(1);
                if (keys_active > 0) {
                    return(0);
                }
            }
            for (index1 = 4; index1 < 14; index1++) {
                Bline_DB(0, index1, 320, index1, 156);
            }
            for (index1 = 0; index1 < 20; index1++) {
                if ((mount_x += 1) > 319) {
                    mount_x -= 320;
                }
                if ((grass1_x += 2) > 319) {
                    grass1_x -= 320;
                }
                if ((water_x += 4) > 319) {
                    water_x -= 320;
                }
                if ((grass2_x += 8) > 319) {
                    grass2_x -= 320;
                }
                if ((grass3_x += 14) > 319) {
                    grass3_x -= 320;
                }
                if ((grass4_x += 24) > 319) {
                    grass4_x -= 320;
                }
                Layer_Draw((layer_ptr)&mountains, mount_x, 0, double_buffer, 65 + 16, 70, 0);
                Layer_Draw((layer_ptr)&grass1, grass1_x, 0, double_buffer, 134 + 16, 5, 0);
                Layer_Draw((layer_ptr)&water, water_x, 0, double_buffer, 139 + 16, 8, 0);
                Layer_Draw((layer_ptr)&grass2, grass2_x, 0, double_buffer, 147 + 16, 9, 0);
                Layer_Draw((layer_ptr)&grass3, grass3_x, 0, double_buffer, 156 + 16, 16, 0);
                Layer_Draw((layer_ptr)&grass4, grass4_x, 0, double_buffer, 172 + 16, 12, 0);
                Blit_String_DB(5, 5, 5, "I Will Try My Best.", 1);
                Show_Double_Buffer(double_buffer);
                Delay(1);
                if (keys_active > 0) {
                    return(0);
                }
            }
        }
        break;
    }
    Layer_Delete((layer_ptr)&mountains);
    Layer_Delete((layer_ptr)&grass1);
    Layer_Delete((layer_ptr)&water);
    Layer_Delete((layer_ptr)&grass2);
    Layer_Delete((layer_ptr)&grass3);
    Layer_Delete((layer_ptr)&grass4);
    if (sound_enabled) {
        Sound_Stop();
        for (index = 0; index < 9; index++) {
            Sound_Unload((sound_ptr)&speech[index]);
        }
    }
    if (music_enabled) {
        Music_Stop();
    }
}


