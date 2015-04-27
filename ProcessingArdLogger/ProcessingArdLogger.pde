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
  myPort = new Serial(this, portName, 57600);
  output = createWriter("/home/jordan/Desktop/log.txt");
}
void draw()
{
  if ( myPort.available() > 0) {  // If data is available,
    val = myPort.readString();
    print(val);    // read it and store it in val
    output.println(val);
    output.flush();
  }
  background(255);             // Set background to white
  if (val=="") {              // If the serial value is 0,
    fill(0);                   // set fill to black
  } 
  else {                       // If the serial value is not 0,
    fill(204);                 // set fill to light gray
  }
  rect(50, 50, 100, 100);
  val = "";
}
void stop(){
  output.close(); 
}
