//Majority of this class is the sample code from the Arduino docs, in order to allow enhanced communication between
//the computer and the Arduino.

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.io.FileReader;
import java.io.FileNotFoundException;
import gnu.io.CommPortIdentifier; 
import gnu.io.SerialPort;
import gnu.io.SerialPortEvent; 
import gnu.io.SerialPortEventListener; 
import java.util.Enumeration;
import java.util.Scanner;

public class SerialTest implements SerialPortEventListener {
	SerialPort serialPort;
        /** The port we're normally going to use. */
	private static final String PORT_NAMES[] = { 
			"/dev/tty.usbserial-A9007UX1", // Mac OS X
                        "/dev/ttyACM0", // Raspberry Pi
			"/dev/ttyUSB0", // Linux
			"COM3", // Windows
	};
	/**
	* A BufferedReader which will be fed by a InputStreamReader 
	* converting the bytes into characters 
	* making the displayed results codepage independent
	*/
	private BufferedReader input;
	/** The output stream to the port */
	private OutputStream output;
	/** Milliseconds to block while waiting for port open */
	private static final int TIME_OUT = 2000;
	/** Default bits per second for COM port. */
	private static final int DATA_RATE = 115200;

	private PrintWriter writeTo; //we use this for writing to a file
	private Scanner readFrom; //we use this for reading from a file

	public void initialize() {
                // the next line is for Raspberry Pi and 
                // gets us into the while loop and was suggested here was suggested http://www.raspberrypi.org/phpBB3/viewtopic.php?f=81&t=32186
                //System.setProperty("gnu.io.rxtx.SerialPorts", "/dev/ttyACM0"); //leave this commented out if not using Raspberry Pi

		CommPortIdentifier portId = null;
		Enumeration portEnum = CommPortIdentifier.getPortIdentifiers();

		//First, Find an instance of serial port as set in PORT_NAMES.
		while (portEnum.hasMoreElements()) {
			CommPortIdentifier currPortId = (CommPortIdentifier) portEnum.nextElement();
			for (String portName : PORT_NAMES) {
				if (currPortId.getName().equals(portName)) {
					portId = currPortId;
					break;
				}
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
				String inputLine=input.readLine();
				if(inputLine.charAt(0) == ';')//For messages you actuallly want to output into the console
				{
					String s = inputLine.substring(1);
					System.out.println(s);
					return;
				}
				handleFileStuff(inputLine);
			} catch (IOException e) {
				System.err.println(e.toString());
			}
		}
		// Ignore all the other eventTypes, but you should consider the other ones.
	}

	public void handleFileStuff(String s) //as you can tell by the method name, this is not the greatest implementation, and is fairly messy
	{
		if(s.indexOf("SETWFILE:") == 0) //setting write file, this is the line header that indicates file is being set
		{
			String g = s.substring("SETWFILE:".length());//strip the header
			try
			{
				FileWriter fw = new FileWriter(g + ".txt", true); //make sure to add file extension.
				BufferedWriter bw = new BufferedWriter(fw);
				writeTo = new PrintWriter(bw);
			}
			catch(IOException e)
			{
				System.out.println(e.toString());
			}
		}
		else if(s.indexOf("WFILE:") == 0)//writing to file, this is the line header that indicates stuff is being written
		{
			writeTo.println(s.substring("WFILE:".length()));//strip header from text being printed
			writeTo.flush();//files won't actually save anything otherwise
		}
		else if(s.indexOf("RFILE:") == 0)//header that a file is being read from
		{								//this is relatively convoluted
			try
			{
				readFrom = new Scanner(new BufferedReader(new FileReader(s.substring("RFILE:".length(), s.indexOf(' ')) + ".txt")));
			} //scanner set up above.
			catch(FileNotFoundException f)//this should be self-explanatory
			{
				System.out.println("ERROR: File " + s.substring("RFILE:".length(), s.indexOf(' ')) + ".txt not found.");
				return;
			}
			String target = s.substring(s.indexOf(' ')+1); //this is the "tag" that we are looking for
			//System.out.println("TARGET = " + target);
			String found = ""; //the value stored in that "tag" if we find it
			while(readFrom.hasNextLine()) //go through the file
			{
				String q;
				q = readFrom.nextLine();
				if(q.substring(0, q.indexOf('=')).equals(target)) //if the tag on this line is the tag we want
					found = q.substring(q.indexOf('=') + 1); // we found it, not breaking out so this will get the LAST instance we find
			}
			if(found != "") //if we found something
			{
				try
				{
					output.write(found.getBytes()); // send data to Arduino as an array of bytes, no method to do it with String
					output.flush(); //probably not strictly necessary but the code for file IO is unstable enough it's probably not a good idea to remove this
				}
				catch(IOException e)
				{
					System.out.println(e.toString());
				}
				//System.out.println("Found = " + found);
			}
			else
				System.out.println("DATA FIELD " + target + " NOT FOUND");
		}
		//System.out.println("ORIGINAL STRING = " + s);
	}

	public static void main(String[] args) throws Exception {
		SerialTest main = new SerialTest();
		main.initialize();
		Thread t=new Thread() {
			public void run() {
				//the following line will keep this app alive for 1000 seconds,
				//waiting for events to occur and responding to them (printing incoming messages to console).
				try {
						Thread.sleep(1000000);
					}
				catch (InterruptedException ie) {}
			}
		};
		t.start();
		System.out.println("Started");
	}
}