/**
 * Zia Junior - Language Interpreter
 * Executes Zia language code generated from blocks
 */

class ZiaInterpreter {
  constructor() {
    this.variables = {}
    this.output = []
    this.isRunning = false
    this.shouldStop = false
  }

  async execute(code) {
    this.variables = {}
    this.output = []
    this.isRunning = true
    this.shouldStop = false

    try {
      // Parse and execute the Zia code
      const lines = code.split("\n").filter((line) => line.trim())
      await this.executeLines(lines)
    } catch (error) {
      this.output.push("❌ Erreur: " + error.message)
    }

    this.isRunning = false
    return this.output.join("\n")
  }

  async executeLines(lines) {
    let i = 0
    while (i < lines.length && !this.shouldStop) {
      const line = lines[i].trim()
      if (line) {
        await this.executeLine(line)
      }
      i++
    }
  }

  async executeLine(line) {
    // Remove semicolon
    line = line.replace(/;$/, "")

    // Variable declaration
    if (line.startsWith("var ")) {
      this.executeVarDeclaration(line)
    }
    // Display function
    else if (line.startsWith("afficher(")) {
      this.executeAfficher(line)
    }
    // Input function
    else if (line.includes(" = demander(")) {
      await this.executeDemander(line)
    }
    // Wait function
    else if (line.startsWith("attendre(")) {
      await this.executeAttendre(line)
    }
    // Control structures (simplified for now)
    else if (line.startsWith("si (")) {
      this.output.push("⚠️ Structures de contrôle non implémentées dans cette version")
    } else if (line.startsWith("pour (")) {
      this.output.push("⚠️ Boucles non implémentées dans cette version")
    } else if (line.startsWith("tantque (")) {
      this.output.push("⚠️ Boucles non implémentées dans cette version")
    }
  }

  executeVarDeclaration(line) {
    const match = line.match(/var\s+(\w+)\s*=\s*(.+)/)
    if (match) {
      const varName = match[1]
      const value = this.evaluateExpression(match[2])
      this.variables[varName] = value
      this.output.push(`📦 Variable ${varName} = ${value}`)
    }
  }

  executeAfficher(line) {
    const match = line.match(/afficher$$(.+)$$/)
    if (match) {
      const value = this.evaluateExpression(match[1])
      this.output.push(`📺 ${value}`)
    }
  }

  async executeDemander(line) {
    const match = line.match(/var\s+(\w+)\s*=\s*demander$$(.+)$$/)
    if (match) {
      const varName = match[1]
      const prompt = this.evaluateExpression(match[2])

      // Simulate user input
      const userInput = window.prompt(prompt) || ""
      this.variables[varName] = userInput
      this.output.push(`❓ ${prompt}`)
      this.output.push(`✏️ Réponse: ${userInput}`)
      this.output.push(`📦 Variable ${varName} = "${userInput}"`)
    }
  }

  async executeAttendre(line) {
    const match = line.match(/attendre$$(.+)$$/)
    if (match) {
      const seconds = Number.parseFloat(this.evaluateExpression(match[1]))
      this.output.push(`⏰ Attente de ${seconds} seconde(s)...`)
      await new Promise((resolve) => setTimeout(resolve, seconds * 1000))
      this.output.push(`✅ Attente terminée`)
    }
  }

  evaluateExpression(expr) {
    expr = expr.trim()

    // String literal
    if (expr.startsWith('"') && expr.endsWith('"')) {
      return expr.slice(1, -1)
    }

    // Number
    if (!isNaN(expr)) {
      return Number.parseFloat(expr)
    }

    // Boolean
    if (expr === "vrai") return true
    if (expr === "faux") return false

    // Variable
    if (this.variables.hasOwnProperty(expr)) {
      return this.variables[expr]
    }

    // String concatenation
    if (expr.includes(" + ")) {
      const parts = expr.split(" + ")
      return parts.map((part) => this.evaluateExpression(part.trim())).join("")
    }

    // Arithmetic operations
    if (expr.includes(" * ")) {
      const parts = expr.split(" * ")
      return parts.reduce((acc, part) => acc * this.evaluateExpression(part.trim()), 1)
    }

    if (expr.includes(" / ")) {
      const parts = expr.split(" / ")
      return parts.reduce((acc, part) => acc / this.evaluateExpression(part.trim()))
    }

    if (expr.includes(" - ")) {
      const parts = expr.split(" - ")
      let result = this.evaluateExpression(parts[0].trim())
      for (let i = 1; i < parts.length; i++) {
        result -= this.evaluateExpression(parts[i].trim())
      }
      return result
    }

    return expr
  }

  stop() {
    this.shouldStop = true
  }
}

// Global interpreter instance
const ziaInterpreter = new ZiaInterpreter()
