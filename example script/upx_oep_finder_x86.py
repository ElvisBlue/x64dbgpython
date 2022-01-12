from x64dbg import Register, Memory, Debug, Pattern

def main():
    currentIP = Register.Get(Register.RegisterEnum.EIP)
    if Memory.ReadWord(currentIP) != 0xBE60:
        print("File is not packed with UPX or IP != EP")
        return
    
    patternAddr = Pattern.FindMem(currentIP, 0x1000, "83 EC ?? E9 ?? ?? ?? ?? 00")
    print("Find Pattern result: 0x%X" % patternAddr)
    
    Debug.SetBreakpoint(patternAddr)
    Debug.Run()
    Debug.Wait()
    Debug.DeleteBreakpoint(patternAddr)
    Debug.StepOver()
    Debug.StepOver()
    print("OEP = 0x%X" % Register.Get(Register.RegisterEnum.EIP))
    return
    
if __name__ == "__main__":
    main()