import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class RMIServer extends UnicastRemoteObject implements Hello 
{ 
    public RMIServer() throws RemoteException {}

    public String sayHello() { return "Hello world!"; }

    public static void main(String args[]) 
    { 
    	// run "start rmiregistry" first
	final String bindName = "HelloServer";
	final String hostname = "155.246.137.73";
	System.setProperty("java.rmi.server.hostname", hostname);
	System.out.println("Hostname set to: " + hostname);
	System.out.println("Bind name set to: " + bindName);

        try 
        { 
            RMIServer obj = new RMIServer(); 
            // Bind this object instance to the name  
            Naming.rebind(bindName, obj);
            System.out.println("Object binded to name: " + bindName);
            
        } 
        catch (Exception e) 
        { 
            System.out.println("RMIServer err: " + e.getMessage()); 
            e.printStackTrace(); 
        } 
    } 
}
