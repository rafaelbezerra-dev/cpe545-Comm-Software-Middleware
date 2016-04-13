import java.rmi.RemoteException;

public interface Hello extends java.rmi.Remote {
	String sayHello() throws RemoteException;
}
