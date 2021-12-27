

class Light {

  public void on() {
    System.out.println("Light on.");
  }

  public void off() {
    System.out.println("Light off.");
  }
}


interface Command {
  public void execute();
}

class LightOnCommand implements Command {

  private Light light;

  public LightOnCommand(Light light) {
    this.light = light;
  }

  public void execute() {
    light.on();
  }
}

class LightOffCommand implements Command {
  Light light;

  public LightOffCommand(Light light) {
    this.light = light;
  }

  public void execute() {
    light.off();
  }
}

public class StereoOnWithCDCommand implements Command {
 Stereo stereo;
 public StereoOnWithCDCommand(Stereo stereo) {
 this.stereo = stereo;
 }
 public void execute() {
 stereo.on();
 stereo.setCD();
 stereo.setVolume(11);
 }
}

class RemoteControl {
  Command[] onCommands;
  Command[] offCommands;

  public RemoteControl() {
    onCommands = new Command[7];
    offCommands = new Command[7];

    Command noCommand = new NoCommand();

    for (int i = 0; i < 7; i++) {
      onCommands[i] = noCommand;
      offCommands[i] = noCommand;
    }
  }

  void setCommand(int slot, Command onCommand, Command offCommand) {
    onCommands[slot] = onCommand;
    offCommands[slot] = offCommand;
  }

  // when an On or Off button is pressed, the hardware takes care of calling the corresponding methods onButtonWasPushed() or offButtonWasPushed()
  void onButtonWasPushed(int slot) {
    onCommands[slot].execute();
  }



  public String toString() {
    StringBuffer sb = new StringBuffer();
    
    sb.append("\n------ Remote Control -------\n");
    
    for (int i = 0; i < onCommands.length; i++) {
      sb.append("[slot " + i + "] " + onCommands[i].getClass().getName() + " " + offCommands[i].getClass().getName() + "\n");
    }

    return sb.toString();
  }
}


public class CommandPattern {

  public static void main(String[] args) {


    // The remote is our Invoker; it will be passed a command object that can be used to make requests.
    SimpleRemoteControl remote = new SimpleRemoteControl();

    // Now we create a Light object. This will be the Receiver of the request
    Light light = new Light();

    // Here, create a command and pass the Receiver to it.
    LightOnCommand lightOn = new LightOnCommand(light);

    // Here, pass the command to the Invoker.
    remote.setCommand(lightOn);

     remote.buttonWasPressed();


     RemoteControl remoteControl = new RemoteControl();
 Light livingRoomLight = new Light("Living Room");
 Light kitchenLight = new Light("Kitchen");
 CeilingFan ceilingFan = new CeilingFan("Living Room");
 GarageDoor garageDoor = new GarageDoor("Garage");
 Stereo stereo = new Stereo("Living Room");

 LightOnCommand livingRoomLightOn =
 new LightOnCommand(livingRoomLight);
 LightOffCommand livingRoomLightOff =
 new LightOffCommand(livingRoomLight);
 LightOnCommand kitchenLightOn =
 new LightOnCommand(kitchenLight);
 LightOffCommand kitchenLightOff =
 new LightOffCommand(kitchenLight);

 CeilingFanOnCommand ceilingFanOn =
 new CeilingFanOnCommand(ceilingFan);
 CeilingFanOffCommand ceilingFanOff =
 new CeilingFanOffCommand(ceilingFan);
 GarageDoorUpCommand garageDoorUp =
 new GarageDoorUpCommand(garageDoor);
 GarageDoorDownCommand garageDoorDown =
 new GarageDoorDownCommand(garageDoor);
 StereoOnWithCDCommand stereoOnWithCD =
 new StereoOnWithCDCommand(stereo);
 StereoOffCommand stereoOff =
 new StereoOffCommand(stereo);
  }
}
