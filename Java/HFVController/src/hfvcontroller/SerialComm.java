package hfvcontroller;

import java.io.IOException;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStream;
import gnu.io.CommPortIdentifier; 
import gnu.io.SerialPort;
import gnu.io.SerialPortEvent; 
import gnu.io.SerialPortEventListener; 
import java.util.Enumeration;
import java.util.LinkedList;


public class SerialComm implements SerialPortEventListener {
  SerialPort serialPort;
  
  /**
  * A BufferedReader which will be fed by a InputStreamReader 
  * converting the bytes into characters 
  * making the displayed results codepage independent
  */
  private BufferedReader input;
  private OutputStream output;
  private static final int TIME_OUT = 2000;
  private static final int DATA_RATE = 9600;
  private static final int BUFFER_LEN = 20;
  private static final String LINE_DELMIN = "\n";
  
  private LinkedList<String> lines = new LinkedList<>();

  public void initialize(String portName) {
    System.setProperty("gnu.io.rxtx.SerialPorts", "/dev/ttyACM0");

    CommPortIdentifier portId = null;
    Enumeration portEnum = CommPortIdentifier.getPortIdentifiers();

    //First, Find an instance of serial port as set in PORT_NAMES.
    while (portEnum.hasMoreElements()) {
        CommPortIdentifier currPortId = (CommPortIdentifier) portEnum.nextElement();
        if (currPortId.getName().equals(portName)) {
            portId = currPortId;
            break;
        }
    }

    if (portId == null) {
      System.out.println("Could not find COM port.");
      return;
    }

    try {
      // open serial port, and use class name for the appName.
      serialPort = (SerialPort) portId.open(this.getClass().getName(),
          TIME_OUT);

      // set port parameters
      serialPort.setSerialPortParams(DATA_RATE,
          SerialPort.DATABITS_8,
          SerialPort.STOPBITS_1,
          SerialPort.PARITY_NONE);

      // open the streams
      input = new BufferedReader(new InputStreamReader(serialPort.getInputStream()));
      output = serialPort.getOutputStream();

      // add event listeners
      serialPort.addEventListener(this);
      serialPort.notifyOnDataAvailable(true);

    } catch (Exception e) {
      System.err.println(e.toString());
    }
  }

  /**
   * This should be called when you stop using the port.
   * This will prevent port locking on platforms like Linux.
   */
  public synchronized void close() {
    if (serialPort != null) {
      serialPort.removeEventListener();
      serialPort.close();
    }
  }

  /**
   * Handle an event on the serial port. Read the data and print it.
   */
  public synchronized void serialEvent(SerialPortEvent oEvent) {
    if (oEvent.getEventType() == SerialPortEvent.DATA_AVAILABLE) {
      try {
        String inputLine = input.readLine();
        
        lines.add(inputLine);
        if (lines.size() > BUFFER_LEN)
            lines.removeFirst();
        
      } catch (Exception e) {
        System.err.println(e.toString());
      }
    }
  }
  
  public String getLastLine() { return (lines.size() > 0) ? lines.getLast() : ""; }

  public void write(String s) {
    try {
      output.write((s + "\n").getBytes());
    } catch (IOException e) {
      System.out.println("Failed to write string: " + s);
    }
  }
}
