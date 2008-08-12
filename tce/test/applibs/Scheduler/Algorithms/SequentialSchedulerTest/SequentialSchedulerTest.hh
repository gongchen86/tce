/**
 * @file SequentialSchedulerTest.hh
 *
 * A test suite for the sequential scheduler. 
 *
 * This is practically the same as the test suite of basic block scheduler.
 *
 * @author Pekka J��skel�inen 2006 (pekka.jaaskelainen@tut.fi)
 * @author Heikki Kultala 2008 (hkultala@cs.tut.fi)
 * @note rating: red
 */

#ifndef FU_SEQUENTIAL_SCHEDULER_TEST_HH
#define FU_SEQUENTIAL_SCHEDULER_TEST_HH

#include <TestSuite.h>
#include "SequentialScheduler.hh"
#include "OperationPool.hh"
#include "UniversalMachine.hh"
#include "BinaryStream.hh"
#include "BinaryReader.hh"
#include "Program.hh"
#include "Application.hh"
#include "POMDisassembler.hh"
#include "SimpleGuardAllocatorCore.hh"
#include "Procedure.hh"
#include "Instruction.hh"
#include "InterPassData.hh"

class SequentialSchedulerTest : public CxxTest::TestSuite {
public:
    SequentialSchedulerTest();

    void setUp();
    void tearDown();

    void testScheduleShortImm();
    void testSchedule();
    void testScheduleShortImmAndReducedConnectivity();

private:
    int countOfGuardedMoves(const TTAProgram::Program& prog);
    /// Universal Machine for the unscheduled part.
    UniversalMachine umach_;
};

SequentialSchedulerTest::SequentialSchedulerTest() {
}

void
SequentialSchedulerTest::setUp() {
}

void
SequentialSchedulerTest::tearDown() {
}

/**
 * Tests scheduling against a machine with very short immediate fields
 * (3-4 bit) and reduced connectivity (which should result in temp moves).
 */
void
SequentialSchedulerTest::testScheduleShortImmAndReducedConnectivity() {
    //return;
    /// The tested input program with registers allocated.
    TTAProgram::Program* srcProgram = NULL;
    /// Target machine to schedule the program for.
    TTAMachine::Machine* targetMachine = NULL;

    CATCH_ANY(
        targetMachine =
        TTAMachine::Machine::loadFromADF(
            "data/3_bus_short_immediate_fields_and_reduced_connectivity.adf"));

    CATCH_ANY(
        srcProgram =
        TTAProgram::Program::loadFromTPEF(
            "data/arrmul_reg_allocated_10_bus.tpef",
            *targetMachine, umach_));

    const int ORIGINAL_PROCEDURE_COUNT = srcProgram->procedureCount();
    const int ORIGINAL_LABEL_COUNT =
        srcProgram->globalScopeConst().globalCodeLabelCount();
    const int ORIGINAL_GUARD_COUNT = countOfGuardedMoves(*srcProgram);

    InterPassData interPassData;

    // allocate the guards before passing the program to the BB scheduler
    for (int j = 0; j < srcProgram->procedureCount(); j++) {
        TTAProgram::Procedure& procedure = srcProgram->procedure(j);
        SimpleGuardAllocatorCore::allocateGuards(
            procedure, *targetMachine, interPassData);
    }

    TS_ASSERT_EQUALS(
        ORIGINAL_GUARD_COUNT, countOfGuardedMoves(*srcProgram));

    SequentialScheduler scheduler(interPassData);

    try {
        scheduler.handleProgram(*srcProgram, *targetMachine);
    } catch (const Exception& e) {
        Application::logStream() << e.errorMessageStack() << std::endl;
        assert(false);
    }
    // two guarded moves split, thus create two new guarded moves
    // + 4 delay slots filled, total +6
    TS_ASSERT_EQUALS(
        ORIGINAL_GUARD_COUNT, countOfGuardedMoves(*srcProgram));

    TS_ASSERT_EQUALS(
        srcProgram->procedureCount(), ORIGINAL_PROCEDURE_COUNT);

    TS_ASSERT_EQUALS(
        srcProgram->globalScopeConst().globalCodeLabelCount(),
        ORIGINAL_LABEL_COUNT);

#if 0
    Application::logStream()
        << std::endl << "after:" << std::endl
        << POMDisassembler::disassemble(*srcProgram, true) << std::endl;
#endif

    TTAProgram::Program::writeToTPEF(
        *srcProgram, "data/out.par.tpef");

    delete srcProgram;
    srcProgram = NULL;

    // test that the written TPEF is loadable back to POM
    CATCH_ANY(
        delete TTAProgram::Program::loadFromTPEF(
            "data/out.par.tpef",
            *targetMachine, umach_));
}


/**
 * Tests scheduling against a machine with very short immediate fields (4 bit),
 * thus should produce long immediate moves.
 */
void
SequentialSchedulerTest::testScheduleShortImm() {
    //return;
    /// The tested input program with registers allocated.
    TTAProgram::Program* srcProgram = NULL;
    /// Target machine to schedule the program for.
    TTAMachine::Machine* targetMachine = NULL;

    CATCH_ANY(
        targetMachine =
        TTAMachine::Machine::loadFromADF(
            "data/3_bus_short_immediate_fields.adf"));

    CATCH_ANY(
        srcProgram =
        TTAProgram::Program::loadFromTPEF(
            "data/arrmul_reg_allocated_10_bus.tpef",
            *targetMachine, umach_));

    const int ORIGINAL_PROCEDURE_COUNT = srcProgram->procedureCount();
    const int ORIGINAL_LABEL_COUNT =
        srcProgram->globalScopeConst().globalCodeLabelCount();
    const int ORIGINAL_GUARD_COUNT = countOfGuardedMoves(*srcProgram);

    InterPassData interPassData;

    // allocate the guards before passing the program to the BB scheduler
    for (int j = 0; j < srcProgram->procedureCount(); j++) {
        TTAProgram::Procedure& procedure = srcProgram->procedure(j);
        SimpleGuardAllocatorCore::allocateGuards(
            procedure, *targetMachine, interPassData);
    }

    SequentialScheduler scheduler(interPassData);

    CATCH_ANY(
        scheduler.handleProgram(*srcProgram, *targetMachine));

    // two guarded moves split, thus create two new guarded moves
    // + 10 guarded moves filled
    TS_ASSERT_EQUALS(
        ORIGINAL_GUARD_COUNT, countOfGuardedMoves(*srcProgram));

    TS_ASSERT_EQUALS(
        srcProgram->procedureCount(), ORIGINAL_PROCEDURE_COUNT);

    TS_ASSERT_EQUALS(
        srcProgram->globalScopeConst().globalCodeLabelCount(),
        ORIGINAL_LABEL_COUNT);

#if 0
    Application::logStream()
        << std::endl << "after:" << std::endl
        << POMDisassembler::disassemble(*srcProgram, true) << std::endl;
#endif

    TTAProgram::Program::writeToTPEF(
        *srcProgram, "data/arrmul_reg_allocated_10_bus.par.tpef");

    delete srcProgram;
    srcProgram = NULL;

    // test that the written TPEF is loadable back to POM
    CATCH_ANY(
        delete TTAProgram::Program::loadFromTPEF(
            "data/arrmul_reg_allocated_10_bus.par.tpef",
            *targetMachine, umach_));
}

/**
 * Returns the total count of guarded moves in the given program.
 *
 * @param prog The program to check.
 */
int
SequentialSchedulerTest::countOfGuardedMoves(
    const TTAProgram::Program& prog) {

    int count = 0;

    for (int procIndex = 0; procIndex < prog.procedureCount(); procIndex++) {
        TTAProgram::Procedure& procedure = prog.procedure(procIndex);
        for (int i = 0; i < procedure.instructionCount(); ++i) {
            TTAProgram::Instruction& instruction =
                procedure.instructionAtIndex(i);
            for (int m = 0; m < instruction.moveCount(); ++m) {
                TTAProgram::Move& move = instruction.move(m);
                if (!move.isUnconditional())
                    ++count;
            }
        }
    }
    return count;
}

/**
 * Tests scheduling the simple test case.
 */
void
SequentialSchedulerTest::testSchedule() {
    //return;
    /// The tested input program with registers allocated.
    TTAProgram::Program* srcProgram = NULL;
    /// Target machine to schedule the program for.
    TTAMachine::Machine* targetMachine = NULL;

    CATCH_ANY(
        targetMachine =
        TTAMachine::Machine::loadFromADF(
            "data/10_bus_full_connectivity.adf"));

    CATCH_ANY(
        srcProgram =
        TTAProgram::Program::loadFromTPEF(
            "data/arrmul_reg_allocated_10_bus.tpef",
            *targetMachine, umach_));

    const int ORIGINAL_PROCEDURE_COUNT = srcProgram->procedureCount();
    const int ORIGINAL_LABEL_COUNT =
        srcProgram->globalScopeConst().globalCodeLabelCount();
    const int ORIGINAL_GUARD_COUNT = countOfGuardedMoves(*srcProgram);

    InterPassData interPassData;

    // allocate the guards before passing the program to the BB scheduler
    for (int j = 0; j < srcProgram->procedureCount(); j++) {
        TTAProgram::Procedure& procedure = srcProgram->procedure(j);
        SimpleGuardAllocatorCore::allocateGuards(
            procedure, *targetMachine, interPassData);
    }

    TS_ASSERT_EQUALS(ORIGINAL_GUARD_COUNT, countOfGuardedMoves(*srcProgram));

    SequentialScheduler scheduler(interPassData);

    CATCH_ANY(
        scheduler.handleProgram(*srcProgram, *targetMachine));

    // delay slots filled with 7 guarded moves
    TS_ASSERT_EQUALS(ORIGINAL_GUARD_COUNT, countOfGuardedMoves(*srcProgram));

    TS_ASSERT_EQUALS(
        srcProgram->procedureCount(), ORIGINAL_PROCEDURE_COUNT);

    TS_ASSERT_EQUALS(
        srcProgram->globalScopeConst().globalCodeLabelCount(),
        ORIGINAL_LABEL_COUNT);

#if 0
    Application::logStream()
        << std::endl << "after:" << std::endl
        << POMDisassembler::disassemble(*srcProgram, true) << std::endl;
#endif

    TTAProgram::Program::writeToTPEF(
        *srcProgram, "data/arrmul_reg_allocated_10_bus.par.tpef");

    delete srcProgram;
    srcProgram = NULL;

    // test that the written TPEF is loadable back to POM
    CATCH_ANY(
        delete TTAProgram::Program::loadFromTPEF(
            "data/arrmul_reg_allocated_10_bus.par.tpef",
            *targetMachine, umach_));
}



#endif