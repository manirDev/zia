/**
 * loadEditor.js
 *
 * This file loads and configures the Monaco editor for the zia interpreter.
 * With improved French syntax support.
 */

// Global variables
let editor = null;
let outputPane = null;
let currentFileName = "main.zia";
let fileTabs = {};

// Enhanced sample files with official Zia syntax
const sampleFiles = {
  "main.zia": `// Programme principal Zia

var message = "Bonjour le monde!";
afficher message;
afficher "\\n";

fonction addition(a, b) {
    retourner a + b;
}

var resultat = addition(5, 3);
afficher "5 + 3 = ", resultat;
afficher "\\n";

// Exemple avec conditions
si (resultat > 7) {
    afficher "Le résultat est grand!";
    afficher "\\n";
} sinon {
    afficher "Le résultat est petit.";
    afficher "\\n";
}`,

    "controlflow.zia": `// Structures de contrôle en Zia

afficher "=== Conditions ===";
afficher "\\n";
var temperature = 25;

si (temperature > 30) {
    afficher "Il fait très chaud!";
    afficher "\\n";
} sinon si (temperature > 20) {
    afficher "Il fait beau!";
    afficher "\\n";
} sinon {
    afficher "Il fait frais!";
    afficher "\\n";
}

// Boucle tantque
afficher "=== Boucle tantque ===";
afficher "\\n";
var compteur = 0;
tantque (compteur < 3) {
    afficher "Compteur: ", compteur;
    afficher "\\n";
    compteur = compteur + 1;
}

// Boucle pour
afficher "=== Boucle pour ===";
afficher "\\n";
pour (var i = 0; i < 3; i = i + 1) {
    afficher "Pour compteur: ", i;
    afficher "\\n";
}

// Structure selon
afficher "=== Structure selon ===";
afficher "\\n";
var jour = 2;
selon (jour) {
    cas 1:
        afficher "Lundi";
        afficher "\\n";
        quitter;
    cas 2:
        afficher "Mardi";
        afficher "\\n";
        quitter;
    cas 3:
        afficher "Mercredi";
        afficher "\\n";
        quitter;
    defaut:
        afficher "Autre jour";
        afficher "\\n";
}`,

    "fonctions.zia": `// Fonctions avancées en Zia

// Fonction récursive
fonction factorielle(n) {
    si (n <= 1) {
        retourner 1;
    } sinon {
        retourner n * factorielle(n - 1);
    }
}

afficher "Factorielle de 5: ", factorielle(5);
afficher "\\n";

// Fonction avec logique complexe
fonction calculer(operation, a, b) {
    si (operation == "addition") {
        retourner a + b;
    } sinon si (operation == "soustraction") {
        retourner a - b;
    } sinon si (operation == "multiplication") {
        retourner a * b;
    } sinon si (operation == "division") {
        si (b != 0) {
            retourner a / b;
        } sinon {
            afficher "Erreur: division par zéro!";
            afficher "\\n";
            retourner nul;
        }
    } sinon {
        afficher "Opération inconnue!";
        afficher "\\n";
        retourner nul;
    }
}

var resultat = calculer("addition", 10, 5);
afficher "10 + 5 = ", resultat;
afficher "\\n";

// Utilisation des opérateurs logiques Zia
fonction verifierAge(age) {
    si (age >= 18 et age <= 65) {
        afficher "Adulte en âge de travailler";
        afficher "\\n";
    } sinon si (age < 18 ou age > 65) {
        afficher "Hors tranche d'âge de travail";
        afficher "\\n";
    }
}

verifierAge(25);`,

    "variables.zia": `// Variables et types en Zia

// Variables numériques
var entier = 42;
var decimal = 3.14159;
var negatif = -10;

// Variables chaînes de caractères
var nom = "Alice";
var prenom = "Bob";
var message = "Bonjour " + nom + "!";

// Variables booléennes et valeur nulle
var estVrai = vrai;
var estFaux = faux;
var valeurNulle = nul;

// Affichage des variables
afficher "=== Variables numériques ===";
afficher "\\n";
afficher "Entier: ", entier;
afficher "\\n";
afficher "Décimal: ", decimal;
afficher "\\n";
afficher "Négatif: ", negatif;
afficher "\\n";

afficher "=== Variables texte ===";
afficher "\\n";
afficher "Nom: ", nom;
afficher "\\n";
afficher "Prénom: ", prenom;
afficher "\\n";
afficher "Message: ", message;
afficher "\\n";

afficher "=== Variables logiques ===";
afficher "\\n";
afficher "Vrai: ", estVrai;
afficher "\\n";
afficher "Faux: ", estFaux;
afficher "\\n";
afficher "Nul: ", valeurNulle;
afficher "\\n";

// Opérations arithmétiques
afficher "=== Opérations ===";
afficher "\\n";
var somme = entier + decimal;
var produit = entier * 2;
var division = decimal / 2;

afficher "Somme: ", somme;
afficher "\\n";
afficher "Produit: ", produit;
afficher "\\n";
afficher "Division: ", division;
afficher "\\n";`
};

/**
 * Initialize the Monaco editor
 * @param {string} containerId - ID of the container element
 * @param {string} initialContent - Initial editor content
 */
function initMonacoEditor(containerId, initialContent = "") {
  require.config({
    paths: { vs: "../build_wasm/node_modules/monaco-editor/min/vs" },
  });

  require(["vs/editor/editor.main"], function () {
    // Register the zia language
    monaco.languages.register({ id: "zia" });

    // Define the language configuration
    monaco.languages.setLanguageConfiguration("zia", {
      comments: {
        lineComment: "//",
        blockComment: ["/*", "*/"],
      },
      brackets: [
        ["{", "}"],
        ["[", "]"],
        ["(", ")"],
      ],
      autoClosingPairs: [
        { open: "{", close: "}" },
        { open: "[", close: "]" },
        { open: "(", close: ")" },
        { open: '"', close: '"' },
        { open: "'", close: "'" },
      ],
      surroundingPairs: [
        { open: "{", close: "}" },
        { open: "[", close: "]" },
        { open: "(", close: ")" },
        { open: '"', close: '"' },
        { open: "'", close: "'" },
      ],
      folding: {
        markers: {
          start: /^\s*\/\/\s*#?region\b/,
          end: /^\s*\/\/\s*#?endregion\b/,
        },
      },
      wordPattern: /(-?\d*\.\d\w*)|([^\`\~\!\@\#\%\^\&\*\(\)\-\=\+\[\{\]\}\\\|\;\:\'\"\,\.\<\>\/\?\s]+)/g,
      indentationRules: {
        increaseIndentPattern: /^.*\{[^}]*$/,
        decreaseIndentPattern: /^.*\}.*$/,
      },
    });

    // Enhanced theme with better French syntax highlighting
    monaco.editor.defineTheme("ziaTheme", {
      base: "vs-dark",
      inherit: true,
      rules: [
        { token: "keyword.control", foreground: "C586C0", fontStyle: "bold" },
        { token: "keyword.function", foreground: "DCDCAA", fontStyle: "bold" },
        { token: "keyword.variable", foreground: "4FC1FF", fontStyle: "bold" },
        { token: "keyword.literal", foreground: "569CD6", fontStyle: "bold" },
        { token: "keyword.class", foreground: "4EC9B0", fontStyle: "bold" },
        { token: "keyword.logical", foreground: "569CD6", fontStyle: "bold" },
        { token: "identifier", foreground: "9CDCFE" },
        { token: "identifier.function", foreground: "DCDCAA" },
        { token: "string", foreground: "CE9178" },
        { token: "string.escape", foreground: "D7BA7D" },
        { token: "number", foreground: "B5CEA8" },
        { token: "number.float", foreground: "B5CEA8" },
        { token: "number.hex", foreground: "B5CEA8" },
        { token: "comment", foreground: "6A9955", fontStyle: "italic" },
        { token: "comment.block", foreground: "6A9955", fontStyle: "italic" },
        { token: "operator", foreground: "D4D4D4" },
        { token: "operator.logical", foreground: "569CD6" },
        { token: "operator.comparison", foreground: "569CD6" },
        { token: "delimiter", foreground: "D4D4D4" },
        { token: "delimiter.parenthesis", foreground: "FFD700" },
        { token: "delimiter.bracket", foreground: "FFD700" },
        { token: "delimiter.curly", foreground: "FFD700" },
      ],
      colors: {
        "editor.background": "#1E1E1E",
        "editor.foreground": "#D4D4D4",
        "editorCursor.foreground": "#AEAFAD",
        "editor.lineHighlightBackground": "#2D2D30",
        "editorLineNumber.foreground": "#858585",
        "editor.selectionBackground": "#264F78",
        "editor.inactiveSelectionBackground": "#3A3D41",
        "editorIndentGuide.background": "#404040",
        "editorIndentGuide.activeBackground": "#707070",
        "editor.wordHighlightBackground": "#575757B8",
        "editor.wordHighlightStrongBackground": "#004972B8",
      },
    });

    // Enhanced token provider with better French keyword support
    monaco.languages.setMonarchTokensProvider("zia", {
      defaultToken: "invalid",

      // Official Zia language keywords
      keywords: {
        control: [
          "si", "sinon si", "sinon", "pour", "tantque", "selon", "cas", "defaut", "quitter", "continuer"
        ],
        function: [
          "fonction", "retourner"
        ],
        variable: [
          "var"
        ],
        literal: [
          "vrai", "faux", "nul"
        ],
        builtin: [
          "afficher"
        ],
        class: [
          "classe", "ceci", "super"
        ],
        logical: [
          "et", "ou"
        ]
      },

      // Zia operators
      operators: [
        "=", "+=", "-=", "*=", "/=", "%=",
        "+", "-", "*", "/", "%", "**",
        "==", "!=", ">", "<", ">=", "<=",
        "&&", "||", "!",
        "&", "|", "^", "~", "<<", ">>",
        "?", ":", "=>", "->", "..", "..."
      ],

      // Enhanced symbols
      symbols: /[=><!~?:&|+\-*\/\^%]+/,

      // Enhanced escape sequences
      escapes: /\\(?:[abfnrtv\\"']|x[0-9A-Fa-f]{1,4}|u[0-9A-Fa-f]{4}|U[0-9A-Fa-f]{8})/,

      // Enhanced tokenizer
      tokenizer: {
        root: [
          // Function definitions
          [/\bfonction\b/, "keyword.function"],
          [/\bafficher\b/, "keyword.builtin"],
          
          // Control flow keywords
          [/\b(si|sinon\s+si|sinon|pour|tantque|selon|cas|defaut|quitter|continuer)\b/, "keyword.control"],
          
          // Variable keywords
          [/\bvar\b/, "keyword.variable"],
          
          // Class keywords
          [/\b(classe|ceci|super)\b/, "keyword.class"],
          
          // Literals
          [/\b(vrai|faux|nul)\b/, "keyword.literal"],
          
          // Return keyword
          [/\bretourner\b/, "keyword.function"],
          
          // Logical operators (French)
          [/\b(et|ou)\b/, "keyword.logical"],

          // Identifiers and function names
          [/[a-zA-Z_$][\w$]*(?=\s*\()/, "identifier.function"],
          [/[a-zA-Z_$][\w$]*/, "identifier"],

          // Whitespace
          { include: "@whitespace" },

          // Numbers
          [/\d*\.\d+([eE][\-+]?\d+)?[fFdD]?/, "number.float"],
          [/0[xX][0-9a-fA-F]+/, "number.hex"],
          [/0[bB][01]+/, "number.binary"],
          [/0[oO][0-7]+/, "number.octal"],
          [/\d+[lL]?/, "number"],

          // Delimiters and operators
          [/[{}]/, "delimiter.curly"],
          [/[[\]]/, "delimiter.bracket"],
          [/[()]/, "delimiter.parenthesis"],
          [/[<>](?!@symbols)/, "operator.comparison"],
          [
            /@symbols/,
            {
              cases: {
                "@operators": {
                  cases: {
                    "(==|!=|===|!==|>=|<=|>|<)": "operator.comparison",
                    "(&&|\\|\\||!)": "operator.logical",
                    "@default": "operator"
                  }
                },
                "@default": "delimiter"
              },
            },
          ],

          // Punctuation
          [/[;,.]/, "delimiter"],

          // Strings
          [/"([^"\\]|\\.)*$/, "string.invalid"],
          [/'([^'\\]|\\.)*$/, "string.invalid"],
          [/"/, "string", "@string_double"],
          [/'/, "string", "@string_single"],
          [/`/, "string", "@string_backtick"],
        ],

        whitespace: [
          [/[ \t\r\n]+/, "white"],
          [/\/\/.*$/, "comment"],
          [/\/\*/, "comment.block", "@comment"],
        ],

        comment: [
          [/[^\/*]+/, "comment.block"],
          [/\/\*/, "comment.block", "@push"],
          [/\*\//, "comment.block", "@pop"],
          [/[\/*]/, "comment.block"],
        ],

        string_double: [
          [/[^\\"]+/, "string"],
          [/@escapes/, "string.escape"],
          [/\\./, "string.escape.invalid"],
          [/"/, "string", "@pop"],
        ],

        string_single: [
          [/[^\\']+/, "string"],
          [/@escapes/, "string.escape"],
          [/\\./, "string.escape.invalid"],
          [/'/, "string", "@pop"],
        ],

        string_backtick: [
          [/[^\\`$]+/, "string"],
          [/@escapes/, "string.escape"],
          [/\\./, "string.escape.invalid"],
          [/`/, "string", "@pop"],
        ],
      },
    });

    // Create the editor with enhanced configuration
    editor = monaco.editor.create(document.getElementById(containerId), {
      value: initialContent || sampleFiles["main.zia"],
      language: "zia",
      theme: "ziaTheme",
      automaticLayout: true,
      minimap: { enabled: true },
      scrollBeyondLastLine: false,
      renderLineHighlight: "all",
      fontFamily: 'Consolas, "Cascadia Code", "Fira Code", "JetBrains Mono", Monaco, "Courier New", monospace',
      fontSize: 14,
      lineHeight: 20,
      letterSpacing: 0.5,
      lineNumbers: "on",
      roundedSelection: false,
      wordWrap: "on",
      cursorBlinking: "blink",
      cursorSmoothCaretAnimation: "on",
      renderWhitespace: "selection",
      showFoldingControls: "always",
      foldingStrategy: "indentation",
      matchBrackets: "always",
      autoIndent: "full",
      formatOnType: true,
      formatOnPaste: true,
      suggestOnTriggerCharacters: true,
      acceptSuggestionOnEnter: "on",
      tabCompletion: "on",
      wordBasedSuggestions: true,
      parameterHints: {
        enabled: true,
        cycle: true
      },
      quickSuggestions: {
        other: true,
        comments: false,
        strings: false
      },
    });

    // Add enhanced completion provider
    monaco.languages.registerCompletionItemProvider("zia", {
      provideCompletionItems: function (model, position) {
        const word = model.getWordUntilPosition(position);
        const range = {
          startLineNumber: position.lineNumber,
          endLineNumber: position.lineNumber,
          startColumn: word.startColumn,
          endColumn: word.endColumn,
        };

        const suggestions = [
          // Control flow structures
          {
            label: "si",
            kind: monaco.languages.CompletionItemKind.Keyword,
            insertText: "si (${1:condition}) {\n\t${2:// code}\n}",
            insertTextRules: monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet,
            documentation: "Structure conditionnelle si",
            range: range,
          },
          {
            label: "sinon si",
            kind: monaco.languages.CompletionItemKind.Keyword,
            insertText: "sinon si (${1:condition}) {\n\t${2:// code}\n}",
            insertTextRules: monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet,
            documentation: "Structure conditionnelle sinon si",
            range: range,
          },
          {
            label: "sinon",
            kind: monaco.languages.CompletionItemKind.Keyword,
            insertText: "sinon {\n\t${1:// code}\n}",
            insertTextRules: monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet,
            documentation: "Structure conditionnelle sinon",
            range: range,
          },
          {
            label: "pour",
            kind: monaco.languages.CompletionItemKind.Keyword,
            insertText: "pour (var ${1:i} = 0; ${1:i} < ${2:length}; ${1:i} = ${1:i} + 1) {\n\t${3:// code}\n}",
            insertTextRules: monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet,
            documentation: "Boucle pour - itération déterminée",
            range: range,
          },
          {
            label: "tantque",
            kind: monaco.languages.CompletionItemKind.Keyword,
            insertText: "tantque (${1:condition}) {\n\t${2:// code}\n}",
            insertTextRules: monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet,
            documentation: "Boucle tantque - répétition conditionnelle",
            range: range,
          },
          {
            label: "selon",
            kind: monaco.languages.CompletionItemKind.Keyword,
            insertText: "selon (${1:variable}) {\n\tcas ${2:valeur1}:\n\t\t${3:// code}\n\t\tquitter;\n\tcas ${4:valeur2}:\n\t\t${5:// code}\n\t\tquitter;\n\tdefaut:\n\t\t${6:// code par défaut}\n}",
            insertTextRules: monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet,
            documentation: "Structure selon - sélection parmi plusieurs cas",
            range: range,
          },
          // Function and variable declarations
          {
            label: "fonction",
            kind: monaco.languages.CompletionItemKind.Keyword,
            insertText: "fonction ${1:nom}(${2:paramètres}) {\n\t${3:// code}\n\tretourner ${4:valeur};\n}",
            insertTextRules: monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet,
            documentation: "Déclaration de fonction - créer un bloc d'actions réutilisable",
            range: range,
          },
          {
            label: "var",
            kind: monaco.languages.CompletionItemKind.Keyword,
            insertText: "var ${1:nom} = ${2:valeur};",
            insertTextRules: monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet,
            documentation: "Déclaration de variable - déclarer l'existence d'une donnée",
            range: range,
          },
          // Class-related
          {
            label: "classe",
            kind: monaco.languages.CompletionItemKind.Class,
            insertText: "classe ${1:NomClasse} {\n\tfonction constructeur(${2:paramètres}) {\n\t\t${3:// initialisation}\n\t}\n\t\n\tfonction ${4:methode}() {\n\t\t${5:// code de la méthode}\n\t}\n}",
            insertTextRules: monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet,
            documentation: "Déclaration de classe - modèle d'objet",
            range: range,
          },
          // Built-in functions and keywords
          {
            label: "afficher",
            kind: monaco.languages.CompletionItemKind.Function,
            insertText: "afficher ${1:message};",
            insertTextRules: monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet,
            documentation: "Afficher - révéler, présenter au monde",
            range: range,
          },
          {
            label: "retourner",
            kind: monaco.languages.CompletionItemKind.Keyword,
            insertText: "retourner ${1:valeur};",
            insertTextRules: monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet,
            documentation: "Retourner - renvoyer un résultat vers l'appelant",
            range: range,
          },
          // Flow control
          {
            label: "quitter",
            kind: monaco.languages.CompletionItemKind.Keyword,
            insertText: "quitter;",
            documentation: "Quitter - sortir d'une boucle ou d'un bloc",
            range: range,
          },
          {
            label: "continuer",
            kind: monaco.languages.CompletionItemKind.Keyword,
            insertText: "continuer;",
            documentation: "Continuer - passer à l'itération suivante",
            range: range,
          },
          // Literals
          {
            label: "vrai",
            kind: monaco.languages.CompletionItemKind.Constant,
            insertText: "vrai",
            documentation: "Valeur de vérité positive",
            range: range,
          },
          {
            label: "faux",
            kind: monaco.languages.CompletionItemKind.Constant,
            insertText: "faux",
            documentation: "Valeur de vérité négative",
            range: range,
          },
          {
            label: "nul",
            kind: monaco.languages.CompletionItemKind.Constant,
            insertText: "nul",
            documentation: "Absence de valeur",
            range: range,
          },
          // Logical operators
          {
            label: "et",
            kind: monaco.languages.CompletionItemKind.Operator,
            insertText: "et",
            documentation: "Conjonction logique inclusive",
            range: range,
          },
          {
            label: "ou",
            kind: monaco.languages.CompletionItemKind.Operator,
            insertText: "ou",
            documentation: "Disjonction logique",
            range: range,
          },
          // Class instance references
          {
            label: "ceci",
            kind: monaco.languages.CompletionItemKind.Reference,
            insertText: "ceci",
            documentation: "Référence à l'instance courante",
            range: range,
          },
          {
            label: "super",
            kind: monaco.languages.CompletionItemKind.Reference,
            insertText: "super",
            documentation: "Référence à la classe parente",
            range: range,
          },
        ];

        return { suggestions: suggestions };
      },
    });

    // Set up file tabs
    setupFileTabs();

    // Return the editor instance
    return editor;
  });
}

/**
 * Set up file tabs in the UI
 */
function setupFileTabs() {
  const tabsContainer = document.getElementById("file-tabs");
  if (!tabsContainer) return;

  // Clear existing tabs
  tabsContainer.innerHTML = "";

  // Add tabs for sample files
  Object.keys(sampleFiles).forEach((fileName) => {
    const tab = document.createElement("button");
    tab.className =
      "file-tab" + (fileName === currentFileName ? " active" : "");
    tab.textContent = fileName;
    tab.onclick = () => switchToFile(fileName);
    tabsContainer.appendChild(tab);

    // Store the file content
    fileTabs[fileName] = sampleFiles[fileName];
  });

  // Add "new file" button
  const newTab = document.createElement("button");
  newTab.className = "file-tab new-tab";
  newTab.textContent = "+";
  newTab.onclick = createNewFile;
  tabsContainer.appendChild(newTab);
}

/**
 * Switch to a different file
 * @param {string} fileName - Name of the file to switch to
 */
function switchToFile(fileName) {
  if (!fileTabs[fileName]) return;

  // Save current content
  if (editor && currentFileName) {
    fileTabs[currentFileName] = editor.getValue();
  }

  // Update current file
  currentFileName = fileName;

  // Update editor content
  if (editor) {
    editor.setValue(fileTabs[fileName]);
  }

  // Update tab UI
  const tabs = document.querySelectorAll(".file-tab");
  tabs.forEach((tab) => {
    if (tab.textContent === fileName) {
      tab.classList.add("active");
    } else {
      tab.classList.remove("active");
    }
  });
}

/**
 * Create a new file
 */
function createNewFile() {
  const fileName = prompt(
    "Entrez le nom du fichier (doit se terminer par .zia):",
    "nouveau_fichier.zia"
  );
  if (!fileName) return;

  // Validate filename
  if (!fileName.endsWith(".zia")) {
    alert("Le nom du fichier doit se terminer par .zia");
    return;
  }

  // Check for duplicates
  if (fileTabs[fileName]) {
    alert("Un fichier avec ce nom existe déjà");
    return;
  }

  // Create new file
  fileTabs[fileName] = "// " + fileName + "\n\n";

  // Add tab
  const tabsContainer = document.getElementById("file-tabs");
  const tab = document.createElement("button");
  tab.className = "file-tab";
  tab.textContent = fileName;
  tab.onclick = () => switchToFile(fileName);
  tabsContainer.insertBefore(tab, document.querySelector(".new-tab"));

  // Switch to the new file
  switchToFile(fileName);
}

/**
 * Get the current editor content
 * @returns {string} - Current editor content
 */
function getEditorContent() {
  if (!editor) return "";
  return editor.getValue();
}

/**
 * Set the output pane content
 * @param {string} content - Content to display in the output pane
 */
function setOutputContent(content) {
  if (!outputPane) {
    outputPane = document.getElementById("output");
  }

  if (outputPane) {
    outputPane.textContent = content;
  }
}

/**
 * Format the output pane content as JSON
 * @param {string} jsonString - JSON string to format
 */
function formatOutput(jsonString) {
  try {
    const tokens = JSON.parse(jsonString);
    let formattedOutput = "";

    if (Array.isArray(tokens)) {
      tokens.forEach((token) => {
        formattedOutput += `Ligne ${token.line}: [${token.type}] "${token.lexeme}"\n`;
      });
    } else {
      formattedOutput = JSON.stringify(tokens, null, 2);
    }

    setOutputContent(formattedOutput);
  } catch (e) {
    // If not valid JSON, display as is
    setOutputContent(jsonString);
  }
}

/**
 * Toggle dark mode
 * @param {boolean} isDark - Whether to enable dark mode
 */
function toggleDarkMode(isDark) {
  if (!editor) return;

  const theme = isDark ? "ziaTheme" : "vs";
  monaco.editor.setTheme(theme);

  document.body.classList.toggle("light-theme", !isDark);
}

// Export functions to global scope
window.MonacoEditorLoader = {
  initEditor: initMonacoEditor,
  getContent: getEditorContent,
  setOutput: setOutputContent,
  formatOutput: formatOutput,
  toggleDarkMode: toggleDarkMode,
};