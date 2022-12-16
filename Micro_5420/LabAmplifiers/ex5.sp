Lab 4: Operational Amplifiers, Exercise 5
******************************************
** By Chris Winstead
** ECE 5420
** Utah State University
******************************************
** Distortion test for OTA
******************************************

.param  Rref=10k
.param  Vin=10m

.include lab_parts.md
.include opamp.sp

**********************************************************
** POWER AND SIGNAL SOURCES
**********************************************************
VDD  nvdd 0 DC 5V
VCM  ncm  0 DC 2.5V
Vplus  nplus ncm    AC 1 DC 0 SIN(0 {Vin} 1k)
Vminus ncm   nminus AC 1 DC 0 SIN(0 {Vin} 1k)


**********************************************************
** CONTROL SCRIPT
**********************************************************
.control
tran 1u 20m 
*plot xlimit 0 2m v(nout)
meas tran Vopp PP v(nout)


fft  v(nout)
wrdata fft.dat vdb(nout)

*plot xlog xlimit 500 20e3 vdb(nout)
.endc

.end
