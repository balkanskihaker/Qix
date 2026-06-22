#pragma once

const int WINDOW_WIDTH  = 800;
const int WINDOW_HEIGHT = 600;
const char* const WINDOW_TITLE = "Qix";

const int BORDER_X = 100;
const int BORDER_Y = 60;

const int BORDER_W = WINDOW_WIDTH  - 2 * BORDER_X;
const int BORDER_H = WINDOW_HEIGHT - 2 * BORDER_Y;

const int TARGET_FPS = 60;

const int STIX_SPEED_BORDER = 2;
const int STIX_SPEED_SLOW   = 1;
const int STIX_SPEED_FAST   = 3;

const int SPARX_SPEED = 2;
const float QIX_SPEED = 1.5f;

// ============================================================
// PALETA BOJA — korigovane za CRT multiply blend filter
// BASE = TARGET / CRT  (CRT: R=0.566, G=0.712, B=0.811)
// ============================================================

// Zuta  #faeb2c  -> target (250,235,44)
const float COL_YELLOW_R = 1.0000f;
const float COL_YELLOW_G = 1.0000f;
const float COL_YELLOW_B = 0.2128f;

// Pink  #f52789  -> target (245,39,137)
const float COL_PINK_R = 1.0000f;
const float COL_PINK_G = 0.2148f;
const float COL_PINK_B = 0.6625f;

// Magenta  #e900ff  -> target (233,0,255)
const float COL_MAGENTA_R = 1.0000f;
const float COL_MAGENTA_G = 0.0000f;
const float COL_MAGENTA_B = 1.0000f;

// Plava  #1685f8  -> target (22,133,248)
const float COL_BLUE_R = 0.1524f;
const float COL_BLUE_G = 0.7325f;
const float COL_BLUE_B = 1.0000f;

// Tamno ljubicasta  #612875c8  -> target (61,20,76)
const float COL_DARK_R = 0.4526f;
const float COL_DARK_G = 0.1102f;
const float COL_DARK_B = 0.3675f;

// Bela (za tekst i border liniju)
const float COL_WHITE_R = 1.0000f;
const float COL_WHITE_G = 1.0000f;
const float COL_WHITE_B = 1.0000f;

// ============================================================
// DODELA BOJA ELEMENTIMA
// ============================================================

// Background
#define BG_R COL_DARK_R
#define BG_G COL_DARK_G
#define BG_B COL_DARK_B

// Border and wall
#define BORDER_COL_R COL_BLUE_R
#define BORDER_COL_G COL_BLUE_G
#define BORDER_COL_B COL_BLUE_B

// Claimed area
#define CLAIMED_COL_R COL_DARK_R + 0.2f
#define CLAIMED_COL_G COL_DARK_G + 0.2f 
#define CLAIMED_COL_B COL_DARK_B + 0.2f

// Stix
#define STIX_COL_R COL_WHITE_R
#define STIX_COL_G COL_WHITE_G
#define STIX_COL_B COL_WHITE_B

// Stix slow draw
#define STIX_SLOW_R COL_YELLOW_R
#define STIX_SLOW_G COL_YELLOW_G
#define STIX_SLOW_B COL_YELLOW_B

// Stix fast draw
#define STIX_FAST_R COL_PINK_R
#define STIX_FAST_G COL_PINK_G
#define STIX_FAST_B COL_PINK_B

// Stix burning line
#define STIX_BURN_R 1.0000f
#define STIX_BURN_G 0.0000f
#define STIX_BURN_B 0.0000f

// Stix burnt line
#define STIX_BURNED_R 0.3f
#define STIX_BURNED_G 0.3f
#define STIX_BURNED_B 0.3f

// Sparx
#define SPARX_COL_R COL_YELLOW_R
#define SPARX_COL_G COL_YELLOW_G
#define SPARX_COL_B COL_YELLOW_B

// Qix body
#define QIX_COL_R COL_MAGENTA_R
#define QIX_COL_G COL_MAGENTA_G
#define QIX_COL_B COL_MAGENTA_B

// Qix tail
#define QIX_TAIL_R COL_PINK_R
#define QIX_TAIL_G COL_PINK_G
#define QIX_TAIL_B COL_PINK_B

// HUD text
#define HUD_COL_R COL_WHITE_R
#define HUD_COL_G COL_WHITE_G
#define HUD_COL_B COL_WHITE_B

// HUD %
#define HUD_PCT_R COL_YELLOW_R
#define HUD_PCT_G COL_YELLOW_G
#define HUD_PCT_B COL_YELLOW_B

// Messages
#define MSG_COL_R COL_YELLOW_R
#define MSG_COL_G COL_YELLOW_G
#define MSG_COL_B COL_YELLOW_B

// Menu title
#define MENU_TITLE_R COL_MAGENTA_R
#define MENU_TITLE_G COL_MAGENTA_G
#define MENU_TITLE_B COL_MAGENTA_B

// Menu selected option
#define MENU_SEL_R COL_YELLOW_R
#define MENU_SEL_G COL_YELLOW_G
#define MENU_SEL_B COL_YELLOW_B

// Menu non-selected option
#define MENU_UNSEL_R 0.7f
#define MENU_UNSEL_G 0.7f
#define MENU_UNSEL_B 0.7f
