#define sensor_right A0 //ประกาศตัวแปรชื่อ sensor_right มีค่าเท่ากับ A0
#define sensor_left A1  //ประกาศตัวแปรชื่อ sensor_left มีค่าเท่ากับ A1

int detect = 33; //กำหนดค่าสีระหว่างสีดำและสีขาว เช่นตัวอย่าง สีดำ 36+ และ สีขาว 26-30 จึงตั้งไว้ 33 , สีดำ 500+ และ สีขาว 100-300 จึงตั้งไว้ 420 เป็นต้น 

void setup() //ส่วนของ void Setup จะทำงานเพียงครั้งแรกที่เปิดเครื่อง
{
  Serial.begin(9600); //กำหนด Board Rate เพื่อที่จะสามารถอ่านค่าต่างๆที่ Arduino ส่งกลับมาให้คอมพิวเตอร์ได้

  pinMode(10, OUTPUT);    //ประกาศขา 10 เป็น OUTPUT
                          //ซึ่งขา 10 คือขาควบคุมความเร็วมอเตอร์ MA บนบอร์ด shield
  pinMode(11, OUTPUT);    //ประกาศขา 11 เป็น OUTPUT
                          //ซึ่งขา 11 คือขาควบคุมความเร็วมอเตอร์ MB บนบอร์ด shield

  pinMode(12, OUTPUT);    //ประกาศขา 12 เป็น OUTPUT
                          //ซึ่งขา 12 คือขาควบคุมทิศทางมอเตอร์ MA บนบอร์ด shield
  pinMode(13, OUTPUT);    //ประกาศขา 13 เป็น OUTPUT
                          //ซึ่งขา 13 คือขาควบคุมทิศทางมอเตอร์ MB บนบอร์ด shield

  pinMode(A11, OUTPUT);   //ประกาศขา A11 เป็น OUTPUT
  pinMode(A15, OUTPUT);   //ประกาศขา A15 เป็น OUTPUT
  digitalWrite(A11,1);    //ประกาศขา A11 เป็น 1 หรือ High คือ จ่ายไฟ 5V ออกมาที่ขา A11 เพื่อเข้าขา VCC ของ Ultrasonic
  digitalWrite(A15,0);    //ประกาศขา A15 เป็น 0 หรือ Low คือ จ่ายไฟ 0V ออกมาที่ขา A15 เพื่อเข้าขา GND ของ Ultrasonic

  delay(500);
  
  do //ฟังชั่น Do while คือการทำงานใน Do ก่อน แล้วจึงเช็คค่าถูกต้องตาม While หรือไม่ ถ้าถูก จะกลับมาทำ Do ใหม่ ถ้าผิดก็จะออกจาก do while loop
  {
    forward(100);         //เดินหน้าด้วยความเร็ว 100
  }
  while(analogRead(sensor_right) < detect);   //ค่อยเช็คค่าสีของ sensor_right (sensor ที่ต่อกับขา A0) ถ้าค่า sensor น้อยกว่าค่าสีที่ตั้งไว้
  
  break_motor(); delay(100);  //เบรคมอเตอร์และหน่วงเวลา 100ms
  backward(150); delay(300);  //ถอยหลังด้วยความเร็ว 150 และหน่วงเวลา 100ms
  tr_left(150); delay(1000);  //หมุนซ้ายด้วยความเร็ว 150 และหน่วงเวลา 100ms

  //การสั่งคำสั่งใดคำสั่งหนึ่งไว้ แล้วหน่วงเวลา จะเปรียบเสมือนกับการสั่งคำสั่งนั้นตลอดช่วงเวลาที่หน่วง 
  //อาทิ เช่น backward(150); delay(300); ก็เปรียบเสมือนกับ ถอยหลัง 300ms นั้นเอง
}

void loop() //ส่วนของ void loop จะทำงานซ้ำไปเรื่อยๆ จนกว่าจะไม่มีไฟ 
{
  if (analogRead(sensor_right) < detect && analogRead(sensor_left) < detect)    //ถ้า ค่าสี sensor_right น้อยกว่า ค่าสีที่ตั้งไว้ (สีขาว) และ(&&) ค่าสี sensor_left น้อยกว่า ค่าสีที่ตั้งไว้ (สีขาว)
  {
    int SonicRead = sonic(A12,A13); //ประกาศตัวแปรประเภทจำนวนเต็ม ชื่อ SonicRead มีค่าเท่ากับ ฟังชั่น sonic(ขา Trig , ขา Echo) ซึ่งจะเป็นค่าของระยะทางระหว่างตัวหุ่นยนต์กับวัตถุตกกระทบ มีหน่วยเป็น cm
    if(SonicRead < 60)  //ถ้าค่าที่อ่านได้น้อยกว่า 60 cm
    {
      forward(255); //เดินหน้าด้วยความเร็ว 255
    }
    else                //ถ้าไม่ใช่
    {
      forward(100); //เดินหน้าด้วยความเร็ว 100
    }
  }
  else if(analogRead(sensor_right) > detect && analogRead(sensor_left) < detect)  //หรือไม่ ถ้า ค่าสี sensor_right มากกว่า ค่าสีที่ตั้งไว้ (สีดำ) และ ค่าสี sensor_left น้อยกว่า ค่าสีที่ตั้งไว้ (สีขาว)
  {
    backward(150); delay(300);  //ถอยหลังด้วยความเร็ว 150 และหน่วงเวลา 300ms
    tr_left(150);delay(500);    //หันซ้ายด้วยความเร็ว 150 และหน่วงเวลา 500ms
  }
  else if(analogRead(sensor_right) < detect && analogRead(sensor_left) > detect)  //หรือไม่ ถ้า ค่าสี sensor_right น้อยกว่า ค่าสีที่ตั้งไว้ (สีขาว) และ ค่าสี sensor_left มากกว่า ค่าสีที่ตั้งไว้ (สีดำ)
  {
    backward(150); delay(300);  //ถอยหลังด้วยความเร็ว 150 และหน่วงเวลา 300ms
    tr_right(150);delay(500);   //หันขวาด้วยความเร็ว 150 และหน่วงเวลา 500ms
  }

}

//ในส่วนของฟังชั่นต่างๆ ที่ใช้ในโปรแกรม เพื่ออำนวยความสะดวกในการแก้ไข
//จะแบ่งออกได้ประมาณ 2 ประเภท คือฟั่งชั่นที่ไม่มีการส่งค่าออก และ ฟั่งชั่นที่มีการส่งค่าออก
//ฟังชั่นที่ไม่มีการส่งค่าออก จะขึ้นต้นด้วย void โดยจะมีโครงสร้างดังนี้
// void name (input_n1,input_n2,.....intput_n)
//จะสามารเพิ่มจำนวน input ได้เรื่อยๆ 

void forward(int speeds)  //ฟังชั่น forward รับค่าประเภทจำนวนเต็ม ชื่อตัวแปร speeds 
{
  digitalWrite(12, 0);  //กำหนดทิศทางการหมุนของมอเตอร์ MA
  digitalWrite(13, 0);  //กำหนดทิศทางการหมุนของมอเตอร์ MB

  analogWrite(10, speeds);  //กำหนดความเร็วในการหมุนของมอเตอร์ MA โดยอาจอิงจาก input ที่เราใส่มาในวงเล็บ
  analogWrite(11, speeds);  //กำหนดความเร็วในการหมุนของมอเตอร์ MB โดยอาจอิงจาก input ที่เราใส่มาในวงเล็บ
}

void backward(int speeds) //ฟังชั่น backward รับค่าประเภทจำนวนเต็ม ชื่อตัวแปร speeds 
{
  digitalWrite(12, 1);  //กำหนดทิศทางการหมุนของมอเตอร์ MA
  digitalWrite(13, 1);  //กำหนดทิศทางการหมุนของมอเตอร์ MB

  analogWrite(10, speeds);  //กำหนดความเร็วในการหมุนของมอเตอร์ MA โดยอาจอิงจาก input ที่เราใส่มาในวงเล็บ
  analogWrite(11, speeds);  //กำหนดความเร็วในการหมุนของมอเตอร์ MB โดยอาจอิงจาก input ที่เราใส่มาในวงเล็บ
}

void tr_right(int speeds)   //ฟังชั่น tr_right รับค่าประเภทจำนวนเต็ม ชื่อตัวแปร speeds 
{
  digitalWrite(12, 0);  //กำหนดทิศทางการหมุนของมอเตอร์ MA
  digitalWrite(13, 1);  //กำหนดทิศทางการหมุนของมอเตอร์ MB

  analogWrite(10, speeds);  //กำหนดความเร็วในการหมุนของมอเตอร์ MA โดยอาจอิงจาก input ที่เราใส่มาในวงเล็บ
  analogWrite(11, speeds);  //กำหนดความเร็วในการหมุนของมอเตอร์ MB โดยอาจอิงจาก input ที่เราใส่มาในวงเล็บ
}

void tr_left(int speeds)  //ฟังชั่น tr_left รับค่าประเภทจำนวนเต็ม ชื่อตัวแปร speeds 
{
  digitalWrite(12, 1);  //กำหนดทิศทางการหมุนของมอเตอร์ MA
  digitalWrite(13, 0);  //กำหนดทิศทางการหมุนของมอเตอร์ MB

  analogWrite(10, speeds);  //กำหนดความเร็วในการหมุนของมอเตอร์ MA โดยอาจอิงจาก input ที่เราใส่มาในวงเล็บ
  analogWrite(11, speeds);  //กำหนดความเร็วในการหมุนของมอเตอร์ MB โดยอาจอิงจาก input ที่เราใส่มาในวงเล็บ
}

void break_motor()
{
  analogWrite(10, 0); //กำหนดความเร็วในการหมุนของมอเตอร์ MA เป็น 0
  analogWrite(11, 0); //กำหนดความเร็วในการหมุนของมอเตอร์ MB เป็น 0
}

//ฟังชั่นที่มีการส่งค่าออก จะมีลักษณะโครงสร้างคล้ายกับ void แต่จะขึ้นต้นด้วย ประเภทของตัวแปรที่ต้องการส่งออก โดยจะมีโครงสร้างดังนี้
// DataTypes name (input_n1,input_n2,.....intput_n)
//จะสามารเพิ่มจำนวน input ได้เรื่อยๆ 
int sonic(int Trig, int Echo)  //ฟังชั่นอ่านค่า Ultrasonic โดยระบุขา Trig และ Echo ลงในวงเล็บตามลำดับ
{
  long duration;
  int cm;
  //ในส่วนที่เป็นส่วนกำเนิดความถี่เพื่อใช้ในการตรวจจับ
  pinMode(Trig, OUTPUT);
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(5);
  digitalWrite(Trig, LOW);
  pinMode(Echo, INPUT);
  duration = pulseIn(Echo, HIGH);
  cm = (duration / 29 / 2);
  
  Serial.print(cm); //แสดงค่าที่วัดได้ อยู่ในตัวแปร cm มีหน่วยเป็นเซนติเมตร
  Serial.print("cm");
  Serial.println();

  return cm; //ส่งค่า cm กลับไปที่ sonic จึงทำให้ sonic = cm
}
