RXI0:
  add     #-4, r15 SORT OUT THE STACK
  sts.l   pr, @-r15
  mov.l   r14, @-r15
  mov     r15, r14
  sts.l   macl, @-r15
  sts.l   mach, @-r15
  mov.l   r10, @-r15
  mov.l   r11, @-r15
  mov.l   r12, @-r15
  mov.l   r13, @-r15
  mov.l   r3, @-r15
  mov.l   r4, @-r15
  mov.l   r5, @-r15
  mov.l   r6, @-r15
  mov.l   r7, @-r15
  mov.l   r0, @-r15

  mov.l (int_disable),r10
  jsr @r10
  nop             

mov.l (RDR0),r10 READ SERIAL PORT
mov.b @r10,r10
extu.b r10,r10
mov.w (E0),r3
cmp/hs r3,r10 EXIT IF NOT E0,E1,E2
bf exitformut
mov.w (E2),r3
cmp/hi r3,r10
bt exitformut
mov.l   (bit7allowslogging), r0
mov.w   @r0, r0
tst     #0x80, r0 EXIT IF NOT LOGGING
bt      exitformut
mov.l   (mutorobd), r0
mov.w   @r0, r0
tst     #0x80, r0 EXIT IF NOT IN MUT MODE
bt      exitformut
mov.l   (receive_transmit_status_bits), r0
mov.w   @r0, r0
tst     #0x80, r0 CHECK WE ARE NOT GETTING AN ECHO OF SOMETHING WE JUST SENT
bf      exitformut

brE012: FOR SETTING UP FIRST DMA TRANSFER
mov.l (DMAOPFLAG2),r11
mov.l r10,@r11
mov #0,r0
mov.l (counter1),r10 STOP MUT TIMEOUT
mov.w r0,@r10
mov #0xfffffffc,r0
mov.l (CHCR3),r10 RESET DMA
mov.l @r10,r10
and r0,r10
mov.l (CHCR3),r11
mov.l r10,@r11
mov.l (RDR0),r0
mov.l (SAR3),r11 SOURCE IS SERIAL PORT
mov.l r0,@r11
mov.l (DMAread),r0
mov.l (DAR3),r11 DESTINATION IS OUR OWN...
mov.l r0,@r11
mov #6,r0 ...6 BYTE MEMORY BLOCK - FOUR BYTES ADDRESS, TWO BYTES LENGTH
mov.l (DMATCR3),r11
mov.l r0,@r11
mov #0x37,r0 0X37 IS THE NUMBER OF MY CHILDHOOD HOME - SUITABLE RANDOM NON ZERO NUMBER THAT WE CAN SET WHEN WE DON'T WANT THE ECU TO KILL OUR DMA PROCESS
mov.l (DMAOPFLAG),r10
mov.l r0,@r10
mov.l (SSR0),r10
mov.b @r10,r0
and #0x87,r0 RESET SERIAL PORT
mov.b r0,@r10
mov.l (DMA3CONFIGread),r0 SET DMA CHANNEL 3 TO READ FROM SERIAL PORT
mov.l (CHCR3),r10
mov.l r0,@r10
  mov.l (int_enable),r10
  jsr @r10
  nop
bra exit
nop

exitformut:
  mov.l (int_enable),r10
  jsr @r10
  nop
mov.l (serialreceivewithoutdma),r10 BACK TO NORMAL SERIAL INTERRUPT, NONE OF OUR BUSINESS!
jsr @r10
nop

exit:
  mov.l   @r15+, r0 STACK
  mov.l   @r15+, r7
  mov.l   @r15+, r6
  mov.l   @r15+, r5
  mov.l   @r15+, r4
  mov.l   @r15+, r3
  mov.l   @r15+, r13
  mov.l   @r15+, r12
  mov.l   @r15+, r11
  mov.l   @r15+, r10
  lds.l   @r15+, mach
  lds.l   @r15+, macl
  mov.l   @r15+, r14
  lds.l   @r15+, pr
  add     #4, r15
  rte RETURN FROM EXCEPTION/INTERRUPT
  nop


.align 2
int_disable:
.long 0x400
int_enable:
.long 0x41e
serialreceivewithoutdma:
.long 0xe32a /*TO BE CHANGED FOR EACH ROM*/
bit7allowslogging:
.long 0xffff6fea /*TO BE CHANGED FOR EACH ROM*/
mutorobd:
.long 0xffff6fb2 /*TO BE CHANGED FOR EACH ROM*/
receive_transmit_status_bits:
.long 0xffff6fe6 /*TO BE CHANGED FOR EACH ROM*/
counter1:
.long 0xffff7054 /*TO BE CHANGED FOR EACH ROM*/
SAR3:
.long 0xffffecf0
DAR3:
.long 0xffffecf4
DMATCR3:
.long 0xffffecf8
CHCR3:
.long 0xffffecfc
DMA3CONFIGread:
.long 0x20105
SSR0:
.long 0xfffff004
RDR0:
.long 0xfffff005
DMAOPFLAG:
.long 0xffff8480
DMAOPFLAG2:
.long 0xffff8484
DMAread:
.long 0xffff8488
E0:
.word 0xE0
E2:
.word 0xE2

/*--------------------------------*/
.align 4
DMAEND:
sts.l pr,@-r15 STACK
mov.l r0,@-r15
mov.l r10,@-r15
mov.l (_int_disable),r10 STOP INTERRUPTS
jsr @r10
nop

mov #0xfffffffc,r0
mov.l (_CHCR3),r10 CLEAR DMA PROCESS
mov.l @r10,r10
and r10,r0
mov.l (_CHCR3),r10
mov.l r0,@r10

mov.l (_DMAOPFLAG2),r0 SINCE WE USE TWO CONSECUTIVE DMA PROCESSES, I USE THIS VARIABLE TO KEEP TRACK OF WHAT WE'RE DOING - READ, WRITE ETC.
mov.l @r0,r0
mov #1,r10
cmp/eq r10,r0
bt brwrite
nop
mov.w (_E0),r10
cmp/eq r10,r0
bt brE0
nop
mov.w (_E1),r10
cmp/eq r10,r0
bt brE1
nop
mov.w (_E2),r10
cmp/eq r10,r0
bt brE2
nop
bra TEIEinvade
nop

brwrite:
mov.l (_SSR0),r10
mov.b @r10,r0
tst #4,r0
bf TEIEinvade DMA MAY HAVE ENDED BUT SERIAL PORT TRANSMISSION MAY NOT HAVE
nop

mov.w (_SCR0_CLRTIE_SETTEIE),r0 SETUP TRANSMIT END INTERRUPT IF NOT YET FINISHED
mov.l (_SCR0),r10
mov.b r0,@r10

mov.l (_int_enable),r10
jsr @r10
nop
mov.l @r15+,r10
mov.l @r15+,r0
lds.l @r15+,pr
rte
nop

brE0: USES EARLIER 4 BYTE ADDRESS AND 2 BYTE LENGTH TO LOG THE MUT TABLE
mov #1,r0
mov.l (_DMAOPFLAG2),r10
mov.l r0,@r10
mov.l (_DMAaddress),r0
mov.l @r0,r0
mov.l (_SAR3),r10
mov.l r0,@r10
mov.l (_TDR0),r0
mov.l (_DAR3),r10
mov.l r0,@r10
mov.l (_DMAlength),r0
mov.w @r0,r0
mov.l (_DMATCR3),r10
mov.l r0,@r10
mov #0x37,r0
mov.l (_DMAOPFLAG),r10
mov.l r0,@r10
mov.w (_SCR0_CLRRE_SETTIE),r0
mov.l (_SCR0),r10
mov.b r0,@r10
mov.l (_DMA3CONFIGwriteindirect),r0
mov.l (_CHCR3),r10
mov.l r0,@r10
mov.l (_int_enable),r10
jsr @r10
nop
mov.l @r15+,r10
mov.l @r15+,r0
lds.l @r15+,pr
rte
nop

brE1: USES PREVIOUS 4 BYTE ADDRESS AND 2 BYTE LENGTH TO WRITE A BLOCK OF RAM TO THE SERIAL PORT
mov #1,r0
mov.l (_DMAOPFLAG2),r10
mov.l r0,@r10
mov.l (_DMAaddress),r0
mov.l @r0,r0
mov.l (_SAR3),r10
mov.l r0,@r10
mov.l (_TDR0),r0
mov.l (_DAR3),r10
mov.l r0,@r10
mov.l (_DMAlength),r0
mov.w @r0,r0
mov.l (_DMATCR3),r10
mov.l r0,@r10
mov #0x37,r0
mov.l (_DMAOPFLAG),r10
mov.l r0,@r10
mov.w (_SCR0_CLRRE_SETTIE),r0
mov.l (_SCR0),r10
mov.b r0,@r10
mov.l (_DMA3CONFIGwritedirect),r0
mov.l (_CHCR3),r10
mov.l r0,@r10
mov.l (_int_enable),r10
jsr @r10
nop
mov.l @r15+,r10
mov.l @r15+,r0
lds.l @r15+,pr
rte
nop

brE2: USES PREVIOUS 4 BYTE ADDRESS AND 2 BYTE LENGTH TO READ A BLOCK FROM SERIAL PORT AND WRITE IT TO RAM
mov #2,r0
mov.l (_DMAOPFLAG2),r10
mov.l r0,@r10
mov.l (_RDR0),r0
mov.l (_SAR3),r10
mov.l r0,@r10
mov.l (_DMAaddress),r0
mov.l @r0,r0
mov.l (_DAR3),r10
mov.l r0,@r10
mov.l (_DMAlength),r0
mov.w @r0,r0
mov.l (_DMATCR3),r10
mov.l r0,@r10
mov #0x37,r0
mov.l (_DMAOPFLAG),r10
mov.l r0,@r10
mov.l (_SSR0),r10
mov.b @r10,r0
and #0x87,r0
mov.b r0,@r10
mov.l (_DMA3CONFIGread),r0
mov.l (_CHCR3),r10
mov.l r0,@r10
mov.l r0,@r10
mov.l (_int_enable),r10
jsr @r10
nop
mov.l @r15+,r10 STACK
mov.l @r15+,r0
lds.l @r15+,pr
rte RETURN FROM EXCEPTION/INTERRUPT
nop

.align 4
TEIE: TRANSMIT END INTERRUPT
sts.l pr,@-r15
mov.l r0,@-r15
mov.l r10,@-r15
mov.l (_int_disable),r10
jsr @r10
nop
TEIEinvade: JUMP HERE FROM EARLIER IF TRANSMISSION HAS ALREADY FINISHED
mov #0,r0
mov.l (_DMAOPFLAG),r10 CLEAR OUR 0X37 VARIABLE
mov.l r0,@r10
mov.w (_SCR0_SETRE_CLRTEIE),r0 RESET SERIAL PORT INTERRUPT CONFIG
mov.l (_SCR0),r10
mov.b r0,@r10

mov.l (_SSR0),r10 RESET SERIAL PORT STATUS FOR NEXT COMMS
mov.b @r10,r0
and #0x87,r0
mov.b r0,@r10

mov.l (_int_enable),r10
jsr @r10
nop
mov.l @r15+,r10 STACK
mov.l @r15+,r0
lds.l @r15+,pr
rte
nop

.align 2

_SAR3:
.long 0xffffecf0
_DAR3:
.long 0xffffecf4
_DMATCR3:
.long 0xffffecf8
_CHCR3:
.long 0xffffecfc

_SCR0:
.long 0xfffff002
_TDR0:
.long 0xfffff003
_SSR0:
.long 0xfffff004
_RDR0:
.long 0xfffff005

_DMA3CONFIGwriteindirect:
.long 0x10011005
_DMA3CONFIGwritedirect:
.long 0x11005
_DMA3CONFIGread:
.long 0x20105

_DMAOPFLAG:
.long 0xffff8480
_DMAOPFLAG2:
.long 0xffff8484
_DMAaddress:
.long 0xffff8488
_DMAlength:
.long 0xffff848c

_int_disable:
.long 0x400
_int_enable:
.long 0x41e

_E0:
.word 0xE0
_E1:
.word 0xE1
_E2:
.word 0xE2

_SCR0_CLRTIE_SETTEIE:
.word 0x24
_SCR0_SETRE_CLRTEIE:
.word 0x70
_SCR0_CLRRE_SETTIE:
.word 0xa0

/*--------------------------------*/
.align 4
COPY: COPY ROM TO RAM IF TEPHRA'S DEAD VARIABLE IS NOT 0XDEAD
sts.l pr,@-r15
mov.l r0,@-r15
mov.l r1,@-r15
mov.l r2,@-r15
mov.l r10,@-r15
mov.l r11,@-r15

mov.w (DEADval),r0
mov.l (DEADloc),r1
mov.w @r1,r1
cmp/eq r1,r0
bt TIMEOUT
nop

mov.l (ROM),r10
mov.l (RAM),r11
mov.l (LENGTH),r1
mov #0,r0
loop:
mov.l @(r0,r10),r2
mov.l r2,@(r0,r11)
add #4,r0
cmp/hs r1,r0
bf loop
nop

TIMEOUT: KILLS DMA IF COMMS HAVE BEEN KILLED
mov.l (__int_disable),r10
jsr @r10
nop

mov.l (__bit7allowslogging),r10
mov.w @r10,r0
tst #0x80,r0
bf __exit
nop

mov #0xfffffffc,r0
mov.l (__CHCR3),r10
mov.l @r10,r10
and r10,r0
mov.l (__CHCR3),r10
mov.l r0,@r10

mov #0,r0
mov.l (__DMAOPFLAG),r10
mov.l r0,@r10

__exit:
mov.l (__int_enable),r10
jsr @r10
nop
mov.l @r15+,r11
mov.l @r15+,r10
mov.l @r15+,r2
mov.l @r15+,r1
mov.l @r15+,r0
lds.l @r15+,pr

mov.l (tephra),r10
jmp @r10
nop

.align 2
DEADloc:
.long 0xffff841c
__CHCR3:
.long 0xffffecfc
__DMAOPFLAG:
.long 0xffff8480
__bit7allowslogging: /*CHANGE FOR EACH ROM*/
.long 0xffff6fea
__int_disable:
.long 0x400
__int_enable:
.long 0x41e
tephra: /*CHANGE FOR 256K ECUS*/
.long 0x48000
ROM:
.long 0x37b00
RAM:
.long 0xFFFF8500
LENGTH:
.long 0x800
DEADval:
.word 0xDEAD
