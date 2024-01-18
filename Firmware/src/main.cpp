#include <SimpleFOC.h>
#include <Wire.h>

#define PIN_UH 1
#define PIN_UL 4
#define PIN_VH 2
#define PIN_VL 5
#define PIN_WH 3
#define PIN_WL 6

#define PP 6
// BLDC motor & driver instance
static BLDCMotor motor = BLDCMotor(PP, 6);
BLDCDriver6PWM driver = BLDCDriver6PWM(PIN_UH, PIN_UL, PIN_VH, PIN_VL, PIN_WH, PIN_WL);

// hall sensors
#define HALL_A 7
#define HALL_B 8
#define HALL_C 9
HallSensor sensor = HallSensor(HALL_A, HALL_B, HALL_C, PP);
void doA(){sensor.handleA();}
void doB(){sensor.handleB();}
void doC(){sensor.handleC();}

// instantiate the commander
Commander command = Commander(Serial);
void doTarget(char* cmd) { command.scalar(&motor.target, cmd); }
void PID (char* cmd) { command.pid(&motor.PID_velocity, cmd); }
// GenericSensor sensor = GenericSensor(get_mag_sensor_angle, init_magnetic_sensor);

void setup() { 
  Wire.begin(43, 44);

  Serial.begin(115200);
  delay(2000);
  Serial.println("init");

  sensor.init();

  sensor.init();
  sensor.enableInterrupts(doA, doB, doC);


  // link the motor to the sensor
  motor.linkSensor(&sensor);
  // motor.LPF_velocity.Tf = 0.98;
  motor.LPF_velocity.Tf = 0.5;


  // driver config
  // power supply voltage [V]
  driver.voltage_power_supply = 11;
  Serial.println("driver init");
  driver.init();
  // link driver
  motor.linkDriver(&driver);

  // aligning voltage
  motor.voltage_sensor_align = 4;

  // set a max limit for the motor
  motor.voltage_limit = 7; // [V]
  motor.current_limit = 7;

  motor.sensor_direction = CCW;

  // set motion control loop to be used
  // motor.torque_controller = TorqueControlType::voltage;
  // motor.controller = MotionControlType::torque;
  motor.controller = MotionControlType::velocity;

  motor.PID_velocity.P = 0.45;
  motor.PID_velocity.I = 0.01;
  motor.PID_velocity.D = 0.3;

  // motor.controller = MotionControlType::velocity_openloop;

  motor.useMonitoring(Serial);

  // initialize motor
  motor.init();
  // align sensor and start FOC
  motor.initFOC();

  // set the initial motor target
  motor.target = 0; // Volts 

  // add target command T
  // command.add('T', doTarget, "target current"); // - if phase resistance defined
  command.add('T', doTarget, "target voltage");
  command.add('C', PID, "PID velocity");

  Serial.println(F("Motor ready."));
  Serial.println(F("Set the target using serial terminal:"));
  _delay(1000);

}

static int counter = 0;
void loop() {
  // main FOC algorithm function
  motor.loopFOC();

  // Motion control function
  motor.move();

  // Serial.println(encoder.getAngle());

  // // user communication
  command.run();
}
