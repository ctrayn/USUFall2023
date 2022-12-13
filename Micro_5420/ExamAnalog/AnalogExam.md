Calvin Passmore

A02107892

ECE 5420

# Exam: Analog Circuits

## Problem 1

    Triode Mode: id = Kn[Vov * Vds - 0.5 * Vds^2] * (1 + lambda * Vds)
    id = 1.5 mA = (W/L) * Ut * [Vov * Vds - 0.5 * Vds ^ 2] * (1 + lambda + Vds)
    i_target = id * 10
    Kn_target * Constants = Kn * Constants * 10
    (W / L) = (W / L ) * 10
    w_target = w * 10
    w = 10 um

The simulation matched the calculations fairly closely but was not exact, the simulation gave a width of 11.3 um.

The calculated results:
![1calc](./1_calc.png)

Adjusting the simulation to give a perfect 15mA:
![1sim](./1_sim.png)

## Problem 2

The NMOS is now in saturation because 

This time, I used the initial current from the EveryCircuit simulation which was 2.78mA to calculate the needed width of the NMOS which was 5.396 $\mu$ m. This reduced the rounding and conversion errors to that setting the NMOS width to 5.39 $\mu$ m resulting in a current of 14.9 mA.

![2](./2.png)

## Problem 3