bool m_handshake_complete = false;

int step_pin = 5;
int direction_pin = 6;
int enable_pin = 7;

int step_time = 100;

void setup()
{
  Serial.begin(9600);

  pinMode (step_pin, OUTPUT);
  pinMode (direction_pin, OUTPUT);
  pinMode (enable_pin, OUTPUT);
  
  digitalWrite(step_pin, LOW);
  digitalWrite(direction_pin, LOW);
  digitalWrite(enable_pin, HIGH);
}

void loop()
{
  if (!m_handshake_complete)
  {
    handshake();
  }
  else
  {
    StepMotor();
  }
}

void handshake()
{
  String incoming = Serial.readString();
  if (incoming == "STEPPER_TERMINAL\0")
  {
    m_handshake_complete = true;
    Serial.write("STEPPER_CONTROLLER");
    Serial.write('\0');
  }
}

void StepMotor()
{
  digitalWrite(step_pin, HIGH);
  delayMicroseconds(100);

  digitalWrite(step_pin, LOW);
  delayMicroseconds(100);
}
