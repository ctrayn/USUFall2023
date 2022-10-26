* Diode 1N914:

.model D1N914 D(Is=168.1E-21 N=1 Rs=.1 Ikf=0 Xti=3 Eg=1.11 Cjo=4p 
+ M=.3333 Vj=.75 Fc=.5 Isr=100p Nr=2 Bv=100 Ibv=100u Tt=11.54n) 


* ======================================================================
* DEVICE MODELS FOR ALD1105 ARRAY: "ncg" and "pcg" are the model names
* ======================================================================

.model ncg  nmos  (level=2
+       gamma=1.09 vto=.750 Uo=650 Ucrit=0.7e4 Uexp=.1 Vmax=1.6e5
+       phi=.70 tpg=+1 nsub=1e16 neff=10 nss=.7e11 nfs=1.17e11
+       tox=.055u Cgso=.94n Cgdo=.59n Cgbo=.138n 
+       cj=.39m cjsw=264p xj=2.0u ld=1.6u  
+       pb=.9 js=20e-6  mj=.5 mjsw=0.18 kf=.75e-28 rsh=10 )


.model pcg pmos (level=2
+       vto=-.74 gamma=.58 Uo=260 Ucrit=1.0e4 Uexp=.16 Vmax=3.0e5
+       phi=.70 tpg=-1 nsub=1e16 Neff=1 nss=1e11 nfs=1.06e11
+       tox=.055u Cgso=.31n Cgdo=.39n Cgbo=.138n 
+       cj=.18m cjsw=322p xj=1.2u ld=.96u pb=.9 
+       js=20e-6 mj=.57 mjsw=0.33 kf=.75e-28  rsh=90)



* ======================================================================
* SUBCIRCUIT DEFINITIONS FOR EASY DECLARATION: aldn and aldp
* ======================================================================
.subckt aldn 1 2 3 4 
m1 1 2 3 4 ncg l=7.8e-6 w=0.138e-3 as=0.603e-8 ps=0.478e-3 ad=0.161e-8
+                                  nrd=.3 nrs=1 
.ends

.subckt aldp 1 2 3 4 
m1 1 2 3 4 pcg l=7.8e-6 w=0.138e-3 as=0.603e-8 ps=0.478e-6 ad=0.161e-8
+                                  nrd=.3 nrs=1 
.ends

*=====================================================
* BJT models
*=====================================================

.model Q2N2222 NPN (Is=14.34f Xti=3 Eg=1.11 Vaf=74.03 Bf=255.9 Ne=1.307
+Ise=14.34f Ikf=.2847 Xtb=1.5 Br=6.092 Nc=2 Isc=0 Ikr=0 Rc=1
+Cjc=7.306p Mjc=.3416 Vjc=.75 Fc=.5 Cje=22.01p Mje=.377 Vje=.75
+Tr=46.91n Tf=411.1p Itf=.6 Vtf=1.7 Xtf=3 Rb=10)

.model Q2N2907A PNP (Is=650.6E-18 Xti=3 Eg=1.11 Vaf=115.7 Bf=231.7 Ne=1.829
+Ise=54.81f Ikf=1.079 Xtb=1.5 Br=3.563 Nc=2 Isc=0 Ikr=0 Rc=.715
+Cjc=14.76p Mjc=.5383 Vjc=.75 Fc=.5 Cje=19.82p Mje=.3357 Vje=.75
+Tr=111.3n Tf=603.7p Itf=.65 Vtf=5 Xtf=1.7 Rb=10)

.model Q2N3904 NPN (Is=6.734f Xti=3 Eg=1.11 Vaf=74.03 Bf=416.4 Ne=1.259
+Ise=6.734f Ikf=66.78m Xtb=1.5 Br=.7371 Nc=2 Isc=0 Ikr=0 Rc=1
+Cjc=3.638p Mjc=.3085 Vjc=.75 Fc=.5 Cje=4.493p Mje=.2593 Vje=.75
+Tr=239.5n Tf=301.2p Itf=.4 Vtf=4 Xtf=2 Rb=10)

.model Q2N3906 PNP (Is=1.41f Xti=3 Eg=1.11 Vaf=18.7 Bf=180.7 Ne=1.5 Ise=0
+Ikf=80m Xtb=1.5 Br=4.977 Nc=2 Isc=0 Ikr=0 Rc=2.5 Cjc=9.728p
+Mjc=.5776 Vjc=.75 Fc=.5 Cje=8.063p Mje=.3677 Vje=.75 Tr=33.42n
+Tf=179.3p Itf=.4 Vtf=4 Xtf=6 Rb=10)

*----------------------------------------------------------------------------
* CD4007 NMOS and PMOS SPICE models
.model CD4007N NMOS
+ Level=1 Gamma= 0 Xj=0 W=30e-6 L=10e-6
+ Tox=1200n Phi=.6 Rs=0 Kp=111u Vto=2.0 Lambda=0.01
+ Rd=0 Cbd=2.0p Cbs=2.0p Pb=.8 Cgso=0.1p
+ Cgdo=0.1p Is=16.64p N=1

*The default W and L is 30 and 10 um respectively and AD and AS
*should not be included.

.model CD4007P PMOS
+ Level=1 Gamma= 0 Xj=0 W=60e-6 L=10e-6
+ Tox=1200n Phi=.6 Rs=0 Kp=55u Vto=-1.5 Lambda=0.04
+ Rd=0 Cbd=4.0p Cbs=4.0p Pb=.8 Cgso=0.2p
+ Cgdo=0.2p Is=16.64p N=1

*The default W and L is 60 and 10 um respectively and AD and AS
*should not be included.

*-----------------------------------------------------------------------------
*
* To use a subcircuit, the name must begin with 'X'.  For example:
* X1 1 2 3 4 5 uA741
*
* connections:   non-inverting input
*                |  inverting input
*                |  |  positive power supply
*                |  |  |  negative power supply
*                |  |  |  |  output
*                |  |  |  |  |
.subckt uA741    1  2  3  4  5
*
  c1   11 12 8.661E-12
  c2    6  7 30.00E-12
  dc    5 53 dx
  de   54  5 dx
  dlp  90 91 dx
  dln  92 90 dx
  dp    4  3 dx
  egnd 99  0 poly(2) (3,0) (4,0) 0 .5 .5
  fb    7 99 poly(5) vb vc ve vlp vln 0 10.61E6 -10E6 10E6 10E6 -10E6
  ga    6  0 11 12 188.5E-6
  gcm   0  6 10 99 5.961E-9
  iee  10  4 dc 15.16E-6
  hlim 90  0 vlim 1K
  q1   11  2 13 qx
  q2   12  1 14 qx
  r2    6  9 100.0E3
  rc1   3 11 5.305E3
  rc2   3 12 5.305E3
  re1  13 10 1.836E3
  re2  14 10 1.836E3
  ree  10 99 13.19E6
  ro1   8  5 50
  ro2   7 99 100
  rp    3  4 18.16E3
  vb    9  0 dc 0
  vc    3 53 dc 1
  ve   54  4 dc 1
  vlim  7  8 dc 0
  vlp  91  0 dc 40
  vln   0 92 dc 40
.model dx D(Is=800.0E-18 Rs=1)
.model qx NPN(Is=800.0E-18 Bf=93.75)
.ends


************************************
* Optoelectronic Components:
* SFH486 ==> IR LED emitter
* SFH235 ==> IR photodiode
************************************
.MODEL SFH486 D		
+	IS=	4.55375E-12
+	N=	2.275980975
+	RS=	1.185436803
+	IKF=	0.065668313
+       Cjo=    15p
+       M=      0.452

.model SFH235 D(IS=3.5e-11 N=1.01 IKF=6e-5 Rs=6.5 Cjo=73.01p M=0.452 
+ Vj=0.304 BV=200)


.MODEL SFH4554 D        
+       IS =    8.04E-17
+       N =     1.584035846
+       RS =    1.976811554
+       IKF =   0.074579378
+       IBV =   1.50E-08
+       NBV =   70
+       IBVL =  1.00E-07
+       NBVL =  1.5
+       BV =    16
+       CJO =   3.71E-11
+       TT =    1.00E-08
+       EG =    1.46
