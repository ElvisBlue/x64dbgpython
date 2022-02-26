from x64dbg import *

def Log(txt):
    print("[Themida IAT fixer] " + txt)
    
def Trace(addr, mainModule):
    Log("Tracing address 0x%x" % addr)
    oldEIP = Register.GetEIP()
    Register.SetEIP(addr)
    while Register.GetEIP() >= mainModule.base and Register.GetEIP() <= (mainModule.base + mainModule.size):
        Debug.StepIn()
    
    resolvedAddr = Register.GetEIP()
    Register.SetEIP(oldEIP)
    return resolvedAddr
    
def main():
    Gui.Message("This script does not support advanced api wrapping")
    startIAT = Gui.InputValue("Start IAT")
    endIAT = Gui.InputValue("End IAT")
    Log("Start IAT: 0x%X" % startIAT)
    Log("End IAT: 0x%X" % endIAT)
    
    currentPtr = startIAT
    mainModule = Module.GetMainModuleInfo()
    while currentPtr <= endIAT:
        addr = Memory.ReadDword(currentPtr)
        if Memory.IsValidPtr(addr):
            if addr >= mainModule.base and addr <= (mainModule.base + mainModule.size):
                resolvedAddr = Trace(addr, mainModule)
                Memory.WriteDword(currentPtr, resolvedAddr)
        currentPtr += 4
    Gui.Message("Done")
    
if __name__ == "__main__":
    main()
    