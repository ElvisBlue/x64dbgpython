from x64dbg import *

def main():
    start, end = Gui.SelectionGet(Gui.Window.DisassemblyWindow)
    print("Disassembly Window: 0x%X - 0x%X" % (start, end))
    
    start, end = Gui.Disassembly.SelectionGet()
    print("Disassembly Window: 0x%X - 0x%X" % (start, end))
    
    start, end = Gui.Dump.SelectionGet()
    print("Dump Window: 0x%X - 0x%X" % (start, end))
    
    start, end = Gui.Stack.SelectionGet()
    print("Stack Window: 0x%X - 0x%X" % (start, end))

if __name__ == "__main__":
    main()