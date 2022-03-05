import java.util.*;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.function.Predicate;

public class BusinessException extends Exception {

  public BusinessException(String message) {
    super(message);
  }
}

public final class CustomerNotFoundException extends BusinessException {

  public CustomerNotFoundException(String message) {
    super(message);
  }
}

public final class DatabaseNotAvailableException extends BusinessException {

  public DatabaseNotAvailableException(String message) {
    super(message);
  }
}


@FunctionalInterface
public interface BusinessOperation<T> {
  
  T perform() throws BusinessException;
}

class Retry<T> implements BusinessOperation<T> {
  
  private BusinessOperation<T> operation;
  private int maxAttempts;  // number of times to retry
  private long delay;       // number of milliseconds between attempts
  private AtomicInteger attempts;
  private List<Exception> errors;

  public Retry(BusinessOperation<T> operation, int maxAttempts, long delay) {
    this.operation = operation;
    this.maxAttempts = maxAttempts;
    this.delay = delay;
    this.attempts = new AtomicInteger();
    this.errors = new ArrayList<>();
  }

  public T perform() throws BusinessException {
    do {
      try {
        return this.operation.perform();
      } catch (BusinessException e) {
        this.errors.add(e);

        if (this.attempts.incrementAndGet() >= this.maxAttempts) {
          throw e;
        }

        try {
          Thread.sleep(this.delay);
        } catch (InterruptedException f) {
          // ignore
        }
      }
    } while (true);
  }

  public List<Exception> errors() {
    return Collections.unmodifiableList(this.errors);
  }

  public int attempts() {
    return this.attempts.intValue();
  }
}

class RetryWithBackoff<T> implements BusinessOperation<T> {

  private static final Random RANDOM = new Random();

  private BusinessOperation<T> operation;
  private int maxAttempts;
  private long maxDelay;
  private AtomicInteger attempts;
  private List<Exception> errors;

  public RetryWithBackoff(BusinessOperation<T> operation, int maxAttempts, long maxDelay) {
    this.operation = operation;
    this.maxAttempts = maxAttempts;
    this.maxDelay = maxDelay;
    this.attempts = new AtomicInteger();
    this.errors = new ArrayList<>();
  }

  public T perform() throws BusinessException {
    do {
      try {
        return this.operation.perform();
      } catch (BusinessException e) {
        this.errors.add(e);

        if (this.attempts.incrementAndGet() >= this.maxAttempts) {
          throw e;
        }

        try {

        } catch (InterruptedException f) {
          Thread.sleep(Math.min((long) Math.pow(2, this.attempts()) * 1000 + RANDOM.nextInt(1000), this.maxDelay));
        }
      }
    } while (true);
  }

  public List<Exception> errors() {
    return Collections.unmodifiableList(this.errors);
  }

  public int attempts() {
    return this.attempts.intValue();
  }
}

class FindCustomer implements BusinessOperation<String> {

  private String customerId;
  private Deque<BusinessException> errors;

  public FindCustomer(String customerId, BusinessException... errors) {
    this.customerId = customerId;
    this.errors = new ArrayDeque<>(List.of(errors));
  }

  public String perform() throws BusinessException {
    if (!this.errors.isEmpty()) {
      throw this.errors.pop();
    }

    return this.customerId;
  }
}

public class Example {

  public static void main(String[] args) {
    Operation operation = new Retry<>(new FindCustomer("123", new CustomerNotFoundException(NOT_FOUND)), 3, 100));

    operation.perform();
  }
}