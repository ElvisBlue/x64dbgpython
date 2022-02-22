from x64dbg import *
from collections import defaultdict

CALL_DWORD = 0
JMP_DWORD = 1
MOV_REG_DWORD = 2

PUSH_EAX = 0x50
PUSH_EBX = 0X53
PUSH_ECX = 0X51
PUSH_EDX = 0X52
PUSH_EBP = 0x55
PUSH_ESI = 0x56
PUSH_EDI = 0X57
POP_ESI = 0x5E
POP_EDI = 0x5F
POP_EBX = 0x5B

PUSH_CALL = 0
CALL_RET = 1
POP_CALL = 2

regsStrList = ["eax", "ebx", "ecx", "edx", "ebp", "esp", "esi", "edi"]

class CTraceInfo():
    def __init__(self, addrFrom, addrTo, type, regStr):
        self.addrFrom = addrFrom
        self.addrTo = addrTo
        self.type = type
        self.regStr = regStr
    
def Log(txt):
    print("[VMP IAT Fixer x86] " + txt)
    
def GetTextSection(sectionList):
    for section in sectionList:
        if section.name == ".text" or section.name == ".CODE":
            return section
            
    return None
    
def GetVMPSection(sectionList):
    for section in sectionList:
        if section.name[-1:] == "0":
            return section
            
    return None
    
def FindVMPCallPattern(textSection, vmpSection):
    retList = []
    currentLocation = textSection.addr
    while True:
        currentLocation = Pattern.FindMem(currentLocation, textSection.size - (currentLocation - textSection.addr), "E8")
        if currentLocation == 0:
            break
        
        if IsValidVMPCall(currentLocation, vmpSection):
            retList.append(currentLocation)
        currentLocation += 1
        
    return retList

def IsValidVMPCall(addr, vmpSection):
    if Memory.ReadByte(addr) == 0xE8:
        dst = (Memory.ReadDword(addr + 1) + addr + 5) & 0xFFFFFFFF
        if dst >= vmpSection.addr and dst <= (vmpSection.addr + vmpSection.size):
            return True
    
    return False
    
def GetCurrentRegisterList():
    registersList = []
    registersList.append(Register.GetEAX())
    registersList.append(Register.GetEBX())
    registersList.append(Register.GetECX())
    registersList.append(Register.GetEDX())
    registersList.append(Register.GetEBP())
    registersList.append(Register.GetESP())
    registersList.append(Register.GetESI())
    registersList.append(Register.GetEDI())
    return registersList

#Return diffirents registers between two list
def GetDiffRegs(regs1, regs2):
    retRegs = []
    for i in range(0, len(regs1)):
        if regs1[i] != regs2[i]:
            retRegs.append(regsStrList[i])
    
    return retRegs

def GetUnredirectedCall(textSection):
    retTraceInfo = []
    currentLocation = textSection.addr
    while True:
        currentLocation = Pattern.FindMem(currentLocation, textSection.size - (currentLocation - textSection.addr), "FF 15")
        if currentLocation == 0:
            break
        apiAddr = Memory.ReadDword(currentLocation + 2)
        if Memory.IsValidPtr(apiAddr):
            retTraceInfo.append(CTraceInfo(currentLocation, Memory.ReadDword(apiAddr), CALL_DWORD, None))
        currentLocation += 1
    return retTraceInfo

#This function returns CTraceInfo   
def TraceAddr(addr, tracingModule, textSection, vmpSection):
    Log("Tracing address 0x%X" % addr)
    oldESP = Register.GetESP()
    oldEIP = Register.GetEIP()
    
    #First we get the patch address
    patchedAddr = 0
    if Memory.ReadByte(addr + 5) == 0xC3:
        patchedAddr = addr
        patternType = CALL_RET
    elif Memory.ReadByte(addr - 1) in [PUSH_EAX, PUSH_EBX, PUSH_ECX, PUSH_EDX, PUSH_ESI, PUSH_EDI, PUSH_EBP]:
        patchedAddr = addr - 1
        patternType = PUSH_CALL
    elif Memory.ReadByte(addr - 1) in [POP_ESI, POP_EDI, POP_EBX]:
        patchedAddr = addr - 1
        patternType = POP_CALL
    else:
        Log("Trace failed at address: 0x%X. Unknown call/jmp/mov API pattern" % addr)
        return None
    
    Register.SetEIP(patchedAddr)
    registersBeforeTrace = GetCurrentRegisterList()
    Debug.StepIn()

    while (Register.GetEIP() >= tracingModule.base) and (Register.GetEIP() <= (tracingModule.base + tracingModule.size)):
        #Check if we land at special case
        if Register.GetEIP() == patchedAddr + 6:
            break
        Debug.StepIn()
    
    newEIP = Register.GetEIP()
    registersAfterTrace = GetCurrentRegisterList()
    
    Register.SetESP(oldESP)
    Register.SetEIP(oldEIP)
    
    traceInfo = None
    if newEIP == patchedAddr + 6:
        #Now this should be MOV_REG_DWORD
        diffRegs = GetDiffRegs(registersAfterTrace, registersBeforeTrace)
        if len(diffRegs) == 1:
            addrTo = registersAfterTrace[regsStrList.index(diffRegs[0])]
            traceInfo = CTraceInfo(patchedAddr, addrTo, MOV_REG_DWORD, diffRegs[0])
        else:
            Log("Could not resolve addr. Instruction at address 0x%X" % addr)
        return traceInfo
    
    if registersBeforeTrace == registersAfterTrace:
        traceInfo = CTraceInfo(patchedAddr, newEIP, JMP_DWORD, None)
    #Now we compare ESP before and after trace to see it's call dword api is not
    elif registersBeforeTrace[5] == registersAfterTrace[5] + 4:
        traceInfo = CTraceInfo(patchedAddr, newEIP, CALL_DWORD, None)
    else:
        Log("Could not find type of instruction at address 0x%X" % addr)
        
    return traceInfo
    
def FindCodeCaveInSection(section, size = 0x200):
    return Pattern.FindMem(section.addr, section.size, "00" * size)

def MakeCallAddr(addr, callToAddr):
    callInst = "call dword ptr ds:[0x%x]" % callToAddr
    Assembler.AssembleMem(addr, callInst)

def RestoreInst(traceInfo, apiAddr):
    inst = ""
    if traceInfo.type == CALL_DWORD:
        inst = "call dword ptr ds:[0x%x]" % apiAddr
    elif traceInfo.type == JMP_DWORD:
        inst = "jmp dword ptr ds:[0x%X]" % apiAddr
    elif traceInfo.type == MOV_REG_DWORD:
        inst = "mov %s, dword ptr ds:[0x%X]" % (traceInfo.regStr, apiAddr)
    else:
        Log("Unknown trace info")
        return
    
    Assembler.AssembleMem(traceInfo.addrFrom, inst)
    
def main():
    mainModule = Module.GetMainModuleInfo()
    sectionList = Module.SectionListFromAddr(mainModule.base)
    textSection = GetTextSection(sectionList)
    if textSection == None:
        Log("Cound not find .text section")
        return
    else:
        Log("Found text section: %s" % textSection.name)
       
    vmpSection = GetVMPSection(sectionList)
    if vmpSection == None:
        Log("Could not find vmp section")
        return
    else:
        Log("Found vmprotect section: %s" % vmpSection.name)
    
    callList = FindVMPCallPattern(textSection, vmpSection)
    
    #Get cal with un-redirected by vmprotect
    traceList = GetUnredirectedCall(textSection)
    
    for callAddr in callList:
        traceInfo = TraceAddr(callAddr, mainModule, textSection, vmpSection)
        if traceInfo != None:
            traceList.append(traceInfo)
    
    Log("Tracing done")    
    IATList = []
    for traceInfo in traceList:
        IATList.append(traceInfo.addrTo)
    
    #Remove duplicate and sort the list
    IATList = list(dict.fromkeys(IATList))
    IATList.sort()
    
    #Build new IAT
    newIATAddr = FindCodeCaveInSection(vmpSection, len(IATList) * 4)
    if newIATAddr == 0:
        Log("Could not find memory do redirect IAT table")
        return
        
    Log("Redirect IAT to address: 0x%X" % newIATAddr)
    currentIATPtr = newIATAddr
    for IAT in IATList:
        Memory.WriteDword(currentIATPtr, IAT)
        currentIATPtr += 4
    
    #Patch the call to the new IAT table
    for traceInfo in traceList:
        apiAddr = newIATAddr + IATList.index(traceInfo.addrTo) * 4
        RestoreInst(traceInfo, apiAddr)
    
    Log("Fixed")
        
    
if __name__ == "__main__":
    main()
        
    