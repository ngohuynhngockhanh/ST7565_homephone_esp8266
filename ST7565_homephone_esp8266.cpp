

/*

ST7565 LCD library!

Copyright (C) 2016 SON
// some of this code was written by <cstone@pobox.com> originally; it is in the public domain.
 Developed by Phùng Thái Sơn - 1996- Hà Nam
Supported by Nguyễn Mạnh Hoạch, Phùng Thái Hải.
Mât rất nhiều thời gian và công sức cho bộ thư viện này
Mình đã chú thích khá đầy đủ trong quá trình viết code
Nếu bạn đã vào phần này, hãy bỏ chút thời gian tham khảo (có nhiều cái hay)và phát triển bộ thư viện này nhé.

Up loaded to Arduino.vn
Đã khắc phục lỗi: tọa độ có thể mang giá trị âm 9:12AM-8/28/2016
12:45 am -1/9/2016 Bổ trợ khả năng in số theo các kiểu biến byte int, uint, long, ulong, float
25/10/2016: viết được kí tự tiếng việt
31/10/2016 get_xy_ellipse và sphere
Thư viện nguồn mở, bạn có thể thay đổi hoặc sử dụng cho mục đích khác, đăng tải ở bất cứ đâu
(có ghi chú )nếu nó hợp pháp và không vi phạm các quy tắc trong cộng đồng nguồn mở!


Copyright (C) 2010 Limor Fried, Adafruit Industries

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*/

/* $Id: ST7656 HOMEPHONE 11/2016 by Thai Son $ */
//#include <Wire.h>


#ifdef __AVR__
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#endif

#ifndef _delay_ms
  #define _delay_ms(t) delay(t)
#endif

#ifndef _BV
  #define _BV(bit) (1<<(bit))
#endif


#include <stdlib.h>
#include <glcdfont.c>
#include "ST7565_homephone.h"


uint8_t is_reversed = 0;

// a handy reference to where the pages are on the screen
const uint8_t pagemap[] = { 7, 6, 5, 4, 3, 2, 1, 0 };
/** điều chỉnh thứ tự quét 8 hàng ( page)
một vài lcd ic st7565 bạn cần chỉnh nó thành
const uint8_t pagemap[] = { 3, 2, 1, 0, 7, 6, 5, 4 };  
*/

// BỘ FONT 5X7


// a 5x7 font table

const extern unsigned char vietnamese_vowel_table[] PROGMEM ;
const extern unsigned char style_number[] PROGMEM ;
const extern unsigned char font[] PROGMEM ;
const extern unsigned char casio_number[] PROGMEM ;
// the memory buffer for the LCD, đây chính là cái mình nhấn mạnh 
byte st7565_buffer[1024] = { };

// reduces how much is refreshed, which speeds it up!
// originally derived from Steve Evans/JCW's mod but cleaned up and
// optimized
#define enablePartialUpdate

#ifdef enablePartialUpdate
static uint8_t xUpdateMin, xUpdateMax, yUpdateMin, yUpdateMax;
#endif


//updateBoundingBox(x,y,w,h); là hàm để update vùng điểm ảnh x,y có kích thước WxH từ bộ nhớ đệm lên màn hình


static void updateBoundingBox(int xmin, int ymin, int xmax, int ymax) {

 if(xmin<0){xmin=0;}
   if(ymin<0){ymin=0;}
   if(xmax>X_MAX){xmax=X_MAX;}//X_MAX=127
   	if(ymax>Y_MAX){ymax=Y_MAX;}//Y_MAX 63
#ifdef enablePartialUpdate
  if (xmin < xUpdateMin) xUpdateMin = xmin;
  if (xmax > xUpdateMax) xUpdateMax = xmax;
  if (ymin < yUpdateMin) yUpdateMin = ymin;
  if (ymax > yUpdateMax) yUpdateMax = ymax;
#endif
}
////////||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
void ST7565::Bitmap(int x, int y, unsigned int w, unsigned int h,const uint8_t *bitmap, bool color) {
  bool non=!color;
 
    for (unsigned int i=0; i<w; i++ ) {

    	for (unsigned int j=0; j<h; j++) {


    if(bitRead( pgm_read_byte(bitmap+i+(j/8)*w),j%8)){// font: địa chỉ của mảng font[] là tên mảng đó

	my_drawpixel(x+i, y+j, color);
      }else{

	my_drawpixel(x+i, y+j, non);
      }
    }
  }

  updateBoundingBox(x, y, x+w, y+h);



}
   //||||||||||||||||||||||||||||||||||||||||||||||
   //|||||||||||||||||||||||
void ST7565::Plus_Bitmap(int x0, int y0, unsigned int w, unsigned int h,const uint8_t *bitmap , int goc, bool mirror, bool color) {
 // xoay ảnh bitmap trong4 góc 0-90-180-270
int x,y;
bool notcolor=!color;
unsigned int w_1,h_1;
w_1=w-1;
h_1=h-1;
 for (unsigned int i=0; i<w; i++ ) {//con trỏ dich byte
  for (unsigned int j=0; j<h; j++) {// con trỏ dịch bit

 switch(goc){
  case 0:

         
  y=y0+j;//0
     if(mirror){
          
                  x=(x0+w_1)-i; // xoay 0 độ  gương
     }else{

                  x=x0+i;   // xoay 0 độ  thường 
     }
  break;
  case 90:

         
     y=(y0+w_1)-i;//90
     if(mirror){
x=(x0+h_1)-j;// xoay 90 độ  gương
     }else{
x=x0+j;// xoay 90 độ  thường
     }
  break;
  case 180:

          y=(y0+h_1)-j;//180
     if(mirror){
                    x=x0+i; // xoay 180 độ gương

     }else{

                    x=(x0+w_1)-i;// xoay 180 độ thường
     }
  break;
  case 270:

        
  y=y0+i;//270
     if(mirror){
               
     	x=x0+j;// xoay 270 độ gương

     }else{
x=(x0+h_1)-j;// xoay 270 độ thường

     }
  break;
default:
         return;  //thoát        
break;

     }// switch

      if (pgm_read_byte(bitmap + i + (j/8)*w) & _BV(j%8)) {
       my_drawpixel(x,y,color);
       }else{
       my_drawpixel(x,y,notcolor);
       }
 }
}
w+=x0;
h+=y0;
if((goc==0)||(goc==90)){
	updateBoundingBox(x0,y0,w,h);
} else{

	updateBoundingBox(x0,y0,h,w);
}
}

////|||||||||||||||||||||||||||||||||||||||||||||||

void  ST7565::Asc_Char(int x1, int y1,char c , bool color) {
bool non=!color;
const unsigned char* dress =font+c*5;
  for (byte i =0; i<5; i++ ) {
  for (byte j =0; j<8; j++ ) {

    if(bitRead( pgm_read_byte(dress+i),j)){// font: địa chỉ của mảng font[] là tên mảng đó
   my_drawpixel(x1+i, y1+j,color);
  }else{
 my_drawpixel(x1+i, y1+j,non);
  }
}
}
updateBoundingBox(x1,y1,x1+5,y1+8);
}

///||||||||||||||||||||||||||||||||||\\\\\\\\\\\\\\\\\\


void ST7565::RunStatus(int x, int y,byte a,unsigned int t,char *c, bool color) { 
  
 bool non=!color;
t=abs(101-t);
byte b,d,m,w;
unsigned int  i;

i=0;
b=a;
while (c[i+1]!=0){ // dịch kí tự bắt đầu
 

//gặp kí tự cuối cùng thì thoát lặp
  i++;
if (c[i+b]==0){// TIÊN ĐOÁN VỊ TRÍ NGẮT
  
a--;//trừ độ dài hiển thị  đi 1
}// ngược lại , thì giữ nguyên độ dài 
b=a; //đặt lại b
 for ( d= 6; d>0 ; d--) { // dịch hoành độ
 m=x;
 for ( w = i; w <( b+i); w++ ) { // viết tất cả a kí tự
 if (m<LCDWIDTH-6) {
   Asc_Char(m+d , y , c[w] , color);

 m+=6;
}

}
Asc_Char(x, y , 16 , color); // vẽ con trỏs
_delay_ms( t);
Display();

FillRect(x+5,y,a*6,8,non); 
/* ẩn đối tượng bằng hình chữ nhật có màu trắng
*/
  }

}
/* đóng hộp
*/

 }


// ví dụ Asc_String( 5,10,PSTR("HELLO"),BLACK);// chú ý đoạn PSTR""
void ST7565::Asc_String(int x1, int y1,PGM_CHAR s , bool color) {
int x=x1;
char c;
  while ((c=pgm_read_byte(s)) != 0) {// mối lần c++ thì địa chỉ của kí tự là c[0] do c=c[]
    // Mời bạn tìm hiểu thêm cách lưu biến tĩnh vào bộ nhớ flash trên trang arduino.vn
    Asc_Char(x, y1, c, color);
    //Serial.println(c[i]);
    s++;
    x += 6; // 6 pixels wide
    if (x + 6 >=  LCDWIDTH  ){
      x = x1;    // ran out of this line
      y1+=8;
    }
    if ( y1 > LCDHEIGHT)
      return;        // dừng vòng lặp
  }

}
/*
void ST7565::Asc_String(int x1, int y1, const  char  *c , bool color) {
int x=x1;
  while (c[0] != 0) {// mối lần c++ thì địa chỉ của kí tự là c[0] do c=c[]
    Asc_Char(x, y1, c[0], color);
    //Serial.println(c[i]);
    c++;
    x += 6; // 6 pixels wide
    if (x + 6 >=  LCDWIDTH  ){
      x = x1;    // ran out of this line
      y1+=8;
    }
    if ( y1 > LCDHEIGHT)
      return;        // dừng vòng lặp
  }

}
*/

//|||||||||||||||||||||||||||||||||||||||||||||||||||||||


void  ST7565::draw_single_number(int x1, int y1,byte single_number, byte select_font,bool color) {
//SET_SIZE(select_font);// chọn cỡ chữ theo font
bool non=!color;
const unsigned char *ADRESS;
switch (select_font){
case 1:
ADRESS=casio_number;
break;
case 2:
ADRESS=style_number;
break;
default:
ADRESS=font+240;//48*5 byte đầu tiên của kí tự 0 (số 0 ) trang bảng ascii
break;
}
ADRESS+=single_number*Font_width;
unsigned int  enter;
    for (byte i=0; i<Font_width; i++ ) {
       for (byte j=0; j<Font_high; j++) {
    	if( j<8){
    		enter=0;
    	}else if(j<15){
    		enter=Font_column_bitmap;


    	}
      if (bitRead(pgm_read_byte(ADRESS + i + enter),j%8)) {
	my_drawpixel(x1+i, y1+j, color);
      }else{
      	my_drawpixel(x1+i, y1+j, non);
      }
    }
  }

  updateBoundingBox(x1, y1, x1+Font_width, y1+Font_high);
}

  void ST7565::Number_Long(int x,int y, long a,byte select_font, bool color){

 SET_SIZE(select_font);// chọn cỡ chữ theo font
if(a==0){

draw_single_number(x,y,0,select_font,color);
return;
}

int denta_x=0;
byte font_w=Font_width+1;
byte du;
long b;
if (a<0){
 // nếu a<0 thì đổi dấu
  a=(-1)*a;

Asc_Char(x,y+Font_width/2,8, color); //  VẼ dấu trừ (-) ở đằng trước

x+=6;//dịch con trỏ sang phải  , bắt đàu ghi số
} 
b=a;
while(b>9){
b/=10;
denta_x+=font_w;
}
x+=denta_x;
while (a>0){
du=a%10;// lấy phần dư hàng đơn vị

draw_single_number(x,y,du,select_font,color);
a=((a-du)/10);// lấy chẵn rồi giảm xuống 10 lần
x-=font_w;// dịch con trỏ sang trái  sau mỗi lần ghi
}// kết thúc a=0

  }


void ST7565::Number_Ulong(int x, int y,unsigned long a,byte select_font, bool color){

 SET_SIZE(select_font);// chọn cỡ chữ theo font

if(a==0){

draw_single_number(x,y,0,select_font,color);
return;
}
int denta_x=0;
byte font_w=Font_width+1;
byte du;
unsigned long b=a;
b=a;
while(b>9){
b/=10;
denta_x+=font_w;
}
x+=denta_x;
while (a>0){
du=a%10;// lấy phần dư hàng đơn vị
draw_single_number(x,y,du,select_font,color);
a=((a-du)/10);// lấy chẵn rồi giảm xuống 10 lần
x-=font_w;// dịch con trỏ sang trái  sau mỗi lần ghi
}// kết thúc a=0

}

void ST7565::Number_Float(int x, int y, float a,byte n, byte select_font,bool color){


 SET_SIZE(select_font);// chọn cỡ chữ theo font
 if(a==0){

draw_single_number(x,y,0,select_font,color);
return;
}
// vẽ phần trước dấu phảy

float sau;// 
unsigned long b1,b2; // để lưu trong tính toán
byte du ;// 1 số dư du
byte x2; // tọa độ thực tế của con trỏ
if (a<0){
 // nếu a<0 thì đổi dấu
a=(-1)*a;
Asc_Char(x,y+Font_width/2,8, color); //  VẼ dấu trừ (-) ở đằng trước
x+=6;//dịch con trỏ sang phải  , bắt đàu ghi số
} 

b1=b2=(unsigned long)a;// ép về kiểu số nguyên, ta được số trước dấu phẩy
sau=a-float(b1);//tách lấy số sau dấu phẩy

byte denta_x=0;
byte font_w=Font_width+1;
while(b1>9){
	b1/=10;
	denta_x+=font_w;
}
x+=denta_x;// VẼ TỪ PHẢI SANG TRÁI


while( b2>0){
du=b2%10; // lấy phần dư hàng đơn vị
draw_single_number(x,y,du,select_font,color);
b2=((b2-du)/10); // kết thúc b2=0
x-=font_w;//dịch con trỏ sang trái sau mỗi lần ghi
}
// //////////////////kết thúc phần trước dấu phảy
x+=denta_x+2+font_w*2;
//vẽ dấu phẩy cách đằng trước và sau 2 pixel
fillrect(x,y+Font_high-2,2,2,color);
x+=4;
// vẽ phần sau dấu phẩy
float d_sau;
d_sau=sau*float(pow(10,n));// nhân =(10 mũ n) n_max=8
b2=(unsigned long)d_sau;

x+=((n-1)*font_w);//dịch con trỏ sang phải 


while( b2>0){
du=b2%10; // lấy phần dư hàng đơn vị

draw_single_number(x,y,du,select_font,color);
b2=((b2-du)/10); // kết thúc b2=0
x-=font_w;//dịch con trỏ sang trái sau mỗi lần ghi

}


}



byte ST7565::tim_thu_tu( char16_t  c){
switch(c){
// nguyên âm thấp   và không mũ(thường)
  case u'a':return 0;break;// 0/6 thương =0 DƯ 0
  case u'à':return 1;break;// 1/6 thương 0 dư 1
  case u'á':return 2;break;
  case u'ả':return 3;break;
  case u'ã':return 4;break;
  case u'ạ':return 5;break;// 5/6 thuong =0 dư 5

  case u'e':return 6;break;
  case u'è':return 7;break;
  case u'é':return 8;break;
  case u'ẻ':return 9;break;
  case u'ẽ':return 10;break;
  case u'ẹ':return 11;break;

  case u'i':return 12;break;
  case u'ì':return 13;break;
  case u'í':return 14;break;
  case u'ỉ':return 15;break;
  case u'ĩ':return 16;break;
  case u'ị':return 17;break;
 
  case u'o':return 18;break;
  case u'ò':return 19;break;
  case u'ó':return 20;break;
  case u'ỏ':return 21;break;
  case u'õ':return 22;break;
  case u'ọ':return 23;break;

  case u'u':return 24;break;
  case u'ù':return 25;break;
  case u'ú':return 26;break;
  case u'ủ':return 27;break;
  case u'ũ':return 28;break;
  case u'ụ':return 29;break;

  case u'ư':return 30;break;
  case u'ừ':return 31;break;
  case u'ứ':return 32;break;
  case u'ử':return 33;break;
  case u'ữ':return 34;break;
  case u'ự':return 35;break;

  case u'y':return 36;break;
  case u'ỳ':return 37;break;
  case u'ý':return 38;break;
  case u'ỷ':return 39;break;
  case u'ỹ':return 40;break;
  case u'ỵ':return 41;break;
// nguyên âm thấp và có mũ
  case u'ă':return 42;break;// 6/6=1 dư 0
  case u'ằ':return 43;break;// 7/6=1 dư 1
  case u'ắ':return 44;break;
  case u'ẳ':return 45;break;
  case u'ẵ':return 46;break;
  case u'ặ':return 47;break;//

  case u'â':return 48;break;
  case u'ầ':return 49;break;
  case u'ấ':return 50;break;
  case u'ẩ':return 51;break;
  case u'ẫ':return 52;break;
  case u'ậ':return 53;break;
 
  case u'ê':return 54;break;
  case u'ề':return 55;break;
  case u'ế':return 56;break;
  case u'ể':return 57;break;
  case u'ễ':return 58;break;
  case u'ệ':return 59;break;
 
  case u'ô':return 60;break;
  case u'ồ':return 61;break;
  case u'ố':return 62;break;
  case u'ổ':return 63;break;
  case u'ỗ':return 64;break;
  case u'ộ':return 65;break;
 
  case u'ơ':return 66;break;
  case u'ờ':return 67;break;
  case u'ớ':return 68;break;
  case u'ở':return 69;break;
  case u'ỡ':return 70;break;
  case u'ợ':return 71;break;
 // nguyên âm cao (in hoa)
  case u'A':return 72;break;
  case u'À':return 73;break;
  case u'Á':return 74;break;
  case u'Ả':return 75;break;
  case u'Ã':return 76;break;
  case u'Ạ':return 77;break;
  
  case u'Ă':return 78;break;
  case u'Ằ':return 79;break;
  case u'Ắ':return 80;break;
  case u'Ẳ':return 81;break;
  case u'Ẵ':return 82;break;
  case u'Ặ':return 83;break;
 
  case u'Â':return 84;break;
  case u'Ầ':return 85;break;
  case u'Ấ':return 86;break;
  case u'Ẩ':return 87;break;
  case u'Ẫ':return 88;break;
  case u'Ậ':return 89;break;
 
  case u'E':return 90;break;
  case u'È':return 91;break;
  case u'É':return 92;break;
  case u'Ẻ':return 93;break;
  case u'Ẽ':return 94;break;
  case u'Ẹ':return 95;break;// 11/6=1 dư 5

  case u'Ê':return 96;break;
  case u'Ề':return 97;break;
  case u'Ế':return 98;break;
  case u'Ể':return 99;break;
  case u'Ễ':return 100;break;
  case u'Ệ':return 101;break;
 
  case u'I':return 102;break;
  case u'Ì':return 103;break;
  case u'Í':return 104;break;
  case u'Ỉ':return 105;break;
  case u'Ĩ':return 106;break;
  case u'Ị':return 107;break;
 
  case u'O':return 108;break;
  case u'Ò':return 109;break;
  case u'Ó':return 110;break;
  case u'Ỏ':return 111;break;
  case u'Õ':return 112;break;
  case u'Ọ':return 113;break;

  case u'Ô':return 114;break;
  case u'Ồ':return 115;break;
  case u'Ố':return 116;break;
  case u'Ổ':return 117;break;
  case u'Ỗ':return 118;break;
  case u'Ộ':return 119;break;
 
  case u'Ơ':return 120;break;
  case u'Ờ':return 121;break;
  case u'Ớ':return 122;break;
  case u'Ở':return 123;break;
  case u'Ỡ':return 124;break;
  case u'Ợ':return 125;break;
 
  case u'U':return 126;break;
  case u'Ù':return 127;break;
  case u'Ú':return 128;break;
  case u'Ủ':return 129;break;
  case u'Ũ':return 130;break;
  case u'Ụ':return 131;break;
 
  case u'Ư':return 132;break;
  case u'Ừ':return 133;break;
  case u'Ứ':return 134;break;
  case u'Ử':return 135;break;
  case u'Ữ':return 136;break;
  case u'Ự':return 137;break;
 
  case u'Y':return 138;break;
  case u'Ỳ':return 139;break;
  case u'Ý':return 140;break;
  case u'Ỷ':return 141;break;
  case u'Ỹ':return 142;break;
  case u'Ỵ':return 143;break;
 //phụ âm
  case u'q':return 144; break; 
  case u'r':return 145; break;
  case u't':return 146; break;
  case u'p':return 147; break;
  case u's':return 148; break;
  case u'd':return 149; break;
  case u'đ':return 150; break;
  case u'g':return 151; break;
  case u'h':return 152; break;
  case u'k':return 153; break;
  case u'l':return 154; break;
  case u'x':return 155; break;
  case u'c':return 156; break;
  case u'v':return 157; break;
  case u'b':return 158; break;
  case u'n':return 159; break;
  case u'm':return 160; break;
  case u'Q':return 161; break;
  case u'R':return 162; break;
  case u'T':return 163; break;
  case u'P':return 164; break;
  case u'S':return 165; break;
  case u'D':return 166; break;
  case u'Đ':return 167; break;
  case u'G':return 168; break;
  case u'H':return 169; break;
  case u'K':return 170; break;
  case u'L':return 171; break;
  case u'X':return 172; break;
  case u'C':return 173; break;
  case u'V':return 174; break;
  case u'B':return 175; break;
  case u'N':return 176; break;
  case u'M':return 177; break;
  default : return 178; break;
}

}
void  ST7565::Uni_Char(int x1, int y1, char16_t c, bool color){
bool non=!color;
byte  thu_tu;
thu_tu=tim_thu_tu(c);

unsigned int thuong;

thuong=thu_tu/6;// thương để xác định vị trí kí tự
thuong*=6;// nhân 6 để cho ra byte đầu tiên của kí tự đó
unsigned int so_du;
so_du=thu_tu%6;// số dư  để xác định vị trí dấu
so_du=so_du*6+348;// tìm byte
byte dich_bit=0;
if(thu_tu<42){
	// nguyên âm thấp và không mũ
	dich_bit=3;
}
else if(thu_tu<72){
	// nguyên âm thấp và có mũ
	dich_bit=1;
} 
// ngược lại là nguyên âm cao, dich_bit=0;
//348 là byte thứ 348 , nơi bắt dầu bộ dấu
bool read_bit_ki_tu=0; // trả về bit 0 hoặc 1
bool read_bit_dau=0; // trả về bit 0 hoặc 1


unsigned int enter;
unsigned int tim_byte_phu_am;
tim_byte_phu_am=(thu_tu-144)*6+144;
// 144 là thứ tự bắt đầu phụ âm
    for (byte i=0; i<6; i++ ) {
       for (byte j=0; j<15; j++) {
    	if( j<8){
    	enter=i;
    	}else if(j<14){
    	enter=384+i;
    	}// mỗi hàng có 384 byte, chúng ta có 2 hàng

if(thu_tu<144){// vẽ nguyên âm

read_bit_ki_tu=bitRead( pgm_read_byte(vietnamese_vowel_table+ enter+thuong),j%8);//đọc bitmap kí tự
read_bit_dau=bitRead( (pgm_read_byte(vietnamese_vowel_table+so_du+ enter)<<dich_bit),j%8);//đọc bit map dấu

}else{// vẽ phụ âm

read_bit_ki_tu=bitRead( pgm_read_byte(vietnamese_vowel_table+tim_byte_phu_am+ enter),j%8);//đọc bitmap kí tự phụ âm
}
//vẽ 

 if(read_bit_ki_tu||read_bit_dau){// j%8 đọc 2 byte 
	my_drawpixel(x1+i, y1+j, color);
      }else{
      	my_drawpixel(x1+i, y1+j, non);
      }
    }//for
}//for

updateBoundingBox(x1,y1,x1+6, y1+14);
}


/*

read_bit_ki_tu=bitRead( pgm_read_byte(vietnamese_vowel_table+thuong_j),j%8);//đọc bitmap kí tự nguyên âm
read_bit_ki_tu=bitRead( pgm_read_byte(vietnamese_vowel_table+thuong*6+i + enter),j%8);//đọc bitmap kí tự
read_bit_ki_tu=bitRead( pgm_read_byte(vietnamese_vowel_table+denta_j),j%8);//đọc bitmap kí tự phụ âm
read_bit_ki_tu=bitRead( pgm_read_byte(vietnamese_vowel_table+(thu_tu-144)*6+144+i + enter),j%8);//đọc bitmap kí tự
read_bit_dau=bitRead( pgm_read_byte(vietnamese_vowel_table+348+so_du*6+i + enter),j%8);//đọc bit map dấu
read_bit_dau=bitRead( (pgm_read_byte(vietnamese_vowel_table+so_du_j)<<dich_bit),j%8);//đọc bit map dấu

*/



void ST7565::Uni_String(int x1, int y1,PGM_CHAR16T s , bool color) {
int x=x1;
char16_t c;// kiểu char 16 bít(  2 bytes)
  while ((c=pgm_read_word(s)) != 0) {// mối lần c++ thì địa chỉ của kí tự là c[0] do c=c[]
    // Mời bạn tìm hiểu thêm cách lưu biến tĩnh vào bộ nhớ flash trên trang arduino.vn
    Uni_Char(x, y1, c, color);
    //Serial.println(c[i]);
    s++;
    x += 7; // 7 pixels wide
    if (x + 7 >=  LCDWIDTH  ){
      x = x1;    // ran out of this line
      y1+=15;
    }
    if ( y1 > LCDHEIGHT)
      return;        // dừng vòng lặp
  }

}


int ST7565::Keep_Angle(int goc){
	// giữ cho góc trong khoảng 0->360
if(goc<0){
	goc=(-1)*goc;// đổi dấu
 goc=(goc%360);// giữ
	return 360-goc;
	} else{
		return goc%360;
	} 

}

  // mình khá tâm đắc về cái này, hehe
void ST7565::Find_XY_Elip( int x0, int y0,  int a, int b, int  goc_alpha){


goc_alpha=Keep_Angle(goc_alpha);
long L_rad;
float  F_rad, Fx_tich, Fy_tich;


 L_rad= map( goc_alpha, 0, 360, 0,62838);
 F_rad=((float(L_rad))/10000.0);
 // đổi góc sang radian
 Fx_tich=(a*(float(cos(F_rad))));//
 Fy_tich=(b*(float(sin(F_rad)))); // quỹ đạo của y thuộc sin*b (-b->+b)
 
X_ELLIPSE=x0+int(Fx_tich);
Y_ELLIPSE=y0-int(Fy_tich);
}
int ST7565::X_Elip() {
return X_ELLIPSE;


}

int ST7565::Y_Elip() {

	return Y_ELLIPSE;
}

void ST7565::Find_XY_Sphere( int x0, int y0, int a0, int b0, int goc_alpha, int goc_beta){
  
//b1: tìm góc beta dạng số thực
goc_beta=Keep_Angle(goc_beta);
if((goc_beta>90)&&(goc_beta<270)){goc_alpha+=180;}
long L_rad; // long để lưu số  nguyên lớn hơn 32768
float  beta_rad;
 L_rad= map( goc_beta, 0, 360, 0,62838); // map chỉ trả về số nguyên, đổi 360 độ =2*pi=6,2838 rad
 beta_rad=((float(L_rad))/10000.0); // chia cho 10000.0 dạng số thực
// b2: 
float a,b,y;
a=float(a0);
b=float(b0);
y=float(y0);
// hmax=a-b;
float hmax=float(a0-b0);
double ti_so_h=sin(beta_rad);
double ti_so_ab=abs(cos(beta_rad));

int  y_ellipse, a_ellipse, b_ellipse;
y_ellipse=int(y-hmax*ti_so_h);
a_ellipse=int(ti_so_ab*a);
b_ellipse=int(ti_so_ab*b);

Find_XY_Elip(x0, y_ellipse, a_ellipse, b_ellipse, goc_alpha);
X_SPHERE= X_Elip();
Y_SPHERE=Y_Elip();
}
 
 int ST7565::X_Sphere( ){
return X_SPHERE;
 }

int ST7565::Y_Sphere(){
	return Y_SPHERE;
}
// nếu thấy khó hiểu thì bạn có thể tham khảo cái hình minh họa 
// ở trong file đính kèm thư viện này nhé..
// hoàn thiện get_xy: 9:44pm 31/10/2016
// đúng  ngày haloween, người ta có gấu đi chơi tối nay,
// mình thì ngồi nhà hoàn thiện cái code 
// (niềm vui bất tận )__SƠN__
















void ST7565::Analog( int x, int y, int w, int h, int value, bool color){
// mặc dù value mình đặt là unsigned int, nhưng, value chỉ được trong đoạn 0->250
 int Xi,Yi, v ;
bool non=!color;
w+=x;
h+=y;
v= map (value, 0,500, h, y); //value max =500
DrawPixel(w-1, v, color); // vẽ mồi điểm xuất phát tại cột cuối cùng, không thì hàm sẽ không hoạt động


for(Xi=x; Xi<w; Xi++ ){ // Quets hàng ngang
for (Yi=y ; Yi< h; Yi++){ //Quets hàng dọc
if (GetPixel(Xi, Yi)) {// Kiểm tra điểm đó có được vẽ hay không
  DrawPixel(Xi-1, Yi, color);// vẽ sang bên trái 1 điểm tương tự
  DrawPixel(Xi, Yi, non); // Xóa ddiemr cũ bẵng một điểm có màu trắng
break;//thoát quét hàng dọc ngay lập tức

}


} //quét xong hàng dọc



}// quét xong hàng ngang


 }


















void ST7565::duong_nam_ngang( long x1, long x2, long y0,bool color){
	if(x1>x2){
		swap(x1,x2);
	}

	int hieu=x2-x1;
	for( int x=x1; x<=x2; x++){
		my_drawpixel(x,y0, color);
	}
	updateBoundingBox(x1,y0,x1+hieu,y0);
}

void ST7565::duong_thang_dung(long x0, long y1, long y2, bool color){
	if(y1>y2){
		swap(y1,y2);
	}
	int hieu=y2-y1;
	for( int y=y1; y<=y2; y++){
my_drawpixel( x0,y,color);

	}
	updateBoundingBox(x0,y1,x0,y1+hieu);
}


void ST7565::Plot4EllipsePoints(int CX,int  CY,int X, int Y, bool color, bool fill)
{
int X1,X2,Y0A,Y0B;
X1=CX-X;
X2=CX+X;
Y0A=CY+Y;
Y0B=CY-Y;
	if(fill)
	{ // to fill rather than draw a line, plot between the points
		duong_nam_ngang(X1, X2,Y0A,color);
		//drawline(CX+X, CY+Y, CX-X, CY+Y, color);
		duong_nam_ngang(X1,X2, Y0B, color); 
		//drawline(CX-X, CY-Y, CX+X, CY-Y, color);
	}
	else
	{
		DrawPixel(X2, Y0A, color); //{point in quadrant 1}

		DrawPixel(X1, Y0A, color); //{point in quadrant 2}

		DrawPixel(X1, Y0B, color); //{point in quadrant 3}

		DrawPixel(X2, Y0B, color); //{point in quadrant 4}

	}
}

void ST7565::DrawEllipse_private(long CX, long  CY, long XRadius,long YRadius, bool color, bool fill)
{
// ported the algorithm by John Kennedy found at
// http://homepage.smc.edu/kennedy_john/belipse.pdf
//
if((XRadius<1)||(YRadius<1)){
	return;//thoát
} 
  long X, Y;
  long XChange, YChange;
  long EllipseError;
  long TwoASquare,TwoBSquare;
  long StoppingX, StoppingY;
  TwoASquare = 2*XRadius*XRadius;
  TwoBSquare = 2*YRadius*YRadius;
  X = XRadius;
  Y = 0;
  XChange = YRadius*YRadius*(1-2*XRadius);
  YChange = XRadius*XRadius;
  EllipseError = 0;
  StoppingX = TwoBSquare*XRadius;
  StoppingY = 0;

  while ( StoppingX >=StoppingY ) 
  { 
    Plot4EllipsePoints(CX,CY,X,Y,color, fill);

    Y++;
    StoppingY=StoppingY+ TwoASquare;
    EllipseError = EllipseError+ YChange;
    YChange=YChange+TwoASquare;
    if ((2*EllipseError + XChange) > 0 ) {
      X--;
      StoppingX=StoppingX- TwoBSquare;
      EllipseError=EllipseError+ XChange;
      XChange=XChange+TwoBSquare;
    }
  }

  Y = YRadius;
  X = 0;
  YChange = XRadius*XRadius*(1-2*YRadius);
  XChange = YRadius*YRadius;
  EllipseError = 0;
  StoppingY = TwoASquare*YRadius;
  StoppingX = 0;
  while ( StoppingY >=StoppingX )
  {
    Plot4EllipsePoints(CX,CY,X,Y,color, fill);



    X++;
    StoppingX=StoppingX + TwoBSquare;
    EllipseError=EllipseError+ XChange;
    XChange=XChange+TwoBSquare;
    if ((2*EllipseError + YChange) > 0 ) {
      Y--;
      StoppingY=StoppingY- TwoASquare;
      EllipseError=EllipseError+ YChange;
      YChange=YChange+TwoASquare;
    }
  }
}


void ST7565::Elip(long xCenter, long  yCenter, long xRadius,long yRadius, bool color)
{
	DrawEllipse_private(xCenter, yCenter, xRadius,yRadius, color, 0);
}

void ST7565::FillElip(long xCenter, long  yCenter, long xRadius, long yRadius, bool color)
{
	DrawEllipse_private(xCenter, yCenter, xRadius, yRadius, color, 1);
}

void ST7565::Tri(int x1,int y1,int x2,int y2,int x3,int y3, bool color){
	DrawLine(x3,y3,x1,y1,color);
	DrawLine(x1,y1,x2,y2,color);
	DrawLine(x2,y2,x3,y3,color);


}


















void ST7565::FillTri(int x1,int y1,int x2,int y2,int x3,int y3, bool color)
{
int8_t sl,sx1,sx2;
double m1,m2,m3;
	if(y2>y3)
	{
		swap(x2,x3);
		swap(y2,y3);
	}
	if(y1>y2)
	{
		swap(x1,x2);
		swap(y1,y2);
	}
	m1=(double)(x1-x2)/(y1-y2);
	m2=(double)(x2-x3)/(y2-y3);
	m3=(double)(x3-x1)/(y3-y1);
	for(sl=y1;sl<=y2;sl++)
	{
		sx1= m1*(sl-y1)+x1;
		sx2= m3*(sl-y1)+x1;
		if(sx1>sx2)
			swap(sx1,sx2);
		duong_nam_ngang(sx1,sx2,sl,color);
		//drawline(sx1, sl, sx2, sl, color);
	}
	for(sl=y2;sl<=y3;sl++)
	{
		sx1= m2*(sl-y3)+x3;
		sx2= m3*(sl-y1)+x1;
		if(sx1>sx2)
			swap(sx1,sx2);
		duong_nam_ngang(sx1, sx2,sl,color);
		//drawline(sx1, sl, sx2, sl, color);
	}
}









// draw a circle outline
void ST7565::Circle(int x0,int y0,int r,bool color) {

 if(r<1){return;}//thoát

 int f = 1 - r;
 int ddF_x = 1;
 int ddF_y = -2 * r;
 int x = 0;
 int y = r;

  my_drawpixel(x0, y0+r, color);
  my_drawpixel(x0, y0-r, color);
  my_drawpixel(x0+r, y0, color);
  my_drawpixel(x0-r, y0, color);

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
  int x0x=x0+x,x0y=x0+y, x0_x=x0-x, x0_y=x0-y;
  int y0x=y0+x, y0y=y0+y, y0_x=y0-x, y0_y=y0-y;
// 1/2 góc phần tư thứ 1-trên
    my_drawpixel(x0x, y0_y, color);
//1/2 góc phần tư thứ 1-dưới
    my_drawpixel(x0y, y0_x, color);
// 1/2 góc phần tư thứ 2-trên
    my_drawpixel(x0_x, y0_y, color);
// 1/2 góc phần tư thứ 2-dưới
    my_drawpixel(x0_y, y0_x, color);
// 1/2 góc phần tư thứ 3-trên
    my_drawpixel(x0_y, y0x, color);
// 1/2 góc phần tư thứ 3-dưới 
    my_drawpixel(x0_x, y0y, color);
// 1/2 góc phần tư thứ 4-trên 
    my_drawpixel(x0y, y0x, color); 
/// 1/2 góc phần tư thứ 4-dưới 
    my_drawpixel(x0x, y0y, color);

    
  }


  updateBoundingBox(x0-r, y0-r, x0+r, y0+r);

}






void ST7565::FillCircle(int x0,int y0,int r, bool color) {
 if(r<1){return;}//thoát
 int f = 1 - r;
 int ddF_x = 1;
 int ddF_y = -2 * r;
 int x = 0;
 int y = r;

  for (int i=y0-r; i<=y0+r; i++) {
    my_drawpixel(x0, i, color);
  }

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
  int y0y=y0+y;
  int y0x=y0+x;
    for (int i=y0-y; i<=y0y; i++) {
      my_drawpixel(x0+x, i, color);
      my_drawpixel(x0-x, i, color);
      Display();
    } 
    for (int i=y0-x; i<=y0x; i++) {
      my_drawpixel(x0+y, i, color);
      my_drawpixel(x0-y, i, color);
    }    
  }

  updateBoundingBox(x0-r, y0-r, x0+r, y0+r);
}


void ST7565::my_drawpixel(int x, int y, bool color) {
  if ((x >= LCDWIDTH) || (y >= LCDHEIGHT)||(x<0)||(y<0)){

    return;
  }
  // x is which column
  if (color==1) 
    st7565_buffer[x+ (y/8)*LCDWIDTH] |= _BV(7-(y%8));  
  else
    st7565_buffer[x+ (y/8)*LCDWIDTH] &= ~_BV(7-(y%8)); 

}

// the most basic function, set a single pixel
void ST7565::DrawPixel(int x,int y, bool color) {
  if ((x >= LCDWIDTH) || (y >= LCDHEIGHT)||(x<0)||(y<0)){

    return;
  }
/*
  if(x<0){x=0;}
  if(y<0){y=0;}
  if(x>127){x=127;}
  if(y>63){y=63;}*/
  // x is which column
  if (color==1) 
    st7565_buffer[x+ (y/8)*LCDWIDTH] |= _BV(7-(y%8));  
  else
    st7565_buffer[x+ (y/8)*LCDWIDTH] &= ~_BV(7-(y%8)); 

  updateBoundingBox(x,y,x,y);
}


// the most basic function, get a single pixel
// hàm trả về giá trị 0 hoặc 1
//Kiểm tra xem một điểm có tọa độ (x,y) trên màn hình có được vẽ hay không.
//giả sử tô trắng toàn màn hình rồi vẽ 1 điểm lcd.drawpixel(20,20,BLACK)
//đặt biến n=lcd.getpixel(20,20);
//thì n=1.
//Mặc định tất cả các điểm có tọa độ âm, hoặc lớn quá kích cỡ màn hình
//thì n sẽ trả về 0.

bool ST7565::GetPixel(int x,int y) {
  if ((x >= LCDWIDTH) || (y >= LCDHEIGHT)||(x<0)||(y<0))
    return 0;

  return (st7565_buffer[x+ (y/8)*LCDWIDTH] >> (7-(y%8))) & 0x1;  //QUÉT BỘ NHỚ ĐỆM
}
// bresenham's algorithm - thx wikpedia

void ST7565::DrawLine(int x0,int y0,int x1,int y1,bool color) {
	if(x0==x1){
		duong_thang_dung(x0,y0,y1,color);
		return;
	}
	if(y0==y1){
		duong_nam_ngang(x0,x1,y0,color);
		return;
	}

 int steep = abs(y1 - y0) > abs(x1 - x0);

  

  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }

  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }


int dx=0, dy=0;
  dx = x1 - x0;
  dy = abs(y1 - y0);
 int err = dx / 2;
 int ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;}

  for (; x0<=x1; x0++) {
    if (steep) {
      DrawPixel(y0, x0, color);

    } else {
      DrawPixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }


}

// filled rectangle
void ST7565::FillRect(int x,int y,int w,int h, bool color) {

  // stupidest version - just pixels - but fast with internal buffer!
	w+=x;
	h+=y;

  for (int i=x; i<w; i++) {
    for (int j=y; j<h; j++) {
      my_drawpixel(i, j, color);
    }
  }

  updateBoundingBox(x, y, w, h); 


}

// draw a rectangle
void ST7565::Rect(int x,int y,int w,int h, bool color) {
  // stupidest version - just pixels - but fast with internal buffer!
	w+=x;
	h+=y;
  for (int i=x; i<w; i++) {
    my_drawpixel(i, y, color);
    my_drawpixel(i, h-1, color);
  }
  for (int i=y; i<h; i++) {
    my_drawpixel(x, i, color);
    my_drawpixel(w-1, i, color);
  } 

  updateBoundingBox(x, y, w, y);// cạnh trên
  updateBoundingBox(x, h, w, h);// cạnh dưới
  updateBoundingBox( x,y, x, h);//cạnh trái
  updateBoundingBox(w,y, w, h);// cạnh phải
 
}

// draw a rect have circle corner



void ST7565::Corner(int x,int y,int w,int h,int r, bool color){ 
// Thủ thuật 
//vẽ 4 cạnh hcn + 4 góc phần tư của hình tròn
//

if( (r>h/2)||(r>w/2)){
	return;// thoát
}
int xr=x+r, xw=x+w, xw_r=x+w-r;
int yr=y+r, yh=y+h, yh_r=y+h-r;
//cạnh trên
	duong_nam_ngang(xr, xw_r,y, color);
// cạnh trái
	duong_thang_dung(x, yr, yh_r,color);
//cạnh dưới
	duong_nam_ngang( xr, xw_r, yh,color);
// cạnh phải
	duong_thang_dung(xw, yr, yh_r,color);
	if(r<1){
		
  return;//thoát
	}
// Bắt đầu vẽ góc
 int f = 1 - r;
 int ddF_a = 1;
 int ddF_b = -2 * r;
 int a = 0;
 int b = r;
  while (a<b) { 
if (f >= 0) {
      b--;
      ddF_b += 2;
      f += ddF_b;
    }
    a++;
    ddF_a += 2;
    f += ddF_a;
  
int xw_ra=xw_r +a, xw_rb=xw_r+b;
 int xr_a=xr-a, xr_b=xr-b;
 int yr_a=yr-a, yr_b=yr-b;
 int yh_ra=yh_r+a, yh_rb=yh_r+b;
// bo góc phải- trên
    my_drawpixel( xw_ra, yr_b, color);
    my_drawpixel(xw_rb, yr_a, color);
// bo góc trái-trên
    my_drawpixel(xr_a, yr_b, color);
    my_drawpixel(xr_b, yr_a, color);
// bo góc trái dưới
    my_drawpixel(xr_b, yh_ra, color);
    my_drawpixel(xr_a, yh_rb, color);
// bo góc phải- dưới 
    my_drawpixel(xw_rb, yh_ra, color); 
    my_drawpixel(xw_ra, yh_rb, color);

    
  }

  updateBoundingBox(x,y,x+w, y+h); //update screen faster
}

void ST7565::FillCorner(int x,int y,int w,int h,int r, bool color){ 
// đành lười biếng kiểu này
// vẽ 4 hình tròn
	int xw_r=x+w-r, yr=y+r, xr=x+r, yh_r=y+h-r;
	int w_2r=w-2*r, h_2r=h-2*r;
FillCircle(xw_r, yr, r, color);
FillCircle(xr, yr, r, color);
FillCircle(xr, yh_r, r,color);
FillCircle(xw_r, yh_r, r,color);
// vẽ hình chữ nhật chèn vào
FillRect(xr, y, w_2r, h+1, color);
FillRect(x, yr, r, h_2r, color);
FillRect(xw_r, yr, r+1,h_2r,color);

  updateBoundingBox(x,y,x+w, y+h); //update screen faster
// xong



}

///////////////////////////////////////////////////////////////////
byte ST7565::Pullup_4(byte right_pin,  byte up_pin, byte left_pin, byte down_pin){
// nếu nút dược nhấn thì đọc bằng 0, mặc định =1

if ((digitalRead(right_pin)==1)&&(digitalRead(up_pin)==1)&&(digitalRead(left_pin)==1)&&(digitalRead(down_pin)==1)){
  return 0; // 0 nút nào được nhấn
} 
else{//có 1 hoặc 2 nút được nhấn 
    if(digitalRead(right_pin)==0){//  right được nhấn
            if(digitalRead(up_pin)==0){ return 20; }// (1*2)*10---nhấn cả up
            if(digitalRead(left_pin)==0){return 30;}// (1*3)*10---nhán cả left
            if(digitalRead(down_pin)==0){ return 40;}//(1*4)*10---nhấn cả down
        return 1;//chỉ mỗi nút right
    }//nếu chưa thỏa mãn thì duyệt tiếp
    if(digitalRead(up_pin)==0){//nếu up được nhấn 
             if(digitalRead(left_pin)==0){ return 60;}// (2*3)*10---nhấn cả left
             if(digitalRead(down_pin)==0){return 80;}//(2*4)*10---nhấn cả down
         return 2;//chỉ mỗi nút up
     }// nếu chưa thỏa mãn thì duyệt tiếp
    if(digitalRead(left_pin)==0){// left được nhấn
             if (digitalRead(down_pin)==0){return 120;}// (3*4)*100---nhấn cả down
         return 3; //chỉ mỗi left

    }
// nếu chưa thỏa mãn thì đây phải là down
    return 4;// chỉ mỗi nút down
}//đóng else

}// đóng 4button_pullup




///////////////////////////////////////////////////////////////////////////

void ST7565::st7565_init(void) {
  // set pin directions
  pinMode(sid, OUTPUT);
  pinMode(sclk, OUTPUT);
  pinMode(a0, OUTPUT);
  pinMode(rst, OUTPUT);
  pinMode(cs, OUTPUT);

  // toggle RST low to reset; CS low so it'll listen to us
  if (cs > 0)
    digitalWrite(cs, LOW);

  digitalWrite(rst, LOW);
  _delay_ms(500);
  digitalWrite(rst, HIGH);

  // LCD bias select
  st7565_command(CMD_SET_BIAS_7);
  
  // Initial display line
  st7565_command(CMD_SET_DISP_START_LINE);
//Khởi động màn hình 
  // turn on voltage converter (VC=1, VR=0, VF=0)
  st7565_command(CMD_SET_POWER_CONTROL | 0x4);
  // wait for 50% rising
  _delay_ms(50);

  // turn on voltage regulator (VC=1, VR=1, VF=0)
  st7565_command(CMD_SET_POWER_CONTROL | 0x6);
  // wait >=50ms
  _delay_ms(50);

  // turn on voltage follower (VC=1, VR=1, VF=1)
  st7565_command(CMD_SET_POWER_CONTROL | 0x7);
  // wait
  _delay_ms(10);


  // initial display line
  // set page address
  // set column address
  // write display data

  // set up a bounding box for screen updates

  updateBoundingBox(0, 0, LCDWIDTH-1, LCDHEIGHT-1);
}

inline void ST7565::spiwrite(uint8_t c) {
    
#if not defined (_VARIANT_ARDUINO_DUE_X_) && not defined (_VARIANT_ARDUINO_ZERO_)
    shiftOut(sid, sclk, MSBFIRST, c);
#else
   int i;
    for (i=7; i>=0; i--) {
        digitalWrite(sclk, LOW);
        delayMicroseconds(5);      //need to slow down the data rate for Due and Zero
        if (c & _BV(i))
            digitalWrite(sid, HIGH);
        else
            digitalWrite(sid, LOW);
  //      delayMicroseconds(5);      //need to slow down the data rate for Due and Zero
        digitalWrite(sclk, HIGH);
    }
#endif
}
void ST7565::st7565_command(uint8_t c) {
  digitalWrite(a0, LOW);

  spiwrite(c);
}

void ST7565::st7565_data(uint8_t c) {
  digitalWrite(a0, HIGH);

  spiwrite(c);
}

//-----------------/////////////////////////////////////////////////
void ST7565::SET(byte contrast, bool negative, bool rotation, bool mirror, byte tyledientro) {
  
    st7565_command(CMD_SET_VOLUME_FIRST);
    st7565_command(CMD_SET_VOLUME_SECOND | contrast);


if (negative){ st7565_command(CMD_SET_DISP_REVERSE );
} else
{  st7565_command(CMD_SET_DISP_NORMAL);
} //âm bản


if (rotation){ 
st7565_command(CMD_SET_COM_REVERSE );
} else
{ 
 st7565_command(CMD_SET_COM_NORMAL );
 }//quay ngang
 
if (mirror){ 
st7565_command(CMD_SET_ADC_REVERSE );
} else
{  st7565_command( CMD_SET_ADC_NORMAL );
 }// gương

  /** tỷ lệ điện trở R1/R2 , điều chỉnh điện áp hoạt động của lcd với biến val trong khoảng từ 0x0 đến 0x6
*/
    st7565_command(CMD_SET_RESISTOR_RATIO | tyledientro );

}

void ST7565::AllPixel(bool val) {
if (val){ 
st7565_command( CMD_ALL_PIXEL );
} else
{  st7565_command(CMD_NONE_ALL_PIXEL );
 }

} //ép hiển thị tất cả các điểm trên màn hình







/////////////////////////////////////////////////////////////////////////////////////

void ST7565::Display(void) {
 uint8_t col, maxcol;
byte p;
  /*
  Serial.print("Refresh ("); Serial.print(xUpdateMin, DEC); 
  Serial.print(", "); Serial.print(xUpdateMax, DEC);
  Serial.print(","); Serial.print(yUpdateMin, DEC); 
  Serial.print(", "); Serial.print(yUpdateMax, DEC); Serial.println(")");
  */

  for(p = 0; p < 8; p++) {
    /*
      putstring("new page! ");
      uart_putw_dec(p);
      putstring_nl("");
    */
#ifdef enablePartialUpdate
    // check if this page is part of update
    if ( yUpdateMin >= ((p+1)*8) ) {
      continue;   // nope, skip it!
    }
    if (yUpdateMax < p*8) {
      break;
    }
#endif

    st7565_command(CMD_SET_PAGE | pagemap[p]);


#ifdef enablePartialUpdate
    col = xUpdateMin;
    maxcol = xUpdateMax;
#else
    // start at the beginning of the row
    col = 0;
    maxcol = LCDWIDTH-1;
#endif

    st7565_command(CMD_SET_COLUMN_LOWER | ((col) & 0xf));
    st7565_command(CMD_SET_COLUMN_UPPER | ((col >> 4) & 0xf));
    st7565_command(CMD_RMW);
    
    for(; col <= maxcol; col++) {
      //uart_putw_dec(col);
      //uart_putchar(' ');
      st7565_data(st7565_buffer[(LCDWIDTH*p)+col]);
    }
  }

#ifdef enablePartialUpdate
  xUpdateMin = LCDWIDTH - 1;
  xUpdateMax = 0;
  yUpdateMin = LCDHEIGHT-1;
  yUpdateMax = 0;
#endif
}


// clear everything
void ST7565::Clear(void) {
  memset(st7565_buffer, 0, 1024);
  updateBoundingBox(0, 0, LCDWIDTH-1, LCDHEIGHT-1);
}

void ST7565::Clear(int x0, int y0, int width, int high){
	width+=x0;
	high+=y0;
	for(int x=x0;x<width; x++){
		for(int y=y0; y<high; y++){
			my_drawpixel(x,y,WHITE);
		}
	}
	updateBoundingBox(x0,y0,width,high);
}

void ST7565::clear_display(void) {
  byte p, c;
  
  for(p = 0; p < 8; p++) {
    /*
      putstring("new page! ");
      uart_putw_dec(p);
      putstring_nl("");
    */

    st7565_command(CMD_SET_PAGE | p);
    for(c = 0; c < 129; c++) {
      //uart_putw_dec(c);
      //uart_putchar(' ');
      st7565_command(CMD_SET_COLUMN_LOWER | (c & 0xf));
      st7565_command(CMD_SET_COLUMN_UPPER | ((c >> 4) & 0xf));
      st7565_data(0x0);
    }     
  }

}// KẾT THÚC
/* Công nghệ phát triển từng phút, đừng quá gò bó bản thân mà kìm hãm chính mình bạn nhé(tác giả)
*/