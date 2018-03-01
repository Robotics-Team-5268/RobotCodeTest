#include <Subsystems/ITG3200Gyro.h>

//Initialize the i2c communication and set the gyro to full scale range and 100 Hz update rate
ITG3200_Gyro::ITG3200_Gyro():
	i2c(frc::I2C::kOnboard, 0x68) { // Was D0
	//twiInit(80000);			//Init. SCL speed to 50 kHz

	// The following writes may or may not be needed

	//Set internal clock to 1kHz with 42Hz LPF and Full Scale to 3 for proper operation
	write(DLPF_FS, DLPF_FS_SEL_0|DLPF_FS_SEL_1|DLPF_CFG_0);

	//Set sample rate divider for 100 Hz operation
	write(SMPLRT_DIV, 9);	//Fsample = Fint / (divider + 1) where Fint is 1kHz

	//Setup the interrupt to trigger when new data is ready.
	write(INT_CFG, INT_CFG_RAW_RDY_EN | INT_CFG_ITG_RDY_EN);

	//Select X gyro PLL for clock source
	write(PWR_MGM, PWR_MGM_CLK_SEL_0);
}

//Read a register value from the gyro
//pre: register_addr is the register address to read
//	   value is a pointer to an integer
//post: value contains the value of the register that was read
//returns: 1-Success
//		   TWSR-Failure (Check out twi.h for TWSR error codes)
//usage: status = gyro.read(DEVID, &value); //value is created as an 'int' in main.cpp
char ITG3200_Gyro::read(char register_addr, char * value) {
	return i2c.Read(register_addr, 1, (unsigned char *) value);
}

//Write a value to a register
//pre: register_addre is the register to write to
//	   value is the value to place in the register
//returns: 1-Success
//		   TWSR- Failure
//usage status=gyro.write(register_addr, value);
char ITG3200_Gyro::write(char register_addr, char value) {
	return i2c.Write(register_addr, value);
}

//Reads the x,y and z registers and stores the contents into x,y and z variables
//returns 1
//usage: gyro.update();
//Note: update must be called before using the getX, getY or getZ functions in order
//      to obtain the most recent values from the gyro
char ITG3200_Gyro::update() {
	char temp = 0;
	char value;

	SmartDashboard::PutBoolean("gyro debug", read(GYRO_XOUT_H, &value));
	temp=value;
	read(GYRO_XOUT_L, &value);
	x = (temp<<8)|value;

	read(GYRO_YOUT_H, &value);
	temp=value;
	read(GYRO_YOUT_L, &value);
	y = (temp<<8)|value;

	read(GYRO_ZOUT_H, &value);
	temp=value;
	read(GYRO_ZOUT_L, &value);
	z = (temp<<8)|value;

	return 1;
}

void ITG3200_Gyro::Calibrate() {
	// TODO
}

void ITG3200_Gyro::Reset() {
	// TODO
}

double ITG3200_Gyro::GetAngle() const {
	return (double)z/14.375; // Wrong - gets rate
}

double ITG3200_Gyro::GetRate() const {
	return (double)z/14.375; // Degrees per second
}

ITG3200_Gyro::~ITG3200_Gyro() {}
