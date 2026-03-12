from x64dbg import Argument, Register

def main():
    currentRIP = Register.Get(Register.RegisterEnum.RIP)
    print("Checking arguments at current RIP: 0x%X" % currentRIP)
    
    info = Argument.GetInfo(currentRIP)
    if info:
        print("Argument found at RIP!")
        print("Module: %s" % info.mod)
        print("Start RVA: 0x%X" % info.rvaStart)
        print("End RVA: 0x%X" % info.rvaEnd)
        print("Manual: %s" % info.manual)
        print("Instruction Count: %d" % info.instructioncount)
    else:
        print("No argument info at current RIP.")
        
    print("\nRetrieving all arguments...")
    argList = Argument.GetList()
    if argList:
        print("Found %d arguments." % len(argList))
        for i, arg in enumerate(argList[:5]): # Print up to top 5
            print("  Arg %d: Module %s, RVA 0x%X" % (i, arg.mod, arg.rvaStart))
        if len(argList) > 5:
            print("  ...")
    else:
        print("No arguments found in the list.")

if __name__ == "__main__":
    main()
