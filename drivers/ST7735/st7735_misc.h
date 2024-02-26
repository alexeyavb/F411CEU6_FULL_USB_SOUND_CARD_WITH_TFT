#ifndef _ST7735_MISC_H_

#define _ST7735_MISC_H_
#include <stdint.h>
#include <stdbool.h>
#include "bsp_misc.h"
//============= Top status bar
#define TOP_X 0U
#define TOP_Y 0U 
#define TOP_FONT_HEIGHT 10U
#define TOP_FONT_WIDTH 7U
#define TOP_BORDER_TOTAL 2U
#define TOP_MESSAGES_X  (TOP_X + ((uint16_t)(TOP_BORDER_TOTAL/2)))
#define TOP_MESSAGES_Y  (TOP_Y + ((uint16_t)(TOP_BORDER_TOTAL/2)))
#define TOP_HEIGHT ((TOP_FONT_HEIGHT) + (TOP_BORDER_TOTAL))
#define TOP_MESSAGES_MAX_SYMB_DEFINED 5U
#define TOP_MESSAGES_WIDTH (uint16_t)(((TOP_MESSAGES_MAX_SYMB_DEFINED) * (TOP_FONT_WIDTH)) + (uint16_t)((TOP_BORDER_TOTAL)/2))
// fsFrq leds
#define FS_REQ_WIDTH (uint16_t)((TOP_FONT_WIDTH)*3) + (uint16_t)((TOP_BORDER_TOTAL)/2U)
#define FS_STARTER_X_POS (uint16_t)((TOP_X) + (TOP_MESSAGES_WIDTH))
#define FS_44K_X    ((FS_STARTER_X_POS) + 1U)
#define FS_48K_X    ((FS_44K_X) + (FS_REQ_WIDTH) + (1U))
#define FS_96K_X    ((FS_48K_X) + (FS_REQ_WIDTH) + (1U))
#define INACTIVE_ST_7735_BG_CLR  color565(100,100,100)
#define INACTIVE_ST_7735_FG_CLR  color565(0x0A,0x0A,0x0A)
// status top field
#define FS_STATUS_X    ((FS_96K_X) + (FS_REQ_WIDTH) + (1U))
#define FS_STATUS_ST_7735_BG_CLR  color565(20U,100U,20U)
#define FS_STATUS_ST_7735_FG_CLR  color565(50U,230U,70U)

//============= Log window
#define LOG_WINDOW_MAX_HEIGHT (uint16_t)(((ST7735_HEIGHT) - (TOP_HEIGHT))/(2U))
#define LOG_WINDOW_MAX_WIDTH (uint16_t)(((ST7735_WIDTH) - 1))
#define LOG_WINDOW_TIMEOUT  (18000U)
// Init coords
#define LOG_WINDOW_Y (LOG_WINDOW_MAX_HEIGHT)
#define LOG_WINDOW_X (TOP_MESSAGES_X)
#define LOG_WINDOW_WIDTH (LOG_WINDOW_MAX_WIDTH)
#define LOG_WINDOW_BOTTOM ((ST7735_HEIGHT) - (LOG_WINDOW_Y))
#define LOG_WINDOW_BG_CLR  color565(130U,130U,130U)
#define LOG_WINDOW_HEAD_CLR color565(240U,140U,50U)

// Log string clolrs
#define LOG_MSG_NRM_CLR color565(10U,200U,10U)
#define LOG_MSG_WRN_CLR color565(230U,100U,20U)
#define LOG_MSG_ERR_CLR color565(200U,10U,10U)
#define LOG_MSG_DBG_CLR color565(7U,27U,7U)

// PEAK Indicators
// Horizontal
#define HRZ_PEAK_X ((TOP_X) + 1U)
#define HRZ_PEAK_Y ((TOP_Y) + (TOP_HEIGHT) + 4U)
#define HRZ_PEAK_MAX_WIDTH ((ST7735_WIDTH) - 1U)

#define PEAK_POINT_H (6U)
#define PEAK_POINT_W (2U)

#define PEAK_L_X_START (HRZ_PEAK_X)
#define PEAK_R_X_START (HRZ_PEAK_X)

#define PEAK_Y_OFFSET 4U
#define PEAK_X_OFFSET 1U
#define PEAK_L_Y_START ((HRZ_PEAK_Y) + (PEAK_Y_OFFSET))
#define PEAK_R_Y_START ((PEAK_L_Y_START) + (PEAK_POINT_H) + (PEAK_Y_OFFSET))

#define PEAK_INITIAL_MAX_DB_VAL (10000654.012345678901f)
#define PEAK_NORNAL_CLR color565(00,200,00)
#define PEAK_MIDDLE_CLR color565(200,150,00)
#define PEAK_LAST_CLR color565(230,00,00)
#define PEAK_INACTIVE_CLR color565(20,50,20)
#define LAST_PEAK_POINT_CLR color565(154,154,154)

#define PEAKS_ON_ONE_LINE   (uint16_t)((HRZ_PEAK_MAX_WIDTH)/((PEAK_POINT_W)+(PEAK_X_OFFSET)))
#define PEAKS_ON_NORMAL     (uint16_t)((uint16_t)(((PEAKS_ON_ONE_LINE)/3U))*2U)
#define PEAKS_ON_MIDDLE     (uint16_t)(((uint16_t)((PEAKS_ON_ONE_LINE) - (PEAKS_ON_NORMAL))/3U)*2U)

#define PEAKS_ON_LAST       (PEAKS_ON_ONE_LINE) - ((PEAKS_ON_NORMAL) + (PEAKS_ON_MIDDLE)) + 1

// #define PEAK_ON_ONE_DB (uint32_t)((PEAK_MAX_DB_VAL)/(PEAKS_ON_ONE_LINE))
// #define PEAK_ON_ONE_DB (float)((PEAK_MAX_DB_VAL)/(PEAKS_ON_ONE_LINE))
#define PEAK_ON_ONE_DB (float)((PEAK_INITIAL_MAX_DB_VAL)/(PEAKS_ON_ONE_LINE))
#define HOW_PEAK_DRAW_FOR_VAL( x ) (uint32_t)(((float)(x))/(PEAK_ON_ONE_DB))
#define DRAW_SINGLE_PEAK( peak_x, peak_y, peak_color)  ST7735_FillRectangleFast( (peak_x), (peak_y), (PEAK_POINT_W), (PEAK_POINT_H), (peak_color) )

#define DRAW_PEAKS(xstart,ystart,peakcount){\
    uint16_t curx = 0, nPeakStartPos = 0;\
    uint16_t nPeak;\
    for(nPeak = nPeakStartPos; nPeak < (peakcount); nPeak++){\
        curx = nPeak*((PEAK_POINT_W)+(PEAK_X_OFFSET));\
        if(nPeak < (PEAKS_ON_NORMAL)){\
            /*printMsg("p:%d,cnt:%d", (PEAKS_ON_NORMAL), nPeak);*/\
            DRAW_SINGLE_PEAK(curx, ystart, (PEAK_NORNAL_CLR));\
        }\
        else{\
            if(nPeak >= (PEAKS_ON_NORMAL) && nPeak < ((PEAKS_ON_NORMAL) + (PEAKS_ON_MIDDLE))){\
                DRAW_SINGLE_PEAK(curx, ystart, (PEAK_MIDDLE_CLR));\
            }\
            else{\
                DRAW_SINGLE_PEAK(curx, ystart, (PEAK_LAST_CLR));\
            }\
        }\
    }\
    \
    for(uint16_t blPick = nPeak; blPick <= (PEAKS_ON_ONE_LINE); blPick++){\
        curx = blPick*((PEAK_POINT_W)+(PEAK_X_OFFSET));\
        DRAW_SINGLE_PEAK(curx, ystart, (PEAK_INACTIVE_CLR));\
    }\
    if((nPeak) >= (PEAKS_ON_NORMAL)-7){\
        nPeak+=1; curx = nPeak*((PEAK_POINT_W)+(PEAK_X_OFFSET)); curx = MIN(curx,((HRZ_PEAK_MAX_WIDTH)-2));\
        DRAW_SINGLE_PEAK(curx,ystart, (LAST_PEAK_POINT_CLR));\
    }\
}

#define DRAW_PEAKS_INDICATORS_BY_VALUE(LPeakValue,RPeakValue){\
    uint16_t LPeaksDraw = HOW_PEAK_DRAW_FOR_VAL(LPeakValue);\
    uint16_t RPeaksDraw = HOW_PEAK_DRAW_FOR_VAL(RPeakValue);\
    /*ST7735_FillRectangleFast((HRZ_PEAK_X), (HRZ_PEAK_Y), (HRZ_PEAK_MAX_WIDTH), ((PEAK_Y_OFFSET) + (2*((PEAK_POINT_H) + (PEAK_Y_OFFSET)))),ST7735_BLACK);*/\
    DRAW_PEAKS(PEAK_L_X_START, PEAK_L_Y_START, (LPeaksDraw-RPeaksDraw > 3) ? (((uint16_t)((((LPeaksDraw+RPeaksDraw)/2)-12)))):RPeaksDraw);\
    DRAW_PEAKS(PEAK_R_X_START, PEAK_R_Y_START, RPeaksDraw);\
    /*HAL_Delay(3);*/\
}
// Horizontal end
// PEAK END

//Log window -----------------------------
#define DRAW_LOG_RECTANGLE() (ST7735_FillRectangleFast(LOG_WINDOW_X, LOG_WINDOW_Y, LOG_WINDOW_WIDTH, LOG_WINDOW_BOTTOM, LOG_WINDOW_BG_CLR))
#define DRAW_LOG_HEADER() (ST7735_FillRectangleFast(LOG_WINDOW_X, LOG_WINDOW_Y-TOP_BORDER_TOTAL, LOG_WINDOW_WIDTH, LOG_WINDOW_Y, LOG_WINDOW_HEAD_CLR))
#define DRAW_LOG_WINDOW(){\
    extern bool flLogWindowOpened;\
    if(!flLogWindowOpened){\
        extern uint32_t LogWindowOpenedTimeStart;\
        DRAW_LOG_HEADER();\
        DRAW_LOG_RECTANGLE();\
        flLogWindowOpened = true;\
        LogWindowOpenedTimeStart = HAL_GetTick();\
    }\
}
#define CLOSE_LOG_WINDOW(){\
    extern bool flLogWindowOpened;\
    if(flLogWindowOpened){\
        ST7735_FillRectangleFast((LOG_WINDOW_X-1), ((LOG_WINDOW_Y)-(TOP_BORDER_TOTAL)-1), (LOG_WINDOW_WIDTH+1), (LOG_WINDOW_BOTTOM+1), (color565(0U,0U,0U)));\
    }\
}

#define DRAW_LOG_MESSAGE(str, MSG_COLOR, current_log_y) { \
    extern uint32_t LogWindowOpenedTimeStart; \
    current_log_y += ((current_log_y == 0) ? (LOG_WINDOW_Y + 1U) : (0U)); \
    ST7735_FillRectangleFast(LOG_WINDOW_X + 1U, current_log_y + 2U, 4U, 4U, MSG_COLOR); \
    ST7735_WriteString(LOG_WINDOW_X + 1U + 4U + 1U, current_log_y, str, Font_7x10, MSG_COLOR, LOG_WINDOW_BG_CLR);\
    LogWindowOpenedTimeStart = HAL_GetTick();\
    current_log_y += (TOP_FONT_HEIGHT + 1U); \
    current_log_y = (current_log_y >= ST7735_HEIGHT) ? (0U) : (current_log_y); \
    if(current_log_y == 0U){ DRAW_LOG_RECTANGLE();} \
}

#define ST7735M_MAX(a,b) ((a) < (b) ? (b) : (a))

//===================================================
extern bool _44k_state, _48k_state, _96k_state;
extern uint16_t current_log_y;

void InitTop(void);

void BSP_LED_On(Led_TypeDef Led);
void BSP_LED_Off(Led_TypeDef Led);
void BSP_LED_Toggle(Led_TypeDef Led);
void BSP_SetLedState(Led_TypeDef Led, bool state);
void BSP_LED_Playing(const char* pstatus);


#endif