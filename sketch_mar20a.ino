#include <Servo.h> 
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  
Servo ServoMotor;

int password_position = 0; //ตำแหน่งของ password
int Servo_Position = 0; //ตำแหน่งที่จะปลดล็อค
char password [6]; //เก็บ password 6 ตัว
char unlock[6]; //ใช้เวลาปลดล็อค
char key; //เก็บข้อมูล ไว้ใช้ตอนดึงข้อมูล

const byte ROWS = 4; // 4 แถวของ keypad
const byte COLS = 3; // 3 คอลัมน์ของ keypad
char keys[ROWS][COLS] = { //กำหนดอาเรย์ 2 มิติ ของหน้าปุ่ม keypad
{'1','2','3'},
{'4','5','6'},
{'7','8','9'},
{'*','0','#'}
};
byte rowPins[ROWS] = {8, 7, 6, 5}; //ตำแหน่งขาที่เชื่อมต่อของแุถว
byte colPins[COLS] = {4, 3, 2}; //ตำแหน่งขาที่เชื่อมต่อของคอน์ลัม
 
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS ); //เซ็ตค่าเพื่อให้ไลบารี่รู้จัก

void setup()
{
  ServoMotor.attach(9); //ขาที่เชื่อมต่อ
  ServoMotor.write(Servo_Position);
  lcd.init(); //สั่งให้จอเริ่มทำงาน
  lcd.backlight(); //เปิดไฟจอ
}
 
void loop()
{
     //เช็คว่าล็อคไม่
  if(Servo_Position == 0 && password_position == 0){ 
    do{
      password_position = 0;
      lcd.setCursor(0,0);
      lcd.print(" Press * to Set");
      lcd.setCursor(0,1);
      lcd.print("    Password");
    }while(keypad.getKey() != '*'); //อ่าน keypad ว่ามีการกด * ไหม
    
     //เมื่อมีการกด * จะออกจาก do while มาเซ็ต password
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SetPasswrod 6DIT");
    
     //เซ็ต password
    while(password_position<6){
      lcd.setCursor(password_position,1);
      key = keypad.getKey();  //รับค่า keypad เก็บไว้ใน key
      
      // กด # เพื่อลบเวลากรอกผิด
      if (key == '#'){
        if(password_position > 0){
          password_position--;
          lcd.setCursor(password_position,1);
          lcd.print(" ");
        }
      }else if(key != NO_KEY){  //มีการกด
        password[password_position] = key; //เก็บค่าไว้ใน password
        lcd.print("*");
        password_position++;
      }
    } 
    Servo_Position = 160;
    ServoMotor.write(Servo_Position); //ให้ servo หมุนไปตำแหน่งที่กำหนด
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" Lock Completed");
    delay(3000);
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Press Password");
  delay(3000);
  
   //การปลดล็อค
  if(Servo_Position == 160){
    int num = 0;
    while(num <6 ){
      lcd.setCursor(num,1);
      key = keypad.getKey();  //รับค่า keypad เก็บไว้ใน key
       // กด # เพื่อลบเวลากรอกผิด
      if(key == '#'){
        if(num > 0){
          num--;
          lcd.setCursor(num,1);
          lcd.print(" ");
        }
      }else if(key != NO_KEY){  //มีการกด
        unlock[num] = key;  //เก็บค่าไว้ใน unlock
        lcd.print("*");
        num++;
      }
    }
      //เช็ค passwrod
    int check = 0;
    while(check < 6){
      if(unlock[check] == password[check]){ //เช็คทีละตัว
        check++;
        
      //เช็คแล้วไม่ตรง  
      }else{
        num = 0;
        check = 7;
        Servo_Position = 160;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  The Password");
        lcd.setCursor(0,1);
        lcd.print("  is Incorrect");
        delay(3000);
        lcd.clear();
      }
    }
      //ปลดล็อค
    if(check == 6){
      Servo_Position = 0;
      ServoMotor.write(Servo_Position);
      password_position = 0;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(" UnLock Completed");
      delay(3000);
    }
  }
}