//
// This file is distributed under the MIT License. See LICENSE for details.
//

#define DEBUG_TYPE "stub-proc-inst"

#include "smack/SmackOptions.h"
#include "llvm/Support/Debug.h"
#include "smack/StubProcInst.h"
#include "llvm/IR/IRBuilder.h"

#include <vector>
#include <map>

namespace smack {

using namespace llvm;

  bool StubProcInst::runOnModule(Module& M) {
    
    for (Module::iterator F = M.begin(); F != M.end(); ++F) {
       if (!F->isDeclaration())
        continue;
       for (auto &arg : F->getArgumentList()) {
          if (const PointerType* t = dyn_cast<const PointerType>(arg.getType())) {
            if (t->getElementType()->isPointerTy()) {
              // need to instrument body
              BasicBlock* block = BasicBlock::Create(getGlobalContext(), "entry", &*F);
              IRBuilder<> builder(block);
              auto allocaInst = builder.CreateAlloca(arg.getType(), nullptr, "tmp");
              auto storeInst = builder.CreateStore(&arg, allocaInst);

              auto callInst = builder.CreateCall(M.getNamedValue("__VERIFIER_nondet_pointer"), None, "call");
              auto bitcastRe = builder.CreateBitCast(callInst, t->getElementType(), "0");
              auto loadInst = builder.CreateLoad(allocaInst, "1");
              auto storeInst1 = builder.CreateStore(bitcastRe, loadInst);
              builder.CreateRetVoid();
              if (!F->hasFnAttribute("av-inst"))
                F->addFnAttr("av-inst");
            }
          }
        }
    }

    return true;
  }

// Pass ID variable
char StubProcInst::ID = 0;

// Register the pass
static RegisterPass<StubProcInst>
X("stub-proc-instrument", "Instrument stub functions with pointer to pointer as argument");

}
