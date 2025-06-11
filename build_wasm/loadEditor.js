/**
 * load_monaco.js
 *
 * This file loads and configures the Monaco editor for the zia interpreter.
 */

// Global variables
let editor = null;
let outputPane = null;
let currentFileName = "main.zia";
let fileTabs = {};

// Sample files
const sampleFiles = {
  "main.zia": `// Programme principal

    var message = "Bonjour le monde!";
    afficher message;

    fon addition(a, b) {
        retourner a + b;
    }

    var resultat = addition(5, 3);
    afficher "5 + 3 = ", resultat;`,

    "closure.zia": `// Les closures fonctionnent comme en JavaScript

    var x = "global";
    afficher "Example avec les closures";
    fon extern() {
        var x = "extern";
        fon intern() {
            afficher x;
        }
        intern();
    }

    extern();    // affiche "extern"

    // Exemple avec compteurs
    afficher "Example avec compteurs, en utilisant une fonction";
    fon createCounter(debut) {
        var compte = debut;
        fon increment() {
            compte = compte + 1;
            afficher compte;
        }
        retourner increment;
    }

    var a = createCounter(100);
    var b = createCounter(200);

    a();    // 101
    a();    // 102
    b();    // 201;`,

    "controlflow.zia": `// Structures de contrôle

    afficher "Example avec condition si";
    var temp = 22;

    si (temp > 30) {
        afficher "Il fait très chaud!";
    } sinon si (temp > 20) {
        afficher "Il fait beau!";
    } sinon {
        afficher "Il fait frais!";
    }

    // Boucle tantque
    afficher "Example avec boucle tantque";
    var i = 0;
    tantque (i < 5) {
        afficher "Compteur: ", i;
        i += 1;
    }

    // Boucle pour
    afficher "Example avec boucle pour";
    pour (var j = 0; j < 5; j++) {
        afficher "Pour compteur: ", j;
    }`,
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
    });

    // Set the editor theme
    monaco.editor.defineTheme("ziaTheme", {
      base: "vs-dark",
      inherit: true,
      rules: [
        { token: "keyword", foreground: "569CD6", fontStyle: "bold" },
        { token: "identifier", foreground: "9CDCFE" },
        { token: "string", foreground: "CE9178" },
        { token: "number", foreground: "B5CEA8" },
        { token: "comment", foreground: "6A9955" },
        { token: "operator", foreground: "D4D4D4" },
        { token: "delimiter", foreground: "D4D4D4" },
      ],
      colors: {
        "editor.background": "#1E1E1E",
        "editor.foreground": "#D4D4D4",
        "editorCursor.foreground": "#AEAFAD",
        "editor.lineHighlightBackground": "#2D2D30",
        "editorLineNumber.foreground": "#858585",
      },
    });

    // Define the token provider
    monaco.languages.setMonarchTokensProvider("zia", {
      defaultToken: "invalid",

      // French keywords for zia language
      keywords: [
        "var",
        "fonction",
        "si",
        "sinon",
        "pour",
        "tantque",
        "retourne",
        "affiche",
        "vrai",
        "faux",
        "nul",
      ],

      operators: [
        "=",
        "+",
        "-",
        "*",
        "/",
        "==",
        "!=",
        ">",
        "<",
        ">=",
        "<=",
        "!",
      ],

      symbols: /[=><!~?:&|+\-*\/\^%]+/,

      escapes:
        /\\(?:[abfnrtv\\"']|x[0-9A-Fa-f]{1,4}|u[0-9A-Fa-f]{4}|U[0-9A-Fa-f]{8})/,

      tokenizer: {
        root: [
          // identifiers and keywords
          [
            /[a-zA-Z_$][\w$]*/,
            {
              cases: {
                "@keywords": "keyword",
                "@default": "identifier",
              },
            },
          ],

          // whitespace
          { include: "@whitespace" },

          // numbers
          [/\d*\.\d+([eE][\-+]?\d+)?/, "number.float"],
          [/0[xX][0-9a-fA-F]+/, "number.hex"],
          [/\d+/, "number"],

          // delimiters and operators
          [/[{}()\[\]]/, "@brackets"],
          [/[<>](?!@symbols)/, "@brackets"],
          [
            /@symbols/,
            {
              cases: {
                "@operators": "operator",
                "@default": "",
              },
            },
          ],

          // delimiter: after number because of .\d floats
          [/[;,.]/, "delimiter"],

          // strings
          [/"([^"\\]|\\.)*$/, "string.invalid"], // non-terminated string
          [/'([^'\\]|\\.)*$/, "string.invalid"], // non-terminated string
          [/"/, "string", "@string_double"],
          [/'/, "string", "@string_single"],
        ],

        whitespace: [
          [/[ \t\r\n]+/, "white"],
          [/\/\/.*$/, "comment"],
          [/\/\*/, "comment", "@comment"],
        ],

        comment: [
          [/[^\/*]+/, "comment"],
          [/\/\*/, "comment", "@push"], // nested comment
          [/\*\//, "comment", "@pop"],
          [/[\/*]/, "comment"],
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
      },
    });

    // Create the editor
    editor = monaco.editor.create(document.getElementById(containerId), {
      value: initialContent || sampleFiles["main.zia"],
      language: "zia",
      theme: "ziaTheme",
      automaticLayout: true,
      minimap: { enabled: true },
      scrollBeyondLastLine: false,
      renderLineHighlight: "all",
      fontFamily: 'Consolas, "Courier New", monospace',
      fontSize: 14,
      lineNumbers: "on",
      roundedSelection: false,
      wordWrap: "on",
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
    "Enter file name (must end with .zia):",
    "new_file.zia"
  );
  if (!fileName) return;

  // Validate filename
  if (!fileName.endsWith(".zia")) {
    alert("File name must end with .zia");
    return;
  }

  // Check for duplicates
  if (fileTabs[fileName]) {
    alert("A file with this name already exists");
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
        formattedOutput += `Line ${token.line}: [${token.type}] "${token.lexeme}"\n`;
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
