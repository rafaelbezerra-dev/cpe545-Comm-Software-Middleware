import java.rmi.RemoteException;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.ConcurrentHashMap;

public interface IRMIConnector extends java.rmi.Remote {
	BlockingQueue<String> getWorkQueue() throws RemoteException;

	ConcurrentHashMap<String, String> getDictionary() throws RemoteException;

	void interruptWorkers() throws RemoteException;

	boolean shouldInterruptWorkers() throws RemoteException;
	
	void addWord(String word) throws RemoteException;
	void enqueueWord(String word) throws RemoteException;
	String getNextWord() throws RemoteException;
	boolean containsWord(String word) throws RemoteException;
}