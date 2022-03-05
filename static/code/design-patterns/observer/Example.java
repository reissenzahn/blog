import java.util.*;

interface Subject {

  public void registerObserver(Observer observer);
  public void removeObserver(Observer observer);
  public void notifyObservers();
}

class WeatherData implements Subject {

  private List<Observer> observers;
  private float temperature;
  private float humidity;
  private float pressure;

  public WeatherData() {
    observers = new ArrayList<Observer>();
  }

  public void registerObserver(Observer observer) {
    observers.add(observer);
  }

  public void removeObserver(Observer observer) {
    observers.remove(observer);
  }

  public void notifyObservers() {
    for (Observer observer : observers) {
      observer.update();
    }
  }

  public void measurementsChanged() {
    notifyObservers();
  }
  
  // test method for adding new measurements
  public void setMeasurements(float temperature, float humidity, float pressure) {
    this.temperature = temperature;
    this.humidity = humidity;
    this.pressure = pressure;
    measurementsChanged();
  }

  // other WeatherData methods here
}


interface Observer {

  public void update();
}

interface DisplayElement {

  public void display();
}

public class CurrentConditionsDisplay implements Observer, DisplayElement {

  private WeatherData weatherData;
  private float temperature;
  private float humidity;

  public CurrentConditionsDisplay(WeatherData weatherData) {

    // store the reference to unregister as an observer
    this.weatherData = weatherData;
    weatherData.registerObserver(this);
  }

  public void update() {
    temperature = weatherData.getTemperature();
    humidity = weatherData.getHumidity();

    // in this simple example we just call display() when the values change though an MVC approach might be better
    display();
  }

  public void display() {
    System.out.println("Current conditions: " + temperature + "F degrees and " + humidity + "% humidity");
  }
}

public class ForecastDisplay implements Observer, DisplayElement {

  private WeatherData weatherData;
  private float currentPressure = 29.92f;
  private float lastPressure;

  public ForecastDisplay(WeatherData weatherData) {

    // store the reference to unregister as an observer
    this.weatherData = weatherData;
    weatherData.registerObserver(this);
  }

  public void update() {
    lastPressure = currentPressure;
    currentPressure = weatherData.getPressure();

    // in this simple example we just call display() when the values change though an MVC approach might be better
    display();
  }

  public void display() {
    System.out.println("Forecast: ???");
  }
}

public class Example {

  // $ javac Example.java && java Example
  public static void main(String[] args) {

  }
} 

public class WeatherStation {
 public static void main(String[] args) {
 WeatherData weatherData = new WeatherData();

 CurrentConditionsDisplay currentDisplay =
 new CurrentConditionsDisplay(weatherData);
 StatisticsDisplay statisticsDisplay = new StatisticsDisplay(weatherData);
 ForecastDisplay forecastDisplay = new ForecastDisplay(weatherData);
 weatherData.setMeasurements(80, 65, 30.4f);
 weatherData.setMeasurements(82, 70, 29.2f);
 weatherData.setMeasurements(78, 90, 29.2f);
 }
}
