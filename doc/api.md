# x64dbgpython API Reference

The `x64dbgpython` plugin exposes the following Python APIs, which map directly to `x64dbg`'s internal script APIs.

## 1. Argument
Argument handling (e.g. function arguments).
- `Argument.Add(start, end, manual, instructioncount)`
- `Argument.Get(addr)`
- `Argument.GetInfo(addr)` -> Returns `ArgumentInfo` object or `None`
- `Argument.Overlaps(start, end)`
- `Argument.Delete(addr)`
- `Argument.DeleteRange(start, end)`
- `Argument.Clear()`
- `Argument.GetList()` -> Returns list of `ArgumentInfo` objects or `None`

## 2. Assembler
Assembling instructions into memory.
- `Assembler.Assemble(addr, instruction)` -> Returns compiled `bytes` or `None`
- `Assembler.AssembleEx(addr, instruction)` -> Returns compiled `bytes` or `None` and prints errors
- `Assembler.AssembleMem(addr, instruction)`
- `Assembler.AssembleMemEx(addr, instruction, fillnop)` -> Assembles instruction directly to memory

## 3. Bookmark
Memory bookmarking.
- `Bookmark.Set(addr, manual)`
- `Bookmark.Get(addr)`
- `Bookmark.GetInfo(addr)` -> Returns `BookmarkInfo` object or `None`
- `Bookmark.Delete(addr)`
- `Bookmark.DeleteRange(start, end)`
- `Bookmark.Clear()`
- `Bookmark.GetList()` -> Returns list of `BookmarkInfo` objects or `None`

## 4. Comment
Inline disassembly comments.
- `Comment.Set(addr, text, manual)`
- `Comment.Get(addr)` -> Returns string
- `Comment.GetInfo(addr)` -> Returns `CommentInfo` object or `None`
- `Comment.Delete(addr)`
- `Comment.DeleteRange(start, end)`
- `Comment.Clear()`
- `Comment.GetList()` -> Returns list of `CommentInfo` objects or `None`

## 5. Debug
Debugger state control.
- `Debug.Wait()`
- `Debug.Run()`
- `Debug.Pause()`
- `Debug.Stop()`
- `Debug.StepIn()`
- `Debug.StepOver()`
- `Debug.StepOut()`
- `Debug.SetBreakpoint(addr)`
- `Debug.DeleteBreakpoint(addr)`
- `Debug.DisableBreakpoint(addr)`
- `Debug.SetHardwareBreakpoint(addr, type)` (HardwareType: HardwareAccess, HardwareWrite, HardwareExecute)
- `Debug.DeleteHardwareBreakpoint(addr)`

## 6. Flag
CPU condition flags.
- `Flag.Get(FlagEnum)`
- `Flag.Set(FlagEnum, value)`
- Getters/Setters for specific flags: `GetZF`, `SetZF`, `GetCF`, `SetCF`, `GetSF`, `SetSF`, `GetOF`, `SetOF`, `GetPF`, `SetPF`, `GetTF`, `SetTF`, `GetAF`, `SetAF`, `GetDF`, `SetDF`, `GetIF`, `SetIF`.

## 7. Function
Function analysis definitions.
- `Function.Add(start, end, manual, instructioncount)`
- `Function.Get(addr)`
- `Function.GetInfo(addr)` -> Returns `FunctionInfo` object or `None`
- `Function.Overlaps(start, end)`
- `Function.Delete(addr)`
- `Function.DeleteRange(start, end, manual)`
- `Function.Clear()`
- `Function.GetList()` -> Returns list of `FunctionInfo` objects or `None`

## 8. Gui
Interacting with x64dbg interface.
- `Gui.SelectionGet(Window)` -> Returns `[start, end]` loop or `None`. (Windows: DisassemblyWindow, DumpWindow, StackWindow, GraphWindow, MemMapWindow, SymModWindow)
- `Gui.SelectionSet(Window, start, end)`
- `Gui.SelectionGetStart(Window)`
- `Gui.SelectionGetEnd(Window)`
- `Gui.Message(text)`
- `Gui.MessageYesNo(text)` -> Returns `bool`
- `Gui.InputLine(title)` -> Returns `string` user input
- `Gui.InputValue(title)` -> Returns `duint` user input
- `Gui.Refresh()`
- *Submodules*: `Gui.Disassembly`, `Gui.Dump`, `Gui.Stack`, `Gui.Graph`, `Gui.MemMap`, `Gui.SymMod` 
  - (These submodules have their own `SelectionGet`, `SelectionSet`, `SelectionGetStart`, `SelectionGetEnd` wrappers).

## 9. Label
Memory address labels.
- `Label.Set(addr, text, manual)`
- `Label.FromString(text)` -> Returns address
- `Label.Get(addr)` -> Returns string
- `Label.GetInfo(addr)` -> Returns `LabelInfo` object or `None`
- `Label.Delete(addr)`
- `Label.DeleteRange(start, end)`
- `Label.Clear()`
- `Label.GetList()` -> Returns list of `LabelInfo` objects or `None`

## 10. Memory
Read/write/allocate memory.
- `Memory.Read(addr, size)` -> Returns `bytes` object or `None`
- `Memory.Write(addr, data)` -> Returns bytes written
- `Memory.IsValidPtr(addr)`
- `Memory.RemoteAlloc(addr, size)` -> Returns allocated address
- `Memory.RemoteFree(addr)`
- `Memory.GetProtect(addr)`
- `Memory.GetBase(addr, ...)` -> Returns base address of memory page
- `Memory.GetSize(addr, ...)` -> Returns size of memory page
- Value retrieval (also implements Write variants): `ReadByte`, `ReadWord`, `ReadDword`, `ReadQword`, `ReadPtr`.

## 11. Misc
Miscellaneous actions.
- `Misc.ParseExpression(expression)` -> Evaluates expression and returns address
- `Misc.RemoteGetProcAddress(module, name)` -> Returns address
- `Misc.ResolveLabel(label)` -> Returns address
- `Misc.Alloc(size)`
- `Misc.Free(address)`

## 12. Module
Module boundaries and exports.
- `Module.InfoFromAddr(addr)` -> Returns `ModuleInfo` object or `None`
- `Module.InfoFromName(name)` -> Returns `ModuleInfo` object or `None`
- `Module.BaseFromAddr(addr)`
- `Module.BaseFromName(name)`
- `Module.SizeFromAddr(addr)`
- `Module.SizeFromName(name)`
- `Module.NameFromAddr(addr)`
- `Module.PathFromAddr(addr)`
- `Module.EntryFromAddr(addr)`
- `Module.EntryFromName(name)`
- `Module.SectionCountFromAddr(addr)`
- `Module.SectionCountFromName(name)`
- `Module.SectionFromAddr(addr, number)` -> Returns `ModuleSectionInfo` 
- `Module.SectionFromName(name, number)` -> Returns `ModuleSectionInfo` 
- `Module.SectionListFromAddr(addr)` -> Returns list of `ModuleSectionInfo`
- `Module.SectionListFromName(name)` -> Returns list of `ModuleSectionInfo`
- `Module.GetMainModuleInfo()` -> Returns `ModuleInfo` object or `None`
- Main module helpers: `GetMainModuleBase`, `GetMainModuleSize`, `GetMainModuleEntry`, `GetMainModuleSectionCount`, `GetMainModuleName`, `GetMainModuleSectionList`.
- `Module.GetList()` -> Returns list of `ModuleInfo` objects
- `Module.GetExports(ModuleInfo)` -> Returns list of `ModuleExport` objects
- `Module.GetImports(ModuleInfo)` -> Returns list of `ModuleImport` objects

## 13. Pattern
Signature scanning.
- `Pattern.Find(addr, size, pattern)`
- `Pattern.FindMem(addr, size, pattern)`
- `Pattern.Write(addr, pattern)`
- `Pattern.WriteMem(addr, pattern)`
- `Pattern.SearchAndReplace(addr, size, searchpattern, replacepattern)`
- `Pattern.SearchAndReplaceMem(addr, size, searchpattern, replacepattern)`

## 14. Register
Reading and modifying CPU registers.
- `Register.Get(RegisterEnum)`
- `Register.Set(RegisterEnum, value)`
- `Register.Size()`
- Getters/Setters for architectural wrappers: `GetCAX`/`SetCAX`, `GetCBX`, `GetCCX`, `GetCDX`, `GetCDI`, `GetCSI`, `GetCBP`, `GetCSP`, `GetCIP`, `GetCFLAGS`.
- Getters/Setters for concrete registers (x86/x64): `GetEAX`, `GetRAX`, `GetAL`, `GetAH`, `GetRIP`, etc.

## 15. Stack
Inspect or manipulate the stack.
- `Stack.Pop()`
- `Stack.Push(value)`
- `Stack.Peek(offset)`

## 16. Symbol
Working with debugging symbols (e.g. from PDBs).
- `Symbol.GetList()` -> Returns list of `SymbolInfo` objects (types: Function, Import, Export).
