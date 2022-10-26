* First line is a TITLE line and is not interpreted

* Comments begin with asterisk


* INCLUDE external files first with .input <filename>
* Usually these are files with model parameters for complex
* devices and subcircuits:
.include lab_parts.md 


* GLOBAL PARAMETERS follow with .param <parameter_name> <parameter_value>
.param VDD  = 5  
.param freq = 1k 
.param A    = 2 
.param R    = 1k 
.param C    = 1p 

* COMPONENT INSTANTIATIONS FOLLOW

* DC VOLTAGE SOURCE
VDD nvdd 0 DC VDD


* SINUSOIDAL VOLTAGE SOURCE: Vxxx Nplus Nminus SIN (VOFFSET AMPLITUDE FREQ DELAY DAMPING PHASE)
Vsig nsig 0 SIN(0 {A} {freq})


* RESISTOR: Rxxx Nplus Nminus VALUE
* The "N" labels are NODE NAMES
R1 nsig nout R

* CAPACITOR: Cxxx Nplus Nminus VALUE
* The GROUND node is always named 0
C1 nout 0 C

* DIODE: Dxxx Nplus Nminus MODEL_NAME
* (check in lab_parts.md for the 1N914 model details)
D1 nout 0 D1N914


* ANALYSIS COMMANDS
.control
   * Here we request a TRANSIENT simulatuion: TRAN TSTEP TSTOP
   tran 1u 4m
   wrdata results.dat nsig nout   
.endc

* END OF FILE
.end

