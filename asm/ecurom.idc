// Auto-Disassembly script for Mitsubishi Evo ROMs.
//
// This is based on Acamus's Auto Disassembler.
//
// vim:foldmethod=syntax foldnestmax=1 sw=2 et smarttab smartindent ft=c
//
// ----------------------------- WARNING --------------------------------------
// This will NOT work with IDA Pro v6.0, but it should work with any other
// version, including 6.1, as long as you switch off the "Convert Immediate
// Loads" option. You can find it in Options -> General -> Analysis ->
// Processor specific analysis options.
// ----------------------------- WARNING --------------------------------------
//
// STEP-BY-STEP TO DISASSEMBLING YOUR ROM IMAGE:
//
// Run "idaw -psh4b <rom image filename>" (use "udal" on Linux). (Another
//   option would be to open IDA Pro, drag your ROM image onto the main
//   window, and choose the "Hitachi SH4B" processor.)
//
// Accept the default options for segments, analysis, etc. Don't bother
//   creating a RAM segment; this script will do it for you.
//
// Once you are presented with the main disassembly view window,
//   go to the "Options" drop-down menu, and select "Target assembler...".
//   Choose "GNU Assembler" when asked. (If you happen to have a copy of
//   SHASM, feel free to leave this alone, but most people will want to
//   use the free GNU assembler for the SH-ELF platform.)
//
// Next, go to the "Options" drop-down meny, and select "Analysis options...".
//   Choose "Kernel analyzer options 1", and uncheck:
//     "Create ascii string if data xref exists"
//     "Create offset if data xref to seg32 exists"
//   Choose "Kernel analyzer options 2", and uncheck:
//     "Check for unicode strings"
//   (There are no ASCII or Unicode strings in a typical SH7052F ROM image,
//   and IDA doesn't take alignment into account when converting referenced
//   32-bit values to offsets for some reason.)
//
// Finally, go to the "File" drop-down menu, select "Load file", select
//   "IDC file...", and choose this script. It will automatically start
//   disassembling the ROM, labeling known memory, register, and function
//   addresses as it goes.
//

#include <idc.idc>
#include <memcpy.idc>

#define H8_ROM_START   0x0140
#define H8_CODE_OFFSET 0x10000
#define SH_ROM_START   0x0400
#define M32R_ROM_START 0x0080


// Locations of well-known functions
#define BYTE_TABLE_LOOKUP_FUNC      0x00000C28
#define AXIS_LOOKUP_FUNC            0x00000CC6
#define WORD_TABLE_LOOKUP_FUNC      0x00000E02

// Set a name for a location if there is not already a manual name set.
#define SafeMakeName(ea, name) if (name) SafeMakeNameEx(ea, name, SN_NOWARN)
#define SafeMakeNameEx(ea, name, opts) if (ea != BADADDR && !HasName(ea)) MakeNameEx(ea, name, opts);
#define HasName(ea) !(Name(ea) == "" || strstr(Name(ea), "off_") == 0 || strstr(Name(ea), "unk_") == 0 || strstr(Name(ea), "sub_") == 0 || strstr(Name(ea), "byte_") == 0 || strstr(Name(ea), "word_") == 0 || strstr(Name(ea), "dword_") == 0)
static MakeNameSequence(ea, name) {
  auto i, fullname;
  for (i = 1; i < 5000; i++) {
    fullname = form("%s_%d", name, i);
    if (LocByName(fullname) == BADADDR) {
      SafeMakeName(ea, fullname);
      return;
    }
  }
}

static H8RegisterNames() {
  SafeMakeName(0xFEC0, "SCI3_SMR");
  SafeMakeName(0xFEC1, "SCI3_BRR");
  SafeMakeName(0xFEC2, "SCI3_SCR");
  SafeMakeName(0xFEC3, "SCI3_TDR");
  SafeMakeName(0xFEC4, "SCI3_SSR");
  SafeMakeName(0xFEC5, "SCI3_RDR");

  SafeMakeName(0xFEC8, "SCI1_SMR");
  SafeMakeName(0xFEC9, "SCI1_BRR");
  SafeMakeName(0xFECA, "SCI1_SCR");
  SafeMakeName(0xFECB, "SCI1_TDR");
  SafeMakeName(0xFECC, "SCI1_SSR");
  SafeMakeName(0xFECD, "SCI1_RDR");

  SafeMakeName(0xFED0, "SCI2_SMR");
  SafeMakeName(0xFED1, "SCI2_BRR");
  SafeMakeName(0xFED2, "SCI2_SCR");
  SafeMakeName(0xFED3, "SCI2_TDR");
  SafeMakeName(0xFED4, "SCI2_SSR");
  SafeMakeName(0xFED5, "SCI2_RDR");

  SafeMakeName(0xFE80, "P1DDR");
  SafeMakeName(0xFE81, "P2DDR");
  SafeMakeName(0xFE82, "P1DR");
  SafeMakeName(0xFE83, "P2DR");
  SafeMakeName(0xFE84, "P3DDR");
  SafeMakeName(0xFE85, "P4DDR");
  SafeMakeName(0xFE86, "P3DR");
  SafeMakeName(0xFE87, "P4DR");
  SafeMakeName(0xFE88, "P5DDR");
  SafeMakeName(0xFE89, "P6DDR");
  SafeMakeName(0xFE8A, "P5DR");
  SafeMakeName(0xFE8B, "P6DR");
  SafeMakeName(0xFE8C, "P7DDR");
  SafeMakeName(0xFE8E, "P7DR");
  SafeMakeName(0xFE8F, "P8DR");

  SafeMakeName(0xFE91, "PADDR");
  SafeMakeName(0xFE92, "P9DR");
  SafeMakeName(0xFE93, "PADR");
  SafeMakeName(0xFE94, "PBDDR");
  SafeMakeName(0xFE95, "PCDDR");
  SafeMakeName(0xFE96, "PBDR");
  SafeMakeName(0xFE97, "PCDR");
  SafeMakeName(0xFE98, "PBPCR");
  SafeMakeName(0xFE99, "PCPCR");
  SafeMakeName(0xFE9A, "oCR");

  SafeMakeName(0xFEA0, "ADDR0H");
  SafeMakeName(0xFEA1, "ADDR0L");
  SafeMakeName(0xFEA2, "ADDR1H");
  SafeMakeName(0xFEA3, "ADDR1L");
  SafeMakeName(0xFEA4, "ADDR2H");
  SafeMakeName(0xFEA5, "ADDR2L");
  SafeMakeName(0xFEA6, "ADDR3H");
  SafeMakeName(0xFEA7, "ADDR3L");
  SafeMakeName(0xFEA8, "ADDR4H");
  SafeMakeName(0xFEA9, "ADDR4L");
  SafeMakeName(0xFEAA, "ADDR5H");
  SafeMakeName(0xFEAB, "ADDR5L");
  SafeMakeName(0xFEAC, "ADDR6H");
  SafeMakeName(0xFEAD, "ADDR6L");
  SafeMakeName(0xFEAE, "ADDR7H");
  SafeMakeName(0xFEAF, "ADDR7L");

  SafeMakeName(0xFEB0, "ADDR8H");
  SafeMakeName(0xFEB1, "ADDR8L");
  SafeMakeName(0xFEB2, "ADDR9H");
  SafeMakeName(0xFEB3, "ADDR9L");
  SafeMakeName(0xFEB4, "ADDRAH");
  SafeMakeName(0xFEB5, "ADDRAL");
  SafeMakeName(0xFEB6, "ADDRBH");
  SafeMakeName(0xFEB7, "ADDRBL");
  SafeMakeName(0xFEB8, "ADCSR");
  SafeMakeName(0xFEB9, "ADCR");

  SafeMakeName(0xFEDA, "PACR");
  SafeMakeName(0xFEDB, "P67CR");
  SafeMakeName(0xFEDC, "ADTRGR");
  SafeMakeName(0xFEDE, "IRQFR");
  SafeMakeName(0xFEDF, "BCR");

  SafeMakeName(0xFEE0, "FLMCR");
  SafeMakeName(0xFEE1, "FLM_EBR1");
  SafeMakeName(0xFEE2, "FLM_EBR2");
  SafeMakeName(0xFEEC, "FLMER");
  SafeMakeName(0xFEED, "FLMSR");

  SafeMakeName(0xFEF0, "PWM1_TCR");
  SafeMakeName(0xFEF1, "PWM1_DTR");
  SafeMakeName(0xFEF2, "PWM1_TCNT");
  SafeMakeName(0xFEF4, "PWM2_TCR");
  SafeMakeName(0xFEF5, "PWM2_DTR");
  SafeMakeName(0xFEF6, "PWM2_TCNT");
  SafeMakeName(0xFEF8, "PWM3_TCR");
  SafeMakeName(0xFEF9, "PWM3_DTR");
  SafeMakeName(0xFEFA, "PWM3_TCNT");

  SafeMakeName(0xFF00, "INTC_IPRA");
  SafeMakeName(0xFF01, "INTC_IPRB");
  SafeMakeName(0xFF02, "INTC_IPRC");
  SafeMakeName(0xFF03, "INTC_IPRD");
  SafeMakeName(0xFF04, "INTC_IPRE");
  SafeMakeName(0xFF05, "INTC_IPRF");
  SafeMakeName(0xFF08, "DTC_DTEA");
  SafeMakeName(0xFF09, "DTC_DTEB");
  SafeMakeName(0xFF0A, "DTC_DTEC");
  SafeMakeName(0xFF0B, "DTC_DTED");
  SafeMakeName(0xFF0C, "DTC_DTEE");
  SafeMakeName(0xFF0D, "DTC_DTEF");

  SafeMakeName(0xFF10, "WDT__TCSR");
  SafeMakeName(0xFF11, "WDT_TCNT");
  SafeMakeName(0xFF14, "WSC_WCR");
  SafeMakeName(0xFF15, "RAMCR");
  SafeMakeName(0xFF16, "BSC_ARBT");
  SafeMakeName(0xFF17, "BSC_AR3T");
  SafeMakeName(0xFF19, "SYSC_MDCR");
  SafeMakeName(0xFF1A, "SYSC_SBYCR");
  SafeMakeName(0xFF1B, "SYSC_BRCR");
  SafeMakeName(0xFF1C, "SYSC_NMICR");
  SafeMakeName(0xFF1D, "SYSC_IRQCR");
  SafeMakeName(0xFF1E, "SYSC_writeCR");
  SafeMakeName(0xFF1F, "SYSC_RSTCSR");

  SafeMakeName(0xFF20, "T1CRH");
  SafeMakeName(0xFF21, "T1CRL");
  SafeMakeName(0xFF22, "T1SRAH");
  SafeMakeName(0xFF23, "T1SRAL");
  SafeMakeName(0xFF24, "T1OERA");
  SafeMakeName(0xFF25, "TMDRA");
  SafeMakeName(0xFF26, "T1CNTH");
  SafeMakeName(0xFF27, "T1CNTL");
  SafeMakeName(0xFF28, "T1GR1H");
  SafeMakeName(0xFF29, "T1GR1L");
  SafeMakeName(0xFF2A, "T1GR2H");
  SafeMakeName(0xFF2B, "T1GR2L");
  SafeMakeName(0xFF2C, "T1DR1H");
  SafeMakeName(0xFF2D, "T1DR1L");
  SafeMakeName(0xFF2E, "T1DR2H");
  SafeMakeName(0xFF2F, "T1DR2L");

  SafeMakeName(0xFF30, "TSTR");
  SafeMakeName(0xFF31, "T1CRA");
  SafeMakeName(0xFF32, "T1SRBH");
  SafeMakeName(0xFF33, "T1SRBL");
  SafeMakeName(0xFF34, "T1OERB");
  SafeMakeName(0xFF35, "TMDRB");
  SafeMakeName(0xFF38, "T1GR3H");
  SafeMakeName(0xFF39, "T1GR3L");
  SafeMakeName(0xFF3A, "T1GR4H");
  SafeMakeName(0xFF3B, "T1GR4L");
  SafeMakeName(0xFF3C, "T1DR3H");
  SafeMakeName(0xFF3D, "T1DR3L");
  SafeMakeName(0xFF3E, "T1DR4H");
  SafeMakeName(0xFF3F, "T1DR4L");

  SafeMakeName(0xFF40, "T2CRH");
  SafeMakeName(0xFF41, "T2CRL");
  SafeMakeName(0xFF42, "T2SRH");
  SafeMakeName(0xFF43, "T2SRL");
  SafeMakeName(0xFF44, "T2OER");
  SafeMakeName(0xFF46, "T2CNTH");
  SafeMakeName(0xFF47, "T2CNTL");
  SafeMakeName(0xFF48, "T2GR1H");
  SafeMakeName(0xFF49, "T2GR1L");
  SafeMakeName(0xFF4A, "T2GR2H");
  SafeMakeName(0xFF4B, "T2GR2L");
  SafeMakeName(0xFF4C, "T2DR1H");
  SafeMakeName(0xFF4D, "T2DR1L");
  SafeMakeName(0xFF4E, "T2DR2H");
  SafeMakeName(0xFF4F, "T2DR2L");

  SafeMakeName(0xFF50, "T3CRH");
  SafeMakeName(0xFF51, "T3CRL");
  SafeMakeName(0xFF52, "T3SRH");
  SafeMakeName(0xFF53, "T3SRL");
  SafeMakeName(0xFF54, "T3OER");
  SafeMakeName(0xFF56, "T3CNTH");
  SafeMakeName(0xFF57, "T3CNTL");
  SafeMakeName(0xFF58, "T3GR1H");
  SafeMakeName(0xFF59, "T3GR1L");
  SafeMakeName(0xFF5A, "T3GR2H");
  SafeMakeName(0xFF5B, "T3GR2L");
  SafeMakeName(0xFF5C, "T3DR1H");
  SafeMakeName(0xFF5D, "T3DR1L");
  SafeMakeName(0xFF5E, "T3DR2H");
  SafeMakeName(0xFF5F, "T3DR2L");

  SafeMakeName(0xFF60, "T4CRH");
  SafeMakeName(0xFF61, "T4CRL");
  SafeMakeName(0xFF62, "T4SRH");
  SafeMakeName(0xFF63, "T4SRL");
  SafeMakeName(0xFF64, "T4OER");
  SafeMakeName(0xFF66, "T4CNTH");
  SafeMakeName(0xFF67, "T4CNTL");
  SafeMakeName(0xFF68, "T4GR1H");
  SafeMakeName(0xFF69, "T4GR1L");
  SafeMakeName(0xFF6A, "T4GR2H");
  SafeMakeName(0xFF6B, "T4GR2L");
  SafeMakeName(0xFF6C, "T4DR1H");
  SafeMakeName(0xFF6D, "T4DR1L");
  SafeMakeName(0xFF6E, "T4DR2H");
  SafeMakeName(0xFF6F, "T4DR2L");

  SafeMakeName(0xFF70, "T5CRH");
  SafeMakeName(0xFF71, "T5CRL");
  SafeMakeName(0xFF72, "T5SRH");
  SafeMakeName(0xFF73, "T5SRL");
  SafeMakeName(0xFF74, "T5OER");
  SafeMakeName(0xFF76, "T5CNTH");
  SafeMakeName(0xFF77, "T5CNTL");
  SafeMakeName(0xFF78, "T5GR1H");
  SafeMakeName(0xFF79, "T5GR1L");
  SafeMakeName(0xFF7A, "T5GR2H");
  SafeMakeName(0xFF7B, "T5GR2L");
  SafeMakeName(0xFF7C, "T5DR1H");
  SafeMakeName(0xFF7D, "T5DR1L");
  SafeMakeName(0xFF7E, "T5DR2H");
  SafeMakeName(0xFF7F, "T5DR2L");

  SafeMakeName(0xFF80, "T6CRH");
  SafeMakeName(0xFF81, "T6CRL");
  SafeMakeName(0xFF82, "T6SRH");
  SafeMakeName(0xFF83, "T6SRL");
  SafeMakeName(0xFF84, "T6OER");
  SafeMakeName(0xFF86, "T6CNTH");
  SafeMakeName(0xFF87, "T6CNTL");
  SafeMakeName(0xFF88, "T6GR1H");
  SafeMakeName(0xFF89, "T6GR1L");
  SafeMakeName(0xFF8A, "T6GR2H");
  SafeMakeName(0xFF8B, "T6GR2L");

  SafeMakeName(0xFF90, "T7CRH");
  SafeMakeName(0xFF91, "T7CRL");
  SafeMakeName(0xFF92, "T7SRH");
  SafeMakeName(0xFF93, "T7SRL");
  SafeMakeName(0xFF94, "T7OER");
  SafeMakeName(0xFF96, "T7CNTH");
  SafeMakeName(0xFF97, "T7CNTL");
  SafeMakeName(0xFF98, "T7GR1H");
  SafeMakeName(0xFF99, "T7GR1L");
  SafeMakeName(0xFF9A, "T7GR2H");
  SafeMakeName(0xFF9B, "T7GR2L");

  SafeMakeName(0xFFA0, "MLTCR");
  SafeMakeName(0xFFA1, "MLTBR");
  SafeMakeName(0xFFA2, "MLTMAR");
  SafeMakeName(0xFFA3, "MLTAR");

  SafeMakeName(0xFFB0, "MULT_CA");
  SafeMakeName(0xFFB1, "MULT_(CA)");
  SafeMakeName(0xFFB2, "MULT_CB");
  SafeMakeName(0xFFB3, "MULT_(CB)");
  SafeMakeName(0xFFB4, "MULT_CC");
  SafeMakeName(0xFFB5, "MULT_(CC)");
  SafeMakeName(0xFFB6, "MULT_XH");
  SafeMakeName(0xFFB7, "MULT_(XH)");
  SafeMakeName(0xFFB8, "MULT_H");
  SafeMakeName(0xFFB9, "MULT_(H)");
  SafeMakeName(0xFFBA, "MULT_L");
  SafeMakeName(0xFFBB, "MULT_(L)");
  SafeMakeName(0xFFBC, "MULT_MR");
  SafeMakeName(0xFFBD, "MULT_(MR)");
  SafeMakeName(0xFFBE, "MULT_MMR");
  SafeMakeName(0xFFBF, "MULT_(MMR)");
}

static SegmentsSH4B() {
  Message("Creating segments... ");
  // Label the ROM segment
  SegRename(0x00000000, "ROM");
  SegClass(0x00000000, "UNK"); // This should be "CODE" but then IDA finds bad code where stack variables are.
  SegDefReg(0x00000000, "br", 0x0);
  SegDefReg(0x00000000, "dp", 0x1);
  if (SegName(0xFFFF6000) != "RAM")
    // Create a RAM segment if it doesn't already exist
    // This is the size for the SH7055. SH7052 has 12k and SH7054 has 16k. They
    // start from 0xFFFF8000, so this start will include all possibilities.
    AddSeg(0xFFFF6000, 0xFFFFE000, 0x0, 1, saAbs, scStack);
  SegRename(0xFFFF6000, "RAM");
  SegClass(0xFFFF6000, "DATA");
  // Create a segment for the hardware registers
  if (SegName(0xFFFFE400) != "HWREG")
    AddSeg(0xFFFFE400, 0xFFFFF860, 0x0, 1, saAbs, scStack);
  SegRename(0xFFFFE400, "HWREG");
  SegClass(0xFFFFE400, "DATA");
  Message("Done\n");
}

static ByteRegister(ea, name, comment) {
  MakeByte(ea);
  if (!HasName(ea))
    MakeNameEx(ea, name, SN_NOLIST);
  if (comment != "")
    MakeComm(ea, comment);
  return ea;
}

static WordRegister(ea, name, comment) {
  MakeWord(ea);
  if (!HasName(ea))
    MakeNameEx(ea, name, SN_NOLIST);
  if (comment != "")
    MakeComm(ea, comment);
  return ea;
}

static LongRegister(ea, name, comment) {
  MakeDword(ea);
  if (!HasName(ea))
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

  RegisterArray(ByteRegister(0xFFFFE420, "MC0", "Message control 0"), 8);
  RegisterArray(ByteRegister(0xFFFFE428, "MC1", "Message control 1"), 8);
  RegisterArray(ByteRegister(0xFFFFE430, "MC2", "Message control 2"), 8);
  RegisterArray(ByteRegister(0xFFFFE438, "MC3", "Message control 3"), 8);
  RegisterArray(ByteRegister(0xFFFFE440, "MC4", "Message control 4"), 8);
  RegisterArray(ByteRegister(0xFFFFE448, "MC5", "Message control 5"), 8);
  RegisterArray(ByteRegister(0xFFFFE450, "MC6", "Message control 6"), 8);
  RegisterArray(ByteRegister(0xFFFFE458, "MC7", "Message control 7"), 8);
  RegisterArray(ByteRegister(0xFFFFE460, "MC8", "Message control 8"), 8);
  RegisterArray(ByteRegister(0xFFFFE468, "MC9", "Message control 9"), 8);
  RegisterArray(ByteRegister(0xFFFFE470, "MC10", "Message control 10"), 8);
  RegisterArray(ByteRegister(0xFFFFE478, "MC11", "Message control 11"), 8);
  RegisterArray(ByteRegister(0xFFFFE480, "MC12", "Message control 12"), 8);
  RegisterArray(ByteRegister(0xFFFFE488, "MC13", "Message control 13"), 8);
  RegisterArray(ByteRegister(0xFFFFE490, "MC14", "Message control 14"), 8);
  RegisterArray(ByteRegister(0xFFFFE498, "MC15", "Message control 15"), 8);
  RegisterArray(ByteRegister(0xFFFFE4B0, "MD0", "Message data 0"), 8);
  RegisterArray(ByteRegister(0xFFFFE4B8, "MD1", "Message data 1"), 8);
  RegisterArray(ByteRegister(0xFFFFE4C0, "MD2", "Message data 2"), 8);
  RegisterArray(ByteRegister(0xFFFFE4C8, "MD3", "Message data 3"), 8);
  RegisterArray(ByteRegister(0xFFFFE4D0, "MD4", "Message data 4"), 8);
  RegisterArray(ByteRegister(0xFFFFE4D8, "MD5", "Message data 5"), 8);
  RegisterArray(ByteRegister(0xFFFFE4E0, "MD6", "Message data 6"), 8);
  RegisterArray(ByteRegister(0xFFFFE4E8, "MD7", "Message data 7"), 8);
  RegisterArray(ByteRegister(0xFFFFE4F0, "MD8", "Message data 8"), 8);
  RegisterArray(ByteRegister(0xFFFFE4F8, "MD9", "Message data 9"), 8);
  RegisterArray(ByteRegister(0xFFFFE500, "MD10", "Message data 10"), 8);
  RegisterArray(ByteRegister(0xFFFFE508, "MD11", "Message data 11"), 8);
  RegisterArray(ByteRegister(0xFFFFE510, "MD12", "Message data 12"), 8);
  RegisterArray(ByteRegister(0xFFFFE518, "MD13", "Message data 13"), 8);
  RegisterArray(ByteRegister(0xFFFFE520, "MD14", "Message data 14"), 8);
  RegisterArray(ByteRegister(0xFFFFE528, "MD15", "Message data 15"), 8);


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
  ByteRegister(0xFFFFF858, "ADCSR2", "");
  ByteRegister(0xFFFFF859, "ADCR2", "");
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

/* M32R Functions
static LoadM32RX() {
  Fixup_VT_M32R(0x0000, M32R_ROM_START / 2 + 1, 0x10);
  Fixup_VT_M32R(0x40, M32R_ROM_START + 1, 0x4);
  Message("M32R Vector Table Fixups Performed\n");
}

static Fixup_VT_M32R(segoffset, romstart, increment) {
  auto i;

  // fixup all vector table entries
  for (i = segoffset; i < (segoffset + romstart); i = i + increment) {
    if (!MakeCode(i)) {
      MakeDword(i);
      OpOff(i, 0, 0);
    } else {
      AutoMark(i, AU_PROC);
    }
  }
  Message("VT_M32R Entry Point Fixups Performed\n", i);
}
*/

static AddVTEntry(ea, name, funcname) {
  auto j;
  j = Dword(ea);
  SafeMakeNameEx(ea, "e" + name, SN_NOLIST);
  SafeMakeName(j, funcname);
  MakeFunction(j, BADADDR);
}

static Fixup_VT(segoffset, romstart) {
  auto i, j, errcode;

  // fixup all vector table entries
  for (i = segoffset; i < (segoffset + romstart); i = i + 4) {
    MakeDword(i);
    OpOff(i, 0, 0);
    // These are stack pointers, not code
    if (i == 0x04 || i == 0x0C)
      continue;
    j = Dword(i);
    MakeCode(j);
    AutoMark(j, AU_PROC);
  }

  // Define known VT entries
  AddVTEntry(0x00000000, "v_power_on_pc", "init");
  AddVTEntry(0x00000004, "v_power_on_sp", "stack");
  AddVTEntry(0x00000008, "v_reset_pc", "reset_pc");
  AddVTEntry(0x0000000C, "v_reset_sp", "");
  AddVTEntry(0x00000010, "v_gen_ill_inst", "reset");
  AddVTEntry(0x00000018, "v_slot_ill_inst", "slot_ill_inst");
  AddVTEntry(0x00000024, "v_cpu_addr_err", "cpu_addr_err");
  AddVTEntry(0x00000028, "v_dmac_addr_err", "dmac_addr_err");
  AddVTEntry(0x0000002C, "NMI", "nmi");
  AddVTEntry(0x00000030, "UBC", "userbreak");
  AddVTEntry(0x00000080, "TRAP0", "trap");
  for (i = 33; i <= 63; i++)
    AddVTEntry(0x00000080 + ((i - 32) * 4), form("TRAP%d", i - 32), "");
  AddVTEntry(0x00000100, "IRQ0", "irq0");
  AddVTEntry(0x00000104, "IRQ1", "irq1");
  AddVTEntry(0x00000108, "IRQ2", "irq2");
  AddVTEntry(0x0000010C, "IRQ3", "irq3");
  AddVTEntry(0x00000120, "DMAC0", "dmac0");
  AddVTEntry(0x00000128, "DMAC1", "dmac1");
  AddVTEntry(0x00000130, "DMAC2", "dmac2");
  AddVTEntry(0x00000138, "DMAC3", "dmac3");
  AddVTEntry(0x000002F0, "CMTI0", "cmti0");
  AddVTEntry(0x000002F8, "ADI0", "adi0");
  AddVTEntry(0x00000300, "CMTI1", "cmti1");
  AddVTEntry(0x00000308, "ADI1", "adi1");
  AddVTEntry(0x00000380, "ITI", "wdt_iti");
  // ATU0
  AddVTEntry(0x00000140, "ITV1", "atu0_itv1");
  AddVTEntry(0x00000150, "ICI0A", "atu0_ici0a");
  AddVTEntry(0x00000158, "ICI0B", "atu0_ici0b");
  AddVTEntry(0x00000160, "ICI0C", "atu0_ici0c");
  AddVTEntry(0x00000168, "ICI0D", "atu0_ici0d");
  AddVTEntry(0x00000170, "OVI0", "atu0_ovi0");
  // ATU1
  AddVTEntry(0x00000180, "IMI1A", "atu1_imi1a");
  AddVTEntry(0x00000184, "IMI1B", "atu1_imi1b");
  AddVTEntry(0x00000188, "IMI1C", "atu1_imi1c");
  AddVTEntry(0x0000018C, "IMI1D", "atu1_imi1d");
  AddVTEntry(0x00000190, "IMI1E", "atu1_imi1e");
  AddVTEntry(0x00000194, "IMI1F", "atu1_imi1f");
  AddVTEntry(0x00000198, "IMI1G", "atu1_imi1g");
  AddVTEntry(0x0000019C, "IMI1H", "atu1_imi1h");
  AddVTEntry(0x000001A0, "OVI1A", "atu1_ovi1a");
  // ATU2
  AddVTEntry(0x000001B0, "IMI2A", "atu2_imi2a");
  AddVTEntry(0x000001B4, "IMI2B", "atu2_imi2b");
  AddVTEntry(0x000001B8, "IMI2C", "atu2_imi2c");
  AddVTEntry(0x000001BC, "IMI2D", "atu2_imi2d");
  AddVTEntry(0x000001C0, "IMI2E", "atu2_imi2e");
  AddVTEntry(0x000001C4, "IMI2F", "atu2_imi2f");
  AddVTEntry(0x000001C8, "IMI2G", "atu2_imi2g");
  AddVTEntry(0x000001CC, "IMI2H", "atu2_imi2h");
  AddVTEntry(0x000001D0, "OVI2A", "atu2_ovi2a");
  // ATU3
  AddVTEntry(0x000001E0, "IMI3A", "atu3_imi3a");
  AddVTEntry(0x000001E4, "IMI3B", "atu3_imi3b");
  AddVTEntry(0x000001E8, "IMI3C", "atu3_imi3c");
  AddVTEntry(0x000001EC, "IMI3D", "atu3_imi3d");
  AddVTEntry(0x000001F0, "OVI3", "atu3_ovi3");
  // ATU4
  AddVTEntry(0x00000200, "IMI4A", "atu4_imi4a");
  AddVTEntry(0x00000204, "IMI4B", "atu4_imi4b");
  AddVTEntry(0x00000208, "IMI4C", "atu4_imi4c");
  AddVTEntry(0x0000020C, "IMI4D", "atu4_imi4d");
  AddVTEntry(0x00000210, "OVI4", "atu4_ovi4");
  // ATU5
  AddVTEntry(0x00000220, "IMI5A", "atu5_imi5a");
  AddVTEntry(0x00000224, "IMI5B", "atu5_imi5b");
  AddVTEntry(0x00000228, "IMI5C", "atu5_imi5c");
  AddVTEntry(0x0000022C, "IMI5D", "atu5_imi5d");
  AddVTEntry(0x00000230, "OVI5", "atu5_ovi5");
  // ATU6
  AddVTEntry(0x00000240, "CMI6A", "atu6_cmi6a");
  AddVTEntry(0x00000244, "CMI6B", "atu6_cmi6b");
  AddVTEntry(0x00000248, "CMI6C", "atu6_cmi6c");
  AddVTEntry(0x0000024C, "CMI6D", "atu6_cmi6d");
  // ATU7
  AddVTEntry(0x00000250, "CMI7A", "atu7_cmi7a");
  AddVTEntry(0x00000254, "CMI7B", "atu7_cmi7b");
  AddVTEntry(0x00000258, "CMI7C", "atu7_cmi7c");
  AddVTEntry(0x0000025C, "CMI7D", "atu7_cmi7d");
  // ATU8
  AddVTEntry(0x00000260, "OSI8A", "atu8_osi8a");
  AddVTEntry(0x00000264, "OSI8B", "atu8_osi8b");
  AddVTEntry(0x00000268, "OSI8C", "atu8_osi8c");
  AddVTEntry(0x0000026C, "OSI8D", "atu8_osi8d");
  AddVTEntry(0x00000270, "OSI8E", "atu8_osi8e");
  AddVTEntry(0x00000274, "OSI8F", "atu8_osi8f");
  AddVTEntry(0x00000278, "OSI8G", "atu8_osi8g");
  AddVTEntry(0x0000027C, "OSI8H", "atu8_osi8h");
  AddVTEntry(0x00000280, "OSI8I", "atu8_osi8i");
  AddVTEntry(0x00000284, "OSI8J", "atu8_osi8j");
  AddVTEntry(0x00000288, "OSI8K", "atu8_osi8k");
  AddVTEntry(0x0000028C, "OSI8L", "atu8_osi8l");
  AddVTEntry(0x00000290, "OSI8M", "atu8_osi8m");
  AddVTEntry(0x00000294, "OSI8N", "atu8_osi8n");
  AddVTEntry(0x00000298, "OSI8O", "atu8_osi8o");
  AddVTEntry(0x0000029C, "OSI8P", "atu8_osi8p");
  // ATU9
  AddVTEntry(0x000002A0, "CMI9A", "atu9_cmi9a");
  AddVTEntry(0x000002A4, "CMI9B", "atu9_cmi9b");
  AddVTEntry(0x000002A8, "CMI9C", "atu9_cmi9c");
  AddVTEntry(0x000002AC, "CMI9D", "atu9_cmi9d");
  AddVTEntry(0x000002B0, "CMI9E", "atu9_cmi9e");
  AddVTEntry(0x000002B8, "CMI9F", "atu9_cmi9f");
  // ATU10
  AddVTEntry(0x000002C0, "CMI10A", "atu10_cmi10a");
  AddVTEntry(0x000002C8, "CMI10B", "atu10_cmi10b");
  AddVTEntry(0x000002D0, "ICI10A", "atu10_ici10a");
  // ATU11
  AddVTEntry(0x000002E0, "IMI11A", "atu11_imi11a");
  AddVTEntry(0x000002E8, "IMI11B", "atu11_imi11b");
  AddVTEntry(0x000002EC, "OVI11", "atu11_ovi11");
  // SCI0
  AddVTEntry(0x00000320, "ERI0", "sci0_eri0");
  AddVTEntry(0x00000324, "RXI0", "sci0_rxi0");
  AddVTEntry(0x00000328, "TXI0", "sci0_txi0");
  AddVTEntry(0x0000032C, "TEI0", "sci0_tei0");
  // SCI1
  AddVTEntry(0x00000330, "ERI1", "sci1_eri1");
  AddVTEntry(0x00000334, "RXI1", "sci1_rxi1");
  AddVTEntry(0x00000338, "TXI1", "sci1_txi1");
  AddVTEntry(0x0000033C, "TEI1", "sci1_tei1");
  // SCI2
  AddVTEntry(0x00000340, "ERI2", "sci2_eri2");
  AddVTEntry(0x00000344, "RXI2", "sci2_rxi2");
  AddVTEntry(0x00000348, "TXI2", "sci2_txi2");
  AddVTEntry(0x0000034C, "TEI2", "sci2_tei2");
  // SCI3
  AddVTEntry(0x00000350, "ERI3", "sci3_eri3");
  AddVTEntry(0x00000354, "RXI3", "sci3_rxi3");
  AddVTEntry(0x00000358, "TXI3", "sci3_txi3");
  AddVTEntry(0x0000035C, "TEI3", "sci3_tei3");
  // SCI4
  AddVTEntry(0x00000360, "ERI4", "sci4_eri4");
  AddVTEntry(0x00000364, "RXI4", "sci4_rxi4");
  AddVTEntry(0x00000368, "TXI4", "sci4_txi4");
  AddVTEntry(0x0000036C, "TEI4", "sci4_tei4");
  // HCAN
  AddVTEntry(0x00000370, "ERS", "hcan_ers");
  AddVTEntry(0x00000374, "OVR", "hcan_ovr");
  AddVTEntry(0x00000378, "RM", "hcan_rm");
  AddVTEntry(0x0000037C, "SLE", "hcan_sle");

  Message("VT Entry Point Fixups Performed\n", j);
}

static FixupJumps(void) {
  auto ea, end, indexa, indexj, xref_from, xref_to;

  ea = 0;
  end = SegEnd(ea);
  Message("Fixing jmp from %x to %x... ", ea, end);

  for (ea; ea <= end; ea = NextAddr(ea)) {
    if (ea == BADADDR) {
      // Message("No more hits\n");
      break;
    }

    // Check for "mova"
    if (GetMnem(ea) == "mova") {
      // Message("mova @ %x\n", ea);
      if (Word(GetOperandValue(ea, 0)) == 0xFFFF)
        indexa = GetOperandValue(ea, 0);
      else
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
          //Message("jmp @ %x\n", xref_from);
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

static LabelMutVar(req, name, comment) {
  auto ea;
  ea = LocByName(req);
  if (ea != BADADDR)
    SafeMakeName(ea - 1, name);
  if (comment != "")
    MakeComm(ea, comment);
}

static Fix_MUT_Table(void) {
  auto ea, end, i, mutname;

  ea = 0;
  end = SegEnd(ea);
  Message("Searching for MUT table %x to %x... ", ea, end);

  for (ea; ea <= end && ea != BADADDR; ea = NextAddr(ea)) {
    // Check for "mov.w"
    if (GetMnem(ea) == "mov.w") {
      //Message("Found mov.w at 0x%x\n", ea);
      if ((GetOperandValue(ea, 0) == 0xBF || Word(NextHead(NextHead(ea, end) + GetOperandValue(ea, 0), end)) == 0xBF) && GetMnem(ea + 6) == "shll2") {
        if (Word(NextHead(NextHead(ea, end) + GetOperandValue(ea, 0), end)) == 0xBF)
          ea = Dword(NextAddr(NextAddr(ea + 8) + GetOperandValue(ea + 8, 0)));
        else
          ea = GetOperandValue(ea + 8, 0);
        //Message("Found at 0x%x... ", ea);
        break;
      }
    }
  }

  if (ea == end || ea == BADADDR) {
    Warning("MUT table not found");
    return;
  }

  i = 0;
  for (ea; ea <= end; ea = ea + 4) {
    if (ea == BADADDR || end == BADADDR) {
      Message("No more matches...\n");
      return;
    }
    if (Dword(ea) == 0xFFFFFFFF)
      break;
    MakeDword(ea);
    mutname = form("MUT_%02X", i++);
    SafeMakeName(Dword(ea), mutname);
  }

  // Common MUT requests
  LabelMutVar("MUT_04", "TimingAdv", "Timing Advance Interpolated");
  LabelMutVar("MUT_06", "TimingAdv", "Timing Advance Scaled");
  LabelMutVar("MUT_06", "TimingAdv", "Timing Advance");
  LabelMutVar("MUT_07", "CoolantTemp", "Coolant Temp");
  LabelMutVar("MUT_0C", "LTFTLo", "Fuel Trim Low (LTFT)");
  LabelMutVar("MUT_0D", "LTFTMid", "Fuel Trim Mid (LTFT)");
  LabelMutVar("MUT_0E", "LTFTHigh", "Fuel Trim High (LTFT)");
  LabelMutVar("MUT_0F", "STFT", "Oxygen Feedback Trim (STFT)");
  LabelMutVar("MUT_10", "CoolantTempScaled", "Coolant Temp Scaled");
  LabelMutVar("MUT_11", "MAFAirTempScaled", "MAF Air Temp Scaled");
  LabelMutVar("MUT_12", "EGRTemp", "EGR Temperature");
  LabelMutVar("MUT_13", "O2Sensor", "Front Oxygen Sensor");
  LabelMutVar("MUT_14", "Battery", "Battery Level");
  LabelMutVar("MUT_15", "Baro", "Barometer");
  LabelMutVar("MUT_16", "ISCSteps", "ISC Steps");
  LabelMutVar("MUT_17", "TPS", "Throttle Position");
  LabelMutVar("MUT_18", "", "Open Loop Bit Array");
  LabelMutVar("MUT_19", "", "Startup Check Bits");
  LabelMutVar("MUT_1A", "AirFlow", "Air Flow - (TPS Idle Adder ?)");
  LabelMutVar("MUT_1A", "", "TPS Idle Adder");
  LabelMutVar("MUT_1C", "Load", "ECULoad");
  LabelMutVar("MUT_1D", "AccelEnrich", "Acceleration Enrichment - (Manifold_Absolute_Pressure_Mean ?)");
  LabelMutVar("MUT_1F", "PrevLoad", "ECU Load Previous");
  LabelMutVar("MUT_20", "RPM_Idle_Scaled", "Engine RPM Idle Scaled");
  LabelMutVar("MUT_21", "RPM", "Engine RPM");
  LabelMutVar("MUT_22", "", "Idle Related Value (unknown)");
  LabelMutVar("MUT_24", "TargetIdleRPM", "Target Idle RPM");
  LabelMutVar("MUT_25", "ISCV_Value", "Idle Stepper Value");
  LabelMutVar("MUT_26", "KnockSum", "Knock Sum");
  LabelMutVar("MUT_27", "OctaneFlag", "Octane Level");
  LabelMutVar("MUT_29", "InjPulseWidth", "Injector Pulse Width (LSB)");
  LabelMutVar("MUT_2A", "InjPulseWidth", "Injector Pulse Width (MSB)");
  LabelMutVar("MUT_2C", "AirVol", "Air Volume");
  LabelMutVar("MUT_2D", "", "Ignition Battery Trim");
  LabelMutVar("MUT_2E", "", "Vehicle speed Frequency");
  LabelMutVar("MUT_2F", "Speed", "Speed");
  LabelMutVar("MUT_30", "Knock", "Knock Voltage");
  LabelMutVar("MUT_31", "VE", "Volumetric Efficiency");
  LabelMutVar("MUT_32", "AFRMAP", "Air/Fuel Ratio (Map reference)");
  LabelMutVar("MUT_33", "Corr_TimingAdv", "Corrected Timing Advance");
  LabelMutVar("MUT_34", "", "MAP Index");
  LabelMutVar("MUT_35", "", "Limp Home Fuel TPS Based");
  LabelMutVar("MUT_36", "", "Active Fault Count");
  LabelMutVar("MUT_37", "Stored Fault Count", "Count");
  LabelMutVar("MUT_38", "MAP", "Boost (MDP)");
  LabelMutVar("MUT_39", "", "Fuel Tank Pressure");
  LabelMutVar("MUT_3A", "UnscaledAirTemp", "Unscaled Air Temperature");
  LabelMutVar("MUT_3B", "", "Masked Map Index");
  LabelMutVar("MUT_3C", "O2Sensor2", "Rear Oxygen Sensor #1");
  LabelMutVar("MUT_3D", "", "Front Oxygen Sensor #2");
  LabelMutVar("MUT_3E", "", "Rear Oxygen Sensor #2");
  LabelMutVar("MUT_3F", "", "Short Term Fuel Feedback Trim O2 Map Index");
  LabelMutVar("MUT_40", "", "Stored Faults Lo");
  LabelMutVar("MUT_41", "", "Stored Faults Hi");
  LabelMutVar("MUT_42", "", "Stored Faults Lo 1");
  LabelMutVar("MUT_43", "", "Stored Faults Hi 1");
  LabelMutVar("MUT_44", "", "Stored Faults Lo 2");
  LabelMutVar("MUT_45", "", "Stored Faults Hi 2");
  LabelMutVar("MUT_47", "", "Active Faults Lo");
  LabelMutVar("MUT_48", "", "Active Faults Hi");
  LabelMutVar("MUT_49", "ACRelaySw", "Air Conditioning Relay");
  LabelMutVar("MUT_4A", "PurgeDuty", "Purge Solenoid Duty Cycle");
  LabelMutVar("MUT_4C", "", "Fuel Trim Low Bank 2");
  LabelMutVar("MUT_4D", "", "Fuel Trim Mid Bank 2");
  LabelMutVar("MUT_4E", "", "Fuel Trim High Bank 2");
  LabelMutVar("MUT_4F", "", "Oxygen Feedback Trim Bank 2");
  LabelMutVar("MUT_50", "", "Long Fuel Trim Bank 1");
  LabelMutVar("MUT_51", "", "Long Fuel Trim Bank 2");
  LabelMutVar("MUT_52", "", "Rear Long Fuel Trim Bank 1");
  LabelMutVar("MUT_53", "", "Rear Long Fuel Trim Bank 2");
  LabelMutVar("MUT_54", "AccelEnrichTPS", "Acceleration Enrichment (increasing TPS)");
  LabelMutVar("MUT_55", "DecelLeanTPS", "Deceleration Enleanment (decreasing TPS)");
  LabelMutVar("MUT_56", "AccelLoadChg", "Acceleration Load Change");
  LabelMutVar("MUT_57", "DecelLoadChg", "Deceleration Load Change");
  LabelMutVar("MUT_58", "", "AFR Ct Adder");
  LabelMutVar("MUT_5B", "", "Rear O2 Voltage");
  LabelMutVar("MUT_5C", "", "ADC Rear O2 Voltage");
  LabelMutVar("MUT_60", "", "Rear O2 Trim - Low");
  LabelMutVar("MUT_61", "", "Rear O2 Trim - Mid");
  LabelMutVar("MUT_62", "", "Rear O2 Trim - High");
  LabelMutVar("MUT_63", "", "Rear O2 Feedback Trim");
  LabelMutVar("MUT_6A", "knock_adc", "knock adc processed");
  LabelMutVar("MUT_6B", "knock_base", "knock base");
  LabelMutVar("MUT_6C", "knock_var", "knock var (AKA Knock Sum Addition)");
  LabelMutVar("MUT_6D", "knock_change", "knock change");
  LabelMutVar("MUT_6E", "knock_dynamics", "knock dynamics");
  LabelMutVar("MUT_6F", "knock_flag", "knock flag (AKA Knock Acceleration)");
  LabelMutVar("MUT_70", "", "Array of Serial Receive Data Register 2 RDR 2 Values");
  LabelMutVar("MUT_71", "", "Sensor Error");
  LabelMutVar("MUT_72", "", "Knock Present");
  LabelMutVar("MUT_73", "", "Throttle Position Delta 1");
  LabelMutVar("MUT_74", "", "Throttle Position Delta 2");
  LabelMutVar("MUT_76", "ISCV % Demand", "ISCV % Demand (Columns)");
  LabelMutVar("MUT_79", "InjectorLatency", "Injector Latency");
  LabelMutVar("MUT_7A", "", "Continuous Monitor Completion Status 1");
  LabelMutVar("MUT_7B", "", "Continuous Monitor Completion Status 2");
  LabelMutVar("MUT_7C", "", "Continuous Monitor Completion Status 3");
  LabelMutVar("MUT_7D", "", "Non Continuous Monitor Completion Status OBD");
  LabelMutVar("MUT_7E", "", "Continuous Monitor Completion Status Low 4");
  LabelMutVar("MUT_7F", "", "Continuous Monitor Completion Status High 4");
  LabelMutVar("MUT_80", "", "ECU ID Type (LSB)");
  LabelMutVar("MUT_81", "", "ECU ID Type (MSB)");
  LabelMutVar("MUT_82", "", "ECU ID Version");
  LabelMutVar("MUT_83", "", "ADC Channel F");
  LabelMutVar("MUT_84", "ThermoFanDuty", "Thermo Fan Dutycycle");
  LabelMutVar("MUT_85", "EgrDuty", "EGR Dutycycle");
  LabelMutVar("MUT_86", "WGDC", "Wastegate Duty Cycle");
  LabelMutVar("MUT_87", "FuelTemperature", "Fuel Temperature");
  LabelMutVar("MUT_88", "FuelLevel", "Fuel Level");
  LabelMutVar("MUT_89", "", "ADC Channel 8 2");
  LabelMutVar("MUT_8A", "LoadError", "Load Error - (Throttle Position Corrected ?)");
  LabelMutVar("MUT_8B", "WGDCCorr", "WGDC Correction");
  LabelMutVar("MUT_8E", "", "Solenoid Duty");
  LabelMutVar("MUT_90", "", "Timer Status Register 9 TSR9");
  LabelMutVar("MUT_96", "MAF_ADC", "RAW MAF ADC value");
  LabelMutVar("MUT_9A", "ACClutch", "AC clutch");
  LabelMutVar("MUT_9B", "", "Output Pins");
  LabelMutVar("MUT_A2", "CrankPulse", "Crankshaft sensor pulse");
  LabelMutVar("MUT_A2", "MafPulse", "MAF sensor pulse");
  LabelMutVar("MUT_A2", "CamPulse", "Camshaft sensor pulse");
  LabelMutVar("MUT_A8", "ATInShaftPulse", "Input shaft speed pulse (A/T)");
  LabelMutVar("MUT_A8", "ATOutShaftPulse", "Output shaft speed pulse (A/T)");
  LabelMutVar("MUT_A8", "ATGearL", "Gear: Low (A/T)");
  LabelMutVar("MUT_A8", "ATGear2", "Gear: 2 (A/T)");
  LabelMutVar("MUT_A8", "ATGear3", "Gear: 3 (A/T)");
  LabelMutVar("MUT_A9", "O2HeaterFrontLeft", "Front O2 heater bank 1 (left)");
  LabelMutVar("MUT_A9", "O2HeaterRearLeft", "Rear O2 heater bank 1 (left)");
  LabelMutVar("MUT_A9", "O2HeaterFrontRight", "Front O2 heater bank 2 (right)");
  LabelMutVar("MUT_A9", "O2HeaterRearRight", "Rear O2 heater bank 2 (right)");
  LabelMutVar("MUT_AA", "Braking", "Brakes Pressed");
  LabelMutVar("MUT_B3", "ATGearNeutral", "Gear: Neutral (A/T)");
  LabelMutVar("MUT_B3", "ATGearDrive", "Gear: Drive (A/T)");
  LabelMutVar("MUT_B4", "ATGearPark", "Gear: Park (A/T)");
  LabelMutVar("MUT_B4", "ATGearRev", "Gear: Reverse (A/T)");
  LabelMutVar("MUT_B7", "O2HeaterBrokenFrRt", "front O2 heater circuit open (broken): bank 2 (right)");
  LabelMutVar("MUT_B8", "O2HeaterBrokenFrLt", "front O2 heater circuit open (broken): bank 1 (left)");
  LabelMutVar("MUT_B8", "NewACSwitch", "Air Conditioning Switch (Mattjin)");
  LabelMutVar("MUT_B8", "PowerSteering", "Power Steering");
  LabelMutVar("MUT_B9", "O2HeaterBrokenRearRt", "rear O2 heater circuit open (broken): bank 2 (right)");
  LabelMutVar("MUT_BA", "O2HeaterBrokenRearLt", "rear O2 heater circuit open (broken): bank 1 (left)");

  // Common MUT commands
  LabelMutVar("MUT_C3", "", "SAS (Speed Adjusting Screw)");
  LabelMutVar("MUT_C5", "", "Purge solenoid venting");
  LabelMutVar("MUT_CA", "", "Invalid command");
  LabelMutVar("MUT_CB", "", "Invalid command");
  LabelMutVar("MUT_CD", "", "A/C fan high");
  LabelMutVar("MUT_CE", "", "A/C fan low");
  LabelMutVar("MUT_CF", "", "Main fan high");
  LabelMutVar("MUT_D0", "", "Main fan low");
  LabelMutVar("MUT_D2", "", "Lower RPM");
  LabelMutVar("MUT_D3", "", "Boost control solenoid");
  LabelMutVar("MUT_D5", "", "EGR solenoid");
  LabelMutVar("MUT_D6", "", "Fuel pressure solenoid");
  LabelMutVar("MUT_D7", "", "Purge solenoid");
  LabelMutVar("MUT_D8", "", "Fuel pump");
  LabelMutVar("MUT_D9", "", "Fix timing at 5 degrees");
  LabelMutVar("MUT_DA", "", "Disable injector 1");
  LabelMutVar("MUT_DB", "", "Disable injector 2");
  LabelMutVar("MUT_DC", "", "Disable injector 3");
  LabelMutVar("MUT_DD", "", "Disable injector 4");
  LabelMutVar("MUT_DE", "", "Disable injector 5 (unused)");
  LabelMutVar("MUT_DF", "", "Disable injector 6 (unused)");
  LabelMutVar("MUT_EC", "", "Calibration F6A");
  LabelMutVar("MUT_ED", "", "Calibration");
  LabelMutVar("MUT_EE", "", "Calibration");
  LabelMutVar("MUT_EF", "", "Calibration");
  LabelMutVar("MUT_F3", "", "Cancel previously-active command (ie. SAS mode)");
  LabelMutVar("MUT_F9", "", "some keep alive function to keep the accuator engaged. response is 0xff");
  LabelMutVar("MUT_FA", "", "Clear active and stored faults");
  LabelMutVar("MUT_FB", "", "Force tests to run");
  LabelMutVar("MUT_FC", "", "Clear active faults");
  LabelMutVar("MUT_FE", "", "Immobilizer");
  LabelMutVar("MUT_FF", "", "Init code");

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
      //Message("fixing %s  @0x%x\n", disass, ea);
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
  for (ea; ea <= end && ea != BADADDR; ea = NextAddr(ea)) {
    if (Name(ea) == "" || strstr(Name(ea), "unk_") == 0)
      MakeWord(ea);
  }
  Message("Done\n");
}

static CreateStructures() {
  auto id, mem;
  if (GetStrucIdByName("map_3d_byte") < 0) {
    id = AddStrucEx(-1, "map_3d_byte", 0);
    AddStrucMember(id, "dimensions", -1, FF_BYTE | FF_0NUMD, 0, 1);
    AddStrucMember(id, "adder", -1, FF_BYTE | FF_0NUMD, 0, 1);
    AddStrucMember(id, "index_x", -1, FF_DWRD, 0, 4);
    AddStrucMember(id, "index_y", -1, FF_DWRD, 0, 4);
    AddStrucMember(id, "nrows", -1, FF_BYTE | FF_0NUMD, 0, 1);
    AddStrucMember(id, "data", -1, FF_BYTE | FF_0NUMD, 0, 0);
  }
  if (GetStrucIdByName("map_3d_word") < 0) {
    id = AddStrucEx(-1, "map_3d_word", 0);
    AddStrucMember(id, "dimensions", -1, FF_WORD | FF_0NUMD, 0, 2);
    AddStrucMember(id, "adder", -1, FF_WORD | FF_0NUMD, 0, 2);
    AddStrucMember(id, "index_x", -1, FF_DWRD, 0, 4);
    AddStrucMember(id, "index_y", -1, FF_DWRD, 0, 4);
    AddStrucMember(id, "nrows", -1, FF_WORD | FF_0NUMD, 0, 2);
    AddStrucMember(id, "data", -1, FF_BYTE | FF_0NUMD, 0, 0);
  }
  if (GetStrucIdByName("map_2d_byte") < 0) {
    id = AddStrucEx(-1, "map_2d_byte", 0);
    AddStrucMember(id, "dimensions", -1, FF_BYTE | FF_0NUMD, 0, 1);
    AddStrucMember(id, "adder", -1, FF_BYTE | FF_0NUMD, 0, 1);
    AddStrucMember(id, "index_x", -1, FF_DWRD, 0, 4);
    AddStrucMember(id, "data", -1, FF_BYTE | FF_0NUMD, 0, 0);
  }
  if (GetStrucIdByName("map_2d_word") < 0) {
    id = AddStrucEx(-1, "map_2d_word", 0);
    AddStrucMember(id, "dimensions", -1, FF_WORD | FF_0NUMD, 0, 2);
    AddStrucMember(id, "adder", -1, FF_WORD | FF_0NUMD, 0, 2);
    AddStrucMember(id, "index_x", -1, FF_DWRD, 0, 4);
    AddStrucMember(id, "data", -1, FF_BYTE | FF_0NUMD, 0, 0);
  }
  if (GetStrucIdByName("axis_table") < 0) {
    id = AddStrucEx(-1, "axis_table", 0);
    AddStrucMember(id, "output", -1, FF_DWRD, 0, 4);
    AddStrucMember(id, "input", -1, FF_DWRD, 0, 4);
    AddStrucMember(id, "length", -1, FF_WORD | FF_0NUMD, 0, 2);
    AddStrucMember(id, "data", -1, FF_WORD | FF_0NUMD, 0, 0);
  }
}

static WellKnownFunc(ea, name, comment) {
  if (ea == BADADDR)
    return;
  SafeMakeName(ea, name);
  MakeCode(ea);
  SetFunctionFlags(ea, 0);
  MakeFunction(ea, BADADDR);
  if (comment != "" && GetFunctionCmt(ea, 0) == "")
    SetFunctionCmt(ea, comment, 0);
}

static LabelLibraryFuncs() {
  auto start, ea, i, end;

  WellKnownFunc(BYTE_TABLE_LOOKUP_FUNC, "table_lookup_byte", "Look up the current BYTE value at the table stored at R4");
  WellKnownFunc(WORD_TABLE_LOOKUP_FUNC, "table_lookup_word", "Look up the current WORD value at the table stored at R4");
  WellKnownFunc(AXIS_LOOKUP_FUNC, "axis_lookup", "Look up the current value in the axis stored at R4");

  {
    // Label the enable and disable_interrupts functions, and the global variables they use
    start = 0x400;
    end = GetFunctionAttr(start, FUNCATTR_END);
    WellKnownFunc(start, "disable_interrupts", "Set interrupt mask of SR to 15, storing the old SR on the stack");
    for (i = start; i != BADADDR; i = NextHead(i, end)) {
      if (GetMnem(i) == "mov.l" && GetOpnd(i, 1) == "r0") {
        SafeMakeName(Dnext(i, Dfirst(i)), "sr_stack_ptr");
        break;
      }
    }
  }

  WellKnownFunc(0x41E, "enable_interrupts", "Restore the previous SR, pushed to the stack by disable_interrupts");
  WellKnownFunc(0x430, "set_interrupt_mask", "Set a specific interrupt mask in SR. Takes a 4 bit number in R4 for the mask.");
  WellKnownFunc(0x500, "add_capped", "Add R4 and R5, storing WORD result in R0, max 0xFFFF");
  WellKnownFunc(0x514, "add_r4_and_r5", "Add R4 and R5, storing WORD result in R0");
  WellKnownFunc(0x51C, "add_word_capped", "Add R4 and R5, storing WORD result in R0, max 0xFFFF");
  WellKnownFunc(0x52C, "memclear", "Clear RAM between R4 and R5");
  WellKnownFunc(0x53e, "decrement_block", "Subtract 1 from all words between R4 and R5. R4 is set to the next word after R5.");
  WellKnownFunc(0x562, "increment_block", "Add 1 to all words between R4 and R5. R4 is set to the next word after R5.");
  WellKnownFunc(0x590, "min_ff", "Set R0 the BYTE minimum of R4 and 0xFF");
  WellKnownFunc(0x598, "min_ffff", "Set R0 the WORD minimum of R4 and 0xFFFF");
  WellKnownFunc(0x5A8, "max_3_word", "Return the WORD maximum of R4, R5, R6 in R0");
  WellKnownFunc(0x5B0, "max_3", "Return the maximum of R4, R5, R6 in R0");
  WellKnownFunc(0x5D0, "r4_mult_r5_div_r6_capped_to_R0", "Lesser of ((R4*R5)/R6) and 0xFFFF -> R0");
  WellKnownFunc(0x5E8, "r4_mult_r5_div_r6_to_R0", "Multiply R4 by R5, divide by R6 and return result in R0, capped at 0xFFFFFFFF");
  WellKnownFunc(0x68A, "multR4R5divr6", "(((R4 * R5) / r6) + 1/2) -> R0");
  WellKnownFunc(0x6A2, "sub_6A2", "(((R4 * R5) / r6) + 1/2) -> R0");
  WellKnownFunc(0x752, "sub_752", "Lesser of [(R4 * R5) / 128] and 0xFFFF");
  WellKnownFunc(0x762, "r0_is_r4_x_r5", "Lesser of ([(R4 * R5) / 128] + 1/2) and 0xFFFF -> R0");
  WellKnownFunc(0x780, "r4xr5_strange", "Lesser of [(R4 * R5) / 128] and 0xFFFFFFFF -> R0, R5 is word length, R4 can be long word");
  WellKnownFunc(0x7A6, "sub_7A6", "Lesser of [(R4 * R5) / 128] and 0xFFFFFFFF -> R0");
  WellKnownFunc(0x7D0, "r4xr5", "Lesser of [(R4 * R5) / 256] and 0xFFFF -> R0");
  WellKnownFunc(0x7E6, "sub_7E6", "Lesser of [(R4 * R5) / 256] and 0xFFFFFFFF -> R0");
  WellKnownFunc(0x804, "r4_mult_r5_div_64_add_1_etc2_into_r0", "Lesser of ([(R4 * R5) / 256] +1/2) and 0xFFFF -> R0");
  WellKnownFunc(0x864, "shlr8_byte", "(R4 / 256) -> R0");
  WellKnownFunc(0x86A, "shlr16_word", "(R4 / 65536) -> R0");
  WellKnownFunc(0x870, "shll8_byte", "(R4 * 256) -> R0");
  WellKnownFunc(0x876, "shll16_word", "(R4 * 65536) -> R0");
  WellKnownFunc(0x87C, "second_byte_plus_1", "Lesser of (MSB of R4) and (0xFF) -> R0");
  WellKnownFunc(0x898, "second_word_plus_1", "Lesser of (MSW of R4) and (0xFFFF) -> R0");
  WellKnownFunc(0x8B8, "NOT_SHLL8_OR_R4_INTO_R0", "inv(byte(R4))|byte(R4) -> R0 (word length value is result)");
  WellKnownFunc(0x8C4, "R5_Div_R4_Into_R0", "min(R4 / R5, 0xFFFF) -> R0");
  WellKnownFunc(0x902, "divide_long_by_word", "min(R4 / R5, 0xFFFF) -> R0");
  WellKnownFunc(0x9B0, "divide_words", "min((R4 / R5) + 1/2), 0xFFFF) -> R0");
  WellKnownFunc(0x9F2, "R4_DIV_R5_Into_R0_0", "min((R4 / R5) + 1/2, 0xFFFF) -> R0");
  WellKnownFunc(0x9FA, "sub_9FA", "min((R4 / R5) + 1/2, 0xFFFF) -> R0");
  WellKnownFunc(0xAB8, "R5x_R0minusR6_plusR6xR4", "min(((R4 * r6) + (R5 * (256 - r6))) / 256), 0xFFFF) -> R0");
  WellKnownFunc(0xAE0, "sub_AE0", "min((R4 * r6) + (R5 * (256 - r6)) / 256, 0xFFFFFFFF) -> R0");
  WellKnownFunc(0xB16, "BETWEEN_R4_R5byR6", "min((R4 * r6 + R5 * (255 - R6)) / 255, 0x????) -> R0. This is a sub to interpolate between R4 and R5 using r6");
  WellKnownFunc(0xD7A, "sub_D7A", "Linear Interpolation of R4 and R5 using r6 as the scalar, results -> R0");
  WellKnownFunc(0xDC6, "read_mapindex_byte", "Reads BYTE at (R4 + (MAPindex * 4)) into R0");
  WellKnownFunc(0xDD2, "read_mapindex_word", "Reads WORD at (R4 + (MAPindex * 4)) into R0");
  WellKnownFunc(0xDE0, "table_lookup_byte_mapindex", "Call table_lookup_byte with a table at (R4 + (MAPindex * 4))");
  WellKnownFunc(0xDF6, "read_mapindex_long", "Reads DWORD at (R4 + (MAPindex * 4)) into R0");
  WellKnownFunc(0xEA6, "table_lookup_word_mapindex", "Call table_lookup_word with a table at (R4 + (MAPindex * 4))");
  WellKnownFunc(0xED8, "multiply_capped", "min(R4 * R5, 0xFFFF) -> R0");
  WellKnownFunc(0xEEE, "multiply", "min(R4 * R5, 0xFFFF) -> R0");
  WellKnownFunc(0xEF8, "multiply_word", "Lesser of R4*R5 and 0xFFFFFFFF -> R0");
  WellKnownFunc(0xF0C, "subtract_nowrap_word", "(R4 > R5) ? (R4 - R5) : 0  -> R0");
  WellKnownFunc(0xF12, "subtract_nowrap_byte", "max(R4 - R5, 0) -> R0");

  {
    // Look for the main() function, it should be the only sub called from 'init'
    start = Dword(0);
    end = GetFunctionAttr(start, FUNCATTR_END);
    for (i = start; i != BADADDR; i = NextHead(i, end)) {
      if (GetMnem(i) == "jsr") {
        start = Rfirst0(i);
        // Message("Real init starts at %x\n", start);
        WellKnownFunc(start, "main", "ROM entry point");
        break;
      }
    }
  }
}

// You can run this handily by selecing the start of an axis table, pressing Shift-F2 to bring up the "Run IDC Command" window and entering:
// MakeAxis(ScreenEA());
static MakeAxis(ea) {
  MakeComm(ea, "lookup result pointer");
  MakeDword(ea);
  MakeDword(ea + 4);
  MakeComm(ea + 4, "lookup input pointer");
  MakeWord(ea + 8);
  MakeComm(ea + 8, "axis length");
  if (Word(ea + 8) > 0) {
    MakeWord(ea + 10);
    MakeArray(ea + 10, Word(ea + 8));
    MakeComm(ea + 10, "axis data");
    SetArrayFormat(ea + 11, 0, Word(ea + 8), -1);
  }
  OpDecimal(ea + 8, 0);
  OpDecimal(ea + 10, 0);
}

static LocateAxisTables() {
  auto i, ea, code, table, counter, found;

  Message("Locating axis tables... ");
  counter = 0;
  for (i = RfirstB(AXIS_LOOKUP_FUNC); i > 0; i = RnextB(AXIS_LOOKUP_FUNC, i)) {
    if (i == BADADDR || i == 0)
      continue;
    found = 0;
    // Look back at most 16 instructions for r4 being set
    for (ea = i; ea > 0 && ea > (i - 32); ea = ea - 2) {
      code = GetDisasm(ea);
      if (strstr(code, "mov.l ") == 0 && strstr(code, ", r4") > 0) {
        for (table = Dfirst(ea); table != BADADDR; table = Dnext(ea, table)) {
          if (!(Dword(table) & 0xFFFF0000) || !(Dword(table + 4) & 0xFFFF0000) || Word(table + 8) == 0)
            continue;
          // First 2 dwords of the table are pointers to RAM, and it has a length, it's probably valid
          MakeNameSequence(table, "unknown_axis");
          MakeAxis(table);
          counter++;
          found = 1;
          break;
        }
        if (found)
          break;
      }
    }
    if (!found)
      Message("Couldn't find axis table searching back from %x\n", i);
  }
  Message("Done, found %d tables\n", counter);
}

static GetMapHeight(table, ea) {
  auto xaxisloc, mapheight, ea2, code, ea3, ref, good;
  if (Byte(table) == 3) {
    xaxisloc = Dword(table + 6);
  } else if (Word(table) == 3) {
    xaxisloc = Dword(table + 8);
  } else if (Byte(table) == 2) {
    xaxisloc = Dword(table + 2);
  } else if (Word(table) == 2) {
    xaxisloc = Dword(table + 4);
  } else {
    return 0;
  }

  if (!xaxisloc || xaxisloc == BADADDR)
    return 0;

  // Search back from the map lookup call to find the axis lookup
  // calls. Multiply the size of both axes to work out how many items are
  // in this map.
  mapheight = 0;
  for (ea2 = ea; ea2 > 0 && ea2 > (ea - 256); ea2 = ea2 - 2) {
    code = GetDisasm(ea2);
    if (strstr(code, "jsr") == 0 && strstr(code, "axis_lookup") > 0) {
      for (ea3 = ea2; ea3 > 0 && ea3 > (ea2 - 32); ea3 = ea3 - 2) {
        code = GetDisasm(ea3);
        if (strstr(code, "mov.l ") == 0 && strstr(code, ", r4") > 0) {
          // This is an axis table, look for the map's x or y input in the output of this axis table
          for (ref = Dfirst(ea3); ref && ref != BADADDR; ref = Dnext(ea3, ref)) {
            if (!mapheight && Dword(ref) == xaxisloc)
              mapheight = Word(ref + 8);
          }
        }
        if (mapheight)
          break;
      }
    }
  }
  if (!mapheight && (Byte(table) == 2 || Word(table) == 2)) {
    // Couldn't find the axis by searching through the code, attempt to
    // find it from references to the input.
    good = 1;
    for (ref = DfirstB(xaxisloc); ref && ref != BADADDR; ref = DnextB(xaxisloc, ref)) {
      if (strstr(CommentEx(ref, 0), "lookup result pointer") != -1) {
        // This is an axis
        if (Word(ref + 8) == 0)
          continue;
        if (mapheight && Word(ref + 8) == mapheight)
          continue;
        if (mapheight) {
          Message("Multiple length axes found referencing %x for map %x\n", xaxisloc, table);
          return 1;
        }
        mapheight = Word(ref + 8);
      }
    }
    if (!good)
      mapheight = 0;
  }
  if (!mapheight) {
    Message("Couldn't detect size for 3d map starting at %x\n", table);
    mapheight = 1;
  }
  return mapheight;
}

static GetMapWidth(table, ea) {
  auto yaxisloc, mapwidth, ea2, ea3, code, ref;
  if (Byte(table) == 3) {
    yaxisloc = Dword(table + 2);
  } else if (Word(table) == 3) {
    yaxisloc = Dword(table + 4);
  } else {
    return 0;
  }

  if (!yaxisloc || yaxisloc == BADADDR)
    return 0;

  // Search back from the map lookup call to find the axis lookup
  // calls. Multiply the size of both axes to work out how many items are
  // in this map.
  mapwidth = 0;
  for (ea2 = ea; ea2 > 0 && ea2 > (ea - 256); ea2 = ea2 - 2) {
    code = GetDisasm(ea2);
    if (strstr(code, "jsr") == 0 && strstr(code, "axis_lookup") > 0) {
      for (ea3 = ea2; ea3 > 0 && ea3 > (ea2 - 32); ea3 = ea3 - 2) {
        code = GetDisasm(ea3);
        if (strstr(code, "mov.l ") == 0 && strstr(code, ", r4") > 0) {
          // This is an axis table, look for the map's x or y input in the output of this axis table
          for (ref = Dfirst(ea3); ref && ref != BADADDR; ref = Dnext(ea3, ref)) {
            if (!mapwidth && Dword(ref) == yaxisloc)
              mapwidth = Word(ref + 8);
          }
        }
        if (mapwidth)
          break;
      }
    }
  }
  if (!mapwidth) {
    Message("Couldn't detect size for 3d map starting at %x\n", table);
    mapwidth = 1;
  }
  return mapwidth;
}

static LabelTable(table, ea) {
  auto mapheight, mapwidth, datastart;
  mapwidth = GetMapWidth(table, ea);
  mapheight = GetMapHeight(table, ea);
  if (Byte(table) == 3) {
    MakeNameSequence(table, "unknown_3d_byte_table");
    MakeByte(table);
    MakeComm(table, "number of dimensions");
    OpDecimal(table, 0);
    MakeByte(table + 1);
    MakeComm(table + 1, "adder");
    OpDecimal(table + 1, 0);
    MakeDword(table + 2);
    MakeComm(table + 2, "x axis position");
    MakeDword(table + 6);
    MakeComm(table + 6, "y axis position");
    MakeByte(table + 10);
    MakeComm(table + 10, "num columns");
    OpDecimal(table + 10, 0);
    MakeByte(table + 11);
    OpDecimal(table + 11, 0);
    MakeComm(table + 11, "map data");
    datastart = table + 11;
  } else if (Word(table) == 3) {
    MakeNameSequence(table, "unknown_3d_word_table");
    MakeWord(table);
    MakeComm(table, "number of dimensions");
    OpDecimal(table, 0);
    MakeWord(table + 2);
    MakeComm(table + 2, "adder");
    OpDecimal(table + 2, 0);
    MakeDword(table + 4);
    MakeComm(table + 4, "x axis position");
    MakeDword(table + 8);
    MakeComm(table + 8, "y axis position");
    MakeWord(table + 12);
    MakeComm(table + 12, "num columns");
    OpDecimal(table + 12, 0);
    MakeWord(table + 14);
    OpDecimal(table + 14, 0);
    MakeComm(table + 14, "map data");
    datastart = table + 14;
  } else if (Byte(table) == 2) {
    MakeNameSequence(table, "unknown_2d_byte_table");
    MakeByte(table);
    MakeComm(table, "number of dimensions");
    OpDecimal(table, 0);
    MakeByte(table + 1);
    MakeComm(table + 1, "adder");
    OpDecimal(table + 1, 0);
    MakeDword(table + 2);
    MakeComm(table + 2, "input position");
    MakeByte(table + 6);
    OpDecimal(table + 6, 0);
    MakeComm(table + 6, "map data");
    datastart = table + 6;
  } else if (Word(table) == 2) {
    MakeNameSequence(table, "unknown_2d_word_table");
    MakeWord(table);
    MakeComm(table, "number of dimensions");
    OpDecimal(table, 0);
    MakeWord(table + 2);
    MakeComm(table + 2, "adder");
    OpDecimal(table + 2, 0);
    MakeDword(table + 4);
    MakeComm(table + 4, "input position");
    MakeWord(table + 8);
    OpDecimal(table + 8, 0);
    MakeComm(table + 8, "map data");
    datastart = table + 8;
  } else {
    Message("Unknown dimensions for table at %x\n", table);
    return;
  }

  if (Byte(table) == 3 || Word(table) == 3) {
    if (mapwidth != Byte(table + 10)) {
      Message("Detected axis lookup for 3d map starting at %x found mismatching width\n", table);
      mapheight = 1;
      mapwidth = 1;
    }

    MakeArray(datastart, mapheight * mapwidth);
    SetArrayFormat(datastart, 0, mapwidth, 0);
  } else if (Byte(table) == 2 || Word(table) == 2) {
    if (!mapheight) {
      Message("Couldn't detect size for 2d map starting at %x (code at %x)\n", table, ea);
      mapheight = 1;
    }

    MakeArray(datastart, mapheight);
    SetArrayFormat(datastart, 0, mapheight, -1);
  }
}

static LocateMapsHelper(base) {
  auto i, ea, code, table;
  Message("Locating maps from %x... ", base);
  for (i = RfirstB(base); i > 0; i = RnextB(base, i)) {
    if (i == BADADDR || i == 0)
      continue;
    // Look back at most 16 instructions for r4 being set
    for (ea = i; ea > 0 && ea > (i - 32); ea = ea - 2) {
      code = GetDisasm(ea);
      if (strstr(code, "mov.l ") == -1 || strstr(code, ", r4") == -1)
        continue;
      for (table = Dfirst(ea); table != BADADDR; table = Dnext(ea, table)) {
        LabelTable(table, ea);
        break;
      }
    }
  }
  Message("Done\n");
}

static LocateMapsIndirectHelper(base) {
  auto i, ea, table, found, code, j, validindex, index;
  for (i = RfirstB(base); i > 0; i = RnextB(base, i)) {
    if (i == BADADDR || i == 0)
      continue;
    // Look back at most 16 instructions for r4 being set
    for (ea = i; ea > 0 && ea > (i - 32); ea = ea - 2) {
      code = GetDisasm(ea);
      if (strstr(code, "mov.l ") == -1 || strstr(code, ", r4") == -1)
        continue;
      found = 0;
      for (index = Dfirst(ea); index != BADADDR; index = Dnext(ea, index)) {
        validindex = 1;
        for (j = 0; j < 8; j++) {
          MakeDword(index + j * 4);
          if (Dword(index + j * 4) > SegEnd(0)) {
            // Only consider tables in ROM
            validindex = 0;
            break;
          }
        }
        if (!validindex)
          break;
        for (j = 0; j < 8; j++) {
          LabelTable(Dword(index + j * 4), ea);
        }
        found = 1;
        MakeArray(index, 8);
        break;
      }
      if (found)
        break;
    }
  }
}

static LocateMaps() {
  LocateMapsHelper(BYTE_TABLE_LOOKUP_FUNC);
  LocateMapsHelper(WORD_TABLE_LOOKUP_FUNC);
  LocateMapsIndirectHelper(LocByName("table_lookup_byte_mapindex"));
  LocateMapsIndirectHelper(LocByName("table_lookup_word_mapindex"));
}

//-----------------------------------------------------------------------
// Get name of the current processor
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
  auto processor, newaf;

  SetLongPrm(INF_MAXREF, 16);
  SetCharPrm(INF_INDENT, 22);
  SetCharPrm(INF_COMMENT, 70);
  SetCharPrm(INF_MARGIN, 120);
  SetCharPrm(INF_PREFFLAG, PREF_SEGADR | PREF_FNCOFF); // show segment and function prefixes
  SetCharPrm(INF_NULL, 0);                             // don't generate empty lines
  SetCharPrm(INF_CMTFLAG, SW_ALLCMT);                  // show all comments
  SetCharPrm(INF_ASMTYPE, 0);                          // use GNU asm format
  {
    // Disable some analysis options that don't suit the SH2/SH4 code
    newaf = GetShortPrm(INF_START_AF);
    newaf = newaf & ~AF_MARKCODE; // Mark typical code sequences as code
    newaf = newaf & ~AF_FLIRT;    // Use flirt signatures
    newaf = newaf & ~AF_PROCPTR;  // Create function if data xref->code32 exists
    newaf = newaf & ~AF_LVAR;     // Create stack variables
    newaf = newaf & ~AF_TRACE;    // Trace stack pointer
    newaf = newaf & ~AF_ASCII;    // Create ascii string if data xref exists
    newaf = newaf & ~AF_FINAL;    // Final pass of analysis
    SetShortPrm(INF_START_AF, newaf);

    newaf = GetShortPrm(INF_AF2);
    newaf = newaf & ~AF2_JUMPTBL; // Locate and create jump tables
    newaf = newaf & ~AF2_DODATA;  // Coagulate data segs in final pass
    newaf = newaf & ~AF2_CHKUNI;  // Check for unicode strings
    SetShortPrm(INF_AF2, newaf);
  }

  processor = get_processor();
  if (strstr(processor, "SH4") == 0) {
    SegmentsSH4B();
    MakeNameEx(0x00000F52, "rom_id", SN_NOLIST);
    MakeDword(0x00000F52);
    Wait();
    CreateStructures();
    FixConstants();
    Wait();
    Fixup_VT(0x0000, SH_ROM_START);
    FixupJumps();
    Wait();
    Fix_Missing_Code(SH_ROM_START, 0xF20, 1);
    Fix_Missing_Code(Word(2), SegEnd(Word(2)), 0);
    Wait();
    FixDataOffsets();
    Wait();
    SH7052RegisterNames();
    Wait();
    Fix_MUT_Table();
    Wait();
    LabelLibraryFuncs();
    Wait();
    LocateAxisTables();
    Wait();
    LocateMaps();
    Wait();
    Message("SH4B Mitsubishi ECU autoanalysis finished\n");
  } else if (processor == "h8500") {
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

    MakeNameEx(0x0001021A, "rom_id", SN_NOLIST);
    MakeDword(0x0001021A);

    LowVoids(0);
    HighVoids(H8_CODE_OFFSET);
    Message("H8/500 Perform Vector Table Fixups \n");
    Fixup_VT(0x10000, H8_ROM_START);
    Message("H8/500 Mitsubishi ECU autonalysis finished.\n");
  } else {
    Message("Unknown processor type %s\n", processor);
  }
}
