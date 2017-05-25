/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WINDOW_INPUTSTRING_H
#define __WINDOW_INPUTSTRING_H

#ifdef __cplusplus
extern "C" {
#endif

/* Include system header files -----------------------------------------------*/
#include <string.h>

/* Include user header files -------------------------------------------------*/
#include "UserCommon.h"

// FreeRTOS
#include "semphr.h"

/* Private macro -------------------------------------------------------------*/
/* ---------------------------------------------------------------- */
/* -- User setting Configuration                                 -- */
/* ---------------------------------------------------------------- */
#define INPUTSTRING_WINDOW_TITLE            "Input string"
#define uGUI_INPUTSTRING_OBJECTS_NUM        45
#define INPUTSTRING_UPDATE_MS               50
#define INPUTSTRING_BUTTON_GAP_PIXEL        2
#define INPUTSTRING_SIZE                    (26+1)          // "+1" is for null-terminated string
#define INPUTSTRING_TITLE_FONT_SIZE         FONT_10X16
#define INPUTSTRING_BUTTON_FONT_SIZE        FONT_12X16
#define INPUTSTRING_TXTBOX_FONT_SIZE        FONT_16X26
#define INPUTSTRING_TXTBOX_RANGE_FONT_SIZE  FONT_6X10


/* Exported types ------------------------------------------------------------*/
/* Exported enum tag ---------------------------------------------------------*/
/* Exported struct/union tag -------------------------------------------------*/
typedef struct
{
    bool StartUpEvent;
    bool *p_isCompleteInFuture;
    bool **pp_isCompleteInFuture;
    char InputString[INPUTSTRING_SIZE];
} _InpStr_t;

// sample
/* static _InpStr_t sample_t = {false, NULL, &(sample_t.p_isCompleteInFuture), {'\0'} }; */

/* Exported variables --------------------------------------------------------*/
// FreeRTOS
TaskHandle_t xHandle_InputString;

/* Exported function prototypes ----------------------------------------------*/
void createInputStringWindow(void);
bool inputString_InBackground(_InpStr_t *);

#ifdef __cplusplus
}
#endif

#endif /* __WINDOW_INPUTSTRING_H */
/***************************************************************END OF FILE****/
