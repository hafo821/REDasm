#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include "../plugins/plugins.h"
#include "types/listing.h"
#include "disassemblerbase.h"

namespace REDasm {

class Disassembler: public DisassemblerBase
{
    public:
        Disassembler(Buffer buffer, ProcessorPlugin* processor, FormatPlugin* format);
        virtual ~Disassembler();
        Listing &listing();
        void disassembleFunction(address_t address);
        void disassemble();

    public: // Primitive functions
        virtual InstructionPtr disassembleInstruction(address_t address);
        virtual void disassemble(address_t address);

    public:
        std::string out(const InstructionPtr& instruction, std::function<void(const Operand&, const std::string&)> opfunc);
        std::string out(const InstructionPtr& instruction);
        std::string comment(const InstructionPtr& instruction) const;

    private:
        void checkJumpTable(const InstructionPtr& instruction, const Operand &op);
        void analyzeOp(const InstructionPtr& instruction, const Operand& operand);
        InstructionPtr disassembleInstruction(address_t address, Buffer &b);

    private:
        ProcessorPlugin* _processor;
        PrinterPtr _printer;
        Listing _listing;
};

}

#endif // DISASSEMBLER_H