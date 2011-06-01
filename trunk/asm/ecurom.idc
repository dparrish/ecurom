// Auto-Disassembly script for Mitsubishi Evo ROMs.
//
// This is based on Acamus's Auto Disassembler.
//

// Change History
// 31/05/11 dparrish (david@dparrish.com):
//  + split out from onload.idc and reformat to be consistent
//  + add sh7052 registers

#include <idc.idc>
#include <memcpy.idc>

#define IVT_START      0x0000
#define H8_ROM_START   0x0140
#define H8_CODE_OFFSET 0x10000
#define SH_ROM_START   0x0400
#define M32R_ROM_START 0x0080

static FixH8500() {
  auto ea, end;

  Message("H8/500 create segment %x-%x\n", 0x10000, 0x1ffff);
  SegCreate(0x10000, 0x1ffff, 0x0, 0, 1, 2);
  SegRename(0x10000, "seg001");
  SegClass (0x10000, "CODE");
  SegDefReg(0x10000, "br", 0x0);
  SegDefReg(0x10000, "dp", 0x1);
  SetSegmentType(0x10000, 2);

  //Message("H8/500 create segment %x-%x\n", 0x14000, 0x1ffff);
  //SegCreate(0x14000, 0x20000, 0x0, 0, 1, 2);
  //SegRename(0x14000, "seg001");
  //SegClass (0x14000, "CODE");
  //SegDefReg(0x14000, "br", 0x0);
  //SegDefReg(0x14000, "dp", 0x1);
  //SetSegmentType(0x14000, 2);

  Message("H8/500 create segment %x-%x\n", 0x20000, 0x2ffff);
  SegCreate(0x20000, 0x2ffff, 0x0, 0, 1, 2);
  SegRename(0x20000, "seg002");
  SegClass (0x20000, "CODE");
  SegDefReg(0x20000, "br", 0x0);
  SegDefReg(0x20000, "dp", 0x2);
  SetSegmentType(0x20000, 2);

  Message("H8/500 Create RAM\n");
  SegCreate(0xEE80, 0xFFFF, 0, 1, saRelWord, 0);
  SegRename(0XEE80, "RAM");

  LowVoids(ea);
  HighVoids(end + H8_CODE_OFFSET);
  Message("H8/500 Perform Vector Table Fixups \n");
  Fixup_VT(0x10000, H8_ROM_START);
  Message("H8/500 Mitsubishi ECU autonalysis finished.\n");
}

static H8RegisterNames() {
  MakeName(0xFEC0, "SCI3_SMR");
  MakeName(0xFEC1, "SCI3_BRR");
  MakeName(0xFEC2, "SCI3_SCR");
  MakeName(0xFEC3, "SCI3_TDR");
  MakeName(0xFEC4, "SCI3_SSR");
  MakeName(0xFEC5, "SCI3_RDR");

  MakeName(0xFEC8, "SCI1_SMR");
  MakeName(0xFEC9, "SCI1_BRR");
  MakeName(0xFECA, "SCI1_SCR");
  MakeName(0xFECB, "SCI1_TDR");
  MakeName(0xFECC, "SCI1_SSR");
  MakeName(0xFECD, "SCI1_RDR");

  MakeName(0xFED0, "SCI2_SMR");
  MakeName(0xFED1, "SCI2_BRR");
  MakeName(0xFED2, "SCI2_SCR");
  MakeName(0xFED3, "SCI2_TDR");
  MakeName(0xFED4, "SCI2_SSR");
  MakeName(0xFED5, "SCI2_RDR");

  MakeName(0xFE80, "P1DDR");
  MakeName(0xFE81, "P2DDR");
  MakeName(0xFE82, "P1DR");
  MakeName(0xFE83, "P2DR");
  MakeName(0xFE84, "P3DDR");
  MakeName(0xFE85, "P4DDR");
  MakeName(0xFE86, "P3DR");
  MakeName(0xFE87, "P4DR");
  MakeName(0xFE88, "P5DDR");
  MakeName(0xFE89, "P6DDR");
  MakeName(0xFE8A, "P5DR");
  MakeName(0xFE8B, "P6DR");
  MakeName(0xFE8C, "P7DDR");
  MakeName(0xFE8E, "P7DR");
  MakeName(0xFE8F, "P8DR");

  MakeName(0xFE91, "PADDR");
  MakeName(0xFE92, "P9DR");
  MakeName(0xFE93, "PADR");
  MakeName(0xFE94, "PBDDR");
  MakeName(0xFE95, "PCDDR");
  MakeName(0xFE96, "PBDR");
  MakeName(0xFE97, "PCDR");
  MakeName(0xFE98, "PBPCR");
  MakeName(0xFE99, "PCPCR");
  MakeName(0xFE9A, "oCR");

  MakeName(0xFEA0, "ADDR0H");
  MakeName(0xFEA1, "ADDR0L");
  MakeName(0xFEA2, "ADDR1H");
  MakeName(0xFEA3, "ADDR1L");
  MakeName(0xFEA4, "ADDR2H");
  MakeName(0xFEA5, "ADDR2L");
  MakeName(0xFEA6, "ADDR3H");
  MakeName(0xFEA7, "ADDR3L");
  MakeName(0xFEA8, "ADDR4H");
  MakeName(0xFEA9, "ADDR4L");
  MakeName(0xFEAA, "ADDR5H");
  MakeName(0xFEAB, "ADDR5L");
  MakeName(0xFEAC, "ADDR6H");
  MakeName(0xFEAD, "ADDR6L");
  MakeName(0xFEAE, "ADDR7H");
  MakeName(0xFEAF, "ADDR7L");

  MakeName(0xFEB0, "ADDR8H");
  MakeName(0xFEB1, "ADDR8L");
  MakeName(0xFEB2, "ADDR9H");
  MakeName(0xFEB3, "ADDR9L");
  MakeName(0xFEB4, "ADDRAH");
  MakeName(0xFEB5, "ADDRAL");
  MakeName(0xFEB6, "ADDRBH");
  MakeName(0xFEB7, "ADDRBL");
  MakeName(0xFEB8, "ADCSR");
  MakeName(0xFEB9, "ADCR");

  MakeName(0xFEDA, "PACR");
  MakeName(0xFEDB, "P67CR");
  MakeName(0xFEDC, "ADTRGR");
  MakeName(0xFEDE, "IRQFR");
  MakeName(0xFEDF, "BCR");

  MakeName(0xFEE0, "FLMCR");
  MakeName(0xFEE1, "FLM_EBR1");
  MakeName(0xFEE2, "FLM_EBR2");
  MakeName(0xFEEC, "FLMER");
  MakeName(0xFEED, "FLMSR");

  MakeName(0xFEF0, "PWM1_TCR");
  MakeName(0xFEF1, "PWM1_DTR");
  MakeName(0xFEF2, "PWM1_TCNT");
  MakeName(0xFEF4, "PWM2_TCR");
  MakeName(0xFEF5, "PWM2_DTR");
  MakeName(0xFEF6, "PWM2_TCNT");
  MakeName(0xFEF8, "PWM3_TCR");
  MakeName(0xFEF9, "PWM3_DTR");
  MakeName(0xFEFA, "PWM3_TCNT");

  MakeName(0xFF00, "INTC_IPRA");
  MakeName(0xFF01, "INTC_IPRB");
  MakeName(0xFF02, "INTC_IPRC");
  MakeName(0xFF03, "INTC_IPRD");
  MakeName(0xFF04, "INTC_IPRE");
  MakeName(0xFF05, "INTC_IPRF");
  MakeName(0xFF08, "DTC_DTEA");
  MakeName(0xFF09, "DTC_DTEB");
  MakeName(0xFF0A, "DTC_DTEC");
  MakeName(0xFF0B, "DTC_DTED");
  MakeName(0xFF0C, "DTC_DTEE");
  MakeName(0xFF0D, "DTC_DTEF");

  MakeName(0xFF10, "WDT__TCSR");
  MakeName(0xFF11, "WDT_TCNT");
  MakeName(0xFF14, "WSC_WCR");
  MakeName(0xFF15, "RAMCR");
  MakeName(0xFF16, "BSC_ARBT");
  MakeName(0xFF17, "BSC_AR3T");
  MakeName(0xFF19, "SYSC_MDCR");
  MakeName(0xFF1A, "SYSC_SBYCR");
  MakeName(0xFF1B, "SYSC_BRCR");
  MakeName(0xFF1C, "SYSC_NMICR");
  MakeName(0xFF1D, "SYSC_IRQCR");
  MakeName(0xFF1E, "SYSC_writeCR");
  MakeName(0xFF1F, "SYSC_RSTCSR");

  MakeName(0xFF20, "T1CRH");
  MakeName(0xFF21, "T1CRL");
  MakeName(0xFF22, "T1SRAH");
  MakeName(0xFF23, "T1SRAL");
  MakeName(0xFF24, "T1OERA");
  MakeName(0xFF25, "TMDRA");
  MakeName(0xFF26, "T1CNTH");
  MakeName(0xFF27, "T1CNTL");
  MakeName(0xFF28, "T1GR1H");
  MakeName(0xFF29, "T1GR1L");
  MakeName(0xFF2A, "T1GR2H");
  MakeName(0xFF2B, "T1GR2L");
  MakeName(0xFF2C, "T1DR1H");
  MakeName(0xFF2D, "T1DR1L");
  MakeName(0xFF2E, "T1DR2H");
  MakeName(0xFF2F, "T1DR2L");

  MakeName(0xFF30, "TSTR");
  MakeName(0xFF31, "T1CRA");
  MakeName(0xFF32, "T1SRBH");
  MakeName(0xFF33, "T1SRBL");
  MakeName(0xFF34, "T1OERB");
  MakeName(0xFF35, "TMDRB");
  MakeName(0xFF38, "T1GR3H");
  MakeName(0xFF39, "T1GR3L");
  MakeName(0xFF3A, "T1GR4H");
  MakeName(0xFF3B, "T1GR4L");
  MakeName(0xFF3C, "T1DR3H");
  MakeName(0xFF3D, "T1DR3L");
  MakeName(0xFF3E, "T1DR4H");
  MakeName(0xFF3F, "T1DR4L");

  MakeName(0xFF40, "T2CRH");
  MakeName(0xFF41, "T2CRL");
  MakeName(0xFF42, "T2SRH");
  MakeName(0xFF43, "T2SRL");
  MakeName(0xFF44, "T2OER");
  MakeName(0xFF46, "T2CNTH");
  MakeName(0xFF47, "T2CNTL");
  MakeName(0xFF48, "T2GR1H");
  MakeName(0xFF49, "T2GR1L");
  MakeName(0xFF4A, "T2GR2H");
  MakeName(0xFF4B, "T2GR2L");
  MakeName(0xFF4C, "T2DR1H");
  MakeName(0xFF4D, "T2DR1L");
  MakeName(0xFF4E, "T2DR2H");
  MakeName(0xFF4F, "T2DR2L");

  MakeName(0xFF50, "T3CRH");
  MakeName(0xFF51, "T3CRL");
  MakeName(0xFF52, "T3SRH");
  MakeName(0xFF53, "T3SRL");
  MakeName(0xFF54, "T3OER");
  MakeName(0xFF56, "T3CNTH");
  MakeName(0xFF57, "T3CNTL");
  MakeName(0xFF58, "T3GR1H");
  MakeName(0xFF59, "T3GR1L");
  MakeName(0xFF5A, "T3GR2H");
  MakeName(0xFF5B, "T3GR2L");
  MakeName(0xFF5C, "T3DR1H");
  MakeName(0xFF5D, "T3DR1L");
  MakeName(0xFF5E, "T3DR2H");
  MakeName(0xFF5F, "T3DR2L");

  MakeName(0xFF60, "T4CRH");
  MakeName(0xFF61, "T4CRL");
  MakeName(0xFF62, "T4SRH");
  MakeName(0xFF63, "T4SRL");
  MakeName(0xFF64, "T4OER");
  MakeName(0xFF66, "T4CNTH");
  MakeName(0xFF67, "T4CNTL");
  MakeName(0xFF68, "T4GR1H");
  MakeName(0xFF69, "T4GR1L");
  MakeName(0xFF6A, "T4GR2H");
  MakeName(0xFF6B, "T4GR2L");
  MakeName(0xFF6C, "T4DR1H");
  MakeName(0xFF6D, "T4DR1L");
  MakeName(0xFF6E, "T4DR2H");
  MakeName(0xFF6F, "T4DR2L");

  MakeName(0xFF70, "T5CRH");
  MakeName(0xFF71, "T5CRL");
  MakeName(0xFF72, "T5SRH");
  MakeName(0xFF73, "T5SRL");
  MakeName(0xFF74, "T5OER");
  MakeName(0xFF76, "T5CNTH");
  MakeName(0xFF77, "T5CNTL");
  MakeName(0xFF78, "T5GR1H");
  MakeName(0xFF79, "T5GR1L");
  MakeName(0xFF7A, "T5GR2H");
  MakeName(0xFF7B, "T5GR2L");
  MakeName(0xFF7C, "T5DR1H");
  MakeName(0xFF7D, "T5DR1L");
  MakeName(0xFF7E, "T5DR2H");
  MakeName(0xFF7F, "T5DR2L");

  MakeName(0xFF80, "T6CRH");
  MakeName(0xFF81, "T6CRL");
  MakeName(0xFF82, "T6SRH");
  MakeName(0xFF83, "T6SRL");
  MakeName(0xFF84, "T6OER");
  MakeName(0xFF86, "T6CNTH");
  MakeName(0xFF87, "T6CNTL");
  MakeName(0xFF88, "T6GR1H");
  MakeName(0xFF89, "T6GR1L");
  MakeName(0xFF8A, "T6GR2H");
  MakeName(0xFF8B, "T6GR2L");

  MakeName(0xFF90, "T7CRH");
  MakeName(0xFF91, "T7CRL");
  MakeName(0xFF92, "T7SRH");
  MakeName(0xFF93, "T7SRL");
  MakeName(0xFF94, "T7OER");
  MakeName(0xFF96, "T7CNTH");
  MakeName(0xFF97, "T7CNTL");
  MakeName(0xFF98, "T7GR1H");
  MakeName(0xFF99, "T7GR1L");
  MakeName(0xFF9A, "T7GR2H");
  MakeName(0xFF9B, "T7GR2L");

  MakeName(0xFFA0, "MLTCR");
  MakeName(0xFFA1, "MLTBR");
  MakeName(0xFFA2, "MLTMAR");
  MakeName(0xFFA3, "MLTAR");

  MakeName(0xFFB0, "MULT_CA");
  MakeName(0xFFB1, "MULT_(CA)");
  MakeName(0xFFB2, "MULT_CB");
  MakeName(0xFFB3, "MULT_(CB)");
  MakeName(0xFFB4, "MULT_CC");
  MakeName(0xFFB5, "MULT_(CC)");
  MakeName(0xFFB6, "MULT_XH");
  MakeName(0xFFB7, "MULT_(XH)");
  MakeName(0xFFB8, "MULT_H");
  MakeName(0xFFB9, "MULT_(H)");
  MakeName(0xFFBA, "MULT_L");
  MakeName(0xFFBB, "MULT_(L)");
  MakeName(0xFFBC, "MULT_MR");
  MakeName(0xFFBD, "MULT_(MR)");
  MakeName(0xFFBE, "MULT_MMR");
  MakeName(0xFFBF, "MULT_(MMR)");
}

static SegmentsSH7052F() {
}

static SegmentsSA4B() {
  AddSeg(0x00000000, 0x00040000, 0x0, 2, saAbs, scPriv);
  SegRename(0x00000000, "OnChipROM");
  SegClass(0x00000000, "CODE");
  SegDefReg(0x00000000, "br", 0x0);
  SegDefReg(0x00000000, "dp", 0x1);
  AddSeg(0x00040000, 0x00040000, 0x0, 2, saAbs, scPriv);
  SegRename(0x00040000, "Reserved 1");
  AddSeg(0x00200000, 0x00400000, 0x0, 2, saAbs, scPriv);
  SegRename(0x00200000, "CS0");
  AddSeg(0x00400000, 0x00800000, 0x0, 2, saAbs, scPriv);
  SegRename(0x00400000, "CS1");
  AddSeg(0x00800000, 0x00C00000, 0x0, 2, saAbs, scPriv);
  SegRename(0x00800000, "CS2");
  AddSeg(0x00C00000, 0x01000000, 0x0, 2, saAbs, scPriv);
  SegRename(0x00C00000, "CS3");
  AddSeg(0x01000000, 0x01000000, 0x0, 2, saAbs, scPriv);
  SegRename(0x01000000, "Reserved 2");
  SegCreate(0xFFFF6000, 0xFFFFFFFF, 0, 1, saRelWord, 0);
  AddSeg(0xFFFF8000, 0xFFFFE000, 0x0, 2, saAbs, scPriv);
  SegRename(0xFFFF8000, "RAM");
  AddSeg(0xFFFFE000, 0xFFFFFFFF, 0x0, 2, saAbs, scPriv);
  SegRename(0xFFFFE000, "On-chip peripheral module");
  Message("Segments created\n");
}

static FixSH7052F() {
  FixConstants();
  Wait();
  Fixup_VT(0x0000, SH_ROM_START);
  Wait();
  FixupJumps();
  Wait();
  Fix_Missing_Code(SH_ROM_START, 0xF20, 1);
  Fix_Missing_Code(Word(2), SegEnd(Word(2)), 0);
  Wait();
  Fix_MUT_Table();
  Wait();
  FixDataOffsets();
  Wait();
  SH7052RegisterNames();
  Wait();
  Message("SH7052F Mitsubishi ECU autoanalysis finished\n");
}

static FixSH4B() {
  FixConstants();
  Wait();
  Fixup_VT(0x0000, SH_ROM_START);
  Wait();
  FixupJumps();
  Wait();
  Fix_Missing_Code(SH_ROM_START, 0xF20, 1);
  Fix_Missing_Code(Word(2), SegEnd(Word(2)), 0);
  Wait();
  Fix_MUT_Table();
  Wait();
  FixDataOffsets();
  Wait();
  SH7052RegisterNames();
  Wait();
  Message("SH4B Mitsubishi ECU autoanalysis finished\n");
}

static ByteRegister(ea, name, comment) {
  MakeByte(ea);
  MakeNameEx(ea, name, SN_NOLIST);
  if (comment != "")
    MakeComm(ea, comment);
  return ea;
}

static WordRegister(ea, name, comment) {
  MakeWord(ea);
  MakeNameEx(ea, name, SN_NOLIST);
  if (comment != "")
    MakeComm(ea, comment);
  return ea;
}

static LongRegister(ea, name, comment) {
  MakeDword(ea);
  MakeNameEx(ea, name, SN_NOLIST);
  if (comment != "")
    MakeComm(ea, comment);
  return ea;
}

static ReservedSpaceArray(start, end) {
  MakeByte(start);
  MakeArray(start, end - start + 1);
  SetArrayFormat(start, AP_ALLOWDUPS, 0, -1);
  MakeComm(start, "Reserved");
  return start;
}

static RegisterArray(ea, count) {
  MakeArray(ea, count);
  SetArrayFormat(ea, AP_ALLOWDUPS, count, -1);
}

static SH7052RegisterNames() {
  auto id;
  Message("Labelling SH7052 Registers... ");

  // Interrupt Controller (INTC)
  MakeComm(0xFFFFED00, "Interrupt Priority Registers");
  WordRegister(0xFFFFED00, "IPRA", "");
  WordRegister(0xFFFFED04, "IPRC", "");
  WordRegister(0xFFFFED06, "IPRD", "");
  WordRegister(0xFFFFED08, "IPRE", "");
  WordRegister(0xFFFFED0A, "IPRF", "");
  WordRegister(0xFFFFED0C, "IPRG", "");
  WordRegister(0xFFFFED0E, "IPRH", "");
  WordRegister(0xFFFFED10, "IPRI", "");
  WordRegister(0xFFFFED12, "IPRJ", "");
  WordRegister(0xFFFFED14, "IPRK", "");
  WordRegister(0xFFFFED16, "IPRL", "");
  WordRegister(0xFFFFED18, "ICR", "");
  WordRegister(0xFFFFED1A, "ISR", "");

  MakeComm(0x0000002C, "Interrupt Exception Processing Vectors");
  LongRegister(0x0000002C, "NMI", "");
  LongRegister(0x00000030, "UBC", "");
  LongRegister(0x00000100, "IRQ0", "");
  LongRegister(0x00000104, "IRQ1", "");
  LongRegister(0x00000108, "IRQ2", "");
  LongRegister(0x0000010C, "IRQ3", "");
  LongRegister(0x00000120, "DMAC0", "");
  LongRegister(0x00000128, "DMAC1", "");
  LongRegister(0x00000130, "DMAC2", "");
  LongRegister(0x00000138, "DMAC3", "");
  LongRegister(0x000002F0, "CMTI0", "");
  LongRegister(0x000002F8, "ADI0", "");
  LongRegister(0x00000300, "CMTI1", "");
  LongRegister(0x00000308, "ADI1", "");
  LongRegister(0x00000380, "ITI", "");
  // ATU0
  LongRegister(0x00000140, "ITV1", "");
  LongRegister(0x00000150, "ICI0A", "");
  LongRegister(0x00000158, "ICI0B", "");
  LongRegister(0x00000160, "ICI0C", "");
  LongRegister(0x00000168, "ICI0D", "");
  LongRegister(0x00000170, "OVI0", "");
  // ATU1
  LongRegister(0x00000180, "IMI1A", "");
  LongRegister(0x00000184, "IMI1B", "");
  LongRegister(0x00000188, "IMI1C", "");
  LongRegister(0x0000018C, "IMI1D", "");
  LongRegister(0x00000190, "IMI1E", "");
  LongRegister(0x00000194, "IMI1F", "");
  LongRegister(0x00000198, "IMI1G", "");
  LongRegister(0x0000019C, "IMI1H", "");
  LongRegister(0x000001A0, "OVI1A", "");
  // ATU2
  LongRegister(0x000001B0, "IMI2A", "");
  LongRegister(0x000001B4, "IMI2B", "");
  LongRegister(0x000001B8, "IMI2C", "");
  LongRegister(0x000001BC, "IMI2D", "");
  LongRegister(0x000001C0, "IMI2E", "");
  LongRegister(0x000001C4, "IMI2F", "");
  LongRegister(0x000001C8, "IMI2G", "");
  LongRegister(0x000001CC, "IMI2H", "");
  LongRegister(0x000001D0, "OVI2A", "");
  // ATU3
  LongRegister(0x000001E0, "IMI3A", "");
  LongRegister(0x000001E4, "IMI3B", "");
  LongRegister(0x000001E8, "IMI3C", "");
  LongRegister(0x000001EC, "IMI3D", "");
  LongRegister(0x000001F0, "OVI3", "");
  // ATU4
  LongRegister(0x00000200, "IMI4A", "");
  LongRegister(0x00000204, "IMI4B", "");
  LongRegister(0x00000208, "IMI4C", "");
  LongRegister(0x0000020C, "IMI4D", "");
  LongRegister(0x00000210, "OVI4", "");
  // ATU5
  LongRegister(0x00000220, "IMI5A", "");
  LongRegister(0x00000224, "IMI5B", "");
  LongRegister(0x00000228, "IMI5C", "");
  LongRegister(0x0000022C, "IMI5D", "");
  LongRegister(0x00000230, "OVI5", "");
  // ATU6
  LongRegister(0x00000240, "CMI6A", "");
  LongRegister(0x00000244, "CMI6B", "");
  LongRegister(0x00000248, "CMI6C", "");
  LongRegister(0x0000024C, "CMI6D", "");
  // ATU7
  LongRegister(0x00000250, "CMI7A", "");
  LongRegister(0x00000254, "CMI7B", "");
  LongRegister(0x00000258, "CMI7C", "");
  LongRegister(0x0000025C, "CMI7D", "");
  // ATU8
  LongRegister(0x00000260, "OSI8A", "");
  LongRegister(0x00000264, "OSI8B", "");
  LongRegister(0x00000268, "OSI8C", "");
  LongRegister(0x0000026C, "OSI8D", "");
  LongRegister(0x00000270, "OSI8E", "");
  LongRegister(0x00000274, "OSI8F", "");
  LongRegister(0x00000278, "OSI8G", "");
  LongRegister(0x0000027C, "OSI8H", "");
  LongRegister(0x00000280, "OSI8I", "");
  LongRegister(0x00000284, "OSI8J", "");
  LongRegister(0x00000288, "OSI8K", "");
  LongRegister(0x0000028C, "OSI8L", "");
  LongRegister(0x00000290, "OSI8M", "");
  LongRegister(0x00000294, "OSI8N", "");
  LongRegister(0x00000298, "OSI8O", "");
  LongRegister(0x0000029C, "OSI8P", "");
  // ATU9
  LongRegister(0x000002A0, "CMI9A", "");
  LongRegister(0x000002A4, "CMI9B", "");
  LongRegister(0x000002A8, "CMI9C", "");
  LongRegister(0x000002AC, "CMI9D", "");
  LongRegister(0x000002B0, "CMI9E", "");
  LongRegister(0x000002B8, "CMI9F", "");
  // ATU10
  LongRegister(0x000002C0, "CMI10A", "");
  LongRegister(0x000002C8, "CMI10B", "");
  LongRegister(0x000002D0, "ICI10A", "");
  // ATU11
  LongRegister(0x000002E0, "IMI11A", "");
  LongRegister(0x000002E8, "IMI11B", "");
  LongRegister(0x000002EC, "OVI11", "");
  // SCI0
  LongRegister(0x00000320, "ERI0", "");
  LongRegister(0x00000324, "RXI0", "");
  LongRegister(0x00000328, "TXI0", "");
  LongRegister(0x0000032C, "TEI0", "");
  // SCI1
  LongRegister(0x00000330, "ERI1", "");
  LongRegister(0x00000334, "RXI1", "");
  LongRegister(0x00000338, "TXI1", "");
  LongRegister(0x0000033C, "TEI1", "");
  // SCI2
  LongRegister(0x00000340, "ERI2", "");
  LongRegister(0x00000344, "RXI2", "");
  LongRegister(0x00000348, "TXI2", "");
  LongRegister(0x0000034C, "TEI2", "");
  // SCI3
  LongRegister(0x00000350, "ERI3", "");
  LongRegister(0x00000354, "RXI3", "");
  LongRegister(0x00000358, "TXI3", "");
  LongRegister(0x0000035C, "TEI3", "");
  // SCI4
  LongRegister(0x00000360, "ERI4", "");
  LongRegister(0x00000364, "RXI4", "");
  LongRegister(0x00000368, "TXI4", "");
  LongRegister(0x0000036C, "TEI4", "");
  // HCAN
  LongRegister(0x00000370, "ERS", "");
  LongRegister(0x00000374, "OVR", "");
  LongRegister(0x00000378, "RM", "");
  LongRegister(0x0000037C, "SLE", "");


  // User Break Controller (UBC)
  WordRegister(0xFFFFEC00, "UBARH", "");
  WordRegister(0xFFFFEC02, "UBARL", "");
  WordRegister(0xFFFFEC04, "UBAMRH", "");
  WordRegister(0xFFFFEC06, "UBAMRL", "");
  WordRegister(0xFFFFEC08, "UBBR", "");
  WordRegister(0xFFFFEC0A, "UBCE", "");


  // Direct Memory Access Controller (DMAC)
  LongRegister(0xFFFFECB0, "DMAOR", "DMA operation register");
  LongRegister(0xFFFFECC0, "SAR0", "DMA source address register");
  LongRegister(0xFFFFECC4, "DAR0", "DMA destination address register");
  LongRegister(0xFFFFECC8, "DMATCR0", "DMA transfer count register");
  LongRegister(0xFFFFECCC, "CHCR0", "DMA channel control register");
  LongRegister(0xFFFFECD0, "SAR1", "");
  LongRegister(0xFFFFECD4, "DAR1", "");
  LongRegister(0xFFFFECD8, "DMATCR1", "");
  LongRegister(0xFFFFECDC, "CHCR1", "");
  LongRegister(0xFFFFECE0, "SAR2", "");
  LongRegister(0xFFFFECE4, "DAR2", "");
  LongRegister(0xFFFFECE8, "DMATCR2", "");
  LongRegister(0xFFFFECEC, "CHCR2", "");
  LongRegister(0xFFFFECF0, "SAR3", "");
  LongRegister(0xFFFFECF4, "DAR3", "");
  LongRegister(0xFFFFECF8, "DMATCR3", "");
  LongRegister(0xFFFFECFC, "CHCR3", "");


  // Advanced Timer Unit-II (ATU-II)
  // Channel 0
  ByteRegister(0xFFFFF401, "TSTR1", "Timer start register 1");
  ByteRegister(0xFFFFF400, "TSTR2", "");
  ByteRegister(0xFFFFF402, "TSTR3", "");
  ByteRegister(0xFFFFF404, "PSCR1", "Prescaler register 1");
  ByteRegister(0xFFFFF406, "PSCR2", "");
  ByteRegister(0xFFFFF408, "PSCR3", "");
  ByteRegister(0xFFFFF40A, "PSCR4", "");
  WordRegister(0xFFFFF430, "TCNT0H", "Free-running counter 0H");
  WordRegister(0xFFFFF432, "TCNT0L", "");
  WordRegister(0xFFFFF434, "ICR0AH", "Input capture register 0AH");
  WordRegister(0xFFFFF436, "ICR0AL", "Input capture register 0AL");
  WordRegister(0xFFFFF438, "ICR0BH", "");
  WordRegister(0xFFFFF43A, "ICR0BL", "");
  WordRegister(0xFFFFF43C, "ICR0CH", "");
  WordRegister(0xFFFFF43E, "ICR0CL", "");
  WordRegister(0xFFFFF420, "ICR0DH", "");
  WordRegister(0xFFFFF422, "ICR0DL", "");
  ByteRegister(0xFFFFF424, "ITVRR1", "Timer interval interrupt request register 1");
  ByteRegister(0xFFFFF426, "ITVRR2A", "Timer interval interrupt request register 2A");
  ByteRegister(0xFFFFF428, "ITVRR2B", "");
  ByteRegister(0xFFFFF42A, "TIOR0", "Timer I/O control register");
  WordRegister(0xFFFFF42C, "TSR0", "Timer status register 0");
  WordRegister(0xFFFFF42E, "TIER0", "Timer interrupt enable register 0");
  // Channel 1
  WordRegister(0xFFFFF440, "TCNT1A", "Free-running counter 1A");
  WordRegister(0xFFFFF442, "TCNT1B", "");
  WordRegister(0xFFFFF444, "GR1A", "General register 1A");
  WordRegister(0xFFFFF446, "GR1B", "");
  WordRegister(0xFFFFF448, "GR1C", "");
  WordRegister(0xFFFFF44A, "GR1D", "");
  WordRegister(0xFFFFF44C, "GR1E", "");
  WordRegister(0xFFFFF44E, "GR1F", "");
  WordRegister(0xFFFFF450, "GR1G", "");
  WordRegister(0xFFFFF452, "GR1H", "");
  WordRegister(0xFFFFF454, "OCR1", "Output compare register 1");
  WordRegister(0xFFFFF456, "OSBR1", "Offset base register 1");
  ByteRegister(0xFFFFF459, "TIOR1A", "Timer I/O control register 1A");
  ByteRegister(0xFFFFF458, "TIOR1B", "");
  ByteRegister(0xFFFFF45B, "TIOR1C", "");
  ByteRegister(0xFFFFF45A, "TIOR1D", "");
  ByteRegister(0xFFFFF45D, "TCR1A", "Timer control register 1A");
  ByteRegister(0xFFFFF45C, "TCR1B", "");
  WordRegister(0xFFFFF45E, "TSR1A", "Timer status register 1A");
  WordRegister(0xFFFFF460, "TSR1B", "");
  WordRegister(0xFFFFF462, "TIER1A", "");
  WordRegister(0xFFFFF464, "TIER1B", "");
  ByteRegister(0xFFFFF466, "TRGMDR", "Trigger mode register");
  // Channel 2
  WordRegister(0xFFFFF600, "TCNT2A", "");
  WordRegister(0xFFFFF602, "TCNT2B", "");
  WordRegister(0xFFFFF604, "GR2A", "");
  WordRegister(0xFFFFF606, "GR2B", "");
  WordRegister(0xFFFFF608, "GR2C", "");
  WordRegister(0xFFFFF60A, "GR2D", "");
  WordRegister(0xFFFFF60C, "GR2E", "");
  WordRegister(0xFFFFF60E, "GR2F", "");
  WordRegister(0xFFFFF610, "GR2G", "");
  WordRegister(0xFFFFF612, "GR2H", "");
  WordRegister(0xFFFFF614, "OCR2A", "");
  WordRegister(0xFFFFF616, "OCR2B", "");
  WordRegister(0xFFFFF618, "OCR2C", "");
  WordRegister(0xFFFFF61A, "OCR2D", "");
  WordRegister(0xFFFFF61C, "OCR2E", "");
  WordRegister(0xFFFFF61E, "OCR2F", "");
  WordRegister(0xFFFFF620, "OCR2G", "");
  WordRegister(0xFFFFF622, "OCR2H", "");
  WordRegister(0xFFFFF624, "OSBR2", "");
  ByteRegister(0xFFFFF627, "TIOR2A", "");
  ByteRegister(0xFFFFF626, "TIOR2B", "");
  ByteRegister(0xFFFFF629, "TIOR2C", "");
  ByteRegister(0xFFFFF628, "TIOR2D", "");
  ByteRegister(0xFFFFF62B, "TCR2A", "");
  ByteRegister(0xFFFFF62A, "TCR2B", "");
  WordRegister(0xFFFFF62C, "TSR2A", "");
  WordRegister(0xFFFFF62E, "TSR2B", "");
  WordRegister(0xFFFFF630, "TIER2A", "");
  WordRegister(0xFFFFF632, "TIER2B", "");
  // Channel 3-5
  WordRegister(0xFFFFF480, "TSR3", "");
  WordRegister(0xFFFFF482, "TIER3", "");
  ByteRegister(0xFFFFF484, "TMDR", "Timer mode register");
  // Channel 3
  WordRegister(0xFFFFF4A0, "TCNT3", "");
  WordRegister(0xFFFFF4A2, "GR3A", "");
  WordRegister(0xFFFFF4A4, "GR3B", "");
  WordRegister(0xFFFFF4A6, "GR3C", "");
  WordRegister(0xFFFFF4A8, "GR3D", "");
  ByteRegister(0xFFFFF4AB, "TIOR3A", "");
  ByteRegister(0xFFFFF4AA, "TIOR3B", "");
  ByteRegister(0xFFFFF4AC, "TCR3", "");
  // Channel 4
  WordRegister(0xFFFFF4C0, "TCNT4", "");
  WordRegister(0xFFFFF4C2, "GR4A", "");
  WordRegister(0xFFFFF4C4, "GR4B", "");
  WordRegister(0xFFFFF4C6, "GR4C", "");
  WordRegister(0xFFFFF4C8, "GR4D", "");
  ByteRegister(0xFFFFF4CB, "TIOR4A", "");
  ByteRegister(0xFFFFF4CA, "TIOR4B", "");
  ByteRegister(0xFFFFF4CC, "TCR4", "");
  // Channel 5
  WordRegister(0xFFFFF4E0, "TCNT5", "");
  WordRegister(0xFFFFF4E2, "GR5A", "");
  WordRegister(0xFFFFF4E4, "GR5B", "");
  WordRegister(0xFFFFF4E6, "GR5C", "");
  WordRegister(0xFFFFF4E8, "GR5D", "");
  ByteRegister(0xFFFFF4EB, "TIOR5A", "");
  ByteRegister(0xFFFFF4EA, "TIOR5B", "");
  ByteRegister(0xFFFFF4EC, "TCR5", "");
  // Channel 6
  WordRegister(0xFFFFF500, "TCNT6A", "");
  WordRegister(0xFFFFF502, "TCNT6B", "");
  WordRegister(0xFFFFF504, "TCNT6C", "");
  WordRegister(0xFFFFF506, "TCNT6D", "");
  WordRegister(0xFFFFF508, "CYLR6A", "Cycle register 6A");
  WordRegister(0xFFFFF50A, "CYLR6B", "");
  WordRegister(0xFFFFF50C, "CYLR6C", "");
  WordRegister(0xFFFFF50E, "CYLR6D", "");
  WordRegister(0xFFFFF510, "BFR6A", "Buffer register 6A");
  WordRegister(0xFFFFF512, "BFR6B", "");
  WordRegister(0xFFFFF514, "BFR6C", "");
  WordRegister(0xFFFFF516, "BFR6D", "");
  WordRegister(0xFFFFF518, "DTR6A", "Duty register 6A");
  WordRegister(0xFFFFF51A, "DTR6B", "");
  WordRegister(0xFFFFF51C, "DTR6C", "");
  WordRegister(0xFFFFF51E, "DTR6D", "");
  ByteRegister(0xFFFFF521, "TCR6A", "");
  ByteRegister(0xFFFFF520, "TCR6B", "");
  WordRegister(0xFFFFF522, "TSR6", "");
  WordRegister(0xFFFFF524, "TIER6", "");
  ByteRegister(0xFFFFF526, "PMDR", "PWM mode register");
  // Channel 7
  WordRegister(0xFFFFF580, "TCNT7A", "");
  WordRegister(0xFFFFF582, "TCNT7B", "");
  WordRegister(0xFFFFF584, "TCNT7C", "");
  WordRegister(0xFFFFF586, "TCNT7D", "");
  WordRegister(0xFFFFF588, "CYLR7A", "");
  WordRegister(0xFFFFF58A, "CYLR7B", "");
  WordRegister(0xFFFFF58C, "CYLR7C", "");
  WordRegister(0xFFFFF58E, "CYLR7D", "");
  WordRegister(0xFFFFF590, "BFR7A", "");
  WordRegister(0xFFFFF592, "BFR7B", "");
  WordRegister(0xFFFFF594, "BFR7C", "");
  WordRegister(0xFFFFF596, "BFR7D", "");
  WordRegister(0xFFFFF598, "DTR7A", "");
  WordRegister(0xFFFFF59A, "DTR7B", "");
  WordRegister(0xFFFFF59C, "DTR7C", "");
  WordRegister(0xFFFFF59E, "DTR7D", "");
  ByteRegister(0xFFFFF5A1, "TCR7A", "");
  ByteRegister(0xFFFFF5A0, "TCR7B", "");
  WordRegister(0xFFFFF5A2, "TSR7", "");
  WordRegister(0xFFFFF5A4, "TIER7", "");
  // Channel 8
  WordRegister(0xFFFFF640, "DCNT8A", "Down-counter 8A");
  WordRegister(0xFFFFF642, "DCNT8B", "");
  WordRegister(0xFFFFF644, "DCNT8C", "");
  WordRegister(0xFFFFF646, "DCNT8D", "");
  WordRegister(0xFFFFF648, "DCNT8E", "");
  WordRegister(0xFFFFF64A, "DCNT8F", "");
  WordRegister(0xFFFFF64C, "DCNT8G", "");
  WordRegister(0xFFFFF64E, "DCNT8H", "");
  WordRegister(0xFFFFF650, "DCNT8I", "");
  WordRegister(0xFFFFF652, "DCNT8J", "");
  WordRegister(0xFFFFF654, "DCNT8K", "");
  WordRegister(0xFFFFF656, "DCNT8L", "");
  WordRegister(0xFFFFF658, "DCNT8M", "");
  WordRegister(0xFFFFF65A, "DCNT8N", "");
  WordRegister(0xFFFFF65C, "DCNT8O", "");
  WordRegister(0xFFFFF65E, "DCNT8P", "");
  WordRegister(0xFFFFF660, "RLDR8", "Reload register 8");
  WordRegister(0xFFFFF662, "TCNR", "Timer connection register");
  WordRegister(0xFFFFF664, "OTR", "One-shot pulse terminate register");
  WordRegister(0xFFFFF666, "DSTR", "Down-count start register");
  ByteRegister(0xFFFFF668, "TCR8", "");
  WordRegister(0xFFFFF66A, "TSR8", "");
  WordRegister(0xFFFFF66C, "TIER8", "");
  ByteRegister(0xFFFFF66E, "RLDENR", "Reload enable register");
  // Channel 9
  ByteRegister(0xFFFFF680, "ECNT9A", "Event counter 9A");
  ByteRegister(0xFFFFF682, "ECNT9B", "");
  ByteRegister(0xFFFFF684, "ECNT9C", "");
  ByteRegister(0xFFFFF686, "ECNT9D", "");
  ByteRegister(0xFFFFF688, "ECNT9E", "");
  ByteRegister(0xFFFFF68A, "ECNT9F", "");
  ByteRegister(0xFFFFF68C, "GR9A", "");
  ByteRegister(0xFFFFF68E, "GR9B", "");
  ByteRegister(0xFFFFF690, "GR9C", "");
  ByteRegister(0xFFFFF692, "GR9D", "");
  ByteRegister(0xFFFFF694, "GR9E", "");
  ByteRegister(0xFFFFF696, "GR9F", "");
  ByteRegister(0xFFFFF698, "TCR9A", "");
  ByteRegister(0xFFFFF69A, "TCR9B", "");
  ByteRegister(0xFFFFF69C, "TCR9C", "");
  WordRegister(0xFFFFF69E, "TSR9", "");
  WordRegister(0xFFFFF6A0, "TIER9", "");
  // Channel 10
  WordRegister(0xFFFFF6C0, "TCNT10AH", "Free-running counter 10AH");
  WordRegister(0xFFFFF6C2, "TCNT10AL", "Free-running counter 10AL");
  ByteRegister(0xFFFFF6C4, "TCNT10B", "Event counter 10B");
  WordRegister(0xFFFFF6C6, "TCNT10C", "Reload counter 10C");
  WordRegister(0xFFFFF6C8, "TCNT10D", "Correction counter 10D");
  WordRegister(0xFFFFF6CA, "TCNT10E", "Correction angle counter 10E");
  WordRegister(0xFFFFF6CC, "TCNT10F", "Correction angle counter 10F");
  WordRegister(0xFFFFF6CE, "TCNT10G", "Free-running counter 10G");
  WordRegister(0xFFFFF6D0, "ICR10AH", "Input capture register 10AH");
  WordRegister(0xFFFFF6D2, "ICR10AL", "");
  WordRegister(0xFFFFF6D4, "OCR10AH", "Output compare register 10AH");
  WordRegister(0xFFFFF6D6, "OCR10AL", "");
  ByteRegister(0xFFFFF6D8, "OCR10B", "");
  WordRegister(0xFFFFF6DA, "RLD10C", "");
  WordRegister(0xFFFFF6DC, "GRG10G", "");
  ByteRegister(0xFFFFF6DE, "TCNT10H", "Noise canceler counter 10H");
  ByteRegister(0xFFFFF6E0, "NCR10", "Noise canceler register 10");
  ByteRegister(0xFFFFF6E2, "TIOR10", "");
  ByteRegister(0xFFFFF6E4, "TCR10", "");
  WordRegister(0xFFFFF6E6, "TCCLR10", "Correction counter clear register 10");
  WordRegister(0xFFFFF6E8, "TSR10", "");
  WordRegister(0xFFFFF6EA, "TIER10", "");
  // Channel 11
  WordRegister(0xFFFFF5C0, "TCNT11", "Free-running counter 11");
  WordRegister(0xFFFFF5C2, "GR11A", "");
  WordRegister(0xFFFFF5C4, "GR11B", "");
  ByteRegister(0xFFFFF5C6, "TIOR11", "");
  ByteRegister(0xFFFFF5C8, "TCR11", "");
  WordRegister(0xFFFFF5CA, "TSR11", "");
  WordRegister(0xFFFFF5CC, "TIER11", "");


  // Advanced Pulse Controller (APC)
  WordRegister(0xFFFFF700, "POPCR", "Pulse output port control register");
  id = AddEnum(-1, "APCPINS", 0x2260000);
  SetEnumBf(id, 1);
  AddConstEx(id, "PULS0SOE", 0x01, 0x01);
  AddConstEx(id, "PULS1SOE", 0x02, 0x02);
  AddConstEx(id, "PULS2SOE", 0x04, 0x04);
  AddConstEx(id, "PULS3SOE", 0x08, 0x08);
  AddConstEx(id, "PULS4SOE", 0x10, 0x10);
  AddConstEx(id, "PULS5SOE", 0x20, 0x20);
  AddConstEx(id, "PULS6SOE", 0x40, 0x40);
  AddConstEx(id, "PULS7SOE", 0x80, 0x80);

  AddConstEx(id, "PULS0ROE", 0x01, 0x100);
  AddConstEx(id, "PULS1ROE", 0x02, 0x200);
  AddConstEx(id, "PULS2ROE", 0x04, 0x400);
  AddConstEx(id, "PULS3ROE", 0x08, 0x800);
  AddConstEx(id, "PULS4ROE", 0x10, 0x1000);
  AddConstEx(id, "PULS5ROE", 0x20, 0x2000);
  AddConstEx(id, "PULS6ROE", 0x40, 0x4000);
  AddConstEx(id, "PULS7ROE", 0x80, 0x8000);


  // Watchdog Timer (WDT)
  ByteRegister(0xFFFFEC10, "TCSR", "Timer control/status register");
  ByteRegister(0xFFFFEC11, "TCNT", "Timer counter");
  ByteRegister(0xFFFFEC12, "RSTCSR", "Reset control/status register Write");
  ByteRegister(0xFFFFEC13, "RSTCSRR", "Reset control/status register Read");


  // Compare Match Timer (CMT)
  WordRegister(0xFFFFF710, "CMSTR", "Compare match timer start register");
  WordRegister(0xFFFFF712, "CMCSR0", "Compare match timer control/status register 0");
  WordRegister(0xFFFFF714, "CMCNT0", "Compare match timer counter 0");
  WordRegister(0xFFFFF716, "CMCOR0", "Compare match timer constant register 0");
  WordRegister(0xFFFFF718, "CMCSR1", "");
  WordRegister(0xFFFFF71A, "CMCNT1", "");
  WordRegister(0xFFFFF71C, "CMCOR1", "");


  // Serial Communication Interface (SCI)
  // Channel 0
  ByteRegister(0xFFFFF000, "SMR0", "Serial mode register");
  ByteRegister(0xFFFFF001, "BRR0", "Bit rate register");
  ByteRegister(0xFFFFF002, "SCR0", "Serial control register");
  ByteRegister(0xFFFFF003, "TDR0", "Transmit data register");
  ByteRegister(0xFFFFF004, "SSR0", "Serial status register");
  ByteRegister(0xFFFFF005, "RDR0", "Receive data register");
  ByteRegister(0xFFFFF006, "SDCR0", "Serial direction control register");
  // Channel 1
  ByteRegister(0xFFFFF008, "SMR1", "");
  ByteRegister(0xFFFFF009, "BRR1", "");
  ByteRegister(0xFFFFF00A, "SCR1", "");
  ByteRegister(0xFFFFF00B, "TDR1", "");
  ByteRegister(0xFFFFF00C, "SSR1", "");
  ByteRegister(0xFFFFF00D, "RDR1", "");
  ByteRegister(0xFFFFF00E, "SDCR1", "");
  // Channel 2
  ByteRegister(0xFFFFF010, "SMR2", "");
  ByteRegister(0xFFFFF011, "BRR2", "");
  ByteRegister(0xFFFFF012, "SCR2", "");
  ByteRegister(0xFFFFF013, "TDR2", "");
  ByteRegister(0xFFFFF014, "SSR2", "");
  ByteRegister(0xFFFFF015, "RDR2", "");
  ByteRegister(0xFFFFF016, "SDCR2", "");
  // Channel 3
  ByteRegister(0xFFFFF018, "SMR3", "");
  ByteRegister(0xFFFFF019, "BRR3", "");
  ByteRegister(0xFFFFF01A, "SCR3", "");
  ByteRegister(0xFFFFF01B, "TDR3", "");
  ByteRegister(0xFFFFF01C, "SSR3", "");
  ByteRegister(0xFFFFF01D, "RDR3", "");
  ByteRegister(0xFFFFF01E, "SDCR3", "");
  // Channel 4
  ByteRegister(0xFFFFF020, "SMR4", "");
  ByteRegister(0xFFFFF021, "BRR4", "");
  ByteRegister(0xFFFFF022, "SCR4", "");
  ByteRegister(0xFFFFF023, "TDR4", "");
  ByteRegister(0xFFFFF024, "SSR4", "");
  ByteRegister(0xFFFFF025, "RDR4", "");
  ByteRegister(0xFFFFF026, "SDCR4", "");


  // Hitachi Controller Area Network (HCAN)
  ByteRegister(0xFFFFE400, "MCR", "Master control register");
  ByteRegister(0xFFFFE401, "GSR", "General status register");
  WordRegister(0xFFFFE402, "BCR", "Bit configuration register");
  WordRegister(0xFFFFE404, "MBCR", "Mailbox configuration register");
  WordRegister(0xFFFFE406, "TXPR", "Transmit wait register");
  WordRegister(0xFFFFE408, "TXCR", "Transmit wait cancel register");
  WordRegister(0xFFFFE40A, "TXACK", "Transmit acknowledge register");
  WordRegister(0xFFFFE40C, "ABACK", "Abort acknowledge register");
  WordRegister(0xFFFFE40E, "RXPR", "Receive complete register");
  WordRegister(0xFFFFE410, "RFPR", "Remote request register");
  WordRegister(0xFFFFE412, "IRR", "Interrupt register");
  WordRegister(0xFFFFE414, "MBIMR", "Mailbox interrupt mask register");
  WordRegister(0xFFFFE416, "IMR", "Interrupt mask register");
  ByteRegister(0xFFFFE418, "REC", "Receive error counter");
  ByteRegister(0xFFFFE419, "TEC", "Transmit error counter");
  WordRegister(0xFFFFE41A, "UMSR", "Unread message status register");
  WordRegister(0xFFFFE41C, "LAFML", "Local acceptance filter mask L");
  WordRegister(0xFFFFE41E, "LAFMH", "Local acceptance filter mask H");

  RegisterArray(WordRegister(0xFFFFE420, "MC0", "Message control 0"), 4);
  RegisterArray(WordRegister(0xFFFFE428, "MC1", "Message control 1"), 4);
  RegisterArray(WordRegister(0xFFFFE430, "MC2", "Message control 2"), 4);
  RegisterArray(WordRegister(0xFFFFE438, "MC3", "Message control 3"), 4);
  RegisterArray(WordRegister(0xFFFFE440, "MC4", "Message control 4"), 4);
  RegisterArray(WordRegister(0xFFFFE448, "MC5", "Message control 5"), 4);
  RegisterArray(WordRegister(0xFFFFE450, "MC6", "Message control 6"), 4);
  RegisterArray(WordRegister(0xFFFFE458, "MC7", "Message control 7"), 4);
  RegisterArray(WordRegister(0xFFFFE460, "MC8", "Message control 8"), 4);
  RegisterArray(WordRegister(0xFFFFE468, "MC9", "Message control 9"), 4);
  RegisterArray(WordRegister(0xFFFFE470, "MC10", "Message control 10"), 4);
  RegisterArray(WordRegister(0xFFFFE478, "MC11", "Message control 11"), 4);
  RegisterArray(WordRegister(0xFFFFE480, "MC12", "Message control 12"), 4);
  RegisterArray(WordRegister(0xFFFFE488, "MC13", "Message control 13"), 4);
  RegisterArray(WordRegister(0xFFFFE490, "MC14", "Message control 14"), 4);
  RegisterArray(WordRegister(0xFFFFE498, "MC15", "Message control 15"), 4);
  RegisterArray(WordRegister(0xFFFFE4B0, "MD0", "Message data 0"), 4);
  RegisterArray(WordRegister(0xFFFFE4B8, "MD1", "Message data 1"), 4);
  RegisterArray(WordRegister(0xFFFFE4C0, "MD2", "Message data 2"), 4);
  RegisterArray(WordRegister(0xFFFFE4C8, "MD3", "Message data 3"), 4);
  RegisterArray(WordRegister(0xFFFFE4D0, "MD4", "Message data 4"), 4);
  RegisterArray(WordRegister(0xFFFFE4D8, "MD5", "Message data 5"), 4);
  RegisterArray(WordRegister(0xFFFFE4E0, "MD6", "Message data 6"), 4);
  RegisterArray(WordRegister(0xFFFFE4E8, "MD7", "Message data 7"), 4);
  RegisterArray(WordRegister(0xFFFFE4F0, "MD8", "Message data 8"), 4);
  RegisterArray(WordRegister(0xFFFFE4F8, "MD9", "Message data 9"), 4);
  RegisterArray(WordRegister(0xFFFFE500, "MD10", "Message data 10"), 4);
  RegisterArray(WordRegister(0xFFFFE508, "MD11", "Message data 11"), 4);
  RegisterArray(WordRegister(0xFFFFE510, "MD12", "Message data 12"), 4);
  RegisterArray(WordRegister(0xFFFFE518, "MD13", "Message data 13"), 4);
  RegisterArray(WordRegister(0xFFFFE520, "MD14", "Message data 14"), 4);
  RegisterArray(WordRegister(0xFFFFE528, "MD15", "Message data 15"), 4);


  // A/D Converter
  WordRegister(0xFFFFF800, "ADDR0", "A/D data register 0");
  WordRegister(0xFFFFF802, "ADDR1", "");
  WordRegister(0xFFFFF804, "ADDR2", "");
  WordRegister(0xFFFFF806, "ADDR3", "");
  WordRegister(0xFFFFF808, "ADDR4", "");
  WordRegister(0xFFFFF80A, "ADDR5", "");
  WordRegister(0xFFFFF80C, "ADDR6", "");
  WordRegister(0xFFFFF80E, "ADDR7", "");
  WordRegister(0xFFFFF810, "ADDR8", "");
  WordRegister(0xFFFFF812, "ADDR9", "");
  WordRegister(0xFFFFF814, "ADDR10", "");
  WordRegister(0xFFFFF816, "ADDR11", "");
  WordRegister(0xFFFFF820, "ADDR12", "");
  WordRegister(0xFFFFF822, "ADDR13", "");
  WordRegister(0xFFFFF824, "ADDR14", "");
  WordRegister(0xFFFFF826, "ADDR15", "");
  ByteRegister(0xFFFFF818, "ADCSR0", "A/D control/status register 0");
  ByteRegister(0xFFFFF819, "ADCR0", "A/D control register 0");
  ByteRegister(0xFFFFF76E, "ADTRGR0", "A/D trigger register 0");
  ByteRegister(0xFFFFF838, "ADCSR1", "");
  ByteRegister(0xFFFFF839, "ADCR1", "");
  ByteRegister(0xFFFFF72E, "ADTRGR1", "");


  // Pin Function Controller (PFC)
  WordRegister(0xFFFFF720, "PAIOR", "Port A IO register");
  WordRegister(0xFFFFF722, "PACRH", "Port A control register H");
  WordRegister(0xFFFFF724, "PACRL", "Port A control register L");
  WordRegister(0xFFFFF730, "PBIOR", "");
  WordRegister(0xFFFFF732, "PBCRH", "");
  WordRegister(0xFFFFF734, "PBCRL", "");
  WordRegister(0xFFFFF736, "PBIR", "Port B invert register");
  WordRegister(0xFFFFF73A, "PCIOR", "");
  WordRegister(0xFFFFF73C, "PCCR", "");
  WordRegister(0xFFFFF740, "PDIOR", "");
  WordRegister(0xFFFFF742, "PDCRH", "");
  WordRegister(0xFFFFF744, "PDCRL", "");
  WordRegister(0xFFFFF750, "PEIOR", "");
  WordRegister(0xFFFFF752, "PECR", "");
  WordRegister(0xFFFFF748, "PFIOR", "");
  WordRegister(0xFFFFF74A, "PFCRH", "");
  WordRegister(0xFFFFF74C, "PFCRL", "");
  WordRegister(0xFFFFF760, "PGIOR", "");
  WordRegister(0xFFFFF762, "PGCR", "");
  WordRegister(0xFFFFF728, "PHIOR", "");
  WordRegister(0xFFFFF72A, "PHCR", "");
  WordRegister(0xFFFFF766, "PJIOR", "");
  WordRegister(0xFFFFF768, "PJCRH", "");
  WordRegister(0xFFFFF76A, "PJCRL", "");
  WordRegister(0xFFFFF770, "PKIOR", "");
  WordRegister(0xFFFFF772, "PKCRH", "");
  WordRegister(0xFFFFF774, "PKCRL", "");
  WordRegister(0xFFFFF776, "PKIR", "");


  // I/P Ports (I/O)
  WordRegister(0xFFFFF726, "PADR", "Port A data register");
  WordRegister(0xFFFFF738, "PBDR", "Port B data register");
  WordRegister(0xFFFFF73E, "PCDR", "Port C data register");
  WordRegister(0xFFFFF746, "PDDR", "Port D data register");
  WordRegister(0xFFFFF754, "PEDR", "Port E data register");
  WordRegister(0xFFFFF74E, "PFDR", "Port F data register");
  WordRegister(0xFFFFF764, "PGDR", "Port G data register");
  WordRegister(0xFFFFF72C, "PHDR", "Port H data register");
  WordRegister(0xFFFFF76C, "PJDR", "Port J data register");
  WordRegister(0xFFFFF778, "PKDR", "Port K data register");


  // ROM
  ByteRegister(0xFFFFE800, "FLMCR1", "Flash memory control register 1");
  ByteRegister(0xFFFFE801, "FLMCR2", "Flash memory control register 2");
  ByteRegister(0xFFFFE802, "EBR1", "Erase block register 1");
  ByteRegister(0xFFFFE803, "EBR2", "Erase block register 1");
  WordRegister(0xFFFFEC20, "BCR1", "Bus control register 1");
  WordRegister(0xFFFFEC22, "BCR2", "Bus control register 2");
  WordRegister(0xFFFFEC24, "WCR", "Wait state control register");
  WordRegister(0xFFFFEC26, "RAMER", "Ram emulation register");
  ByteRegister(0xFFFFEC14, "SBYCR", "Standby control register");
  ByteRegister(0xFFFFF708, "SYSCR", "System control register");
  ByteRegister(0xFFFFF70A, "MSTCR", "Module standby control register");


  // Reserved register space
  ReservedSpaceArray(0xFFFFE4A0, 0xFFFFE4AF);
  ReservedSpaceArray(0xFFFFE530, 0xFFFFE7FF);
  ReservedSpaceArray(0xFFFFE804, 0xFFFFEBFF);
  ReservedSpaceArray(0xFFFFEC0C, 0xFFFFEC0F);
  ReservedSpaceArray(0xFFFFEC12, 0xFFFFEC12);
  ReservedSpaceArray(0xFFFFEC15, 0xFFFFEC1F);
  ReservedSpaceArray(0xFFFFEC28, 0xFFFFECAF);
  ReservedSpaceArray(0xFFFFECB2, 0xFFFFECBF);
  ReservedSpaceArray(0xFFFFED1C, 0xFFFFEFFF);
  ReservedSpaceArray(0xFFFFF01F, 0xFFFFF01F);
  ReservedSpaceArray(0xFFFFF027, 0xFFFFF3FF);
  ReservedSpaceArray(0xFFFFF40C, 0xFFFFF41F);
  ReservedSpaceArray(0xFFFFF467, 0xFFFFF47F);
  ReservedSpaceArray(0xFFFFF485, 0xFFFFF49F);
  ReservedSpaceArray(0xFFFFF4AD, 0xFFFFF4BF);
  ReservedSpaceArray(0xFFFFF4CD, 0xFFFFF4DF);
  ReservedSpaceArray(0xFFFFF5A6, 0xFFFFF5BF);
  ReservedSpaceArray(0xFFFFF5CE, 0xFFFFF5FF);
  ReservedSpaceArray(0xFFFFF634, 0xFFFFF63F);
  ReservedSpaceArray(0xFFFFF66F, 0xFFFFF67F);
  ReservedSpaceArray(0xFFFFF66F, 0xFFFFF67F);
  ReservedSpaceArray(0xFFFFF6A2, 0xFFFFF6BF);
  ReservedSpaceArray(0xFFFFF6EC, 0xFFFFF6FF);
  ReservedSpaceArray(0xFFFFF77A, 0xFFFFF7FF);
  ReservedSpaceArray(0xFFFFF828, 0xFFFFF837);
  ReservedSpaceArray(0xFFFFF83A, 0xFFFFF85F);

  Message("Done\n");
}

static LoadM32RX() {
  Fixup_VT_M32R(0x0000, M32R_ROM_START/2+1, 0x10);
  Fixup_VT_M32R(0x40, M32R_ROM_START+1, 0x4);
  Message("M32R Vector Table Fixups Performed\n");
}


static Fixup_VT_M32R(segoffset, romstart, increment) {
  auto i;

  // fixup all vector table entries
  for (i = (segoffset + IVT_START); i < (segoffset + romstart); i = i + increment) {
    if (!MakeCode(i)) {
      MakeDword(i);
      OpOff(i, 0, 0);
    } else {
      AutoMark(i, AU_PROC);
    }
  }
  Message("VT_M32R Entry Point Fixups Performed\n", i);
}


static AddVTEntry(ea, name) {
  auto j, errcode;
  MakeDword(ea);
  OpOff(ea, 0, 0);
  MakeNameEx(ea, "e" + name, SN_NOLIST);
  j = Dword(ea);
  errcode = AddEntryPoint(j, j, "f" + name, 1);
  MakeCode(j);
  AutoMark(j, AU_PROC);
}


static Fixup_VT(segoffset, romstart) {
  auto i, j, errcode;

  // fixup all vector table entries
  for (i = (segoffset + IVT_START); i < (segoffset + romstart); i = i + 4) {
    MakeDword(i);
    OpOff(i, 0, 0);
    j = Dword(i);
    errcode = AddEntryPoint(j, j, "", 1);
    MakeCode(j);
    AutoMark(j, AU_PROC);
  }

  // Define known specific VT entries
  AddVTEntry(0x00000000, "PowerOn");
  AddVTEntry(0x00000008, "Reset");
  AddVTEntry(0x00000010, "GeneralIllegal");
  AddVTEntry(0x00000018, "SlotIllegal");
  AddVTEntry(0x00000024, "CPUAddrErr");
  AddVTEntry(0x00000028, "DMACAddrErr");
  AddVTEntry(0x0000002C, "NMI");
  AddVTEntry(0x00000030, "UserBreak");
  for (i = 32; i <= 63; i++) {
    AddVTEntry(0x00000080 + ((i - 32) * 4), form("Trap%d", i - 32));
  }
  AddVTEntry(0x00000100, "IRQ0");
  AddVTEntry(0x00000104, "IRQ1");
  AddVTEntry(0x00000108, "IRQ2");
  AddVTEntry(0x0000010C, "IRQ3");
  Message("VT Entry Point Fixups Performed\n", j);
}


static FixupJumps(void) {
  auto ea, end, indexa, indexj, xref_from, xref_to;

  ea = 0;
  end = SegEnd(ea);
  if (ea == BADADDR || end == BADADDR) {
    Message("nothing selected\n");
    return;
  }
  Message("Fixing jmp from %x to %x... ", ea, end);

  for (ea; ea <= end; ea = NextAddr(ea)) {
    if (ea == BADADDR || ea >= end) {
      // Message("No more hits\n");
      break;
    } else {
      // Check for "mova"
      if (GetMnem(ea) == "mova") {
        //Message("mova @ %x\n", ea);
        indexa = NextHead(ea, end) + GetOperandValue(ea, 0);
        //Message("mova initial @ %x\n", indexa);

        while (Word(indexa) == 0xFFFF) {
          MakeWord(indexa);
          indexa = indexa + 2;
        }
        //Message("mova points to @ %x\n", indexa);
        do {
          ea = NextAddr(ea);
          if (GetMnem(ea) == "jmp") {
            xref_from = ea;
            // Message("jmp @ %x\n", xref_from);
            break;
          }

        } while (ea != BADADDR);

        indexj = indexa;
        while (isUnknown(GetFlags(indexj))) {
          MakeWord(indexj);
          xref_to = indexa + Word(indexj);
          MakeCode(xref_to);
          AddCodeXref(xref_from, xref_to, fl_JN);
          //Message("Adding jump from %x to %x\n", xref_from, xref_to);
          MakeComm(indexj, "jsr " + NameEx(indexj, xref_to));
          indexj = indexj + 2;
          ea = indexj;
        }
      }
    }
  }
  Message("Done\n");
}

static Fix_Missing_Code(ea, end, is_byte_check) {
  if (ea == BADADDR || end == BADADDR) {
    Message("nothing selected\n");
    return;
  }
  Message("Fixing missing code from %x to %x... ", ea, end);

  for (ea; ea <= end; ea = NextAddr(ea)) {
    if (ea == BADADDR)
      break;

    if (isUnknown(GetFlags(ea)) && (is_byte_check || Byte(ea) == 0x2F || Byte(ea) == 0x4F)) {
      AddEntryPoint(ea, ea, "", 1);
      //MakeCode(ea);
    }
  }
  Message("Done\n");
}

static Fix_MUT_Table(void) {
  auto ea, end, i, mutname;

  ea = 0;
  end = SegEnd(ea);
  if (ea == BADADDR || end == BADADDR) {
    Message("nothing selected\n");
    return;
  }
  Message("searching for MUT table %x to %x... ", ea, end);

  for (ea; ea <= end; ea = NextAddr(ea)) {
    if (ea == BADADDR)
      break;

    // Check for "mov.w"
    if (GetMnem(ea) == "mov.w") {
      if (Word(NextHead(NextHead(ea, end) + GetOperandValue(ea, 0), end)) == 0xBF && GetMnem(ea + 6) == "shll2") {
        ea = Dword(NextAddr(NextAddr(ea + 8) + GetOperandValue(ea + 8, 0)));
        Message("Found at 0x%x... ", ea);
        break;
      }
    }
  }

  if (ea == end) {
    Warning("MUT table not found");
    return;
  }

  i = 0;
  for (ea; ea <= end; ea = ea + 4) {
    if (ea == BADADDR || end == BADADDR) {
      Message("No more matches...\n");
      return;
    }
    if (Dword(ea) != 0xFFFFFFFF) {
      MakeDword(ea);
      mutname = form("MUT_%X", i++);
      MakeName(Dword(ea), mutname);
    } else {
      break;
    }
  }
  Message("Done\n");
}

static FixDataOffsets(void) {
  auto ea, end, disass;

  ea = 0;
  end = SegEnd(ea);
  if (ea == BADADDR || end == BADADDR) {
    Message("nothing selected\n");
    return;
  }
  Message("Fixing offsets from %x to %x... ", ea, end);

  for (ea; ea <= end; ea = NextAddr(ea)) {
    if (ea == BADADDR)
      break;

    // Check for loc_, off_xxx + y
    disass = GetDisasm(ea);

    if ((strstr(disass, ".data.l loc_") != -1 && strstr(disass, "+") != -1) ||
        (strstr(disass, ".data.l off_") != -1 && strstr(disass, "+") != -1)) {
      // Message("fixing %s  @0x%x\n", disass, ea);
      MakeWord(ea);
    }
  }
  Message("Done\n");
}

static FixConstants(void) {
  auto ea, end, disass;

  ea = 0x1500;
  end = 0x3500;

  Message("Fixing constants from %x to %x... ", ea, end);

  for (ea; ea <= end; ea = NextAddr(ea)) {
    if (ea == BADADDR)
      break;
    MakeWord(ea);
  }
  Message("done\n");
}

//-----------------------------------------------------------------------
// Get name of the current processor

// usually, we don't use this function now...
static get_processor(void) {
  auto i, procname, chr;

  procname = "";
  for (i = 0; i < 8; i++) {
    chr = GetCharPrm(INF_PROCNAME + i);
    if (chr == 0) break;
    procname = procname + chr;
  }
  return procname;
}


static main() {
  auto end, ea_rebase, processor, user_message;
  processor = get_processor();
  if (processor == "h8500")
    FixH8500();
  if (processor == "SH4B")
    FixSH4B();
  if (processor == "m32rx")
    FixM32RX();
}


