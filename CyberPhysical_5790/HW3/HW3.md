Calvin Passmore

A02107892

ECE 6790

# Homework 3

## 3.13

Consider the transition system $Mult(m,n)$ described in exercise 3.1. Describe this transition systme symbolically using initialization and transition formulas.

### 3.1
Given two natural numbers *m* and *n*, consider the program Mult that multiplies the input numbers using two variables *x* and *y*, of type nat, as shown in the below figure.
Describe the transition system *Mult*(m,n) that captures the behavior of this program on input numbers *m* and *n*, that is, describe the states, initial states, and transitions.
Argue that when the value of the variable *x* is 0, the value of the variable *y* must equal the product of the input numbers *m* and *n*, that is, the following property is an invariant of this transition system:

```
(mode = stop) -> (y = m*n)
```
![Figure 3.2: Program for Multiplication](./Figure3.2.png)
___

Initial States = {x = m; y = 0;}

States = {loop, stop}

Transitions

    if x > 0 then
        x := x + 1;
        y := y + n;
        states := loop;
    else
        states := stop

When x = 0, y must be the product of *m* and *n* because that is the definition of multiplication. Taking a number (*n*) and adding it to itself a number of times (*m*). x is set to the number of times and y is counting the running total.
---

TODO Answer

## 3.14
Consider the description fo the component *Switch* given as an extended-state machine in [figure 2.2](figure2.2). Give the initialization and reaction formulas corresponding to *Switch*. Obtain the transition formula for the corresponding transition system in as simplified form as possible.

![figure2.2](./figure2.2.png)

If $f$ si a Boolean formula over a set $V$ of variables and $x$ is a variable in $V$, then $\includes x$. $f$ is a Boolean formula over $V/{x}$. For example, if $x$ and $y$ are Boolean variables, then the formula $\includes x.(x and y)$ expresses a constraint only over the variable $y$ and is equivalent to the formula $y$(that is, $\includes x.(x and y)$ evaluates to 1 exactly when $y$ is assigned to 1). Similarly, the formula $\includes x.(x and y)$ is equivalent to 1 (that is, this formula evaluates to 1 independent of the value of y).

The transition formula for the transition sysmte corresponding to *Delay* is

$$ \includes in \includes out.[(out = x) and (x^' = in)] $$

This formula simplifies to the logical constant 1 since the formula is satisfied no matter what the values of $x$ and $x^'$ are. This corresponds to the fact that for this transition system, there is a transition between every pair of states.

More generally, if $\phi _R$ is the reaction formula for the component $C$, then the transition fomula $\Phi _T$ for the corresponding transition system is obtained by existentially quantifying al the input and output variables: $\includes I. \includes O. \Phi _R$

As another example, consider the component *TriggeredCopy* of [figure 2.5]. The only state variable is $x$ of type nat, the initailization formula is $x = 0$, and the reaction formula is 
$$(in? and out = in and x^' = x + 1) or (~in? and out = 1 and x^' = x)$$

The assumption that when the input event is absent, the component leaves the state unchanged and the ouput is absent, is captured explicitly in the reaction formula as a seperate case. The corresponding transition formula is obtained by existnentially quantifying the input and output variable.

$$ \invludes in, out [(in? and out = in and x^' = x = 1) or (~in? and out = 1 and x^' = x)] $$

This transition formula can be simplified to a logically equivalent formula.

$$ (X^' = x + 1) or (x^' = x)$$

![TriggeredCopy]() TODO
---

TODO: Answer

---
---

## 3.16
Consider the symbolic image computation for a transition system with two real-value variables $x$ ad $y$ and transition description given by the formula $x^' = x + 1 or y^' = x$. Suppose the region $A$ is described by the formula $0 \le x \le 4 or y \le 7$. Compute the formula describing the post-image of $A$.
---

TODO: Answer
---
---

## 3.17
Consider a transition system *T* with two integer variables $x$ and $y$. The transitions of the system corresponding to executing the statement:

$$ if (x < y) then x:= x + y else y: = y + 1 $$

Write the transition forumla over the variables $x, y, x^', and y$ that captures the transition relation of the system. Consider a region $A$ of the above transition systme described by the formula $0 \lt x \lt 5$. Compute the formula describing the post-image of $A$.
---
TODO: Answer
---
---

## 3.20
Consider the Boolean formula
$$(x or y) and (~x or z) and (~y or ~z)$$

Draw the ROBDD for this formula with respect to the variable ordering $x<y<z$.

Note: The textbook has an examlpe
---
TODO: Answer
---
---

## 4.1
We want to design an asynchronous adder process *AsyncAdd* with input channels $x_1$ and $x_2$ and an output channel $y$, all of type nat. If the *i*th input message arriving on the channel $x_1$ is $v$ and the *i*th input message arriving on channel $x_2$ is $w$, then the *i*th value output byt the process *AsyncAdd* on its output channel should be $v + w$. Describe all the components of the process *AsyncAdd*
---
TODO: Answer
---
---
## 4.2
We want to design an asynchronous process *Split* that is the dual of *Merge*. The process *Split* has on einput channel *in* and two output channels $out_1$ and $out_2$. The messages receieved on the input channel should be routed to one of the output channels in a nodeteministic manner so that all possible splittings of the input stream are feasable executions. Describe all the components of the desired process *Split*.

https://microsoft.github.io/ivy/examples/specification.html



