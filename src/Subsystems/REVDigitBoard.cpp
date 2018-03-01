#include <I2C.h>
#include <Timer.h>
#include <DigitalInput.h>
#include <AnalogInput.h>
#include "Subsystems/RevDigitBoard.h"
#include <string.h>
#include <cctype>

REVDigitBoard::REVDigitBoard() : Subsystem("REVDigitBoard")
		, i2c( frc::I2C::kMXP, 0x70 )
		, buttonA( 19 )
		, buttonB( 20 )
		, pot( 3 )
	{
		byte osc;
	 	byte blink;
	 	byte bright;
	 	osc = (byte)0x21;
	 	blink = (byte)0x81;
	 	bright = (byte)0xEF;

		i2c.WriteBulk(&osc, 1);
		Wait(.01);
		i2c.WriteBulk(&bright, 1);
		Wait(.01);
		i2c.WriteBulk(&blink, 1);
		Wait(.01);

		charreg[0][0] = (byte)0b00111111; charreg[9][1] = (byte)0b00000000; //0
		charmap['0'] = 0;
		charreg[1][0] = (byte)0b00000110; charreg[0][1] = (byte)0b00000000; //1
		charmap['1'] = 1;
	 	charreg[2][0] = (byte)0b11011011; charreg[1][1] = (byte)0b00000000; //2
		charmap['2'] = 2;
	 	charreg[3][0] = (byte)0b11001111; charreg[2][1] = (byte)0b00000000; //3
		charmap['3'] = 3;
	 	charreg[4][0] = (byte)0b11100110; charreg[3][1] = (byte)0b00000000; //4
		charmap['4'] = 4;
	 	charreg[5][0] = (byte)0b11101101; charreg[4][1] = (byte)0b00000000; //5
		charmap['5'] = 5;
	 	charreg[6][0] = (byte)0b11111101; charreg[5][1] = (byte)0b00000000; //6
		charmap['6'] = 6;
	 	charreg[7][0] = (byte)0b00000111; charreg[6][1] = (byte)0b00000000; //7
		charmap['7'] = 7;
	 	charreg[8][0] = (byte)0b11111111; charreg[7][1] = (byte)0b00000000; //8
		charmap['8'] = 8;
	 	charreg[9][0] = (byte)0b11101111; charreg[8][1] = (byte)0b00000000; //9
		charmap['9'] = 9;

	 	charreg[10][0] = (byte)0b11110111; charreg[10][1] = (byte)0b00000000; //A
		charmap['A'] = 10;
	 	charreg[11][0] = (byte)0b10001111; charreg[11][1] = (byte)0b00010010; //B
		charmap['B'] = 11;
	 	charreg[12][0] = (byte)0b00111001; charreg[12][1] = (byte)0b00000000; //C
		charmap['C'] = 12;
	 	charreg[13][0] = (byte)0b00001111; charreg[13][1] = (byte)0b00010010; //D
		charmap['D'] = 13;
	 	charreg[14][0] = (byte)0b11111001; charreg[14][1] = (byte)0b00000000; //E
		charmap['E'] = 14;
	 	charreg[15][0] = (byte)0b11110001; charreg[15][1] = (byte)0b00000000; //F
		charmap['F'] = 15;
	 	charreg[16][0] = (byte)0b10111101; charreg[16][1] = (byte)0b00000000; //G
		charmap['G'] = 16;
	 	charreg[17][0] = (byte)0b11110110; charreg[17][1] = (byte)0b00000000; //H
		charmap['H'] = 17;
	 	charreg[18][0] = (byte)0b00001001; charreg[18][1] = (byte)0b00010010; //I
		charmap['I'] = 18;
	 	charreg[19][0] = (byte)0b00011110; charreg[19][1] = (byte)0b00000000; //J
		charmap['J'] = 19;
	 	charreg[20][0] = (byte)0b01110000; charreg[20][1] = (byte)0b00001100; //K
		charmap['K'] = 20;
	 	charreg[21][0] = (byte)0b00111000; charreg[21][1] = (byte)0b00000000; //L
		charmap['L'] = 21;
	 	charreg[22][0] = (byte)0b00110110; charreg[22][1] = (byte)0b00000101; //M
		charmap['M'] = 22;
	 	charreg[23][0] = (byte)0b00110110; charreg[23][1] = (byte)0b00001001; //N
		charmap['N'] = 23;
	 	charreg[24][0] = (byte)0b00111111; charreg[24][1] = (byte)0b00000000; //O
	 	charmap['O'] = 24;
	 	charreg[25][0] = (byte)0b11110011; charreg[25][1] = (byte)0b00000000; //P
	 	charmap['P'] = 25;
	 	charreg[26][0] = (byte)0b00111111; charreg[26][1] = (byte)0b00001000; //Q
	 	charmap['Q'] = 26;
	 	charreg[27][0] = (byte)0b11110011; charreg[27][1] = (byte)0b00001000; //R
	 	charmap['R'] = 27;
	 	charreg[28][0] = (byte)0b10001101; charreg[28][1] = (byte)0b00000001; //S
	 	charmap['S'] = 28;
	 	charreg[29][0] = (byte)0b00000001; charreg[29][1] = (byte)0b00010010; //T
	 	charmap['T'] = 29;
	 	charreg[30][0] = (byte)0b00111110; charreg[30][1] = (byte)0b00000000; //U
	 	charmap['U'] = 30;
	 	charreg[31][0] = (byte)0b00110000; charreg[31][1] = (byte)0b00100100; //V
	 	charmap['V'] = 31;
	 	charreg[32][0] = (byte)0b00110110; charreg[32][1] = (byte)0b00101000; //W
	 	charmap['W'] = 32;
	 	charreg[33][0] = (byte)0b00000000; charreg[33][1] = (byte)0b00101101; //X
	 	charmap['X'] = 33;
	 	charreg[34][0] = (byte)0b00000000; charreg[34][1] = (byte)0b00010101; //Y
	 	charmap['Y'] = 34;
	 	charreg[35][0] = (byte)0b00001001; charreg[35][1] = (byte)0b00100100; //Z
	 	charmap['Z'] = 35;
		charreg[36][0] = (byte)0b00000000; charreg[36][1] = (byte)0b00000000; //space
		charmap[' '] = 36;
	}

	void REVDigitBoard::InitDefaultCommand() {}

	void REVDigitBoard::display(std::string str) { // only displays first 4 chars
		byte charz[4] = {36,36,36,36};

		for(size_t i = 0; i < 4 && i < str.length(); i++) {
			charz[i] = charmap[std::toupper(str[i])];
		}

		_display(charz);
	}

	void REVDigitBoard::display(double batt) { // optimized for battery voltage, needs a double like 12.34
		byte charz[4] = {36,36,36,36};
		// idk how to decimal point
		int ten = (int)(batt/10);
		int one = ((int)batt)%10;
		int tenth = ((int)(batt*10)%10);
		int hundredth = ((int)(batt*100)%10);

		if (ten != 0) charz[0] = ten;
		charz[1] = one;
		charz[2] = tenth;
		charz[3] = hundredth;

		_display(charz);
	}

	 void REVDigitBoard::clear() {
		 byte charz[4] = {36,36,36,36};
		 _display(charz);
	 }

	 bool REVDigitBoard::getButtonA() {
		 return !buttonA.Get();// Not symbol because digital input is true when open
	 }
	 bool REVDigitBoard::getButtonB() {
		 return !buttonB.Get();// Not symbol because digital input is true when open
	 }
	 double REVDigitBoard::getPot() {
		 return pot.GetVoltage();
	 }

////// not supposed to be publicly used..

	void REVDigitBoard::_display( byte charz[4]) {
		byte byte1[10];
		byte1[0] = (byte)(0b00001111);
		byte1[1] = 0; //(byte)(0b00001111);
 		byte1[2] = charreg[charz[3]][0];
 		byte1[3] = charreg[charz[3]][1];
 		byte1[4] = charreg[charz[2]][0];
 		byte1[5] = charreg[charz[2]][1];
 		byte1[6] = charreg[charz[1]][0];
 		byte1[7] = charreg[charz[1]][1];
 		byte1[8] = charreg[charz[0]][0];
 		byte1[9] = charreg[charz[0]][1];
 		//send the array to the board
 		i2c.WriteBulk(byte1, sizeof( byte1 ));
 		Wait(0.01);
	}


