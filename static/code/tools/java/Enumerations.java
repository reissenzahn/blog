
// an enumeration is a type that has a fixed set of constant values
enum AlertState {

  LOW("green"),
  MEDIUM("orange"),
  HIGH("red"),
  UNKNOWN;

  // enumerations can have fields, constructors and methods
  private final String color;

  Level(String color) {
    this.color = color;
  }

  public String getColor() {
    return this.color;
  }

  public static Level findByColor(String color) {
    for (Level value: values()) {
      if (color.equals(value.color)) {
        return value;
      }
    }
    return null;
  }
}


Level[] levels = Level.values();
System.out.println(Arrays.toString(levels));  // 

Level low = Level.LOW;
System.out.println(low.name());     // 
System.out.println(low.ordinal());  //

Level medium = Level.valueOf("MEDIUM");
System.out.println(medium.equals(Level.MEDIUM);  // true
System.out.println(medium == Level.MEDIUM);      // true

Level high = Level.HIGH;
System.out.println(high.getColor());           // 
System.out.println(Level.findByColor("red"));  // LOW


// enumerations can be used with switch statements
Level level = Level.HIGH;

switch (level) {
  case LOW:
    System.out.println(LOW.getColor());)]
    break;
  case MEDIUM:
    System.out.println(LOW.getColor());)]
    break;
  case HIGH:
    System.out.println(LOW.getColor());)]
    break;
}




// enumerations may not be generic and cannot be subclassed


// 
// enum Direction {
//   MONDAY,
//   TUESDAY,
//   WEDNESDAY,
//   THURSDAY,
//   FRIDAY,
//   SATURDAY,
//   SUNDAY;
// }

// Day day = Day.WEDNESDAY;

// switch (day) {
//   case MONDAY: case FRIDAY: case SUNDAY:
//     System.out.println("6");
//     break;
//   case TUESDAY:
//     System.out.println("7");
//     break;
//   case THURSDAY: case SATURDAY:
//     System.out.println("8");
//     break;
//   case WEDNESDAY:
//     System.out.println("9");
//     break;
//   default:
//     throw new UnexpectedDayException(day);
// }