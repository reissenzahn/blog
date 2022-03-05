

Handle costly remote service calls in such a way that the failure of a single service/component cannot bring the whole application down, and we can reconnect to the service as soon as possible.

It is used to detect failures and encapsulates the logic of preventing a failure from constantly recurring, during maintenance, temporary external system failure or unexpected system difficulties.


class RemoteServiceException extends Exception {

  public RemoteServiceException(String message) {
    super(message);
  }
}

interface RemoteService {

  public String call() throws RemoteServiceException;
}

class WorkingRemoteService implements RemoteService {

  public String call() throws RemoteServiceException {
    return "Success!";
  }
}

class DelayedRemoteService implements RemoteService {

  private long startTime;
  private int delay;

  public SlowRemoteService() {
    this.startTime = System.nanoTime();

  }
}

class FaultyRemoteService implements RemoteService {

}

public class DelayedRemoteService implements RemoteService {

  private final long serverStartTime;
  private final int delay;

  /**
   * Constructor to create an instance of DelayedService, which is down for first few seconds.
   *
   * @param delay the delay after which service would behave properly, in seconds
   */
  public DelayedRemoteService(long serverStartTime, int delay) {
    this.serverStartTime = serverStartTime;
    this.delay = delay;
  }

  public DelayedRemoteService() {
    this.serverStartTime = System.nanoTime();
    this.delay = 20;
  }

  public String call() throws RemoteServiceException {
    var currentTime = System.nanoTime();
    //Since currentTime and serverStartTime are both in nanoseconds, we convert it to
    //seconds by diving by 10e9 and ensure floating point division by multiplying it
    //with 1.0 first. We then check if it is greater or less than specified delay and then
    //send the reply
    if ((currentTime - serverStartTime) * 1.0 / (1000 * 1000 * 1000) < delay) {
      //Can use Thread.sleep() here to block and simulate a hung server
      throw new RemoteServiceException("Delayed service is down");
    }
    return "Delayed service is working";
  }
}

enum CircuitBreakerState {
  CLOSED,
  OPEN,
  HALF_OPEN
}

interface CircuitBreaker {

  // Success response. Reset everything to defaults
  void recordSuccess();

  // Failure response. Handle accordingly with response and change state if required.
  void recordFailure(String response);

  // Get the current state of circuit breaker
  String getState();

  // Set the specific state manually.
  void setState(State state);

  // Attempt to fetch response from the remote service.
  String attemptRequest() throws RemoteServiceException;
}



class DefaultCircuitBreaker implements CircuitBreaker {

  private long timeout;  // timeout for API requests
  private long retryTimePeriod;
  private RemoteService service;
  private int failureThreshold;
  private CircuitBreakerState state;
  private int failureCount;
  private long lastFailureTime;


  public String attemptRequest() throws RemoteServiceException {
    if (failureCount >= failureThreshold) {
      if ((System.nanoTime() 0 lastFailureTime))
    }
  }
}

  public String attemptRequest() throws RemoteServiceException {
    if (failureCount >= failureThreshold) { //Then something is wrong with remote service
      if ((System.nanoTime() - lastFailureTime) > retryTimePeriod) {
        //We have waited long enough and should try checking if service is up
        state = State.HALF_OPEN;
      } else {
        //Service would still probably be down
        state = State.OPEN;
      }
    } else {
      //Everything is working fine
      state = State.CLOSED;
    }

    if (state == State.OPEN) {
      // return cached response if the circuit is in OPEN state
      return this.lastFailureResponse;
    } else {
      // Make the API request if the circuit is not OPEN
      try {
        //In a real application, this would be run in a thread and the timeout
        //parameter of the circuit breaker would be utilized to know if service
        //is working. Here, we simulate that based on server response itself
        var response = service.call();
        // Yay!! the API responded fine. Let's reset everything.
        recordSuccess();
        return response;
      } catch (RemoteServiceException ex) {
        recordFailure(ex.getMessage());
        throw ex;
      }
    }
  }




  private final long timeout;
  private final long retryTimePeriod;
  private final RemoteService service;
  private String lastFailureResponse;
  private final long futureTime = 1000 * 1000 * 1000 * 1000;

  /**
   * Constructor to create an instance of Circuit Breaker.
   *
   * @param failureThreshold Number of failures we receive from the depended service before changing
   *                         state to 'OPEN'
   * @param retryTimePeriod  Time period after which a new request is made to remote service for
   *                         status check.
   */
  DefaultCircuitBreaker(RemoteService serviceToCall, long timeout, int failureThreshold,
      long retryTimePeriod) {
    this.service = serviceToCall;
    // We start in a closed state hoping that everything is fine
    this.state = State.CLOSED;
    this.failureThreshold = failureThreshold;
    // Timeout for the API request.
    // Used to break the calls made to remote resource if it exceeds the limit
    this.timeout = timeout;
    this.retryTimePeriod = retryTimePeriod;
    //An absurd amount of time in future which basically indicates the last failure never happened
    this.lastFailureTime = System.nanoTime() + futureTime;
    this.failureCount = 0;
  }

  // Reset everything to defaults
  @Override
  public void recordSuccess() {
    this.failureCount = 0;
    this.lastFailureTime = System.nanoTime() + futureTime;
    this.state = State.CLOSED;
  }

  @Override
  public void recordFailure(String response) {
    failureCount = failureCount + 1;
    this.lastFailureTime = System.nanoTime();
    // Cache the failure response for returning on open state
    this.lastFailureResponse = response;
  }


  /**
   * Break the circuit beforehand if it is known service is down Or connect the circuit manually if
   * service comes online before expected.
   *
   * @param state State at which circuit is in
   */
  @Override
  public void setState(State state) {
    this.state = state;
    switch (state) {
      case OPEN:
        this.failureCount = failureThreshold;
        this.lastFailureTime = System.nanoTime();
        break;
      case HALF_OPEN:
        this.failureCount = failureThreshold;
        this.lastFailureTime = System.nanoTime() - retryTimePeriod;
        break;
      default:
        this.failureCount = 0;
    }
  }


}
