import processing.serial.*;

Serial myPort;  // Create object from Serial class
String val;      // Data received from the serial port
PrintWriter output;
void setup() 
{
  size(200, 200);
  // I know that the first port in the serial list on my mac
  // is always my  FTDI adaptor, so I open Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  print(Serial.list());
  String [] ports = Serial.list();
  String portName = ports[ports.length-1];
  myPort = new Serial(this, portName, 9600);
  output = createWriter("/home/jordan/Desktop/plantlog.txt");
}
void draw()
{
  if ( myPort.available() > 0) {  // If data is available,
    val = myPort.readString();
    val = val.trim().replace("\"", "");
    println(val);    // read it and store it in val
    output.println(val);
    output.flush();
    background(255);
    try {
      int hum = Integer.parseInt(val.replace("\"", ""));
      fill(20, 255*hum/100, 20);
    } 
    catch (NumberFormatException e) {
      print(e);
      fill(204);
    }
    rect(50, 50, 100, 100);
    val = "";
  }
}
void stop() {
  output.close();
}

