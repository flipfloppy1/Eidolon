#pragma once

// Abstract class for functions that interpolate between two points over time
class Interp
{
  protected:
    bool state;
    float val;
    virtual float Increment(float deltaTime) = 0;
    virtual float Decrement(float deltaTime) = 0;

  public:
    float start, end, speed;
    Interp() : start(0.0f), end(1.0f), speed(1.0f) { state = false; }
    Interp(float start, float end) : start(start), end(end), speed(1.0f) { state = false; }
    Interp(float start, float end, float speed) : start(start), end(end), speed(speed) { state = false; }

    // "Jump" to a new interpolation target, by restarting from the current value and changing the end value
    void Jump(float newEnd) { start = val; end = newEnd; }

    // Sets the interpolation to increment if applicable
    void On() { state = true; }

    // Sets the interpolation to decrement if applicable
    void Off() { state = false; }

    // Getter for value
    float Val() { return val; }

    // Setter for value
    void Val(float val) { Interp::val = val; }

    // Update the interpolation with respect to time. Should be called once per frame
    void Update(float deltaTime);

    // Convenience function for updating the on/off status and interpolating with respect to time at once
    void Update(float deltaTime, bool state);
};

// Linear implementation of Interp
class Lerp : public Interp
{
    float Increment(float deltaTime) override;
    float Decrement(float deltaTime) override;
    public:
    Lerp() : Interp() { state = false; }
    Lerp(float start, float end) : Interp(start, end) { state = false; }
    Lerp(float start, float end, float speed) : Interp(start, end, speed) { start = false; }
};

// Implementation of Interp that starts quickly and slows towards the end
class EaseOut : public Interp
{
    float Increment(float deltaTime) override;
    float Decrement(float deltaTime) override;
    public:
    EaseOut() : Interp() { state = false; }
    EaseOut(float start, float end) : Interp(start, end) { state = false; }
    EaseOut(float start, float end, float speed) : Interp(start, end, speed) { start = false; }
};

// Implementation of Interp that starts slowly and quickens towards the end
class EaseIn : public Interp
{
    float Increment(float deltaTime) override;
    float Decrement(float deltaTime) override;
    public:
    EaseIn() : Interp() { state = false; }
    EaseIn(float start, float end) : Interp(start, end) { state = false; }
    EaseIn(float start, float end, float speed) : Interp(start, end, speed) { start = false; }
};
