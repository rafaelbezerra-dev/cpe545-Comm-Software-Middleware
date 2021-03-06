import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.MalformedURLException;
import java.net.UnknownHostException;
import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.ArrayList;
import java.util.List;

public class RMIServer{

	final static String CALL_EXAMPLE = "\nUsage:\n" 
			+ "    java RMIServer [ip address]\n" 
			+ "Example	:\n"
			+ "    java RMIServer 127.0.0.1\n";
	
	static ArrayList<String> studentNameList;
	
	static class RemoteNameSearch extends UnicastRemoteObject implements INameSearch {

		protected RemoteNameSearch(String bindName) throws RemoteException, MalformedURLException {
			super();
			// Bind this object instance to the name
			Naming.rebind(bindName, this);
		}

		@Override
		public List<String> search(String prefix) throws RemoteException {
			prefix = prefix.toLowerCase();
			List<String> list = new ArrayList<>();
			for (String name : studentNameList) {
				if (name.toLowerCase().startsWith(prefix)){
					list.add(name);
				}
			}
			return list;
		}
		
	}
	

	public static void main(String args[]) throws UnknownHostException {
		// http://random-name-generator.info/
		// windows: run >> start rmiregistry
		// ubuntu: run >> rmiregistry &
//		System.out.println(CALL_EXAMPLE);
		if (!validateArgs(args)) {
			return;
		}
		
		try {
			final String bindName = "NameSearchServer";
			final String hostname = args[0];
			
			studentNameList = getList();// InetAddress.getLocalHost().getHostAddress();
			System.setProperty("java.rmi.server.hostname", hostname);
			System.out.println("Hostname set to: " + hostname);
			System.out.println("Bind name set to: " + bindName);
			
			RemoteNameSearch nameSearch = new RemoteNameSearch(bindName);
			System.out.println("Object binded to name: " + bindName);
			System.out.println("Ready for connections...");

		} catch (Exception e) {
			System.out.println("RMIServer err: " + e.getMessage());
			e.printStackTrace();
		}
	}

	private static ArrayList<String> getList() {
		ArrayList<String> list = new ArrayList<>();
		list.add("Susan Hart");
		list.add("Kent Ward");
		list.add("Delbert Maldonado");
		list.add("Delia Phillips");
		list.add("Herbert Blair");
		list.add("Ada Cunningham");
		list.add("Alfredo Mccormick");
		list.add("Wendell Weaver");
		list.add("Marianne Matthews");
		list.add("Saul Frazier");
		list.add("Betty Morgan");
		list.add("Alexandra Harrington");
		list.add("Darla Gardner");
		list.add("Hannah Mathis");
		list.add("Marian Hansen");
		list.add("Tracy Barrett");
		list.add("Sharon Carson");
		list.add("Karen Douglas");
		list.add("Shirley Bennett");
		list.add("Kristopher Brown");
		list.add("Courtney Allison");
		list.add("Helen Bradley");
		list.add("Sonia Hines");
		list.add("Ann Stephens");
		list.add("Vera Love");
		list.add("Gary Mclaughlin");
		list.add("Alberto Abbott");
		list.add("Jeffrey Hodges");
		list.add("Clayton Porter");
		list.add("Mildred Bass");
		list.add("Ruby Phelps");
		list.add("Santos Scott");
		list.add("Enrique Franklin");
		list.add("Madeline Arnold");
		list.add("Martha Young");
		list.add("Anita Stanley");
		list.add("Louise Collins");
		list.add("Antonio Richards");
		list.add("Sonja Harmon");
		list.add("Alfred Lewis");
		list.add("Doyle Frazier");
		list.add("Anthony Wells");
		return list;
	}

	private static boolean validateArgs(String[] args) {
		int expectedNumberOfArgs = 1;
		if (args != null && args.length >= expectedNumberOfArgs){
			try {
			  Inet4Address.getByName(args[0]);
			  return true;
			} catch (UnknownHostException e) {
//				e.printStackTrace();
				System.out.println("Invalid argument. You must provide a valid IP Address.");
			}			
		}
		else{
			System.out.println("Incorrect call. "
					+ "Expected " + expectedNumberOfArgs + " arguments.");
		}
		System.out.println(CALL_EXAMPLE);
		return false;
	}
}
