//
// Class to parse and provide information about instructions.  Note: you will be adding more functionality.
//
#pragma once

// The elements of an instruction.
class Instruction {

public:

    Instruction() :m_IsNumericOperand{ false }, m_Operand1Value{ 0 }, m_type{ ST_Comment }, m_NumOpCode{ 0 }{ };
    ~Instruction( ) { };

    // Codes to indicate the type of instruction we are processing.  Why is this inside the
    // class?  We should make this an enum class.  We will do this during a lecture.
    enum InstructionType {
        ST_MachineLanguage, 	// A machine language instruction.
        ST_AssemblerInstr,      // Assembler Language instruction.
        ST_Comment,             // Comment or blank line
        ST_End                  // end instruction.
    };
    // Parse the Instruction.
    InstructionType ParseInstruction(string a_line);
    // Compute the location of the next instruction.
    int LocationNextInstruction(int a_loc);

    // To access the label
    inline string &GetLabel( ) {
         
        return m_Label;
    };
    // Access the OpCode
    inline string& GetOpCode() {

        return m_OpCode;
    };
    // Access the symbolic OpCode
    inline int& GetNumOpCode() {

        m_NumOpCode = CompNumCode();
        return m_NumOpCode;
    };
    //Access the operand
    inline string& GetOperand() {

        return m_Operand;
    };
    // Access the original instruction
    inline string& GetOrgInst() {

        return m_instruction;
    };
    // To determine if a label is blank.
    inline bool isLabel( ) {

        return ! m_Label.empty();
    };

    string CommnadsCap(string a_opcode);
    int CompNumCode();

private:


    // The elemements of a instruction
    string m_Label;        // The label.
    string m_OpCode;       // The symbolic op code.
    string m_Operand;     // The first operand. 
    string m_Operand2;     // The second operand.


    string m_instruction;    // The original instruction.

    // Derived values.
    int m_NumOpCode;     // The numerical value of the op code for machine language equivalents.
    InstructionType m_type; // The type of instruction.

    bool m_IsNumericOperand;// == true if the operand 1 is numeric.
    int m_Operand1Value;   // The value of the operand 1 if it is numeric.
    string CommnadCap(string& a_opcode);
    bool ParseLine(const string& a_line, string& a_label, string& a_opcode, string& a_operand);


};


