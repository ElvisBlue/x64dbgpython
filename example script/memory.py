from x64dbg import *

if __name__ == "__main__":
    currentAddr = Register.GetEIP()
    print("Current address: 0x%X" % currentAddr)
    print("Now read 100 bytes from address 0x%X" % currentAddr)
    
    data = Memory.Read(currentAddr, 100)
    print(data)
    
    print("Now write 100 bytes to address 0x%X" % currentAddr)
    sampleData = b""
    for i in range(0, 100):
        sampleData += i.to_bytes(1, "little")
    
    ret = Memory.Write(currentAddr, sampleData)
    print("%d byte(s) written to address 0x%X" % (ret, currentAddr))