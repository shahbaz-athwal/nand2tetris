import { Parser } from "./parser";

const file = Bun.file("./Add.asm");
const text = await file.text();

const parser = new Parser(text);

while (await parser.advance()) {
  parser.print();
}
