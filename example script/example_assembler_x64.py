from x64dbg import Assembler, Register

def main():
    currentRIP = Register.Get(Register.RegisterEnum.RIP)
    print("Current RIP: 0x%X" % currentRIP)
    
    ops = Assembler.Assemble(currentRIP, "mov rax, 0")
    
    hexString = ""
    for c in ops:
        hexString += ("%02X " % c)
    print("'mov rax, 0' ops code: " + hexString)
    
    print("Try to assemble 'xor rax, rax' at memory 0x%X." % currentRIP)
    if(Assembler.AssembleMemEx(currentRIP, "xor rax, rax", True) == False):
        print("Failed to assemble at 0x%X" % currentRIP)
    
    print("Try to assemble 'pushad' at memory 0x%X. Result should be fail" % currentRIP)
    if(Assembler.AssembleMemEx(currentRIP, "pushad", True) == False):
        print("Failed to assemble at 0x%X" % currentRIP)
    
if __name__ == "__main__":
    main()