#ifndef __I386_SCANCODES__
#define __I386_SCANCODES__
/* Keypad keys have not been coded in. */
#define MAX_KEYCODES 128
#define SPECIAL 0xFFFF
#define EMPTY 0xFFFE
#define NOTCODEDUSELOWKEYS 0xFFFC

#define LOWKEYS \
EMPTY /*0*/, \
SPECIAL/*1*/, \
'1'/*2*/, \
'2'/*3*/, \
'3'/*4*/, \
'4'/*5*/, \
'5'/*6*/, \
'6'/*7*/, \
'7'/*8*/, \
'8'/*9*/, \
'9'/*10*/, \
'0'/*11*/, \
'-'/*12*/, \
'='/*13*/, \
SPECIAL/*14*/, \
'\t'/*15*/, \
'q'/*16*/, \
'w'/*17*/, \
'e'/*18*/, \
'r'/*19*/, \
't'/*20*/, \
'y'/*21*/, \
'u'/*22*/, \
'i'/*23*/, \
'o'/*24*/, \
'p'/*25*/, \
'['/*26*/, \
']'/*27*/, \
'\n'/*28*/, \
SPECIAL/*29*/, \
'a'/*30*/, \
's'/*31*/, \
'd'/*32*/, \
'f'/*33*/, \
'g'/*34*/, \
'h'/*35*/, \
'j'/*36*/, \
'k'/*37*/, \
'l'/*38*/, \
';'/*39*/, \
'\''/*40*/, \
'`'/*41*/, \
SPECIAL/*42*/, \
EMPTY /*43*/, \
'z'/*44*/, \
'x'/*45*/, \
'c'/*46*/, \
'v'/*47*/, \
'b'/*48*/, \
'n'/*49*/, \
'm'/*50*/, \
','/*51*/, \
'.'/*52*/, \
'/'/*53*/, \
SPECIAL/*54*/, \
SPECIAL/*55*/, \
SPECIAL/*56*/, \
' '/*57*/, \
SPECIAL/*58*/, \
EMPTY /*59*/, \
EMPTY /*60*/, \
EMPTY /*61*/, \
EMPTY /*62*/, \
EMPTY /*63*/, \
EMPTY /*64*/, \
EMPTY /*65*/, \
EMPTY /*66*/, \
EMPTY /*67*/, \
EMPTY /*68*/, \
SPECIAL/*69*/, \
SPECIAL/*70*/, \
SPECIAL/*71*/, \
SPECIAL/*72*/, \
SPECIAL/*73*/, \
EMPTY /*74*/, \
SPECIAL/*75*/, \
SPECIAL/*76*/, \
SPECIAL/*77*/, \
SPECIAL/*78*/, \
SPECIAL/*79*/, \
SPECIAL/*80*/, \
SPECIAL/*81*/, \
SPECIAL/*82*/, \
SPECIAL/*83*/, \
SPECIAL/*84*/, \
SPECIAL/*85*/, \
SPECIAL/*86*/, \
EMPTY /*87*/, \
SPECIAL/*88*/, \
SPECIAL/*89*/, \
SPECIAL/*90*/, \
SPECIAL/*91*/, \
SPECIAL/*92*/, \
SPECIAL/*93*/, \
SPECIAL/*94*/, \
SPECIAL/*95*/, \
SPECIAL/*96*/, \
SPECIAL/*97*/, \
SPECIAL/*98*/, \
SPECIAL/*99*/, \
SPECIAL/*100*/, \
EMPTY /*101*/, \
EMPTY /*102*/, \
EMPTY /*103*/, \
EMPTY /*104*/, \
EMPTY /*105*/, \
EMPTY /*106*/, \
EMPTY /*107*/, \
EMPTY /*108*/, \
EMPTY /*109*/, \
SPECIAL/*110*/, \
SPECIAL/*111*/, \
EMPTY /*112*/, \
SPECIAL/*113*/, \
SPECIAL/*114*/, \
EMPTY /*115*/, \
EMPTY /*116*/, \
'\\' /*117*/, \
SPECIAL/*118*/, \
SPECIAL/*119*/, \
SPECIAL/*120*/, \
SPECIAL/*121*/, \
SPECIAL/*122*/, \
SPECIAL/*123*/, \
EMPTY /*124*/, \
SPECIAL/*125*/, \
SPECIAL/*126*/, \
EMPTY /*127*/

#define HIGHKEYS \
EMPTY /*0*/, \
SPECIAL/*1*/, \
'!'/*2*/, \
'@'/*3*/, \
'#'/*4*/, \
'$'/*5*/, \
'%'/*6*/, \
'^'/*7*/, \
'&'/*8*/, \
'*'/*9*/, \
'('/*10*/, \
')'/*11*/, \
'_'/*12*/, \
'+'/*13*/, \
SPECIAL/*14*/, \
'\t'/*15*/, \
'Q'/*16*/, \
'W'/*17*/, \
'E'/*18*/, \
'R'/*19*/, \
'T'/*20*/, \
'Y'/*21*/, \
'U'/*22*/, \
'I'/*23*/, \
'O'/*24*/, \
'P'/*25*/, \
'{'/*26*/, \
'}'/*27*/, \
'\n'/*28*/, \
SPECIAL/*29*/, \
'A'/*30*/, \
'S'/*31*/, \
'D'/*32*/, \
'F'/*33*/, \
'G'/*34*/, \
'H'/*35*/, \
'J'/*36*/, \
'K'/*37*/, \
'L'/*38*/, \
':'/*39*/, \
'"'/*40*/, \
'~'/*41*/, \
SPECIAL/*42*/, \
EMPTY /*43*/, \
'Z'/*44*/, \
'X'/*45*/, \
'C'/*46*/, \
'V'/*47*/, \
'B'/*48*/, \
'N'/*49*/, \
'M'/*50*/, \
'<'/*51*/, \
'>'/*52*/, \
'?'/*53*/, \
SPECIAL/*54*/, \
SPECIAL/*55*/, \
SPECIAL/*56*/, \
' '/*57*/, \
SPECIAL/*58*/, \
EMPTY /*59*/, \
EMPTY /*60*/, \
EMPTY /*61*/, \
EMPTY /*62*/, \
EMPTY /*63*/, \
EMPTY /*64*/, \
EMPTY /*65*/, \
EMPTY /*66*/, \
EMPTY /*67*/, \
EMPTY /*68*/, \
SPECIAL/*69*/, \
SPECIAL/*70*/, \
SPECIAL/*71*/, \
SPECIAL/*72*/, \
SPECIAL/*73*/, \
EMPTY /*74*/, \
SPECIAL/*75*/, \
SPECIAL/*76*/, \
SPECIAL/*77*/, \
SPECIAL/*78*/, \
SPECIAL/*79*/, \
SPECIAL/*80*/, \
SPECIAL/*81*/, \
SPECIAL/*82*/, \
SPECIAL/*83*/, \
SPECIAL/*84*/, \
SPECIAL/*85*/, \
SPECIAL/*86*/, \
EMPTY /*87*/, \
SPECIAL/*88*/, \
SPECIAL/*89*/, \
SPECIAL/*90*/, \
SPECIAL/*91*/, \
SPECIAL/*92*/, \
SPECIAL/*93*/, \
SPECIAL/*94*/, \
SPECIAL/*95*/, \
SPECIAL/*96*/, \
SPECIAL/*97*/, \
SPECIAL/*98*/, \
SPECIAL/*99*/, \
SPECIAL/*100*/, \
EMPTY /*101*/, \
EMPTY /*102*/, \
EMPTY /*103*/, \
EMPTY /*104*/, \
EMPTY /*105*/, \
EMPTY /*106*/, \
EMPTY /*107*/, \
EMPTY /*108*/, \
EMPTY /*109*/, \
SPECIAL/*110*/, \
SPECIAL/*111*/, \
EMPTY /*112*/, \
SPECIAL/*113*/, \
SPECIAL/*114*/, \
EMPTY /*115*/, \
EMPTY /*116*/, \
'|'/*117*/, \
SPECIAL/*118*/, \
SPECIAL/*119*/, \
SPECIAL/*120*/, \
SPECIAL/*121*/, \
SPECIAL/*122*/, \
SPECIAL/*123*/, \
EMPTY /*124*/, \
SPECIAL/*125*/, \
SPECIAL/*126*/, \
EMPTY /*127*/


#define KEY_BS 0x0e 
#define KEY_TAB 0x0f 
#define KEY_CAPS 0x3a
#define KEY_LSHIFT 0x2a 
#define KEY_RSHIFT 0x36 
#define KEY_LCTRL 0x1d
#define KEY_LALT 0x38
#define KEY_RALT 0x38
#define KEY_RCTRL 0x1d
#define KEY_INSERT 0x52
#define KEY_DELETE 0x53
#define KEY_HOME 0x47
#define KEY_END 0x4f
#define KEY_PGUP 0x49
#define KEY_PGDOWN 0x51
#define KEY_LEFT 0x4b
#define KEY_UP 0x48
#define KEY_DOWN 0x50 
#define KEY_RIGHT 0x4d 
#define KEY_NUMLOCK 0x45 
#define KEY_KPHOME 0x47 
#define KEY_KPLEFT 0x4b 
#define KEY_KPEND 0x4f 
#define KEY_KPUP 0x48
#define KEY_KPDOWN 0x50 
#define KEY_KPINS 0x52 
#define KEY_KPPGUP 0x49 
#define KEY_KPRIGHT 0x4d 
#define KEY_KPPGDN 0x51 
#define KEY_KPDEL 0x53 
#define KEY_ESC 0x01
#define KEY_F1 0x3b
#define KEY_F2 0x3c
#define KEY_F3 0x3d
#define KEY_F4 0x3e
#define KEY_F5 0x3f
#define KEY_F6 0x40
#define KEY_F7 0x41
#define KEY_F8 0x42
#define KEY_F9 0x43
#define KEY_F10 0x44
#define KEY_F11 0x57
#define KEY_F12 0x58
#define KEY_PRTSCR 0x37
#define KEY_SCROLLLOCK 0x46 

#endif
