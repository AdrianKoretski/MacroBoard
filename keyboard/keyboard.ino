bool m_handshake_complete = false;
bool m_skip_handshake = false;

int step_pin = 5;
int direction_pin = 6;
int enable_pin = 7;

int stepCount = 10;
int stepDir = 1;

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
  if (!m_handshake_complete && !m_skip_handshake)
  {
    handshake();
  }
  else
  {
    ReadInput();
    MoveMotor();
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

void ReadInput()
{
  int incomingByte = 0;
  int index = 0;
  while (incomingByte != '\n')
  {
    if (Serial.available() > 0)
    {
      incomingByte = Serial.read();
      if (index == 0)
      {
        stepCount = incomingByte;
      }
      if (index == 1)
      {
        stepCount += 256 * incomingByte;
      }
      if (index == 2)
      {
        stepDir = incomingByte;
      }
      index++;
    }
  }
}

void MoveMotor()
{
  digitalWrite(direction_pin, stepDir);
  for (int i = 0; i < stepCount; i++)
  {
    StepMotor();
  }
}

void StepMotor()
{
  digitalWrite(step_pin, HIGH);
  delayMicroseconds(250*10);

  digitalWrite(step_pin, LOW);
  delayMicroseconds(250*10);
}
