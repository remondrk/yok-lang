# Yok
Yok is an esoteric programming language that takes a list of instructions written like regular English and executes them linearly.

## Notes
This implementation is a prototype for a similar and more flexible language rather than a finished project. Error handling and execution pipeline can be improved and the code is not optimal.

## Instructions
- `float` is a decimal number, comma as decimal seperator: `42,1907`, `1923`
- (`float` literals don't support negative numbers, but it can be achieved using subtraction.)
- `string` is started and ended with `"`: `"this is a string"`
- `value` is string, float or variable id: `6`, `"foo"`, `bar`
- `id` starts with a letter but can include `-` and `'`: `str's-len`
&nbsp;

> Create a variable. Can't be executed twice even on the same line
```
create a unique, uninitialized variable and name it {id}
```
&nbsp;

> Assign a value to a variable. Same variable can be assigned different types.
```
take the value of {value} and assign it to the variable {id}
```
&nbsp;

> Label a point. Same `id` can be used multiple times.
```
this place will be referred to as {id} so that we can find it when we need to
```
&nbsp;

> Go to closest instance of a specified label. If there are two labels in equal distance go to the one below.
```
go upwards and downwards until you find a place that is referred to as {id}
```
&nbsp;

> Similar to the one above, but this one can be used as a function call because it saves it's location to a stack to return later. 
```
find a place that is referred to as {id}, however, return back here when you are told to do so
```
&nbsp;

> Return to the previous saved location and remove it from the stack.
```
return back to the previous place that you promised to return back
```
&nbsp;

> Go to the first instance of a specified label while going up.
```
go upwards until you find the place that is referred to as {id}
```
&nbsp;

> Go to the first instance of a specified label while going down.
```
go downwards until you find the place that is referred to as {id}
```
&nbsp;

> If the condition is met, skip a specified amount of lines. Plural suffix in line/lines must be used correctly. Comparison operators are as follows:
> - shares-the-same-value-with
> - is-in-no-way-identical-to
> - holds-a-greater-value-compared-to
> - holds-a-lesser-value-compared-to
> - shares-the-same-value-with-or-holds-a-greater-value-compared-to
> - shares-the-same-value-with-or-holds-a-lesser-value-compared-to
```
if it happens to be that {value} {comparison} {value} ignore the following {value} line/lines
```
&nbsp;

> Unless the condition is met, skip a specified amount of lines. Same rules apply as 'if'.
```
unless it happens to be that {value} {id} {value} ignore the following {quantity} line/lines
```
&nbsp;

> Read line and save it to `the-ingressed-string` variable. If it can be converted to float save it to `the-ingressed-float`, otherwise reset its value.
```
wait here until there is a useful ingress from the user
```
&nbsp;

> Write to console and put a new line at the end.
```
write {value} to the console and skip to the next line afterwards
```
&nbsp;

> Write to console without a new line at the end.
```
write {value} to the console , but do not skip to the next line
```
&nbsp;

In the following instructions, if the result is a number it is saved to `the-resulting-number` and `the-resulting-string` variable after it's converted. If the result is a string it is saved to `the-resulting-string` variable and the value of `the-resulting-number` variable is reset.
&nbsp;

> If both values are floats, perform a division. If only first value is string, get the first `y` characters. If only second value is string, remove the first `x` characters.
```
take the value of {value(x)} and divide it using the value of {value(y)}
```
&nbsp;

> If both values are floats, perform a multiplication. If only first value is string, repeat it `y` times.
```
repeat the string or the number {value(x)} an amount of times equal to the number {value(y)}
```
&nbsp;

> If both values are floats, perform an addition. If at least one of them is string, convert the non-string ones and concatenate them.
```
take two numbers or strings , {value} and {value} , then merge their values together
```
&nbsp;

> If both values are numbers, perform a subtraction (x - y).
```
perform a subtraction between {value(x)} and {value(y)} , the first one being the minuend
```
&nbsp;
