bool m_handshake_complete = false;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  handshake();
}

void handshake()
{
  if (m_handshake_complete)
    return;
  String incoming = Serial.readString();
  if (incoming == "STEPPER_TERMINAL\0")
  {
    m_handshake_complete = true;
    Serial.write("STEPPER_CONTROLLER");
    Serial.write('\0');
  }
}
