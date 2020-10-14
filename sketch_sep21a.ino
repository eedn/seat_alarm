// made by 조현우
// 의자에 오래 앉으면 진동이 울리는 장치.
// 45분 앉으면 30초 이상 일어서 있도록 유도함. 일어서서 스트레칭 권장.
int SensorPin = A0; //analog pin 0
int seat_cnt = 0;
int seat_cnt_SEC30 = 0;
int unseat_cnt = 0;
int INTERVAL = 500; //체크 간격 0.5초
int SEC30;
int sens_history = 0;
int THRESHOLD = 11;

void setup(){
  Serial.begin(9600);
  SEC30 = 30000/INTERVAL; //30초 루프 횟수
  analogWrite(3,0); //진동 세기 0
}
 
void loop(){
  
  int SensorReading = analogRead(SensorPin); 
 
  int mfsr_r18 = map(SensorReading, 0, 1024, 0, 255);
  
  if(mfsr_r18 >= THRESHOLD){ //앉으면
    if(sens_history < THRESHOLD){ //일어섰다가 -> 앉은거면 신호
      analogWrite(3,200);
      delay(200);
      analogWrite(3,0);
    }
    
    seat_cnt++;
    unseat_cnt = 0;

    if(seat_cnt > SEC30){ //30초 넘게 앉아 있으면
      seat_cnt_SEC30++;
      seat_cnt = 0;
    }

    // *****진동*****
    if(seat_cnt_SEC30 >= 90){ // 45분 이상 앉으면
      analogWrite(3,200); //200세기의 진동
      seat_cnt--; //seat_cnt가 오버플로 나지 않도록.
    }
  }
  else{ //일어서면
    if(sens_history >= THRESHOLD){ //앉았다가 -> 일어선거면 신호
      analogWrite(3,200);
      delay(200);
      analogWrite(3,0);
      delay(80);
      analogWrite(3,150);
      delay(150);
      analogWrite(3,0);
    }
    
    unseat_cnt++;
    analogWrite(3,0); //0세기의 진동
    
    if(unseat_cnt > SEC30){ //30초 넘게 일어서있으면
      seat_cnt = 0;
      seat_cnt_SEC30 = 0;
      unseat_cnt--; //unseat_cnt가 오버플로 나지 않도록.
    }
  }

  Serial.print("seat_cnt: "); Serial.print(seat_cnt);
  Serial.print("\tseat_cnt_SEC30: "); Serial.println(seat_cnt_SEC30);
  Serial.print("unseat_cnt: "); Serial.print(unseat_cnt);
  Serial.print("\tVALUE: ");Serial.println(mfsr_r18);

  
  sens_history = mfsr_r18;
  delay(INTERVAL); //0.5초마다 확인
}
