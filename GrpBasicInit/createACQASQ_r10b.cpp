#include "createACQASQ_r10b.h"
#include "globals.h"
#include "../Queues/acq.h"
#include "../Queues/asq.h"
#include "../Utils/kernelAPI.h"


CreateACQASQ_r10b::CreateACQASQ_r10b(int fd, string grpName, string testName) :
    Test(fd, grpName, testName, SPECREV_10b)
{
    // 66 chars allowed:     xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
    mTestDesc.SetCompliance("revision 1.0b, section 7");
    mTestDesc.SetShort(     "Create an ACQ & ASQ");
    // No string size limit for the long description
    mTestDesc.SetLong(
        "Create ACQ & ASQ kernel objects with group lifespan. Report this data "
        "to the log directory filename: CreateACQASQ_r10b");
}


CreateACQASQ_r10b::~CreateACQASQ_r10b()
{
    ///////////////////////////////////////////////////////////////////////////
    // Allocations taken from the heap and not under the control of the
    // RsrcMngr need to be freed/deleted here.
    ///////////////////////////////////////////////////////////////////////////
}


CreateACQASQ_r10b::
CreateACQASQ_r10b(const CreateACQASQ_r10b &other) : Test(other)
{
    ///////////////////////////////////////////////////////////////////////////
    // All pointers in this object must be NULL, never allow shallow or deep
    // copies, see Test::Clone() header comment.
    ///////////////////////////////////////////////////////////////////////////
}


CreateACQASQ_r10b &
CreateACQASQ_r10b::operator=(const CreateACQASQ_r10b &other)
{
    ///////////////////////////////////////////////////////////////////////////
    // All pointers in this object must be NULL, never allow shallow or deep
    // copies, see Test::Clone() header comment.
    ///////////////////////////////////////////////////////////////////////////
    Test::operator=(other);
    return *this;
}


bool
CreateACQASQ_r10b::RunCoreTest()
{
    /** \verbatim
     * Assumptions:
     * 1) This is the 1st within GrpBasicInit.
     * 2) The NVME device is disabled
     * 3) All interrupts are disabled.
     * \endverbatim
     */

    KernelAPI::DumpKernelMetrics(mFd,
        FileSystem::PrepLogFile(mGrpName, mTestName, "kmetrics", "before"));

    // After disabling the defaults for AMS & CSS are fine, no need to modify
    if (gCtrlrConfig->SetMPS() == false)
        throw exception();

    SharedACQPtr acq = CAST_TO_ACQ(
        gRsrcMngr->AllocObj(Trackable::OBJ_ACQ, ACQ_GROUP_ID))
    acq->Init(5);

    SharedASQPtr asq = CAST_TO_ASQ(
        gRsrcMngr->AllocObj(Trackable::OBJ_ASQ, ASQ_GROUP_ID))
    asq->Init(5);

    if (gCtrlrConfig->SetState(ST_ENABLE) == false)
        throw exception();

    KernelAPI::DumpKernelMetrics(mFd,
        FileSystem::PrepLogFile(mGrpName, mTestName, "kmetrics", "after"));
    return true;
}
