from x64dbg import Symbol

def main():
    print("Testing Symbol API...")
    
    print("Retrieving all symbols...")
    sym_list = Symbol.GetList()
    if sym_list:
        print("Found %d symbols." % len(sym_list))
        
        funcs = 0
        imports = 0
        exports = 0
        
        for sym in sym_list[:10]: # Print up to top 10
            sym_type_str = "Unknown"
            if sym.type == Symbol.SymbolType.Function:
                sym_type_str = "Function"
                funcs += 1
            elif sym.type == Symbol.SymbolType.Import:
                sym_type_str = "Import"
                imports += 1
            elif sym.type == Symbol.SymbolType.Export:
                sym_type_str = "Export"
                exports += 1
                
            print("  Symbol '%s': Module %s, RVA 0x%X (%s)" % (sym.name, sym.mod, sym.rva, sym_type_str))
            
        print("\nTypes of first 10 symbols:")
        print("Functions: %d, Imports: %d, Exports: %d" % (funcs, imports, exports))
        
        if len(sym_list) > 10:
            print("  ...")
    else:
        print("No symbols found.")

if __name__ == "__main__":
    main()
