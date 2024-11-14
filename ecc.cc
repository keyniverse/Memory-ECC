/*
 * Copyright (c) 2006 The Regents of The University of Michigan
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <iostream>
#include <stdio.h>
#include <random>
#include <cstdlib>
#include <ctime>

using namespace std;

int *Encode(int data[8]){
	static int outputdata[12]={0,0,0,0,0,0,0,0,0,0,0,0};
	outputdata[2]=data[0];
	outputdata[4]=data[1];
	outputdata[5]=data[2];
	outputdata[6]=data[3];
	outputdata[8]=data[4];
	outputdata[9]=data[5];
	outputdata[10]=data[6];
	outputdata[11]=data[7];
		
	//Calculation of even parity   XOR
	outputdata[0]=outputdata[2]^outputdata[4]^outputdata[6]^outputdata[8]^outputdata[10];
	outputdata[1]=outputdata[2]^outputdata[5]^outputdata[6]^outputdata[9]^outputdata[10];
	outputdata[3]=outputdata[4]^outputdata[5]^outputdata[6]^outputdata[11];
 	outputdata[7]=outputdata[8]^outputdata[9]^outputdata[10]^outputdata[11];
	return outputdata;
}

struct fixdata{
	int *data;
	int fix;
};

fixdata ecc(int datacheck[12]){
	int p,p0,p1,p3,p7;
	fixdata fd;
	p0=datacheck[0]^datacheck[2]^datacheck[4]^datacheck[6]^datacheck[8]^datacheck[10];
	p1=datacheck[1]^datacheck[2]^datacheck[5]^datacheck[6]^datacheck[9]^datacheck[10];
	p3=datacheck[3]^datacheck[4]^datacheck[5]^datacheck[6]^datacheck[11];
	p7=datacheck[7]^datacheck[8]^datacheck[9]^datacheck[10]^datacheck[11];
	//printf("p7: %d p3: %d p1: %d p0: %d \n",p7,p3,p1,p0);
	p=p7*8+p3*4+p1*2+p0;
	
	if(p==0){
		fd.data=datacheck;
		fd.fix=0;
		printf("\nno error !\n\n");
	}
	else if(p>12){
		//error  bug
		printf("\n have two error cannot fix ! \n");
		fd.fix=1;
	}
	else{
		printf("\nWrong bit is index %d \n",p-1);
		if(datacheck[p-1]==0){
			datacheck[p-1]=1;
		}else{
			datacheck[p-1]=0;
		}
		fd.data=datacheck;
		fd.fix=0;
	}
	return fd;
}


int main() {
	
    srand(time(NULL));
    int inputdata[8]={};
    int wrong_data[12]={};
    for (int i=0;i<8;i++) {
		cin >> inputdata[i];
	}
	int *encode_data;
	
	encode_data = Encode(inputdata);
	printf("\nData after encode is ");
	for (int i=0;i<12;i++) {
		printf("%d",encode_data[i]);
	}

    for (int i=0;i<12;i++) {
		wrong_data[i] = encode_data[i];
	}
	int tmp = rand()%12; //bit that comes wrong

    if(wrong_data[tmp] == 1) {
        wrong_data[tmp]=0;
    }
    else if(wrong_data[tmp] == 0){
        wrong_data[tmp]=1;
    }
    
    printf("\nWrong data is ");
	for (int i=0;i<12;i++) {
		printf("%d",wrong_data[i]);
	}

	fixdata fd; //宣告自創結構 
	
	fd=ecc(wrong_data);
	printf("\n");
	for (int i=0;i<12;i++) {
		printf("%d",fd.data[i]);
	}
	
	printf("\nNeed to fix : %d\n",fd.fix);
}



