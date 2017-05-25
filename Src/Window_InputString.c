/* Include system header files -----------------------------------------------*/
/* Include user header files -------------------------------------------------*/
#include "Window_InputString.h"

/* Imported variables --------------------------------------------------------*/
/* Private function macro ----------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private enum tag ----------------------------------------------------------*/
static enum BTN_ID_TAG
{
	BTN_ID_Numeral_0 = BTN_ID_0,
    BTN_ID_Numeral_1,
    BTN_ID_Numeral_2,
    BTN_ID_Numeral_3,
    BTN_ID_Numeral_4,
    BTN_ID_Numeral_5,
    BTN_ID_Numeral_6,
    BTN_ID_Numeral_7,
    BTN_ID_Numeral_8,
    BTN_ID_Numeral_9,
    BTN_ID_Alphabet_A,
    BTN_ID_Alphabet_B,
    BTN_ID_Alphabet_C,
    BTN_ID_Alphabet_D,
    BTN_ID_Alphabet_E,
    BTN_ID_Alphabet_F,
    BTN_ID_Alphabet_G,
    BTN_ID_Alphabet_H,
    BTN_ID_Alphabet_I,
    BTN_ID_Alphabet_J,
    BTN_ID_Alphabet_K,
    BTN_ID_Alphabet_L,
    BTN_ID_Alphabet_M,
    BTN_ID_Alphabet_N,
    BTN_ID_Alphabet_O,
    BTN_ID_Alphabet_P,
    BTN_ID_Alphabet_Q,
    BTN_ID_Alphabet_R,
    BTN_ID_Alphabet_S,
    BTN_ID_Alphabet_T,
    BTN_ID_Alphabet_U,
    BTN_ID_Alphabet_V,
    BTN_ID_Alphabet_W,
    BTN_ID_Alphabet_X,
    BTN_ID_Alphabet_Y,
    BTN_ID_Alphabet_Z,
    BTN_ID_Symbol_Shift,
    BTN_ID_Symbol_Space,
    BTN_ID_Symbol_Underscore,
    BTN_ID_Symbol_Hyphen,
    BTN_ID_OK,
    BTN_ID_Cancel,
    BTN_ID_Clear,
    BTN_ID_BackSpace
};

static enum TXB_ID_TAG
{
    TXB_ID_DisplayString = TXB_ID_0
};

/* Private variables ---------------------------------------------------------*/
/* uGUI */
static UG_WINDOW wnd_InputString;

/* "this" pointer */
static TaskHandle_t* pthis_xHandle = &xHandle_InputString;
static UG_WINDOW* pthis_wnd = &wnd_InputString;

/* thread control */
static bool needFinalize;  // This flag is used in "WindowControlThread" and "window_callback" function
static bool isCompletedInputString = false;
static SemaphoreHandle_t xSemaphore_InputString;

/* Objects of uGUI */
static UG_OBJECT obj_this_wnd[uGUI_INPUTSTRING_OBJECTS_NUM];
static UG_BUTTON btn_Numeral_0;
static UG_BUTTON btn_Numeral_1;
static UG_BUTTON btn_Numeral_2;
static UG_BUTTON btn_Numeral_3;
static UG_BUTTON btn_Numeral_4;
static UG_BUTTON btn_Numeral_5;
static UG_BUTTON btn_Numeral_6;
static UG_BUTTON btn_Numeral_7;
static UG_BUTTON btn_Numeral_8;
static UG_BUTTON btn_Numeral_9;
static UG_BUTTON btn_Alphabet_A;
static UG_BUTTON btn_Alphabet_B;
static UG_BUTTON btn_Alphabet_C;
static UG_BUTTON btn_Alphabet_D;
static UG_BUTTON btn_Alphabet_E;
static UG_BUTTON btn_Alphabet_F;
static UG_BUTTON btn_Alphabet_G;
static UG_BUTTON btn_Alphabet_H;
static UG_BUTTON btn_Alphabet_I;
static UG_BUTTON btn_Alphabet_J;
static UG_BUTTON btn_Alphabet_K;
static UG_BUTTON btn_Alphabet_L;
static UG_BUTTON btn_Alphabet_M;
static UG_BUTTON btn_Alphabet_N;
static UG_BUTTON btn_Alphabet_O;
static UG_BUTTON btn_Alphabet_P;
static UG_BUTTON btn_Alphabet_Q;
static UG_BUTTON btn_Alphabet_R;
static UG_BUTTON btn_Alphabet_S;
static UG_BUTTON btn_Alphabet_T;
static UG_BUTTON btn_Alphabet_U;
static UG_BUTTON btn_Alphabet_V;
static UG_BUTTON btn_Alphabet_W;
static UG_BUTTON btn_Alphabet_X;
static UG_BUTTON btn_Alphabet_Y;
static UG_BUTTON btn_Alphabet_Z;
static UG_BUTTON btn_Symbol_Shift;
static UG_BUTTON btn_Symbol_Space;
static UG_BUTTON btn_Symbol_Underscore;
static UG_BUTTON btn_Symbol_Hyphen;
static UG_BUTTON btn_OK;
static UG_BUTTON btn_Cancel;
static UG_BUTTON btn_Clear;
static UG_BUTTON btn_BackSpace;
static UG_TEXTBOX txb_DisplayString;

/* Value */
static char strInputString[INPUTSTRING_SIZE];
static size_t len_strInputString, len_strInputString_prev;
static bool isPushed_Shift;
static char *pResultString;

/* Control flow */
static bool hasUpdated;

// Character
static const char INPUTSTRING_SHIFT_CHAR[] = {0x7F, '\0'};

/* Private function prototypes -----------------------------------------------*/
static void WindowControlThread(void const *argument);
static void window_callback( UG_MESSAGE* msg );
static void initialize(void);
static void finalize(void);

static void initString(void);
static void addChar(char);
static void addChar_Shift(bool, char, char);
static void deleteChar(void);

static void createButton(UG_U8, UG_BUTTON*, char*, float, float, float, float);
static void changeButtonText_Shift(UG_U8, char*, char*, bool);
static void changeAllButtonText(bool);

static bool* requestInputString(char*);

/* Exported functions --------------------------------------------------------*/

/* ---------------------------------------------------------------- */
/* -- Create Window                                              -- */
/* ---------------------------------------------------------------- */
void createInputStringWindow(void)
{
    UG_U8 id_buf;

    UG_WindowCreate(pthis_wnd, obj_this_wnd, COUNTOF(obj_this_wnd), window_callback);
    UG_WindowSetTitleText(pthis_wnd, INPUTSTRING_WINDOW_TITLE);
    UG_WindowSetTitleTextFont(pthis_wnd, &INPUTSTRING_TITLE_FONT_SIZE);
    UG_WindowResize(pthis_wnd, 0, 0, BSP_LCD_GetXSize(), BSP_LCD_GetYSize() );
            /*BSP_LCD_GetXSize() * 0.02f,
            BSP_LCD_GetYSize() * 0.03f,
            BSP_LCD_GetXSize() * (1.0f - 0.02f),
            BSP_LCD_GetYSize() * (1.0f - 0.03f) );*/
    
    // Create buttons
    createButton(BTN_ID_Numeral_1, &btn_Numeral_1, "1",  0.f/10.f,  1.f/10.f, 1.f/6.f, 2.f/6.f);
    createButton(BTN_ID_Numeral_2, &btn_Numeral_2, "2",  1.f/10.f,  2.f/10.f, 1.f/6.f, 2.f/6.f);
    createButton(BTN_ID_Numeral_3, &btn_Numeral_3, "3",  2.f/10.f,  3.f/10.f, 1.f/6.f, 2.f/6.f);
    createButton(BTN_ID_Numeral_4, &btn_Numeral_4, "4",  3.f/10.f,  4.f/10.f, 1.f/6.f, 2.f/6.f);
    createButton(BTN_ID_Numeral_5, &btn_Numeral_5, "5",  4.f/10.f,  5.f/10.f, 1.f/6.f, 2.f/6.f);
    createButton(BTN_ID_Numeral_6, &btn_Numeral_6, "6",  5.f/10.f,  6.f/10.f, 1.f/6.f, 2.f/6.f);
    createButton(BTN_ID_Numeral_7, &btn_Numeral_7, "7",  6.f/10.f,  7.f/10.f, 1.f/6.f, 2.f/6.f);
    createButton(BTN_ID_Numeral_8, &btn_Numeral_8, "8",  7.f/10.f,  8.f/10.f, 1.f/6.f, 2.f/6.f);
    createButton(BTN_ID_Numeral_9, &btn_Numeral_9, "9",  8.f/10.f,  9.f/10.f, 1.f/6.f, 2.f/6.f);
    createButton(BTN_ID_Numeral_0, &btn_Numeral_0, "0",  9.f/10.f, 10.f/10.f, 1.f/6.f, 2.f/6.f);
    
    // Alphabet
    createButton(BTN_ID_Alphabet_Q, &btn_Alphabet_Q, "Q",  0.f/10.f,  1.f/10.f, 2.f/6.f, 3.f/6.f);
    createButton(BTN_ID_Alphabet_W, &btn_Alphabet_W, "W",  1.f/10.f,  2.f/10.f, 2.f/6.f, 3.f/6.f);
    createButton(BTN_ID_Alphabet_E, &btn_Alphabet_E, "E",  2.f/10.f,  3.f/10.f, 2.f/6.f, 3.f/6.f);
    createButton(BTN_ID_Alphabet_R, &btn_Alphabet_R, "R",  3.f/10.f,  4.f/10.f, 2.f/6.f, 3.f/6.f);
    createButton(BTN_ID_Alphabet_T, &btn_Alphabet_T, "T",  4.f/10.f,  5.f/10.f, 2.f/6.f, 3.f/6.f);
    createButton(BTN_ID_Alphabet_Y, &btn_Alphabet_Y, "Y",  5.f/10.f,  6.f/10.f, 2.f/6.f, 3.f/6.f);
    createButton(BTN_ID_Alphabet_U, &btn_Alphabet_U, "U",  6.f/10.f,  7.f/10.f, 2.f/6.f, 3.f/6.f);
    createButton(BTN_ID_Alphabet_I, &btn_Alphabet_I, "I",  7.f/10.f,  8.f/10.f, 2.f/6.f, 3.f/6.f);
    createButton(BTN_ID_Alphabet_O, &btn_Alphabet_O, "O",  8.f/10.f,  9.f/10.f, 2.f/6.f, 3.f/6.f);
    createButton(BTN_ID_Alphabet_P, &btn_Alphabet_P, "P",  9.f/10.f, 10.f/10.f, 2.f/6.f, 3.f/6.f);
    
    createButton(BTN_ID_Alphabet_A, &btn_Alphabet_A, "A",  0.f/10.f,  1.f/10.f, 3.f/6.f, 4.f/6.f);
    createButton(BTN_ID_Alphabet_S, &btn_Alphabet_S, "S",  1.f/10.f,  2.f/10.f, 3.f/6.f, 4.f/6.f);
    createButton(BTN_ID_Alphabet_D, &btn_Alphabet_D, "D",  2.f/10.f,  3.f/10.f, 3.f/6.f, 4.f/6.f);
    createButton(BTN_ID_Alphabet_F, &btn_Alphabet_F, "F",  3.f/10.f,  4.f/10.f, 3.f/6.f, 4.f/6.f);
    createButton(BTN_ID_Alphabet_G, &btn_Alphabet_G, "G",  4.f/10.f,  5.f/10.f, 3.f/6.f, 4.f/6.f);
    createButton(BTN_ID_Alphabet_H, &btn_Alphabet_H, "H",  5.f/10.f,  6.f/10.f, 3.f/6.f, 4.f/6.f);
    createButton(BTN_ID_Alphabet_J, &btn_Alphabet_J, "J",  6.f/10.f,  7.f/10.f, 3.f/6.f, 4.f/6.f);
    createButton(BTN_ID_Alphabet_K, &btn_Alphabet_K, "K",  7.f/10.f,  8.f/10.f, 3.f/6.f, 4.f/6.f);
    createButton(BTN_ID_Alphabet_L, &btn_Alphabet_L, "L",  8.f/10.f,  9.f/10.f, 3.f/6.f, 4.f/6.f);
    
    createButton(BTN_ID_Alphabet_Z, &btn_Alphabet_Z, "Z",  1.f/10.f,  2.f/10.f, 4.f/6.f, 5.f/6.f);
    createButton(BTN_ID_Alphabet_X, &btn_Alphabet_X, "X",  2.f/10.f,  3.f/10.f, 4.f/6.f, 5.f/6.f);
    createButton(BTN_ID_Alphabet_C, &btn_Alphabet_C, "C",  3.f/10.f,  4.f/10.f, 4.f/6.f, 5.f/6.f);
    createButton(BTN_ID_Alphabet_V, &btn_Alphabet_V, "V",  4.f/10.f,  5.f/10.f, 4.f/6.f, 5.f/6.f);
    createButton(BTN_ID_Alphabet_B, &btn_Alphabet_B, "B",  5.f/10.f,  6.f/10.f, 4.f/6.f, 5.f/6.f);
    createButton(BTN_ID_Alphabet_N, &btn_Alphabet_N, "N",  6.f/10.f,  7.f/10.f, 4.f/6.f, 5.f/6.f);
    createButton(BTN_ID_Alphabet_M, &btn_Alphabet_M, "M",  7.f/10.f,  8.f/10.f, 4.f/6.f, 5.f/6.f);
    
    createButton(BTN_ID_Symbol_Shift        , &btn_Symbol_Shift         , INPUTSTRING_SHIFT_CHAR,  0.f/10.f,  1.f/10.f, 4.f/6.f, 5.f/6.f);
    createButton(BTN_ID_Symbol_Space        , &btn_Symbol_Space         , "Sp",  8.f/10.f,  9.f/10.f, 4.f/6.f, 5.f/6.f);
    createButton(BTN_ID_Symbol_Underscore   , &btn_Symbol_Underscore    , "_" ,  9.f/10.f, 10.f/10.f, 3.f/6.f, 4.f/6.f);
    createButton(BTN_ID_Symbol_Hyphen       , &btn_Symbol_Hyphen        , "-" ,  9.f/10.f, 10.f/10.f, 4.f/6.f, 5.f/6.f);

    createButton(BTN_ID_OK,         &btn_OK,        "OK"        ,  0.f/ 4.f,  1.f/ 4.f, 5.f/6.f, 6.f/6.f);
    createButton(BTN_ID_Cancel,     &btn_Cancel,    "Cancel"    ,  1.f/ 4.f,  2.f/ 4.f, 5.f/6.f, 6.f/6.f);
    createButton(BTN_ID_Clear,      &btn_Clear,     "Clear"     ,  2.f/ 4.f,  3.f/ 4.f, 5.f/6.f, 6.f/6.f);
    createButton(BTN_ID_BackSpace,  &btn_BackSpace, "Back"      ,  3.f/ 4.f,  4.f/ 4.f, 5.f/6.f, 6.f/6.f);
    
    /* Configure textbox */
    id_buf = TXB_ID_DisplayString;
    UG_TextboxCreate(pthis_wnd, &txb_DisplayString, id_buf,
            UG_WindowGetInnerWidth(pthis_wnd)  *  0 / 10 + INPUTSTRING_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) *  0 /  6 + INPUTSTRING_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerWidth(pthis_wnd)  * 10 / 10 - INPUTSTRING_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) *  1 /  6 - INPUTSTRING_BUTTON_GAP_PIXEL );
    UG_TextboxSetFont(pthis_wnd, id_buf, &INPUTSTRING_TXTBOX_FONT_SIZE);
    UG_TextboxSetText(pthis_wnd, id_buf, "");
    UG_TextboxSetBackColor(pthis_wnd, id_buf, C_WHITE);
    UG_TextboxSetForeColor(pthis_wnd, id_buf, C_BLACK);
    UG_TextboxSetAlignment(pthis_wnd, id_buf, ALIGN_CENTER_RIGHT);
    
    isPushed_Shift = false;
    changeAllButtonText(isPushed_Shift);

    xTaskCreate( (TaskFunction_t)WindowControlThread, "InputStringTask", 
            configMINIMAL_STACK_SIZE, NULL, Priority_High, pthis_xHandle);
}

// return value -> true : update
// Arrange of "Future pattern" : Future instance is "*p_isCompleteInFuture"
bool inputString_InBackground(_InpStr_t *p)
{
    // "p_StartUpEvent" is normally set "true" in callback function
    if(p->StartUpEvent)
    {
        p->StartUpEvent = false;
        *(p->pp_isCompleteInFuture) = requestInputString(p->InputString);
    }
    
    if(*(p->pp_isCompleteInFuture) != NULL)
    {
        if(**(p->pp_isCompleteInFuture))
        {
            **(p->pp_isCompleteInFuture) = false;
            *(p->pp_isCompleteInFuture) = NULL;
            if(hasUpdated)
                return true;
        }
    }
    return false;
}


/* Private functions ---------------------------------------------------------*/

/* ---------------------------------------------------------------- */
/* -- Thread                                                     -- */
/* ---------------------------------------------------------------- */
static void WindowControlThread(void const *argument)
{
#ifdef PRINTF_DEBUG_MDOE
    printf("%s thread start\r\n", pcTaskGetName(*pthis_xHandle) );
#endif
    
	/* Variables initialization ------------------------------------*/
    xSemaphore_InputString = xSemaphoreCreateMutex();
	
	bool shouldSuspend  = true ;
    bool needInitialize = false;
    	 needFinalize   = false;    /* only "needFinalize" flag is changed from "window_callback" function */
	
    while (1)
    {
    	/* Thread flow */
    	vTaskDelay(INPUTSTRING_UPDATE_MS);
		
    	if(shouldSuspend)
    	{
#ifdef PRINTF_DEBUG_MDOE
            printf("%s thread suspend\r\n", pcTaskGetName(*pthis_xHandle) );
#endif
            vTaskSuspend(NULL);     // <- start here when resume
#ifdef PRINTF_DEBUG_MDOE
            printf("%s thread resume\r\n", pcTaskGetName(*pthis_xHandle) );
#endif
    		shouldSuspend  = false;
    		needInitialize = true;
    	}

        if(needInitialize)
        {
        	initialize();
        	needInitialize = false;
        }

        if(needFinalize)
        {
        	finalize();
        	needFinalize   = false;
        	shouldSuspend  = true;
        }
    }
}

/* ---------------------------------------------------------------- */
/* -- Callback function                                          -- */
/* ---------------------------------------------------------------- */
static void window_callback(UG_MESSAGE* msg)
{
    if (msg->type == MSG_TYPE_OBJECT)
    {
        if (msg->id == OBJ_TYPE_BUTTON)
        {
            len_strInputString_prev = strlen(strInputString);
            
            switch (msg->sub_id)
            {
            case BTN_ID_Numeral_0: addChar('0'); break;
            case BTN_ID_Numeral_1: addChar('1'); break;
            case BTN_ID_Numeral_2: addChar('2'); break;
            case BTN_ID_Numeral_3: addChar('3'); break;
            case BTN_ID_Numeral_4: addChar('4'); break;
            case BTN_ID_Numeral_5: addChar('5'); break;
            case BTN_ID_Numeral_6: addChar('6'); break;
            case BTN_ID_Numeral_7: addChar('7'); break;
            case BTN_ID_Numeral_8: addChar('8'); break;
            case BTN_ID_Numeral_9: addChar('9'); break;
                
            case BTN_ID_Alphabet_A: addChar_Shift(isPushed_Shift, 'A', 'a'); break;
            case BTN_ID_Alphabet_B: addChar_Shift(isPushed_Shift, 'B', 'b'); break;
            case BTN_ID_Alphabet_C: addChar_Shift(isPushed_Shift, 'C', 'c'); break;
            case BTN_ID_Alphabet_D: addChar_Shift(isPushed_Shift, 'D', 'd'); break;
            case BTN_ID_Alphabet_E: addChar_Shift(isPushed_Shift, 'E', 'e'); break;
            case BTN_ID_Alphabet_F: addChar_Shift(isPushed_Shift, 'F', 'f'); break;
            case BTN_ID_Alphabet_G: addChar_Shift(isPushed_Shift, 'G', 'g'); break;
            case BTN_ID_Alphabet_H: addChar_Shift(isPushed_Shift, 'H', 'h'); break;
            case BTN_ID_Alphabet_I: addChar_Shift(isPushed_Shift, 'I', 'i'); break;
            case BTN_ID_Alphabet_J: addChar_Shift(isPushed_Shift, 'J', 'j'); break;
            case BTN_ID_Alphabet_K: addChar_Shift(isPushed_Shift, 'K', 'k'); break;
            case BTN_ID_Alphabet_L: addChar_Shift(isPushed_Shift, 'L', 'l'); break;
            case BTN_ID_Alphabet_M: addChar_Shift(isPushed_Shift, 'M', 'm'); break;
            case BTN_ID_Alphabet_N: addChar_Shift(isPushed_Shift, 'N', 'n'); break;
            case BTN_ID_Alphabet_O: addChar_Shift(isPushed_Shift, 'O', 'o'); break;
            case BTN_ID_Alphabet_P: addChar_Shift(isPushed_Shift, 'P', 'p'); break;
            case BTN_ID_Alphabet_Q: addChar_Shift(isPushed_Shift, 'Q', 'q'); break;
            case BTN_ID_Alphabet_R: addChar_Shift(isPushed_Shift, 'R', 'r'); break;
            case BTN_ID_Alphabet_S: addChar_Shift(isPushed_Shift, 'S', 's'); break;
            case BTN_ID_Alphabet_T: addChar_Shift(isPushed_Shift, 'T', 't'); break;
            case BTN_ID_Alphabet_U: addChar_Shift(isPushed_Shift, 'U', 'u'); break;
            case BTN_ID_Alphabet_V: addChar_Shift(isPushed_Shift, 'V', 'v'); break;
            case BTN_ID_Alphabet_W: addChar_Shift(isPushed_Shift, 'W', 'w'); break;
            case BTN_ID_Alphabet_X: addChar_Shift(isPushed_Shift, 'X', 'x'); break;
            case BTN_ID_Alphabet_Y: addChar_Shift(isPushed_Shift, 'Y', 'y'); break;
            case BTN_ID_Alphabet_Z: addChar_Shift(isPushed_Shift, 'Z', 'z'); break;

            case BTN_ID_Symbol_Shift:
                if(isPushed_Shift)
                {
                    isPushed_Shift = false;
                    UG_ButtonSetBackColor(pthis_wnd, BTN_ID_Symbol_Shift, C_WHITE_SMOKE);
                }
                else
                {
                    isPushed_Shift = true;
                    UG_ButtonSetBackColor(pthis_wnd, BTN_ID_Symbol_Shift, C_GRAY);
                }
                changeAllButtonText(isPushed_Shift);
                break;

            case BTN_ID_Symbol_Space:       addChar(' '); break;
            case BTN_ID_Symbol_Underscore:  addChar('_'); break;
            case BTN_ID_Symbol_Hyphen:      addChar('-'); break;

            case BTN_ID_OK:
                /* Complete WITH value reflection */
                // Copy result string
                strcpy(pResultString, strInputString);
                
                hasUpdated   = true;
                needFinalize = true;
                break;
                
            case BTN_ID_Cancel:
                /* Complete WITHOUT value reflection */
                hasUpdated   = false;
                needFinalize = true;
                break;
                
            case BTN_ID_Clear:
                initString();
                break;
                
            case BTN_ID_BackSpace:
                deleteChar();
                break;

            default:
#ifdef PRINTF_DEBUG_MDOE
                printf("Error: " HERE "\r\n");
#endif
            break;
            }
            len_strInputString = strlen(strInputString);
            
            // To avoid flickering when string is full, update displayed string only if string is changed
            // Unnecessary flicker -> Len_prev = MAX && Len_now = MAX
            if( !((len_strInputString_prev == (INPUTSTRING_SIZE-1)) && (len_strInputString == len_strInputString_prev)) )
            {
                UG_TextboxSetText(pthis_wnd, TXB_ID_DisplayString, strInputString);
            }
        }
    }
}

/* ---------------------------------------------------------------- */
/* -- Initialize                                                 -- */
/* ---------------------------------------------------------------- */
static void initialize(void)
{
	/* Variables Initialization */
    hasUpdated = false;
    isPushed_Shift = false;
    UG_ButtonSetBackColor(pthis_wnd, BTN_ID_Symbol_Shift, C_WHITE_SMOKE);
    changeAllButtonText(isPushed_Shift);
	
	/* Show this Window */
    UG_WindowShow(pthis_wnd);
}

/* ---------------------------------------------------------------- */
/* -- Finalize                                                   -- */
/* ---------------------------------------------------------------- */
static void finalize(void)
{
    /* Variables Finalization */
    initString();
    
	/* Hide this Window */
	UG_WindowHide(pthis_wnd);
	
	/* Control flow */
    isCompletedInputString = true;
    xSemaphoreGive(xSemaphore_InputString);
}


static void initString(void)
{
    for (uint16_t i = 0; i < INPUTSTRING_SIZE; i++)
        strInputString[i] = '\0';
}

static void addChar(char c)
{
    if(strlen(strInputString) < (INPUTSTRING_SIZE-1) )      // "-1" is for null-terminated string
        strncat(strInputString, &c, 1);
}

static void addChar_Shift(bool shift, char cTrue, char cFalse)
{
    if(strlen(strInputString) < (INPUTSTRING_SIZE-1) )
    {
        if(shift)
            strncat(strInputString, &cTrue, 1);
        else
            strncat(strInputString, &cFalse, 1);
    }
}

static void deleteChar(void)
{
    size_t len = strlen(strInputString);
    if(len > 0)
        strInputString[len - 1] = '\0';
}

static bool* requestInputString(char* InputString)
{
    // Take Mutex
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if(xSemaphoreTakeFromISR(xSemaphore_InputString, &xHigherPriorityTaskWoken) == pdFALSE)
    {
#ifdef PRINTF_DEBUG_MDOE
        printf("xSemaphoreTakeFromISR error\r\n");
#endif
        return NULL;
    }
    if (xHigherPriorityTaskWoken != pdFALSE)
        taskYIELD();
    
    pResultString = InputString;
    
    vTaskResume(*pthis_xHandle);
    return &isCompletedInputString;
}



static void createButton(UG_U8 id, UG_BUTTON* pbtn, char* ptext, float XsRatio, float XeRatio, float YsRatio, float YeRatio)
{
    UG_ButtonCreate(pthis_wnd, pbtn, id,
            XsRatio * UG_WindowGetInnerWidth (pthis_wnd) + INPUTSTRING_BUTTON_GAP_PIXEL,
            YsRatio * UG_WindowGetInnerHeight(pthis_wnd) + INPUTSTRING_BUTTON_GAP_PIXEL,
            XeRatio * UG_WindowGetInnerWidth (pthis_wnd) - INPUTSTRING_BUTTON_GAP_PIXEL,
            YeRatio * UG_WindowGetInnerHeight(pthis_wnd) - INPUTSTRING_BUTTON_GAP_PIXEL );
    UG_ButtonSetFont(pthis_wnd, id, &INPUTSTRING_BUTTON_FONT_SIZE);
    UG_ButtonSetText(pthis_wnd, id, ptext);
}

static void changeButtonText_Shift(UG_U8 id, char* pText_true, char* pText_false, bool shift)
{
    if(shift)
        UG_ButtonSetText(pthis_wnd, id, pText_true);
    else
        UG_ButtonSetText(pthis_wnd, id, pText_false);
}

static void changeAllButtonText(bool shift)
{
    changeButtonText_Shift(BTN_ID_Alphabet_A, "A", "a", shift);
    changeButtonText_Shift(BTN_ID_Alphabet_B, "B", "b", shift);
    changeButtonText_Shift(BTN_ID_Alphabet_C, "C", "c", shift);
    changeButtonText_Shift(BTN_ID_Alphabet_D, "D", "d", shift);
    changeButtonText_Shift(BTN_ID_Alphabet_E, "E", "e", shift);
    changeButtonText_Shift(BTN_ID_Alphabet_F, "F", "f", shift);
    changeButtonText_Shift(BTN_ID_Alphabet_G, "G", "g", shift);
    changeButtonText_Shift(BTN_ID_Alphabet_H, "H", "h", shift);
    changeButtonText_Shift(BTN_ID_Alphabet_I, "I", "i", shift);
    changeButtonText_Shift(BTN_ID_Alphabet_J, "J", "j", shift);
    changeButtonText_Shift(BTN_ID_Alphabet_K, "K", "k", shift);
    changeButtonText_Shift(BTN_ID_Alphabet_L, "L", "l", shift);
    changeButtonText_Shift(BTN_ID_Alphabet_M, "M", "m", shift);
    changeButtonText_Shift(BTN_ID_Alphabet_N, "N", "n", shift);
    changeButtonText_Shift(BTN_ID_Alphabet_O, "O", "o", shift);
    changeButtonText_Shift(BTN_ID_Alphabet_P, "P", "p", shift);
    changeButtonText_Shift(BTN_ID_Alphabet_Q, "Q", "q", shift);
    changeButtonText_Shift(BTN_ID_Alphabet_R, "R", "r", shift);
    changeButtonText_Shift(BTN_ID_Alphabet_S, "S", "s", shift);
    changeButtonText_Shift(BTN_ID_Alphabet_T, "T", "t", shift);
    changeButtonText_Shift(BTN_ID_Alphabet_U, "U", "u", shift);
    changeButtonText_Shift(BTN_ID_Alphabet_V, "V", "v", shift);
    changeButtonText_Shift(BTN_ID_Alphabet_W, "W", "w", shift);
    changeButtonText_Shift(BTN_ID_Alphabet_X, "X", "x", shift);
    changeButtonText_Shift(BTN_ID_Alphabet_Y, "Y", "y", shift);
    changeButtonText_Shift(BTN_ID_Alphabet_Z, "Z", "z", shift);
}

/***************************************************************END OF FILE****/
