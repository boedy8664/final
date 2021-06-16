#include"mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"

//void gogo(Arguments *in, Reply *out);
//RPCFunction rpcgogo(&gogo, "gogo");
BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(D1,D0); //tx,rx
BufferedSerial xbee(D10,D9);
Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
BBCar car(pin5, pin6, servo_ticker);
DigitalOut led1(LED1);
DigitalInOut ping(D12);
Thread t, t1;
Timer tt;
void turn();
char at[18], line[13];
int i = 0;
int j = 0;
int st = 0, stt = 0, sttt = 0;
int step = 1;
int END = 0;
int change = 0;
int c = 0;


void turn() {
    char buff[20];
   
    if (step== 2) {
        stt = 1;
        sprintf(buff, "Turn 90 degree\r\n");
        xbee.write(buff,16);
       // printf("EEEND = %d", END);
        car.follow(30, -30, 0.5, 1);
        ThisThread::sleep_for(1700ms);
        car.stop();
        ThisThread::sleep_for(100ms);
        car.goStraight(30);
        ThisThread::sleep_for(1500ms);
   // step = 0;
    }
}

void Data() {
    float Tx, Tz, Ry;
    int x1, x2, del;
    char buff[20];
    pc.set_baud(9600);
    car.stop();
  
    
    while(1) {
        if(line[0] == 'L') {
            x1 = 100*(int(line[1])-48) + 10*(int(line[2])-48) + int(line[3])-48;
            x2 = 100*(int(line[4])-48) + 10*(int(line[5])-48) + int(line[6])-48;
        }
        if(at[0] == 'A') {
            Tx = (int(at[1])-48)*((int(at[2])-48) + 0.1*(int(at[4])-48) + 0.01*(int(at[5])-48));
            Tz = -1*(10*(int(at[7])-48) + (int(at[8])-48) + 0.1*(int(at[10])-48) + 0.01*(int(at[11])-48));
            Ry = 100*(int(at[12])-48) + 10*(int(at[13])-48) + (int(at[14])-48) + 0.1*(int(at[16]-48)) + 0.01*(int(at[17])-48);
        }
        c++;
        printf(" step = %d, st = %d, stt = %d, sttt = %d", step,st, stt, sttt); 
        del = x2-x1;
        step = 1;
        if(st == 0) {
            sprintf(buff, "Line Follow\r\n");
            xbee.write(buff,13);
            st = 1;
            //ThisThread::sleep_for(100ms);
        }
        if (abs(Tz) >=  5.5 && step >= 1) {
          //  printf("go");
            if (del<0) {
                del = -del;
                car.follow(20,20+del/2,0.4,1);
               // car.goStraight(100);
                ThisThread::sleep_for(100ms);
            }
            else {
               // del = -del;
                car.follow(20+del/2,20,0.5,1);
               // car.goStraight(-100);
                ThisThread::sleep_for(100ms);
            }
        }
        else if(abs(Tz) < 5.5 && c>5) {
            step++;
            if(END == 0) {
              //  step = 1;
                END=1;
                turn();
            }
            else {
                sttt = 1;
                car.stop();
                ThisThread::sleep_for(1s);
                sprintf(buff, "Goal!!!\r\n");
                xbee.write(buff,9);
            }

        }
        if(line[0] == 'N') car.stop();
        ThisThread::sleep_for(100ms);
    }
}

int main(){
    uart.set_baud(9600);
    t.start(Data);
    while(1){
        
        if(uart.readable()){
            char recv[1];
            uart.read(recv, sizeof(recv));

            if(recv[0] == 'L') change = 0;
            if(recv[0] == 'A') change = 1;
            
            if (change == 0) {
               if (i != 12) {
                line[i] = recv[0];
               // printf("%c", line[i]);
                i++;
               }
               else if(i == 12){
                   line[i] = recv[0];
                   //printf("%c",line[i]);
                   i = 0;
               }
           }
            if (change == 1) {
               if(j!=17) {
                    if(j == 1) {
                        if(recv[0] == '-') at[j] = '/';
                        else at[j] = '1';
                     //   printf("%c", at[j]);
                        j++;
                    }
                    else {
                        at[j] = recv[0];
                      //  printf("%c", at[j]);
                        j++;
                    }
                }
                else if(j == 17){
                    at[j] = recv[0];
                   // printf("%c", at[j]);
                    j = 0;
                }
                   
            }
        }
    }
}