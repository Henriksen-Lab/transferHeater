package transferHeater;

import jssc.SerialPortException;

public class Runner {
	public static void main(String []args) {
		
		try {

			SerialComm comm = new SerialComm("com3");
			//comm.setDebug(true);
			//Listens for command inputs in separate thread so to not block main thread.
			CommandListener commandListener = new CommandListener(comm);
			Thread t = new Thread(commandListener);
			t.start();
			while(true){
				if(comm.available()) {
					char result = (char)comm.readByte();
					System.out.print(result);
				}
			}
		} catch (SerialPortException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
}
