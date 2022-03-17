
JUnit in Action

8
9
14
21
22
A


# 18. Testing a REST API

For now, let’s just say that if your API is re-defining the HTTP verbs or if it is
assigning new meanings to HTTP status codes or making up its own status codes, it
is not RESTful.
—George Reese, The REST API Design Handbook
Representational state transfer (REST) is a software architectural style for creating
web services; it also provides a set of constraints. The American computer scientist
Roy Fielding first defined REST, presenting the REST principles in his PhD dissertation in 2000. (Fielding is also one of the authors of the HTTP specification.) Web
services following this REST architectural style are called RESTful web services. RESTful web services allow interoperability between the internet and computer systems.
Requesting systems can access and manipulate web resources represented as text
using a well-known set of stateless operations. We’ll study them in more detail in
this chapter.
This chapter covers
 Creating a RESTful API to manage one or more
entities
 Testing a RESTful API
Introducing REST applications 349
18.1 Introducing REST applications
We will first define the terms client and resource in order to shape what makes an API
RESTful. A client may be a person or software using the RESTful API. For example, a
programmer using a RESTful API to execute actions against the LinkedIn website is
such a client. The client can also be a web browser. When you go to the LinkedIn website, your browser is the client that calls the website API and displays the obtained
information on the screen.
 A resource can be any object about which the API can obtain information. In the
LinkedIn API, a resource can be a message, a photo, or a user. Each resource has a
unique identifier.
 The REST architecture style defines six constraints:
 Client-server—Clients are separated from servers, and each has its own concern.
Most frequently, a client is concerned with the representation of the user, and a
server is concerned with data storage and domain model logic—the conceptual
model of a domain including data and behavior.
 Stateless—The server does not keep any information about the client between
requests. Each request from a client contains all of the information necessary to
respond to that request. The client keeps the state on its side.
 Uniform interface—The client and the server may evolve independently of each
other. The uniform interface between them makes them loosely coupled.
 Layered systems—The client does not have any way to determine if it is interacting directly with the server or with an intermediary. Layers can be dynamically
added and removed. They can provide security, load balancing, and shared
caching.
 Cacheable—Clients can cache responses. Responses define themselves as cacheable or not.
 Code on demand (optional)—Servers can temporarily customize or extend the
functionality of a client. The server can transfer to the client some logic that the
client can execute: JavaScript client-side scripts and Java applets.
A RESTful web application provides information about its resources. Resources are
identified with the help of URLs. The client can execute actions against such a
resource: create, read, update, or delete the resource.
 The REST architectural style is not protocol specific, but the most widely used is
REST over HTTP. HTTP is a synchronous application network protocol based on
requests and responses.
 To make an API RESTful, you have to follow a set of rules while developing it. A
RESTful API will transfer information to the client, which uses it as a representation of
the state of the accessed resource. For example, when you call the LinkedIn API to
access a specific user, the API will return the state of that user (name, biography, professional experience, posts). The REST rules make the API easier to understand and
simpler for new programmers to use when they join a team.
350 CHAPTER 18 Testing a REST API
 The representation of the state can be in JSON, XML, or HTML format. The client
uses the API to send the following to the server:
 The identifier (URL) of the resource you want to access.
 The operation you want the server to perform on that resource. This is an
HTTP method. The common HTTP methods are GET, POST, PUT, PATCH, and
DELETE.
For example, using the LinkedIn RESTful API to fetch a specific LinkedIn user
requires that you have a URL that identifies the user and that you use the HTTP
method GET.
18.2 Creating a RESTful API to manage one entity
The flight-management application developed at Tested Data Systems, as we left it at
the end of chapter 17, allows us to register a list of passengers for a flight and test the
confirmation of this registration. Mike, the programmer involved in developing the
application, receives a new feature to implement: he has to create a REST API to manage a flight’s passengers.
 The first thing Mike will do is to create the REST API that will deal with countries.
For that purpose, Mike will add new dependencies to the Maven pom.xml configuration file.
<dependency>
 <groupId>org.springframework.boot</groupId>
 <artifactId>spring-boot-starter-web</artifactId>
</dependency>
<dependency>
 <groupId>org.springframework.boot</groupId>
 <artifactId>spring-boot-starter-data-jpa</artifactId>
</dependency>
<dependency>
 <groupId>com.h2database</groupId>
 <artifactId>h2</artifactId>
</dependency>
In this listing:
 Mike adds the spring-boot-starter-web dependency for building web
(including RESTful) applications using Spring Boot B.
 Because he needs to use Spring and the Java Persistence API (JPA) for persisting information, he includes the spring-boot-starter-data-jpa dependency c.
 H2 is an open source, lightweight Java database that Mike will embed in the Java
application and persist information to it. Consequently, he adds the h2 dependency d.
Listing 18.1 Newly added dependencies in the pom.xml configuration file
b
c
d
Creating a RESTful API to manage one entity 351
As the application will be a functional RESTful one that can be used to access, create,
modify, and delete information about the list of passengers, Mike moves the FlightBuilder class from the test folder into the main folder. He also extends its functionality to provide access to managing countries.
[...]
public class FlightBuilder {
 private Map<String, Country> countriesMap = new HashMap<>();
 public FlightBuilder() throws IOException {
 try(BufferedReader reader = new BufferedReader(new
 FileReader("src/main/resources/countries_information.csv"))) {
 String line = null;
 do {
 line = reader.readLine();
 if (line != null) {
 String[] countriesString = line.toString().split(";");
 Country country = new Country(countriesString[0].trim(),
 countriesString[1].trim());
 countriesMap.put(countriesString[1].trim(), country);
 }
 } while (line != null);
 }
 }
 @Bean
 Map<String, Country> getCountriesMap() {
 return Collections.unmodifiableMap(countriesMap);
 }
 @Bean
 public Flight buildFlightFromCsv() throws IOException {
 Flight flight = new Flight("AA1234", 20);
 try(BufferedReader reader = new BufferedReader(new
 FileReader("src/main/resources/flights_information.csv"))) {
 String line = null;
 do {
 line = reader.readLine();
 if (line != null) {
 String[] passengerString = line.toString()
 .split(";");
 Passenger passenger = new
 Passenger(passengerString[0].trim());

 passenger.setCountry(
 countriesMap.get(passengerString[1].trim()));
 passenger.setIsRegistered(false);
 flight.addPassenger(passenger);
 }
 } while (line != null);
 }
Listing 18.2 FlightBuilder class
b
c
d
e
f
g
h
i
j
1)
1! 1@
1#
1$
1%
1^
1* 1&
1(
352 CHAPTER 18 Testing a REST API
 return flight;
 }
}
In this listing:
 Mike defines the countries map B.
 He populates the countries map in the constructor with three countries c by
parsing the CSV file d.
 He initializes the line variable with null e, parses the CSV file, and reads it
line by line f.
 The line is split with the ; separator g. Mike creates a country with the help of
the constructor, which has as an argument the part of the line before the separator h.
 He adds the parsed information to countriesMap i.
 He creates the getCountriesMap method and annotates it with @Bean j. Its
purpose is to create and configure a Map bean that will be injected into the
application.
 He creates the buildFlightFromCsv method and annotates it with @Bean
1). Its purpose is to create and configure a Flight bean that will be injected
into the application.
 He creates a flight with the help of the constructor 1! by parsing the CSV file
1@.
 He initializes the line variable with null 1#, parses the CSV file, and reads it
line by line 1$.
 The line is split with the ; separator 1%. Mike creates a passenger with the help
of the constructor, which has as an argument the part of the line before the separator 1^. He sets the passenger’s country by taking from the countries map the
value corresponding to the country code included in the part of the line after
the separator 1&.
 He sets the passenger as not registered 1* and adds them to the flight 1(. After
all the lines from the CSV file are parsed, the fully configured flight is returned
2).
The content of the countries_information.csv file used for building the countries map
is shown next.
Australia; AU
USA; US
United Kingdom; UK
Mike would like to start the RESTful application on the custom 8081 port to avoid
possible port conflicts. Spring Boot allows the externalization of the configuration so
Listing 18.3 countries_information.csv file
2)
Creating a RESTful API to manage one entity 353
that different people can work with the same application code in different environments. Various properties can be specified in the application.properties file; in this
case, Mike only needs to set server.port as 8081.
server.port=8081
Mike modifies the Country class to make it a model component of the RESTful application (see listing 18.5). The model is one of the components of the model-viewcontroller (MVC) pattern: it is the application’s dynamic data structure, independent
of the user interface. It directly manages the application’s data.
DEFINITION Model-view-controller (MVC)—A software design pattern used for
creating programs that access data through user interfaces. The program is
split into three related parts: model, view, and controller. Thus the inner representation of information is separated from its presentation on the user side,
and the system’s parts are loosely coupled.
[...]
@Entity
public class Country {
 @Id
 private String codeName;
 private String name;
 // avoid "No default constructor for entity"
 public Country() {
 }
 [...]
}
In this listing:
 Mike annotates the Country class with @Entity so it can represent objects that
can be persisted B.
 He annotates the codeName field with @Id c. This means the codeName field
is a primary key: it uniquely identifies an entity that is persisted.
 He adds a default constructor to the Country class d. Every class annotated
with @Entity needs a default constructor, as the persistence layer will use it to
create a new instance of this class through reflection. The compiler no longer
provides the default constructor because Mike created one.
Next, Mike creates the CountryRepository interface, which extends JpaRepository.
Listing 18.4 application.properties file
Listing 18.5 Modified Country class
b
c
d
354 CHAPTER 18 Testing a REST API
public interface CountryRepository extends JpaRepository<Country, Long> {
}
Defining this interface serves two purposes. First, by extending JpaRepository,
Mike gets a bunch of generic CRUD (create, read, update, and delete) methods in the
CountryRepository type that make it possible to work with Country objects. Second, this allows Spring to scan the classpath for this interface and create a Spring
bean for it.
DEFINITION CRUD (create, read, update, delete)—The four basic functions of persistence. In addition to REST, they are often used in database applications
and user interfaces.
Mike now writes a controller for CountryRepository. A controller is responsible for
controlling the application logic and acts as the coordinator between the view (the
way data is displayed to the user) and the model (the data).
@RestController
public class CountryController {
 @Autowired
 private CountryRepository repository;
 @GetMapping("/countries")
 List<Country> findAll() {
 return repository.findAll();
 }
}
In this listing:
 Mike creates the CountryController class and annotates it with @RestController B. The @RestController annotation was introduced in Spring
4.0 to simplify creating RESTful web services. It marks the class as a controller
and also eliminates the need to annotate every one of its request-handling
methods with the @ResponseBody annotation (as required before Spring 4.0).
 He declares a CountryRepository field and autowires it c. As CountryRepository extends JpaRepository, Spring will scan the classpath for this
interface, create a Spring bean for it, and autowire it here.
 He creates the findAll method and annotates it with @GetMapping("/countries") d. The @GetMapping annotation maps HTTP GET requests to the
/countries URL onto the specific handler method. Because Mike uses the
@RestController annotation on the class itself, he does not need to annotate
Listing 18.6 CountryRepository interface
Listing 18.7 CountryController class
b
c
d e
Creating a RESTful API to manage one entity 355
the response object of the method with @ResponseBody. As mentioned earlier,
that was necessary before Spring 4.0, when we also had to use @Controller
instead of @RestController on the class.
 The findAll method returns the result of executing repository
.findAll() e. repository.findAll() is a CRUD method that is automatically generated because CountryRepository extends JpaRepository. As its
name says, it will return all objects from the repository.
Mike now revises the Application class, previously created by Spring Boot (see
chapter 17).
[...]
@SpringBootApplication
@Import(FlightBuilder.class)
public class Application {
 @Autowired
 private Map<String, Country> countriesMap;
 public static void main(String[] args) {
 SpringApplication.run(Application.class, args);
 }
 @Bean
 CommandLineRunner configureRepository
 (CountryRepository countryRepository) {
 return args -> {
 for (Country country: countriesMap.values()) {
 countryRepository.save(country);
 }
 };
 }
}
In this listing:
 Mike imports FlightBuilder, which created the countriesMap bean B, and
autowires it c.
 He creates a bean of type CommandLineRunner d. CommandLineRunner is a
Spring Boot functional interface (an interface with a single method) that gives
access to application arguments as a string array. The created bean browses all
the countries in countriesMap and saves them into countryRepository e.
This CommandLineRunner interface is created, and its single method is executed, just before the run() method from SpringApplication completes.
Mike now executes Application. So far, the RESTful application only provides
access to the /countries endpoint through the GET method. (An endpoint is a resource
Listing 18.8 Modified Application class
b
c
d
e
356 CHAPTER 18 Testing a REST API
that can be referenced and to which client messages can be addressed.) We can test
this REST API endpoint using the curl program. curl (which stands for client URL)
is a command-line tool that transfers data using various protocols, including HTTP.
We simply execute the following command
curl -v localhost:8081/countries
because the application is running on port 8081 and /countries is the only available
endpoint. The result is shown in figure 18.1—the command lists the countries in
JSON format.
Figure 18.1 The result of running the curl -v localhost:8081/countries command is the list of created
countries.
We can also test accessing this endpoint through the browser by going to localhost:8081/countries. Again, the result is provided in JSON format, as shown in figure
18.2.
Figure 18.2 Accessing the
localhost:8081/countries URL
from the browser displays the list
of created countries.
Creating a RESTful API to manage two related entities 357
18.3 Creating a RESTful API to manage two related entities
Mike continues the implementation by modifying the Passenger class to make it a
model component of the RESTful application.
@Entity
public class Passenger {
 @Id
 @GeneratedValue
 private Long id;
 private String name;
 @ManyToOne
 private Country country;
 private boolean isRegistered;
 // avoid "No default constructor for entity"
 public Passenger() {
 }
 [...]
}
In this listing:
 Mike annotates the Passenger class with @Entity so it has the ability to represent objects that can be persisted B.
 He introduces a new Long field, id, and annotates it with @Id and
@GeneratedValue c. This means the id field is a primary key, and its value
will be automatically generated for that field by the persistence layer.
 He annotates the country field with @ManyToOne d. This means many passengers can be mapped to a single country.
 He adds a default constructor to the Passenger class e. Every class annotated
with @Entity needs a default constructor, as the persistence layer will use it to
create a new instance of this class through reflection.
Mike next creates the PassengerRepository interface, which extends JpaRepository.
public interface PassengerRepository extends JpaRepository<Country, Long> {
}
Defining this interface serves two purposes. First, extending JpaRepository gives
the PassengerRepository generic CRUD methods that let Mike work with Passenger objects. Second, this allows Spring to scan the classpath for this interface and
create a Spring bean for it.
Listing 18.9 Modified Passenger class
Listing 18.10 PassengerRepository interface
b
c
d
e
358 CHAPTER 18 Testing a REST API
 Mike now writes a custom exception to be thrown when a passenger is not found.
public class PassengerNotFoundException extends RuntimeException {
 public PassengerNotFoundException(Long id) {
 super("Passenger id not found : " + id);
 }
}
In this listing, Mike declares PassengerNotFoundException by extending RuntimeException B and creates a constructor that receives an id as a parameter c.
 Next, Mike writes a controller for PassengerRepository.
[...]
@RestController
public class PassengerController {
 @Autowired
 private PassengerRepository repository;
 @Autowired
 private Map<String, Country> countriesMap;
 @GetMapping("/passengers")
 List<Passenger> findAll() {
 return repository.findAll();
 }
 @PostMapping("/passengers")
 @ResponseStatus(HttpStatus.CREATED)
 Passenger createPassenger(@RequestBody Passenger passenger) {
 return repository.save(passenger);
 }
 @GetMapping("/passengers/{id}")
 Passenger findPassenger(@PathVariable Long id) {
 return repository.findById(id)
 .orElseThrow(() -> new PassengerNotFoundException(id));
 }
 @PatchMapping("/passengers/{id}")
 Passenger patchPassenger(@RequestBody Map<String, String> updates,
 @PathVariable Long id) {
 return repository.findById(id)
 .map(passenger -> {
Listing 18.11 PassengerNotFoundException class
Listing 18.12 PassengerController class
b
c
b
c
d
e
f
g
h
j
i
1) 1!
1@
Creating a RESTful API to manage two related entities 359
 String name = updates.get("name");
 if (null!= name) {
 passenger.setName(name);
 }
 Country country =
 countriesMap.get(updates.get("country"));
 if (null != country) {
 passenger.setCountry(country);
 }
 String isRegistered = updates.get("isRegistered");
 if(null != isRegistered) {
 passenger.setIsRegistered(
 isRegistered.equalsIgnoreCase("true")? true: false);
 }
 return repository.save(passenger);
 })
 .orElseGet(() -> {
 throw new PassengerNotFoundException(id);
 });
 }
 @DeleteMapping("/passengers/{id}")
 void deletePassenger(@PathVariable Long id) {
 repository.deleteById(id);
 }
}
In this listing:
 Mike creates the PassengerController class and annotates it with @RestController B.
 He declares a PassengerRepository field and autowires it c. Because
PassengerRepository extends JpaRepository, Spring will scan the classpath for this interface, create a Spring bean for it, and autowire it here. Mike
also declares a countriesMap field and autowires it d.
 He creates the findAll method and annotates it with @GetMapping("/passengers") e. This @GetMapping annotation maps HTTP GET requests to
the /passengers URL onto the specific handler method.
 He declares the createPassenger method and annotates it with @PostMapping("/passengers") f. @PostMapping-annotated methods handle
the HTTP POST requests matched with given URL expressions. Mike marks that
method with @ResponseStatus, specifying the response status as HttpStatus.CREATED g. The @RequestBody annotation maps the HttpRequest
body to the annotated domain object. The HttpRequest body is deserialized
to a Java object h.
 Mike declares the findPassenger method that will look for the passenger by
ID and annotates it with @GetMapping("/passengers/{id}") i. The
1#
1$
1%
1^
1&
1*
1(
360 CHAPTER 18 Testing a REST API
@GetMapping annotation maps HTTP GET requests to the /passengers/{id}
URL onto the specific handler method. It searches for the passenger in the
repository and returns them; or, if the passenger does not exist, it throws the
custom declared PassengerNotFoundException. The id argument of the
method is annotated with @PathVariable, meaning it will be extracted from
the URL’s {id} value j.
 Mike declares the patchPassenger method and annotates it with @PatchMapping("/passengers/{id}") 1). The @PatchMapping annotation maps
HTTP PATCH requests to the /passengers/{id} URL onto the specific handler
method. He annotates the updates parameter with @RequestBody and the id
parameter with @PathVariable 1!. The @RequestBody annotation maps the
HttpRequest body to the annotated domain object. The HttpRequest body
is deserialized to a Java object. The id argument of the method, annotated with
@PathVariable, will be extracted from the URL’s {id} value.
 Mike searches the repository by the input id 1@. He changes the name of an
existing passenger 1#, the country 1$, and the registration status 1%. The modified passenger is saved in the repository 1^.
 If the passenger does not exist, the custom declared PassengerNotFoundException is thrown 1&.
 Mike declares the delete method and annotates it with @DeleteMapping
("/passengers/{id}") 1*. The @DeleteMapping annotation maps HTTP
DELETE requests to the /passengers/{id} URL onto the specific handler
method. It deletes the passenger from the repository. The id argument of the
method is annotated with @PathVariable, meaning it will be extracted from
the URL’s {id} value 1(.
Mike now modifies the Application class.
@SpringBootApplication
@Import(FlightBuilder.class)
public class Application {
 @Autowired
 private Flight flight;
 @Autowired
 private Map<String, Country> countriesMap;
 public static void main(String[] args) {
 SpringApplication.run(Application.class, args);
 }
 @Bean
 CommandLineRunner configureRepository
 (CountryRepository countryRepository,
 PassengerRepository passengerRepository) {
 return args -> {
Listing 18.13 Modified Application class
b
c
Creating a RESTful API to manage two related entities 361
 for (Country country: countriesMap.values()) {
 countryRepository.save(country);
 }
 for (Passenger passenger : flight.getPassengers()) {
 passengerRepository.save(passenger);
 }
 };
 }
}
In this listing:
 Mike autowires the flight bean B imported from the FlightBuilder.
 He modifies the bean of type CommandLineRunner by adding a new parameter
of type PassengerRepository to the configureRepository method that
creates it c. CommandLineRunner is a Spring Boot interface that provides
access to application arguments as a string array. The created bean will additionally browse all passengers in the flight and save them in passengerRepository d. This CommandLineRunner interface is created, and its
method is executed, just before the run() method from SpringApplication
completes.
The list of passengers of the flight is shown in the following CSV file. A passenger is
described using a name and a country code; there are 20 passengers total.
John Smith; UK
Jane Underwood; AU
James Perkins; US
Mary Calderon; US
Noah Graves; UK
Jake Chavez; AU
Oliver Aguilar; US
Emma McCann; AU
Margaret Knight; US
Amelia Curry; UK
Jack Vaughn; US
Liam Lewis; AU
Olivia Reyes; US
Samantha Poole; AU
Patricia Jordan; UK
Robert Sherman; US
Mason Burton; AU
Harry Christensen; UK
Jennifer Mills; US
Sophia Graham; UK
When the application starts, the FlightBuilder class parses the file, creates the
flight with the list of passengers, and injects the flight into the application. The application browses the list and saves each passenger in the repository.
Listing 18.14 flights_information.csv file
d
362 CHAPTER 18 Testing a REST API
 Mike launches Application into execution. The RESTful application now also
provides access to the /passengers endpoint. We can test the new functionalities of
the REST API endpoint using the curl program:
curl -v localhost:8081/passengers
The application is running on port 8081, and /passengers is available as an endpoint.
The result is shown in figure 18.3: a list of passengers in JSON format.
Figure 18.3 The result of running the curl -v localhost:8081/passengers command is
the list of passengers.
We can also test the other functionalities implemented for the /passengers endpoint.
For example, to get the passenger having ID 4, we execute this command:
curl -v localhost:8081/passengers/4
The result is shown in figure 18.4: the passenger information is provided in JSON
format.
Figure 18.4 Running the curl -v localhost:8081/passengers/4 command shows the
passenger with ID 4.
Creating a RESTful API to manage two related entities 363
We can update the name, country, and registered status of the passenger with ID 4 by
executing this command:
curl -v -X PATCH localhost:8081/passengers/4
-H "Content-type:application/json"
-d "{\"name\":\"Sophia Jones\", \"country\":\"AU\",
 \"isRegistered\":\"true\"}"
The result is shown in figure 18.5.
Figure 18.5 The result of successfully updating the information for the passenger with ID 4
To delete this passenger, we use this command:
curl -v -X DELETE localhost:8081/passengers/4
The result is shown in figure 18.6.
Figure 18.6 The passenger with ID 4 has been deleted.
364 CHAPTER 18 Testing a REST API
Finally, we can post a new passenger:
curl -v -X POST localhost:8081/passengers
-H "Content-type:application/json"
-d "{\"name\":\"John Smith\"}"
The result is shown in figure 18.7.
Figure 18.7 We successfully posted a new passenger, John Smith.
18.4 Testing the RESTful API
Next, Mike writes the tests to automatically verify the behavior of the RESTful API.
[...]
@SpringBootTest
@AutoConfigureMockMvc
@Import(FlightBuilder.class)
public class RestApplicationTest {
 @Autowired
 private MockMvc mvc;
 @Autowired
 private Flight flight;
 @Autowired
 private Map<String, Country> countriesMap;
 @MockBean
 private PassengerRepository passengerRepository;
 @MockBean
 private CountryRepository countryRepository;
Listing 18.15 RestApplicationTest class
b
c
d
e
f
g
Testing the RESTful API 365
 @Test
 void testGetAllCountries() throws Exception {
 when(countryRepository.findAll()).thenReturn(new
 ArrayList<>(countriesMap.values()));
 mvc.perform(get("/countries"))
 .andExpect(status().isOk())
 .andExpect(content().contentType(MediaType.APPLICATION_JSON))
 .andExpect(jsonPath("$", hasSize(3)));
 verify(countryRepository, times(1)).findAll();
 }
 @Test
 void testGetAllPassengers() throws Exception {
 when(passengerRepository.findAll()).thenReturn(new
 ArrayList<>(flight.getPassengers()));
 mvc.perform(get("/passengers"))
 .andExpect(status().isOk())
 .andExpect(content().contentType(MediaType.APPLICATION_JSON))
 .andExpect(jsonPath("$", hasSize(20)));
 verify(passengerRepository, times(1)).findAll();
 }
 @Test
 void testPassengerNotFound() {
 Throwable throwable = assertThrows(NestedServletException.class,
 () -> mvc.perform(get("/passengers/30"))
 .andExpect(status().isNotFound()));
 assertEquals(PassengerNotFoundException.class,
 throwable.getCause().getClass());
 }
 @Test
 void testPostPassenger() throws Exception {
 Passenger passenger = new Passenger("Peter Michelsen");
 passenger.setCountry(countriesMap.get("US"));
 passenger.setIsRegistered(false);
 when(passengerRepository.save(passenger))
 .thenReturn(passenger);
 mvc.perform(post("/passengers")
 .content(new ObjectMapper().writeValueAsString(passenger))
 .header(HttpHeaders.CONTENT_TYPE, MediaType.APPLICATION_JSON))
 .andExpect(status().isCreated())
 .andExpect(jsonPath("$.name", is("Peter Michelsen")))
 .andExpect(jsonPath("$.country.codeName", is("US")))
 .andExpect(jsonPath("$.country.name", is("USA")))
 .andExpect(jsonPath("$.registered", is(Boolean.FALSE)));
 verify(passengerRepository, times(1)).save(passenger);
 }
h i
j
1)
1!
1@
1#
1$
1%
1^
1&
1*
366 CHAPTER 18 Testing a REST API
 @Test
 void testPatchPassenger() throws Exception {
 Passenger passenger = new Passenger("Sophia Graham");
 passenger.setCountry(countriesMap.get("UK"));
 passenger.setIsRegistered(false);
 when(passengerRepository.findById(1L))
 .thenReturn(Optional.of(passenger));
 when(passengerRepository.save(passenger))
 .thenReturn(passenger);
 String updates =
 "{\"name\":\"Sophia Jones\", \"country\":\"AU\",
 \"isRegistered\":\"true\"}";
 mvc.perform(patch("/passengers/1")
 .content(updates)
 .header(HttpHeaders.CONTENT_TYPE,
 MediaType.APPLICATION_JSON))
 .andExpect(content().contentType(MediaType.APPLICATION_JSON))
 .andExpect(status().isOk());
 verify(passengerRepository, times(1)).findById(1L);
 verify(passengerRepository, times(1)).save(passenger);
 }
 @Test
 public void testDeletePassenger() throws Exception {
 mvc.perform(delete("/passengers/4"))
 .andExpect(status().isOk());
 verify(passengerRepository, times(1)).deleteById(4L);
 }
}
In this listing:
 Mike creates the RestApplicationTest class and annotates it with @SpringBootTest B. @SpringBootTest searches the current package of the test class
and its subpackages for bean definitions.
 He also annotates the class with @AutoConfigureMockMvc in order to enable
all autoconfiguration related to the MockMvc objects used in the test c.
 He imports FlightBuilder, which creates a flight bean and a countries map
bean d.
 He autowires a MockMvc object e. MockMvc is the main entry point for serverside Spring REST application testing: Mike will perform a series of REST operations against this MockMvc object during the tests.
 He declares a flight and a countriesMap field and autowires them f. These
fields are injected from the FlightBuilder class.
 He declares countryRepository and passengerRepository fields and
annotates them with @MockBean g. @MockBean is used to add mock objects to
1(
2)
2!
2@
2#
2$
Testing the RESTful API 367
the Spring application context; the mock will replace any existing bean of the
same type in the application context. Mike will provide instructions for the
behavior of the mock objects during the tests.
 In the testGetAllCountries test, he instructs the mock countryRepository bean to return the array of values from countriesMap when the
findAll method is executed on it h.
 Mike simulates the execution of the GET method on the /countries URL i and
verifies the returned status, expected content type, and returned JSON size j.
He also verifies that the method findAll has been executed exactly once on
the countryRepository bean 1).
 In the testGetAllPassengers test, he instructs the mock passengerRepository bean to return the passengers from the flight bean when the
findAll method is executed on it 1!.
 He simulates the execution of the GET method on the /passengers URL and
verifies the returned status, expected content type, and returned JSON size 1@.
He also verifies that the method findAll has been executed exactly once on
the passengerRepository bean 1#.
 In testPassengerNotFound, Mike tries to get the passenger having ID 30
and checks that a NestedServletException is thrown and that the returned
status is “Not Found” 1$. He also checks that the cause of the NestedServletException is PassengerNotFoundException 1%.
 In testPostPassenger, Mike creates a passenger object, configures it, and
instructs passengerRepository to return that object when a save is executed on that passenger 1^.
 He simulates the execution of the POST method on the /passengers URL and
verifies that the content consists of the JSON string value of the passenger
object, the header type, the returned status, and the content of the JSON 1&.
He uses an object of type com.fasterxml.jackson.databind.ObjectMapper, which is the main class of the Jackson library (the standard JSON
library for Java). ObjectMapper offers functionality for reading and writing
JSON to and from basic POJOs.
 Mike also verifies that the save method has been executed exactly once on the
previously defined passenger 1*.
 In testPatchPassenger, he creates a passenger object, configures it, and
instructs passengerRepository to return that object when a passenger
findById is executed with the argument 1 1(. When the save method is executed on passengerRepository, that passenger is returned as well 2).
 Mike sets a JSON object named updates, performs a PATCH on the /passengers/1
URL using that update, and checks the content and the returned status 2!.
 He verifies that the findById and save methods have been executed exactly
once on passengerRepository 2@.
368 CHAPTER 18 Testing a REST API
 He performs a DELETE operation on the /passengers/4 URL, verifies that the
returned status is OK 2#, and verifies that the deleteById method has been
executed exactly once 2$.
Running RestApplicationTest is successful, as shown in figure 18.8.
Figure 18.8 Successfully running RestApplicationTest, which checks the RESTful application’s
functionality
The next chapter will be dedicated to testing database applications and the various
alternatives for doing so.
Summary
This chapter has covered the following:
 Introducing the REST architectural style and the concept of REST applications
 Demonstrating what makes an API RESTful, and the REST architecture constraints: client-server, stateless, uniform interface, layered system, cacheable,
and code on demand
 Creating a RESTful API to manage a single entity—the country from the flightmanagement application—and executing GET operations against it to obtain
the list of countries
 Creating a RESTful API to manage two related entities—the country and the
passenger—and executing GET, PATCH, DELETE, and POST operations against it
to get the list of passengers; to get a particular passenger by ID; and to create,
update, or delete a passenger
 Testing the RESTful API that manages two related entities by creating and executing tests against a Spring REST MockMvc object to test the previously mentioned GET, PATCH, DELETE, and POST operations


# 1. JUnit Jump-Start

<dependency>
 <groupId>org.junit.jupiter</groupId>
 <artifactId>junit-jupiter-api</artifactId>
 <version>5.6.0</version>
 <scope>test</scope>
</dependency>
<dependency>
 <groupId>org.junit.jupiter</groupId>
 <artifactId>junit-jupiter-engine</artifactId>
 <version>5.6.0</version>
 <scope>test</scope>
</dependency>

<build>
 <plugins>
 <plugin>
 <artifactId>maven-surefire-plugin</artifactId>
 <version>2.22.2</version>
 </plugin>
 </plugins>
</build>

```java
public class Calculator {
 public double add(double number1, double number2) {
 return number1 + number2;
 }
}

You also need to configure the JAVA_HOME environment variable on your
OS to point to the Java installation folder (figure 1.2).

mvn clean install

This command will take the Java source code, compile it, test it, and convert it into a runnable Java program (a jar file, in our case).


@BeforeEach, @BeforeAll, @AfterEach, and @AfterAll (starting from version 5); and @Before, @BeforeClass, @After, and @AfterClass (up to
version 4)

import static org.junit.jupiter.api.Assertions.assertEquals;
import org.junit.jupiter.api.Test;

// It’s common practice to end the class name with Test.
public class CalculatorTest {

  // annotation marks the method as a unit test method
 @Test
 public void testAdd() {
 Calculator calculator = new Calculator();
 double result = calculator.add(10, 50);

 // assert that expected and actual are equal within the non-negative delta
 assertEquals(60, result, 0);
 }
}

if the actual value is not equal to the expected value, JUnit
throws an unchecked exception, which causes the test to fail
```

# 2. Exploring core JUnit


```java
import static org.junit.jupiter.api.Assertions.assertEquals;
import org.junit.jupiter.api.Test;

public class CalculatorTest {
 @Test
 public void testAdd() {
 Calculator calculator = new Calculator();
 double result = calculator.add(10, 50);
 assertEquals(60, result, 0);
 }
}

These are the most important concepts:

- A test class may be a top-level class, a static member class, or an inner class annotated as @Nested that contains one or more test methods. Test classes cannot be abstract and must have a single constructor. The constructor must have no arguments, or arguments that can be dynamically resolved at runtime through dependency injection. A test class is allowed to be package-private as a minimum requirement for visibility.


- A test method is an instance method that is annotated with @Test, @RepeatedTest, @ParameterizedTest, @TestFactory, or @TestTemplate. Test methods must not be abstract and must not return a value (the return type should be void).

- A life cycle method is a method that is annotated with @BeforeAll, @AfterAll, @BeforeEach, or @AfterEach.


JUnit creates a new instance of the test class before invoking each @Test method to ensure the independence of test methods and prevent unintentional side effects in
the test code. Also, it is a universally accepted fact that the tests must produce the
same result independent of the order of their execution.

Because each test method
runs on a new test class instance, you cannot reuse instance variable values across test
methods. One test instance is created for the execution of each test method/

If you annotate your test class with @TestInstance(Lifecycle.PER_CLASS),
JUnit 5 will execute all test methods on the same test instance. A new test instance will
be created for each test class when using this annotation.

class SUTTest {
 private static ResourceForAllTests resourceForAllTests;
 private SUT systemUnderTest;

 @BeforeAll
 static void setUpClass() {
 resourceForAllTests =
 new ResourceForAllTests("Our resource for all tests");
 }

 @AfterAll
 static void tearDownClass() {
 resourceForAllTests.close();
 }

 @BeforeEach
 void setUp() {
 systemUnderTest = new SUT("Our system under test");
 }

 @AfterEach
 void tearDown() {
 systemUnderTest.close();
 }

 @Test
 void testRegularWork() {
 boolean canReceiveRegularWork =
 systemUnderTest.canReceiveRegularWork();
 assertTrue(canReceiveRegularWork);
 }

 @Test
 void testAdditionalWork() {
 boolean canReceiveAdditionalWork =
 systemUnderTest.canReceiveAdditionalWork();
 assertFalse(canReceiveAdditionalWork);
 }
}


- The method annotated with @BeforeAll B is executed once: before all tests.
This method needs to be static unless the entire test class is annotated with
@TestInstance(Lifecycle.PER_CLASS).

- The method annotated with @BeforeEach d is executed before each test.
- The method annotated with @AfterEach e is executed after each test.
 The method annotated with @AfterAll c is executed once: after all tests. This method needs to be static unless the entire test class is annotated with
@TestInstance(Lifecycle.PER_CLASS).


$ mvn -Dtest=SUTTest.java clean install.



- The @DisplayName annotation can be used over classes and test methods. It helps
the engineers at Tested Data Systems declare their own display name for an annotated
test class or test method. The name that’s displayed is
usually a full phrase that provides significant information about the purpose of the test.

@DisplayName("Test class showing the @DisplayName annotation.")
class DisplayNameTest {
 private SUT systemUnderTest = new SUT();
 @Test
 @DisplayName("Our system under test says hello.")
 void testHello() {
 assertEquals("Hello", systemUnderTest.hello());
 }
 @Test
 @DisplayName("😱")
 void testTalking() {
 assertEquals("How are you?", systemUnderTest.talk());
 }
 @Test
 void testBye() {
 assertEquals("Bye", systemUnderTest.bye());
 }
}

A test that does not have an associated display name simply shows the method name.

The @Disabled annotation can be used over classes and test methods. It signals that
the annotated test class or test method is disabled and should not be executed. If this annotation is applied to a
class, it disables all the methods of the test.

@Disabled("Feature is still under construction.")
class DisabledClassTest {
 private SUT systemUnderTest= new SUT("Our system under test");
 @Test
 void testRegularWork() {
 boolean canReceiveRegularWork = systemUnderTest.
 canReceiveRegularWork();
 assertTrue(canReceiveRegularWork);
 }

 @Test
 void testAdditionalWork() {
 boolean canReceiveAdditionalWork =
 systemUnderTest.canReceiveAdditionalWork();
 assertFalse(canReceiveAdditionalWork);
 }

class DisabledMethodsTest {
 private SUT systemUnderTest= new SUT("Our system under test");
 @Test
 @Disabled
 void testRegularWork() {
 boolean canReceiveRegularWork =
 systemUnderTest.canReceiveRegularWork ();
 assertTrue(canReceiveRegularWork);
 }
 @Test
 @Disabled("Feature still under construction.")
 void testAdditionalWork() {
 boolean canReceiveAdditionalWork =
 systemUnderTest.canReceiveAdditionalWork ();
 assertFalse(canReceiveAdditionalWork);
 }
}

An inner class is a class that is a member of another class. It can access any private
instance variable of the outer class, as it is effectively part of that outer class. The typical use case is when two classes are tightly coupled, and it’s logical to provide direct
access from the inner class to all instance variables of the outer class. For example, we
may test a flight that has two types of passengers trying to board. The behavior of the
flight will be described in the outer test class, while the behavior of each type of
Listing 2.6 @Disabled annotation used on methods
b
c
passenger will be described in its own nested class. Each passenger is able to interact
with the flight. The nested tests will follow the business logic and lead to writing
clearer code, as you will be able to follow the testing process more easily.
 Following this tight-coupling idea, nested tests give the test writer more capabilities
to express the relationships among several groups of tests. Inner classes may be
package-private.
 The Tested Data Systems company works with customers. Each customer has a gender, a first name, a last name, sometimes a middle name, and the date when they
became a customer (if known). Some parameters may not be present, so the engineers are using the builder pattern to create and test a customer.
 The following listing demonstrates the use of the @Nested annotation on the class
NestedTestsTest. The customer being tested is John Michael Smith, and the date
when he became a customer is known.
public class NestedTestsTest {
 private static final String FIRST_NAME = "John";
 private static final String LAST_NAME = "Smith";
 @Nested
 class BuilderTest {
 private String MIDDLE_NAME = "Michael";
 @Test
 void customerBuilder() throws ParseException {
 SimpleDateFormat simpleDateFormat =
 new SimpleDateFormat("MM-dd-yyyy");
 Date customerDate = simpleDateFormat.parse("04-21-2019");
 Customer customer = new Customer.Builder(
 Gender.MALE, FIRST_NAME, LAST_NAME)
 .withMiddleName(MIDDLE_NAME)
 .withBecomeCustomer(customerDate)
 .build();
 assertAll(() -> {
 assertEquals(Gender.MALE, customer.getGender());
 assertEquals(FIRST_NAME, customer.getFirstName());
 assertEquals(LAST_NAME, customer.getLastName());
 assertEquals(MIDDLE_NAME, customer.getMiddleName());
 assertEquals(customerDate,
 customer.getBecomeCustomer());
 });
 }
 }
}
The main test is NestedTestsTest B, and it makes sense here that it is tightly coupled with the nested test BuilderTest d. First, NestedTestsTest defines the first
Listing 2.7 Nested tests
c b
d
e
f
g
Tagged tests 23
name and last name of a customer that will be used for all nested tests c. The nested
test, BuilderTest, verifies the construction of a Customer object e with the help of
the builder pattern f. The equality of the fields is verified at the end of the
customerBuilder test g.
 The source code file has another nested class, CustomerHashCodeTest, containing two more tests. You can follow along with it.
2.3 Tagged tests
If you are familiar with JUnit 4, tagged tests are replacements for JUnit 4 categories. You
can use the @Tag annotation over classes and test methods. Later, you can use tags to
filter test discovery and execution.
 Listing 2.8 presents the CustomerTest tagged class, which tests the correct creation of Tested Data Systems customers. Listing 2.9 presents the CustomersRepositoryTest tagged class, which tests the existence and nonexistence of
customers inside a repository. One use case may be to group your tests into a few categories based on the business logic and the things you are effectively testing. (Each test
category has its own tag.) Then you may decide to run only some tests or alternate
among categories, depending on your current needs.
@Tag("individual")
public class CustomerTest {
 private String CUSTOMER_NAME = "John Smith";
 @Test
 void testCustomer() {
 Customer customer = new Customer(CUSTOMER_NAME);
 assertEquals("John Smith", customer.getName());
 }
}
The @Tag annotation is added to the entire CustomerTest class B.
@Tag("repository")
public class CustomersRepositoryTest {
 private String CUSTOMER_NAME = "John Smith";
 private CustomersRepository repository = new CustomersRepository();
 @Test
 void testNonExistence() {
 boolean exists = repository.contains("John Smith");
 assertFalse(exists);
 }
Listing 2.8 CustomerTest tagged class
Listing 2.9 CustomersRepositoryTest tagged class
b
b
24 CHAPTER 2 Exploring core JUnit
 @Test
 void testCustomerPersistence() {
 repository.persist(new Customer(CUSTOMER_NAME));
 assertTrue(repository.contains("John Smith"));
 }
}
Similarly, the @Tag annotation is added to the entire CustomersRepositoryTest
class B. Here is the Maven configuration file for these tests.
<plugin>
 <artifactId>maven-surefire-plugin</artifactId>
 <version>2.22.2</version>
 <!--
 <configuration>
 <groups>individual</groups>
 <excludedGroups>repository</excludedGroups>
 </configuration>
 -->
</plugin>
To activate the tags, you have a few alternatives. One is to work at the level of the
pom.xml configuration file. In this example, it’s enough to uncomment the configuration node of the Surefire plugin B and run mvn clean install.
 Another alternative in the IntelliJ IDEA is to activate the tags by creating a configuration by choosing Run > Run > Edit Configurations > Tags (JUnit 5) as the test kind
(figure 2.2). This is fine when you would like to quickly make some changes about
which tests to run locally. However, it is strongly recommended that you make the
changes at the level of the pom.xml file—otherwise, any automated build of the project will fail.
Figure 2.2 Configuring the tagged tests from the IntelliJ IDEA
Listing 2.10 pom.xml configuration file
b



2.4 Assertions

To perform test validation, you use the assert methods provided by the JUnit
Assertions class.

assertAll Overloaded method. It asserts that none of the supplied executables throw exceptions. An executable is an object of type
org.junit.jupiter.api.function.Executable.
assertArrayEquals Overloaded method. It asserts that the expected array and
the actual array are equal.
assertEquals Overloaded method. It asserts that the expected values and
the actual values are equal.
assertX(..., String message) Assertion that delivers the supplied message to the test
framework if the assertion fails.
assertX(..., Supplier<String>
messageSupplier)
Assertion that delivers the supplied message to the test
framework if the assertion fails. The failure message is
retrieved lazily from the supplied messageSupplier. 

Lambda expressions treat functionality as a method argument and code as data. We
can pass around a lambda expression as if it were an object and execute it on demand.
 This section presents a few examples provided by the assertions package. Listing 2.11 shows some of the overloaded assertAll methods. The heading parameter
allows us to recognize the group of assertions within the assertAll() methods. The
failure message of the assertAll() method can provide detailed information about
every particular assertion within a group. Also, we’re using the @DisplayName annotation to provide easy-to-understand information about what the test is looking for.
Our purpose is the verification of the same system under test (SUT) class that we
introduced earlier.
After the heading parameter from the assertAll method, we provide the rest of
the arguments as a collection of executables—a shorter, more convenient way to assert
that supplied executables do not throw exceptions.

class AssertAllTest {
 @Test
 @DisplayName(
 "SUT should default to not being under current verification")
 void testSystemNotVerified() {
 SUT systemUnderTest = new SUT("Our system under test");
 assertAll("By default,
 SUT is not under current verification",
 () -> assertEquals("Our system under test",
 systemUnderTest.getSystemName()),
 () -> assertFalse(systemUnderTest.isVerified())
 );
 }
 @Test
 @DisplayName("SUT should be under current verification")
 void testSystemUnderVerification() {
 SUT systemUnderTest = new SUT("Our system under test");
 systemUnderTest.verify();
 assertAll("SUT under current verification",
 () -> assertEquals("Our system under test",
 systemUnderTest.getSystemName()),
 () -> assertTrue(systemUnderTest.isVerified())
 );
 }
}
The assertAll method will always check all the assertions that are provided to it, even
if some of them fail—if any of the executables fail, the remaining ones will still be run.
That is not true for the JUnit 4 approach: if you have a few assert methods, one under
the other, and one of them fails, that failure will stop the execution of the others.
Listing 2.11 assertAll method
b
c
d
e
f
g
Assertions 27
 In the first test, the assertAll method receives as a parameter the message to be
displayed if one of the supplied executables throws an exception B. Then the
method receives one executable to be verified with assertEquals c and one to be
verified with assertFalse d. The assertion conditions are brief so that they can be
read at a glance.
 In the second test, the assertAll method receives as a parameter the message to
be displayed if one of the supplied executables throws an exception e. Then it
receives one executable to be verified with assertEquals f and one to be verified
with assertTrue g. Just like in the first test, the assertion conditions are easy to
read.
 The next listing shows the use of some assertion methods with messages. Thanks to
Supplier<String>, the instructions required to create a complex message aren’t
provided in the case of success. We can use lambda or method references to verify our
SUT; they improve performance.
...
@Test
@DisplayName("SUT should be under current verification")
void testSystemUnderVerification() {
 systemUnderTest.verify();
 assertTrue(systemUnderTest.isVerified(),
 () -> "System should be under verification");
}
@Test
@DisplayName("SUT should not be under current verification")
void testSystemNotUnderVerification() {
 assertFalse(systemUnderTest.isVerified(),
 () -> "System should not be under verification.");
}
@Test
@DisplayName("SUT should have no current job")
void testNoJob() {
 assertNull(systemUnderTest.getCurrentJob(),
 () -> "There should be no current job");
}
...
In this example:
 A condition is verified with the help of the assertTrue method B. In case of
failure, a message is lazily created c.
 A condition is verified with the help of the assertFalse method d. In case of
failure, a message is lazily created e.
 The existence of an object is verified with the help of the assertNull method
f. In case of failure, a message is lazily created g.
Listing 2.12 Some assertion methods with messages
b
c
d
e
f
g

The advantage of using lambda expressions as arguments for assertion methods is that
all of them are lazily created, resulting in improved performance. If the condition at B
is fulfilled, meaning the test succeeded, the invocation of the lambda expression at c
does not take place, which would be impossible if the test were written in the old style.
 There may be situations in which you expect a test to be executed within a given
interval. In our example, it is natural for the user to expect that the system under test
will run the given jobs quickly. JUnit 5 offers an elegant solution for this kind of use
case.
 The following listing shows the use of some assertTimeout and assertTimeoutPreemptively methods, which replace the JUnit 4 Timeout rule. The methods need to check whether the SUT is performant enough, meaning it is executing its
jobs within a given timeout.
class AssertTimeoutTest {
 private SUT systemUnderTest = new SUT("Our system under test");
 @Test
 @DisplayName("A job is executed within a timeout")
 void testTimeout() throws InterruptedException {
 systemUnderTest.addJob(new Job("Job 1"));
 assertTimeout(ofMillis(500), () -> systemUnderTest.run(200));
 }
 @Test
 @DisplayName("A job is executed preemptively within a timeout")
 void testTimeoutPreemptively() throws InterruptedException {
 systemUnderTest.addJob(new Job("Job 1"));
 assertTimeoutPreemptively(ofMillis(500),
 () -> systemUnderTest.run(200));
 }
}
assertTimeout waits until the executable finishes B. The failure message looks
something like this: execution exceeded timeout of 500 ms by 193 ms.
assertTimeoutPreemptively stops the executable when the time has expired
c. The failure message looks like this: execution timed out after 500 ms.
 In some situations, you expect a test to be executed and to throw an exception, so
you may force the rest to run under inappropriate conditions or to receive inappropriate input. In our example, it is natural that the SUT that tries to run without a job
assigned to it will throw an exception. Again, JUnit 5 offers an elegant solution.
 Listing 2.14 shows the use of some assertThrows methods, which replace the
JUnit 4 ExpectedException rule and the expected attribute of the @Test annotation. All assertions can be made against the returned instance of Throwable. This
makes the tests more readable, as we are verifying that the SUT is throwing exceptions: a current job is expected but not found.
Listing 2.13 Some assertTimeout methods
b
c
Assumptions 29
class AssertThrowsTest {
 private SUT systemUnderTest = new SUT("Our system under test");
 @Test
 @DisplayName("An exception is expected")
 void testExpectedException() {
 assertThrows(NoJobException.class, systemUnderTest::run);
 }
 @Test
 @DisplayName("An exception is caught")
 void testCatchException() {
 Throwable throwable = assertThrows(NoJobException.class,
 () -> systemUnderTest.run(1000));
 assertEquals("No jobs on the execution list!",
 throwable.getMessage());
 }
}
In this example:
 We verify that the systemUnderTest object’s call of the run method throws a
NoJobException B.
 We verify that a call to systemUnderTest.run(1000) throws a NoJobException, and we keep a reference to the thrown exception in the throwable variable c.
 We check the message kept in the throwable exception variable d.
2.5 Assumptions
Sometimes tests fail due to an external environment configuration or a date or time
zone issue that we cannot control. We can prevent our tests from being executed
under inappropriate conditions.
Assumptions verify the fulfillment of preconditions that are essential for running
the tests. You can use assumptions when it does not make sense to continue the execution of a given test method. In the test report, these tests are marked as aborted.
 JUnit 5 comes with a set of assumption methods suitable for use with Java 8 lambdas. The JUnit 5 assumptions are static methods belonging to the org.junit
.jupiter.api.Assumptions class. The message parameter is in the last position.
 JUnit 4 users should be aware that not all previously existing assumptions are provided in JUnit 5. There is no assumeThat() method, which we may regard as confirmation that matchers are no longer part of JUnit. The new assumingThat() method
executes an assertion only if the assumption is fulfilled.
 Suppose we have a test that needs to run only in the Windows OS and in the Java 8
version. These preconditions are turned into JUnit 5 assumptions. A test is executed
only if the assumptions are true. The following listing shows the use of some assumption
Listing 2.14 Some assertThrows methods
b
c
d
30 CHAPTER 2 Exploring core JUnit
methods and verifies our SUT only under the environmental conditions we imposed:
the OS needs to be Windows, and the Java version needs to be 8. If these conditions
(assumptions) are not fulfilled, the check is not made.
class AssumptionsTest {
 private static String EXPECTED_JAVA_VERSION = "1.8";
 private TestsEnvironment environment = new TestsEnvironment(
 new JavaSpecification(
 System.getProperty("java.vm.specification.version")),
 new OperationSystem(
 System.getProperty("os.name"),
 System.getProperty("os.arch"))
 );
 private SUT systemUnderTest = new SUT();
 @BeforeEach
 void setUp() {
 assumeTrue(environment.isWindows());
 }
 @Test
 void testNoJobToRun() {
 assumingThat(
 () -> environment.getJavaVersion()
 .equals(EXPECTED_JAVA_VERSION),
 () -> assertFalse(systemUnderTest.hasJobToRun()));
 }
 @Test
 void testJobToRun() {
 assumeTrue(environment.isAmd64Architecture());
 systemUnderTest.run(new Job());
 assertTrue(systemUnderTest.hasJobToRun());
 }
}
In this example:
 The @BeforeEach annotated method is executed before each test. The test
will not run unless the assumption that the current environment is Windows is
true B.
 The first test checks that the current Java version is the expected one c. Only if
this assumption is true does it verify that no job is currently being run by the
SUT d.
 The second test checks the current environment architecture e. Only if this
architecture is the expected one does it run a new job on the SUT f and verify
that the system has a job to run g.
Listing 2.15 Some assumption methods
b
c
d
e
f
g



2.6 Dependency injection in JUnit 5

Test constructors and methods can have parameters that are resolved using dependency injection.

The ParameterResolver interface dynamically resolves parameters at runtime.

A parameter of a constructor or method must be resolved at runtime by a registered
ParameterResolver.

You can inject as many parameters as you want, in any order.

JUnit 5 now has three built-in resolvers. You must explicitly enable other parameter resolvers by registering appropriate extensions via @ExtendWith. The parameter
resolvers that are automatically registered are discussed in the following sections.
2.6.1 TestInfoParameterResolver
If a constructor or method parameter is of type TestInfo, TestInfoParameterResolver supplies an instance of this type. TestInfo is a class whose objects are used
to inject information about the currently executed test or container into the @Test,
@BeforeEach, @AfterEach, @BeforeAll, and @AfterAll methods. Then TestInfo gets information about the current test: the display name, test class or method,
and associated tags. The display name can be the name of the test class or test method
or a custom name provided with the help of @DisplayName. Here’s how to use a TestInfo parameter as an argument of a constructor and annotated methods.
class TestInfoTest {
 TestInfoTest(TestInfo testInfo) {
 assertEquals("TestInfoTest", testInfo.getDisplayName());
 }
 @BeforeEach
 void setUp(TestInfo testInfo) {
 String displayName = testInfo.getDisplayName();
 assertTrue(displayName.equals("display name of the method") ||
 displayName.equals(
 "testGetNameOfTheMethod(TestInfo)"));
 }
 @Test
 void testGetNameOfTheMethod(TestInfo testInfo) {
 assertEquals("testGetNameOfTheMethod(TestInfo)",
 testInfo.getDisplayName());
 }
 @Test
 @DisplayName("display name of the method")
 void testGetNameOfTheMethodWithDisplayNameAnnotation(TestInfo testInfo) {
 assertEquals("display name of the method",
 testInfo.getDisplayName());
 }
}
Listing 2.16 TestInfo parameters
b
c
d
e
32 CHAPTER 2 Exploring core JUnit
In this example:
 A TestInfo parameter is injected into the constructor and into three methods.
The constructor verifies that the display name is TestInfoTest: its own name
B. This is the default behavior, which we can vary using @DisplayName annotations.
 The @BeforeEach annotated method is executed before each test. It has an
injected TestInfo parameter, and it verifies that the displayed name is the
expected one: the name of the method or the name specified by the @DisplayName annotation c.
 Both tests have an injected TestInfo parameter. Each parameter verifies that
the displayed name is the expected one: the name of the method in the first test
d or the name specified by the @DisplayName annotation in the second test e.
 The built-in TestInfoParameterResolver supplies an instance of TestInfo
that corresponds to the current container or test as the value of the expected
parameters of the constructor and of the methods.
2.6.2 TestReporterParameterResolver
If a constructor or method parameter is of type TestReporter, TestReporterParameterResolver supplies an instance of this type. TestReporter is a functional interface and therefore can be used as the assignment target for a lambda
expression or method reference. TestReporter has a single publishEntry abstract
method and several overloaded publishEntry default methods. Parameters of type
TestReporter can be injected into methods of test classes annotated with
@BeforeEach, @AfterEach, and @Test. TestReporter can also be used to provide
additional information about the test that is run. Here’s how to use a TestReporter
parameter as an argument of @Test annotated methods.
class TestReporterTest {
 @Test
 void testReportSingleValue(TestReporter testReporter) {
 testReporter.publishEntry("Single value");
 }
 @Test
 void testReportKeyValuePair(TestReporter testReporter) {
 testReporter.publishEntry("Key", "Value");
 }
 @Test
 void testReportMultipleKeyValuePairs(TestReporter testReporter) {
 Map<String, String> values = new HashMap<>();
 values.put("user", "John");
 values.put("password", "secret");
Listing 2.17 TestReporter parameters
b
c
d
e
Repeated tests 33
 testReporter.publishEntry(values);
 }
}
In this example, a TestReporter parameter is injected into three methods:
 In the first method, it is used to publish a single value entry B.
 In the second method, it is used to publish a key-value pair c.
 In the third method, we construct a map d, populate it with two key-value pairs
e, and then use it to publish the constructed map f.
 The built-in TestReporterParameterResolver supplies the instance of
TestReporter needed to publish the entries.
The result of the execution of this test is shown in figure 2.3.
Figure 2.3 The result of executing TestReporterTest
2.6.3 RepetitionInfoParameterResolver
If a parameter in a method annotated with @RepeatedTest, @BeforeEach, or
@AfterEach is of type RepetitionInfo, RepetitionInfoParameterResolver
supplies an instance of this type. Then RepetitionInfo gets information about the
current repetition and the total number of repetitions for a test annotated with
@RepeatedTest. Repeated tests and examples are discussed in the following section.
2.7 Repeated tests
JUnit 5 allows us to repeat a test a specified number of times using the @RepeatedTest annotation, which has as a parameter the required number of repetitions. This
feature can be particularly useful when conditions may change from one execution of
a test to another. For example, some data that affects success may have changed
between two executions of the same test, and an unexpected change in this data
would be a bug that needs to be fixed.
 A custom display name can be configured for each repetition using the name attribute of the @RepeatedTest annotation. The following placeholders are now supported:
 {displayName}—Display name of the method annotated with @RepeatedTest
f
34 CHAPTER 2 Exploring core JUnit
 {currentRepetition}—Current repetition number
 {totalRepetitions}—Total number of repetitions
Listing 2.18 shows the use of repeated tests, display name placeholders, and
RepetitionInfo parameters. The first repeated test verifies that the execution of
the add method from the Calculator class is stable and always provides the same
result. The second repeated test verifies that collections follow the appropriate behavior: a list receives a new element at each iteration, and a set does not get duplicate elements even if we try to insert such an element multiple times.
public class RepeatedTestsTest {
 private static Set<Integer> integerSet = new HashSet<>();
 private static List<Integer> integerList = new ArrayList<>();
 @RepeatedTest(value = 5, name =
"{displayName} - repetition {currentRepetition}/{totalRepetitions}")
 @DisplayName("Test add operation")
 void addNumber() {
 Calculator calculator = new Calculator();
 assertEquals(2, calculator.add(1, 1),
 "1 + 1 should equal 2");
 }
 @RepeatedTest(value = 5, name = "the list contains
{currentRepetition} elements(s), the set contains 1 element")
 void testAddingToCollections(TestReporter testReporter,
 RepetitionInfo repetitionInfo) {
 integerSet.add(1);
 integerList.add(repetitionInfo.getCurrentRepetition());
 testReporter.publishEntry("Repetition number",
 String.valueOf(repetitionInfo.getCurrentRepetition()));
 assertEquals(1, integerSet.size());
 assertEquals(repetitionInfo.getCurrentRepetition(),
 integerList.size());
 }
}
In this example:
 The first test is repeated five times. Each repetition shows the display name, the
current repetition number, and the total number of repetitions B.
 The second test is repeated five times. Each repetition shows the number of elements in the list (the current repetition number) and checks whether the set
always has only one element c.
 Each time the second test is repeated, the repetition number is displayed as it is
injected into the RepetitionInfo parameter d.
Listing 2.18 Repeated tests
b
c
d
Parameterized tests 35
The results of executing these tests are shown in figures 2.4 and 2.5. Each invocation
of a repeated test behaves like the execution of a regular @Test method with full support for life cycle callbacks and extensions. That is why the list and the set in the example are declared as static.
Figure 2.4 The names of the repeated tests at the time of execution
Figure 2.5 The messages shown on the console by the second repeated test
2.8 Parameterized tests
Parameterized tests allow a test to run multiple times with different arguments. The
great benefit is that we can write a single test to be performed using arguments that
check various input data. The methods are annotated with @ParameterizedTest.
We must declare at least one source providing the arguments for each invocation. The
arguments are then passed to the test method.
@ValueSource lets us specify a single array of literal values. At execution, this
array provides a single argument for each invocation of the parameterized test. The
following test checks the number of words in some phrases that are provided as
parameters.
36 CHAPTER 2 Exploring core JUnit
class ParameterizedWithValueSourceTest {
 private WordCounter wordCounter = new WordCounter();
 @ParameterizedTest
 @ValueSource(strings = {"Check three parameters",
 "JUnit in Action"})
 void testWordsInSentence(String sentence) {
 assertEquals(3, wordCounter.countWords(sentence));
 }
}
In this example:
 We mark the test as being parameterized by using the corresponding annotation B.
 We specify the values to be passed as an argument of the testing method c.
The testing method is executed twice: once for each argument provided by the
@ValueSource annotation.
@EnumSource enables us to use enum instances. The annotation provides an optional
names parameter that lets us specify which instances must be used or excluded. By
default, all instances of an enum are used.
 The following listing shows the use of the @EnumSource annotation to check the
number of words in some phrases that are provided as enum instances.
class ParameterizedWithEnumSourceTest {
 private WordCounter wordCounter = new WordCounter();
 @ParameterizedTest
 @EnumSource(Sentences.class)
 void testWordsInSentence(Sentences sentence) {
 assertEquals(3, wordCounter.countWords(sentence.value()));
 }
 @ParameterizedTest
 @EnumSource(value=Sentences.class,
 names = { "JUNIT_IN_ACTION", "THREE_PARAMETERS" })
 void testSelectedWordsInSentence(Sentences sentence) {
 assertEquals(3, wordCounter.countWords(sentence.value()));
 }
 @ParameterizedTest #3
 @EnumSource(value=Sentences.class, mode = EXCLUDE, names =
 { "THREE_PARAMETERS" })
 void testExcludedWordsInSentence(Sentences sentence) {
 assertEquals(3, wordCounter.countWords(sentence.value()));
 }
Listing 2.19 @ValueSource annotation
Listing 2.20 @EnumSource annotation
b
c
b
c
d
Parameterized tests 37
 enum Sentences {
 JUNIT_IN_ACTION("JUnit in Action"),
 SOME_PARAMETERS("Check some parameters"),
 THREE_PARAMETERS("Check three parameters");
 private final String sentence;
 Sentences(String sentence) {
 this.sentence = sentence;
 }
 public String value() {
 return sentence;
 }
 }
}
This example has three tests, which work as follows:
 The first test is annotated as being parameterized. Then we specify the enum
source as the entire Sentences.class B. So this test is executed three times,
once for each instance of the Sentences enum: JUNIT_IN_ACTION,
SOME_PARAMETERS, and THREE_PARAMETERS.
 The second test is annotated as being parameterized. Then we specify the enum
source as Sentences.class, but we restrict the instances to be passed to the
test to JUNIT_IN_ACTION and THREE_PARAMETERS c. So, this test will be executed twice.
 The third test is annotated as being parameterized. Then we specify the enum
source as Sentences.class, but we exclude the THREE_PARAMETERS
instance d. So, this test is executed twice: for JUNIT_IN_ACTION and
SOME_PARAMETERS.
We can use @CsvSource to express argument lists as comma-separated values (CSV),
such as String literals. The following test uses the @CsvSource annotation to check
the number of words in some phrases that are provided as parameters—this time, in
CSV format.
class ParameterizedWithCsvSourceTest {
 private WordCounter wordCounter = new WordCounter();
 @ParameterizedTest
 @CsvSource({"2, Unit testing", "3, JUnit in Action",
 "4, Write solid Java code"})
 void testWordsInSentence(int expected, String sentence) {
 assertEquals(expected, wordCounter.countWords(sentence));
 }
}
Listing 2.21 @CsvSource annotation
b
c
38 CHAPTER 2 Exploring core JUnit
This example has one parameterized test, which functions as follows:
 The test is parameterized, as indicated by the appropriate annotation B.
 The parameters passed to the test are from the parsed CSV strings listed in the
@CsvSource annotation c. So, this test is executed three times: once for each
CSV line.
 Each CSV line is parsed. The first value is assigned to the expected parameter,
and the second value is assigned to the sentence parameter.
@CsvFileSource allows us to use CSV files from the classpath. The parameterized
test is executed once for each line of a CSV file. Listing 2.22 shows the use of the
@CsvFileSource annotation, and listing 2.23 displays the contents of the
word_counter.csv file on the classpath. The Maven build tool automatically adds the
src/test/resources folder to the classpath. The test checks the number of words in
some phrases that are provided as parameters—this time, in CSV format with a CSV
file as resource input.
class ParameterizedWithCsvFileSourceTest {
 private WordCounter wordCounter = new WordCounter();
 @ParameterizedTest
 @CsvFileSource(resources = "/word_counter.csv")
 void testWordsInSentence(int expected, String sentence) {
 assertEquals(expected, wordCounter.countWords(sentence));
 }
}
2, Unit testing
3, JUnit in Action
4, Write solid Java code
This example has one parameterized test that receives as parameters the lines indicated
in the @CsvFileSource annotation B. So, this test is executed three times: once for
each CSV file line. The CSV file line is parsed, the first value is assigned to the expected
parameter, and then the second value is assigned to the sentence parameter.
2.9 Dynamic tests
JUnit 5 introduces a dynamic new programming model that can generate tests at runtime. We write a factory method, and at runtime, it creates a series of tests to be executed. Such a factory method must be annotated with @TestFactory.
 A @TestFactory method is not a regular test but a factory that generates tests. A
method annotated as @TestFactory must return one of the following:
 A DynamicNode (an abstract class; DynamicContainer and DynamicTest are
the instantiable concrete classes)
Listing 2.22 @CsvFileSource annotation
Listing 2.23 Contents of the word_counter.csv file
b
Dynamic tests 39
 An array of DynamicNode objects
 A Stream of DynamicNode objects
 A Collection of DynamicNode objects
 An Iterable of DynamicNode objects
 An Iterator of DynamicNode objects
As with the requirements for @Test-annotated methods, @TestFactory-annotated
methods are allowed to be package-private as a minimum requirement for visibility,
but they cannot be private or static. They may also declare parameters to be resolved
by a ParameterResolver.
 A DynamicTest is a test case generated at runtime, composed of a display name
and an Executable. Because the Executable is a Java 8 functional interface, the
implementation of a dynamic test can be provided as a lambda expression or as a
method reference.
 A dynamic test has a different life cycle than a standard test annotated with @Test.
The methods annotated with @BeforeEach and @AfterEach are executed for the
@TestFactory method but not for each dynamic test; other than these methods,
there are no life cycle callbacks for individual dynamic tests. The behavior of
@BeforeAll and @AfterAll remains the same; they are executed before all tests
and at the end of all tests.
 Listing 2.24 demonstrates dynamic tests. We want to check a predicate against a
numerical value. To do so, we use a single factory to generate three tests to be created
at runtime: one for a negative value, one for zero, and one for a positive value. We
write one method but get three tests dynamically.
class DynamicTestsTest {
 private PositiveNumberPredicate predicate = new
PositiveNumberPredicate();
 @BeforeAll
 static void setUpClass() {
 System.out.println("@BeforeAll method");
 }
 @AfterAll
 static void tearDownClass() {
 System.out.println("@AfterAll method");
 }
 @BeforeEach
 void setUp() {
 System.out.println("@BeforeEach method");
 }
 @AfterEach
 void tearDown() {
Listing 2.24 Dynamic tests
b
c
d
e
40 CHAPTER 2 Exploring core JUnit
 System.out.println("@AfterEach method");
 }
 @TestFactory
 Iterator<DynamicTest> positiveNumberPredicateTestCases() {
 return asList(
 dynamicTest("negative number",
 () -> assertFalse(predicate.check(-1))),
 dynamicTest("zero",
 () -> assertFalse(predicate.check(0))),
 dynamicTest("positive number",
 () -> assertTrue(predicate.check(1)))
 ).iterator();
 }
}
In this example:
 The methods annotated with @BeforeAll B and @AfterAll c are executed
once, as expected: at the beginning and at the end of the entire tests list,
respectively.
 The methods annotated with @BeforeEach d and @AfterEach e are executed before and after the execution of the @TestFactory-annotated method,
respectively f.
 This factory method generates three test methods labeled "negative number" g, "zero" h, and "positive number" i.
 The effective behavior of each test is given by the Executable provided as the
second parameter of the dynamicTest method.
The result of executing these tests is shown in figure 2.6.
2.10 Using Hamcrest matchers
Statistics show that people easily become hooked on the unit-testing philosophy.
When we get accustomed to writing tests and see how good it feels to be protected
from possible mistakes, we wonder how it was possible to live without unit testing.
 As we write more unit tests and assertions, we inevitably find that some assertions
are big and hard to read. Our example company, Tested Data Systems, is working with
customers whose data may be kept in lists. Engineers may populate a list with values
like "Michael", "John", and "Edwin"; then they may search for customers like
f
g
h
i
Figure 2.6 The result of
executing dynamic tests
Using Hamcrest matchers 41
"Oliver", "Jack", and "Harry", as in the following listing. This test is intended to
fail and show the description of the assertion failure.
[...]
public class HamcrestListTest {
 private List<String> values;
 @BeforeEach
 public void setUp () {
 values = new ArrayList< >();
 values.add("Michael");
 values.add("John");
 values.add("Edwin");
 }

 @Test
 @DisplayName("List without Hamcrest")
 public void testWithoutHamcrest() {
 assertEquals(3, values.size());
 assertTrue(values.contains("Oliver")
 || values.contains("Jack")
 || values.contains("Harry"));
 }
 }
This example constructs a simple JUnit test like those described earlier in this chapter:
 A @BeforeEach fixture B initializes some data for the test. A single test
method is used c.
 This test method makes a long, hard-to-read assertion d. (Maybe the assertion
itself is not too hard to read, but what it does definitely is not obvious at first
glance.)
 The goal is to simplify the assertion made in the test method.
To solve this problem, Tested Data Systems uses a library of matchers for building test
expressions: Hamcrest. Hamcrest (https://code.google.com/archive/p/hamcrest) is a
library that contains a lot of helpful Matcher objects (also known as constraints or predicates) ported in several languages, such as Java, C++, Objective-C, Python, and PHP.
The next listing provides the same test method as listing 2.25, this time written with
the Hamcrest library.
Listing 2.25 Cumbersome JUnit assert method
The Hamcrest library
Hamcrest is not a testing framework itself, but it helps us declaratively specify simple
matching rules. These matching rules can be used in many situations, but they are
particularly helpful for unit testing.
b
c
d
42 CHAPTER 2 Exploring core JUnit
[...]
import static org.hamcrest.CoreMatchers.anyOf;
import static org.hamcrest.CoreMatchers.equalTo;
import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.*;
[...]
 @DisplayName("List with Hamcrest")
 public void testListWithHamcrest() {
 assertThat(values, hasSize(3));
 assertThat(values, hasItem(anyOf(equalTo("Oliver"),
 equalTo("Jack"), equalTo("Harry"))));
 }
[...]
This example adds a test method that imports the needed matchers and the assertThat method B and then constructs a test method. The test method uses one of the
most powerful features of matchers: they can nest c. What Hamcrest gives us, and
standard assertions do not, is a human-readable description of an assertion failure.
Using assertion code with or without Hamcrest matchers is a personal preference.
 The examples in the previous two listings construct a List with the customers
"Michael", "John", and "Edwin" as elements. After that, the code asserts the presence of "Oliver", "Jack", or "Harry", so the tests will fail on purpose. The result
from the execution without Hamcrest is shown in figure 2.7, and the result from the
execution with Hamcrest is shown in figure 2.8. As the figures show, the test that uses
Hamcrest provides more details.
Figure 2.7 The result of the test execution without Hamcrest
Figure 2.8 The result of the test execution with Hamcrest
Listing 2.26 Using the Hamcrest library
b
c
Using Hamcrest matchers 43
To use Hamcrest in our projects, we need to add the required dependency to the
pom.xml file.
<dependency>
 <groupId>org.hamcrest</groupId>
 <artifactId>hamcrest-library</artifactId>
 <version>2.1</version>
 <scope>test</scope>
</dependency>
To use Hamcrest in JUnit 4, you have to use the assertThat method from the
org.junit.Assert class. But as explained earlier in this chapter, JUnit 5 removes
the assertThat() method. The user guide justifies the decision this way:
[...] org.junit.jupiter.api.Assertions class does not provide an
assertThat() method like the one found in JUnit 4’s org.junit.Assert class,
which accepts a Hamcrest Matcher. Instead, developers are encouraged to use the built-in
support for matchers provided by third-party assertion libraries.
This text means that if we want to use Hamcrest matchers, we have to use the
assertThat() methods of the org.hamcrest.MatcherAssert class. As the previous examples illustrated, the overloaded methods take two or three method
parameters:
 An error message shown when the assertion fails (optional)
 The actual value or object
 A Matcher object for the expected value
To create the Matcher object, we need to use one of the static factory methods provided by the org.hamcrest.Matchers class, as shown in table 2.2.
Listing 2.27 pom.xml Hamcrest dependency
Table 2.2 Sample of common Hamcrest static factory methods
Factory method Logical
anything Matches absolutely anything; useful when we want to make
the assert statement more readable
is Used only to improve the readability of statements
allOf Checks whether all contained matchers match (like the &&
operator)
anyOf Checks whether any of the contained matchers match (like
the || operator)
not Inverts the meaning of the contained matchers (like the !
operator in Java)
instanceOf Check whether objects are instances of one another
44 CHAPTER 2 Exploring core JUnit
All of these methods are pretty straightforward to read and use. Also, remember that
we can compose them into one another.
 For each service provided to customers, Tested Data Systems charges a particular
price. The following code tests the properties of a customer and some service prices
using a few Hamcrest methods.
public class HamcrestMatchersTest {
 private static String FIRST_NAME = "John";
 private static String LAST_NAME = "Smith";
 private static Customer customer = new Customer(FIRST_NAME, LAST_NAME);
 @Test
 @DisplayName("Hamcrest is, anyOf, allOf")
 public void testHamcrestIs() {
 int price1 = 1, price2 = 1, price3 = 2;
 assertThat(1, is(price1));
 assertThat(1, anyOf(is(price2), is(price3)));
 assertThat(1, allOf(is(price1), is(price2)));
 }
 @Test
 @DisplayName("Null expected")
 void testNull() {
 assertThat(null, nullValue());
 }
sameInstance Tests object identity
nullValue, notNullValue Tests for null or non-null values
hasProperty Tests whether a Java Bean has a certain property
hasEntry, hasKey, hasValue Tests whether a given Map has a given entry, key, or value
hasItem, hasItems Tests a given collection for the presence of an item or items
closeTo, greaterThan,
greaterThanOrEqualTo,
lessThan, lessThanOrEqualTo
Tests whether given numbers are close to, greater than, greater
than or equal, less than, or less than or equal to a given value
equalToIgnoringCase Tests whether a given string equals another one, ignoring the
case
equalToIgnoringWhiteSpace Tests whether a given string equals another one, ignoring white
space
containsString, endsWith,
startsWith
Tests whether a given string contains, starts with, or ends with
a certain string
Listing 2.28 A few Hamcrest static factory methods
Table 2.2 Sample of common Hamcrest static factory methods (continued)
Factory method Logical
b
c
Summary 45
 @Test
 @DisplayName("Object expected")
 void testNotNull() {
 assertThat(customer, notNullValue());
 }
 @Test
 @DisplayName("Check correct customer properties")
 void checkCorrectCustomerProperties() {
 assertThat(customer, allOf(
 hasProperty("firstName", is(FIRST_NAME)),
 hasProperty("lastName", is(LAST_NAME))
 ));
 }
}
This example shows
 The use of the is, anyOf, and allOf methods B  The use of the nullValue method c  The use of the notNullValue method d  The use of the assertThat method B, c, d, and e, as described in
table 2.2
We have also constructed a Customer object and check its properties with the help of
the hasProperty method e.
 Last but not least, Hamcrest is extremely extensible. Writing matchers that check a
certain condition is easy: we implement the Matcher interface and an appropriately
named factory method.
 Chapter 3 analyzes the architectures of JUnit 4 and JUnit 5 and explains the move
to the new architecture.
```



# 5. Software testing principles

```java
public class JUnit5ExceptionTester {
 private Calculator calculator = new Calculator();
 @Test
 public void expectIllegalArgumentException() {
 assertThrows(IllegalArgumentException.class,
 () -> calculator.sqrt(-1));
 }
 @Test
 public void expectArithmeticException() {
 assertThrows(ArithmeticException.class,
 () -> calculator.divide(1, 0));
 }
}
```


# 7. Coarse-grained testing with stubs

```java
import java.net.URL;
import java.net.HttpURLConnection;
import java.io.InputStream;
import java.io.IOException;

public class WebClient {
 public String getContent(URL url) {
 StringBuffer content = new StringBuffer();
 try {
 HttpURLConnection connection = (HttpURLConnection)
 url.openConnection();
 connection.setDoInput(true);
 InputStream is = connection.getInputStream();
 byte[] buffer = new byte[2048];
 int count;
 while (-1 != (count = is.read(buffer))) {
 content.append(new String(buffer, 0, count));
 }
 } catch (IOException e) {
 throw new RuntimeException(e);
 }
 return content.toString();
 }
}

To understand how Jetty works, let’s implement and examine an example of setting up and controlling it from the tests. The following listing demonstrates how to
start Jetty from Java and how to define a document root (/) from which to start serving files.
[...]
import org.mortbay.jetty.Server;
import org.mortbay.jetty.handler.ResourceHandler;
import org.mortbay.jetty.servlet.Context;
public class JettySample {
 public static void main(String[] args) throws Exception {
 Server server = new Server(8081);

 Context root = new Context(server, “/”);
 root.setResourceBase(“./pom.xml”);
 root.setHandler(new ResourceHandler());
 server.start();
 }
}
In this listing:
 We start by creating the Jetty Server object B and specifying in the constructor which port to listen to for HTTP requests (port 8081). Be sure to check
that port 8081 is not already in use—you may change it in the source files, if
necessary.
 Next, we create a Context object c that processes the HTTP requests and
passes them to various handlers. We map the context to the already created
server instance, and to the root (/) URL. The setResourceBase method sets
the document root from which to serve resources. On the next line, we attach a
ResourceHandler to the root to serve files from the filesystem.
 Finally, we start the server d.
If you start the program from listing 7.2 and navigate your browser to http://localhost:8081, you should be able to see the content of the pom.xml file (figure 7.4). You
get a similar effect by changing the code to set the base as root.setResourceBase("."), restarting the server, and then navigating to http://localhost:8081/
pom.xml.
 Figure 7.4 demonstrates the results of running the code in listing 7.2 after opening
a browser on http://localhost:8081. Now that you have seen how to run Jetty as an
embedded server, the next section shows you how to stub the server resources.
Listing 7.2 Starting Jetty in embedded mode: JettySample class
b
c
d
130 CHAPTER 7 Coarse-grained testing with stubs
Figure 7.4 Testing the JettySample in a browser. It displays the content of the pom.xml file to
demonstrate how the Jetty web server works.
7.3 Stubbing the web server resources
This section focuses on the HTTP connection unit test. The developers at Tested Data
Systems will write tests to verify that they can call a valid URL and get its content.
These tests are the first steps in checking the functionality of web applications that are
interacting with external customers.
7.3.1 Setting up the first stub test
To verify that the WebClient works with a valid URL, we need to start the Jetty server
before the test, which we can implement in a test case setUp method. We can also
stop the server in a tearDown method.
[...]
import java.net.URL;
import org.junit.jupiter.api.*;
public class TestWebClientSkeleton {
 @BeforeAll
 public static void setUp() {
 // Start Jetty and configure it to return "It works" when
 // the http://localhost:8081/testGetContentOk URL is
 // called.
 }
Listing 7.3 Skeleton of the first test to verify that WebClient works with a valid URL
Stubbing the web server resources 131
 @AfterAll
 public static void tearDown() {
 // Stop Jetty.
 }
 @Test
 @Disabled(value = "This is just the initial skeleton of a test.
 Therefore, if we run it now, it will fail.")
 public void testGetContentOk() throws MalformedURLException {
 WebClient client = new WebClient();
 String workingContent = client.getContent(new URL(
 "http://localhost:8081/testGetContentOk"));
 assertEquals ("It works", workingContent);
 }
}
To implement the @BeforeAll and @AfterAll methods, we have two options. We
can prepare a static page containing the text "It works", which we put in the document root (controlled by the call to root.setResourceBase(String) in listing
7.2). Alternatively, we can configure Jetty to use a custom handler that returns the
string "It works" instead of getting it from a file. This technique is much more powerful because it lets us unit test the case when the remote HTTP server returns an
error code to the WebClient client application.
CREATING A JETTY HANDLER
The next listing creates a Jetty Handler that returns the string "It works".
private static class TestGetContentOkHandler extends AbstractHandler {
 @Override
 public void handle(String target, HttpServletRequest request,
 HttpServletResponse response, int dispatch) throws IOException {

 OutputStream out = response.getOutputStream();
 ByteArrayISO8859Writer writer = new ByteArrayISO8859Writer();
 writer.write("It works");
 writer.flush();
 response.setIntHeader(HttpHeaders.CONTENT_LENGTH, writer.size());
 writer.writeTo(out);
 out.flush();
 }
In this listing:
 The class creates a handler B by extending the Jetty AbstractHandler class
and implementing a single method: handle.
 Jetty calls the handle method to forward an incoming request to our handler.
After that, we use the Jetty ByteArrayISO8859Writer class c to send back
the string "It works", which we write in the HTTP response d.
Listing 7.4 Creating a Jetty Handler that returns "It works"
b
c
d
e
132 CHAPTER 7 Coarse-grained testing with stubs
 The last step is setting the response content length to the length of the string
written to the output stream (required by Jetty) and then sending the response
e.
Now that this handler is written, we can tell Jetty to use it by calling context.setHandler(new TestGetContentOkHandler()).
WRITING THE TEST CLASS
We are almost ready to run the test that is the basis of verifying the functionality of the
web applications interacting with the external customers of Tested Data Systems, as
shown in the following listing.
[...]
import java.net.URL;
[...]
public class TestWebClient {
 private WebClient client = new WebClient();
 @BeforeAll
 public static void setUp() throws Exception() {
 Server server = new Server(8081);

 Context contentOkContext = new Context(server, "/testGetContentOk");
 contentOkContext.setHandler(new TestGetContentOkHandler());
 server.setStopAtShutDown(true);
 server.start();
 }
 @AfterAll
 public static void tearDown() {
 // Empty
 }
 @Test
 public void testGetContentOk() throws Exception {
 String workingContent = client.getContent(new URL(
 "http://localhost:8081/testGetContentOk"));
 assertEquals("It works", workingContent);
 }
 private static class TestGetContentOkHandler extends AbstractHandler {
 //Listing 7.4 here.
 }
}
Listing 7.5 Putting it all together
Stubbing the web server resources 133
The test class has become quite simple. The @BeforeAll setUp method constructs
the Server object the same way as in listing 7.2. Then we have our @Test method.
We leave our @AfterAll method empty intentionally because we programmed the
server to stop at shutdown; the server instance is explicitly stopped when the JVM is
shut down. If you run the test, you see the result in figure 7.5. The test passes.
7.3.2 Reviewing the first stub test
You have been able to fully unit test the getContent method in isolation by stubbing
the web resource. What have you really tested? What kind of test have you achieved?
You have done something quite powerful: unit tested the method and, at the same
time, executed an integration test. In addition, you have tested not only the code logic
but also the connection part that is outside the code (through the Java HttpURLConnection class).
 The drawback of this approach is that it is complicated. At Tested Data Systems it
can take a Jetty novice half a day to learn enough about Jetty to set it up correctly. In
some instances, the novice will have to debug stubs to get them to work correctly.
Keep in mind that the stub must stay simple, not become a full-fledged application
that requires tests and maintenance. If you spend too much time debugging the stubs,
a different solution may be called for.
 In these examples, we need a web server—but another example and stub will be
different and need a different setup. Experience helps, but different cases usually
require different stubbing solutions.
 The example tests the web application developed at Tested Data Systems, which is
nice because it allows us to unit test the code and to perform some integration tests at
the same time. This functionality, however, comes at the cost of complexity. More solutions that are lightweight focus on unit testing the code without performing integration tests. The rationale is that although we need integration tests, they could run in a
separate test suite or as part of functional tests.
Figure 7.5 Result of the
first working test using a
Jetty stub. JUnit 5 starts the
server before the first test,
and the server shuts itself
down after the last test.
134 CHAPTER 7 Coarse-grained testing with stubs
 The next section looks at another solution that still qualifies as stubbing and is simpler in the sense that it does not require stubbing a whole web server. This solution
brings us one step closer to the mock-object strategy, which is described in chapter 8.
7.4 Stubbing the connection
So far, we have stubbed the web server resources. Next, we will stub the HTTP connection instead. Doing so will prevent us from effectively testing the connection, which is
fine because that is not the real goal at this point. We want to test the code in isolation.
Functional or integration tests will test the connection at a later stage.
 When it comes to stubbing the connection without changing the code, we benefit
from the Java URL and HttpURLConnection classes, which let us plug in custom protocol handlers to process any kind of communication protocol. We can have any call
to the HttpURLConnection class redirected to our own class, which will return whatever we need for the test.
7.4.1 Producing a custom URL protocol handler
To implement a custom URL protocol handler, we need to call the URL static method
setURLStreamHandlerFactory and pass it a custom URLStreamHandlerFactory.
The engineers at Tested Data Systems are using this approach to create their stub implementation of the URL stream handler. Whenever the URL openConnection method
is called, the URLStreamHandlerFactory class is called to return a URLStreamHandler. The next listing shows the code that performs this action. The idea is to call
setURLStreamHandlerFactory in the JUnit 5 setUp method.
[...]
import java.net.URL;
import java.net.URLStreamHandlerFactory;
import java.net.URLStreamHandler;
import java.net.URLConnection;
import java.net.MalformedUrlException;
public class TestWebClient1 {
 @BeforeAll
 public static void setUp() {
 URL.setURLStreamHandlerFactory(new StubStreamHandlerFactory());
 }
 private static class StubStreamHandlerFactory implements
 URLStreamHandlerFactory {
 @Override
 public URLStreamHandler createURLStreamHandler(String protocol) {
 return new StubHttpURLStreamHandler();
 }
 }
Listing 7.6 Providing custom stream handler classes for testing
b
c
Stubbing the connection 135
 private static class StubHttpURLStreamHandler
 extends URLStreamHandler {
 @Override
 protected URLConnection openConnection(URL url)
 throws IOException {
 return new StubHttpURLConnection(url);
 }
 }
 @Test
 public void testGetContentOk() throws MalformedURLException {
 WebClient client = new WebClient();
 String workingContent = client.getContent(
 new URL("http://localhost"));
 assertEquals("It works", workingContent);
 }
}
In this listing:
 We start by calling setURLStreamHandlerFactory B with our first stub class,
StubStreamHandlerFactory.
 We use several (inner) classes (c and d) to use the StubHttpURLConnection
class.
 In StubStreamHandlerFactory, we override the createURLStreamHandler
method c, in which we return a new instance of our second private stub class,
StubHttpURLStreamHandler.
 In StubHttpURLStreamHandler, we override one method, openConnection,
to open a connection to the given URL d.
Note that we have not written the StubHttpURLConnection class yet. That class is
the topic of the next section.
7.4.2 Creating a JDK HttpURLConnection stub
The last step is creating a stub implementation of the HttpURLConnection class so
that we can return any value we want for the test. This simple implementation returns
the string "It works" as a stream to the caller.
[...]
import java.net.HttpURLConnection;
import java.net.ProtocolException;
import java.net.URL;
import java.io.InputStream;
import java.io.IOException;
import java.io.ByteArrayInputStream;
public class StubHttpURLConnection extends HttpURLConnection {
 private boolean isInput = true;
 protected StubHttpURLConnection(URL url) {
Listing 7.7 Stubbed HttpURLConnection class
d
136 CHAPTER 7 Coarse-grained testing with stubs
 super(url);
 }
 @Override
 public InputStream getInputStream() throws IOException {
 if (!isInput) {
 throw new ProtocolException(
 "Cannot read from URLConnection"
 + " if doInput=false (call setDoInput(true))");
 }
 ByteArrayInputStream readStream = new ByteArrayInputStream(
 new String("It works").getBytes());
 return readStream;
 }

 @Override
 public void connect() throws IOException {}

 @Override
 public void disconnect() {}

 @Override
 public boolean usingProxy() {
 return false;
 }
}
In this listing:
 HttpURLConnection is an abstract public class that does not implement an
interface, so we extend it and override the methods wanted by the stub.
 In this stub, we provide an implementation for the getInputStream method,
as it is the only method used by our code under test.
 If the code to be tested used more APIs from HttpURLConnection, we would
need to stub these additional methods. This part is where the code would
become more complex; we would need to reproduce the same behavior as the
real HttpURLConnection.
 We test whether setDoInput(false) has been called in the code under test
B. The isInput flag will tell if we use the URL connection for input. Then, a
call to the getInputStream method returns a ProtocolException (the
behavior of HttpURLConnection). Fortunately, in most cases, we need to stub
only a few methods, not the whole API.
7.4.3 Running the test
We will test the getContent method by stubbing the connection to the remote
resource using the TestWebClient1 test, which uses the StubHttpURLConnection.
Figure 7.6 shows the result of the test.
 As you can see, it is much easier to stub the connection than to stub the web
resource. This approach does not provide the same level of testing (does not perform
b
Summary 137
integration tests), but it enables you to write a focused unit test for the WebClient
logic more easily.
 Chapter 8 demonstrates using mock objects, which allows fine-grained unit testing
that is completely generic and (best of all) forces you to write good code. Although
stubs are very useful in some cases, some people consider them to be vestiges of the
past, when the consensus was that tests should be separate activities and should not
modify existing code. The new mock-objects strategy not only allows modification of
code but also favors it. Using mock objects is more than a unit testing strategy: it is a
completely new way of writing code.
Summary
 This chapter has covered the following:
 Analyzing when to use stubs: when you cannot modify an existing complex or
fragile system, when you depend on an uncontrollable environment, to replace
a full-blown external system, or for coarse-grained testing.
 Analyzing when not to use stubs: when you need fine-grained tests to provide
precise messages that underline the exact cause of a failure, or when you would
like to test a small portion of the code in isolation.
 Demonstrating how using a stub helps us unit test code accessing a remote web
server using the Java HttpURLConnection API.
 In particular, implementing a stub for a remote web server by using the open
source Jetty server. The embeddable nature of Jetty lets us concentrate on stubbing only the Jetty HTTP request handler, instead of having to stub the whole
container.
 Implementing a more lightweight solution by stubbing the Java HttpURLConnection class. 




# 20. Test-driven development with JUnit 5

testdriven development (TDD): a technique that can greatly increase development
speed and eliminate much of the debugging nightmare—all with the help of
JUnit 5 and its features. We will point out the main concepts involved in TDD and
apply them in developing a Java application that Tested Data Systems (our example
company) will use to implement the business logic for managing flights and passengers and following a set of policies. Our focus will be on clearly explaining TDD and
proving its benefits by demonstrating how to put it in practice, step by step.
This chapter covers
 Moving a non-TDD application to TDD
 Refactoring a TDD application
 Using TDD to implement new functionality
406 CHAPTER 20 Test-driven development with JUnit 5
20.1 TDD main concepts
Test-driven development is a programming practice that uses a short, repeating development cycle in which requirements are converted into test cases, and then the program is modified to make the tests pass:
1 Write a failing test before writing new code.
2 Write the smallest piece of code that will make the new test pass.
The development of this technique is attributed to the American software engineer
Kent Beck. TDD supports simple designs and inspires safety: it looks for “clean code
that works.”
 This is different from traditional software development, where code may be added
without having to verify that it meets requirements. In a classical approach, developing a program means we write code and then do some testing by observing its behavior. So, the conventional development cycle goes something like this:
[code, test, (repeat)]
TDD uses a surprising variation:
[test, code, (repeat)]
The test drives the design and becomes the first client of the method.
 TDD’s benefits include the following:
 We write code that is driven by clear goals, and we make sure we address exactly
what our application needs to do.
 Introducing new functionality is much faster. On the one hand, tests drive us to
implement code that does what it is supposed to do. On the other hand, tests
will prevent us from introducing bugs into the existing working code.
 Tests act as documentation for the application. We can follow them and understand what problems our code is supposed to solve.
We said that TDD uses this development cycle:
[test, code, (repeat)]
In fact, it looks like this:
[test, code, refactor, (repeat)]
Refactoring is the process of modifying a software system in a way that does not impact
its external behavior but does improve its internal structure. To make sure external
behavior is not affected, we need to rely on the tests.
 When we receive specifications to add new functionality to an application, we have
to first understand them before we can add them to the code. What if we first implement a test that will show us what we have to do, and then think about how to do it?
This is one of the fundamental principles of TDD.
 When we begin working on an application, at the very least, we need to understand the fundamental idea of what the software is supposed to do. But if we want to
The flight-management application 407
check what classes or methods do, our choices are limited: read the documentation
or look for sample code that invokes the functionality. Most programmers prefer to
work with the code. And well-written unit tests do exactly this: they invoke our code
and, consequently, provide a working specification for the code’s functionality. As a
result, TDD effectively helps to build a significant part of the application’s technical
documentation.
20.2 The flight-management application
As we have discussed throughout this book, Tested Data Systems (our example company) is developing a flight-management application for one of its customers. Currently, the application is able to create and set up flights, and add passengers to and
remove them from flights.
 In this chapter, we’ll walk through scenarios that follow the developers’ everyday
work. We’ll start with the non-TDD application, which is supposed to do several things
such as follow company policies for regular and VIP passengers. We need to understand the application and make sure it is really implementing the expected operations. So, we have to cover the existing code with unit tests. Once we’ve done that,
we’ll address another challenge: adding new functionality by first understanding what
needs to be done; next, writing tests that fail; and then, writing the code that fixes the
tests. This work cycle is one of the foundations of TDD.
 John is joining the development of the flight-management application, which is a
Java application built with the help of Maven. The software must maintain a policy
regarding adding passengers to and removing them from flights. Flights may be different types: currently, there are economy and business flights, but other types may be
added later, depending on customer requirements. Both VIP passengers and regular
customers may be added to economy flights, but only VIP passengers may be added to
business flights (figure 20.1).
Add passenger to the flight
Approve request Reject request
Is flight economy?
Is passenger VIP?
NoYes
Approve request
NoYes
Figure 20.1 The business
logic of adding passengers to a
flight: if it is a business flight,
only VIP passengers may be
added to it. Any passenger can
be added to an economy flight.
408 CHAPTER 20 Test-driven development with JUnit 5
There is also a policy for removing passengers from flights: a regular passenger may
be removed from a flight, but a VIP passenger cannot be removed (figure 20.2). As we
can see from these two activity diagrams, the initial business logic focuses on decision
making.
Let’s look at the initial design for this application (figure 20.3). It has a field called
flightType in the Flight class. Its value determines the behavior of the addPassenger and removePassenger methods. The developers need to focus on decision making at the level of the code for these two methods.
The following listing shows the Passenger class.
public class Passenger {
 private String name;
 private boolean vip;
Listing 20.1 Passenger class
Remove passenger from the flight
Approve request
Is passenger VIP?
NoYes
Reject request
Figure 20.2 The business logic of removing passengers
from a flight: only regular passengers may be removed.
Flight
– String flightType
Have (1-> 0..n)
+ boolean addPassenger(Passenger passenger)
+ boolean removePassenger(Passenger passenger)
C
PassengerC Figure 20.3 The class diagram for the
flight-management application: the flight
type is kept as a field in the Flight class.
b
c
The flight-management application 409
 public Passenger(String name, boolean vip) {
 this.name = name;
 this.vip = vip;
 }
 public String getName() {
 return name;
 }
 public boolean isVip() {
 return vip;
 }
}
In this listing:
 The Passenger class contains a name field B together with a getter for it e.
 It also contains a vip field c together with a getter for it f.
 The constructor of the Passenger class initializes the name and vip fields d.
The next listing shows the Flight class.
public class Flight {
 private String id;
 private List<Passenger> passengers = new ArrayList<Passenger>();
 private String flightType;
 public Flight(String id, String flightType) {
 this.id = id;
 this.flightType = flightType;
 }
 public String getId() {
 return id;
 }
 public List<Passenger> getPassengersList() {
 return Collections.unmodifiableList(passengers);
 }
 public String getFlightType() {
 return flightType;
 }
 public boolean addPassenger(Passenger passenger) {
 switch (flightType) {
 case "Economy":
 return passengers.add(passenger);
Listing 20.2 Flight class
d
e
f
b
c
d
e
f
g
h
i
j
410 CHAPTER 20 Test-driven development with JUnit 5
 case "Business":
 if (passenger.isVip()) {
 return passengers.add(passenger);
 }
 return false;
 default:
 throw new RuntimeException("Unknown type: " + flightType);
 }
 }
 public boolean removePassenger(Passenger passenger) {
 switch (flightType) {
 case "Economy":
 if (!passenger.isVip()) {
 return passengers.remove(passenger);
 }
 return false;
 case "Business":
 return false;
 default:
 throw new RuntimeException("Unknown type: " + flightType);
 }
 }
}
In this listing:
 The Flight class contains an identifier B together with a getter for it f, a list
of passengers initialized as an empty list c together with a getter for it g, and
a flight type d together with a getter for it h.
 The constructor of the Flight class initializes the id and the flightType
fields e.
 The addPassenger method checks the flight type i. If it is an economy flight,
any passengers can be added j. If it is a business flight, only VIP passengers
can be added 1). Otherwise (if the flight is neither an economy nor a business
flight), the method will throw an exception, as it cannot handle an unknown
flight type 1!.
 The removePassenger method checks the flight type 1@. If it is an economy
flight, only regular passengers can be removed 1#. If it is a business flight, passengers cannot be removed 1$. Otherwise (if the flight is neither an economy
nor a business flight), the method will throw an exception, as it cannot handle
an unknown flight type 1%.
The application has no tests yet. Instead, the initial developers wrote some code in
which they simply followed the execution and compared it with their expectations.
For example, there is an Airport class, including a main method that acts as a client
of the Flight and Passenger classes and works with the different types of flights and
passengers.
1)
1!
1@
1#
1$
1%
The flight-management application 411
public class Airport {
 public static void main(String[] args) {
 Flight economyFlight = new Flight("1", "Economy");
 Flight businessFlight = new Flight("2", "Business");
 Passenger james = new Passenger("James", true);
 Passenger mike = new Passenger("Mike", false);
 businessFlight.addPassenger(james);
 businessFlight.removePassenger(james);
 businessFlight.addPassenger(mike);
 economyFlight.addPassenger(mike);
 System.out.println("Business flight passengers list:");
 for (Passenger passenger: businessFlight.getPassengersList()) {
 System.out.println(passenger.getName());
 }
 System.out.println("Economy flight passengers list:");
 for (Passenger passenger: economyFlight.getPassengersList()) {
 System.out.println(passenger.getName());
 }
 }
}
In this listing:
 We initialize an economy flight and a business flight B. We also initialize James
as a VIP passenger and Mike as a regular passenger c.
 We try to add James to and remove him from the business flight d, and then we
try to add Mike to and remove him from the business flight e and the economy flight f.
 We print the list of passengers on the business flight g and the economy
flight h.
The result of running this program is shown in figure 20.4. James, a VIP passenger,
has been added to the business flight, and we could not remove him. Mike, a regular
passenger, could not be added to the business flight, but we were able to add him to
the economy flight.
Listing 20.3 Airport class, including the main method
b
c
e d
f
g
h
Figure 20.4 The result of
running the non-TDD flightmanagement application: the
VIP passenger has been added
to the business flight, and the
regular passenger has been
added to the economy flight.
412 CHAPTER 20 Test-driven development with JUnit 5
So far, things are working as expected, following the policies that we previously
defined. John is satisfied with the way the application works, but he needs to develop it
further. To build a reliable application and to be able to easily and safely understand
and implement the business logic, John considers moving the application to the TDD
approach.
20.3 Preparing the flight-management application for TDD
To move the flight-management application to TDD, John first needs to cover the
existing business logic with JUnit 5 tests. He adds the JUnit 5 dependencies we are
already familiar with (junit-jupiter-api and junit-jupiter-engine) to the
Maven pom.xml file.
<dependencies>
 <dependency>
 <groupId>org.junit.jupiter</groupId>
 <artifactId>junit-jupiter-api</artifactId>
 <version>5.6.0</version>
 <scope>test</scope>
 </dependency>
 <dependency>
 <groupId>org.junit.jupiter</groupId>
 <artifactId>junit-jupiter-engine</artifactId>
 <version>5.6.0</version>
 <scope>test</scope>
 </dependency>
</dependencies>
Inspecting the business logic from figures 20.1 and 20.2, John understands that he has
to check the add/remove passenger scenarios by providing tests for two flight types
and two passenger types. So, multiplying two flight types by two passenger types, this
means four tests in total. For each of the tests, he has to verify the possible add and
remove operations.
 John follows the business logic for an economy flight and uses the JUnit 5 nested
test capability, as the tests share similarities between them and can be grouped: tests
for economy flights and tests for business flights.
public class AirportTest {
 @DisplayName("Given there is an economy flight")
 @Nested
 class EconomyFlightTest {
 private Flight economyFlight;
 @BeforeEach
Listing 20.4 JUnit 5 dependencies added to the pom.xml file
Listing 20.5 Testing the business logic for an economy flight
b
c
Preparing the flight-management application for TDD 413
 void setUp() {
 economyFlight = new Flight("1", "Economy");
 }
 @Test
 public void testEconomyFlightRegularPassenger() {
 Passenger mike = new Passenger("Mike", false);
 assertEquals("1", economyFlight.getId());
 assertEquals(true, economyFlight.addPassenger(mike));
 assertEquals(1, economyFlight.getPassengersList().size());
 assertEquals("Mike",
 economyFlight.getPassengersList().get(0).getName());
 assertEquals(true, economyFlight.removePassenger(mike));
 assertEquals(0, economyFlight.getPassengersList().size());
 }
 @Test
 public void testEconomyFlightVipPassenger() {
 Passenger james = new Passenger("James", true);
 assertEquals("1", economyFlight.getId());
 assertEquals(true, economyFlight.addPassenger(james));
 assertEquals(1, economyFlight.getPassengersList().size());
 assertEquals("James",
 economyFlight.getPassengersList().get(0).getName());
 assertEquals(false, economyFlight.removePassenger(james));
 assertEquals(1, economyFlight.getPassengersList().size());
 }
 }
}
In this listing:
 John declares a nested test class EconomyFlightTest and labels it "Given
there is an economy flight" with the help of the @DisplayName annotation B.
 He declares an economy flight and initializes it before the execution of each
test c.
 When testing how the economy flight works with a regular passenger, he creates
Mike as a regular passenger d. Then, he checks the ID of the flight e, whether
he can add Mike on the economy flight and that he can find Mike there f, and
whether he can remove Mike from the economy flight and that Mike is no longer there g.
 When testing how the economy flight works with a VIP passenger, he creates
James as a VIP passenger h. Then, he checks the ID of the flight i, whether
he can add James on the economy flight and that he can find James there j,
and whether he cannot remove James from the economy flight and that James
is still there 1).
c
d
e
f
g
h
i
j
1)
414 CHAPTER 20 Test-driven development with JUnit 5
John follows the business logic for a business flight and translates it into the following
tests.
public class AirportTest {
[...]
@DisplayName("Given there is a business flight")
@Nested
class BusinessFlightTest {
 private Flight businessFlight;
 @BeforeEach
 void setUp() {
 businessFlight = new Flight("2", "Business");
 }
 @Test
 public void testBusinessFlightRegularPassenger() {
 Passenger mike = new Passenger("Mike", false);
 assertEquals(false, businessFlight.addPassenger(mike));
 assertEquals(0, businessFlight.getPassengersList().size());
 assertEquals(false, businessFlight.removePassenger(mike));
 assertEquals(0, businessFlight.getPassengersList().size());
 }
 @Test
 public void testBusinessFlightVipPassenger() {
 Passenger james = new Passenger("James", true);
 assertEquals(true, businessFlight.addPassenger(james));
 assertEquals(1, businessFlight.getPassengersList().size());
 assertEquals(false, businessFlight.removePassenger(james));
 assertEquals(1, businessFlight.getPassengersList().size());
 }
}
}
In this listing:
 John declares a nested test class BusinessFlightTest and labels it "Given
there is a business flight" with the help of the @DisplayName annotation B.
 He declares a business flight and initializes it before the execution of each
test c.
 When testing how the business flight works with a regular passenger, he creates
Mike as a regular passenger d. Then, he checks that he cannot add Mike to the
business flight e and that trying to remove Mike from the business flight also
has no effect f.
Listing 20.6 Testing the business logic of the business flight
b
c
d
e
f
g
h
i
Preparing the flight-management application for TDD 415
 When testing how the business flight works with a VIP passenger, he creates
James as a VIP passenger g. Then, he checks that he can add James to the business flight and that he can find James there h and that he cannot remove
James from the business flight and that James is still there i.
If we run the tests with coverage from within IntelliJ IDEA, we get the results shown in
figure 20.5. For more details about test coverage and how to run tests with coverage
from IntelliJ IDEA, you can revisit chapter 6.
Figure 20.5 The result of running the economy and business flight tests with coverage using IntelliJ IDEA: the
Airport class is uncovered (it contains the main method; we do not test it), and the Flight class has coverage
of less than 100%.
John has successfully verified the functionality of the application by writing tests for all
the scenarios that result from the business logic (figure 20.1 and 20.2). It is possible
that in real life you may begin working with an application that has no tests and want
to move to TDD. Before you do, you will have to test the application as it is.
 John’s work also provides additional conclusions. The Airport class is not
tested—it served as a client for the Passenger and Flight classes. The tests are now
serving as clients, so Airport can be removed. In addition, the code coverage is not
100%. The getFlightType method is not used, and the default case—when a flight
is neither economy nor business type—is not covered. This suggests to John the need
to refactor the application, to remove the elements that are not used. He is confident
about doing this because the application is now covered with tests and, as we said earlier, TDD enables us to gain confidence in our code over time.
416 CHAPTER 20 Test-driven development with JUnit 5
20.4 Refactoring the flight-management application
John has noticed that the lines of code that are not being executed are those related
to using the flightType field. And the default case will never be executed, as the
flight type is expected to be either economy or business; these default alternatives are
needed because the code will not compile otherwise. Can John get rid of them by
doing some refactoring and replacing the conditional statements with polymorphism?
 The key to refactoring is to move the design to using polymorphism instead of procedural-style conditional code. With polymorphism (the ability of one object to pass
more than one IS-A test), the method you are calling is determined not at compile
time, but at runtime, depending on the effective object type (see chapter 6).
 The principle in action here is called the open/closed principle (figure 20.6). Practically, it means the design shown on the left will require changes to the existing class
each time we add a new flight type. These changes may reflect in each conditional
decision made based on the flight type. In addition, we are forced to rely on the
flightType field and introduce unexecuted default cases.
Figure 20.6 Refactoring the flight-management application by replacing the conditional with polymorphism: the
flightType field is removed, and a hierarchy of classes is introduced.
With the design on the right—which is refactored by replacing conditional with polymorphism—we do not need a flightType evaluation or a default value in the
switch instructions from listing 20.2. We can even add a new type—let’s anticipate a
little and call it PremiumFlight—by simply extending the base class and defining its
behavior. According to the open/closed principle, the hierarchy will be open for
extensions (we can easily add new classes) but closed for modifications (existing
classes, starting with the Flight base class, will not be modified).
 John will, of course, ask himself, “How can I be sure I am doing the right thing and
not affecting already working functionality?” The answer is that passing the tests
Flight
Using conditional for decisions
– String flightType
+ boolean addPassenger(Passenger passenger)
+ boolean removePassenger(Passenger passenger)
C Flight
Using polymorphism for decisions
– String flightType
+ boolean addPassenger(Passenger passenger)
+ boolean removePassenger(Passenger passenger)
C
BusinessFlightC EconomyFlightC PremiumFlightC
Have (1-> 0..n)
PassengerC
Refactoring the flight-management application 417
provides assurance that existing functionality is untouched. The benefits of the TDD
approach really show themselves!
 The refactoring will be achieved by keeping the base Flight class (listing 20.7)
and, for each conditional type, adding a separate class to extend Flight. John will
change addPassenger and removePassenger to abstract methods and delegate
their implementation to subclasses. The flightType field is no longer significant
and will be removed.
public abstract class Flight {
 private String id;
 List<Passenger> passengers = new ArrayList<Passenger>();
 public Flight(String id) {
 this.id = id;
 }
 public String getId() {
 return id;
 }
 public List<Passenger> getPassengersList() {
 return Collections.unmodifiableList(passengers);
 }
 public abstract boolean addPassenger(Passenger passenger);
 public abstract boolean removePassenger(Passenger passenger);
}
In this listing:
 John declares the class as abstract, making it the basis of the flight hierarchy B.
 He makes the passengers list package-private, allowing it to be directly inherited by the subclasses in the same package c.
 John declares addPassenger and removePassenger as abstract methods, delegating their implementation to the subclasses d.
John introduces an EconomyFlight class that extends Flight and implements the
inherited addPassenger and removePassenger abstract methods.
public class EconomyFlight extends Flight {
 public EconomyFlight(String id) {
 super(id);
 }
Listing 20.7 Abstract Flight class, the basis of the hierarchy
Listing 20.8 EconomyFlight class, extending the abstract Flight class
b
c
d
b c
418 CHAPTER 20 Test-driven development with JUnit 5
 @Override
 public boolean addPassenger(Passenger passenger) {
 return passengers.add(passenger);
 }
 @Override
 public boolean removePassenger(Passenger passenger) {
 if (!passenger.isVip()) {
 return passengers.remove(passenger);
 }
 return false;
 }
}
In this listing:
 John declares the EconomyFlight class extending the Flight abstract class B
and creates a constructor calling the constructor of the superclass c.
 He implements the addPassenger method according to the business logic: he
simply adds a passenger to an economy flight with no restrictions d.
 He implements the removePassenger method according to the business
logic: a passenger can be removed from a flight only if the passenger is not a
VIP e.
John also introduces a BusinessFlight class that extends Flight and implements
the inherited addPassenger and removePassenger abstract methods.
public class BusinessFlight extends Flight {

 public BusinessFlight(String id) {
 super(id);
 }
 @Override
 public boolean addPassenger(Passenger passenger) {
 if (passenger.isVip()) {
 return passengers.add(passenger);
 }
 return false;
 }
 @Override
 public boolean removePassenger(Passenger passenger) {
 return false;
 }
}
Listing 20.9 BusinessFlight class, extending the abstract Flight class
d
e
b
c
d
e
Refactoring the flight-management application 419
In this listing:
 John declares the BusinessFlight class extending the Flight abstract class
B and creates a constructor calling the constructor of the superclass c.
 He implements the addPassenger method according to the business logic:
only a VIP passenger can be added to a business flight d.
 He implements the removePassenger method according to the business
logic: a passenger cannot be removed from a business flight e.
Refactoring by replacing the conditional with polymorphism, we immediately see that
the methods now look much shorter and clearer, not cluttered with decision making.
Also, we are not forced to treat the previous default case that was never expected and
that threw an exception. Of course, the refactoring and the API changes propagate
into the tests, as shown next.
public class AirportTest {
 @DisplayName("Given there is an economy flight")
 @Nested
 class EconomyFlightTest {
 private Flight economyFlight;
 @BeforeEach
 void setUp() {
 economyFlight = new EconomyFlight("1");
 }
 [...]
 }
 @DisplayName("Given there is a business flight")
 @Nested
 class BusinessFlightTest {
 private Flight businessFlight;
 @BeforeEach
 void setUp() {
 businessFlight = new BusinessFlight("2");
 }
 [...]
 }
}
In this listing, John replaces the previous Flight instantiations with instantiations of
EconomyFlightB and BusinessFlightc. He also removes the Airport class that
served as a client for the Passenger and Flight classes—it is no longer needed, now
that John has introduced the tests. It previously served to declare the main method that
created different types of flights and passengers and made them act together.
Listing 20.10 Refactoring propagation into the AirportTest class
b
c
420 CHAPTER 20 Test-driven development with JUnit 5
Figure 20.7 Running the economy and business flight tests after refactoring the flight-management application
brings us to 100% code coverage.
If we run the tests now, we see that the code coverage is 100% (figure 20.7). So, refactoring the TDD application has helped both to improve the quality of the code and to
increase the testing code coverage.
 John has covered the flight-management application with tests and refactored it,
resulting in better code quality and 100% code coverage. It is time for him to start
introducing new features by working with TDD!
20.5 Introducing new features using TDD
After moving the software to TDD and refactoring it, John is responsible for the
implementation of new features required by the customer that extend the application
policies.
20.5.1 Adding a premium flight
The first new features that John will implement are a new flight type—premium—and
policies concerning this flight type. There is a policy for adding a passenger: if the passenger is a VIP, the passenger should be added to the premium flight; otherwise, the
request must be rejected (figure 20.8). There is also a policy for removing a passenger: if required, a passenger may be removed from a flight (figure 20.9). (The company is sorry; you may be an important person, but there are rules and restrictions,
and the company may be forced to remove you from a flight.)
Introducing new features using TDD 421
John realizes that this new feature has similarities to the previous ones. He would like
to take increased advantage of working TDD style and do more refactoring—this time,
to the tests. This is in the spirit of the Rule of Three, as stated by Don Roberts (https://
en.wikipedia.org/wiki/Rule_of_three_(computer_programming)):
The first time you do something, you just do it. The second time you do something similar,
you wince at the duplication, but you do the duplicate thing anyway. The third time you
do something similar, you refactor.
So, three strikes and you refactor.
John considers that, after receiving the requirement for the implementation of this
third flight type, it is time to do some more grouping of the existing tests using the
JUnit 5 @Nested annotation and then implement the premium flight requirement in
a similar way. Following is the refactored AirportTest class before moving to the
work for the premium flight.
public class AirportTest {
 @DisplayName("Given there is an economy flight")
 @Nested
 class EconomyFlightTest {
 private Flight economyFlight;
 private Passenger mike;
 private Passenger james;
 @BeforeEach
 void setUp() {
Listing 20.11 Refactored AirportTest class
Add passenger to premium flight
Reject request
Is passenger VIP?
NoYes
Approve request
Figure 20.8 The extended business logic
of adding a passenger to a premium flight:
only VIP passengers are allowed to join.
Remove passenger from premium flight
Approve request
Figure 20.9 The extended business logic of
removing a passenger: any type of passenger
may be removed from a premium flight.
b
422 CHAPTER 20 Test-driven development with JUnit 5
 economyFlight = new EconomyFlight("1");
 mike = new Passenger("Mike", false);
 james = new Passenger("James", true);
 }
 @Nested
 @DisplayName("When we have a regular passenger")
 class RegularPassenger {
 @Test
 @DisplayName(
 "Then you can add and remove him from an economy flight")
 public void testEconomyFlightRegularPassenger() {
 assertAll(
 "Verify all conditions for a regular passenger
 and an economy flight",
 () -> assertEquals("1", economyFlight.getId()),
 () -> assertEquals(true,
 economyFlight.addPassenger(mike)),
 () -> assertEquals(1,
 economyFlight.getPassengersList().size()),
 () -> assertEquals("Mike",
 economyFlight.getPassengersList()
 .get(0).getName()),
 () -> assertEquals(true,
 economyFlight.removePassenger(mike)),
 () -> assertEquals(0,
 economyFlight.getPassengersList().size())
 );
 }
 }
 @Nested
 @DisplayName("When we have a VIP passenger")
 class VipPassenger {
 @Test
 @DisplayName("Then you can add him but
 cannot remove him from an economy flight")
 public void testEconomyFlightVipPassenger() {
 assertAll("Verify all conditions for a VIP passenger
 and an economy flight",
 () -> assertEquals("1", economyFlight.getId()),
 () -> assertEquals(true,
 economyFlight.addPassenger(james)),
 () -> assertEquals(1,
 economyFlight.getPassengersList().size()),
 () -> assertEquals("James",
 economyFlight.getPassengersList().get(0).getName()),
 () -> assertEquals(false,
 economyFlight.removePassenger(james)),
 () -> assertEquals(1,
 economyFlight.getPassengersList().size())
 );
 }
 }
 }
c
d
e
f
e
f
Introducing new features using TDD 423
 @DisplayName("Given there is a business flight")
 @Nested
 class BusinessFlightTest {
 private Flight businessFlight;
 private Passenger mike;
 private Passenger james;
 @BeforeEach
 void setUp() {
 businessFlight = new BusinessFlight("2");
 mike = new Passenger("Mike", false);
 james = new Passenger("James", true);
 }
 @Nested
 @DisplayName("When we have a regular passenger")
 class RegularPassenger {
 @Test
 @DisplayName("Then you cannot add or remove him
 from a business flight")
 public void testBusinessFlightRegularPassenger() {
 assertAll("Verify all conditions for a regular passenger
 and a business flight",
 () -> assertEquals(false,
 businessFlight.addPassenger(mike)),
 () -> assertEquals(0,
 businessFlight.getPassengersList().size()),
 () -> assertEquals(false,
 businessFlight.removePassenger(mike)),
 () -> assertEquals(0,
 businessFlight.getPassengersList().size())
 );
 }
 }
 @Nested
 @DisplayName("When we have a VIP passenger")
 class VipPassenger {
 @Test
 @DisplayName("Then you can add him but cannot remove him
 from a business flight")
 public void testBusinessFlightVipPassenger() {
 assertAll("Verify all conditions for a VIP passenger
 and a business flight",
 () -> assertEquals(true,
 businessFlight.addPassenger(james)),
 () -> assertEquals(1,
 businessFlight.getPassengersList().size()),
 () -> assertEquals(false,
 businessFlight.removePassenger(james)),
 () -> assertEquals(1,
 businessFlight.getPassengersList().size())
 );
 }
 }
 }
}
b
c
d
e
f
e
f
424 CHAPTER 20 Test-driven development with JUnit 5
In this listing:
 In the existing nested classes EconomyFlightTest and BusinessFlightTest, John groups the flight and passenger fields, as he would like to add one
more testing level and reuse these fields for all tests concerning a particular
flight type B. He initializes these fields before the execution of each test c.
 He introduces a new nesting level to test different passenger types. He uses the
JUnit 5 @DisplayName annotation to label the classes in a way that is more
expressive and easier to follow d. All of these labels start with the keyword When.
 He labels all existing tests with the help of the JUnit 5 @DisplayName annotation e. All of these labels start with the keyword Then.
 He refactors the checking of the conditions by using the assertAll JUnit 5
method and grouping all previously existing conditions, which can now be read
fluently f.
This is how John has refactored the existing tests, to facilitate continuing to work in
TDD style and to introduce the newly required premium flight business logic. If we
run the tests now, we can easily follow the way they work and how they check the business logic (figure 20.10). Any new developer joining the project will find these tests
extremely valuable as part of the documentation!
Figure 20.10 Running the refactored AirportTest for the economy flight and the business flight allows the
developer to follow how the tests are working.
Introducing new features using TDD 425
John moves now to the implementation of the PremiumFlight class and its logic.
He will create PremiumFlight as a subclass of Flight and override the addPassenger and removePassenger methods, but they act like stubs—they do not do
anything and simply return false. Their behavior will be extended later. Working
TDD style involves creating the tests first and then the business logic.
public class PremiumFlight extends Flight {
 public PremiumFlight(String id) {
 super(id);
 }
 @Override
 public boolean addPassenger(Passenger passenger) {
 return false;
 }
 @Override
 public boolean removePassenger(Passenger passenger) {
 return false;
 }
}
In this listing:
 John declares the PremiumFlight class that extends Flight B, and he creates a constructor for it c.
 He creates the addPassenger d and removePassenger e methods as stubs,
without any business logic. They simply return false.
John now implements the tests according to the premium flight business logic from
figures 20.8 and 20.9.
public class AirportTest {
 [...]
 @DisplayName("Given there is a premium flight")
 @Nested
 class PremiumFlightTest {
 private Flight premiumFlight;
 private Passenger mike;
 private Passenger james;
 @BeforeEach
 void setUp() {
Listing 20.12 Initial design of the PremiumFlight class
Listing 20.13 Tests for the behavior of PremiumFlight
b
c
d
e
b
c
426 CHAPTER 20 Test-driven development with JUnit 5
 premiumFlight = new PremiumFlight("3");
 mike = new Passenger("Mike", false);
 james = new Passenger("James", true);
 }
 @Nested
 @DisplayName("When we have a regular passenger")
 class RegularPassenger {

 @Test
 @DisplayName("Then you cannot add or remove him
 from a premium flight")
 public void testPremiumFlightRegularPassenger() {
 assertAll("Verify all conditions for a regular passenger
 and a premium flight",
 () -> assertEquals(false,
 premiumFlight.addPassenger(mike)),
 () -> assertEquals(0,
 premiumFlight.getPassengersList().size()),
 () -> assertEquals(false,
 premiumFlight.removePassenger(mike)),
 () -> assertEquals(0,
 premiumFlight.getPassengersList().size())
 );
 }
 }
 @Nested
 @DisplayName("When we have a VIP passenger")
 class VipPassenger {
 @Test
 @DisplayName("Then you can add and remove him
 from a premium flight")
 public void testPremiumFlightVipPassenger() {
 assertAll("Verify all conditions for a VIP passenger
 and a premium flight",
 () -> assertEquals(true,
 premiumFlight.addPassenger(james)),
 () -> assertEquals(1,
 premiumFlight.getPassengersList().size()),
 () -> assertEquals(true,
 premiumFlight.removePassenger(james)),
 () -> assertEquals(0,
 premiumFlight.getPassengersList().size())
 );
 }
 }
 }
}
In this listing:
 John declares the nested class PremiumFlightTest b that contains the fields
representing the flight and the passengers c that are set up before each test d.
d
e
f
g
h
i
j
1)
1!
1@
1#
Introducing new features using TDD 427
 He creates two classes nested at the second level in PremiumFlightTest:
RegularPassengere and VipPassengerj. He uses the JUnit 5 @DisplayName annotation to label these classes starting with the keyword When.
 He inserts one test in each of the newly added RegularPassenger f and
VipPassenger 1) classes. He labels these tests with the JUnit 5 @DisplayName
annotation starting with the keyword Then.
 Testing a premium flight and a regular passenger, John uses the assertAll
method to verify multiple conditions g. He checks that he cannot add a passenger to a premium flight and that trying to add a passenger does not change
the size of the passenger list h. Then, he checks that he cannot remove a passenger from a premium flight and that trying to remove a passenger does not
change the size of the passenger list i.
 Testing a premium flight and a VIP passenger, John again uses assertAll 1!.
He checks that he can add a passenger to a premium flight and that doing so
increases the size of the passenger list 1@. Then, he checks that he can remove a
passenger from a premium flight and that doing so decreases the size of the
passenger list 1#.
After writing the tests, John runs them. Remember, he is working TDD style, so tests
come first. The result is shown in figure 20.11.
Figure 20.11 Running the newly added tests to check premium flights before the code implementation results in
some test failures. We need to understand which behavior to introduce to fix the failing tests.
428 CHAPTER 20 Test-driven development with JUnit 5
The fact that one of the tests is failing is not a problem. On the contrary: it is what John
expected. Remember, working TDD style means being driven by tests, so we first create
the test to fail and then write the piece of code that will make the test pass. But there is
another remarkable thing here: the test for a premium flight and a regular passenger
is already green. This means the existing business logic (the addPassenger and
removePassenger methods returning false) is just enough for this case. John
understands that he only has to focus on the VIP passenger. To quote Kent Beck again,
“TDD helps you to pay attention to the right issues at the right time so you can make
your designs cleaner, you can refine your designs as you learn. TDD enables you to gain
confidence in the code over time.”
 So, John moves back to the PremiumFlight class and adds the business logic only
for VIP passengers. Driven by tests, he gets straight to the point.
public class PremiumFlight extends Flight {
 public PremiumFlight(String id) {
 super(id);
 }
 @Override
 public boolean addPassenger(Passenger passenger) {
 if (passenger.isVip()) {
 return passengers.add(passenger);
 }
 return false;
 }
 @Override
 public boolean removePassenger(Passenger passenger) {
 if (passenger.isVip()) {
 return passengers.remove(passenger);
 }
 return false;
 }
}
In this listing:
 John adds a passenger only if the passenger is a VIP B.
 John removes a passenger only if the passenger is a VIP c.
The result of running the tests now is shown in figure 20.12. Everything went smoothly
and was driven by the tests that guide the developer in writing the code that makes
them pass. Additionally, the code coverage is 100%.
Listing 20.14 PremiumFlight class with the full business logic
b
c
Introducing new features using TDD 429
Figure 20.12 Running the full test suite (economy, business, and premium flights) after adding the business logic
for PremiumFlight: code coverage is 100%.
20.5.2 Adding a passenger only once
Occasionally, on purpose or by mistake, the same passenger has been added to a flight
more than once. This has caused problems with managing seats, and these situations
must be avoided. John needs to make sure that whenever someone tries to add a passenger, if the passenger has been previously added to the flight, the request should be
rejected. This is new business logic, and John will implement it TDD style.
 John will begin the implementation of this new feature by adding the test to check
it. He will try repeatedly to add the same passenger to a flight, as shown in the following listing. We’ll detail only the case of a regular passenger repeatedly added to an
economy flight, all other cases being similar.
public class AirportTest {
 @DisplayName("Given there is an economy flight")
 @Nested
 class EconomyFlightTest {
 private Flight economyFlight;
 private Passenger mike;
 private Passenger james;
Listing 20.15 Trying to add the same passenger repeatedly to the same flight
430 CHAPTER 20 Test-driven development with JUnit 5
 @BeforeEach
 void setUp() {
 economyFlight = new EconomyFlight("1");
 mike = new Passenger("Mike", false);
 james = new Passenger("James", true);
 }
 @Nested
 @DisplayName("When we have a regular passenger")
 class RegularPassenger {
 [...]
 @DisplayName("Then you cannot add him to an economy flight
 more than once")
 @RepeatedTest(5)
 public void testEconomyFlightRegularPassengerAddedOnlyOnce
 (RepetitionInfo repetitionInfo) {
 for (int i=0; i<repetitionInfo.getCurrentRepetition();
 i++) {
 economyFlight.addPassenger(mike);
 }
 assertAll("Verify a regular passenger can be added
 to an economy flight only once",
 () -> assertEquals(1,
 economyFlight.getPassengersList().size()),
 () -> assertTrue(
 economyFlight.getPassengersList().
 contains(mike)),
 () -> assertTrue(
 economyFlight.getPassengersList()
 .get(0).getName().equals("Mike")));
 }
 }
 }
}
In this listing:
 John marks the test as @RepeatedTest five times b and uses the RepetitionInfo parameter in it c.
 Each time a test is executed, he tries to add the passenger the number of times
specified by the RepetitionInfo parameter d.
 He performs verifications using the assertAll method e: he checks that the
list of passengers is size 1 f, that the list contains the added passenger g, and
that the passenger is in the first position h.
If we run the tests, they fail. There is no business logic yet to prevent adding a passenger more than once (figure 20.13).
b
c d
e
f
g
h
Introducing new features using TDD 431
Figure 20.13 Running the tests that check whether a passenger can be added only once to a flight, before
implementing the business logic, will result in failure.
To ensure the unicity of the passengers on a flight, John changes the passenger list
structure to a set. So, he does some code refactoring that will also propagate across
the tests. The Flight class changes as shown next.
public abstract class Flight {
 [...]
 Set<Passenger> passengers = new HashSet<>();
 [...]
 public Set<Passenger> getPassengersSet() {
 return Collections.unmodifiableSet(passengers);
 }
 [...]
}
In this listing, John changes the type and the initialization of the passengers attribute to a set B, changes the name of the method c, and returns an unmodifiable
set d.
 John then creates a new test to check that a passenger can be added only once to a
flight.
Listing 20.16 Flight class after changing the list of passengers to a set
b
c
d
432 CHAPTER 20 Test-driven development with JUnit 5
@DisplayName("Then you cannot add him to an economy flight
 more than once")
@RepeatedTest(5)
public void testEconomyFlightRegularPassengerAddedOnlyOnce
 (RepetitionInfo repetitionInfo) {
 for (int i=0; i<repetitionInfo.getCurrentRepetition(); i++){
 economyFlight.addPassenger(mike);
 }
 assertAll("Verify a regular passenger can be added
 to an economy flight only once",
 () -> assertEquals(1,
 economyFlight.getPassengersSet().size()),
 () -> assertTrue(
 economyFlight.getPassengersSet().contains(mike)),
 () -> assertTrue(
 new ArrayList<>(economyFlight.getPassengersSet())
 .get(0).getName().equals("Mike")));
}
In this listing, John checks the size of the passengers set B, the fact that this set contains the newly added passenger c, and that the passenger is in the first position d,
after constructing a list from the existing set (he needs to do this because a set has no
order for the elements).
 Running the tests is now successful, with code coverage of 100% (figure 20.14).
John has implemented this new feature in TDD style.
Figure 20.14 Successfully running the entire test suite after implementing the business logic to check that a
passenger can be added only once to a flight
Listing 20.17 New test checking that a passenger can be added only once to a flight
b
c
d
Summary 433
The next chapter is dedicated to another software development process that is frequently used today: behavior-driven development (BDD).
Summary
This chapter has covered the following:
 Examining the concept of TDD and demonstrating how it helps us develop safe
applications because tests prevent the introduction of bugs into working code
and act as part of the documentation
 Preparing a non-TDD application to be moved to TDD by adding hierarchical
JUnit 5 tests that cover the existing business logic
 Refactoring and improving the code quality of this TDD application by replacing conditional with polymorphism while relying on the tests we developed
 Implementing new features in by working TDD style, starting by writing tests
and then implementing the business logic


# 10. Running JUnit tests from Maven 3

mvn archetype:generate -DgroupId=com.manning.junitbook
-DartifactId=maven-sampling
-DarchetypeArtifactid=maven-artifact-mojo

<project xmlns="http://maven.apache.org/POM/4.0.0"
 xmlns:xsi=http://www.w3.org/2001/XMLSchema-instance
 xsi:schemaLocation="http://maven.apache.org/POM/4.0.0
 http://maven.apache.org/maven-v4_0_0.xsd">

 <modelVersion>4.0.0</modelVersion>
 <groupId>com.manning.junitbook</groupId>
 <artifactId>maven-sampling</artifactId>
 <version>1.0-SNAPSHOT</version>
 <name>maven-sampling</name>
 <!-- FIXME change it to the project's website -->
 <url>http://www.example.com</url>
 [...]
 <dependencies>
 <dependency>
 <groupId>junit</groupId>
 <artifactId>junit</artifactId>
 <version>4.11</version>
 <scope>test</scope>
 </dependency>
 </dependencies>
 [...]
</project>


 modelVersion—Represents the version of the model of the pom being used.
Currently, the only supported version is 4.0.0.
 groupId—Acts as the Java packaging in the filesystem, grouping different projects from one organization, company, or group of people. We provide this value
in the command line when invoking Maven.
 artifactId—Represents the name that the project is known by. Again, the
value here is the one we specified in the command line.
 version—Identifies the current version of our project (or project artifact).
The SNAPSHOT ending indicates that this artifact is still in development mode;
we have not released it yet.
 dependencies—Lists our dependencies.
Now that we have our project descriptor, we can improve it a little (listing 10.2). First,
we need to change the version of the JUnit dependency, because we are using JUnit
Jupiter 5.4.2, and the version that the plugin generated is 4.11. After that, we can
Listing 10.1 pom.xml for the maven-sampling project
192 CHAPTER 10 Running JUnit tests from Maven 3
insert some additional information to make the pom.xml more descriptive, such as a
developers section. This information not only makes the pom.xml more descriptive
but also will be included later when we build the website.
<dependencies>
 <dependency>
 <groupId>org.junit.jupiter</groupId>
 <artifactId>junit-jupiter-api</artifactId>
 <version>5.6.0</version>
 <scope>test</scope>
 </dependency>
 <dependency>
 <groupId>org.junit.jupiter</groupId>
 <artifactId>junit-jupiter-engine</artifactId>
 <version>5.6.0</version>
 <scope>test</scope>
 </dependency>
</dependencies>
<developers>
 <developer>
 <name>Catalin Tudose</name>
 <id>ctudose</id>
 <organization>Manning</organization>
 <roles>
 <role>Java Developer</role>
 </roles>
 </developer>
 <developer>
 <name>Petar Tahchiev</name>
 <id>ptahchiev</id>
 <organization>Apache Software Foundation</organization>
 <roles>
 <role>Java Developer</role>
 </roles>
 </developer>
</developers>
We also specify organization, description, and inceptionYear.
<description>
 "JUnit in Action III" book, the sample project for the "Running Junit
 tests from Maven" chapter.
</description>
<organization>
 <name>Manning Publications</name>
 <url>http://manning.com/</url>
</organization>
<inceptionYear>2019</inceptionYear>
Listing 10.2 Changes and additions to pom.xml
Listing 10.3 Description elements in pom.xml
Using the Maven plugins 193
Now we can start developing our software. What if you want to use a Java IDE other
than IntelliJ IDEA, such as Eclipse? No problem. Maven offers additional plugins that
let you import the project into your favorite IDE. To use Eclipse, open a terminal and
navigate to the directory that contains the project descriptor (pom.xml). Then type
the following and press Enter:
mvn eclipse:eclipse
This command invokes maven-eclipse-plugin, which, after downloading the necessary artifacts, produces the two files (.project and .classpath) that Eclipse needs to
recognize the project as an Eclipse project. Next, you can import your project into
Eclipse. All the dependencies listed in the pom.xml file are added to the project (figure 10.2).
Developers who use IntelliJ IDEA can import the project directly (refer to figure
10.1), as this IDE invokes the Maven plugin when such a project is open.
10.2 Using the Maven plugins
We have seen what Maven is and how to use it to start a project from scratch. We have
also seen how to generate the project documentation and how to import a project
into Eclipse and IntelliJ.
Figure 10.2 The imported project,
with the Maven folders structure
and needed dependencies
(including the JUnit 5 ones)
194 CHAPTER 10 Running JUnit tests from Maven 3
 Whenever you would like to clean the project from previous activities, execute the
following command:
mvn clean
This command causes Maven to go through the clean phase and invoke all the plugins
that are attached to this phase—in particular, maven-clean-plugin, which deletes
the target/ folder where your generated site resides.
10.2.1 Maven compiler plugin
Like any other build system, Maven is supposed to build our projects (compile our
software and package in an archive). Every task in Maven is performed by an appropriate plugin, the configuration of which is in the <plugins> section of the project
descriptor. To compile the source code, all you need to do is invoke the compile phase
on the command line
mvn compile
which causes Maven to execute all the plugins attached to the compile phase (in particular, it will invoke maven-compiler-plugin). But before invoking the compile
phase, as already discussed, Maven goes through the validate phase, downloads all the
dependencies listed in the pom.xml file, and includes them in the classpath of the
project. When the compilation process is complete, you can go to the target/classes/
folder and see the compiled classes there.
 Next, we’ll try to configure the compiler plugin, escaping from the conventionover-configuration principle. So far, the conventional compiler plugin has worked
well. But what if we need to include the –source and –target attributes in the compiler invocation to generate class files for the specific version of the JVM? We can add
the following code to the <build> section of our build file.
<build>
 <plugins>
 <plugin>
 <artifactId>maven-compiler-plugin</artifactId>
 <version>2.3.2</version>
 <configuration>
 <source>1.8</source>
 <target>1.8</target>
 </configuration>
 </plugin>
 </plugins>
</build>
Listing 10.4 Configuring maven-compiler-plugin
Using the Maven plugins 195
This code is a general way to configure each of our Maven plugins: enter a
<plugins> section in our <build> section. There, we list each plugin that we want
to configure—in this case, the maven-compiler-plugin. We need to enter the configuration parameters in the plugin configuration section. We can get a list of
parameters for every plugin from the Maven website. Without the <source> and
<target> parameters, the Java version to be used will be 5, which is quite old.
 As we see in the declaration of the maven-compiler-plugin in listing 10.4, we
have not set the groupId parameter. maven-compiler-plugin is one of the core
Maven plugins that has an org.apache.maven.plugins groupId, and plugins
with such a groupId can skip the groupId parameter.
10.2.2 Maven Surefire plugin
To process the unit tests from our project, Maven uses (of course) a plugin. The
Maven plugin that executes the unit tests is called maven-surefire-plugin. The
Surefire plugin executes the unit tests for our code, but these unit tests are not necessarily JUnit tests.
 There are other frameworks for unit testing, and the Surefire plugin can execute
their tests, too. The following listing shows the configuration of the Maven Surefire
plugin.
<build>
 <plugins>
 <plugin>
 <artifactId>maven-surefire-plugin</artifactId>
 <version>2.22.2</version>
 </plugin>
 </plugins>
</build>
The conventional way to start maven-surefire-plugin is very simple: invoke the
Maven test phase. This way, Maven first invokes all the phases that are supposed to
come before the test phase (validate and compile) and then invokes all the plugins
that are attached to the test phase, thus invoking maven-surefire-plugin. So by
calling
mvn clean test
Maven first cleans the target/ directory, then compiles the source code and the tests,
and finally lets JUnit 5 execute all the tests that are in the src/test/java directory
(remember the convention). The output should be similar to figure 10.3.
 That’s great, but what if we want to execute only a single test case? This execution
is unconventional, so we need to configure maven-surefire-plugin to do it.
Listing 10.5 The Maven Surefire plugin
196 CHAPTER 10 Running JUnit tests from Maven 3
Ideally, a parameter for the plugin allows us to specify the pattern of test cases that we
want to execute. We configure the Surefire plugin in exactly the same way that we configure the compiler plugin.
<build>
 <plugins>
 [...]
 <plugin>
 <artifactId>maven-surefire-plugin</artifactId>
 <version>2.22.2</version>
 <configuration>
 <includes>**/*Test.java</includes>
 </configuration>
 [...]
 </plugin>
 [...]
 </plugins>
</build>
We have specified the includes parameter to denote that we want only the test cases
matching the given pattern to be executed. But how do we know what parameters
maven-surefire-plugin accepts? No one knows all the parameters by heart, of
course, but we can always consult the maven-surefire-plugin documentation (and
any other plugin documentation) on the Maven website (http://maven.apache.org).
Listing 10.6 Configuration of maven-surefire-plugin
Figure 10.3 Execution of JUnit tests with Maven 3
Using the Maven plugins 197
 The next step is generating documentation for the project. But wait a second—
how are we supposed to do that with no files to generate the documentation from?
This is another one of Maven’s great benefits: with a little configuration and description, we can produce a fully functional website skeleton.
 First, add the maven-site-plugin to the Maven pom.xml configuration file:
<plugin>
 <groupId>org.apache.maven.plugins</groupId>
 <artifactId>maven-site-plugin</artifactId>
 <version>3.7.1</version>
</plugin>
Then, type
mvn site
on the command line where the pom.xml file is. Maven should start downloading its
plugins; and after their successful installation, it produces the website shown in figure
10.4.
Figure 10.4 Maven produces a website documenting the project.
This website is generated in the Maven build directory—another convention. Maven
uses the target/ folder for all the needs of the build itself. The source code is compiled
in the target/classes/ folder, and the documentation is generated in target/site/.
 Notice that the website is more like a skeleton of a website. Remember that we
entered a small amount of data; we could enter more data and web pages in src/site,
and Maven would include them on the website, thus generating full-blown documentation.
10.2.3 Generating HTML JUnit reports with Maven
Maven can generate reports from the JUnit XML output. Because, by default, Maven
produces plain-text and XML-formatted output (by convention, they go in the target/
Surefire-reports/ folder), we do not need any other configuration to produce HTML
Surefire reports for the JUnit tests.
198 CHAPTER 10 Running JUnit tests from Maven 3
 As you may already guess, the job of producing these reports is done by a Maven
plugin. The name of the plugin is maven-surefire-report-plugin, and by
default, it is not attached to any of the core phases that we already know. (Many people do not need HTML reports every time they build software.) We can’t invoke the
plugin by running a certain phase (as we did with both the compiler plugin and the
Surefire plugin). Instead, we have to call it directly from the command line:
mvn surefire-report:report
Maven tries to compile the source files and the test cases, and then it invokes the Surefire plugin to produce the plain-text and XML-formatted output of the tests. After
that, the surefire-report plugin tries to transform all the XML from the target/
surefire-reports/ directory into an HTML report that will be placed in the target/site
directory. (Remember that this is the convention for the folder—to keep all the generated documentation of the project—and that the HTML reports are considered to
be documentation.) If we open the generated HTML report, it looks something like
figure 10.5.
Figure 10.5 HTML report from the maven-surefire-report plugin
10.3 Putting it all together
This section demonstrates all the steps involved in creating a JUnit 5 project that is
managed by Maven. As discussed in chapter 9, one of the projects under development
at Tested Data Systems is a flight-management application. To start such an application, George, a project developer, creates the C:\Work\ folder, which will become the
working directory and the one where he will set up the Maven project. He types the
following on the command line:
mvn archetype:generate -DgroupId=com.testeddatasystems.flights
-DartifactId=flightsmanagement -DarchetypeArtifactid=maven-artifact-mojo
After pressing Enter and waiting for the appropriate artifacts to be downloaded,
George accepts the default options (figure 10.6). Figure 10.7 shows the structure of
Putting it all together 199
the project: src/main/java/ is the Maven folder in which the Java code for the project
resides, and src/test/java contains the unit tests. The .idea folder and the flightsmanagement.iml file are created by the IDE.
Figure 10.6 Creating a Maven 3 project and accepting the default options
Figure 10.7 The newly created flight-management project 
200 CHAPTER 10 Running JUnit tests from Maven 3
George invokes maven-archetype-plugin from the command line and tells it to
generate a new project from scratch with the given parameters. As a result, the Maven
plugin creates a new project with the default folder structure, as well as a sample
App.java class with the main method and a corresponding AppTest.java file that is a
unit test for the application. The Maven plugin also generates a pom.xml file, as
shown in the next listing.
<project xmlns="http://maven.apache.org/POM/4.0.0"
 xmlns:xsi=http://www.w3.org/2001/XMLSchema-instance
 xsi:schemaLocation="http://maven.apache.org/POM/4.0.0
 http://maven.apache.org/maven-v4_0_0.xsd">

 <modelVersion>4.0.0</modelVersion>
 <groupId>com.testeddatasystems.flights</groupId>
 <artifactId>flightsmanagement</artifactId>
 <version>1.0-SNAPSHOT</version>
 <name> flightsmanagement </name>
 [...]
 <dependencies>
 <dependency>
 <groupId>junit</groupId>
 <artifactId>junit</artifactId>
 <version>4.11</version>
 <scope>test</scope>
 </dependency>
 </dependencies>
 [...]
</project>
This code is the build descriptor for the project. It includes the modelVersion (the
version of the model of the POM being used, which currently is 4.0.0), the groupId
(com.testeddatasystems.flights), the artifactId (the name by which the
project is known—flightsmanagement in this case), and the version. 1.0-SNAPSHOT indicates that the artifact is still in development mode. POM stands for project
object model; the POM contains information about the project and its configuration,
and is the fundamental unit of work in Maven.
 George needs to change the version of the JUnit dependency because he is using
JUnit Jupiter 5.6.0, and the one that the plugin generated is version 4.11.
<dependencies>
 <dependency>
 <groupId>org.junit.jupiter</groupId>
 <artifactId>junit-jupiter-api</artifactId>
 <version>5.6.0</version>
 <scope>test</scope>
 </dependency>
Listing 10.7 pom.xml for the flight-management project
Listing 10.8 Changes and additions to pom.xml
Putting it all together 201
 <dependency>
 <groupId>org.junit.jupiter</groupId>
 <artifactId>junit-jupiter-engine</artifactId>
 <version>5.6.0</version>
 <scope>test</scope>
 </dependency>
</dependencies>
George removes the existing autogenerated App and AppTest classes and instead
introduces the two classes that will start his application: Passenger (listing 10.9) and
PassengerTest (listing 10.10).
 package com.testeddatasystems.flights;
public class Passenger {
 private String identifier;
 private String name;
 public Passenger(String identifier, String name) {
 this.identifier = identifier;
 this.name = name;
 }
 public String getIdentifier() {
 return identifier;
 }
 public String getName() {
 return name;
 }
 @Override
 public String toString() {
 return "Passenger " + getName() +
 " with identifier: " + getIdentifier();
 }
}
The Passenger class contains the following:
 Two fields: identifier and name B  A constructor, receiving as arguments identifier and name c  Getters for the identifier and name fields d  The overridden toString method e
 package com.testeddatasystems.flights;
[...}
public class PassengerTest {
Listing 10.9 Passenger class
Listing 10.10 PassengerTest class
b
c
d
e
202 CHAPTER 10 Running JUnit tests from Maven 3
 @Test
 void testPassenger() {
 Passenger passenger =
 new Passenger("123-456-789", "John Smith");
 assertEquals("Passenger John Smith with identifier:
 123-456-789",
 passenger.toString());
 }
}
The PassengerTest class contains one test: testPassenger, which verifies the output of the overridden toString method B.
 Next, George executes the mvn clean install command at the level of the project folder (figure 10.8). This command first cleans the project, removing existing artifacts. Then it compiles the source code of the project, tests the compiled source code
(using JUnit 5), packages the compiled code in JAR format (figure 10.9), and installs
the package in the local Maven repository (figure 10.10). The local Maven repository
is located at ~/.m2/repository/ in UNIX or C:\Documents and Settings\<UserName>
\.m2\repository\ in Windows.
 George has created a fully functional Maven project to develop the flight-management application and test it with JUnit 5. From now on, he can continue to add classes
and tests and execute the Maven commands to package the application and run the
tests.
Figure 10.8 Executing the mvn clean install command for the Maven project under development
b
Maven challenges 203
Figure 10.9 The jar-packaged file in the Maven target folder of the flight-management project
Figure 10.10 The local Maven repository containing the flight-management project’s artifacts





