

// HttpClient can be used either synchronously or asynchronously

BodyHandlers define the expected type of response body


// 
var request = HttpRequest.newBuilder()
  .uri(URI.create("https://example.com"))
  .GET()
  .build();

var client = HttpClient.newHttpClient();

HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString());
System.out.println(response.body());


The same request can be performed asynchronously. Calling sendAsync does not block the current thread and instead returns a CompletableFuture to construct asynchronous operation pipelines.

var request = HttpRequest.newBuilder()
    .uri(URI.create("https://winterbe.com"))
    .build();

var client = HttpClient.newHttpClient();
client.sendAsync(request, HttpResponse.BodyHandlers.ofString())
    .thenApply(HttpResponse::body)
    .thenAccept(System.out::println);

We can omit the .GET() call as it’s the default request method.

The next example sends data to a given URL via POST. Similiar to BodyHandlers you use BodyPublishers to define the type of data you want to send as body of the request such as strings, byte-arrays, files or input-streams:

var request = HttpRequest.newBuilder()
    .uri(URI.create("https://postman-echo.com/post"))
    .header("Content-Type", "text/plain")
    .POST(HttpRequest.BodyPublishers.ofString("Hi there!"))
    .build();
var client = HttpClient.newHttpClient();
var response = client.send(request, HttpResponse.BodyHandlers.ofString());
System.out.println(response.statusCode());      // 200

---

The HTTP Client was added in Java 11. It can be used to request HTTP resources over the network. It supports HTTP/1.1 and HTTP/2, both synchronous and asynchronous programming models, handles request and response bodies as reactive-streams, and follows the familiar builder pattern.

Example: GET request that prints the response body as a String
HttpClient client = HttpClient.newHttpClient();
HttpRequest request = HttpRequest.newBuilder()
      .uri(URI.create("http://openjdk.java.net/"))
      .build();
client.sendAsync(request, BodyHandlers.ofString())
      .thenApply(HttpResponse::body)
      .thenAccept(System.out::println)
      .join();
HttpClient
To send a request, first create an HttpClient from its builder. The builder can be used to configure per-client state, like:

The preferred protocol version ( HTTP/1.1 or HTTP/2 )
Whether to follow redirects
A proxy
An authenticator
HttpClient client = HttpClient.newBuilder()
      .version(Version.HTTP_2)
      .followRedirects(Redirect.SAME_PROTOCOL)
      .proxy(ProxySelector.of(new InetSocketAddress("www-proxy.com", 8080)))
      .authenticator(Authenticator.getDefault())
      .build();
Once built, an HttpClient can be used to send multiple requests.

HttpRequest
An HttpRequest is created from its builder. The request builder can be used to set:

The request URI
The request method ( GET, PUT, POST )
The request body ( if any )
A timeout
Request headers
HttpRequest request = HttpRequest.newBuilder()
      .uri(URI.create("http://openjdk.java.net/"))
      .timeout(Duration.ofMinutes(1))
      .header("Content-Type", "application/json")
      .POST(BodyPublishers.ofFile(Paths.get("file.json")))
      .build()
Once built an HttpRequest is immutable, and can be sent multiple times.

Synchronous or Asynchronous
Requests can be sent either synchronously or asynchronously. The synchronous API, as expected, blocks until the HttpResponse is available.

HttpResponse<String> response =
      client.send(request, BodyHandlers.ofString());
System.out.println(response.statusCode());
System.out.println(response.body());
The asynchronous API returns immediately with a CompletableFuture that completes with the HttpResponse when it becomes available. CompletableFuture was added in Java 8 and supports composable asynchronous programming.

client.sendAsync(request, BodyHandlers.ofString())
      .thenApply(response -> { System.out.println(response.statusCode());
                               return response; } )
      .thenApply(HttpResponse::body)
      .thenAccept(System.out::println);
Data as reactive-streams
The request and response bodies are exposed as reactive streams ( asynchronous streams of data with non-blocking back pressure.) The HttpClient is effectively a Subscriber of request body and a Publisher of response body bytes. The BodyHandler interface allows inspection of the response code and headers, before the actual response body is received, and is responsible for creating the response BodySubscriber.

public abstract class HttpRequest {
    ...
    public interface BodyPublisher
                extends Flow.Publisher<ByteBuffer> { ... }
}

public abstract class HttpResponse<T> {
    ...
    public interface BodyHandler<T> {
        BodySubscriber<T> apply(int statusCode, HttpHeaders responseHeaders);
    }

    public interface BodySubscriber<T>
                extends Flow.Subscriber<List<ByteBuffer>> { ... }
}
The HttpRequest and HttpResponse types provide a number of convenience factory methods for creating request publishers and response subscribers for handling common body types such as files, Strings, and bytes. These convenience implementations either accumulate data until the higher-level Java type can be created, like a String, or stream the data in the case of a file. The BodySubscriber and BodyPublisher interfaces can be implemented for handling data as a custom reactive stream.

HttpRequest.BodyPublishers::ofByteArray(byte[])
HttpRequest.BodyPublishers::ofByteArrays(Iterable)
HttpRequest.BodyPublishers::ofFile(Path)
HttpRequest.BodyPublishers::ofString(String)
HttpRequest.BodyPublishers::ofInputStream(Supplier<InputStream>)

HttpResponse.BodyHandlers::ofByteArray()
HttpResponse.BodyHandlers::ofString()
HttpResponse.BodyHandlers::ofFile(Path)
HttpResponse.BodyHandlers::discarding()
Thre are adapters between java.util.concurrent.Flow's Publisher/Subscriber types to the HTTP Client's BodyPublisher/BodySubscriber types:

HttpRequest.BodyPublishers::fromPublisher(...)

HttpResponse.BodyHandlers::fromSubscriber(...)
HttpResponse.BodyHandlers::fromLineSubscriber(...)
HTTP/2
The Java HTTP Client supports both HTTP/1.1 and HTTP/2. By default the client will send requests using HTTP/2. Requests sent to servers that do not yet support HTTP/2 will automatically be downgraded to HTTP/1.1. Here's a summary of the major improvements that HTTP/2 brings:

Header Compression. HTTP/2 uses HPACK compression, which reduces overhead.
Single Connection to the server, reduces the number of round trips needed to set up multiple TCP connections.
Multiplexing. Multiple requests are allowed at the same time, on the same connection.
Server Push. Additional future needed resources can be sent to a client.
Binary format. More compact.
Since HTTP/2 is the default preferred protocol, and the implementation seamlessly fallbacks to HTTP/1.1 where necessary, then the Java HTTP Client is well positioned for the future, when HTTP/2 is more widely deployed.