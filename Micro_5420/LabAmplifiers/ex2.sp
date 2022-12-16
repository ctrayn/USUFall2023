Operational Amplifiers, Exercise 2
******************************************
** By Chris Winstead
** ECE 5420
** Utah State University
******************************************
** Input Common Mode Range test
** for OTA based on ALD1105
******************************************
.include lab_parts.md

.param  Rref=10k
.include opamp.sp


**********************************************************
** POWER AND SIGNAL SOURCES
**********************************************************
VDD  nvdd 0 DC 5V
VSS  nvss 0 DC -5V
VCM  ncm  0 DC 0
Vplus  nplus ncm    AC 1 DC 0 SIN(0 1m 1k)
Vminus ncm   nminus AC 1 DC 0 SIN(0 1m 1k)


**********************************************************
** CONTROL SCRIPT
**********************************************************
.control

* Tell the simulator to save internal device parameters:
save @m.x1.xn1.m1[gm]
save @m.x1.xn1.m1[gds]
save @m.x2.xp1.m1[gds]
save @m.x2.xp1.m1[gm]
save all

let idx=0
let cmstart=-5
let cmstep=0.5
let cm=cmstart

* Declare arrays for results:
let Gain=unitvec(20)
let GaindB=unitvec(20)
let vicm=unitvec(20)
let Iref=unitvec(20)
let bw=unitvec(20)
let gmn=unitvec(20)
let gmp=unitvec(20)
let rop=unitvec(20)
let ron=unitvec(20)

* Perform analysis loop:
while cm<5

* Note on SPICE variables:
* "set" defines a shell STRING variable 
* "let" defines a spice NUMERICAL variable (double-precision)
* The "$&" operator converts a numerical variable to a string.
* SPICE commands need to be written as strings, so there are
* a few num->str conversions in the script that follows

* Get new CM value
  let cm=cmstart+idx*cmstep
  print cm
  
* Alter VCM voltage source (num->str with $&):
  alter VCM=$&cm

* Calculate Operating Point
  op

  let gmn[idx]=@m.x1.xn1.m1[gm]
  let gmp[idx]=@m.x2.xp1.m1[gm]
  let ron[idx]={1/@m.x1.xn1.m1[gds]}
  let rop[idx]={1/@m.x2.xp1.m1[gds]}


* Perform transient analysis
  tran 1us 2ms
  set tranfname=tran{$&idx}.dat
  wrdata $tranfname v(nplus) Y(nminus) v(nout) v(ncm)
*  plot v(nplus) v(nminus) v(nout) v(ncm)

* Measure Gain:
  meas tran yval PP v(nout)
  meas tran xval PP v(nplus) 
  meas tran tailCurrent AVG Vmtail#branch

* Store measurements in defined arrays:
  let vicm[idx]=cm
  let Gain[idx]=0.5*yval/xval
  let GaindB[idx]=db(0.5*yval/xval)
  let Iref[idx]=tailCurrent

* Perform AC analysis:
  AC dec 10 1 1G
  set acfname=ac{$&idx}.dat
  wrdata $acfname vdb(nout)

* Measure BW by finding the -3dB point (if it exists)
  meas ac maxout MAX vdb(nout)
  let m=maxout-3
  if (maxout > 0)
    meas ac w WHEN vdb(nout)="$&m"
    let bw[idx]=w
  else
    let bw[idx]=0
  end
  let idx=idx+1
end

* Print out results:
set width=160
print vicm Iref Gain GaindB bw gmn gmp ron rop

.endc

.end
