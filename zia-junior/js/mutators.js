/**
 * Zia Junior - Mutator Definitions
 * Defines mutators for complex blocks like if/else and switch/case
 */


// Create French conditional blocks with proper mutator
const CONTROLS_SI_MUTATOR_MIXIN = {
  elseifCount_: 0,
  elseCount_: 0,

  mutationToDom: function () {
    if (!this.elseifCount_ && !this.elseCount_) {
      return null
    }
    const container = Blockly.utils.xml.createElement("mutation")
    if (this.elseifCount_) {
      container.setAttribute("elseif", this.elseifCount_)
    }
    if (this.elseCount_) {
      container.setAttribute("else", 1)
    }
    return container
  },

  domToMutation: function (xmlElement) {
    this.elseifCount_ = Number.parseInt(xmlElement.getAttribute("elseif"), 10) || 0
    this.elseCount_ = Number.parseInt(xmlElement.getAttribute("else"), 10) || 0
    this.rebuildShape_()
  },

  decompose: function (workspace) {
    const containerBlock = workspace.newBlock("controls_si_if")
    containerBlock.initSvg()
    let connection = containerBlock.nextConnection
    for (let i = 1; i <= this.elseifCount_; i++) {
      const elseifBlock = workspace.newBlock("controls_si_elseif")
      elseifBlock.initSvg()
      connection.connect(elseifBlock.previousConnection)
      connection = elseifBlock.nextConnection
    }
    if (this.elseCount_) {
      const elseBlock = workspace.newBlock("controls_si_else")
      elseBlock.initSvg()
      connection.connect(elseBlock.previousConnection)
    }
    return containerBlock
  },

  compose: function (containerBlock) {
    let clauseBlock = containerBlock.nextConnection.targetBlock()
    this.elseifCount_ = 0
    this.elseCount_ = 0
    const valueConnections = [null]
    const statementConnections = [null]
    let elseStatementConnection = null

    while (clauseBlock && !clauseBlock.isInsertionMarker()) {
      switch (clauseBlock.type) {
        case "controls_si_elseif":
          this.elseifCount_++
          valueConnections.push(clauseBlock.valueConnection_)
          statementConnections.push(clauseBlock.statementConnection_)
          break
        case "controls_si_else":
          this.elseCount_++
          elseStatementConnection = clauseBlock.statementConnection_
          break
      }
      clauseBlock = clauseBlock.nextConnection && clauseBlock.nextConnection.targetBlock()
    }
    this.updateShape_()
    this.reconnectChildBlocks_(valueConnections, statementConnections, elseStatementConnection)
  },

  saveConnections: function (containerBlock) {
    let clauseBlock = containerBlock.nextConnection.targetBlock()
    let i = 1
    while (clauseBlock) {
      switch (clauseBlock.type) {
        case "controls_si_elseif":
          const inputIf = this.getInput("IF" + i)
          const inputDo = this.getInput("DO" + i)
          clauseBlock.valueConnection_ = inputIf && inputIf.connection.targetConnection
          clauseBlock.statementConnection_ = inputDo && inputDo.connection.targetConnection
          i++
          break
        case "controls_si_else":
          const inputElse = this.getInput("ELSE")
          clauseBlock.statementConnection_ = inputElse && inputElse.connection.targetConnection
          break
      }
      clauseBlock = clauseBlock.nextConnection && clauseBlock.nextConnection.targetBlock()
    }
  },

  rebuildShape_: function () {
    const valueConnections = [null]
    const statementConnections = [null]
    let elseStatementConnection = null

    if (this.getInput("ELSE")) {
      elseStatementConnection = this.getInput("ELSE").connection.targetConnection
    }
    for (let i = 1; i <= this.elseifCount_; i++) {
      const inputIf = this.getInput("IF" + i)
      const inputDo = this.getInput("DO" + i)
      if (inputIf) {
        valueConnections.push(inputIf.connection.targetConnection)
      }
      if (inputDo) {
        statementConnections.push(inputDo.connection.targetConnection)
      }
    }
    this.updateShape_()
    this.reconnectChildBlocks_(valueConnections, statementConnections, elseStatementConnection)
  },

  updateShape_: function () {
    if (this.getInput("ELSE")) {
      this.removeInput("ELSE")
    }
    for (let i = 1; this.getInput("IF" + i); i++) {
      this.removeInput("IF" + i)
      this.removeInput("DO" + i)
    }
    for (let i = 1; i <= this.elseifCount_; i++) {
      this.appendValueInput("IF" + i)
        .setCheck("Boolean")
        .appendField("sinon si")
      this.appendStatementInput("DO" + i).appendField("faire")
    }
    if (this.elseCount_) {
      this.appendStatementInput("ELSE").appendField("sinon")
    }
  },

  reconnectChildBlocks_: function (valueConnections, statementConnections, elseStatementConnection) {
    for (let i = 1; i <= this.elseifCount_; i++) {
      Blockly.Mutator.reconnect(valueConnections[i], this, "IF" + i)
      Blockly.Mutator.reconnect(statementConnections[i], this, "DO" + i)
    }
    Blockly.Mutator.reconnect(elseStatementConnection, this, "ELSE")
  },
}

Blockly.Extensions.registerMutator("controls_si_mutator", CONTROLS_SI_MUTATOR_MIXIN, null, [
  "controls_si_if",
  "controls_si_elseif",
  "controls_si_else",
])

// Create selon (switch) blocks with proper mutator
const CONTROLS_SELON_MUTATOR_MIXIN = {
  caseCount_: 0,
  defaultCount_: 0,

  mutationToDom: function () {
    if (!this.caseCount_ && !this.defaultCount_) {
      return null
    }
    const container = Blockly.utils.xml.createElement("mutation")
    if (this.caseCount_) {
      container.setAttribute("case", this.caseCount_)
    }
    if (this.defaultCount_) {
      container.setAttribute("default", 1)
    }
    return container
  },

  domToMutation: function (xmlElement) {
    this.caseCount_ = Number.parseInt(xmlElement.getAttribute("case"), 10) || 0
    this.defaultCount_ = Number.parseInt(xmlElement.getAttribute("default"), 10) || 0
    this.rebuildShape_()
  },

  decompose: function (workspace) {
    const containerBlock = workspace.newBlock("controls_selon_switch")
    containerBlock.initSvg()
    let connection = containerBlock.nextConnection
    for (let i = 1; i <= this.caseCount_; i++) {
      const caseBlock = workspace.newBlock("controls_selon_case")
      caseBlock.initSvg()
      connection.connect(caseBlock.previousConnection)
      connection = caseBlock.nextConnection
    }
    if (this.defaultCount_) {
      const defaultBlock = workspace.newBlock("controls_selon_default")
      defaultBlock.initSvg()
      connection.connect(defaultBlock.previousConnection)
    }
    return containerBlock
  },

  compose: function (containerBlock) {
    let clauseBlock = containerBlock.nextConnection.targetBlock()
    this.caseCount_ = 0
    this.defaultCount_ = 0
    const valueConnections = []
    const statementConnections = []
    let defaultStatementConnection = null

    while (clauseBlock && !clauseBlock.isInsertionMarker()) {
      switch (clauseBlock.type) {
        case "controls_selon_case":
          this.caseCount_++
          valueConnections.push(clauseBlock.valueConnection_)
          statementConnections.push(clauseBlock.statementConnection_)
          break
        case "controls_selon_default":
          this.defaultCount_++
          defaultStatementConnection = clauseBlock.statementConnection_
          break
      }
      clauseBlock = clauseBlock.nextConnection && clauseBlock.nextConnection.targetBlock()
    }
    this.updateShape_()
    this.reconnectChildBlocks_(valueConnections, statementConnections, defaultStatementConnection)
  },

  saveConnections: function (containerBlock) {
    let clauseBlock = containerBlock.nextConnection.targetBlock()
    let i = 1
    while (clauseBlock) {
      switch (clauseBlock.type) {
        case "controls_selon_case":
          const inputCase = this.getInput("CASE" + i)
          const inputDo = this.getInput("DO" + i)
          clauseBlock.valueConnection_ = inputCase && inputCase.connection.targetConnection
          clauseBlock.statementConnection_ = inputDo && inputDo.connection.targetConnection
          i++
          break
        case "controls_selon_default":
          const inputDefault = this.getInput("DEFAULT")
          clauseBlock.statementConnection_ = inputDefault && inputDefault.connection.targetConnection
          break
      }
      clauseBlock = clauseBlock.nextConnection && clauseBlock.nextConnection.targetBlock()
    }
  },

  rebuildShape_: function () {
    const valueConnections = []
    const statementConnections = []
    let defaultStatementConnection = null

    if (this.getInput("DEFAULT")) {
      defaultStatementConnection = this.getInput("DEFAULT").connection.targetConnection
    }
    for (let i = 1; i <= this.caseCount_; i++) {
      const inputCase = this.getInput("CASE" + i)
      const inputDo = this.getInput("DO" + i)
      if (inputCase) {
        valueConnections.push(inputCase.connection.targetConnection)
      }
      if (inputDo) {
        statementConnections.push(inputDo.connection.targetConnection)
      }
    }
    this.updateShape_()
    this.reconnectChildBlocks_(valueConnections, statementConnections, defaultStatementConnection)
  },

  updateShape_: function () {
    if (this.getInput("DEFAULT")) {
      this.removeInput("DEFAULT")
    }
    for (let i = 1; this.getInput("CASE" + i); i++) {
      this.removeInput("CASE" + i)
      this.removeInput("DO" + i)
    }
    for (let i = 1; i <= this.caseCount_; i++) {
      this.appendValueInput("CASE" + i).appendField("cas")
      this.appendStatementInput("DO" + i).appendField("faire")
    }
    if (this.defaultCount_) {
      this.appendStatementInput("DEFAULT").appendField("defaut")
    }
  },

  reconnectChildBlocks_: function (valueConnections, statementConnections, defaultStatementConnection) {
    for (let i = 1; i <= this.caseCount_; i++) {
      Blockly.Mutator.reconnect(valueConnections[i - 1], this, "CASE" + i)
      Blockly.Mutator.reconnect(statementConnections[i - 1], this, "DO" + i)
    }
    Blockly.Mutator.reconnect(defaultStatementConnection, this, "DEFAULT")
  },
}

Blockly.Extensions.registerMutator("controls_selon_mutator", CONTROLS_SELON_MUTATOR_MIXIN, null, [
  "controls_selon_switch",
  "controls_selon_case",
  "controls_selon_default",
])

// Define the helper blocks for the mutators
Blockly.defineBlocksWithJsonArray([
  {
    type: "controls_selon_switch",
    message0: "selon",
    nextStatement: null,
    colour: "#3498db",
    tooltip: "Selon - structure de sélection",
    helpUrl: "",
  },
  {
    type: "controls_selon_case",
    message0: "cas",
    previousStatement: null,
    nextStatement: null,
    colour: "#3498db",
    tooltip: "Cas - option spécifique",
    helpUrl: "",
  },
  {
    type: "controls_selon_default",
    message0: "defaut",
    previousStatement: null,
    colour: "#3498db",
    tooltip: "Defaut - cas par défaut",
    helpUrl: "",
  },
  {
    type: "controls_si_if",
    message0: "si",
    nextStatement: null,
    colour: "#3498db",
    tooltip: "Si - condition principale",
    helpUrl: "",
  },
  {
    type: "controls_si_elseif",
    message0: "sinon si",
    previousStatement: null,
    nextStatement: null,
    colour: "#3498db",
    tooltip: "Sinon si - condition supplémentaire",
    helpUrl: "",
  },
  {
    type: "controls_si_else",
    message0: "sinon",
    previousStatement: null,
    colour: "#3498db",
    tooltip: "Sinon - cas par défaut",
    helpUrl: "",
  },
])

console.log("✅ Mutators defined")
