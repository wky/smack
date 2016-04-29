//
// This file is distributed under the MIT License. See LICENSE for details.
//

#define DEBUG_TYPE "remove-dead-defs"

#include "smack/SmackOptions.h"
#include "smack/RemoveDeadDefs.h"
#include "llvm/Support/Debug.h"
#include "llvm/IR/DataLayout.h"

#include <vector>
#include <set>

namespace smack {

using namespace llvm;

bool RemoveDeadDefs::runOnModule(Module& M) {
  TD = &getAnalysis<DataLayoutPass>().getDataLayout();
  std::vector<Function*> dead;
  std::set<Function*> entryPoints;

  Module::iterator TF;
  for (TF = M.begin(); TF != M.end(); ++TF) {
    std::string name = TF->getName();
    if (SmackOptions::isEntryPoint(name))
      break;
  }

  if (TF == M.end()) {
    for (TF = M.begin(); TF != M.end(); ++TF) {
      std::string name = TF->getName();
      if (!TF->isDeclaration() 
           && name.find("__VERIFIER_") == std::string::npos
            && name.find("__SMACK_") == std::string::npos)
        entryPoints.insert(&*TF);
    }
  }

  do {
    dead.clear();
    for (Module::iterator F = M.begin(); F != M.end(); ++F) {
      std::string name = F->getName();

      if (!(F->isDefTriviallyDead() || F->getNumUses() == 0))
          continue;

      if (name.find("__SMACK_") != std::string::npos)
        if (name.find("__SMACK_check_memory_safety") == std::string::npos)
          continue;

      if (SmackOptions::isEntryPoint(name)
           || entryPoints.find(&*F) != entryPoints.end())
        continue;

      DEBUG(errs() << "removing dead definition: " << name << "\n");
      dead.push_back(F);
    }

    for (auto F : dead)
      F->eraseFromParent();
  } while (!dead.empty());

  return true;
}

// Pass ID variable
char RemoveDeadDefs::ID = 0;

// Register the pass
static RegisterPass<RemoveDeadDefs>
X("remove-dead-defs", "Remove Dead Definitions");

}
