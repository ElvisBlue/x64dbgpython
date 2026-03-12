from x64dbg import Register

def main():
    print("Testing Register API...")
    
    rip = Register.GetRIP()
    
    try:
        rax = Register.GetRAX()
        is_64bit = True
    except AttributeError:
        # If we are strictly on x86, we might not have RAX available!
        is_64bit = False
        rax = 0
        eax = Register.GetEAX()
        print("Current EAX: 0x%X" % eax)

    print("Current RIP: 0x%X" % rip)
    if is_64bit:
        print("Current RAX: 0x%X" % rax)
    
    print("\nReading AL, AH, AX:")
    al = Register.GetAL()
    ah = Register.GetAH()
    ax = Register.GetAX()
    print("AL: 0x%02X" % al)
    print("AH: 0x%02X" % ah)
    print("AX: 0x%04X" % ax)
    
    print("\nGetting via Register enum...")
    cip = Register.Get(Register.RegisterEnum.CIP)
    print("Current CIP (depends on architecture): 0x%X" % cip)
    
    print("Size of CIP: %d bytes" % Register.Size())

if __name__ == "__main__":
    main()
