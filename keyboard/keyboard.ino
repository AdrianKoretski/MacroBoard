void setup()
{
  Serial.begin(9600);
  for (int i = 2; i < 27; i++)
  {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  for (int i = 27; i < 55; i++)
    pinMode(i, INPUT_PULLUP);
}

void loop()
{
  handshake();
  for (int i = 2; i < 27; i++)
  {
    digitalWrite(i, LOW);
    for (int j = 27; j < 55; j++)
      if (digitalRead(j) == LOW)
      {
        Serial.write(i);
        Serial.write(j);
      }
    digitalWrite(i, HIGH);
  }
}

void handshake()
{
  if (Serial.available() != 10)
    return;
  String incoming = Serial.readString();
  if (incoming == "MACROBRD?\0")
    Serial.write("MACROBRD!");
}
