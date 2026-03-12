from x64dbg import Misc, Module

def main():
    print("Testing Misc API...")
    
    print("\nParsing expression: 'cip+10'")
    result = Misc.ParseExpression("cip+10")
    print("Result of 'cip+10' is: 0x%X" % result)
    
    print("\nTesting GetProcAddress...")
    ntdll_info = Module.InfoFromName("ntdll.dll")
    if ntdll_info:
        proc_addr = Misc.RemoteGetProcAddress("ntdll.dll", "NtClose")
        if proc_addr:
            print("Remote address of ntdll.NtClose is: 0x%X" % proc_addr)
        else:
            print("Failed to get remote address of ntdll.NtClose")
    else:
        print("ntdll.dll not found in modules.")
        
    print("\nResolving label 'EntryPoint'...")
    label_addr = Misc.ResolveLabel("EntryPoint")
    if label_addr:
        print("EntryPoint found at: 0x%X" % label_addr)
    else:
        print("EntryPoint label not found.")

if __name__ == "__main__":
    main()
