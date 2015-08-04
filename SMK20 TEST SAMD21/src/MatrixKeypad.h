#ifndef _Matrix_Keypad_h
#define _Matrix_Keypad_h

/************************************************************************/
/* Key Definitions                                                      */
/************************************************************************/

//Hardware
#define	ROW1	PB10
#define	ROW2	PB11
#define	ROW3	PB12
#define	COL1	PB13
#define	COL2	PB14

//Bit variables
volatile bool	ParaKey;
volatile bool	RunKey;
volatile bool	StartKey;
volatile bool	StopKey;
volatile bool	UpdateKey;
volatile bool	EnterKey;

//Alternate names
#define StoreKey	EnterKey
#define IncrKey		StartKey
#define DecrKey		StopKey
//////////////////////////////////////////////////////////////////////////
#define debounce_ms 20		//milliseconds to wait between two scan to confirm key press and debounce keys.
#define NoKeyDetected !KeyDetected

//Below is a working code
/************************************************************************/
/* Multi line macro. 
This Function will loop until said key is pressed
Usage: WaitFor(EnterKey); 
macro expansion made is

ClearKeys();
while(EnterKey==0)
 {
	Keyscan();
 }*/

/************************************************************************/
#define	WaitFor(i) \
ClearKeys();\
while(i==0){\
	Keyscan();\
}
	
void ClearKeys(void);
bool KeyDetected(void);
void Config_MatrixKeypad(void);
void Keyscan(void);
void LoopUntilEnterPressed(void);

#endif