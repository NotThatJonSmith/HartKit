# HartKit

Archived / Deprecated / Etc. All the useful code has been rolled up into GRIM.

Useful components for developing simulators of RISC-V processors. Includes:

* `HartState`, a concrete C++ class template that represents the state of a hart. The functional interface of `HartState` is designed to make instruction execution code concise and fast.
* `Translator` and `Transactor`, two virtual interfaces used by instruction code to access memory, which clients shall implement.
* `Translation` and `Transaction`, the corresponding concrete class templates that `Translator` and `Transactor` implementations shall serve.
* `Operands`, a concrete struct of the decoded operands of a RISC-V instruction. Thankfully, this doesn't change with XLEN.
* Instruction decode function templates, which produce `Operands` from `__uint32_t` instruction encodings.
* Instruction executor function templates, which mutate a `HartState` given an `Operands` and a `Transactor`.
* `CodePoint`, a struct with all the relevant function pointers (across all XLEN modes) and the width of a given instruction *encoding*, given the current XLEN mode and extensions vector of a hart.
    * Even so, this is not an *instruction* as such, because the same encoding could have different behavior given the state of the hart executing it.
    * Compressed instructions overlap some 128-bit instructions with some floating-point extension instructions, dependent on MXLEN. From there, the *current* XLEN mode the hart is in may attempt execution.
* `decode_instruction`, a naive (read: all nested switch statements) decoder of the RISC-V ISA. This is blessed with `constexpr` to enable fast precomputed lookups. It returns a `CodePoint`.
* `HartState` finishes the instruction decode procedure, by providing `Decode`, which trims down a `CodePoint` into a `HartState::Instruction`. Usually, clients should call this function instead of the raw `decode_instruction`.
    * This may indicate that the instructions and decoder logic could be split off into yet another repo. Maybe one day. It actually feels more likely that `HartState` should be split off as its own entity.

For all the templates provided in this repo, the template parameter is the XLEN mode the hart is currently operating in; switching XLEN implies switching all the objects of simulation. This is a rather deep-cutting architectural choice that was made to optimize the common paths and pessimize the uncommon paths. Since switching XLEN mode is considered rare, the potential speed increase of smaller (and hopefully more cache-friendly) register banks, simulator code blocks, etc. is considered worth the relatively enormous cost of reconstructing the whole simulator every time the Hart calls for a change in XLEN. A previous incarnation of this had many sub-objects under HartState templated over XLEN instead. Although the compiled code should look similar, the data in situ is very sparse in that old model.
