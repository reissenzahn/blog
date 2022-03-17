


// Clock provides access to the current date and time

// clocks are aware of a timezone
Clock clock = Clock.systemDefaultZone();

// clocks may be used instead of System.currentTimeMillis()
long millis = clock.millis();

// an instantaneous point in time is represented an an Instant object
Instant instant = clock.instant();

// Instant objects can be used to create legacy java.util.Date objects
Date legacy = Date.from(instant);



Timezones#
Timezones are represented by a ZoneId. They can easily be accessed via static factory methods. Timezones define the offsets which are important to convert between instants and local dates and times.

System.out.println(ZoneId.getAvailableZoneIds());
// prints all available timezone ids

ZoneId zone1 = ZoneId.of("Europe/Berlin");
ZoneId zone2 = ZoneId.of("Brazil/East");
System.out.println(zone1.getRules());
System.out.println(zone2.getRules());

// ZoneRules[currentStandardOffset=+01:00]
// ZoneRules[currentStandardOffset=-03:00]
LocalTime#
LocalTime represents a time without a timezone, e.g. 10pm or 17:30:15. The following example creates two local times for the timezones defined above. Then we compare both times and calculate the difference in hours and minutes between both times.

LocalTime now1 = LocalTime.now(zone1);
LocalTime now2 = LocalTime.now(zone2);

System.out.println(now1.isBefore(now2));  // false

long hoursBetween = ChronoUnit.HOURS.between(now1, now2);
long minutesBetween = ChronoUnit.MINUTES.between(now1, now2);

System.out.println(hoursBetween);       // -3
System.out.println(minutesBetween);     // -239
LocalTime comes with various factory method to simplify the creation of new instances, including parsing of time strings.

LocalTime late = LocalTime.of(23, 59, 59);
System.out.println(late);       // 23:59:59

DateTimeFormatter germanFormatter =
    DateTimeFormatter
        .ofLocalizedTime(FormatStyle.SHORT)
        .withLocale(Locale.GERMAN);

LocalTime leetTime = LocalTime.parse("13:37", germanFormatter);
System.out.println(leetTime);   // 13:37


LocalDate#

// LocalDate represents a distinct date

// an instance of LocalDate is immutable

// we can calculate new dates by adding or subtracting days, months or years
LocalDate today = LocalDate.now();
LocalDate tomorrow = today.plus(1, ChronoUnit.DAYS);
LocalDate yesterday = tomorrow.minusDays(2);

LocalDate independenceDay = LocalDate.of(2002, Month.JULY, 4);
DayOfWeek dayOfWeek = independenceDay.getDayOfWeek();
System.out.println(dayOfWeek);    // FRIDAY

// parsing a LocalDate from a string is just as simple as parsing a LocalTime:

DateTimeFormatter germanFormatter = DateTimeFormatter.ofLocalizedDate(FormatStyle.MEDIUM).withLocale(Locale.GERMAN);

LocalDate xmas = LocalDate.parse("24.12.2014", germanFormatter);
System.out.println(xmas);   // 2014-12-24


LocalDateTime#

// LocalDateTime represents a datetime that combines date and time

LocalDateTime is immutable and works similar to LocalTime and LocalDate. We can utilize methods for retrieving certain fields from a date-time:

LocalDateTime sylvester = LocalDateTime.of(2014, Month.DECEMBER, 31, 23, 59, 59);

DayOfWeek dayOfWeek = sylvester.getDayOfWeek();
System.out.println(dayOfWeek);      // WEDNESDAY

Month month = sylvester.getMonth();
System.out.println(month);          // DECEMBER

long minuteOfDay = sylvester.getLong(ChronoField.MINUTE_OF_DAY);
System.out.println(minuteOfDay);    // 1439
With the additional information of a timezone it can be converted to an instant. Instants can easily be converted to legacy dates of type java.util.Date.

Instant instant = sylvester
        .atZone(ZoneId.systemDefault())
        .toInstant();

Date legacyDate = Date.from(instant);
System.out.println(legacyDate);     // Wed Dec 31 23:59:59 CET 2014
Formatting date-times works just like formatting dates or times. Instead of using pre-defined formats we can create formatters from custom patterns.

DateTimeFormatter formatter =
    DateTimeFormatter
        .ofPattern("MMM dd, yyyy - HH:mm");

LocalDateTime parsed = LocalDateTime.parse("Nov 03, 2014 - 07:13", formatter);
String string = formatter.format(parsed);
System.out.println(string);     // Nov 03, 2014 - 07:13
Unlike java.text.NumberFormat the new DateTimeFormatter is immutable and thread-safe.


===


The java.time packge includes many new classes to help you: LocalDate, LocalTime, LocalDateTime,
Instant, Duration, and Period.
LocalDate, LocalTime, Instant, Duration, and Period 275

12.1.1 Working with LocalDate and LocalTime
The class LocalDate is probably the first one you’ll come across when you start using
the new Date and Time API. An instance of this class is an immutable object representing just a plain date without the time of day. In particular, it doesn’t carry any information about the time zone.
 You can create a LocalDate instance using the of static factory method. A LocalDate instance provides many methods to read its most commonly used values such as
year, month, day of the week, and so on, as shown in the listing that follows.
LocalDate date = LocalDate.of(2014, 3, 18);
int year = date.getYear();
Month month = date.getMonth();
int day = date.getDayOfMonth();
DayOfWeek dow = date.getDayOfWeek();
int len = date.lengthOfMonth();
boolean leap = date.isLeapYear();
It’s also possible to obtains the current date from the system clock using the now factory method:
LocalDate today = LocalDate.now();
All the other date-time classes we’ll investigate in the remaining part of this chapter
provide a similar factory method. You can also access the same information by passing
a TemporalField to the get method. The TemporalField is an interface defining how
to access the value of a specific field of a temporal object. The ChronoField enumeration implements this interface, so you can conveniently use an element of that enumeration with the get method, as shown in the next listing.
int year = date.get(ChronoField.YEAR);
int month = date.get(ChronoField.MONTH_OF_YEAR);
int day = date.get(ChronoField.DAY_OF_MONTH);
Similarly, the time of day, such as 13:45:20, is represented by the LocalTime class. You
can create instances of LocalTime using two overloaded static factory methods named
of. The first one accepts an hour and a minute and the second one also accepts a second. Just like the LocalDate class, the LocalTime class provides some getter methods
to access its values, as shown in the following listing.
LocalTime time = LocalTime.of(13, 45, 20);
int hour = time.getHour();
int minute = time.getMinute();
int second = time.getSecond();
Listing 12.1 Creating a LocalDate and reading its values
Listing 12.2 Reading LocalDate values using a TemporalField
Listing 12.3 Creating a LocalTime and reading its values
2014-03-18
2014
MARCH
18
TUESDAY
31 (days
false (not a in March)
leap year)
13:45:20
13
45
20
276 CHAPTER 12 New Date and Time API
Both LocalDate and LocalTime can be created by parsing a String representing
them. You can achieve this using their parse static methods:
LocalDate date = LocalDate.parse("2014-03-18");
LocalTime time = LocalTime.parse("13:45:20");
It’s possible to pass a DateTimeFormatter to the parse method. An instance of this class
specifies how to format a date and/or a time object. It’s intended as a replacement for
the old java.util.DateFormat that we mentioned earlier. We show in more detail how
you can use a DateTimeFormatter in section 12.2. Also note that these parse methods
both throw a DateTimeParseException, which extends RuntimeException in case the
String argument can’t be parsed as a valid LocalDate or LocalTime.
12.1.2 Combining a date and a time
The composite class called LocalDateTime pairs a LocalDate and a LocalTime. It represents both a date and a time, without a time zone, and can be created either directly
or by combining a date and time, as shown in the next listing.
// 2014-03-18T13:45:20
LocalDateTime dt1 = LocalDateTime.of(2014, Month.MARCH, 18, 13, 45, 20);
LocalDateTime dt2 = LocalDateTime.of(date, time);
LocalDateTime dt3 = date.atTime(13, 45, 20);
LocalDateTime dt4 = date.atTime(time);
LocalDateTime dt5 = time.atDate(date);
Note that it’s possible to create a LocalDateTime by passing a time to a LocalDate, or
conversely a date to a LocalTime, using respectively their atTime or atDate methods.
You can also extract the LocalDate or LocalTime component from a LocalDateTime
using the toLocalDate and toLocalTime methods:
LocalDate date1 = dt1.toLocalDate();
LocalTime time1 = dt1.toLocalTime();
12.1.3 Instant: a date and time for machines
As humans we’re used to thinking of dates and time in terms of weeks, days, hours,
and minutes. Nonetheless, this representation isn’t easy for a computer to deal with.
From a machine point of view, the most natural format to model time is with a single
large number representing a point on a continuous timeline. This is the approach
used by the new java.time.Instant class, which basically represents the number of
seconds passed since the Unix epoch time, set by convention to midnight of January 1,
1970 UTC.
 You can create an instance of this class by passing the number of seconds to its
ofEpochSecond static factory method. In addition, the Instant class supports nanosecond precision. There’s a supplementary overloaded version of the ofEpochSecond static
factory method that accepts a second argument that’s a nanosecond adjustment to the
passed number of seconds. This overloaded version adjusts the nanosecond argument,
ensuring that the stored nanosecond fraction is between 0 and 999,999,999. This means
all the following invocations of the ofEpochSecond factory method will return exactly
the same Instant:
Instant.ofEpochSecond(3);
Instant.ofEpochSecond(3, 0);
Instant.ofEpochSecond(2, 1_000_000_000);
Instant.ofEpochSecond(4, -1_000_000_000);
As you’ve already seen for the LocalDate and the other human-readable date-time
classes, the Instant class also supports another static factory method named now,
which allows you to capture a timestamp of the current moment. It’s important to
reinforce that an Instant is intended for use only by a machine. It consists of a number of seconds and nanoseconds. As a consequence, it doesn’t provide any ability to
handle units of time that are meaningful to humans. For example, this statement
int day = Instant.now().get(ChronoField.DAY_OF_MONTH);
will just throw an exception like
java.time.temporal.UnsupportedTemporalTypeException: Unsupported field:
DayOfMonth
But you can work with Instants by using the Duration and Period classes, which we
look at next.
12.1.4 Defining a Duration or a Period
All the classes you’ve seen so far implement the Temporal interface, which defines
how to read and manipulate the values of an object modeling a generic point in time.
We’ve shown you a few ways to create different Temporal instances. The next natural
step is to create a duration between two temporal objects. The between static factory
method of the Duration class serves exactly this purpose. You can create a duration
between two LocalTimes, two LocalDateTimes, or two Instants as follows:
Duration d1 = Duration.between(time1, time2);
Duration d1 = Duration.between(dateTime1, dateTime2);
Duration d2 = Duration.between(instant1, instant2);
Because LocalDateTime and Instant are made for different purposes, one to be used
by humans and the other by machines, you’re not allowed to mix them. If you try to
create a duration between them, you’ll only obtain a DateTimeException. Moreover,
because the Duration class is used to represent an amount of time measured in seconds and eventually nanoseconds, you can’t pass a LocalDate to the between method.
 When you need to model an amount of time in terms of years, months, and days,
you can use the Period class. You can find out the difference between two LocalDates
with the between factory method of that class:
Period tenDays = Period.between(LocalDate.of(2014, 3, 8),
 LocalDate.of(2014, 3, 18));
One billion nanoseconds
(1 second) after 2 seconds
One billion nanoseconds
(1 second) before 4 seconds
278 CHAPTER 12 New Date and Time API
Finally, both the Duration and Period classes have other convenient factory methods
to create instances of them directly, in other words, without defining them as the difference between two temporal objects, as shown in the next listing.
Duration threeMinutes = Duration.ofMinutes(3);
Duration threeMinutes = Duration.of(3, ChronoUnit.MINUTES);
Period tenDays = Period.ofDays(10);
Period threeWeeks = Period.ofWeeks(3);
Period twoYearsSixMonthsOneDay = Period.of(2, 6, 1);
Both the Duration and Period classes share many similar methods, and table 12.1
lists these.
All the classes we’ve investigated so far are immutable, and this is a great design choice
to allow a more functional programming style, ensure thread-safety, and preserve the
consistency of the domain model. Nevertheless, the new Date and Time API offers
some handy methods to create a modified version of those objects. For example, you
may wish to add three days to an existing LocalDate instance. We explore how to do
this in the next section. In addition, we explore how to create a date-time formatter
from a given pattern, such as dd/MM/yyyy, or even programmatically, and how to use
this formatter for both parsing and printing a date.
Listing 12.5 Creating Durations and Periods
Table 12.1 The common methods of date-time classes representing an interval
Method Static Description
between Yes Creates an interval between two points in time
from Yes Creates an interval from a temporal unit
of Yes Creates an instance of this interval from its constituent parts
parse Yes Creates an instance of this interval from a String
addTo No Creates a copy of this interval adding to it the specified temporal object
get No Reads part of the state of this interval
isNegative No Checks if this interval is negative, excluding zero
isZero No Checks if this interval is zero length
minus No Creates a copy of this interval with an amount of time subtracted
multipliedBy No Creates a copy of this interval multiplied by the given scalar
negated No Creates a copy of this interval with the length negated
plus No Creates a copy of this interval with an amount of time added
subtractFrom No Subtracts this interval from the specified temporal object
Manipulating, parsing, and formatting dates 279
12.2 Manipulating, parsing, and formatting dates
The most immediate and easiest way to create a modified version of an existing
LocalDate is changing one of its attributes, using one of its withAttribute methods.
Note that all the methods return a new object with the modified attribute, as shown in
the following listing. They don’t mutate the existing object!
LocalDate date1 = LocalDate.of(2014, 3, 18);
LocalDate date2 = date1.withYear(2011);
LocalDate date3 = date2.withDayOfMonth(25);
LocalDate date4 = date3.with(ChronoField.MONTH_OF_YEAR, 9);
You can also do this with the more generic with method, taking a TemporalField as
first argument, as in the last statement of listing 12.6. This last with method is the
dual of the get method used in listing 12.2. Both of these methods are declared in
the Temporal interface implemented by all the classes of the Date and Time API,
which define a single point in time such as LocalDate, LocalTime, LocalDateTime,
and Instant. More precisely, the get and with methods let you respectively read
and modify the value of a field of a Temporal object. They throw an UnsupportedTemporalTypeException if the requested field isn’t supported by the specific Temporal,
for example, a ChronoField.MONTH_OF_YEAR on an Instant or a ChronoField.NANO
_OF_SECOND on a LocalDate.
 It’s even possible to manipulate a LocalDate in a declarative manner. For example,
you can add or subtract a given amount of time, as shown in the next listing.
LocalDate date1 = LocalDate.of(2014, 3, 18);
LocalDate date2 = date1.plusWeeks(1);
LocalDate date3 = date2.minusYears(3);
LocalDate date4 = date3.plus(6, ChronoUnit.MONTHS);
Similarly to what we’ve explained about the with and get methods, the generic
plus method used in the last statement of listing 12.7, together with the analogous
minus method, is declared in the Temporal interface. These methods allow you to
move a Temporal back or forward a given amount of time, defined by a number plus a
TemporalUnit, where the ChronoUnit enumeration offers a convenient implementation of the TemporalUnit interface.
 As you may have anticipated, all the date-time classes representing a point in time
like LocalDate, LocalTime, LocalDateTime, and Instant have many methods in common; table 12.2 summarizes these.

Table 12.2 The common methods of date-time classes representing a point in time
Method Static Description
from Yes Creates an instance of this class from the passed temporal object
now Yes Creates a temporal object from the system clock
of Yes Creates an instance of this temporal object from its constituent parts
parse Yes Creates an instance of this temporal object from a String
atOffset No Combines this temporal object with a zone offset
atZone No Combines this temporal object with a time zone
format No Converts this temporal object into a String using the specified formatter
(not available for Instant)
get No Reads part of the state of this temporal object
minus No Creates a copy of this temporal object with an amount of time subtracted
plus No Creates a copy of this temporal object with an amount of time added
with No Creates a copy of this temporal object with part of the state changed
Quiz 12.1: Manipulating a LocalDate
What will the value of the date variable be after the following manipulations?
LocalDate date = LocalDate.of(2014, 3, 18);
date = date.with(ChronoField.MONTH_OF_YEAR, 9);
date = date.plusYears(2).minusDays(10);
date.withYear(2011);
Answer:
2016-09-08
As you’ve seen, you can manipulate the date both in an absolute way and in a relative
way. You can also concatenate more manipulations in a single statement, because
every change will create a new LocalDate object, and the subsequent invocation will
manipulate the object created by the former one. Finally, the last statement in this
code snippet has no observable effect because, as usual, it creates a new LocalDate instance, but we’re not assigning this new value to any variable.
Manipulating, parsing, and formatting dates 281
you can pass to an overloaded version of the with method a TemporalAdjuster that
provides a more customizable way to define the manipulation needed to operate on a
specific date. The Date and Time API already provides many predefined TemporalAdjusters for the most common use cases. You can access them using the static factory methods contained in the TemporalAdjusters class, as shown next.
import static java.time.temporal.TemporalAdjusters.*;
LocalDate date1 = LocalDate.of(2014, 3, 18);
LocalDate date2 = date1.with(nextOrSame(DayOfWeek.SUNDAY));
LocalDate date3 = date2.with(lastDayOfMonth());
Table 12.3 provides a list of the TemporalAdjusters that can be created with these factory methods.
As you can see, TemporalAdjusters allow you to perform more complex date manipulations that still read like the problem statement. Moreover, it’s relatively simple to create
your own custom TemporalAdjuster implementation if you can’t find a predefined
TemporalAdjuster that fits your need. In fact, the TemporalAdjuster interface declares
only a single method (this makes it a functional interface), defined as follows.
Listing 12.8 Using the predefined TemporalAdjusters
Table 12.3 The factory methods of the TemporalAdjusters class
Method Description
dayOfWeekInMonth Creates a new date in the same month with the ordinal day of week
firstDayOfMonth Creates a new date set to the first day of the current month
firstDayOfNextMonth Creates a new date set to the first day of the next month
firstDayOfNextYear Creates a new date set to the first day of the next year
firstDayOfYear Creates a new date set to the first day of the current year
firstInMonth Creates a new date in the same month with the first matching day of week
lastDayOfMonth Creates a new date set to the last day of the current month
lastDayOfNextMonth Creates a new date set to the last day of the next month
lastDayOfNextYear Creates a new date set to the last day of the next year
lastDayOfYear Creates a new date set to the last day of the current year
lastInMonth Creates a new date in the same month with the last matching day of week
next
previous
Creates a new date set to the first occurrence of the specified day of
week after/before the date being adjusted
nextOrSame
previousOrSame
Creates a new date set to the first occurrence of the specified day of
week after/before the date being adjusted unless it’s already on that day,
in which case the same object is returned
2014-03-18
2014-03-23
2014-03-31
282 CHAPTER 12 New Date and Time API
@FunctionalInterface
public interface TemporalAdjuster {
 Temporal adjustInto(Temporal temporal);
}
This means an implementation of the TemporalAdjuster interface defines how to
convert a Temporal object into another Temporal. You can think of it as being like a
UnaryOperator<Temporal>. Take a few minutes to practice what you’ve learned so far
and implement your own TemporalAdjuster in Quiz 12.2.
Listing 12.9 The TemporalAdjuster interface
Quiz 12.2: Implementing a custom TemporalAdjuster
Develop a class named NextWorkingDay, implementing the TemporalAdjuster interface
that moves a date forward by one day but skips Saturdays and Sundays. Doing the following
date = date.with(new NextWorkingDay());
should move the date to the next day, if this day is between Monday and Friday, but to the next
Monday if it’s a Saturday or a Sunday.
Answer:
You can implement the NextWorkingDay adjuster as follows:
public class NextWorkingDay implements TemporalAdjuster {
 @Override
 public Temporal adjustInto(Temporal temporal) {
 DayOfWeek dow =
 DayOfWeek.of(temporal.get(ChronoField.DAY_OF_WEEK));
 int dayToAdd = 1;
 if (dow == DayOfWeek.FRIDAY) dayToAdd = 3;
 else if (dow == DayOfWeek.SATURDAY) dayToAdd = 2;
 return temporal.plus(dayToAdd, ChronoUnit.DAYS);
 }
}
This TemporalAdjuster normally moves a date forward one day, except if today is a
Friday or Saturday, in which case it advances the dates three or two days, respectively. Note that since a TemporalAdjuster is a functional interface, you could just
pass the behavior of this adjuster in a lambda expression:
date = date.with(temporal -> {
 DayOfWeek dow =
 DayOfWeek.of(temporal.get(ChronoField.DAY_OF_WEEK));
 int dayToAdd = 1;
 if (dow == DayOfWeek.FRIDAY) dayToAdd = 3;
 else if (dow == DayOfWeek.SATURDAY) dayToAdd = 2;
 return temporal.plus(dayToAdd, ChronoUnit.DAYS);
});
Read the
current day.
Normally
add one day.
But add three
days if today
is a Friday.
Or two if it’s
a Saturday.
Return the modified date adding
the right number of days.
Manipulating, parsing, and formatting dates 283
Another common operation you may want to perform on your date and time objects
is printing them in different formats specific to your business domain. Similarly, you
may want to convert Strings representing dates in those formats into actual date
objects. In the next section, we demonstrate the mechanisms provided by the new
Date and Time API to accomplish these tasks.
12.2.2 Printing and parsing date-time objects
Formatting and parsing is another relevant feature when working with dates and
times. The new java.time.format package is entirely devoted to this purpose. The
most important class of this package is DateTimeFormatter. The easiest way to create a
formatter is through its static factory methods and constants. The constants such as
BASIC_ISO_DATE and ISO_LOCAL_DATE are just predefined instances of the DateTimeFormatter class. All DateTimeFormatters can be used to create a String representing
a given date or time in a specific format. For example, here we produce a String
using two different formatters:
LocalDate date = LocalDate.of(2014, 3, 18);
String s1 = date.format(DateTimeFormatter.BASIC_ISO_DATE);
String s2 = date.format(DateTimeFormatter.ISO_LOCAL_DATE);
You can also parse a String representing a date or a time in that format to re-create
the date object itself. You can achieve this by using the parse factory method provided
by all the classes of the Date and Time API representing a point in time or an interval:
LocalDate date1 = LocalDate.parse("20140318",
 DateTimeFormatter.BASIC_ISO_DATE);
LocalDate date2 = LocalDate.parse("2014-03-18",
 DateTimeFormatter.ISO_LOCAL_DATE);
(continued)
It’s likely that you may want to apply this manipulation to a date in several points of
your code, and for this reason we suggest encapsulating its logic in a proper class as
we did here. Do the same for all the manipulations you use frequently. You’ll end up
with a small library of adjusters you and your team could easily reuse in your codebase.
If you want to define the TemporalAdjuster with a lambda expression, it’s preferable to do it using the ofDateAdjuster static factory of the TemporalAdjusters
class that accepts a UnaryOperator<LocalDate> as follows:
TemporalAdjuster nextWorkingDay = TemporalAdjusters.ofDateAdjuster(
 temporal -> {
 DayOfWeek dow =
 DayOfWeek.of(temporal.get(ChronoField.DAY_OF_WEEK));
 int dayToAdd = 1;
 if (dow == DayOfWeek.FRIDAY) dayToAdd = 3;
 if (dow == DayOfWeek.SATURDAY) dayToAdd = 2;
 return temporal.plus(dayToAdd, ChronoUnit.DAYS);
 });
date = date.with(nextWorkingDay);
20140318
2014-03-18
284 CHAPTER 12 New Date and Time API
In comparison to the old java.util.DateFormat class, all the DateTimeFormatter
instances are thread-safe. Therefore, you can create singleton formatters, like the
ones defined by the DateTimeFormatter constants, and share them among multiple
threads. The DateTimeFormatter class also supports a static factory method that lets
you create a formatter from a specific pattern, as shown in the next listing.
DateTimeFormatter formatter = DateTimeFormatter.ofPattern("dd/MM/yyyy");
LocalDate date1 = LocalDate.of(2014, 3, 18);
String formattedDate = date1.format(formatter);
LocalDate date2 = LocalDate.parse(formattedDate, formatter);
Here the LocalDate’s format method produces a String representing the date with
the requested pattern. Next, the static parse method re-creates the same date by parsing the generated String using the same formatter. The ofPattern method also has
an overloaded version allowing you to create a formatter for a given Locale, as shown
in the following listing.
DateTimeFormatter italianFormatter =
 DateTimeFormatter.ofPattern("d. MMMM yyyy", Locale.ITALIAN);
LocalDate date1 = LocalDate.of(2014, 3, 18);
String formattedDate = date.format(italianFormatter); // 18. marzo 2014
LocalDate date2 = LocalDate.parse(formattedDate, italianFormatter);
Finally, in case you need even more control, the DateTimeFormatterBuilder class lets
you define complex formatters step by step using meaningful methods. In addition, it
provides you with the ability to have case-insensitive parsing, lenient parsing (allowing
the parser to use heuristics to interpret inputs that don’t precisely match the specified
format), padding, and optional sections of the formatter. For example, you can programmatically build the same italianFormatter we used in listing 12.11 through the
DateTimeFormatterBuilder as follows.
DateTimeFormatter italianFormatter = new DateTimeFormatterBuilder()
 .appendText(ChronoField.DAY_OF_MONTH)
 .appendLiteral(". ")
 .appendText(ChronoField.MONTH_OF_YEAR)
 .appendLiteral(" ")
 .appendText(ChronoField.YEAR)
 .parseCaseInsensitive()
 .toFormatter(Locale.ITALIAN);
So far you’ve learned how to create, manipulate, format, and parse both points in
time and intervals. But you haven’t seen how to deal with subtleties involving dates
and time. For example, you may need to deal with different time zones or work with
alternative calendar systems. In the next sections, we explore these topics using the
new Date and Time API.

 Working with different time zones and calendars
None of the classes you’ve seen so far contained any information about time zones.
Dealing with time zones is another important issue that’s been vastly simplified by the
new Date and Time API. The new java.time.ZoneId class is the replacement for the old
java.util.TimeZone class. It aims to better shield you from the complexities related
to time zones, such as dealing with Daylight Saving Time (DST). Like the other classes
of the Date and Time API, it’s immutable.
 A time zone is a set of rules corresponding to a region in which the standard time
is the same. There are about 40 of them held in instances of the ZoneRules class. You
can simply call getRules() on a ZoneId to obtain the rules for that given time zone. A
specific ZoneId is identified by a region ID, for example:
ZoneId romeZone = ZoneId.of("Europe/Rome");
The region IDs are all in the format “{area}/{city}” and the set of available locations is
the one supplied by the IANA Time Zone Database. You can also convert an old TimeZone object to a ZoneId by using the new method toZoneId:
ZoneId zoneId = TimeZone.getDefault().toZoneId();
Once you have a ZoneId object, you can combine it with a LocalDate, a LocalDateTime, or an Instant, to transform it into ZonedDateTime instances, which represent
points in time relative to the specified time zone, as shown in the next listing.
LocalDate date = LocalDate.of(2014, Month.MARCH, 18);
ZonedDateTime zdt1 = date.atStartOfDay(romeZone);
LocalDateTime dateTime = LocalDateTime.of(2014, Month.MARCH, 18, 13, 45);
ZonedDateTime zdt2 = dateTime.atZone(romeZone);
Instant instant = Instant.now();
ZonedDateTime zdt3 = instant.atZone(romeZone);
Figure 12.1 illustrates the components of a ZonedDateTime to help you understand
the differences between LocaleDate, LocalTime, LocalDateTime, and ZoneId.
 You can also convert a LocalDateTime to an Instant by using a ZoneId:
LocalDateTime dateTime = LocalDateTime.of(2014, Month.MARCH, 18, 13, 45);
Instant instantFromDateTime = dateTime.toInstant(romeZone);
Listing 12.13 Applying a time zone to a point in time
2014-05-14T15:33:05.941+01:00[Europe/London]
LocalDate
LocateDateTime
ZonedDateTime
LocalTime ZoneId
Figure 12.1 Making sense of a ZonedDateTime
286 CHAPTER 12 New Date and Time API
Or you can do it the other way around:
Instant instant = Instant.now();
LocalDateTime timeFromInstant = LocalDateTime.ofInstant(instant, romeZone);
12.3.1 Fixed offset from UTC/Greenwich
Another common way to express a time zone is with a fixed offset from UTC/Greenwich. For instance, you can use this notation to say that “New York is five hours
behind London.” In cases like this you can use the ZoneOffset class, a subclass of
ZoneId that represents the difference between a time and the zero meridian of Greenwich, London:
ZoneOffset newYorkOffset = ZoneOffset.of("-05:00");
The -05:00 offset indeed corresponds to the US Eastern Standard Time. Be aware that
a ZoneOffset defined in this way doesn’t have any Daylight Saving Time management,
and for this reason it isn’t suggested in the majority of cases. Because a ZoneOffset is
also a ZoneId, you can use it as shown in listing 12.13. You can also create an OffsetDateTime, which represents a date-time with an offset from UTC/Greenwich in the
ISO-8601 calendar system:
LocalDateTime dateTime = LocalDateTime.of(2014, Month.MARCH, 18, 13, 45);
OffsetDateTime dateTimeInNewYork = OffsetDateTime.of(date, newYorkOffset);
Another advanced feature supported by the new Date and Time API is support for
non-ISO calendaring systems.
12.3.2 Using alternative calendar systems
The ISO-8601 calendar system is the de facto world civil calendar system. But four
additional calendar systems are provided in Java 8. Each of these calendar systems has
a dedicated date class: ThaiBuddhistDate, MinguoDate, JapaneseDate, and HijrahDate. All these classes together with LocalDate implement the ChronoLocalDate
interface intended to model a date in an arbitrary chronology. You can create an
instance of one of these classes out of a LocalDate. More generally, you can create any
other Temporal instance using their from static factory methods as follows:
LocalDate date = LocalDate.of(2014, Month.MARCH, 18);
JapaneseDate japaneseDate = JapaneseDate.from(date);
Alternatively, you can explicitly create a calendar system for a specific Locale and create an instance of a date for that Locale. In the new Date and Time API, the Chronology
interface models a calendar system, and you can obtain an instance of it using its
ofLocale static factory method:
Chronology japaneseChronology = Chronology.ofLocale(Locale.JAPAN);
ChronoLocalDate now = japaneseChronology.dateNow(); 
Summary 287
The designers of the Date and Time API advise using LocalDate instead of ChronoLocalDate for most cases; this is because a developer could make assumptions in
their code that unfortunately aren’t true in a multicalendar system. Such assumptions might include that the value of a day or month will never be higher than 31,
that a year contains 12 months, or even that a year has a fixed number of months.
For these reasons, it’s recommended to use LocalDate throughout your application,
including all storage, manipulation, and interpretation of business rules, whereas
you should employ ChronoLocalDate only when you need to localize the input or
output of your program.
ISLAMIC CALENDAR
Out of the new calendars added to Java 8, the HijrahDate (Islamic calendar) seems to
be the most complex because it can have variants. The Hijrah calendar system is based
on lunar months. There are a variety of methods to determine a new month, such as a
new moon that could be visible anywhere in the world or that must be visible first in
Saudi Arabia. The withVariant method is used to choose the desired variant. Java 8
has included the Umm Al-Qura variant for HijrahDate as standard.
 The following code illustrates an example of displaying the start and end dates of
Ramadan for the current Islamic year in ISO date:
HijrahDate ramadanDate =
 HijrahDate.now().with(ChronoField.DAY_OF_MONTH, 1)
 .with(ChronoField.MONTH_OF_YEAR, 9);
System.out.println("Ramadan starts on " +
 IsoChronology.INSTANCE.date(ramadanDate) +
 " and ends on " +
 IsoChronology.INSTANCE.date(
 ramadanDate.with(
 TemporalAdjusters.lastDayOfMonth())));
12.4 Summary
In this chapter, you’ve learned the following:
■ The old java.util.Date class and all other classes used to model date and time
in Java before Java 8 have many inconsistencies and design flaws, including
their mutability and some poorly chosen offsets, defaults, and naming.
■ The date-time objects of the new Date and Time API are all immutable.
■ This new API provides two different time representations to manage the different needs of humans and machines when operating on it.
■ You can manipulate date and time objects in both an absolute and relative manner, and the result of these manipulations is always a new instance, leaving the
original one unchanged.
■ TemporalAdjusters allow you to manipulate a date in a more complex way than
just changing one of its values, and you can define and use your own custom
date transformations.
Get current Hijrah
date; then change it
to have the first day
of Ramadan, which is
the 9th month.
IsoChronology.INSTANCE
is a static instance of the
IsoChronology class.
Ramadan starts on
2014-06-28 and
ends on 2014-07-27.
288 CHAPTER 12 New Date and Time API
■ You can define a formatter to both print and parse date-time objects in a specific format. These formatters can be created from a pattern or programmatically and they’re all thread-safe.
■ You can represent a time zone, both relative to a specific region/location and as
a fixed offset from UTC/Greenwich, and apply it to a date-time object in order
to localize it.
■ You can use calendar systems different from the ISO-8601 standard system.

