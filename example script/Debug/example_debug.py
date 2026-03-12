from x64dbg import Debug, Register

def main():
    currentRIP = Register.Get(Register.RegisterEnum.RIP)
    print("Testing Debug API at current RIP: 0x%X" % currentRIP)
    
    # Example: Set a breakpoint at current instruction, step over, then remove it
    print("Setting breakpoint at 0x%X" % currentRIP)
    Debug.SetBreakpoint(currentRIP)
    
    print("Stepping over...")
    Debug.StepOver()
    
    # We might need to wait for the debug event to process
    Debug.Wait()
    
    print("Deleting breakpoint at 0x%X" % currentRIP)
    Debug.DeleteBreakpoint(currentRIP)
    
    newRIP = Register.Get(Register.RegisterEnum.RIP)
    print("New RIP after step over: 0x%X" % newRIP)

if __name__ == "__main__":
    main()
