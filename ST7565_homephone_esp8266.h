

/*
// some of this code was written by <cstone@pobox.com> originally; it is in the public domain.
*/

#if ARDUINO >= 100
 #include "Arduino.h"
#endif
#ifdef __AVR__
#include <avr/pgmspace.h>
#endif

#define swap(a, b) {int t = a; a = b; b = t; }
#define DELETE 0
#define MIRROR 1
#define NO_MIRROR 0
#define CLEAR 0
#define BLACK 1
#define WHITE 0
#define black 1
#define  white 0
#define LCDWIDTH 128
#define LCDHEIGHT 64
#define X_MAX 127
#define Y_MAX 63


//lựa chọn font chữ số
#define ASCII_NUMBER 0
#define CASIO_NUMBER 1
#define STYLE_NUMBER 2
//
#define CMD_DISPLAY_OFF   0xAE
#define CMD_DISPLAY_ON    0xAF
/** tắt - bật màn hình
*/


#define CMD_SET_DISP_START_LINE  0x40
#define CMD_SET_PAGE  0xB0

#define CMD_SET_COLUMN_UPPER  0x10
#define CMD_SET_COLUMN_LOWER  0x00

#define CMD_SET_ADC_NORMAL  0xA0
/** đặt đầu ra ADC bình thường
*/

#define CMD_SET_ADC_REVERSE 0xA1
/**  
hiệu ứng mirror- gương
bạn nên dùng hàm glcd_flip_screen để độ rộng được căn chỉnh một cách chính xác
 */

# define CMD_NONE_ALL_PIXEL 0xA4
/** màn hình bình thường 
*/
#define CMD_ALL_PIXEL 0xA5
/** buộc màn hình hiển thị tất cả điểm ảnh có trên lcd
*/

#define CMD_SET_DISP_NORMAL 0xA6
/** hiển thị bình thường -không âm bản
*/
#define CMD_SET_DISP_REVERSE 0xA7
/** hiển thị âm bản- màu sắc của tất cả sẽ được chuyển  ngược lại đen<>trắng
*/

#define CMD_SET_COM_NORMAL  0xC8
/** hiển thị bình thường- không ngược
*/
#define CMD_SET_COM_REVERSE  0xC0
/** hiển thị ngược 
*/

#define CMD_SET_ALLPTS_NORMAL 0xA4
#define CMD_SET_ALLPTS_ON  0xA5

#define CMD_SET_BIAS_9 0xA2 
/** nghiêng màn hình đến 1/9th */

#define CMD_SET_BIAS_7 0xA3
/** nghiêng màn hình đến 1/9th */

#define CMD_RMW  0xE0
#define CMD_RMW_CLEAR 0xEE
#define CMD_INTERNAL_RESET  0xE2
#define CMD_SET_POWER_CONTROL  0x28
/** chế độ cung cấp điện nội bộ
*/

#define CMD_SET_RESISTOR_RATIO  0x20
/** thiết lập tỉ lệ nội trở R1/ R2  (OR có giá trị từ 0 đến 7 đơn vị( 3bit)) để điều chỉnh điện áp hoạt động
có giá trị từ 0x0 đến 0x6
*/


#define CMD_SET_VOLUME_FIRST  0x81

#define  CMD_SET_VOLUME_SECOND  0


#define CMD_SET_STATIC_OFF  0xAC
#define  CMD_SET_STATIC_ON  0xAD
#define CMD_SET_STATIC_REG  0x0
#define CMD_SET_BOOSTER_FIRST  0xF8
#define CMD_SET_BOOSTER_234  0
#define  CMD_SET_BOOSTER_5  1
#define  CMD_SET_BOOSTER_6  3

#define CMD_NOP  0xE3
/** No Operartion - không hoạt động 
theo datasheet đề nghị gửi lệnh định kì để giữ cho các kết nối dữ liệu liên tục
*/

#define CMD_TEST  0xF0

///////////ĐỔI TÊN HÀM THÀNH VIẾT HOA
 #define allpixel AllPixel  
 #define   drawline DrawLine 
 #define drawpixel DrawPixel     
 #define getpixel GetPixel    
 #define  fillcircle FillCircle   
 #define circle Circle   
 #define corner Corner  
 #define fillcorner FillCorner 
 #define rect Rect   
 #define  fillrect FillRect   
#define  tri Tri 
 #define  filltri FillTri   
#define  elip Elip  
#define fillelip FillElip  
 #define bitmap Bitmap  
#define  plus_bitmap  Plus_Bitmap   
 #define asc_char Asc_Char  
#define asc_string Asc_String  
 #define uni_char Uni_Char  
#define uni_string Uni_String 
#define runstatus RunStatus 
 #define number_long Number_Long  
#define number_ulong  Number_Ulong 
#define number_float Number_Float  
#define  keep_angle Keep_Angle  
#define  find_xy_elip Find_XY_Elip  
#define   x_elip X_Elip  
#define  y_elip Y_Elip  
#define  find_xy_sphere Find_XY_Sphere 
#define  x_sphere X_Sphere 
#define  y_sphere Y_Sphere  
#define analog Analog 
#define pullup_4 Pullup_4 

#define clear Clear
#define display Display




///////////class
class ST7565 {// khởi tạo class có tên ST7565
 public://cấp quyền truy cập công khai
 //TRUONGWF KHOIWR TAOJ CUAR Constructor
ST7565(byte RST,byte SCLK,byte A0,byte SID,byte CS) :rst(RST),sclk(SCLK),a0(A0),sid(SID)  , cs(CS) { 


}
ST7565(byte RST,byte SCLK,byte A0,byte SID) :rst(RST),sclk(SCLK),a0(A0),sid(SID)  , cs(-1) { 


}

void ON(void){
  st7565_init();

  st7565_command(CMD_DISPLAY_ON);

  st7565_command(CMD_SET_ALLPTS_NORMAL);
  SET(23,0,0,0,4);
  clear();

}
// chi can dung 4 chan sid, clk, A0,reset la du roi
  // khi đó nối chân CS của màn hình xuống GND .
// NẾU SỬ DỤNG CHÂN DIGITAL (0-RX) VÀ (1-TX):
// ĐÂY LÀ 2 CHÂN ĐƯỜNG TRUYỀN SERIAL VÀO MÁY TÍNH,  NẾU DÙNG 2 CHÂN NÀY THÌ PHẢI BỎ DÙNG CHức năng serial
//cụ thể hãy xóa dòng khai báo này: Serial.begin(...);
  void st7565_init(void);


  void SET(byte contrast, bool negative, bool rotation, bool mirror, byte tyledientro);
/** 
* biến contrast- độ tương phản- giá trị 0->63(hex từ 0x3f trở xuống 0x00)
* biến negative- hiển thị âm bản- giá trị 0 hoặc 1
* biến rotation- đảo ngược màn hình- giá trị 0 hoặc 1
* biến mirror- hiển thi gương- giá trị 0 hoặc 1
* biến tyledientro- bằng R1/R2- thiết lập giá trị nội trở hoạt động- 0->6(hex từ 0x6 đến 0x0)
*/
/** các thủ tục hiển thị */
  void st7565_command(uint8_t c);
  void st7565_data(uint8_t c);
/** ép màn hình hiển thị tất cả điểm ảnh- giá trị 0 hoặc 1
*/
  void clear_display(void);
  void Clear();
void Clear(int x0, int y0, int width, int high);
  void Display();

void AllPixel( bool val);

  void DrawPixel(int x,int y, bool color);  
 bool GetPixel(int x,int y);
  void Circle(int x0,int y0, int r, bool color);

  void FillCircle(int x0,int y0, int r, bool color);



void Corner( int x,int y,int w,int h, int r, bool color);
void FillCorner( int x,int y, int w, int h, int r, bool color);
/* corner vẽ hình chữ nhật có góc bo tròn
vd: corner( 5,7,40, 40, 5, BLACK);

x=5: tọa độ (ngang )của đỉnh ( góc trái trên cùng) tính từ trái sang.
y=7: tọa độ (trục đứng )của đỉnh ( góc trái trên cùng) tính từ trên xuống 
Note! : Chúng ta không được nhầm lẫn với trục tung trục hoành trong hình học phổ thông.
w=40: chiều dài hình chữ nhật.
h= 40, chiều cao hỉnh chữ nhật.
( đây là hình vuông)
r=5: bán kính của đường tròn bo góc .
Hãy thử vẽ với ví dụ sau:
drawconer(5,7, 40,40,20,BLACK);
Kết quả: hình tròn!
*/

  void FillRect(int x,int y, int w, int h, bool color);
  void Rect(int x,int y, int w, int h, bool color);
  void DrawLine(int x0,int y0,int x1,int y1,	bool color);

  void Bitmap(int x, int y,unsigned int w, unsigned int h,  const  uint8_t *bitmap ,   bool color);
/* vẽ một ảnh bitmap
x,y : tọa độ góc trái của ảnh
*bitmap: tên của ảnh ( được đặt trong tệp bmps.h)
w,h: cỡ của ảnh sẽ hiển thị
Note! Phải khai báo đúng kích cỡ của ảnh trong file hex, nếu không ảnh sẽ bị dịch lỗi.
Xem thêm trong phần ví dụ.
*/
void Plus_Bitmap(int x0, int y0, unsigned int w, unsigned int h,const uint8_t *bitmap , int goc, bool mirror, bool color);

  //void drawchar(byte x1, byte line, char c);

//chuẩn mỹ
  void Asc_Char(int x1, int y1,char c , bool color);
typedef char prog_char ;
#ifndef PGM_CHAR
#define PGM_CHAR const prog_char *
#endif

//#define ss ({})

#define A(s) ( {const static char __attribute__ ((progmem))__c[] = (s);&__c[0];})


void Asc_String(int x1, int y1,PGM_CHAR s , bool color);


// chuẩn quốc tế và tiếng việt thuộc chuẩn này


void  Uni_Char(int x1, int y1, char16_t c, bool color);

// mình đã trích đoạn này trong tệp pgmspace.h và có chỉnh sửa
typedef char16_t prog_char16_t ;
#ifndef PGM_CHAR16T
#define PGM_CHAR16T const prog_char16_t *
#endif
#define U(s) ({const static char16_t __attribute__ ((progmem))__c[] = (s); &__c[0];})

void Uni_String(int x1, int y1,PGM_CHAR16T s , bool color);
// hoàn thiện font việt 25/10/2016
//phùng thái sơn (thuật toán vẫn tốn khoảng 5kb nền, hãy cải thiện thuật toán nếu có thể)
 // viết một chữ số kiểu byte
 



/* drawchar - viết một kí tự trong bảng ASCII mở rộng 
char là kiểu dữ liệu kí tự, có miền giá trị 0 đến 255
nếu dùng số:
 lcd.drawchar( 2,5, 65,BLACK);
Hoặc viết 
lcd.drawchar (2,5, 'a');
lcd.display();
delay(10000);
>> chữ a

*/

void RunStatus( int x, int y, byte a,unsigned int t, char *c, bool color) ;

/** runstatus- string- chuỗi chữ chạy ví dụ:
x1,y tọa độ con trỏ
a=10: số cột hiển thị- mỗi cột tương ứng với một chữ sẽ được hiển thị, a từ 0 đến 22.
t=12: tốc độ chạy chữ, t từ 1 đến 100 ( speed min- max).
cuối cùng là chuỗi kí tự *c, color màu
*/
// mình đã trích đoạn này trong tệp pgmspace.h và có chỉnh sửa
  void Number_Long(int x, int y,long a,byte select_font, bool color);
  // in ra một số kiểu dữ liệu long
  // x, line : tọa đọ con trỏ
  // a biến vào kiểu long

void Number_Ulong(int x, int y,unsigned long a,byte select_font, bool color);
//biến kiểu unsigned long -nguyên dương

void Number_Float(int x, int y, float a,byte n, byte select_font,bool color);
// biến vào kiểu float 
// n là số chữ số muốn láy sau dấu phẩy
//float số dương lớn nhất là 999999,9
//float dương bé nhất là 0,000001

void  draw_single_number(int x1, int y1, byte single_number, byte select_font,bool color);
 /*
 Font_style_select:
 ASCII_NUMBER hoặc 0
 CASIO_NUMBER hoặc 1
 STYLE_NUMBER hoặc 2
*/

//hoàn thiện draw_number 12:25 AM-1/9/2016
  // Phùng Thái Sơn 

/* drawstring viết một chuỗi kí tự
x : tọa độ ngang của con trỏ. x từ 0-128
line: dòng bắt đầu . line từ 0-7
a: số chữ hiển thị trên 1 dòng
*/
	void FillTri (int x1,int y1,int x2,int y2,int x3,int y3, bool color);
/* vẽ tam giác với tọa độ 3 đỉnh
*/

void Tri(int x1,int y1,int x2,int y2 ,int x3,int y3, bool color);


/* vẽ tập hợp điểm ellipse thử nghiermj */

/* thử hàm get_pixel_ellipse
*/

int Keep_Angle(int goc);
  // luôn giữ cho góc nhập vào (độ) nằm trong khoảng 0->360 độ

void Find_XY_Elip(int x0, int y0,  int a,  int b,int goc_alpha) ;
int X_Elip() ;
  
int Y_Elip();
/* Lấy tọa độ điểm rên mặt cầu
*/

void Find_XY_Sphere( int x0, int y0, int a0, int b0, int goc_alpha, int goc_beta);
  
int X_Sphere();

int Y_Sphere();
/* Vẽ ellipse 
*/







void Analog( int x, int y, int w, int h,  int value,bool color);







void DrawEllipse_private(long CX, long  CY, long XRadius,long YRadius, bool color, bool fill);
void Plot4EllipsePoints(int  CX,int   CY, int X, int  Y, bool color, bool fill);

void FillElip( long xCenter, long  yCenter,  long xRadius, long  yRadius, bool color);

void Elip(long xCenter, long  yCenter, long   xRadius,  long  yRadius, bool color);

/* vẽ elip
vi du: 
lcd.drawellipse(60, 30, 10, 20, BLACK);
lcd.fillellipse(60, 30, 10, 20, BLACK);
60,60 tọa độ x, y của tâm elip
xRadius=10: bán kính trục ngang
yRaadius=20: bán kính trục đứng

*/


byte Pullup_4(byte right_pin,  byte up_pin, byte left_pin, byte down_pin);
/*
cài dặt nối Pullup nội trở trước khi sử dụng chức năng này
4 biến pin vào nhé

=0 nếu không có nút nào được nhấn
=1 right
=2 up
=3 left 
=4 down
10*(1*2)=20 right+up
10*(1*3)=30 right+left
10*(1*4)=40 right+down
10*(2*3)=60 up+left
10*(2*4)=80 up+down
10*(3*4)=120 left + down

_______[2]
___[3]_____[1]
_______[4]



 */




 private:
int X_SPHERE,Y_SPHERE,X_ELLIPSE,Y_ELLIPSE;

void duong_nam_ngang( long x1, long x2, long y0,bool color);

  //vẽ đường thẳng nối điểm A(X1,Y0) và B(X2,Y0) với x1<=x2 
void duong_thang_dung(long x0, long y1, long y2, bool color);

  //vẽ đường thẳng nối điểm A(X0,Y1) Và B(XO,Y2) với y1<=y2
 byte sid, sclk, a0, rst, cs;
 
int Font_width, Font_high,Font_column_bitmap;// cài kích thước cho font được chọn
 
// Font_column_bitmap ảnh bitmap mỗi hàng có bao nhiêu cột(chiều dài)
// font denta là độ dịch bytes so với địa chỉ của font đó
void SET_SIZE ( byte select_font){
switch(select_font){
case CASIO_NUMBER:
Font_width=12;
Font_high=16;
Font_column_bitmap=120;//12*10
break;

case STYLE_NUMBER:
Font_width=16;
Font_high=16;
Font_column_bitmap=160;//16*10
break;
default:
//ascii
Font_width=5;
Font_high=7;
break;




}
  }
  void spiwrite(uint8_t c);
  void my_drawpixel(int x, int y, bool color);
  byte tim_thu_tu( char16_t  c);
  //uint8_t buffer[128*64/8]; 
};
 