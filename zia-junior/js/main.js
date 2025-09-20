/**
 * Zia Junior - Main Application Logic
 * Handles workspace initialization and user interactions
 */

// Global variables
let workspace
let currentTheme = "dark"

// Function to load examples
function loadExample(exampleName) {
  // Placeholder for loading example logic
  console.log(`Loading example: ${exampleName}`)
}

// Initialize Blockly workspace
function initBlockly() {
  // Create custom dark theme for Blockly
  const darkTheme = Blockly.Theme.defineTheme("zia_dark", {
    base: Blockly.Themes.Classic,
    componentStyles: {
      workspaceBackgroundColour: "#0f172a",
      toolboxBackgroundColour: "#1e293b",
      toolboxForegroundColour: "#f8fafc",
      flyoutBackgroundColour: "#334155",
      flyoutForegroundColour: "#f8fafc",
      flyoutOpacity: 0.95,
      scrollbarColour: "#475569",
      insertionMarkerColour: "#6366f1",
      insertionMarkerOpacity: 0.3,
      markerColour: "#6366f1",
      cursorColour: "#818cf8",
      selectedGlowColour: "#6366f1",
      selectedGlowOpacity: 0.3,
      replacementGlowColour: "#f59e0b",
      replacementGlowOpacity: 0.3,
    },
    blockStyles: {
      colour_blocks: {
        colourPrimary: "#a855f7",
        colourSecondary: "#9333ea",
        colourTertiary: "#7c3aed",
      },
      list_blocks: {
        colourPrimary: "#06b6d4",
        colourSecondary: "#0891b2",
        colourTertiary: "#0e7490",
      },
      logic_blocks: {
        colourPrimary: "#10b981",
        colourSecondary: "#059669",
        colourTertiary: "#047857",
      },
      loop_blocks: {
        colourPrimary: "#3b82f6",
        colourSecondary: "#2563eb",
        colourTertiary: "#1d4ed8",
      },
      math_blocks: {
        colourPrimary: "#f59e0b",
        colourSecondary: "#d97706",
        colourTertiary: "#b45309",
      },
      procedure_blocks: {
        colourPrimary: "#8b5cf6",
        colourSecondary: "#7c3aed",
        colourTertiary: "#6d28d9",
      },
      text_blocks: {
        colourPrimary: "#10b981",
        colourSecondary: "#059669",
        colourTertiary: "#047857",
      },
      variable_blocks: {
        colourPrimary: "#a855f7",
        colourSecondary: "#9333ea",
        colourTertiary: "#7c3aed",
      },
      variable_dynamic_blocks: {
        colourPrimary: "#a855f7",
        colourSecondary: "#9333ea",
        colourTertiary: "#7c3aed",
      },
      hat_blocks: {
        colourPrimary: "#ef4444",
        colourSecondary: "#dc2626",
        colourTertiary: "#b91c1c",
        hat: "cap",
      },
    },
    categoryStyles: {
      colour_category: {
        colour: "#a855f7",
      },
      list_category: {
        colour: "#06b6d4",
      },
      logic_category: {
        colour: "#10b981",
      },
      loop_category: {
        colour: "#3b82f6",
      },
      math_category: {
        colour: "#f59e0b",
      },
      procedure_category: {
        colour: "#8b5cf6",
      },
      text_category: {
        colour: "#10b981",
      },
      variable_category: {
        colour: "#a855f7",
      },
      variable_dynamic_category: {
        colour: "#a855f7",
      },
    },
    fontStyle: {
      family: 'Inter, -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif',
      weight: "500",
      size: 12,
    },
    startHats: true,
  })

  // Create custom light theme for Blockly
  const lightTheme = Blockly.Theme.defineTheme("zia_light", {
    base: Blockly.Themes.Classic,
    componentStyles: {
      workspaceBackgroundColour: "#ffffff",
      toolboxBackgroundColour: "#f8fafc",
      toolboxForegroundColour: "#0f172a",
      flyoutBackgroundColour: "#f1f5f9",
      flyoutForegroundColour: "#0f172a",
      flyoutOpacity: 0.95,
      scrollbarColour: "#cbd5e1",
      insertionMarkerColour: "#6366f1",
      insertionMarkerOpacity: 0.3,
      markerColour: "#6366f1",
      cursorColour: "#818cf8",
      selectedGlowColour: "#6366f1",
      selectedGlowOpacity: 0.3,
      replacementGlowColour: "#f59e0b",
      replacementGlowOpacity: 0.3,
    },
    fontStyle: {
      family: 'Inter, -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif',
      weight: "500",
      size: 12,
    },
  })

  const selectedTheme = currentTheme === "dark" ? darkTheme : lightTheme

  workspace = Blockly.inject("blocklyDiv", {
    toolbox: document.getElementById("toolbox"),
    theme: selectedTheme,
    grid: {
      spacing: 25,
      length: 3,
      colour: currentTheme === "dark" ? "#334155" : "#e2e8f0",
      snap: true,
    },
    zoom: {
      controls: true,
      wheel: true,
      startScale: 1.0,
      maxScale: 3,
      minScale: 0.3,
      scaleSpeed: 1.2,
    },
    trashcan: true,
    scrollbars: true,
    sounds: false,
    renderer: "zelos",
    move: {
      scrollbars: {
        horizontal: true,
        vertical: true,
      },
      drag: true,
      wheel: true,
    },
  })

  // Make workspace globally available
  window.workspace = workspace

  // Update block count on workspace changes
  workspace.addChangeListener(updateBlockCount)

  console.log("✅ Blockly workspace initialized with theme:", currentTheme)
}

// Update block count in status bar
function updateBlockCount() {
  const blockCount = workspace.getAllBlocks().length
  document.getElementById("blockCount").textContent = `${blockCount} blocs`
}

// Generate and execute Zia code
function executeCode() {
  try {
    console.log("🔄 Generating Zia code...")

    // Make sure the generator is properly initialized
    Blockly.Zia.init(workspace)

    const code = Blockly.Zia.workspaceToCode(workspace)

    if (!code.trim()) {
      updateOutput("⚠️ Aucun programme à exécuter. Ajoutez des blocs!", "warning")
      return
    }

    console.log("=== 🎯 CODE ZIA GÉNÉRÉ ===")
    console.log(code)
    console.log("=== ✅ FIN DU CODE ===")

    // Show code in preview if enabled
    if (document.getElementById("showCode").checked) {
      showCodePreview(code)
    }

    // Execute the code using the interpreter
    if (window.ziaInterpreter) {
      executeWithInterpreter(code)
    } else {
      updateOutput(`✅ Code généré avec succès!\n\n${code}`, "success")
    }
  } catch (error) {
    console.error("❌ Erreur lors de la génération du code:", error)
    updateOutput(`❌ Erreur lors de la génération du code:\n${error.message}`, "error")
  }
}

// Execute code with interpreter
async function executeWithInterpreter(code) {
  try {
    updateOutput("⚡ Exécution en cours...", "info")
    const result = await window.ziaInterpreter.execute(code)
    updateOutput(result, "success")
  } catch (error) {
    updateOutput(`❌ Erreur d'exécution:\n${error.message}`, "error")
  }
}

// Update output area with styled content
function updateOutput(content, type = "info") {
  const output = document.getElementById("output")
  const timestamp = new Date().toLocaleTimeString()

  let icon = "💻"
  let color = "var(--text-primary)"

  switch (type) {
    case "success":
      icon = "✅"
      color = "var(--success-color)"
      break
    case "warning":
      icon = "⚠️"
      color = "var(--warning-color)"
      break
    case "error":
      icon = "❌"
      color = "var(--error-color)"
      break
    case "info":
      icon = "💻"
      color = "var(--accent-color)"
      break
  }

  output.innerHTML = `
    <div style="color: ${color}; margin-bottom: 0.5rem; font-weight: 600;">
      ${icon} [${timestamp}]
    </div>
    <div style="color: var(--text-primary); line-height: 1.6;">
      ${content}
    </div>
  `

  // Auto-scroll to bottom
  output.scrollTop = output.scrollHeight
}

// Show code preview
function showCodePreview(code) {
  const codePreview = document.getElementById("codePreview")
  const generatedCode = document.getElementById("generatedCode")

  // Apply syntax highlighting
  const highlightedCode = highlightZiaCode(code)
  generatedCode.innerHTML = highlightedCode

  codePreview.style.display = "flex"
}

// Simple syntax highlighting for Zia code
function highlightZiaCode(code) {
  return code
    .replace(
      /\b(var|fonction|si|sinon|pour|tantque|selon|cas|defaut|afficher|demander|attendre|retourner|quitter|continuer)\b/g,
      '<span class="keyword">$1</span>',
    )
    .replace(/"([^"]*)"/g, '<span class="string">"$1"</span>')
    .replace(/\b(\d+\.?\d*)\b/g, '<span class="number">$1</span>')
    .replace(/\/\/.*$/gm, '<span class="comment">$&</span>')
}

// Generate code (same as execute for now)
function generateCode() {
  executeCode()
}

// Clear workspace
function clearWorkspace() {
  if (confirm("Êtes-vous sûr de vouloir effacer tout le workspace ?")) {
    workspace.clear()
    updateOutput("🗑️ Workspace effacé. Commencez un nouveau programme!", "info")
    console.log("🗑️ Workspace cleared")
  }
}

// Clear output
function clearOutput() {
  document.getElementById("output").innerHTML = ""
}

// Copy output to clipboard
function copyOutput() {
  const output = document.getElementById("output")
  navigator.clipboard.writeText(output.textContent).then(() => {
    updateOutput("📋 Sortie copiée dans le presse-papiers!", "success")
  })
}

// Copy code to clipboard
function copyCode() {
  const code = document.getElementById("generatedCode")
  navigator.clipboard.writeText(code.textContent).then(() => {
    updateOutput("📋 Code copié dans le presse-papiers!", "success")
  })
}

// Save project
function saveProject() {
  const xml = Blockly.Xml.workspaceToDom(workspace)
  const xmlText = Blockly.Xml.domToText(xml)

  const blob = new Blob([xmlText], { type: "application/xml" })
  const url = URL.createObjectURL(blob)

  const a = document.createElement("a")
  a.href = url
  a.download = "zia-junior-project.xml"
  a.click()

  URL.revokeObjectURL(url)
  updateOutput("💾 Projet sauvegardé!", "success")
}

// Load project
function loadProject() {
  const input = document.createElement("input")
  input.type = "file"
  input.accept = ".xml"

  input.onchange = (e) => {
    const file = e.target.files[0]
    if (file) {
      const reader = new FileReader()
      reader.onload = (e) => {
        try {
          const xml = Blockly.Xml.textToDom(e.target.result)
          workspace.clear()
          Blockly.Xml.domToWorkspace(xml, workspace)
          updateOutput("📁 Projet chargé avec succès!", "success")
        } catch (error) {
          updateOutput("❌ Erreur lors du chargement du projet", "error")
        }
      }
      reader.readAsText(file)
    }
  }

  input.click()
}

// Theme toggle
function toggleTheme() {
  const body = document.body
  const isDark = body.getAttribute("data-theme") === "dark"

  currentTheme = isDark ? "light" : "dark"
  body.setAttribute("data-theme", currentTheme)

  // Save current workspace state
  let xml = null
  if (workspace) {
    xml = Blockly.Xml.workspaceToDom(workspace)
    workspace.dispose()
  }

  // Reinitialize Blockly with new theme
  initBlockly()

  // Restore workspace state
  if (xml) {
    Blockly.Xml.domToWorkspace(xml, workspace)
  }

  updateOutput(`🎨 Thème changé vers ${currentTheme === "dark" ? "sombre" : "clair"}`, "info")
}

// Initialize the application when the page loads
document.addEventListener("DOMContentLoaded", () => {
  initBlockly()

  // Set up event listeners
  document.getElementById("darkMode").addEventListener("change", toggleTheme)

  document.getElementById("showCode").addEventListener("change", (e) => {
    const codePreview = document.getElementById("codePreview")
    codePreview.style.display = e.target.checked ? "flex" : "none"
  })

  document.getElementById("autoExecute").addEventListener("change", (e) => {
    if (e.target.checked) {
      workspace.addChangeListener(executeCode)
    } else {
      workspace.removeChangeListener(executeCode)
    }
  })

  // Load the hello example by default
  setTimeout(() => {
    loadExample("hello")
  }, 500)

  // Keyboard shortcuts
  document.addEventListener("keydown", (e) => {
    if (e.ctrlKey || e.metaKey) {
      switch (e.key) {
        case "Enter":
          e.preventDefault()
          executeCode()
          break
        case "g":
          e.preventDefault()
          generateCode()
          break
        case "Delete":
          e.preventDefault()
          clearWorkspace()
          break
        case "s":
          e.preventDefault()
          saveProject()
          break
        case "o":
          e.preventDefault()
          loadProject()
          break
      }
    }
    // Quick execute with just 'g' key
    if (e.key === "g" && !e.ctrlKey && !e.metaKey) {
      executeCode()
    }
  })

  // Handle window resize
  window.addEventListener("resize", () => {
    Blockly.svgResize(workspace)
  })

  // Initial message
  updateOutput(
    `🎉 Bienvenue dans Zia Junior!

Commencez par faire glisser des blocs depuis la boîte à outils vers l'espace de travail.

Un exemple "Dire Bonjour" a été chargé pour vous aider à commencer.

Raccourcis clavier:
• G: Exécuter le code
• Ctrl+S: Sauvegarder
• Ctrl+O: Charger
• Ctrl+Delete: Effacer tout`,
    "info",
  )

  console.log("🚀 Zia Junior ready!")
})
