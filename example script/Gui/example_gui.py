from x64dbg import Gui

def main():
    print("Testing Gui API...")
    
    # Gui.Message("Hello from x64dbgpython Gui example!")
    
    ans = Gui.MessageYesNo("Do you like x64dbgpython?")
    if ans:
        print("User clicked Yes!")
    else:
        print("User clicked No.")
        
    # Example to get user input
    # text = Gui.InputLine("Enter some text:")
    # print("User entered: %s" % text)
    
    # value = Gui.InputValue("Enter a hex value:")
    # print("User entered value: 0x%X" % value)
    
    print("\nGetting selection from Disassembly...")
    sel = Gui.Disassembly.SelectionGet()
    if sel and len(sel) == 2:
        print("Disassembly Selection: 0x%X - 0x%X" % (sel[0], sel[1]))
    else:
        print("Could not get Disassembly selection.")

if __name__ == "__main__":
    main()
