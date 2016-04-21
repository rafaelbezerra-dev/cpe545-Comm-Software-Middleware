import java.util.concurrent.BlockingQueue;

public class Producer extends Thread{

	private final BlockingQueue<String> workQueue;

	public Producer(BlockingQueue<String> workQueue) {
		super();
		this.workQueue = workQueue;
	}

	@Override
	public void run() {
		super.run();
		int counter = 0;
		while (!Thread.currentThread().isInterrupted()) {
            try {
	           workQueue.put("item " + counter++);
	           Thread.sleep(250);    	        
            } catch (InterruptedException ex) {
                Thread.currentThread().interrupt();
                break;
            }
        }
	}

	
}
