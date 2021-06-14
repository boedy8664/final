#include"mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"

void gogo(Arguments *in, Reply *out);
RPCFunction rpcgogo(&gogo, "gogo");
BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(D1,D0); //tx,rx
BufferedSerial xbee(D10,D9);
Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
BBCar car(pin5, pin6, servo_ticker);
DigitalOut led1(LED1);
DigitalInOut ping(D12);
Thread t;
Timer tt;
char at[18], line[13];
int i = 0;
int st = 0;
int step = 0;
int END = 0;

void turn() {
    car.line(30, 30, 0.4, 1);
    ThisThread::sleep_for(1500ms);
    step = 0;
}

void Data() {
    float Tx, Tz, Ry;
    int x1, x2, del;
   // float val;
    pc.set_baud(9600);
    car.stop();
   // printf("start = %d", st);
    
    while(1) {
       // printf("FFF = %d", st);
    if(st == 1) {
      //  printf("SSS = %d", st);
        if(line[0] == 'L') {
            x1 = 100*(int(line[1])-48) + 10*(int(line[2])-48) + int(line[3])-48;
            x2 = 100*(int(line[7])-48) + 10*(int(line[8])-48) + int(line[9])-48;
        }
        if(at[0] == 'A') {
            Tx = (int(at[1])-48)*((int(at[2])-48) + 0.1*(int(at[4])-48) + 0.01*(int(at[5])-48));
            Tz = -1*(10*(int(at[7])-48) + (int(at[8])-48) + 0.1*(int(at[10])-48) + 0.01*(int(at[11])-48));
            Ry = 100*(int(at[12])-48) + 10*(int(at[13])-48) + (int(at[14])-48) + 0.1*(int(at[16]-48)) + 0.01*(int(at[17])-48);
        }

        printf("%d %d %f %f %f", x1, x2, Tx,Tz,Ry);
        del = x2-x1;
        if (abs(Tz) >=  8 && step == 0) {
            if (del<0) {
                del = -del;
                car.line(20,20+del/2,1,1);
               // car.goStraight(100);
                ThisThread::sleep_for(100ms);
            }
            else {
               // del = -del;
                car.line(20+del/4,20,1,1);
               // car.goStraight(-100);
                ThisThread::sleep_for(100ms);
            }
        }
        else {
            if(END == 0) {
                turn();
                END = 1;
            }
            else {
                car.stop();
                ThisThread::sleep_for(1s);
            } 

        }
        if(line[0] == 'N') car.stop();
    
    }
    }
}

int main(){
    uart.set_baud(9600);
    t.start(Data);
  //  parallax_ping  ping1(pin10);
    while(1){
        char buf[256], outbuf[256];
        FILE *devin = fdopen(&xbee, "r");
        FILE *devout = fdopen(&xbee, "w");

        memset(buf, 0, 256);
        for( int i = 0; ; i++ ) {
            char Recv = fgetc(devin);
            if(Recv == '\n') {
                printf("\r\n");
                break;
            }
            buf[i] = fputc(Recv, devout);
        }
        RPC::call(buf, outbuf);

        if(uart.readable()){
            char recv[1];
            uart.read(recv, sizeof(recv));
            pc.write(recv, sizeof(recv));
           if (recv[0] == 'L') {
               if (i != 12) {
                line[i] = recv[0];
                i++;
               }
               else {
                   line[i] = recv[0];
                   i = 0;
               }
           }
           if (recv[0] == 'A') {
               if(i!=17) {
                    if(i == 1) {
                        if(recv[0] == '-') at[i] = '/';
                        else at[i] = '1';
                        //   printf("%c", data[i]);
                    i++;
                    }
                    else {
                        at[i] = recv[0];
                      //  printf("%c", data[i]);
                        i++;
                    }
               }
            }
            else {
                at[i] = recv[0];
                //    printf("%c", data[i]);
                i = 0;
                   
            }
        }
    }
}

void gogo(Arguments *in, Reply *out) {
    st = 1;
    car.goStraight(30);
    ThisThread::sleep_for(1s);
    car.stop();
    ThisThread::sleep_for(1s);
    printf("hellostart = %d", st);
}
