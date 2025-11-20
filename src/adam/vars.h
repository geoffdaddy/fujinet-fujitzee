#ifdef __ADAM__

#ifndef KEYMAP_H
#define KEYMAP_H

// Screen dimensions for platform

#define WIDTH 32
#define HEIGHT 24

// Other platform specific constnats

#define ROLL_SOUND_MOD 2 // How often to play roll sound
#define ROLL_FRAMES 16 // How many roll frames to play
//#define SCORE_CURSOR_ALT 2 // Alternate score cursor color
#define BOTTOM_HEIGHT 3 // How high the bottom panel is
#define SCORES_X 2 // X start of scoreboard
#define GAMEOVER_PROMPT_Y HEIGHT-2
#define ROLL_X WIDTH-24
#define TIMER_X 5
#define TIMER_NUM_OFFSET_X 2
#define TIMER_NUM_OFFSET_Y 1
#define ONLINE_HELP 1

/**
 * Platform specific key map for common input
 */

 /**
 * ADAM keyboard mapping
 */
// #define KEY_BACKSPACE    0x08
// #define KEY_TAB          0x09
// #define KEY_RETURN       0x0D
// #define KEY_ESCAPE       0x1B
// #define KEY_SPACE        0x20
// #define KEY_1            0x31
// #define KEY_2            0x32
// #define KEY_3            0x33
// #define KEY_4            0x34
// #define KEY_5            0x35
// #define KEY_6            0x36
// #define KEY_7            0x37
// #define KEY_8            0x38
// #define KEY_HOME         0x80
// #define KEY_SMART_I      0x81
// #define KEY_SMART_II     0x82
// #define KEY_SMART_III    0x83
// #define KEY_SMART_IV     0x84
// #define KEY_SMART_V      0x85
// #define KEY_SMART_VI     0x86
// #define KEY_WILD_CARD    0x90
// #define KEY_UNDO         0x91
// #define KEY_MOVE         0x9A
// #define KEY_GET          0x93
// #define KEY_INSERT       0x94
// #define KEY_PRINT        0x95
// #define KEY_CLEAR        0x96
// #define KEY_DELETE       0x97
// #define KEY_COPY         0x92
// #define KEY_STORE        0x9B
// #define KEY_S_INSERT     0x9C
// #define KEY_S_PRINT      0x9D
// #define KEY_S_CLEAR      0x9E
// #define KEY_S_DELETE     0x9F

#define KEY_LEFT_ARROW      0xA3
#define KEY_LEFT_ARROW_2    0xAB
#define KEY_LEFT_ARROW_3    0xA7 // ,

#define KEY_RIGHT_ARROW     0xA1
#define KEY_RIGHT_ARROW_2   0xA9
#define KEY_RIGHT_ARROW_3   0xA5 // .

#define KEY_UP_ARROW        0xA0
#define KEY_UP_ARROW_2      0xA4
#define KEY_UP_ARROW_3      0xA8 // -

#define KEY_DOWN_ARROW      0xA4
#define KEY_DOWN_ARROW_2    0xA6
#define KEY_DOWN_ARROW_3    0xAA // =

#define KEY_RETURN       0x0D
#define KEY_ESCAPE       0x1B
#define KEY_ESCAPE_ALT   0x1B
#define KEY_SPACEBAR     0x20
#define KEY_BACKSPACE    0x08

#define CHAR_CURSOR     0x8B

/*
  Mapping for converting incoming ALT letters to a standard case
*/

#define LINE_ENDING 0x0A
#define ALT_LETTER_START 0x61
#define ALT_LETTER_END 0x7A
#define ALT_LETTER_AND 0x5f

// Icons
#define ICON_MARK         0x8C
#define ICON_MARK_ALT     0x5B
#define ICON_PLAYER       0x8A
#define ICON_CURSOR       0x8D
#define ICON_SPEC         0x8C
#define ICON_TEXT_CURSOR  0x8D
#define ICON_CURSOR_ALT   0x8E
#define ICON_CURSOR_BLIP  0x8F


#define QUERY_SUFFIX "" // No extra params for Adam

#endif /* KEYMAP_H */

#endif /* __ADAM__ */