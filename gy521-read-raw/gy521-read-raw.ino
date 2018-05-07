/**
 * Reading raw data from the GY-521 sensor module (MPU-6050)
 *
 * (c) 2018 Christian Grieger
 * GNU GENERAL PUBLIC LICENSE
 */

#include <Wire.h>

#define MPU6050_ADRESS 0x68
int16_t accX, accY, accZ, gyrX, gyrY, gyrZ, tVal;
double temperature = 0.0;

void setup()
{
  Wire.begin();
  Wire.beginTransmission(MPU6050_ADRESS); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  Serial.begin(9600);
  Serial.flush();
}
void loop()
{
  Wire.beginTransmission(MPU6050_ADRESS);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU6050_ADRESS, 7*2, true); // request a total of 7*2=14 registers

  // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
  accX = Wire.read()<<8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
  accY = Wire.read()<<8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
  accZ = Wire.read()<<8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
  tVal = Wire.read()<<8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
  gyrX = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  gyrY = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
  gyrZ = Wire.read()<<8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)

  Serial.print("aX = ");    Serial.print(toStr(accX));
  Serial.print(" | aY = "); Serial.print(toStr(accY));
  Serial.print(" | aZ = "); Serial.print(toStr(accZ));
  Serial.print(" | gX = "); Serial.print(toStr(gyrX));
  Serial.print(" | gY = "); Serial.print(toStr(gyrY));
  Serial.print(" | gZ = "); Serial.print(toStr(gyrZ));

  /**
   * The following parameters are taken from the documentation [MPU-6000/MPU-6050 Product Specification, p.14]:
   *
   * Temperature sensor is -40°C to +85°C (signed integer)
   * 340 per °C
   * Offset = -512 at 35°C
   * At 0°C: -512 - (340 * 35) = -12412
   */
  temperature = (tVal + 12412.0) / 340.0;
  Serial.print(" | T = ");  Serial.print(toStr((int16_t)temperature)); Serial.print("°C");

  Serial.println();
  delay(500);
}

/**
 * Converts int16 to string.
 * Moreover, resulting strings will have the same length
 */
char* toStr(int16_t value)
{
  char result[7];
  sprintf(result, "%6d", value);
  return result;
}
