#include <iostream>
#include <fstream>
#include "pin.H"
using std::cerr;
using std::ofstream;
using std::ios;
using std::string;
using std::endl;

ofstream OutFile;

// count of total dynamic instructions
// static UINT64 ins_count = 0;

// This function is called before every instruction that is executed
VOID dynamicInsTrace(VOID * ip) { OutFile << "==> " << ip << endl; }

// Print a memory read record
VOID memTraceRead(VOID * addr) { OutFile << "Read " << addr << endl; }

// Print a memory write record
VOID memTraceWrite(VOID * addr) { OutFile << "Write " << addr <<endl; }

// Pin calls this function every time a new instruction is encountered
VOID Instruction(INS ins, VOID *v)
{
	// for dynamic instruction count
	INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)dynamicInsTrace, IARG_INST_PTR, IARG_END);

	UINT32 memOperands = INS_MemoryOperandCount(ins);
	// Iterate over each memory operand of the instruction.
	for (UINT32 memOp = 0; memOp < memOperands; memOp++)
	{
		// read
		if (INS_MemoryOperandIsRead(ins, memOp))
		{
			INS_InsertPredicatedCall(
				ins, IPOINT_BEFORE, (AFUNPTR)memTraceRead,
				IARG_MEMORYOP_EA, memOp,
				IARG_END);
		}
		// write
		else if (INS_MemoryOperandIsWritten(ins, memOp))
		{
			INS_InsertPredicatedCall(
				ins, IPOINT_BEFORE, (AFUNPTR)memTraceWrite,
				IARG_MEMORYOP_EA, memOp,
				IARG_END);
		}
	}
}

// Pin calls this function every time a new img is loaded
// It can instrument the image, but this example merely
// counts the number of static instructions in the image

VOID ImageLoad(IMG img, VOID *v)
{
	UINT32 count = 0;

	for (SEC sec = IMG_SecHead(img); SEC_Valid(sec); sec = SEC_Next(sec))
	{ 
		for (RTN rtn = SEC_RtnHead(sec); RTN_Valid(rtn); rtn = RTN_Next(rtn))
		{
			// Prepare for processing of RTN, an  RTN is not broken up into BBLs,
			// it is merely a sequence of INSs 
			RTN_Open(rtn);
			
			for (INS ins = RTN_InsHead(rtn); INS_Valid(ins); ins = INS_Next(ins))
			{
				count++;
			}

			// to preserve space, release data associated with RTN after we have processed it
			RTN_Close(rtn);
		}
	}
	OutFile << "Image " << IMG_Name(img).c_str() << " has " << count << " instructions\n";
}

KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool",
    "o", "p1.out", "specify output file name");

// This function is called when the application exits
VOID Fini(INT32 code, VOID *v)
{
    OutFile.close();
}

INT32 Usage()
{
    cerr << "This tool gives instruction count, Instruction Address Trace, Memory Reference Trace" << endl;
    cerr << endl << KNOB_BASE::StringKnobSummary() << endl;
    return -1;
}

int main(int argc, char * argv[])
{
    // Initialize pin
    if (PIN_Init(argc, argv)) return Usage();

	OutFile.open(KnobOutputFile.Value().c_str());
    OutFile.setf(ios::showbase);

    // Register Instruction to be called to instrument instructions
    INS_AddInstrumentFunction(Instruction, 0);
	// Register ImageLoad to be called when an image is loaded
    IMG_AddInstrumentFunction(ImageLoad, 0);

    // Register Fini to be called when the application exits
    // PIN_AddFiniFunction(Fini, 0);
    
    // Start the program, never returns
    PIN_StartProgram();
    
    return 0;
}
