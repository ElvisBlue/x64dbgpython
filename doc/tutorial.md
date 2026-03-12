# x64dbgpython API Comprehensive Tutorial

This tutorial provides a detailed, step-by-step walk-through of practically everything you can accomplish using the `x64dbgpython` API wrappers. 

By the end of this guide, you should be comfortable manipulating the debuggee's execution flow, inspecting structures in memory, disassembling and assembling code on the fly, and interacting with `x64dbg`'s user interface.

## Prerequisites
To interact with the x64dbg API, write your scripts in Python and ensure `x64dbgpython` is loaded in your x64dbg installation. You will need to import the associated wrappers:

```python
from x64dbg import *
```
*(Note: While `import *` works if you need everything, it's often better practice to explicitly import what you need—e.g., `from x64dbg import Register, Memory, Debug`)*

---

## Part 1: Register and Execution State
Before interacting with the program's code, you need to understand where the execution currently is. 

### Reading and Writing Registers
Registers hold the current context of the debugger.
```python
from x64dbg import Register

# Get the Instruction Pointer (Architecture independent: CIP. 64-bit: RIP. 32-bit: EIP)
current_ip = Register.GetCIP()
print("Currently stopped at: 0x%X" % current_ip)

# Read general purpose registers
rax = Register.Get(Register.RegisterEnum.CAX)
rcx = Register.GetCCX()
print("RAX/EAX:", hex(rax), "| RCX/ECX:", hex(rcx))

# Modify a register
Register.SetCAX(0xDEADBEEF)
print("New RAX/EAX value:", hex(Register.GetCAX()))
```

### Inspecting CPU Flags
You occasionally need to evaluate condition flags (Zero flag, Carry flag, Sign flag, etc.).
```python
from x64dbg import Flag

# Output Zero flag (often checked after 'cmp' or 'test' instructions)
is_zero = Flag.GetZF()
print("Zero Flag is:", is_zero)

# Force the Zero Flag to trigger a 'je' / 'jz' (Jump if Equal / Zero) branch
Flag.SetZF(True)
```

---

## Part 2: Module Information & Code Analysis
What module is your Instruction Pointer executing inside? Use the `Module` and `Symbol` APIs.

### Inspecting Modules
```python
from x64dbg import Module

# Find out what module the IP is located in
mod_info = Module.InfoFromAddr(current_ip)
if mod_info:
    print("Executing inside module:", mod_info.name)
    print("Module Base: 0x%X, Size: 0x%X" % (mod_info.base, mod_info.size))
    print("Module File Path:", mod_info.path)
    print("Number of Sections:", mod_info.sectionCount)

    # Dump the Exports
    exports = Module.GetExports(mod_info)
    if exports:
        print("Module has %d exported functions." % len(exports))
        for exp in exports[:3]:  # Print first 3
            print(" - Export: %s at 0x%X" % (exp.name, exp.rva + mod_info.base))
```

### Inspecting Symbols and Functions
If debugging symbols (PDBs) are loaded, you can resolve addresses to names.
```python
from x64dbg import Symbol, Function

# Retrieve all known symbols
syms = Symbol.GetList()
for sym in (syms[:5] if syms else []):
    print("Symbol Found: %s at RVA 0x%X" % (sym.name, sym.rva))

# Information about a specific function block at the current IP
func_info = Function.GetInfo(current_ip)
if func_info:
    print("Found function bounds: 0x%X to 0x%X" % (func_info.rvaStart, func_info.rvaEnd))
```

---

## Part 3: Reading, Writing, and Scanning Memory
The bread and butter of reverse engineering: What is stored in the debuggee's memory?

### Reading Basic Memory
```python
from x64dbg import Memory

# Check if memory is accessible before reading it to avoid crashes!
if Memory.IsValidPtr(current_ip):
    # Read the first byte (usually the opcode)
    first_byte = Memory.ReadByte(current_ip)
    print("First Opcode Byte: 0x%02X" % first_byte)
    
    # Read larger chunks (returns a bytes object)
    opcodes = Memory.Read(current_ip, 16)
    hex_str = " ".join(["%02X" % b for b in opcodes])
    print("16 Bytes at IP:", hex_str)
```

### Pattern Scanning (AOB Scanning)
Looking for a specific sequence of bytes or instructions (e.g. `48 83 EC ?? E9`)?
```python
from x64dbg import Pattern
import struct

# Scan the next 0x1000 bytes for a signature
# '??' acts as a wildcard
match_addr = Pattern.FindMem(current_ip, 0x1000, "48 83 EC ?? E9")
if match_addr:
    print("Pattern matched exactly at: 0x%X" % match_addr)

    # We can write memory directly to patch it!
    # Let's write some NOPs (0x90) over the first two bytes
    Memory.Write(match_addr, b"\x90\x90")
```

### Dynamic Allocation
You can safely allocate (and free) external memory inside the debuggee process.
```python
# Allocate a new 4096-byte (0x1000) page natively
cave_address = Memory.RemoteAlloc(0, 0x1000)
if cave_address:
    print("Code cave allocated at: 0x%X" % cave_address)
    
    # Do work here...
    
    # Free memory when you finish
    Memory.RemoteFree(cave_address)
```

---

## Part 4: Dynamic Code Assembly & Stack Execution
Rather than manually writing byte arrays (e.g. `\x90\x90`), try dynamically assembling human-readable instructions.

### The Assembler Wrapper
```python
from x64dbg import Assembler

# Output compiled bytes for an instruction list
bytecode = Assembler.Assemble(current_ip, "xor rax, rax")
print("Compiled 'xor rax, rax':", bytecode)

# Write the instruction natively to the debuggee's memory 
# The True flag means it pads any remaining bytes from the overwritten instruction with NOPs
success = Assembler.AssembleMemEx(current_ip, "mov rax, 0", True)
if success:
    print("Successfully patched 'mov rax, 0' directly at 0x%X" % current_ip)
```

### Stack Manipulation
You can quickly check the stack state.
```python
from x64dbg import Stack

# Peak the return address or argument at the top of the stack
top_of_stack = Stack.Peek(0)
print("Top of stack contains: 0x%X" % top_of_stack)

# Peak inside the stack securely (e.g., [ESP+8])
ptr_size = Register.Size()  # Returns 4 in x86 or 8 in x64
second_arg = Stack.Peek(ptr_size * 2) 
print("Stack [SP + %d] contains: 0x%X" % (ptr_size * 2, second_arg))
```

---

## Part 5: Interacting with the Debugger and GUI
Now that you have collected data, you want to actually automate the execution or interact with the user taking advantage of the script.

### Debug Execution
Breakpoints, stepping, and running.
```python
from x64dbg import Debug

# Set a breakpoint 10 bytes ahead
target_ip = current_ip + 10
Debug.SetBreakpoint(target_ip)

print("Running...")
Debug.Run()

# Wait for the debugger to hit a breakpoint or pause
Debug.Wait()

print("Hit target! Current IP is now: 0x%X" % Register.GetCIP())
Debug.DeleteBreakpoint(target_ip)

# Execute one single assembly instruction
Debug.StepIn()
Debug.StepOver()
```

### Graphical View & Automation
Provide interactive prompts, visual bookmarks, or annotations directly in the GUI.
```python
from x64dbg import Gui, Bookmark, Comment, Label

# Highlight the address by setting a Bookmark
Bookmark.Set(current_ip, True)

# Give it a name in the Disassembly view
Label.Set(current_ip, "MyHookedFunction", True)

# Give it an informative comment
Comment.Set(current_ip, "Script reached this point successfully!", True)

# Message the User
ans = Gui.MessageYesNo("Did you see the new Label, Comment, and Bookmark at the Instruction Pointer?")
if ans:
     print("Awesome.")

# Read User Input manually 
user_str = Gui.InputLine("Enter a module name to inspect (e.g. ntdll.dll):")
if user_str:
    print("You entered: %s" % user_str)
```

### Retrieving Information from UI
Allow the script user to select an address in the Disassembly window before running the script.
```python
# Returns an array: [start_address, end_address]
selection = Gui.Disassembly.SelectionGet()
if selection and len(selection) == 2:
    print("User highlighted: 0x%X to 0x%X. Patching now..." % (selection[0], selection[1]))
    # Iterate and manipulate instructions in selection!
```

---

## Part 6: Complex Address Resolution (Misc)
If you dynamically reference strings, labels, offsets, or expressions, use `Misc`.

```python
from x64dbg import Misc

# Resolving math strings to addresses exactly as if written in the x64dbg Command Bar
resolved_addr = Misc.ParseExpression("cip + 0x20")
print("Parsed Expression 'cip + 0x20' -> 0x%X" % resolved_addr)

# Grabbing Remote Windows APIs
nt_close_addr = Misc.RemoteGetProcAddress("ntdll.dll", "NtClose")
if nt_close_addr:
    print("Resolved ntdll!NtClose -> 0x%X" % nt_close_addr)
```

---
**Summary**

With these concepts, you can dynamically orchestrate highly complex debug iterations. A typical advanced script structure looks like this:

1. **Wait and Validate** (Are we in the right module?) -> (`Module`, `Register`)
2. **Scan the Code** (Where is the target algorithm?) -> (`Pattern`, `Memory`)
3. **Patch the Target** (Rewrite or hook it) -> (`Assembler`, `Memory.RemoteAlloc`)
4. **Automate Debugging** (Run until address, read registry, repeat) -> (`Debug`, `Register`, `Stack`)
5. **Report to User** (Add Comments, Labels, or Prompts) -> (`Comment`, `Label`, `Gui`)
