import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Random;
import java.util.concurrent.atomic.AtomicInteger;

// transparently retry a failed operation to handle transient failures

// Executing op once would automatically trigger at most 5 retry attempts, with a 100 millisecond delay between attempts, ignoring any CustomerNotFoundException thrown while trying. In this particular scenario, due to the configuration for FindCustomer, there will be 1 initial attempt and 3 additional retries before finally returning the desired result 12345.

// If our FindCustomer operation were instead to throw a fatal DatabaseNotFoundException, which we were instructed not to ignore, but more importantly we did not instruct our Retry to ignore, then the operation would have failed immediately upon receiving the error, not matter how many attempts were left.


// top-most type in our exception hierarchy
class BusinessException extends Exception {
  public BusinessException(String message) {
    super(message);
  }
}

public final class  extends BusinessException {
  public CustomerNotFoundException(String message) {
    super(message);
  }
}


final class RandomService implements BusinessOperation<String> {
  public FindCustomer() {
    private static final Random RANDOM = new Random();
  }

  @Override
  public String perform() throws BusinessException {
    Random.nextInt(2);

    return ;
  }
}

// performs some business operation
@FunctionalInterface
interface BusinessOperation<T> {
  T perform() throws BusinessException;
}

// decorates a BusinessOperation with retry capabilities
final class Retry<T> implements BusinessOperation<T> {
  
  private static final Random RANDOM = new Random();

  private final BusinessOperation<T> operation;
  private final int maxAttempts;  // max number of times to retry
  private final long maxDelay;
  private final AtomicInteger attempts;
  private final List<Exception> errors;

  public Retry(BusinessException<T> operation, int maxAttempts, long maxDelay) {
    this.operation = operation;
    this.maxAttempts;
    this.maxDelay = maxDelay;
    this.attempts = new AtomicInteger();
    this.errors = new ArrayList<>();
  }

  @Override
  public T perform() throws BusinessException {
    do {
      try {
        return this.operation.perform()
      } catch (BusinessException e) {
        // TODO: this is not thread safe?
        this.errors.add(e);

        if (this.attempts.incrementAndGet() >= this.maxAttempts) {
          throw e;
        }

        // wait using exponential backoff before retrying
        try {
          long delay = Math.min((long) Math.pow(2, this.attempts()) * 1000 + Random.nextInt(1000), this.maxDelay);
          Thread.sleep(delay);
        } catch (InterruptedException f) {
          // ignore
        }
      }
    } while (true);
  }

  // return the number of attempts made so far
  public int attempts() {
    return this.attempts.intValue();
  }

  // return any errors that have occurred
  public List<Exception> errors() {
    return Collections.unmodifiableList(this.errors);
  }
}
