# coopScript#++ Basic, its like assembly but terrible

### print [value]
#### prints either a variable or a string
`print Hello, World!`
### var
#### Allocates memory for variable
types are automatically determined to either floats or strings
`var name`
### mov
#### assigns data to a variable
`mov name 5.4`

`mov name string`
## Variable Math
### add
`add var var`

`add var 2.3`
#### concat:
```
var name
mov name Hello
add name , world!
print name
``` 
##### output: Hello, world!
### sub
`sub var var`

`sub var num`
### mul
`mul var1 var2`

`mul var1 2`
### abs
calculates absolute value of num

`abs var`

### more:
- div, divides
- rem, modulo
## Waypoints:
`wp name` : creates a waypoint at the current line

`goto name` : goes to the line of name

for loop implementation:
```
var count
mov count 0
wp beginning
if count < 10
	add count 1
	goto beginning
end
```
C++ equivelent:
```
for (int i = 0; i < 10; i++)
{
	std::cout << i << endl;
}
```
## Other
### ask
asks for input from the console

`ask var`

c++:

`std::cin << var;`
### comments
same as c++:
`// this is text`