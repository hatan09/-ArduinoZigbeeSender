#include "TRANSMIT_CLASS.h"

//Truyền 1 hàm đọc 1 byte dữ liệu 
uint8_t RR() { 
  return (uint8_t) Serial.read();   
}

// Truyền 1 hàm đọc kích thước sẵn có
uint32_t AV() {  
  return (uint32_t)Serial.available();
}

// Truyền 1 hàm gửi 1 byte dữ liệu
uint8_t COUNT_OVER=0; // dùng để làm trễ 
void WB(uint8_t c) { 
  Serial.write(c); 
  COUNT_OVER ++; 
  if(COUNT_OVER> 50){ 
    delay(10); COUNT_OVER=0;  
  }
}


//làm gì  đó để clear buffer vào đây, ví dụ :
void CL() {
  uint32_t Buffer_available= AV() ;
  for( uint32_t i=0; i< Buffer_available ; i++){  
    RR();
  }//đọc hết nhũng gì chưa đọc
}


TRANSMIT_CLASS Master(&RR, &AV, &CL, &WB);//Gửi 

DATA_TRANSMIT_PACKET A("abc");// tạo 1 gói tin với tên "abc"
DATA_TRANSMIT_PACKET *PACKET_ALL[] = {&A};//TỔNG HỢP GÓI 

//******************************************************************************************************************
void setup() {
  /*
  //bạn có thể thay đổi lại tùy thích ở đây
    Master.ADD_READ_BYTE( &RR);
    Master.ADD_AVAILABLE(  &AV);
    Master.ADD_CLEAR( &CL);
    Master.ADD_WRITE_BYTE(&WB);
  */

  Serial.begin(9600);
  

}


uint8_t text_array_buffer[100];//tạo mảng buffer lớn hơn kích thước String 
//bắt buộc kiểu uint8_t
 uint16_t  from_string_to_array(String t){
   // phân giải String về mảng
  for(uint16_t i=0; i<t.length() ; i++){
     text_array_buffer[i]=(uint8_t)t.charAt(i);
  }
  return t.length();//trả về độ dài (kích thước ) String
}
void loop() {
  //ON
  String text_st="turn_led_on";
  uint16_t size_=from_string_to_array(text_st); //get size and put String to the variable used to send data

  Master.SEND_DATA_ARRAY(
        A, //gửi gói tin A ("abc")
        (uint8_t)(0), // mảng kiểu (uint8_t)  
        size_,// kích thước cần gửi
        text_array_buffer // mảng cần gửi
      );

  // khi arduino đang ở chế độ gửi 
  //  thì không nên sử dụng   Serial để xuất dữ liệu kiểm tra (nó sẽ gây chồng chéo dữ liệu gửi)
  delay(3000);

  //OFF
  text_st = "turn_led_off";
  size_ = from_string_to_array(text_st); //get size and put String to the variable used to send data

  Master.SEND_DATA_ARRAY(
        A, //gửi gói tin A ("abc")
        (uint8_t)(0), // mảng kiểu (uint8_t)  
        size_,// kích thước cần gửi
        text_array_buffer // mảng cần gửi
      );
  
  delay(3000);

}
