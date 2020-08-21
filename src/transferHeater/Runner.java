package transferHeater;

import jssc.SerialPortException;

public class Runner {
	public static void main(String []args) {
		
		try {
			//Create SerialComm with the port listed in the Arduino IDE.
			SerialComm comm = new SerialComm("COM4");
			//comm.setDebug(true);
			//Listens for command inputs in separate thread so to not block main thread.
			CommandListener commandListener = new CommandListener(comm);
			Thread t = new Thread(commandListener);
			t.start();
			
			//Continuously reads status messages from Arduino.
			while(true){
				if(comm.available()) {
					char result = (char)comm.readByte();
					System.out.print(result);
				}
			}
		} catch (SerialPortException e) {
			e.printStackTrace();
		}
		
	}
}
