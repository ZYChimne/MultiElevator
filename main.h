#ifndef __MAIN_H__
#define __MAIN_H__
#include<reg52.h>
sbit AL3=P1^0;
sbit AL2=P1^1;
sbit AL1=P1^2;
sbit BL3=P1^3;
sbit BL2=P1^4;
sbit BL1=P1^5;
sbit FL2Down=P1^6;
sbit FL1Up=P1^7;
sbit FL3Down=P0^0;
sbit FL2Up=P0^1;
sbit ANum1=P0^2;
sbit ANum0=P0^3;
sbit AUp=P0^4;
sbit ADown=P0^5;
sbit BNum1=P0^6;
sbit BNum0=P0^7;
sbit AAlarm=P2^0;
sbit AButton3=P2^1;
sbit AButton2=P2^2;
sbit AButton1=P2^3;
sbit BButton3=P2^4;
sbit BButton2=P2^5;
sbit BButton1=P2^6;
sbit FButton3Down=P2^7;
sbit FButton2Up=P3^0;
sbit FButton2Down=P3^1;
sbit FButton1Up=P3^2;
sbit BUp=P3^3;
sbit BDown=P3^4;
sbit BAlarm=P3^5;
sbit BOpen=P3^6;
sbit AOpen=P3^7;
char curFA=1;
char curFB=1;
char AFS[3]={0};
char BFS[3]={0};
char time=0;
char last_time=0;
char timer0_cnt=0;
char max_floor=3;
char min_floor=1;
char dif_floor=0;
void init_timer0(void){
	TMOD=0x01;
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	ET0=1;
	EA=1;
	TR0=1;
}
void init(void){
	AL3=0;
	AL2=0;
	AL1=0;
	BL3=0;
	BL2=0;
	BL1=0;
	FL3Down=0;
	FL2Up=0;
	FL2Down=0;
	FL1Up=0;
	AUp=0;
	ADown=0;
	BUp=0;
	BDown=0;
	ANum1=0;
	BNum1=0;
	AOpen=0;
	BOpen=0;
	dif_floor=max_floor-min_floor+1;
	init_timer0();
}
void display_num(char curFA, char curFB){
	switch(curFA){
		case 3:ANum1=1;ANum0=1;break;
		case 2:ANum1=1;ANum0=0;break;
		case 1:ANum1=0;ANum0=1;break;
	}
	switch(curFB){
		case 3:BNum1=1;BNum0=1;break;
		case 2:BNum1=1;BNum0=0;break;
		case 1:BNum1=0;BNum0=1;break;
	}
}
char abs(char a){
	return a>=0?a:-a;
}
char farthest_floor(char a[], char b, char c){
	char i=b-1;
	char res=b;
	if(c==0){
		for(i=b-1;i<max_floor;i++)
			if(a[i]==1)res=i+1;
	}
	else if(c==1){
		for(i=b-1;i>=min_floor-1;i--)
			if(a[i]==1)res=i+1;
	}
	return res;
}
void assign_elevator(char curF, char dir){
	char difA=100;
	char difB=100;
	char ca=(dir==0&&(AUp==1&&curFA>curF))||(dir==0&&(ADown==1&&curFA<curF));
	char cb=(dir==0&&(BUp==1&&curFB>curF))||(dir==0&&(BDown==1&&curFB<curF));
	if(ca==1)difA=abs(farthest_floor(AFS,curFA,dir)*2-curFA-curF);
	else difA=abs(curF-curFA);
	if(cb==1)difB=abs(farthest_floor(BFS,curFB,dir)*2-curFB-curF);
	else difB=abs(curF-curFB);
	if(difA<=difB)AFS[curF-1]=1;
	else BFS[curF-1]=1;
}
void close_led_out(char b, char c){
	switch(b){
		case 1:FL1Up=0;break;
		case 2:if(c==0)FL2Up=0;else if(c==1)FL2Down=0;break;
		case 3:FL3Down=0;break;
	}
}
void close_led(char a, char b, char c){
	if(a==0){
		switch(b){
			case 1:AL1=0;break;
			case 2:AL2=0;break;
			case 3:AL3=0;break;
		}
	}
	else if(a==1){
		switch(b){
			case 1:BL1=0;break;
			case 2:BL2=0;break;
			case 3:BL3=0;break;
		}
	}
	close_led_out(b,c);
}
char find_direction(char a[], char b){
	char l=b-2;
	char r=b;
	if(a[b-1]==1)return 2;
	while(l>=min_floor-1||r<max_floor){
		if(l>=min_floor-1&&a[l]==1){return 0;}
		if(r<max_floor&&a[r]==1){return 1;}
		r++;l--;
	}
	return 3;
}
char not_zero_array(char a[]){
	char i=0;
	for(i=0;i<dif_floor;i++){
		if(a[i]==1)
			return 1;
	}
	return 0;
}
void press(void){
	if(AL3==0&&AButton3==0){
		AL3=1;
		AFS[2]=1;
	}
	if(AL2==0&&AButton2==0){
		AL2=1;
		AFS[1]=1;
	}
	if(AL1==0&&AButton1==0){
		AL1=1;
		AFS[0]=1;
	}
	if(BL3==0&&BButton3==0){
		BL3=1;
		BFS[2]=1;
	}
	if(BL2==0&&BButton2==0){
		BL2=1;
		BFS[1]=1;
	}
	if(BL1==0&&BButton1==0){
		BL1=1;
		BFS[0]=1;
	}
	if(FL3Down==0&&FButton3Down==0){
		FL3Down=1;
		assign_elevator(3,1);
	}
	if(FL2Up==0&&FButton2Up==0){
		FL2Up=1;
		assign_elevator(2,0);
	}
	if(FL2Down==0&&FButton2Down==0){
		FL2Down=1;
		assign_elevator(2,1);
	}
	if(FL1Up==0&&FButton1Up==0){
		FL1Up=1;
		assign_elevator(1,0);
	}
}
void timer0(void) interrupt 1{
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	timer0_cnt++;
	if(timer0_cnt%10==0){
		time++;
	}
}
void run(void){
	if(time-last_time>0){
		if(AOpen==1){
			AOpen=0;
			AAlarm=0;
		}
		else{
			if(not_zero_array(AFS)){
				if(AUp==0&&ADown==0){
					if(AFS[curFA-1]==1){
						AOpen=1;
						AAlarm=1;
						AFS[curFA-1]=0;
						close_led(0,curFA,0);
						close_led(0,curFA,1);
					}
					switch(find_direction(AFS, curFA)){
						case 1:AUp=1;;break;
						case 0:ADown=1;break;
					}
				}
				else if(AUp==1){
					char max_cur=farthest_floor(AFS,curFA,0);
					if(curFA<max_cur){
						curFA++;
						if(AFS[curFA-1]==1){
							AOpen=1;
							AAlarm=1;
							AFS[curFA-1]=0;
							close_led(0,curFA,0);
						}
					}
					if(curFA==max_cur){
						if(not_zero_array(AFS)){
							AUp=0;
							ADown=1;
						}
						else{
							AUp=0;
							ADown=0;
						}
					}
				}
				else if(ADown==1){
					char min_cur=farthest_floor(AFS,curFA,1);
					if(curFA>min_cur){
						curFA--;
						if(AFS[curFA-1]==1){
							AOpen=1;
							AAlarm=1;
							AFS[curFA-1]=0;
							close_led(0,curFA,1);
						}
					}
					if(curFA==min_cur){
						if(not_zero_array(AFS)){
							AUp=1;
							ADown=0;
						}
						else{
							AUp=0;
							ADown=0;
						}
					}
				}
			}
		}
		//*
		if(BOpen==1){
			BOpen=0;
			BAlarm=0;
		}
		else{
			if(not_zero_array(BFS)){
				if(BUp==0&&BDown==0){
					if(BFS[curFB-1]==1){
						BOpen=1;
						BAlarm=1;
						BFS[curFB-1]=0;
						close_led(1,curFB,0);
						close_led(1,curFB,1);
					}
					switch(find_direction(BFS, curFB)){
						case 1:BUp=1;break;
						case 0:BDown=1;break;
					}
				}
				else if(BUp==1){
					char max_cur=farthest_floor(BFS,curFB,0);
					if(curFB<max_cur){
						curFB++;
						if(BFS[curFB-1]==1){
							BOpen=1;
							BAlarm=1;
							BFS[curFB-1]=0;
							close_led(1,curFB,0);
						}
					}
					if(curFB==max_cur){
						if(not_zero_array(BFS)){
							BUp=0;
							BDown=1;
						}
						else{
							BUp=0;
							BDown=0;
						}
					}
				}
				else if(BDown==1){
					char min_cur=farthest_floor(BFS,curFB,1);
					if(curFB>min_cur){
						curFB--;
						if(BFS[curFB-1]==1){
							BOpen=1;
							BAlarm=1;
							BFS[curFB-1]=0;
							close_led(1,curFB,1);
						}
					}
					if(curFB==min_cur){
						if(not_zero_array(BFS)){
							BUp=1;
							BDown=0;
						}
						else{
							BUp=0;
							BDown=0;
						}
					}
				}
			}
		}
		//*/
	}
	display_num(curFA, curFB);
	last_time=time;
}
