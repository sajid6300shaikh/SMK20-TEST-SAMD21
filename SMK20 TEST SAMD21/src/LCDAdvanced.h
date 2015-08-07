#define ARROW	127
#define LCDCOLS	20
#define LCDROWS	4
#define MenuSize(a)	sizeof(a)/LCDCOLS

void LCD_DispFourOptionsIncrementingFrom(uint8_t StartLine, const char MenuOptions[][LCDCOLS]);
void LCD_DispMenuArrow(uint8_t Row);
int8_t CheckMenuIndexBounds(int8_t MenuIndex, int8_t TotalOptions);
uint8_t LCD_MenuHandle(uint8_t TotalOptions, const char MenuOptions[][LCDCOLS]);
