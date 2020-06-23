package transferHeater;
import java.util.Scanner;

import jssc.*;

public class SerialComm {

	SerialPort port;
	
	private boolean debug;  // Indicator of "debugging mode"
	private int counts = 0;
	public static void main(String []args) {
		Scanner in = new Scanner(System.in);
		try {
			SerialComm comm = new SerialComm("com3");
			//comm.setDebug(true);
			while(true){
				if(comm.available()) {
					char result = (char)comm.readByte();
					System.out.print(result);
				}
				/*
				if(in.hasNext()) {
				String s = in.nextLine();
					for(int i = 0; i<s.length(); i++) {
						byte num = (byte)s.charAt(i);
						comm.writeByte(num);
					}
				}
				*/
			}
		} catch (SerialPortException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
	
	
	
	// This function can be called to enable or disable "debugging mode"
	void setDebug(boolean mode) {
		debug = mode;
	}	
	

	// Constructor for the SerialComm class
	public SerialComm(String name) throws SerialPortException {
		port = new SerialPort(name);		
		port.openPort();
		port.setParams(SerialPort.BAUDRATE_9600,
			SerialPort.DATABITS_8,
			SerialPort.STOPBITS_1,
			SerialPort.PARITY_NONE);
		
		debug = false; // Default is to NOT be in debug mode
	}
	public void writeByte(byte num) {
		if(this.debug) {
			try {
				this.port.writeByte(num);
				System.out.println(String.format("<0x%x>", num));
			} catch (SerialPortException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}else {			
			try {
				this.port.writeByte(num);
			} catch (SerialPortException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	public boolean available() {
		try {
			if(this.port.getInputBufferBytesCount()>0) {
				return true;
			}else {
				return false;
			}
		} catch (SerialPortException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return false;
		}
	}
	public byte readByte() {
		try {
			byte[] result = this.port.readBytes(1);
			//System.out.println(counts+"reads");
			counts++;
			if(this.debug) {
				String debugString = "[0x"+String.format("%02x", result[0])+"]";
				System.out.println(debugString);
			}
			return result[0];
		} catch (SerialPortException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			byte num = 0;
			return num;
		}

	}

}