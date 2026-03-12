from x64dbg import Function, Register

def main():
    currentRIP = Register.Get(Register.RegisterEnum.RIP)
    print("Checking for function at current RIP: 0x%X" % currentRIP)
    
    info = Function.GetInfo(currentRIP)
    if info:
        print("Function found!")
        print("Module: %s" % info.mod)
        print("Start RVA: 0x%X" % info.rvaStart)
        print("End RVA: 0x%X" % info.rvaEnd)
        print("Manual: %s" % info.manual)
        print("Instruction Count: %d" % info.instructioncount)
    else:
        print("No function at current RIP.")
        
    print("\nRetrieving all functions...")
    funcList = Function.GetList()
    if funcList:
        print("Found %d functions." % len(funcList))
        for i, func in enumerate(funcList[:5]): # Print up to top 5
            print("  Function %d: Module %s, RVA 0x%X-0x%X" % (i, func.mod, func.rvaStart, func.rvaEnd))
        if len(funcList) > 5:
            print("  ...")
    else:
        print("No functions found in the list.")

if __name__ == "__main__":
    main()
