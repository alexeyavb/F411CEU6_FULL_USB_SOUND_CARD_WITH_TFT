#include "st7735_misc.h"
#include "st7735.h"
#include "fonts.h"

uint16_t current_log_y;
bool _44k_state, _48k_state, _96k_state;
bool flLogWindowOpened;
uint32_t LogWindowOpenedTimeStart;

void InitTop(void){
    _44k_state = false; _48k_state = false; _96k_state = false;
    current_log_y = 0U;

    ST7735_FillRectangleFast(TOP_X,TOP_Y, ST7735_WIDTH, TOP_HEIGHT+1U, ST7735_BLUE);
    ST7735_WriteString(TOP_MESSAGES_X, TOP_MESSAGES_Y+1U, "24bit", Font_7x10, ST7735_WHITE, ST7735_BLUE);

    ST7735_FillRectangleFast(FS_44K_X, TOP_Y+1U, FS_REQ_WIDTH, TOP_HEIGHT-1, ST7735_WHITE);
    ST7735_FillRectangleFast(FS_48K_X, TOP_Y+1U, FS_REQ_WIDTH, TOP_HEIGHT-1, ST7735_WHITE);
    ST7735_FillRectangleFast(FS_96K_X, TOP_Y+1U, FS_REQ_WIDTH, TOP_HEIGHT-1, ST7735_WHITE);
    BSP_SetLedState(LED_RED, !_96k_state); BSP_SetLedState(LED_GREEN,!_48k_state); BSP_SetLedState(LED_BLUE,!_44k_state);
    
}

void DrawTop_44K_B(bool isActive){ 
    if(_44k_state == isActive)
        return;
    uint16_t ST_7735_BG_CLR = ST7735_BLUE;
    uint16_t ST_7735_FG_CLR = ST7735_WHITE;
    if(!isActive){
        ST_7735_BG_CLR = INACTIVE_ST_7735_BG_CLR;
        ST_7735_FG_CLR = INACTIVE_ST_7735_FG_CLR;        
    }
    ST7735_FillRectangleFast(FS_44K_X, TOP_Y+1U, FS_REQ_WIDTH, TOP_HEIGHT-1U, ST_7735_BG_CLR);
    ST7735_WriteString(FS_44K_X+1U, TOP_MESSAGES_Y+1, "44K", Font_7x10, ST_7735_FG_CLR, ST_7735_BG_CLR);
    _44k_state = isActive;
}

void DrawTop_48K_G(bool isActive){ 
    if(_48k_state == isActive)
        return;
    uint16_t ST_7735_BG_CLR = ST7735_GREEN;
    uint16_t ST_7735_FG_CLR = ST7735_WHITE;
    if(!isActive){
        ST_7735_BG_CLR = INACTIVE_ST_7735_BG_CLR;
        ST_7735_FG_CLR = INACTIVE_ST_7735_FG_CLR;        
    }
    ST7735_FillRectangleFast(FS_48K_X, TOP_Y+1, FS_REQ_WIDTH, TOP_HEIGHT-1U, ST_7735_BG_CLR);
    ST7735_WriteString(FS_48K_X+1, TOP_MESSAGES_Y+1U, "48K", Font_7x10, ST_7735_FG_CLR, ST_7735_BG_CLR);
    _48k_state = isActive;
}

void DrawTop_96K_R(bool isActive){
    if(_96k_state == isActive)
        return;
    uint16_t ST_7735_BG_CLR = ST7735_RED;
    uint16_t ST_7735_FG_CLR = ST7735_WHITE;
    if(!isActive){
        ST_7735_BG_CLR = INACTIVE_ST_7735_BG_CLR;
        ST_7735_FG_CLR = INACTIVE_ST_7735_FG_CLR;        
    }
    ST7735_FillRectangleFast(FS_96K_X, TOP_Y+1U, FS_REQ_WIDTH, TOP_HEIGHT-1U, ST_7735_BG_CLR);
    ST7735_WriteString(FS_96K_X+1U, TOP_MESSAGES_Y+1U, "96K", Font_7x10, ST_7735_FG_CLR, ST_7735_BG_CLR);
    _96k_state = isActive;
}

void BSP_SetLedState(Led_TypeDef Led, bool state){
    switch (Led){
        case LED_BLUE:
            DrawTop_44K_B(state);
            break;
        case LED_GREEN:
            DrawTop_48K_G(state);
            break;
        case LED_RED:
            DrawTop_96K_R(state);
            break;
        default:
            break;
    }
}


void BSP_LED_On(Led_TypeDef Led) {
    BSP_SetLedState(Led, true);
}

void BSP_LED_Off(Led_TypeDef Led){
    BSP_SetLedState(Led, false);
}

void BSP_LED_Toggle(Led_TypeDef Led){
    bool newstate = false;
    switch (Led){
        case LED_GREEN:            
            newstate = (!_44k_state);
            break;
        case LED_BLUE:
            newstate = (!_48k_state);
            break;
        case LED_RED:
            newstate = (!_96k_state);
            break;
        default:
            break;
    }
    BSP_SetLedState(Led, newstate);
}

void BSP_LED_Playing(const char* pstatus){
    ST7735_WriteString(FS_STATUS_X, TOP_MESSAGES_Y+1, pstatus, Font_7x10, FS_STATUS_ST_7735_BG_CLR, FS_STATUS_ST_7735_FG_CLR);
}

void DrawPeaksIndicator(float LVal, float RVal){
  DRAW_PEAKS_INDICATORS_BY_VALUE(LVal, RVal);
}

