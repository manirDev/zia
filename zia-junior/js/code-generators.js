/**
 * Zia Junior - Code Generators
 * Generates Zia language code from blocks
 */

// Initialize the forBlock object
Blockly.Zia.forBlock = Blockly.Zia.forBlock || {}

// Code generators for custom blocks
Blockly.Zia.forBlock["afficher_text"] = (block, generator) => {
  const msg = generator.valueToCode(block, "TEXT", Blockly.Zia.ORDER_NONE) || '""'
  return "afficher(" + msg + ")\n"
}

Blockly.Zia.forBlock["demander_text"] = (block, generator) => {
  const prompt = generator.valueToCode(block, "PROMPT", Blockly.Zia.ORDER_NONE) || '"Question"'
  const variable = generator.nameDB_.getName(block.getFieldValue("VAR"), Blockly.Names.NameType.VARIABLE)
  return variable + " = demander(" + prompt + ")\n"
}

Blockly.Zia.forBlock["attendre"] = (block, generator) => {
  const duration = generator.valueToCode(block, "DURATION", Blockly.Zia.ORDER_NONE) || "1"
  return "attendre(" + duration + ")\n"
}

Blockly.Zia.forBlock["text"] = (block, generator) => {
  const code = JSON.stringify(block.getFieldValue("TEXT"))
  return [code, Blockly.Zia.ORDER_ATOMIC]
}

Blockly.Zia.forBlock["math_number"] = (block, generator) => {
  const code = Number.parseFloat(block.getFieldValue("NUM"))
  return [code, Blockly.Zia.ORDER_ATOMIC]
}

Blockly.Zia.forBlock["text_join"] = (block, generator) => {
  const elements = []
  for (let i = 0; i < block.itemCount_; i++) {
    elements[i] = generator.valueToCode(block, "ADD" + i, Blockly.Zia.ORDER_NONE) || '""'
  }
  return ["concatener(" + elements.join(", ") + ")", Blockly.Zia.ORDER_FUNCTION_CALL]
}

Blockly.Zia.forBlock["math_arithmetic"] = (block, generator) => {
  const OPERATORS = {
    ADD: "+",
    MINUS: "-",
    MULTIPLY: "*",
    DIVIDE: "/",
    POWER: "^",
  }
  const operator = OPERATORS[block.getFieldValue("OP")]
  const order = operator === "+" || operator === "-" ? Blockly.Zia.ORDER_ADDITION : Blockly.Zia.ORDER_MULTIPLICATION
  const a = generator.valueToCode(block, "A", order) || "0"
  const b = generator.valueToCode(block, "B", order) || "0"
  return ["(" + a + " " + operator + " " + b + ")", order]
}

Blockly.Zia.forBlock["controls_pour"] = (block, generator) => {
  const variable = generator.nameDB_.getName(block.getFieldValue("VAR"), Blockly.Names.NameType.VARIABLE)
  const from = generator.valueToCode(block, "FROM", Blockly.Zia.ORDER_NONE) || "1"
  const to = generator.valueToCode(block, "TO", Blockly.Zia.ORDER_NONE) || "10"
  const branch = generator.statementToCode(block, "DO")
  return "pour " + variable + " de " + from + " à " + to + ":\n" + generator.prefixLines(branch, "  ") + "fin\n"
}

Blockly.Zia.forBlock["controls_tantque"] = (block, generator) => {
  const condition = generator.valueToCode(block, "WHILE", Blockly.Zia.ORDER_NONE) || "faux"
  const branch = generator.statementToCode(block, "DO")
  return "tantque " + condition + ":\n" + generator.prefixLines(branch, "  ") + "fin\n"
}

Blockly.Zia.forBlock["controls_si"] = (block, generator) => {
  let code = ""
  const condition = generator.valueToCode(block, "IF0", Blockly.Zia.ORDER_NONE) || "faux"
  const branch = generator.statementToCode(block, "DO0")
  code += "si " + condition + ":\n" + generator.prefixLines(branch, "  ")

  for (let i = 1; i <= (block.elseifCount_ || 0); i++) {
    const elseifCondition = generator.valueToCode(block, "IF" + i, Blockly.Zia.ORDER_NONE) || "faux"
    const elseifBranch = generator.statementToCode(block, "DO" + i)
    code += "sinon si " + elseifCondition + ":\n" + generator.prefixLines(elseifBranch, "  ")
  }

  if (block.elseCount_) {
    const elseBranch = generator.statementToCode(block, "ELSE")
    code += "sinon:\n" + generator.prefixLines(elseBranch, "  ")
  }

  return code + "fin\n"
}

Blockly.Zia.forBlock["controls_selon"] = (block, generator) => {
  const switchValue = generator.valueToCode(block, "SWITCH", Blockly.Zia.ORDER_NONE) || "0"
  let code = "selon " + switchValue + ":\n"

  for (let i = 1; i <= (block.caseCount_ || 0); i++) {
    const caseValue = generator.valueToCode(block, "CASE" + i, Blockly.Zia.ORDER_NONE) || "0"
    const caseBranch = generator.statementToCode(block, "DO" + i)
    code += "  cas " + caseValue + ":\n" + generator.prefixLines(caseBranch, "    ")
  }

  if (block.defaultCount_) {
    const defaultBranch = generator.statementToCode(block, "DEFAULT")
    code += "  defaut:\n" + generator.prefixLines(defaultBranch, "    ")
  }

  return code + "fin\n"
}

Blockly.Zia.forBlock["logic_boolean"] = (block, generator) => {
  const code = block.getFieldValue("BOOL") === "TRUE" ? "vrai" : "faux"
  return [code, Blockly.Zia.ORDER_ATOMIC]
}

Blockly.Zia.forBlock["logic_compare"] = (block, generator) => {
  const OPERATORS = {
    EQ: "==",
    NEQ: "!=",
    LT: "<",
    LTE: "<=",
    GT: ">",
    GTE: ">=",
  }
  const operator = OPERATORS[block.getFieldValue("OP")]
  const order = Blockly.Zia.ORDER_NONE
  const a = generator.valueToCode(block, "A", order) || "0"
  const b = generator.valueToCode(block, "B", order) || "0"
  return ["(" + a + " " + operator + " " + b + ")", order]
}

Blockly.Zia.forBlock["logic_operation"] = (block, generator) => {
  const operator = block.getFieldValue("OP") === "AND" ? "et" : "ou"
  const order = Blockly.Zia.ORDER_NONE
  const a = generator.valueToCode(block, "A", order) || "faux"
  const b = generator.valueToCode(block, "B", order) || "faux"
  return ["(" + a + " " + operator + " " + b + ")", order]
}

Blockly.Zia.forBlock["variables_get"] = (block, generator) => {
  const code = generator.nameDB_.getName(block.getFieldValue("VAR"), Blockly.Names.NameType.VARIABLE)
  return [code, Blockly.Zia.ORDER_ATOMIC]
}

Blockly.Zia.forBlock["variables_set"] = (block, generator) => {
  const variable = generator.nameDB_.getName(block.getFieldValue("VAR"), Blockly.Names.NameType.VARIABLE)
  const value = generator.valueToCode(block, "VALUE", Blockly.Zia.ORDER_NONE) || "0"
  return variable + " = " + value + "\n"
}

Blockly.Zia.forBlock["var_declaration"] = (block, generator) => {
  const variable = generator.nameDB_.getName(block.getFieldValue("VAR"), Blockly.Names.NameType.VARIABLE)
  const value = generator.valueToCode(block, "VALUE", Blockly.Zia.ORDER_NONE) || "0"
  return "var " + variable + " = " + value + "\n"
}

Blockly.Zia.forBlock["controls_quitter"] = (block, generator) => "quitter\n"

Blockly.Zia.forBlock["controls_continuer"] = (block, generator) => "continuer\n"

// Function definition
Blockly.Zia.forBlock["fonction_definition"] = (block, generator) => {
  const name = block.getFieldValue("NAME")
  const params = block.getFieldValue("PARAMS")
  const body = generator.statementToCode(block, "STACK")
  return `fonction ${name}(${params}):\n${generator.prefixLines(body, "  ")}fin\n`
}

// Function call
Blockly.Zia.forBlock["fonction_call"] = (block, generator) => {
  const name = block.getFieldValue("NAME")
  const args = block.getFieldValue("ARGS")
  return `${name}(${args})\n`
}

// Return statement
Blockly.Zia.forBlock["retourner"] = (block, generator) => {
  const value = generator.valueToCode(block, "VALUE", Blockly.Zia.ORDER_NONE) || "nul"
  return `retourner ${value}\n`
}

// List creation
Blockly.Zia.forBlock["liste_create"] = (block, generator) => {
  const items = generator.valueToCode(block, "ITEMS", Blockly.Zia.ORDER_NONE) || ""
  return [`[${items}]`, Blockly.Zia.ORDER_ATOMIC]
}

// List access
Blockly.Zia.forBlock["liste_get"] = (block, generator) => {
  const list = generator.valueToCode(block, "LIST", Blockly.Zia.ORDER_NONE) || "[]"
  const index = generator.valueToCode(block, "INDEX", Blockly.Zia.ORDER_NONE) || "0"
  return [`${list}[${index}]`, Blockly.Zia.ORDER_ATOMIC]
}

// List set
Blockly.Zia.forBlock["liste_set"] = (block, generator) => {
  const list = generator.valueToCode(block, "LIST", Blockly.Zia.ORDER_NONE) || "liste"
  const index = generator.valueToCode(block, "INDEX", Blockly.Zia.ORDER_NONE) || "0"
  const value = generator.valueToCode(block, "VALUE", Blockly.Zia.ORDER_NONE) || "0"
  return `${list}[${index}] = ${value}\n`
}

// String length
Blockly.Zia.forBlock["texte_longueur"] = (block, generator) => {
  const text = generator.valueToCode(block, "TEXT", Blockly.Zia.ORDER_NONE) || '""'
  return [`longueur(${text})`, Blockly.Zia.ORDER_FUNCTION_CALL]
}

// String substring
Blockly.Zia.forBlock["texte_sous_chaine"] = (block, generator) => {
  const text = generator.valueToCode(block, "TEXT", Blockly.Zia.ORDER_NONE) || '""'
  const from = generator.valueToCode(block, "FROM", Blockly.Zia.ORDER_NONE) || "0"
  const to = generator.valueToCode(block, "TO", Blockly.Zia.ORDER_NONE) || "1"
  return [`sous_chaine(${text}, ${from}, ${to})`, Blockly.Zia.ORDER_FUNCTION_CALL]
}

// Math random
Blockly.Zia.forBlock["math_random"] = (block, generator) => {
  const from = generator.valueToCode(block, "FROM", Blockly.Zia.ORDER_NONE) || "0"
  const to = generator.valueToCode(block, "TO", Blockly.Zia.ORDER_NONE) || "100"
  return [`aleatoire(${from}, ${to})`, Blockly.Zia.ORDER_FUNCTION_CALL]
}

// Math round
Blockly.Zia.forBlock["math_round"] = (block, generator) => {
  const num = generator.valueToCode(block, "NUM", Blockly.Zia.ORDER_NONE) || "0"
  return [`arrondir(${num})`, Blockly.Zia.ORDER_FUNCTION_CALL]
}

// Comments
Blockly.Zia.forBlock["commentaire"] = (block, generator) => {
  const text = block.getFieldValue("TEXT")
  return `// ${text}\n`
}

// Try-catch
Blockly.Zia.forBlock["essayer_attraper"] = (block, generator) => {
  const tryCode = generator.statementToCode(block, "TRY")
  const errorVar = generator.nameDB_.getName(block.getFieldValue("ERROR_VAR"), Blockly.Names.NameType.VARIABLE)
  const catchCode = generator.statementToCode(block, "CATCH")
  return `essayer:\n${generator.prefixLines(tryCode, "  ")}attraper ${errorVar}:\n${generator.prefixLines(catchCode, "  ")}fin\n`
}

// Throw error
Blockly.Zia.forBlock["lancer_erreur"] = (block, generator) => {
  const message = generator.valueToCode(block, "MESSAGE", Blockly.Zia.ORDER_NONE) || '"Erreur"'
  return `lancer ${message}\n`
}

console.log("✅ Code generators defined")
console.log("📦 Available generators:", Object.keys(Blockly.Zia.forBlock || {}))
