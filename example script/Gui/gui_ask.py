from x64dbg import Gui

def main():
    result = Gui.InputLine("Enter some string")
    print("Result of input line is %s" % result)
    result = Gui.InputValue("Enter value")
    print("Result of input value is %d" % result)
    
    if Gui.MessageYesNo("Is x64dbg python cool?"):
        Gui.Message(":)")
    else:
        Gui.Message(":(")
        

if __name__ == "__main__":
    main()