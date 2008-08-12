
/**
 * @file SequentialMoveNodeSelector.cc
 *
 * Implementation of SequentialModeNodeSelector class.
 *
 * @author Heikki Kultala 2008 (hkultala@cs.tut.fi)
 * @note rating: red
 */

#include "SequentialMoveNodeSelector.hh"
#include "Instruction.hh"
#include "Move.hh"
#include "SpecialRegisterPort.hh"
#include "TerminalFUPort.hh"
#include "ProgramOperation.hh"

namespace TTAProgram {
}

namespace TTAMachine {
    class SpecialRegisterPort;
}

/**
 * Constructor. 
 * 
 * @param bb Basic block containing moves to be selected.
 */
SequentialMoveNodeSelector::SequentialMoveNodeSelector(BasicBlock& bb) {
    createMoveNodes(bb);
    mngIter_ = mngs_.begin();
}


/**
 * Destructor
 */
SequentialMoveNodeSelector::~SequentialMoveNodeSelector() {
/* TODO: delete  movenodes. and PO's  */
    while(mngs_.size()) {
        std::list<MoveNodeGroup*>::iterator iter = mngs_.begin();
        MoveNodeGroup* mng = *iter;
        for (int i = 0; i < mng->nodeCount(); i++) {
            delete &mng->node(i);
        }
        delete mng;
        mngs_.erase(iter);
    }

    while(programOperations_.size()) {
        std::list<ProgramOperation*>::iterator iter = 
            programOperations_.begin();
        delete *iter;
        programOperations_.erase(iter);
    }
}

/**
 * Gives a group of unselected movenodes to schedules.
 *
 * This always returns them in order; and every one just once.
 */
MoveNodeGroup 
SequentialMoveNodeSelector::candidates() {
    // are there any unselected movenodegroups left?
    if (mngIter_ != mngs_.end()) {
        // return one group and advance iter to next one
        return **mngIter_++;
    } else {
        // return empty mng
        return MoveNodeGroup();
    }
}

void 
SequentialMoveNodeSelector::mightBeReady(MoveNode&) {
}

void 
SequentialMoveNodeSelector::notifyScheduled(MoveNode&) {}

/**
 * Creates movenodes and programoperations from the given bb.
 */
void
SequentialMoveNodeSelector::createMoveNodes(BasicBlock& bb) {

    // PO which is being constructed.
    ProgramOperation* po = NULL;
    MoveNodeGroup* mng = NULL;

    for (int i = 0; i < bb.instructionCount(); i++) {
        TTAProgram::Instruction& ins = bb.instructionAtIndex(i);
        for (int j = 0; j < ins.moveCount(); j++) {
            // handle one move
            TTAProgram::Move& move = ins.move(j);
            MoveNode* moveNode = new MoveNode(move);
            TTAProgram::Terminal& source = move.source();
            TTAProgram::Terminal& dest = move.destination();
            bool isInOp = false;

            // handle dest of move, ie operand writes
            if( dest.isFUPort() &&
                !(dynamic_cast<const TTAMachine::SpecialRegisterPort*>(
                      &dest.port()))) {
                
                isInOp = true;
                TTAProgram::TerminalFUPort& tfpd=
                    dynamic_cast<TTAProgram::TerminalFUPort&>(dest);

                Operation* op = NULL;
                // if trigger
                if( tfpd.isOpcodeSetting()) {
                    op = &tfpd.operation();
                } else {
                    op = &tfpd.hintOperation();
                }
                if (po == NULL) {
                    assert(mng == NULL);
                    po = new ProgramOperation(*op);
                    mng = new MoveNodeGroup();
                }

                moveNode->setDestinationOperation(*po);
                po->addInputNode(*moveNode);
                mng->addNode(*moveNode);

                // if no result reads, like store.
                if (po->isComplete()) {
                    programOperations_.push_back(po);
                    po = NULL;
                    mngs_.push_back(mng);
                    mng = NULL;
                }
            }

            // check source of move, ie handle result reads.
            if (source.isFUPort() &&
                !(dynamic_cast<const TTAMachine::SpecialRegisterPort*>(
                      &source.port()))) {
                
                isInOp = true;

                if (po == NULL || mng == NULL) {
                    throw IllegalProgram(
                        __FILE__,__LINE__,__func__,"Orphan result read");
                }
                
                if (!po->isReady()) {
                    throw IllegalProgram(
                        __FILE__,__LINE__,__func__,"Missing some operand");
                }

                po->addOutputNode(*moveNode);
                moveNode->setSourceOperation(*po);
                mng->addNode(*moveNode);

                if (po->isComplete()) {
                    programOperations_.push_back(po);
                    po = NULL;
                    mngs_.push_back(mng);
                    mng = NULL;

                }
            }
            // register-to-register move.
            if (!isInOp) {
                if (mng != NULL) {
                    throw IllegalProgram(
                        __FILE__,__LINE__,__func__,
                        "reg-to-reg move in the middle of an operation!");
                }
                MoveNodeGroup* mng2 = new MoveNodeGroup;
                mng2->addNode(*moveNode);
                mngs_.push_back(mng2);
            }
        }
    }
    if (po != NULL || mng != NULL) {
        throw IllegalProgram(
            __FILE__,__LINE__,__func__,"Uncomplete operation at end of BB");
    }
}                