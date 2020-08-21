package transferHeater;

import jssc.*;

public class SerialComm {

	SerialPort port;
	
	private boolean debug;  // Indicator of "debugging mode"	
	
	// This function can be called to enable or disable "debugging mode"
	void setDebug(final boolean mode) {
		debug = mode;
	}	
	

	// Constructor for the SerialComm class
	public SerialComm(final String name) throws SerialPortException {
		port = new SerialPort(name);		
		port.openPort();
		port.setParams(SerialPort.BAUDRATE_9600,
			SerialPort.DATABITS_8,
			SerialPort.STOPBITS_1,
			SerialPort.PARITY_NONE);
		
		debug = false; // Default is to NOT be in debug mode
	}
	public void writeByte(final byte num) {
		if(this.debug) {
			try {
				this.port.writeByte(num);
				System.out.println(String.format("<0x%x>", num));
			} catch (final SerialPortException e) {

				e.printStackTrace();
			}
		}else {			
			try {
				this.port.writeByte(num);
			} catch (final SerialPortException e) {

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
		} catch (final SerialPortException e) {
			e.printStackTrace();
			return false;
		}
	}
	public byte readByte() {
		try {
			final byte[] result = this.port.readBytes(1);
			if(this.debug) {
				final String debugString = "[0x"+String.format("%02x", result[0])+"]";
				System.out.println(debugString);
			}
			return result[0];
		} catch (final SerialPortException e) {
			e.printStackTrace();
			final byte num = 0;
			return num;
		}

	}

}