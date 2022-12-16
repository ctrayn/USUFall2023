* OTA Circuit Netlist
********************************
** By Chris Winstead
** ECE 5420
** Utah State University
********************************

.global 0 nvdd nvss


**********************************************************
** SUBCIRCUIT DEFINITIONS
**********************************************************
.subckt nmirror in out 
XN1 in  in nvss nvss aldn
XN2 out in nvss nvss aldn
.ends

.subckt pmirror in out 
XP1 in  in nvdd nvdd aldp
XP2 out in nvdd nvdd aldp
.ends

.subckt ndiffpair inplus inminus outplus outminus source
XN1 outminus inplus  source nvss aldn
XN2 outplus  inminus source nvss aldn
.ends

.subckt pmoscs in out
XCS nvdd in out nvdd aldp
R out nss 10000
.ends


**********************************************************
** REFERENCE RESISTOR
**********************************************************
Rref nvdd nref {Rref}



**********************************************************
** OTA CIRCUIT
**********************************************************
X1  nplus  nminus  nx ny nsource  ndiffpair
X2  nx     ny                     pmirror
X3  nref   ns                     nmirror
X4  ny     nout                   pmoscs

RF nout nminus 100k
R1 nin  nminus  10k

**********************************************************
** METER SOURCE FOR THE TAIL CURRENT
**********************************************************
Vmtail nsource ns DC 0V


