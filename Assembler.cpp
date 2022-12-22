//
//      Implementation of the Assembler class.
//
#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <iterator>
// Constructor for the assembler.  Note: we are passing argc and argv to the file access constructor.
// See main program.  
Assembler::Assembler( int argc, char *argv[] )
: m_facc( argc, argv )
{
    // Nothing else to do here at this point.
}  
// Destructor currently does nothing.  You might need to add something as you develope this project.  If not, we can delete it.
Assembler::~Assembler( )
{
}
/*
NAME

    PassI - Look for the established label positions.

SYNOPSIS

   void Assembler::PassI( )
       


DESCRIPTION

    This function will iterate through the source file, add symbols and establish label positions.
*/
// Pass I establishes the location of the labels.  You will write better function comments according to the coding standards.
void Assembler::PassI( ) 
{
    int loc = 0;// Tracks the location of the instructions to be generated.
    // Successively process each line of source code.
    for( ; ; ) {

        // Read the next line from the source file.
        string line; 
        if( ! m_facc.GetNextLine( line ) ) {

            // If there are no more lines, we are missing an end statement.
            // We will let this error be reported by Pass II.
            return;
        }
        
        // Parse the line and get the instruction type.
        Instruction::InstructionType st =  m_inst.ParseInstruction( line );

        // If this is an end statement, there is nothing left to do in pass I.
        // Pass II will determine if the end is the last statement and report an error if it isn't.
        if( st == Instruction::ST_End ) return;
        // Labels can only be on machine language and assembler language
        // instructions.  So, skip comments.
        if( st == Instruction::ST_Comment )  
        {
        	continue;
	    }
        // If the instruction has a label, record it and its location in the
        // symbol table.
        if( m_inst.isLabel( ) ) {

            m_symtab.AddSymbol( m_inst.GetLabel( ), loc );
        }
        // Compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction( loc );
    }
}
/*
NAME

    PassII - Look for the established label positions.

SYNOPSIS

   void Assembler::PassII()



DESCRIPTION

    This function will iterate through the source files to generate a translation from the test code.
*/
void Assembler::PassII() {
    string msg;

    m_facc.rewind();
    Errors::InitErrorReporting();
    int loc = 0; 
    cout << "Translation of the Program" << endl;
    cout << "Location    " << "Contents    " << "Original Statement" << endl;
    string newBuff = "";
    cout << loc << "             ";
    string m_Operd = m_inst.GetOperand(); //Operand
    string m_Op = m_inst.GetOpCode(); //OpCode
    int m_NumOp = m_inst.GetNumOpCode(); //NumOpCode

    //It should successively process each line of source code.
	for (; ; ) {
		// Read the next line from the source file.
		string buff; 
		if (!m_facc.GetNextLine(buff)) {
			msg = "Error:No End Statement";
			// Error for end statement
			Errors::RecordError(msg);
			Errors::DisplayErrors();
			return;
		}
		// Parse the line and get the instruction type.
		Instruction::InstructionType st = m_inst.ParseInstruction(buff);
		if (st == Instruction::ST_End) {
			cout << setw(35) << right << buff << endl;
			newBuff = buff;
			break;
		}
		// If this is a comment, just print it out and continue
		if (st == Instruction::ST_Comment) {
			cout << "                         " << buff << endl;
			continue;
		}
		cout << loc << "         ";
		string m_Oprd = m_inst.GetOperand(); // Operand
		string m_Op = m_inst.GetOpCode(); // Opcode
		int m_NumOp = m_inst.GetNumOpCode(); // NumOpCode
		// If this is a machine language instruction  error 
		if (st == Instruction::ST_MachineLanguage) {
			if (m_NumOp < 10) cout << "0";
			cout << m_NumOp;
			for (int i = 0; i < m_inst.GetLabel().length(); i++) {
				if (isdigit(m_inst.GetLabel().at(i))) {// Error if it has digits
					msg = "Error: Label in incorrect format";
					Errors::RecordError(msg);
					continue;
				}
			}
			for (int i = 0; i < m_Oprd.length(); i++) {
				if (isdigit(m_Oprd.at(i))) {
					msg = "Error: Invalid Operand Choice";
					Errors::RecordError(msg);
				}
			}
			int value = m_symtab.GetValueForKey(m_Oprd);
			int symVal = m_symtab.GetValueForKey(m_inst.GetLabel());
			if (symVal == -999) { // If symbol already exists report error for duplicate symbol
				msg = "Error: Duplicate Symbol";
				Errors::RecordError(msg);
			}
			else if (value < 1000) cout << "0";
			if (m_Op != "halt" && value == 1) {
				msg = "Error: Undefined Symbol";
				Errors::RecordError(msg);
			}
			else if ( m_Op == "halt") cout << "000";
			else cout << value;
			cout << "       " << setw(15) << right << m_inst.GetOrgInst();
			for (int i = 0; i < m_Oprd.length(); i++) {
				if (isdigit(m_Oprd.at(i))) {
					msg = "Error: Operand must be symbolic";
					cout << msg;
					Errors::RecordError(msg);
				}
			}
			if ( m_Op != "halt" && (m_Oprd.empty() || m_inst.GetOpCode().empty())) {
				msg = "Error: Invalid instruction";
				cout << msg; // If this is not a halt /operand /opcode are empty then throw an error
				Errors::RecordError(msg);
			}
			int lines;
			if (m_Op == "halt")  lines = stoi(to_string(m_NumOp) + "0000");
			else if (m_symtab.GetValueForKey(m_Oprd) < 10000) {
				lines = stoi(to_string(m_NumOp) + "0" + to_string(m_symtab.GetValueForKey(m_Oprd)));
			}
			else if (!m_emul.insertMemory(loc, lines)) {
				// If location value exceeds past 10000 then error 
				msg = "Error: Location hit the max for VC407 memory!";
				cout << msg;
				Errors::RecordError(msg);
			}
			else lines = stoi(to_string(m_NumOp) + to_string(m_symtab.GetValueForKey(m_Oprd)));
			
		}
		else if ( m_Op == "dc") { 
			for (int i = 0; i < m_Oprd.length(); i++) {
				if (!isdigit(m_Oprd.at(i))) {
					break;
				}
			}
			if (m_Oprd.empty()) {
				msg = "Error: DC operand must be numeric.";
				cout << "????" << setw(16) << right << m_inst.GetOrgInst() << msg << endl;
				Errors::RecordError(msg);
				continue;
			}
			if (stoi(m_Oprd) < 10) cout << "00000";
			else if (stoi(m_Oprd) < 100) cout << "0000";
			else if (stoi(m_Oprd) < 1000) cout << "000";
			else if (stoi(m_Oprd) < 10000) cout << "00";
			else if (stoi(m_Oprd) < 100000) cout << "0";
			else if (stoi(m_Oprd) > 999999) { // Report error if greater than 999999
				msg = "Error: Invalid constant";
				Errors::RecordError(msg);
				loc = m_inst.LocationNextInstruction(loc);
				continue;
			}
			cout << m_Oprd << "       " << setw(10) << right << m_inst.GetOrgInst();
			if (!m_emul.insertMemory(loc, stoi(m_Oprd))) {
				msg = "Error: Location too large for VC407 memory!";
				cout << msg;
				Errors::RecordError(msg);
			}
		}
		else if ( m_Op == "org") {
			cout << "             " << setw(15) << right << m_inst.GetOrgInst();
			for (int i = 0; i < m_Oprd.length(); i++) {
				if (!isdigit(m_Oprd.at(i))) {
					break;
				}
			}
			if ( m_Oprd.empty()) {
				msg = "Error: ORG must be numeric.";
				cout << msg << endl;
				Errors::RecordError(msg);
				continue;
			}
			if (stoi(m_Oprd) > 9999) {//Error, if greater than 9999
				Errors::RecordError(buff + " Invalid constant");
				continue;
			}
		}
		else if ( m_Op == "ds") {
			cout << "             " << setw(15) << right << m_inst.GetOrgInst();
			for (int i = 0; i < m_Oprd.length(); i++) {
				if (!isdigit(m_Oprd.at(i))) {
					break;
				}
			}
			if ( m_Oprd.empty()) {
				msg = "Error: DS operand must be numeric.";
				cout << msg << endl;
				Errors::RecordError(msg);
				continue;
			}
			if (stoi(m_Oprd) > 9999) { // Error, if greater than 9999
				msg = "Error: Invalid constant";
				Errors::RecordError(msg);
				loc = m_inst.LocationNextInstruction(loc);
				continue;
			}
		}
		cout << endl;
		loc = m_inst.LocationNextInstruction(loc);
	}
	Errors::DisplayErrors();
}

/*
NAME

     RunProgramInEmulator()- run the program by loading up the accumulator.

SYNOPSIS

   void Assembler::RunProgramInEmulator()



DESCRIPTION

    This function will run the source code within the file.
*/
void Assembler::RunProgramInEmulator() {
    m_emul.runProgram();
}
