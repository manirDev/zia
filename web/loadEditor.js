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
  "main.zia":
    '// Programme principal\n\nvar message = "Bonjour le monde!";\naffiche message;\n\nfonction addition(a, b) {\n    retourne a + b;\n}\n\nvar resultat = addition(5, 3);\naffiche "5 + 3 = " + resultat;',
  "closure.zia":
    '// Closures sont fonctionnelles comme en JavaScript\n\nvar x = "global";\n\nfonction outer() {\n    var x = "outer";\n    fonction inner() {\n        affiche x;\n    }\n    inner();\n}\n\nouter();    // affiche "outer"\n\n// Exemple avec des compteurs\nfonction createCounter(debut) {\n    var count = debut;\n    fonction increment() {\n        count = count + 1;\n        affiche count;\n    }\n    retourne increment;\n}\n\nvar a = createCounter(100);\nvar b = createCounter(200);\n\na();    // 101\na();    // 102\nb();    // 201',
  "controlflow.zia":
    '// Structures de contrôle\n\nvar temp = 22;\n\nsi (temp > 30) {\n    affiche "Il fait très chaud!";\n} sinon si (temp > 20) {\n    affiche "Il fait beau!";\n} sinon {\n    affiche "Il fait frais!";\n}\n\n// Boucle\nvar i = 0;\ntantque (i < 5) {\n    affiche "Compteur: " + i;\n    i = i + 1;\n}',
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
