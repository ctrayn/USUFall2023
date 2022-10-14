### __Exercise 2.20__:
Design a 1-bit synchronous adder *1BitAdder* by composing instances of And, Or, Not, and Xor gates.
The component *1BitAdder* has three input variables, *x*, *y*, and _carry-in_, and two output variables *z* and *carry-out*, where *z* is the least significant bit, should equal the sum of the values of the three input variables.
Then, design a 3-bit synchronous adder *3BitAdder* by composing three instances of the component *1BitAdder*. The component *3BitAdder* has input variables *x<sub>0</sub>*, *x<sub>1</sub>*, *x<sub>2</sub>*, *y<sub>0</sub>*, *y<sub>1</sub>*, *y<sub>2</sub>*, and *carry_in* and has output variables *z<sub>0</sub>*, *z<sub>1</sub>*, *z<sub>2</sub>*, and *carry_out* should equal the sum of the 3-bit number encoded by the input variables *x<sub>0</sub>*, *x<sub>1</sub>*,  *x<sub>2</sub>*, and the 3-bit number encoded by the input variables y<sub>0</sub>*, *y<sub>1</sub>*, *y<sub>2</sub>*, and the input value of *carry_in*.

TODO Answer

### __Exercise 2.22__:
Consider the leader election algorithm in synchronous networks.
Argue that if the value of *id* does not charge in a given round, then there is no need to send it in the following round (that is, the output *out* can be absent in the next round).
This can reduce the number of messages sent.
Modify the description of the component *SyncLENode* to implement this change.

![Figure 2.35](/CyberPhysical_5790/HW2/Figure2.35.png)

TODO Answer

### __Exercise 3.6__:
Consider a transition system *T* with two integer variables *x* and *y* and a Boolean variable *z*.
All the variables are initially 0.
The transitions of the system corresponding to executing the conditional statement.

    if (z = 0) then {x := x + 1; z := 1} else {y := y + 1; z := 0}

Consider the property &Phi; given by *(x = y) || (x = y + 1)*.
Is &Phi; and invariant of the transition system *T*? Is &Phi; an inductive invariant of the transition system *T*?
Find a formula &Psi; such that &Psi; is stronger than &Phi; and is an inductive invariant of the transition system *T*.
Justify your answers.

TODO Answer

### __Exercise 3.7__:
Recall that the transition system *Mult*(m,n) from exercise 3.1.
First, show that the invariant property (mode = *stop*) -> (y = m * n) is not an inductive invariant.
Then find a stronger property that is an inductive invariant.
Justify your answers.

TODO Answer

### __Exercise 3.1__:
Given two natural numbers *m* and *n*, consider the program Mult that multiplies the input numbers using two variables *x* and *y*, of type nat, as shown in the below figure.
Describe the transition system *Mult*(m,n) that captures the behavior of this program on input numbers *m* and *n*, that is, describe the states, initial states, and transitions.
Argue that when the value of the variable *x* is 0, the value of the variable *y* must equal the product of the input numbers *m* and *n*, that is, the following property is an invariant of this transition system:

```
(mode = stop) -> (y = m*n)
```
![Figure 3.2: Program for Multiplication](/CyberPhysical_5790/HW2/Figure3.2.png)

TODO Answer

### __Exercise 3.3__:
This reaction description for the controller *Controller2* consists of three tasks as shown below.
*Split* the task A<sub>3</sub> into four tasks, each of which writes exactly on of the state variables *east*, *west*, *near<sub>w</sub>*, and *near<sub>E</sub>.
Each task should be described by its read-set, write-set, and update code, along with the necessary precedence constraints.
The revised description should have the same set of reactions as the original description.
Does this splitting impact output/input await dependencies?
If not, what would be the potential benefits and/or drawbacks of the revised description compared to the original description?

![Figure 3.8: A Safe Controller for the Railroad Problem](/CyberPhysical_5790/HW2/Figure3.8.png)

TODO Answer

### __Exercise 3.4__:
Consider a component *C* with an output variable *x* of type *int*.
Design a safety monitor to capture the requirement that the sequence of values output by the component *C* is strictly increasing (that is, the output in each round should be strictly greater than the output in the preceding round).

TODO Answer

