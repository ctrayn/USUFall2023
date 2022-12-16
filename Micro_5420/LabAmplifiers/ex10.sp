Lab 4: Operational Amplifiers, Exercise 3
******************************************
** By Chris Winstead
** ECE 5420
** Utah State University
******************************************
** Input Common Mode Range test
** for OTA based on ALD1105
******************************************

.param  Rref=10k


.include ../lab_parts.md
.include opamp.sp


**********************************************************
** POWER AND SIGNAL SOURCES
**********************************************************
VDD  nvdd   0 DC 5V
VCM  nplus  0 DC 2.5
Vsig nin nplus    AC 1 DC 0 SIN(0 1m 1k)

RF  nout nminus 10k
R1  nin  nminus 1k


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
let cmstart=0
let cmstep=0.25

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
while idx<21
* Get new CM value:
  set idx="$&idx"
  let newcm=cmstart+idx*cmstep
  set cm="$&newcm"

* Alter VCM voltage source:
  alter VCM=$cm

* Perform transient analysis
  tran 1us 2ms
  plot v(nin) v(nout) 

* Measure Gain:
  meas tran yval PP v(nout)
  meas tran xval PP v(nin) 
  meas tran tailCurrent AVG Vmtail#branch

* Store measurements in defined arrays:
  let vicm[idx]=$cm
  let Gain[idx]=yval/xval
  let GaindB[idx]=db(0.5*yval/xval)
  let Iref[idx]=tailCurrent
  let gmn[idx]=@m.x1.xn1.m1[gm]
  let gmp[idx]=@m.x2.xp1.m1[gm]
  let ron[idx]={1/@m.x1.xn1.m1[gds]}
  let rop[idx]={1/@m.x2.xp1.m1[gds]}

* Perform AC analysis:
  AC dec 10 1 1G

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
