/**
 * Zia Junior - Block Definitions
 * Defines all custom blocks for the Zia visual programming language
 */


// Define custom blocks for Zia language
Blockly.defineBlocksWithJsonArray([
  {
    type: "afficher_text",
    message0: "afficher %1",
    args0: [
      {
        type: "input_value",
        name: "TEXT",
        check: ["String", "Number"],
      },
    ],
    previousStatement: null,
    nextStatement: null,
    colour: "#e74c3c",
    tooltip: "Affiche du texte à l'écran",
    helpUrl: "",
  },
  {
    type: "var_declaration",
    message0: "var %1 = %2",
    args0: [
      {
        type: "field_variable",
        name: "VAR",
        variable: "ma_variable",
      },
      {
        type: "input_value",
        name: "VALUE",
        check: ["String", "Number", "Boolean"],
      },
    ],
    previousStatement: null,
    nextStatement: null,
    colour: "#9b59b6",
    tooltip: "Déclare une variable avec une valeur",
    helpUrl: "",
  },
  {
    type: "demander_text",
    message0: "demander %1 et stocker dans %2",
    args0: [
      {
        type: "input_value",
        name: "PROMPT",
        check: "String",
      },
      {
        type: "field_variable",
        name: "VAR",
        variable: "réponse",
      },
    ],
    previousStatement: null,
    nextStatement: null,
    colour: "#e74c3c",
    tooltip: "Demande une saisie à l'utilisateur",
    helpUrl: "",
  },
  {
    type: "attendre",
    message0: "attendre %1 secondes",
    args0: [
      {
        type: "input_value",
        name: "DURATION",
        check: "Number",
      },
    ],
    previousStatement: null,
    nextStatement: null,
    colour: "#e74c3c",
    tooltip: "Fait une pause dans l'exécution",
    helpUrl: "",
  },
  {
    type: "controls_si",
    message0: "si %1",
    args0: [
      {
        type: "input_value",
        name: "IF0",
        check: "Boolean",
      },
    ],
    message1: "faire %1",
    args1: [
      {
        type: "input_statement",
        name: "DO0",
      },
    ],
    previousStatement: null,
    nextStatement: null,
    colour: "#3498db",
    tooltip: "Condition si/alors",
    helpUrl: "",
    mutator: "controls_si_mutator",
  },
  {
    type: "controls_pour",
    message0: "pour %1 de %2 à %3",
    args0: [
      {
        type: "field_variable",
        name: "VAR",
        variable: "i",
      },
      {
        type: "input_value",
        name: "FROM",
        check: "Number",
      },
      {
        type: "input_value",
        name: "TO",
        check: "Number",
      },
    ],
    message1: "faire %1",
    args1: [
      {
        type: "input_statement",
        name: "DO",
      },
    ],
    previousStatement: null,
    nextStatement: null,
    colour: "#3498db",
    tooltip: "Boucle pour avec compteur",
    helpUrl: "",
  },
  {
    type: "controls_tantque",
    message0: "tantque %1",
    args0: [
      {
        type: "input_value",
        name: "WHILE",
        check: "Boolean",
      },
    ],
    message1: "faire %1",
    args1: [
      {
        type: "input_statement",
        name: "DO",
      },
    ],
    previousStatement: null,
    nextStatement: null,
    colour: "#3498db",
    tooltip: "Répète tant que la condition est vraie",
    helpUrl: "",
  },
  {
    type: "logic_boolean",
    message0: "%1",
    args0: [
      {
        type: "field_dropdown",
        name: "BOOL",
        options: [
          ["vrai", "TRUE"],
          ["faux", "FALSE"],
        ],
      },
    ],
    output: "Boolean",
    colour: "#5ba55b",
    tooltip: "Valeur booléenne vraie ou fausse",
    helpUrl: "",
  },
  {
    type: "logic_compare",
    message0: "%1 %2 %3",
    args0: [
      {
        type: "input_value",
        name: "A",
      },
      {
        type: "field_dropdown",
        name: "OP",
        options: [
          ["=", "EQ"],
          ["≠", "NEQ"],
          ["<", "LT"],
          ["≤", "LTE"],
          [">", "GT"],
          ["≥", "GTE"],
        ],
      },
      {
        type: "input_value",
        name: "B",
      },
    ],
    output: "Boolean",
    colour: "#5ba55b",
    tooltip: "Compare deux valeurs",
    helpUrl: "",
  },
  {
    type: "logic_operation",
    message0: "%1 %2 %3",
    args0: [
      {
        type: "input_value",
        name: "A",
        check: "Boolean",
      },
      {
        type: "field_dropdown",
        name: "OP",
        options: [
          ["et", "AND"],
          ["ou", "OR"],
        ],
      },
      {
        type: "input_value",
        name: "B",
        check: "Boolean",
      },
    ],
    output: "Boolean",
    colour: "#5ba55b",
    tooltip: "Opération logique et/ou",
    helpUrl: "",
  },
  {
    type: "controls_selon",
    message0: "selon %1",
    args0: [
      {
        type: "input_value",
        name: "SWITCH",
      },
    ],
    previousStatement: null,
    nextStatement: null,
    colour: "#3498db",
    tooltip: "Structure de sélection selon/cas",
    helpUrl: "",
    mutator: "controls_selon_mutator",
  },
  {
    type: "controls_quitter",
    message0: "quitter",
    previousStatement: null,
    nextStatement: null,
    colour: "#e67e22",
    tooltip: "Sortir d'une boucle ou d'un bloc",
    helpUrl: "",
  },
  {
    type: "controls_continuer",
    message0: "continuer",
    previousStatement: null,
    nextStatement: null,
    colour: "#e67e22",
    tooltip: "Passer à l'itération suivante",
    helpUrl: "",
  },
  // Function definition blocks
  {
    type: "fonction_definition",
    message0: "fonction %1 ( %2 )",
    args0: [
      {
        type: "field_input",
        name: "NAME",
        text: "ma_fonction",
      },
      {
        type: "field_input",
        name: "PARAMS",
        text: "paramètres",
      },
    ],
    message1: "faire %1",
    args1: [
      {
        type: "input_statement",
        name: "STACK",
      },
    ],
    colour: "#8b5cf6",
    tooltip: "Définit une nouvelle fonction",
    helpUrl: "",
  },

  // Function call block
  {
    type: "fonction_call",
    message0: "appeler %1 ( %2 )",
    args0: [
      {
        type: "field_input",
        name: "NAME",
        text: "fonction",
      },
      {
        type: "field_input",
        name: "ARGS",
        text: "arguments",
      },
    ],
    previousStatement: null,
    nextStatement: null,
    colour: "#8b5cf6",
    tooltip: "Appelle une fonction",
    helpUrl: "",
  },

  // Return statement
  {
    type: "retourner",
    message0: "retourner %1",
    args0: [
      {
        type: "input_value",
        name: "VALUE",
      },
    ],
    previousStatement: null,
    colour: "#8b5cf6",
    tooltip: "Retourne une valeur depuis une fonction",
    helpUrl: "",
  },

  // Array/List creation
  {
    type: "liste_create",
    message0: "liste [ %1 ]",
    args0: [
      {
        type: "input_value",
        name: "ITEMS",
      },
    ],
    output: "Array",
    colour: "#06b6d4",
    tooltip: "Crée une nouvelle liste",
    helpUrl: "",
  },

  // Array access
  {
    type: "liste_get",
    message0: "%1 [ %2 ]",
    args0: [
      {
        type: "input_value",
        name: "LIST",
        check: "Array",
      },
      {
        type: "input_value",
        name: "INDEX",
        check: "Number",
      },
    ],
    output: null,
    colour: "#06b6d4",
    tooltip: "Obtient un élément de la liste",
    helpUrl: "",
  },

  // Array set
  {
    type: "liste_set",
    message0: "%1 [ %2 ] = %3",
    args0: [
      {
        type: "input_value",
        name: "LIST",
        check: "Array",
      },
      {
        type: "input_value",
        name: "INDEX",
        check: "Number",
      },
      {
        type: "input_value",
        name: "VALUE",
      },
    ],
    previousStatement: null,
    nextStatement: null,
    colour: "#06b6d4",
    tooltip: "Définit un élément de la liste",
    helpUrl: "",
  },

  // String length
  {
    type: "texte_longueur",
    message0: "longueur de %1",
    args0: [
      {
        type: "input_value",
        name: "TEXT",
        check: "String",
      },
    ],
    output: "Number",
    colour: "#10b981",
    tooltip: "Retourne la longueur du texte",
    helpUrl: "",
  },

  // String substring
  {
    type: "texte_sous_chaine",
    message0: "sous-chaîne de %1 de %2 à %3",
    args0: [
      {
        type: "input_value",
        name: "TEXT",
        check: "String",
      },
      {
        type: "input_value",
        name: "FROM",
        check: "Number",
      },
      {
        type: "input_value",
        name: "TO",
        check: "Number",
      },
    ],
    output: "String",
    colour: "#10b981",
    tooltip: "Extrait une partie du texte",
    helpUrl: "",
  },

  // Math operations
  {
    type: "math_random",
    message0: "nombre aléatoire entre %1 et %2",
    args0: [
      {
        type: "input_value",
        name: "FROM",
        check: "Number",
      },
      {
        type: "input_value",
        name: "TO",
        check: "Number",
      },
    ],
    output: "Number",
    colour: "#f59e0b",
    tooltip: "Génère un nombre aléatoire",
    helpUrl: "",
  },

  // Math round
  {
    type: "math_round",
    message0: "arrondir %1",
    args0: [
      {
        type: "input_value",
        name: "NUM",
        check: "Number",
      },
    ],
    output: "Number",
    colour: "#f59e0b",
    tooltip: "Arrondit un nombre",
    helpUrl: "",
  },

  // Comments
  {
    type: "commentaire",
    message0: "// %1",
    args0: [
      {
        type: "field_input",
        name: "TEXT",
        text: "commentaire",
      },
    ],
    previousStatement: null,
    nextStatement: null,
    colour: "#6b7280",
    tooltip: "Ajoute un commentaire",
    helpUrl: "",
  },

  // Try-catch error handling
  {
    type: "essayer_attraper",
    message0: "essayer",
    message1: "%1",
    args1: [
      {
        type: "input_statement",
        name: "TRY",
      },
    ],
    message2: "attraper erreur %1",
    args2: [
      {
        type: "field_variable",
        name: "ERROR_VAR",
        variable: "erreur",
      },
    ],
    message3: "%1",
    args3: [
      {
        type: "input_statement",
        name: "CATCH",
      },
    ],
    previousStatement: null,
    nextStatement: null,
    colour: "#ef4444",
    tooltip: "Gestion d'erreurs",
    helpUrl: "",
  },

  // Throw error
  {
    type: "lancer_erreur",
    message0: "lancer erreur %1",
    args0: [
      {
        type: "input_value",
        name: "MESSAGE",
        check: "String",
      },
    ],
    previousStatement: null,
    colour: "#ef4444",
    tooltip: "Lance une erreur",
    helpUrl: "",
  },
])

console.log("✅ Custom blocks defined")
