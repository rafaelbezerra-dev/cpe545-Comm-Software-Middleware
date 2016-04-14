import java.rmi.RemoteException;
import java.util.List;

public interface INameSearch extends java.rmi.Remote {
	List<String> search(String prefix) throws RemoteException;
}