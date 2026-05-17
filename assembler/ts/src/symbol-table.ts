export class SymbolTable {
  symbolTable: Map<string, number> = new Map([
    ["R0", 0],
    ["R1", 1],
    ["R2", 2],
    ["R3", 3],
    ["R4", 4],
    ["R5", 5],
    ["R6", 6],
    ["R7", 7],
    ["R8", 8],
    ["R9", 9],
    ["R10", 10],
    ["R11", 11],
    ["R12", 12],
    ["R13", 13],
    ["R14", 14],
    ["R15", 15],
    ["SP", 0],
    ["LCL", 1],
    ["ARG", 2],
    ["THIS", 3],
    ["THAT", 4],
    ["SCREEN", 16384],
    ["KBD", 24576],
  ]);
  private nextAddress = 16;

  addSymbol(symbol: string, address?: number) {
    this.symbolTable.set(symbol, address ?? this.nextAddress++);
  }

  contains(symbol: string): boolean {
    return this.symbolTable.has(symbol);
  }

  getAddress(symbol: string): number {
    return this.symbolTable.get(symbol)!;
  }
}
