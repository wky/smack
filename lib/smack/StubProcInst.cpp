//
// This file is distributed under the MIT License. See LICENSE for details.
//

#define DEBUG_TYPE "stub-proc-inst"

#include "smack/SmackOptions.h"
#include "llvm/Support/Debug.h"
#include "smack/StubProcInst.h"

#include <vector>
#include <map>
#include <cassert>

namespace smack {

using namespace llvm;

std::string StubProcInst::CreateVarName(unsigned& seq)
{
    std::string ret = std::to_string(seq);
    seq += 1;
    return ret;
}

bool StubProcInst::runOnModule(Module& M) {

    
    for (Module::iterator F = M.begin(); F != M.end(); ++F) {
       if (!F->isDeclaration())
        continue;

       std::vector<llvm::Value*>allocaInsts;
       std::vector<llvm::Value*>storeArgInsts;
       std::vector<llvm::Value*>dpArgs;
       llvm::Value* retV = nullptr;
       unsigned int regCounter;

       for (auto &arg : F->getArgumentList()) {
          if (const PointerType* t = dyn_cast<const PointerType>(arg.getType()))
            if (t->getElementType()->isPointerTy())
              dpArgs.push_back(&arg);
       }

       if (!dpArgs.size())
         continue;

        BasicBlock* block = BasicBlock::Create(getGlobalContext(), "entry", &*F);
        IRBuilder<> builder(block);

        // First create alloca and store instructions
        for (auto dpArg : dpArgs) {
          auto allocaInst = builder.CreateAlloca(dpArg->getType(), nullptr, dpArg->getName() + ".addr");
          allocaInsts.push_back(allocaInst);
          auto storeArgInst = builder.CreateStore(dpArg, allocaInst);
          storeArgInsts.push_back(storeArgInst);
        }

        regCounter = F->arg_size();
        // Assign call to __VERIFIER_nondet_pointer to deference to dp args
        for (unsigned i = 0; i < dpArgs.size(); ++i) {
          auto callInst = builder.CreateCall(M.getNamedValue("__VERIFIER_nondet_pointer"),
                           None,
                            "call" + std::to_string(i));
          auto bitcastRe = builder.CreateBitCast(callInst,
                            (cast<const PointerType>(dpArgs[i]->getType()))->getElementType(),
                             CreateVarName(regCounter));
          auto loadInst = builder.CreateLoad(allocaInsts[i], CreateVarName(regCounter));
          auto storeRetInst = builder.CreateStore(bitcastRe, loadInst);
        }

        // TODO: create proper return value
        if (!F->getReturnType()->isVoidTy()) {
          Type* t = F->getReturnType();
          Value* ret;
          if (auto pt = dyn_cast<const PointerType>(t)) {
            auto callInst = builder.CreateCall(M.getNamedValue("__VERIFIER_nondet_pointer"),
                                                None, "call" + std::to_string(dpArgs.size()));
            if (pt->getElementType()->isVoidTy())
              ret = callInst;
            else {
              auto bitcastInst = builder.CreateBitCast(callInst, t, CreateVarName(regCounter));
              ret = bitcastInst;
            }
          } else if (auto it = dyn_cast<const IntegerType>(t)) {
            unsigned ByteWidth = (it->getBitWidth() >> 3U);
            std::string callFunc;
            if (ByteWidth == sizeof(char))
              callFunc = "__VERIFIER_nondet_uchar";
            else if (ByteWidth == sizeof(int))
              callFunc = "__VERIFIER_nondet_int";
            else if (ByteWidth == sizeof(short))
              callFunc = "__VERIFIER_nondet_ushort";
            else if (ByteWidth == sizeof(long))
              callFunc = "__VERIFIER_nondet_ulong";
            else
              callFunc = "__VERIFIER_nondet_bool";
            ret = builder.CreateCall(M.getNamedValue(callFunc), None, "call" + std::to_string(dpArgs.size()));
          } else
            assert(0 && "Return Type not supported");
          builder.CreateRet(ret);
        } else
          builder.CreateRetVoid();

        // TODO: should pass along a list of procedure
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
