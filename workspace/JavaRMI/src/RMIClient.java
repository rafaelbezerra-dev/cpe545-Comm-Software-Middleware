import java.awt.List;
import java.net.Inet4Address;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.rmi.Naming;
import java.rmi.RMISecurityManager;
import java.util.ArrayList;

public class RMIClient {

	final static String CALL_EXAMPLE = "\nUsage:\n" + "    java RMIClient [policy file] [host:port] [prefix]\n" + "Example:\n"
			+ "    java RMIClient ..\\client.policy 127.0.0.1 \n";

	public static void main(String args[]) {
//		System.out.println(CALL_EXAMPLE);
		if (!validateArgs(args))
			return;
		final String securityPolicy = args[0];
		final String host = args[1];
		final String prefix = args[2];

		System.out.println("Setting policy");
		System.setProperty("java.security.policy", securityPolicy);

		// I download server's stubs ==> must set a SecurityManager
		System.setSecurityManager(new RMISecurityManager());

		try {
			INameSearch searcher = (INameSearch) Naming.lookup("//" + host + "/NameSearchServer");
			ArrayList<String> result = (ArrayList<String>) searcher.search(prefix);

			if (result != null && !result.isEmpty()) {
				System.out.println("Search Result for prefix " + prefix + ":");
				for (String name : result) {
					System.out.println("    " + name);
				}
			} else {
				System.out.println("No name was found that starts with the prefix " + prefix);
			}
		} catch (Exception e) {
			System.out.println("RMIClient exception: " + e.getMessage());
			e.printStackTrace();
		}

	}

	private static boolean validateArgs(String[] args) {
		int expectedNumberOfArgs = 2;
		if (args != null && args.length >= expectedNumberOfArgs) {

			if (!new java.io.File(args[0]).exists()) {
				System.out.println("Invalid argument. File " + args[0] + " could not be found.");
			} else {
				try {
					Inet4Address.getByName(args[1]);
					return true;
				} catch (UnknownHostException e) {
					// e.printStackTrace();
					System.out.println("Invalid argument. You must provide a valid IP Address.");
				}
			}
		} else {
			System.out.println("Incorrect call. " + "Expected " + expectedNumberOfArgs + " arguments.");
		}
		System.out.println(CALL_EXAMPLE);
		return false;
	}

}
