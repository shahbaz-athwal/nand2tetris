export enum CommandType {
  A_COMMAND,
  C_COMMAND,
  L_COMMAND,
}

export class Parser {
  currentCommand: string = "";
  instructionAddress = 0;
  private lines: string[];
  private cursor = 0;

  constructor(fileContent: string) {
    this.lines = fileContent.split(/\r?\n/);
  }

  async advance(): Promise<boolean> {
    while (this.cursor < this.lines.length) {
      const line = this.lines[this.cursor++];

      // 1. Remove comments (anything after //)
      // 2. Remove all whitespace
      const cleaned = line!.split("//")[0]!.replace(/\s+/g, "");

      if (cleaned === "") {
        continue;
      }

      this.currentCommand = cleaned;
      if (this.commandType() !== CommandType.L_COMMAND) {
        this.instructionAddress++;
      }
      return true;
    }
    return false;
  }

  commandType(): CommandType {
    if (this.currentCommand.startsWith("@")) {
      return CommandType.A_COMMAND;
    } else if (this.currentCommand.startsWith("(")) {
      return CommandType.L_COMMAND;
    } else {
      return CommandType.C_COMMAND;
    }
  }

  symbol() {
    if (this.commandType() === CommandType.C_COMMAND) {
      throw Error("Not a symbol.");
    }

    if (this.currentCommand.startsWith("@")) {
      return this.currentCommand.replace("@", "");
    }

    return this.currentCommand.replace("(", "").replace(")", "");
  }

  dest() {
    if (this.commandType() !== CommandType.C_COMMAND) {
      throw Error("Not a C_COMMAND.");
    }
    const hasDest = this.currentCommand.includes("=");
    if (!hasDest) {
      // throw Error("No destination in the command.");
      return "null";
    }
    return this.currentCommand.split("=")[0]!;
  }

  comp() {
    if (this.commandType() !== CommandType.C_COMMAND) {
      throw Error("Not a C_COMMAND.");
    }
    const hasDest = this.currentCommand.includes("=");
    const hasJump = this.currentCommand.includes(";");

    if (hasDest && hasJump) {
      return this.currentCommand.split("=")[1]!.split(";")[0]!;
    }

    if (hasDest) {
      return this.currentCommand.split("=")[1]!;
    }

    if (hasJump) {
      return this.currentCommand.split(";")[0]!;
    }
    return this.currentCommand;
  }

  jump() {
    if (this.commandType() !== CommandType.C_COMMAND) {
      throw Error("Not a C_COMMAND.");
    }
    const hasJump = this.currentCommand.includes(";");
    if (!hasJump) {
      // throw Error("No jump in the command.");
      return "null";
    }
    return this.currentCommand.split(";")[1]!;
  }

  resetCursor() {
    this.cursor = 0;
    this.currentCommand = "";
    this.instructionAddress = 0;
  }

  print() {
    console.log(this.currentCommand);
  }
}
