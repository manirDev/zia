/**
 * Zia Junior - Code Generator
 * Converts Blockly blocks to Zia language code
 */


// Create the Zia code generator
if (!Blockly.Zia) {
  Blockly.Zia = new Blockly.Generator("Zia")

  // Set up order of operations
  Blockly.Zia.ORDER_ATOMIC = 0
  Blockly.Zia.ORDER_FUNCTION_CALL = 2
  Blockly.Zia.ORDER_UNARY_NEGATION = 4.3
  Blockly.Zia.ORDER_MULTIPLICATION = 5.1
  Blockly.Zia.ORDER_DIVISION = 5.2
  Blockly.Zia.ORDER_ADDITION = 6.2
  Blockly.Zia.ORDER_SUBTRACTION = 6.1
  Blockly.Zia.ORDER_NONE = 99

  // Initialize the name database
  Blockly.Zia.init = function (workspace) {
    Object.getPrototypeOf(this).init.call(this, workspace)
    if (!this.nameDB_) {
      this.nameDB_ = new Blockly.Names(this.RESERVED_WORDS_)
    }
    // Fix for proper variable name handling
    this.nameDB_.setVariableMap(workspace.getVariableMap())
  }

  // Reserved words in Zia language
  Blockly.Zia.RESERVED_WORDS_ =
    "afficher,var,fonction,retourner,si,sinon,pour,tantque,selon,cas,defaut,quitter,continuer,et,ou,vrai,faux,nul,classe,ceci,super"
}

console.log("✅ Zia generator initialized")
