Example

// the FrameBuffer class provides an operation for setting individual pixels in a buffer
class Framebuffer
{
private:
  static const int WIDTH = 160;
  static const int HEIGHT = 120;

  char pixels[WIDTH * HEIGHT];

public:
  Framebuffer()
  {
    clear();
  }

  void clear()
  {
    for (int i = 0; i < WIDTH * HEIGHT; i++)
    {
      pixels[i] = WHITE;
    }
  }

  void draw(int x, int y)
  {
    pixels[(WIDTH * y) + x] = BLACK;
  }

  // the video driver will call getPixels() frequently to stream memory from the buffer onto the screen
  const char* getPixels()
  {
    return pixels;
  }
};


// the Scene class wraps a FrameBuffer object and renders an image to the buffer by setting individual pixels
class Scene
{
private:
  Framebuffer buffer;

public:

  // every frame, the game tells the scene to draw
  void draw()
  {
    buffer.clear();

    buffer.draw(1, 1);
    buffer.draw(4, 1);
    buffer.draw(1, 3);
    buffer.draw(2, 4);
    buffer.draw(3, 4);
    buffer.draw(4, 3);
  }

  Framebuffer& getBuffer() {
    return buffer;
  }
};


The trouble is that the video driver can call getPixels() on the buffer at any point in time, even here:

buffer_.draw(1, 1);
buffer_.draw(4, 1);
// <- Video driver reads pixels here!
buffer_.draw(1, 3);
buffer_.draw(2, 4);
buffer_.draw(3, 4);
buffer_.draw(4, 3);


When that happens, the user will see the eyes of the face, but the mouth will disappear for a single frame. In the next frame, it could get interrupted at some other point.


class Scene
{
public:
  Scene()
  : current_(&buffers_[0]),
    next_(&buffers_[1])
  {}

  void draw()
  {
    next_->clear();

    buffer.draw(1, 1);
    buffer.draw(4, 1);
    buffer.draw(1, 3);
    buffer.draw(2, 4);
    buffer.draw(3, 4);
    buffer.draw(4, 3);

    swap();
  }

  Framebuffer& getBuffer() { return *current_; }

private:
  void swap()
  {
    // Just switch the pointers.
    Framebuffer* temp = current_;
    current_ = next_;
    next_ = temp;
  }

  Framebuffer  buffers_[2];
  Framebuffer* current_;
  Framebuffer* next_;
};


Now Scene has two buffers, stored in the buffers_ array. We don’t directly reference them from the array. Instead, there are two members, next_ and current_, that point into the array. When we draw, we draw onto the next buffer, referenced by next_. When the video driver needs to get the pixels, it always accesses the other buffer through current_.

This way, the video driver never sees the buffer that we’re working on. The only remaining piece of the puzzle is the call to swap() when the scene is done drawing the frame. That swaps the two buffers by simply switching the next_ and current_ references. The next time the video driver calls getBuffer(), it will get the new buffer we just finished drawing and put our recently drawn buffer on screen. No more tearing or unsightly glitches.



The core problem that double buffering solves is state being accessed while it’s being modified. There are two common causes of this. We’ve covered the first one with our graphics example — the state is directly accessed from code on another thread or interrupt.

There is another equally common cause, though: when the code doing the modification is accessing the same state that it’s modifying. This can manifest in a variety of places, especially physics and AI where you have entities interacting with each other. Double-buffering is often helpful here too.



EXAMPLE 2

Artificial unintelligence
Let’s say we’re building the behavioral system for, of all things, a game based on slapstick comedy. The game has a stage containing a bunch of actors that run around and get up to various hijinks and shenanigans. Here’s our base actor:

class Actor
{
public:
  Actor() : slapped_(false) {}

  virtual ~Actor() {}
  virtual void update() = 0;

  void reset()      { slapped_ = false; }
  void slap()       { slapped_ = true; }
  bool wasSlapped() { return slapped_; }

private:
  bool slapped_;
};
Every frame, the game is responsible for calling update() on the actor so that it has a chance to do some processing. Critically, from the user’s perspective, all actors should appear to update simultaneously.

This is an example of the Update Method pattern.

Actors can also interact with each other, if by “interacting”, we mean “they can slap each other around”. When updating, the actor can call slap() on another actor to slap it and call wasSlapped() to determine if it has been slapped.

The actors need a stage where they can interact, so let’s build that:

class Stage
{
public:
  void add(Actor* actor, int index)
  {
    actors_[index] = actor;
  }

  void update()
  {
    for (int i = 0; i < NUM_ACTORS; i++)
    {
      actors_[i]->update();
      actors_[i]->reset();
    }
  }

private:
  static const int NUM_ACTORS = 3;

  Actor* actors_[NUM_ACTORS];
};
Stage lets us add actors, and provides a single update() call that updates each actor. To the user, actors appear to move simultaneously, but internally, they are updated one at a time.

The only other point to note is that each actor’s “slapped” state is cleared immediately after updating. This is so that an actor only responds to a given slap once.

To get things going, let’s define a concrete actor subclass. Our comedian here is pretty simple. He faces a single actor. Whenever he gets slapped — by anyone — he responds by slapping the actor he faces.

class Comedian : public Actor
{
public:
  void face(Actor* actor) { facing_ = actor; }

  virtual void update()
  {
    if (wasSlapped()) facing_->slap();
  }

private:
  Actor* facing_;
};
Now, let’s throw some comedians on a stage and see what happens. We’ll set up three comedians, each facing the next. The last one will face the first, in a big circle:

Stage stage;

Comedian* harry = new Comedian();
Comedian* baldy = new Comedian();
Comedian* chump = new Comedian();

harry->face(baldy);
baldy->face(chump);
chump->face(harry);

stage.add(harry, 0);
stage.add(baldy, 1);
stage.add(chump, 2);
The resulting stage is set up as shown in the following image. The arrows show who the actors are facing, and the numbers show their index in the stage’s array.

Boxes for Harry, Baldy, and Chump, in that order. Harry has an arrow pointing to Baldy, who has an arrow pointing to Chump, who has an arrow pointing back to Harry.

We’ll slap Harry to get things going and see what happens when we start processing:

harry->slap();

stage.update();
Remember that the update() function in Stage updates each actor in turn, so if we step through the code, we’ll find that the following occurs:

Stage updates actor 0 (Harry)
  Harry was slapped, so he slaps Baldy
Stage updates actor 1 (Baldy)
  Baldy was slapped, so he slaps Chump
Stage updates actor 2 (Chump)
  Chump was slapped, so he slaps Harry
Stage update ends
In a single frame, our initial slap on Harry has propagated through all of the comedians. Now, to mix things up a bit, let’s say we reorder the comedians within the stage’s array but leave them facing each other the same way.

The same boxes as before with the same arrows, but now they are ordered Chump, Baldy, Harry.

We’ll leave the rest of the stage setup alone, but we’ll replace the chunk of code where we add the actors to the stage with this:

stage.add(harry, 2);
stage.add(baldy, 1);
stage.add(chump, 0);
Let’s see what happens when we run our experiment again:

Stage updates actor 0 (Chump)
  Chump was not slapped, so he does nothing
Stage updates actor 1 (Baldy)
  Baldy was not slapped, so he does nothing
Stage updates actor 2 (Harry)
  Harry was slapped, so he slaps Baldy
Stage update ends
Uh, oh. Totally different. The problem is straightforward. When we update the actors, we modify their “slapped” states, the exact same state we also read during the update. Because of this, changes to that state early in the update affect later parts of that same update step.

If you continue to update the stage, you’ll see the slaps gradually cascade through the actors, one per frame. In the first frame, Harry slaps Baldy. In the next frame, Baldy slaps Chump, and so on.

The ultimate result is that an actor may respond to being slapped in either the same frame as the slap or in the next frame based entirely on how the two actors happen to be ordered on the stage. This violates our requirement that actors need to appear to run in parallel — the order that they update within a single frame shouldn’t matter.

Buffered slaps
Fortunately, our Double Buffer pattern can help. This time, instead of having two copies of a monolithic “buffer” object, we’ll be buffering at a much finer granularity: each actor’s “slapped” state:

class Actor
{
public:
  Actor() : currentSlapped_(false) {}

  virtual ~Actor() {}
  virtual void update() = 0;

  void swap()
  {
    // Swap the buffer.
    currentSlapped_ = nextSlapped_;

    // Clear the new "next" buffer.
    nextSlapped_ = false;
  }

  void slap()       { nextSlapped_ = true; }
  bool wasSlapped() { return currentSlapped_; }

private:
  bool currentSlapped_;
  bool nextSlapped_;
};
Instead of a single slapped_ state, each actor now has two. Just like the previous graphics example, the current state is used for reading, and the next state is used for writing.

The reset() function has been replaced with swap(). Now, right before clearing the swap state, it copies the next state into the current one, making it the new current state. This also requires a small change in Stage:

void Stage::update()
{
  for (int i = 0; i < NUM_ACTORS; i++)
  {
    actors_[i]->update();
  }

  for (int i = 0; i < NUM_ACTORS; i++)
  {
    actors_[i]->swap();
  }
}
The update() function now updates all of the actors and then swaps all of their states. The end result of this is that an actor will only see a slap in the frame after it was actually slapped. This way, the actors will behave the same no matter their order in the stage’s array. As far as the user or any outside code can tell, all of the actors update simultaneously within a frame.






Design Decisions

The swap operation is the most critical step of the process since we must lock out all reading and modification of both buffers while it’s occurring.


Swap pointers or references to the buffer:

This is how our graphics example works, and it’s the most common solution for double-buffering graphics.

It’s fast. Regardless of how big the buffer is, the swap is simply a couple of pointer assignments. It’s hard to beat that for speed and simplicity.

Outside code cannot store persistent pointers to the buffer. This is the main limitation. Since we don’t actually move the data, what we’re essentially doing is periodically telling the rest of the codebase to look somewhere else for the buffer, like in our original stage analogy. This means that the rest of the codebase can’t store pointers directly to data within the buffer — they may be pointing at the wrong one a moment later.

This can be particularly troublesome on a system where the video driver expects the framebuffer to always be at a fixed location in memory. In that case, we won’t be able to use this option.

Existing data on the buffer will be from two frames ago, not the last frame. Successive frames are drawn on alternating buffers with no data copied between them, like so:

Frame 1 drawn on buffer A
Frame 2 drawn on buffer B
Frame 3 drawn on buffer A
...
You’ll note that when we go to draw the third frame, the data already on the buffer is from frame one, not the more recent second frame. In most cases, this isn’t an issue — we usually clear the whole buffer right before drawing. But if we intend to reuse some of the existing data on the buffer, it’s important to take into account that that data will be a frame older than we might expect.

One classic use of old framebuffer data is simulating motion blur. The current frame is blended with a bit of the previously rendered frame to make a resulting image that looks more like what a real camera captures.

Copy the data between the buffers:

If we can’t repoint users to the other buffer, the only other option is to actually copy the data from the next frame to the current frame. This is how our slapstick comedians work. In that case, we chose this method because the state — a single Boolean flag — doesn’t take any longer to copy than a pointer to the buffer would.

Data on the next buffer is only a single frame old. This is the nice thing about copying the data as opposed to ping-ponging back and forth between the two buffers. If we need access to previous buffer data, this will give us more up-to-date data to work with.

Swapping can take more time. This, of course, is the big negative point. Our swap operation now means copying the entire buffer in memory. If the buffer is large, like an entire framebuffer, it can take a significant chunk of time to do this. Since nothing can read or write to either buffer while this is happening, that’s a big limitation.

What is the granularity of the buffer?
The other question is how the buffer itself is organized — is it a single monolithic chunk of data or distributed among a collection of objects? Our graphics example uses the former, and the actors use the latter.

Most of the time, the nature of what you’re buffering will lead to the answer, but there’s some flexibility. For example, our actors all could have stored their messages in a single message block that they all reference into by their index.

If the buffer is monolithic:

Swapping is simpler. Since there is only one pair of buffers, a single swap does it. If you can swap by changing pointers, then you can swap the entire buffer, regardless of size, with just a couple of assignments.

If many objects have a piece of data:

Swapping is slower. In order to swap, we need to iterate through the entire collection of objects and tell each one to swap.

In our comedian example, that was OK since we needed to clear the next slap state anyway — every piece of buffered state needed to be touched each frame. If we don’t need to otherwise touch the old buffer, there’s a simple optimization we can do to get the same performance of a monolithic buffer while distributing the buffer across multiple objects.

The idea is to get the “current” and “next” pointer concept and apply it to each of our objects by turning them into object-relative offsets. Like so:

class Actor
{
public:
  static void init() { current_ = 0; }
  static void swap() { current_ = next(); }

  void slap()        { slapped_[next()] = true; }
  bool wasSlapped()  { return slapped_[current_]; }

private:
  static int current_;
  static int next()  { return 1 - current_; }

  bool slapped_[2];
};
Actors access their current slap state by using current_ to index into the state array. The next state is always the other index in the array, so we can calculate that with next(). Swapping the state simply alternates the current_ index. The clever bit is that swap() is now a static function — it only needs to be called once, and every actor’s state will be swapped.