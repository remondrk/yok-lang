# Yok
Yok is an esoteric programming language that takes a list of instructions written like regular English and executes them linearly.

## Notes
This implementation is a prototype for a similar and more flexible language rather than a finished project. Error handling and execution pipeline can be improved and the code is not optimal.

## Instructions
- `value` is string, float or variable id.
- `id` starts with a letter but can include `-` and `'`.

### Variables
> Create an uninitialized variable. If a variable already exists it can't be created again, even if it is in the same line. Code must be structured accordingly.
```
create a variable named {id}
```
&nbsp;  

> Assign a value to a variable.
```
assign {value} to {id}
```
&nbsp;  

### Control Flow
> Label a point in code.
```
set a waypoint here labelled {id}
```
&nbsp;  

> Go to the closest instance of a label in code. If there are two labels in equal distance going up and down, the one below is choosen.
```
teleport to the waypoint labelled {id}
```
&nbsp;  

> Push the current location to stack to return later and go to the closest instance of a label in code.
```
teleport to the waypoint labelled {id} but teleport back when you're done
```
&nbsp;  

> Return to the previous location in the stack. Functions created using these instructions will be able to call other functions and return back to the correct location.
```
teleport back to the previous place we said we'll
```
&nbsp;  

> Go upwards to a label.
```
teleport to the waypoint above labelled {id}
```
&nbsp;  

> Go downwards to a label.
```
teleport to the waypoint below labelled {id}
```
&nbsp;  

> If the condition is met, skip a specified amount of lines. The instruction must end with `lines` instead of `line` if more than 1 is specified. Condition (called `id` here) must be one of the following.
- `is`
- `isn't`
- `is-greater-than`
- `is-less-than`
- `is-greater-than-or-equal-to`
- `is-less-than-or-equal-to`
```
if {value} {id(condition)} {value} skip next {value} line/lines
```
&nbsp;  

> Unless the condition is met, skip a specified amount of lines. Rules are same as "if".
```
unless {value} {id(condition)} {value} skip next {value} line/lines
```
&nbsp;  

### Input/Output
> Wait for user input and save it to the variable `the-inputted-string`. If it can be converted to a float, convert it and save it to the variable `the-inputted-number`, otherwise just reset its value.
```
wait for user input
```
&nbsp;  

> Print a value and put a new line at the end.
```
say {value} out loud
```
&nbsp;  

> Print a value but don't put a new line.
```
whisper {value} silently
```
&nbsp;  

### Operations
In all of the operations, `the-resulting-number` and `the-resulting-string` will be manipulated. If the result is a number both variables will be set after converting the value. If it is string, number variable will be reset.

> If both values are numbers perform a division. If first value is a string get a substring starting from the first character of a size equal to second value, if not possible get the whole string back.
```
divide {value} by {value}
```
&nbsp;  

> If both values are numbers perform a multiplication. If first value is a string, repeat it an amount of times equal to the second value.
```
multiply {value} by {value}
```
&nbsp;  

> Add two numbers.
```
add {value} and {value}
```
&nbsp;  

> Subtract a number from another number.
```
subtract {value} from {value}
```
&nbsp;  

> Concatenate two strings.
```
concatenate {value} and {value}
```
&nbsp;  

### Inline Instructions
> Comment out until end of the line. Comma is optional.
```
{instructions}, btw {comment}
```
&nbsp;  

> Write multiple instructions in one line.
```
{instruction 1} then {instruction 2} ...
```
&nbsp;  
