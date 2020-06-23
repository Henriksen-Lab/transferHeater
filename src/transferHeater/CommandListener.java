package transferHeater;
import java.util.Scanner;

public class CommandListener implements Runnable {

	private SerialComm comm;
	public CommandListener(SerialComm comm) {
		this.comm=comm;
	}

	@Override
	public void run() {
		//This runs when the thread is spawned. Needs to be multithreaded because .hasNext() is blocking.
		@SuppressWarnings("resource")
		Scanner in = new Scanner(System.in);
		while(in.hasNext()) {
			String s = in.nextLine();
			for(int i = 0; i<s.length(); i++) {
				byte num = (byte)s.charAt(i);
				comm.writeByte(num);
			}
		}
	}
}
