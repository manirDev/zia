/**
 * Zia Junior - Example Programs
 * Pre-built example programs to demonstrate functionality
 */

const examples = {
  hello: `<xml xmlns="https://developers.google.com/blockly/xml">
        <block type="afficher_text" x="20" y="20">
            <value name="TEXT">
                <block type="text">
                    <field name="TEXT">Bonjour! Je suis Zia Junior 🎉</field>
                </block>
            </value>
        </block>
    </xml>`,

  count: `<xml xmlns="https://developers.google.com/blockly/xml">
        <block type="controls_pour" x="20" y="20">
            <field name="VAR">i</field>
            <value name="FROM">
                <block type="math_number">
                    <field name="NUM">1</field>
                </block>
            </value>
            <value name="TO">
                <block type="math_number">
                    <field name="NUM">5</field>
                </block>
            </value>
            <statement name="DO">
                <block type="afficher_text">
                    <value name="TEXT">
                        <block type="text_join">
                            <mutation items="2"></mutation>
                            <value name="ADD0">
                                <block type="text">
                                    <field name="TEXT">Comptage: </field>
                                </block>
                            </value>
                            <value name="ADD1">
                                <block type="variables_get">
                                    <field name="VAR">i</field>
                                </block>
                            </value>
                        </block>
                    </value>
                </block>
            </statement>
        </block>
    </xml>`,

  story: `<xml xmlns="https://developers.google.com/blockly/xml">
        <block type="var_declaration" x="20" y="20">
            <field name="VAR">jour</field>
            <value name="VALUE">
                <block type="math_number">
                    <field name="NUM">3</field>
                </block>
            </value>
            <next>
                <block type="controls_selon">
                    <value name="SWITCH">
                        <block type="variables_get">
                            <field name="VAR">jour</field>
                        </block>
                    </value>
                    <statement name="DO1">
                        <block type="afficher_text">
                            <value name="TEXT">
                                <block type="text">
                                    <field name="TEXT">Lundi - Début de semaine!</field>
                                </block>
                            </value>
                        </block>
                    </statement>
                    <statement name="DO2">
                        <block type="afficher_text">
                            <value name="TEXT">
                                <block type="text">
                                    <field name="TEXT">Mercredi - Milieu de semaine</field>
                                </block>
                            </value>
                        </block>
                    </statement>
                    <statement name="DEFAULT">
                        <block type="afficher_text">
                            <value name="TEXT">
                                <block type="text">
                                    <field name="TEXT">Autre jour de la semaine</field>
                                </block>
                            </value>
                        </block>
                    </statement>
                </block>
            </next>
        </block>
    </xml>`,
}

function loadExample(example) {
  if (examples[example] && window.workspace) {
    window.workspace.clear()
    const xml = Blockly.Xml.textToDom(examples[example])
    Blockly.Xml.domToWorkspace(xml, window.workspace)
    document.getElementById("output").textContent =
      `Exemple "${example}" chargé! Cliquez sur "Exécuter le Code" pour le tester.`
    console.log(`📚 Example "${example}" loaded`)
  }
}

// Declare Blockly variable before using it
const Blockly = window.Blockly

console.log("✅ Examples defined")
