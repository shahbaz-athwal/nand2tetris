enum CommandType {
  A_COMMAND,
  C_COMMAND,
  L_COMMAND,
}

export class Parser {
  currentCommand: string;
  constructor() {}

  hasMoreCommand(): boolean {
    return false;
  }

  advance() {
    if (!this.hasMoreCommand()) {
      return;
    }
    // this.currentCommand = nextCommand
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
      throw Error("No destination in the command.");
    }
    return this.currentCommand.split("=")[0].split("").sort().join("");
  }

  comp() {
    if (this.commandType() !== CommandType.C_COMMAND) {
      throw Error("Not a C_COMMAND.");
    }
    const hasDest = this.currentCommand.includes("=");
    const hasJump = this.currentCommand.includes(";");

    if (hasDest && hasJump) {
      return this.currentCommand.split("=")[1].split(";")[0];
    }

    if (hasDest) {
      return this.currentCommand.split("=")[1];
    }

    if (hasJump) {
      return this.currentCommand.split(";")[0];
    }
    return this.currentCommand;
  }

  jump() {
    if (this.commandType() !== CommandType.C_COMMAND) {
      throw Error("Not a C_COMMAND.");
    }
    const hasJump = this.currentCommand.includes(";");
    if (!hasJump) {
      throw Error("No jump in the command.");
    }
    return this.currentCommand.split(";")[1];
  }
}
