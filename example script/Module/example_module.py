from x64dbg import Module, Register

def main():
    currentRIP = Register.Get(Register.RegisterEnum.RIP)
    print("Testing Module API...")
    
    main_mod = Module.GetMainModuleInfo()
    if main_mod:
        print("Main Module Info:")
        print("  Name: %s" % main_mod.name)
        print("  Base: 0x%X" % main_mod.base)
        print("  Size: 0x%X" % main_mod.size)
        print("  Entry: 0x%X" % main_mod.entry)
        print("  Path: %s" % main_mod.path)
        print("  Sections: %d" % main_mod.sectionCount)
        
        print("\nExports in main module:")
        exports = Module.GetExports(main_mod)
        if exports:
            print("  Found %d exports." % len(exports))
            for i, exp in enumerate(exports[:3]):
                print("    - %s (RVA: 0x%X)" % (exp.name, exp.rva))
        else:
            print("  No exports found.")
            
        print("\nImports in main module:")
        imports = Module.GetImports(main_mod)
        if imports:
            print("  Found %d imports." % len(imports))
            for i, imp in enumerate(imports[:3]):
                print("    - %s (IAT RVA: 0x%X)" % (imp.name, imp.iatRva))
        else:
            print("  No imports found.")
    
    print("\nRetrieving module info for current RIP...")
    info = Module.InfoFromAddr(currentRIP)
    if info:
        print("Current RIP is inside module: %s" % info.name)
    else:
        print("Current RIP is not inside any known module.")
        
    print("\nRetrieving all modules...")
    mod_list = Module.GetList()
    if mod_list:
        print("Found %d modules." % len(mod_list))
        for mod in mod_list[:5]:
            print("  Module: %s (Base: 0x%X)" % (mod.name, mod.base))

if __name__ == "__main__":
    main()
