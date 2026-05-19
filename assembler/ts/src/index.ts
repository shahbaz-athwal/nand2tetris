import { CodeTranslator } from "./code";
import { CommandType, Parser } from "./parser";
import { SymbolTable } from "./symbol-table";

const file = Bun.file("../../projects/6/pong/Pong.asm");
const text = await file.text();

const parser = new Parser(text);
const tranlator = new CodeTranslator();
const symbolTable = new SymbolTable();

const out: string[] = [];

// first pass
while (await parser.advance()) {
  const type = parser.commandType();
  if (type !== CommandType.L_COMMAND) continue;

  const symbol = parser.symbol();
  if (symbolTable.contains(symbol)) continue;

  symbolTable.addSymbol(symbol, parser.instructionAddress);
}
parser.resetCursor();

// second pass
while (await parser.advance()) {
  const type = parser.commandType();
  if (type !== CommandType.A_COMMAND) continue;

  const symbol = parser.symbol();
  if (symbolTable.contains(symbol)) continue;

  if (Number.isNaN(parseInt(symbol))) {
    symbolTable.addSymbol(symbol);
  }
}
parser.resetCursor();

while (await parser.advance()) {
  const type = parser.commandType();
  if (type === CommandType.A_COMMAND) {
    const parsedSymbol = parseInt(parser.symbol(), 10);
    const decimal = Number.isNaN(parsedSymbol)
      ? symbolTable.getAddress(parser.symbol())
      : parsedSymbol;
    out.push("0" + decimal.toString(2).padStart(15, "0"));
  }
  if (type === CommandType.C_COMMAND) {
    out.push(
      "111" +
        tranlator.comp(parser.comp()) +
        tranlator.dest(parser.dest()) +
        tranlator.jump(parser.jump()),
    );
  }
}

Bun.write("out/Add.hack", out.join("\n"));
