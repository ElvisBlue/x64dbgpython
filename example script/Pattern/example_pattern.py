from x64dbg import Pattern, Register

def main():
    currentRIP = Register.Get(Register.RegisterEnum.RIP)
    print("Testing Pattern API at current RIP: 0x%X" % currentRIP)
    
    # Simple pattern search (searching for 5 bytes of CC or whatever)
    # The signature format is like "90 90" or "CC ??"
    print("Searching for pattern 'CC CC' near RIP...")
    addr = Pattern.FindMem(currentRIP, 0x1000, "CC CC")
    if addr:
        print("Pattern found at: 0x%X" % addr)
    else:
        print("Pattern not found in the next 0x1000 bytes.")

if __name__ == "__main__":
    main()
