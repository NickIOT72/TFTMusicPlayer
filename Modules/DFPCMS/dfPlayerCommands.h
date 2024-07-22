
#define START_BYTE 0x50
#define END_BYTE 0x51 

#define INIT_CMD 0X30 
#define PLAY_CMD 0X31 
#define PAUSE_CMD 0X32 
#define PREV_CMD 0X33 
#define NEXT_CMD 0X34 
#define SETEQ_CMD 0X35 
#define VOLUP_CMD 0X36 
#define VOLDOWN_CMD 0X37 
#define SETSONG_CMD 0X38 
#define SETSONGFOLDER_CMD 0X39 
#define GETNUMSONGS_CMD 0X3A 
#define GETNUMFOLDERS_CMD 0X3B
#define GETVOL_CMD 0X3C
#define SETVOL_CMD 0X3D
#define RESUME_CMD 0X3E
#define VOLSET_CMD 0X3F
#define STATUS_CMD 0X40

/**
 * @brief
 * Formar of UART sequence
 * [ START_BYTE[1BYTE]    COMMAND[1BYTE]   DATAINFO[3BYTE]   END_CMD[1byte]]
 * 
 * 
 */

#define SEQ_SIZE_CMD 6 