from x64dbg import Memory, Register

def main():
    currentRIP = Register.Get(Register.RegisterEnum.RIP)
    print("Testing Memory API at current RIP: 0x%X" % currentRIP)
    
    is_valid = Memory.IsValidPtr(currentRIP)
    print("Is pointer valid? %s" % str(is_valid))
    
    if is_valid:
        byte_val = Memory.ReadByte(currentRIP)
        word_val = Memory.ReadWord(currentRIP)
        dword_val = Memory.ReadDword(currentRIP)
        
        print("Read Byte: 0x%02X" % byte_val)
        print("Read Word: 0x%04X" % word_val)
        print("Read Dword: 0x%08X" % dword_val)
        
        print("\nReading multiple bytes...")
        read_bytes = Memory.Read(currentRIP, 16)
        if read_bytes:
            hex_str = " ".join(["%02X" % c for c in read_bytes])
            print("16 bytes: %s" % hex_str)
            
        base = Memory.GetBase(currentRIP, False, True)
        size = Memory.GetSize(currentRIP, False, True)
        print("\nMemory Block Base: 0x%X" % base)
        print("Memory Block Size: 0x%X" % size)
    else:
        print("Cannot read from invalid pointer.")
        
    print("\nRemote allocation test...")
    remote_addr = Memory.RemoteAlloc(0x0, 0x1000)
    print("Allocated 0x1000 bytes at remote address: 0x%X" % remote_addr)
    if remote_addr:
        print("Freeing remote allocation...")
        Memory.RemoteFree(remote_addr)

if __name__ == "__main__":
    main()
