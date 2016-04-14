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

std::string StubProcInst::CreateVarName(std::string prefix, unsigned seq)
{
  static unsigned counter;
  if (prefix == "")
    return std::to_string(counter);
  else
    return prefix + (seq? "" : std::to_string(seq));
}

bool StubProcInst::runOnModule(Module& M) {

    std::vector<llvm::Value*>allocaInsts;
    std::vector<llvm::Value*>storeArgInsts;
    std::vector<llvm::Value*>dpArgs;
    llvm::Value* retV = nullptr;
    
    for (Module::iterator F = M.begin(); F != M.end(); ++F) {
       if (!F->isDeclaration())
        continue;

       for (auto &arg : F->getArgumentList()) {
          if (const PointerType* t = dyn_cast<const PointerType>(arg.getType()))
            if (t->getElementType()->isPointerTy())
              dpArgs.push_back(&arg);
       }

       if (!dpArgs.size())
         continue;

        BasicBlock* block = BasicBlock::Create(getGlobalContext(), "entry", &*F);
        IRBuilder<> builder(block);

        if (!F->getReturnType()->isVoidTy())
          retV = builder.CreateAlloca(F->getReturnType());

        // First create alloca and store instructions
        for (auto dpArg : dpArgs) {
          auto allocaInst = builder.CreateAlloca(dpArg->getType(), nullptr, dpArg->getName() + ".addr");
          allocaInsts.push_back(allocaInst);
          auto storeArgInst = builder.CreateStore(dpArg, allocaInst);
          storeArgInsts.push_back(storeArgInst);
        }

        // Assign call to __VERIFIER_nondet_pointer to deference to dp args
        for (unsigned i = 0; i < dpArgs.size(); ++i) {
          auto callInst = builder.CreateCall(M.getNamedValue("__VERIFIER_nondet_pointer"), None, CreateVarName("call", i));
          auto bitcastRe = builder.CreateBitCast(callInst, (cast<const PointerType>(dpArgs[i]->getType()))->getElementType(), CreateVarName(""));
          auto loadInst = builder.CreateLoad(allocaInsts[i], CreateVarName(""));
          auto storeRetInst = builder.CreateStore(bitcastRe, loadInst);
        }

        // TODO: create proper return value
        if (!retV)
          builder.CreateRetVoid();
        if (!F->hasFnAttribute("av-inst"))
        F->addFnAttr("av-inst");
    }

    return true;
  }

// Pass ID variable
char StubProcInst::ID = 0;

// Register the pass
static RegisterPass<StubProcInst>
X("stub-proc-instrument", "Instrument stub functions with pointer to pointer as argument");

}
